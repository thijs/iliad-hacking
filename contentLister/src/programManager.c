/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file programManager.c
 * \brief content lister - eReader program manager
 * 
 * <File description>
 * 
 * Manages the programs running on the eReader
 * spawns processes and sends keys
 */
 
/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <liberregxml/erregapi.h>
#include <liberipc/eripctoolbar.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "erbusy.h"
#include "displayUpdate.h"
#include "programManager.h"
#include "regValue.h"
#include "button.h"
#include "erConnect.h"
#include "erMSDisk.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "toolbar.h"
#include "languages.h"
#include "timedids.h"

static GArray *g_xClientInfo = NULL;
static Window g_myWin;
static GHashTable *g_programInfo = NULL;
static GHashTable *g_extensionInfo = NULL;
static gchar g_uaOnTop[UAID_MAX_SIZE];
static gchar g_lastStartedUa[UAID_MAX_SIZE];

//internally used routines
static int pm_XErrorHandler(Display *display, XErrorEvent *event);
static int pm_UpdateState();
static gboolean pm_InitProgramTable(void);
static void pm_InitExtTable(void);
static int pm_GetClientList(Display * display, Window window, Atom atom, Window ** windowList);
//static unsigned long pm_CheckWindowState(Display * dpy, Window window);
static void pm_PatchProgramTable();
static void pm_UpdateXClientReference(gpointer key, gpointer value, gpointer user_data);
static void pm_IconifyUserApp(gpointer key, gpointer value, gpointer user_data);
static void pm_TerminateUserApp(gpointer key, gpointer value, gpointer user_data);
static void pm_AddExtensionsToTable(gpointer key, gpointer value, gpointer user_data);
static GString *pm_CreateCommand(char *szExec, char *szArgMask, char *filename, char *manifest, char* options);

static Time pm_fake_timestamp();
static int pm_getWinOnTop();
static void pm_setLastStartedUa(gchar * uaID);

void    pt_key_destroy(gpointer key);
void    pt_value_destroy(gpointer value);

void    et_value_destroy(gpointer value);

// GTK-safe handling of Unix signals
static void     signals_init(void);
static void     on_signal(int signal);
static gboolean on_signal_gtk(GIOChannel* source, GIOCondition cond, gpointer p);
static void     on_sigchld(int signo, pid_t pid, int status);

//only used for debug reasons
static void pm_DisplayXClientInfo(GArray * clientInfo);
//static void pm_DisplayProgramInfo(GHashTable * g_programInfo);
//static void pm_DisplayAppInfo(gpointer key, gpointer value, gpointer user_data);
//static void pm_DisplayExtensionInfo(GHashTable * g_extensionInfo);
//static void pm_DisplayExtInfo(gpointer key, gpointer value, gpointer user_data);


//create and  init the global data used by the program manager module
gboolean pm_InitProgramManager(void)
{
    gboolean returnValue = TRUE;

    CL_PMPRINTF("entry");

    // Install Unix signal handlers
    signals_init();

    strncpy(g_uaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE);
    strncpy(g_lastStartedUa, CONTENTLISTER_UAID, UAID_MAX_SIZE);
    g_myWin = -1; // Set this to the correct value when available with pm_SetMyWindow()
    g_xClientInfo = g_array_sized_new(FALSE, TRUE, sizeof(erXClient_t), XCLIENT_ARRAY_RESERVED_SIZE);

    returnValue = pm_InitProgramTable();
    if (returnValue)
    {
        pm_InitExtTable();
    }

    // install our own handler for X errors
    XSetErrorHandler(pm_XErrorHandler);

    return returnValue;
}

static int pm_XErrorHandler(Display *display, XErrorEvent *event)
{
    char buf[1000];

    XGetErrorText(display, event->error_code, buf, sizeof(buf));
    CL_ERRORPRINTF("X error: %s", buf);

    erbusy_off();

    return 0;  // TODO: what to return here, zero seems to work fine
}

void pm_SetMyWindow(Window window)
{
    g_myWin = window;
}

void pm_DestroyProgramManager(void)
{
    CL_PMPRINTF("entry");
    signal(SIGCHLD, SIG_IGN);

    // Do NOT destroy data, these are used even after pm_DestroyProgramManager()
    // Let's rely on the OS to free all memory when the contentLister exits.
    // g_hash_table_destroy(g_extensionInfo);
    // g_hash_table_destroy(g_programInfo);
    // g_array_free(g_xClientInfo, TRUE);
}

static void on_sigchld(int signo, pid_t pid, int status)
{
    listerState_t lister_state = getListerState();

    CL_WARNPRINTF("entry: signo [%d] pid [%d] status [0x%04X]", signo, pid, status);

    if (pid == erConnectPid())
    {
        // connection manager has exited -> did it terminate normally?
        if (WIFEXITED(status))
        {
            CL_WARNPRINTF("ConnectionMgr terminated with return value %d", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            CL_WARNPRINTF("ConnectionMgr terminated by signal [%d]", WTERMSIG(status));
        }

        // it's possible downloadMgr changes pincode
        ctrl_pincode_setting_init();

        // check if this is a timed-iDS connection
        timed_ids_handle_connmgr_quit();
    }
    else
    {
        // Note: Searching hash table on g_lastStartedUa only works when at most one viewer/application running.
        //       When contentLister keeps two or more viewers/applications alive, search the hash table on pid.
        erApplicationInfo_t *theAppInfo = (erApplicationInfo_t *) g_hash_table_lookup(g_programInfo, g_lastStartedUa);
        if (theAppInfo)
        {
            CL_WARNPRINTF("last started [%s] pid [%d]", g_lastStartedUa, theAppInfo->pid);
            
            // check which process has died and free resources
            if (theAppInfo->pid == pid) 
            {
                // Remember application not running anymore;
                theAppInfo->pid = -1;
                if (lister_state != STATE_DOWNLOADMGR)
                {
                    pm_setUaOnTop(CONTENTLISTER_UAID);
                }

                // restore toolbar to contentLister layout
                toolbar_restore();

                // Hack to rescan the language etc. when SETUP app finishes
                if (strncmp(g_lastStartedUa, SETUP_APP, UAID_MAX_SIZE) == 0)
                {
                    CL_PMPRINTF("Setup app was on top, do languagesInit() and mdsInit()");

                    // reload registry data
                    reload_registry();

                    // 
                    languagesInit();
                    mdsInit();
                    flipbarDirectionSettingInit();
                    keybufferingSettingInit();
                    ctrlSetScreenTexts();
                    ctrl_pincode_setting_init();
                    if (getListerState() == STATE_LANG_SEL)
                    {
                        CL_PMPRINTF("Exiting setup for language selection.");
                        setListerState(STATE_NORMAL);
                        ctrl_startup();
                    }
                    // (re)start background-connect
                    ctrl_background_connect_timeout_start(TRUE);

                    // allow USB connect again
                    erMSDisk_handle_reread_settings();
                    
                    // (re)start timed-iDS connect
                    timed_ids_handle_reread_settings();
                }

                // allow USB connect again
                if (   lister_state != STATE_PINCODE
                    && lister_state != STATE_DOWNLOADMGR)
                {
                    erStartMSDiskApp();
                }
                
                // rescan the current folder
                if (getListerState() != STATE_USB_CONNECTED)
                {
                    ctrl_rescan_current();
                }
            }
        }
    }
    
    CL_WARNPRINTF("end");
}

char   *pm_getUaID(char *extension)
{
    erExtInfo_t *theExtInfo = NULL;

    CL_PMPRINTF("entry");

    if (extension)
    {
        theExtInfo = g_hash_table_lookup(g_extensionInfo, extension);

        if (theExtInfo)
        {
            CL_PMPRINTF("uaID %s - extension %s", (theExtInfo->uaID == NULL) ? "NULL" : theExtInfo->uaID, extension);
            return theExtInfo->uaID;
        }
    }
    return NULL;
}

char   *pm_getIcon(char *extension)
{
    erExtInfo_t *theExtInfo = NULL;

    if (extension)
    {
        theExtInfo = g_hash_table_lookup(g_extensionInfo, extension);

        if (theExtInfo)
        {
            CL_PMPRINTF("iconLocation %s - extension %s",
                        (theExtInfo->iconLocation == NULL) ? "NULL" : theExtInfo->iconLocation, extension);
            return theExtInfo->iconLocation;
        }
    }

    return NULL;
}

gboolean pm_RaiseContentLister()
{
    Window  win = 0;
    Display *display;
    int     nmbrOfXClients;

    CL_PMPRINTF("entry");

    //Update the view on XClients
    nmbrOfXClients = pm_UpdateState();
    if (nmbrOfXClients < 0)
    {
        CL_ERRORPRINTF("Failed to update state");
        return FALSE;
    }

    // Patch the program table with the info retrieved on the XClients
    pm_PatchProgramTable();

    // The contentLister window
    win = g_myWin;
    display = gdk_x11_display_get_xdisplay(gdk_display_get_default());

    if (strcmp(pm_getUaOnTop(), CONTENTLISTER_UAID) != 0)
    {
        // minimize current application
        erApplicationInfo_t *theAppInfo= (erApplicationInfo_t *) 
            g_hash_table_lookup(g_programInfo, pm_getUaOnTop());
        pm_IconifyUserApp(NULL, theAppInfo, display);
        toolbar_restore();
    }

    // stop the user application that was on top,
    // in fact stop all user applications and viewers
    g_hash_table_foreach(g_programInfo, pm_TerminateUserApp, display);

    XRaiseWindow(display, win);
    XMapRaised(display, win);
    XFlush(display);
    pm_setUaOnTop(CONTENTLISTER_UAID);

    CL_PMPRINTF("end");
    return TRUE;
}

gboolean pm_RaiseUserApp(gchar * uaId)
{
    Window  win = 0;
    Window  CL_win = 0;
    Display *display;
    int     nmbrOfXClients;
    int     index;
    erApplicationInfo_t *theAppInfo;
    int     screen;
    erXClient_t *client;

    CL_PMPRINTF("entry: uaId [%s]", uaId);

    theAppInfo = (erApplicationInfo_t *) g_hash_table_lookup(g_programInfo, uaId);

    if (theAppInfo)
    {
        //Update the view on XClients
        nmbrOfXClients = pm_UpdateState();
        if (nmbrOfXClients < 0)
        {
            CL_ERRORPRINTF("Failed to update state");
            return FALSE;
        }

        // Patch the program table with the info retrieved on the XClients
        pm_PatchProgramTable();

        // find viewer and contentlister window
        for (index = 0; index < g_xClientInfo->len; index++)
        {
            client = &g_array_index(g_xClientInfo, erXClient_t, index);

            if (strcmp(CONTENTLISTER_RESNAME, client->resName) == 0)
            {
                CL_win = client->win;
            }
            if (strcmp(theAppInfo->userApp->xResName, client->resName) == 0)
            {
                win = client->win;
            }
        }

        if ((win != 0) && (CL_win != 0))
        {
            screen = gdk_x11_get_default_screen();
            display = gdk_x11_display_get_xdisplay(gdk_display_get_default());

            CL_PMPRINTF("Lower CL window");
            XIconifyWindow(display, CL_win, screen);
            XRaiseWindow(display, win);
            XMapRaised(display, win);
            XFlush(display);
            pm_setUaOnTop(uaId);
            toolbar_clear();
            return TRUE;
        }
        else
        {
            CL_ERRORPRINTF("could not find window info");
            return FALSE;
        }
    }
    else
    {
        CL_ERRORPRINTF("invalid uaID %s", (NULL == uaId) ? "NULL" : uaId);
        return FALSE;
    }
}

gboolean pm_IsActive(gchar * uaId)
{
    int     nmbrOfXClients;
    erApplicationInfo_t *theAppInfo;

    CL_PMPRINTF("entry %s", (uaId == NULL) ? "NULL" : uaId);

    theAppInfo = (erApplicationInfo_t *) g_hash_table_lookup(g_programInfo, uaId);

    if (theAppInfo)
    {
        //Update the view on XClients
        nmbrOfXClients = pm_UpdateState();
        if (nmbrOfXClients < 0)
        {
            CL_ERRORPRINTF("Failed to update state");
            return FALSE;
        }

        // Patch the program table with the info retrieved on the XClients
        pm_PatchProgramTable();

        if (NULL != theAppInfo->xclient)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        CL_ERRORPRINTF("invalid uaID %s", (NULL == uaId) ? "NULL" : uaId);
        return FALSE;
    }
}

void pm_SendKey(KeySym keysym)
{
    XKeyEvent xev;
    Display *display;
    Window  window, root;
    int     screen;

    display = gdk_x11_display_get_xdisplay(gdk_display_get_default());
    root = gdk_x11_get_default_root_xwindow();
    window = pm_getWinOnTop();
    if (window == -1)
    {
        CL_ERRORPRINTF("Failed: no valid window found");
        return;
    }

    if (window == g_myWin)
    {
        CL_PMPRINTF("ContentLister window 0x%x", (unsigned int)window);

        display = XOpenDisplay(NULL);
        screen = DefaultScreen(display);
        root = RootWindow(display, screen);
    }

    // create and send a keypress event followed by a keyrelease event 
    xev.type = KeyPress;
    xev.display = display;
    xev.root = root;
    xev.window = window;
    xev.subwindow = None;
    xev.time = pm_fake_timestamp();
    xev.x = 1;
    xev.y = 1;
    xev.x_root = 1;
    xev.y_root = 1;
    xev.same_screen = 1;
    xev.state = 0;
    xev.keycode = XKeysymToKeycode(display, keysym);
//    CL_ERRORPRINTF("Sending keypress....");
    XSendEvent(xev.display, window, 1, KeyPressMask, (XEvent *) & xev);

    // We check to see if this is the UP key
    // so we don't send the release event for that, because it
    // can crash the contentLister with a BadWindow error (req.code 25)
    if ( keysym == GDK_F5 )
    {
        CL_ERRORPRINTF("NOT Sending keyrelease for UP key....");
    }
    else
    {
//        usleep(2000); // MvdW: this usleep doesn't seem to be nescessary
        xev.type = KeyRelease;
        xev.time = pm_fake_timestamp();
        XSendEvent(xev.display, window, 1, KeyReleaseMask, (XEvent *) & xev);
    }

    //XSync(xev.display, 1);
    XSync(xev.display, FALSE);

    if (window == g_myWin)
    {
        CL_PMPRINTF("Before XCloseDisplay()");
        XCloseDisplay(display);
    }
}

gchar  *pm_getUaOnTop()
{
    CL_PMPRINTF("return UaID [%s]", g_uaOnTop);
    return g_uaOnTop;
}

static Time pm_fake_timestamp()
{
    int     tint;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    tint = (int) tv.tv_sec * 1000;
    tint = tint / 1000 * 1000;
    tint = tint + tv.tv_usec / 1000;
    return (Time) tint;
}


static void pm_IconifyUserApp(gpointer key, gpointer value, gpointer user_data)
{
    erApplicationInfo_t *theAppInfo = (erApplicationInfo_t *) value;
    Display *display = (Display *) user_data;

    CL_PMPRINTF("entry");

    if ((theAppInfo) && (user_data))
    {
        int     screen = gdk_x11_get_default_screen();

        if (theAppInfo->xclient != NULL)
        {
            CL_PMPRINTF("Lower window %s", theAppInfo->userApp->xResName);
            XIconifyWindow(display, theAppInfo->xclient->win, screen);
        }
    }
}


static void pm_TerminateUserApp(gpointer key, gpointer value, gpointer user_data)
{
    int i;
    pid_t pid;
    erApplicationInfo_t *theAppInfo = (erApplicationInfo_t *) value;

    CL_PMPRINTF("entry: xResName [%s]", theAppInfo->userApp->xResName);

    if ((theAppInfo) && (user_data))
    {
        if (theAppInfo->pid != -1)
        {
            // stop application
            pid = theAppInfo->pid;
            CL_WARNPRINTF("Terminate process [%s] pid [%d]", theAppInfo->userApp->xResName, pid);
            kill(pid, SIGTERM);
            
            // wait for application to die
            i = 300;
            while (kill(pid, 0) == 0  &&  i > 0)
            {
                // process still present: wait for application to die
                usleep(100*1000);
                i--;
            }

            if (i == 0)
            {
                // oops, application is a die-hard: use brute force
                kill(pid, SIGKILL);
                i = 100;
                while (kill(pid, 0) == 0  &&  i > 0)
                {
                    // wait for application to die
                    usleep(100*1000);
                    i--;
                }

                if (i == 0)
                {
                    // application won't die: declare it dead
                    theAppInfo->pid = -1;
                }
            }
        }
    }
    CL_PMPRINTF("end");
}

void pm_setUaOnTop(gchar * uaID)
{
    CL_PMPRINTF("entry: uaID [%s]", uaID);
    strncpy(g_uaOnTop, uaID, UAID_MAX_SIZE);
}

void pm_setLastStartedUa(gchar * uaID)
{
    CL_PMPRINTF("entry: uaID [%s]", uaID);
    strncpy(g_lastStartedUa, uaID, UAID_MAX_SIZE);
}

// Returns the window identifier of the user application currently on top of the window stack.
// returns -1 in case of failure
static int pm_getWinOnTop()
{
    int     nmbrOfXClients;
    char   *uaId;
    Window  win = -1;
    erApplicationInfo_t *theAppInfo;

    CL_PMPRINTF("entry");

    uaId = pm_getUaOnTop();

    if (strncmp(uaId, CONTENTLISTER_UAID, UAID_MAX_SIZE) == 0)
    {
        // My own window 
        win = g_myWin;
    }
    else
    {
        // Update the view on XClients
        nmbrOfXClients = pm_UpdateState();
        if (nmbrOfXClients < 0)
        {
            CL_ERRORPRINTF("Failed to update state");
        }
        else
        {
            // Patch the program table with the info retrieved on the XClients
            pm_PatchProgramTable();
            
            theAppInfo = (erApplicationInfo_t *) g_hash_table_lookup(g_programInfo, uaId);
            if (theAppInfo != NULL  &&  theAppInfo->xclient != NULL)
            {
                // application info found and xclient present -> this is the one
                win = theAppInfo->xclient->win;
                CL_PMPRINTF("window with id (0x%x)", (unsigned int)win);
            }
            else
            {
                // app-info or xclient problem -> error
                CL_ERRORPRINTF("theAppInfo [%p]", theAppInfo);
                if (theAppInfo)
                {
                    CL_ERRORPRINTF("theAppInfo->xclient [%p]", theAppInfo->xclient);
                }
                return -1;
            }
        }
    }
    return win;
}

int pm_RunViewer(gchar * uaID, char *filename, char *manifest, char *options, int reuse)
{
    pid_t   pid;
    int     nmbrOfXClients;
    erApplicationInfo_t *theAppInfo;
    Display *display;
    Window  ownWin = 0;
    int     index;
    erXClient_t *client;
    int     screen;
    GString *theCommand;

    theAppInfo = (erApplicationInfo_t *) g_hash_table_lookup(g_programInfo, (const char*)uaID);

    if (theAppInfo)
    {
        CL_PMPRINTF("App %s", theAppInfo->uaID);

        toolbar_clear();

        //Update the view on XClients
        nmbrOfXClients = pm_UpdateState();
        if (nmbrOfXClients < 0)
        {
            CL_ERRORPRINTF("Failed to update state");
            return (ERR_STATE_UPDATE);
        }

        // Patch the program table with the info retrieved on the XClients
        pm_PatchProgramTable();

        if (reuse == 1)
        {
            CL_PMPRINTF("Re-use process branch");

            // Is there already a process running?
            if (theAppInfo->pid != -1)
            {
                // TODO: Here I need to bring the selected app on top of the stack
                // and inform it on the new content to display
                CL_WARNPRINTF("TODO: Bring %s on top of window stack", uaID);

                if (theAppInfo->xclient != NULL)
                {
                    CL_PMPRINTF("Do something with %s 0x%xd",
                                theAppInfo->xclient->resName,
                                (unsigned int)theAppInfo->xclient->win);

                    CL_PMPRINTF("XRaiseWindow: 0x%x (window)", (unsigned int)theAppInfo->xclient->win);

                    display = gdk_x11_display_get_xdisplay(gdk_display_get_default());

                    // Iconize contentLister
                    for (index = 0; index < g_xClientInfo->len; index++)
                    {
                        client = &g_array_index(g_xClientInfo, erXClient_t, index);

                        if (strcmp(CONTENTLISTER_RESNAME, client->resName) == 0)
                        {
                            ownWin = client->win;
                            break;
                        }
                    }

                    if (ownWin != 0)
                    {
                        screen = gdk_x11_get_default_screen();

                        CL_PMPRINTF("Lower window %s", client->resName);
                        XIconifyWindow(display, ownWin, screen);
                    }

                    XMapRaised(display, theAppInfo->xclient->win);
                    XRaiseWindow(display, theAppInfo->xclient->win);
                    XFlush(display);
                    pm_setUaOnTop(uaID);

                    // done
                    return (ERR_OK);
                }
                else
                {
                    CL_ERRORPRINTF("Did not find xclient info on %s. Should I kill the process here????", uaID);
                }
            }
            // The else branch is implicit. Just create the process
        }
        else
        {
            CL_PMPRINTF("Create new process branch");

            // Is there already a process running?
            if (theAppInfo->pid != -1)
            {
                CL_WARNPRINTF("Process was already started, not starting it again!");
                return (ERR_VIEWER_RUNNING);
            }
            // The else branch is implicit. Just create the process
        }

        //handle the argument information
        theCommand = pm_CreateCommand(theAppInfo->userApp->szExec, theAppInfo->userApp->szArgMask, filename, manifest, options);

        if (theCommand)
        {
            CL_PMPRINTF("szCommand %s", theCommand->str);
        }
        else
        {
            CL_ERRORPRINTF("pm_CreateArgument can't create command");
            return (ERR_ARGUMENT);
        }

        // Check if ARINC viewer, this is a special case
        if (strcmp(uaID, ARINC_APP) == 0)
        {
            // For Arinc application, exec it, so that contentLister will be
            // replaced by eFlyBook

            // Make sure the executable exists; not only the symlink. Otherwise the iLiad will crash.
            char *p = realpath(theAppInfo->userApp->szExec, NULL); // p will be allocated by realpath
            if (p == NULL) 
            {
                CL_WARNPRINTF("Cannot find executable [%s]; not executing in place.", theAppInfo->userApp->szExec);
                return (ERR_FORK);
            }
            else
            {
                g_free(p); // free realpath allocated buffer
                int     nRet;
                CL_WARNPRINTF("Executing [%s] in place; replaces contentLister!", uaID);    
                nRet = shell_exec(theCommand->str);
                CL_ERRORPRINTF("shell_exec of [%s] failed!", uaID);
                CL_ERRORPRINTF("shell exec command [%s]", theCommand->str);
                CL_ERRORPRINTF("shell_exec returned %d", nRet);
                perror("Error upon calling shell_exec");
                fflush(stderr);
                return (ERR_FORK);
            }
        }
        else
        {
            erStopMSDiskApp();

            switch (pid = fork())
            {
                case 0:
                {
                    // Here I am in the child process
                    int     nRet;

                    nRet = shell_exec(theCommand->str);
                    CL_ERRORPRINTF("shell_exec of [%s] failed, cleaning up child", uaID);
                    CL_ERRORPRINTF("shell exec command [%s]", theCommand->str);
                    CL_ERRORPRINTF("shell_exec returned %d", nRet);
                    perror("Error upon calling shell_exec");
                    fflush(stderr);
                    sleep(1);
                    _exit(1);
                }
                case -1:
                    // something went wrong with fork => this is the parent process
                    CL_ERRORPRINTF("pmRunViewer: can't fork %s", uaID);
                    g_string_free(theCommand, TRUE);
                    erStartMSDiskApp();
                    return (ERR_FORK);
            }
        }

        // Child process started with succes => here the parent process continues
        // Store the pid
        theAppInfo->pid = pid;
        pm_setUaOnTop(uaID);
        pm_setLastStartedUa(uaID);
        g_string_free(theCommand, TRUE);
        return (ERR_OK);
    }
    else
    {
        CL_WARNPRINTF("invalid uaID %s", (NULL == uaID) ? "NULL" : uaID);
        return (ERR_INVALID_UAID);
    }

}

static GString *pm_CreateCommand(char *szExec, char *szArgMask, char *filename, char *manifest, char *options)
{
    GString *theCommand = NULL;
    char   *file_ptr;
    char   *manifest_ptr;
    int     location = 0;

    if ((szExec) && (szArgMask))
    {
        theCommand = g_string_sized_new(COMMAND_MAX_SIZE);

        if (theCommand)
        {
            g_string_printf(theCommand, "%s", szExec);
            g_string_append_printf(theCommand, " ");
            g_string_append_printf(theCommand, "%s", szArgMask);

            CL_PMPRINTF("theCommand %s", theCommand->str);

            file_ptr = g_strstr_len(theCommand->str, theCommand->len, FILENAME_ARG);

            if (file_ptr && filename)
            {
                location = file_ptr - theCommand->str;

                CL_PMPRINTF("%s location %d", FILENAME_ARG, location);

                g_string_erase(theCommand, location, strlen(FILENAME_ARG));
                g_string_insert(theCommand, location, "\"\"");
                g_string_insert(theCommand, location + 1, filename);

                CL_PMPRINTF("inserted filename => %s", theCommand->str);
            }
            else
            {
                CL_PMPRINTF("file_ptr %s - filename %s", (file_ptr == NULL) ? "NULL" : file_ptr,
                            (filename == NULL) ? "NULL" : filename);
            }

            manifest_ptr = g_strstr_len(theCommand->str, theCommand->len, MANIFEST_ARG);
            if (manifest_ptr && manifest)
            {
                location = manifest_ptr - theCommand->str;

                CL_PMPRINTF("%s location %d", MANIFEST_ARG, location);

                g_string_erase(theCommand, location, strlen(FILENAME_ARG));
                g_string_insert(theCommand, location, "\"\"");
                g_string_insert(theCommand, location + 1, manifest);

                CL_PMPRINTF("inserted manifest => %s", theCommand->str);
            }
            else
            {
                CL_PMPRINTF("manifest_ptr %s - manifest %s", (manifest_ptr == NULL) ? "NULL" : manifest_ptr,
                            (filename == NULL) ? "NULL" : manifest);
            }

            if (options)
            {
                g_string_append_printf(theCommand, " ");
                g_string_append_printf(theCommand, "%s", options);
                CL_PMPRINTF("inserted extra options => %s", theCommand->str);
            }
        }                       // theCommand
    }
    return theCommand;
}

//Retrieve the current xClient situation and store the list
//of available windows and their state in the clientInfo Array
//returns the window count of -1 in case of an error
static int pm_UpdateState()
{
    Display *display;
    Window  rootWindow;
    Atom    net_client_list;
    int     nWindows;
    Window *windowList;
    int     i;
    erXClient_t xClient;
    
    GArray *clientInfo = g_xClientInfo;

    CL_PMPRINTF("entry");

    display = gdk_x11_display_get_xdisplay(gdk_display_get_default());
    XSync(display, FALSE);

    net_client_list = XInternAtom(display, "_NET_CLIENT_LIST", False);
    rootWindow = gdk_x11_get_default_root_xwindow();
    nWindows = pm_GetClientList(display, rootWindow, net_client_list, &windowList);
    if (nWindows < 0)
    {
        CL_ERRORPRINTF("Failed to retrieve the number of clients");
        return -1;
    }

    // Clear the previously stored window information.
    if (clientInfo->len > 0)
    {
        clientInfo = g_array_remove_range(clientInfo, 0, clientInfo->len);
    }

    for (i = 0; i < nWindows; i++)
    {
        XClassHint classHints;

        CL_PMPRINTF("windowList[%d]: 0x%lx", i, windowList[i]);

        memset(&xClient, 0, sizeof(erXClient_t));

        // Store the window identifier
        xClient.win = windowList[i];

        // Fetch and Store the 'resource name'
        if (XGetClassHint(display, windowList[i], &classHints))
        {
            CL_PMPRINTF("Res Name: %s - Res Class: %s.", classHints.res_name, classHints.res_class);
            strncpy(xClient.resName, classHints.res_name, WM_NAME_MAX - 1);
            XFree(classHints.res_name);
            XFree(classHints.res_class);
        }
        else
        {
            CL_ERRORPRINTF("Failed to fetch class hint for window 0x%lx", windowList[i]);
        }

        // Fetch and Store the Window State
        //xClient.winState = pm_CheckWindowState(display, windowList[i]);

        //store the client information
        g_array_append_val(clientInfo, xClient);
    }

    //pm_DisplayXClientInfo(clientInfo);
    
    g_xClientInfo = clientInfo;

    // Free the windowList
    XFree(windowList);
    return nWindows;
}

//only used for debug reasons
static void pm_DisplayXClientInfo(GArray * clientInfo)
{
    int     index;
    erXClient_t *client;

    CL_PMPRINTF("entry");

    for (index = 0; index < clientInfo->len; index++)
    {
        client = &g_array_index(clientInfo, erXClient_t, index);

        CL_PMPRINTF("");
        CL_PMPRINTF("clientInfo[%d] address 0x%x", index, (unsigned int)client);
        CL_PMPRINTF("resName %s", client->resName);
        CL_PMPRINTF("window id 0x%x", (unsigned int)client->win);
//        CL_PMPRINTF("state %ld", client->winState);
    }
}

//  Returns number of clients. -1 upon error
static int pm_GetClientList(Display * display, Window window, Atom atom, Window ** windowList)
{
    unsigned char *data;
    int     real_format;
    Atom    real_type;
    unsigned long itemsRead, itemsLeft;

    CL_PMPRINTF("entry");

    if (XGetWindowProperty(display, window, atom, 0, 0x7fffffff,
                           False, XA_WINDOW, &real_type, &real_format, &itemsRead, &itemsLeft,
                           (unsigned char **) &data) == Success && itemsRead >= 1)
    {

        CL_PMPRINTF("%ld top level windows", itemsRead);
        *windowList = (Window *) data;
        return itemsRead;
    }
    return -1;
}

// Init the hash table with the program information 
// which is retrieved from the registry.
//
// NOTE : theApplicationInfo structure is only created when "theUserApp" registry
//        info is available. So userApp is never NULL in the erApplicationInfo_t 
//        structure in the program table.  
//
static gboolean pm_InitProgramTable(void)
{
    gboolean returnValue = TRUE;
    regUserApp_t *theUserApp = NULL;
    regUserAppList_t *theUserAppList = NULL;
    erApplicationInfo_t *theApplicationInfo = NULL;
    char   *key = NULL;
    int     i;
    struct stat file_stat;

    CL_PMPRINTF("entry");

    theUserAppList = erRegGetUserAppList();
    if (theUserAppList)
    {
        CL_PMPRINTF("erRegReadUserAppList returned %d values", theUserAppList->uaIDArraySize);
        g_programInfo = g_hash_table_new_full(g_str_hash, g_str_equal, pt_key_destroy, pt_value_destroy);
        for (i = 0; i < theUserAppList->uaIDArraySize; i++)
        {
            theUserApp = erRegGetUserApp(theUserAppList->uaIDArray[i]);
            if (theUserApp)
            {
                theApplicationInfo = g_new0(erApplicationInfo_t, 1);

                theApplicationInfo->userApp = theUserApp;
                strncpy(theApplicationInfo->uaID, theUserAppList->uaIDArray[i], UAID_MAX_SIZE);
                theApplicationInfo->pid = -1;
                
                // Check if application executable exists
                if (stat(theApplicationInfo->userApp->szExec, &file_stat) == 0)
                {
                    theApplicationInfo->enabled = TRUE;
                }
                else
                {
                    CL_WARNPRINTF("Application [%s] executable [%s] is not found: disabled it", theApplicationInfo->uaID, theApplicationInfo->userApp->szExec);
                    theApplicationInfo->enabled = FALSE;
                }

                CL_PMPRINTF("g_hash_table_insert %s", theUserAppList->uaIDArray[i]);

                key = g_strdup(theUserAppList->uaIDArray[i]);

                g_hash_table_insert(g_programInfo, key, theApplicationInfo);
            }
        }
        erRegFreeUserAppList(theUserAppList);
        // pm_DisplayProgramInfo(g_programInfo);
    }
    else
    {
        CL_ERRORPRINTF("erRegReadUserAppList returned NULL");
        returnValue = FALSE;
    }
        
    return returnValue;
}

//  Make a hash table to retrieve the uaID associated with an extension
static void pm_InitExtTable(void)
{
    CL_PMPRINTF("entry");

    g_extensionInfo = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, et_value_destroy);

    /// going to load extension information from registry file.
    regExtInfoList_t *theRegExtInfoList = 0;    
    theRegExtInfoList = erRegGetExtInfoList(NULL);
    if (theRegExtInfoList)
    {
        g_hash_table_foreach(g_programInfo, pm_AddExtensionsToTable, theRegExtInfoList);
        erRegFreeExtInfoList(theRegExtInfoList);
    }
    else
    {
        CL_ERRORPRINTF("erRegGetExtInfoList returns NULL");
    }
    // pm_DisplayExtensionInfo(g_extensionInfo);
}

static void pm_AddExtensionsToTable(gpointer key, gpointer value, gpointer user_data)
{
    int     index;
    erApplicationInfo_t *theApplicationInfo = (erApplicationInfo_t *) value;
    regExtInfoList_t *theRegExtInfoList = (regExtInfoList_t *) user_data;
    erExtInfo_t *theExtInfo;

    // Only need to check the applications enabled.
    if ((theApplicationInfo) && (theRegExtInfoList) && (theApplicationInfo->enabled))
    {
        // Need to compare every extension name that input application supports with the 
        // extension list from registry to decide which application is the best one to 
        // open the document with the certain extension name.
        for (index = 0; index < theApplicationInfo->userApp->extensionArraySize; index++)
        {
            theExtInfo = g_new0(erExtInfo_t, 1);

            if (theRegExtInfoList)
            {
                // Choose the smallest associate order with same extension name.
                // It's faster than using xpath.
                int i = 0, order = INT_MAX, pos = -1;
                for(; i < theRegExtInfoList->extArraySize; ++i)
                {
                    // Compare extension from ExtInfoList with those from theApplicationInfo 
                    if ((theRegExtInfoList->extArray[i].extName) && 
                        (strcmp(theApplicationInfo->userApp->extensionArray[index], 
                                theRegExtInfoList->extArray[i].extName) == 0))
                    {                                
                        if (order > theRegExtInfoList->extArray[i].associateOrder)
                        {
                            order = theRegExtInfoList->extArray[i].associateOrder;
                            pos = i;
                        }
                    }
                }
                // If an association is found, use it. Otherwise use the application that referenced this extension.
                if (pos != -1)
                {
                    theExtInfo->uaID = g_strdup(theRegExtInfoList->extArray[pos].associateApp);
                    theExtInfo->iconLocation = g_strdup(theRegExtInfoList->extArray[pos].iconLocation);
                }
                else
                {
                    CL_WARNPRINTF("Program %s associated with [%s] without extensionInfo entry in registry. Consider fixing this!", 
                                theApplicationInfo->uaID, theApplicationInfo->userApp->extensionArray[index]);
                    theExtInfo->uaID = theApplicationInfo->uaID;
                    theExtInfo->iconLocation = NULL;
                }
            }
            else
            {
                theExtInfo->uaID = theApplicationInfo->uaID;
            }

            CL_PMPRINTF("add %s - %s - %s", theApplicationInfo->userApp->extensionArray[index],
                         theApplicationInfo->uaID,
                         (theExtInfo->iconLocation == NULL) ? "NULL" : theExtInfo->iconLocation);

            g_hash_table_insert(g_extensionInfo, theApplicationInfo->userApp->extensionArray[index], theExtInfo);
        }
    }
}

//key in the program hash table is the user application ID
void pt_key_destroy(gpointer key)
{
    if (key)
    {
        CL_PMPRINTF("%s", (char*)key);
        g_free(key);
        key = NULL;
    }
}

// value in the program hash table is the user application information structure 
// the release of the xclient memory is handled by the g_xClientInfo array, since
// the erApplicationInfo_t only stores a reference 
void pt_value_destroy(gpointer value)
{
    erApplicationInfo_t *theApplicationInfo = (erApplicationInfo_t*)value;

    if (theApplicationInfo)
    {
        CL_PMPRINTF("%s", theApplicationInfo->uaID);

        if (theApplicationInfo->userApp)
        {
            erRegFreeUserApp(theApplicationInfo->userApp);
        }

        g_free(theApplicationInfo);
        theApplicationInfo = NULL;
    }
}

// value in the extension hash table is the erExtInfo_t struct 
// the release of the uaID memory is handled by the g_programInfo hash table
void et_value_destroy(gpointer value)
{
    erExtInfo_t *theExtInfo;

    if (value)
    {
        theExtInfo = (erExtInfo_t *) value;

        CL_PMPRINTF("%s", theExtInfo->uaID);

        if (theExtInfo->iconLocation)
        {
            g_free(theExtInfo->iconLocation);
        }

        g_free(theExtInfo);
        theExtInfo = NULL;
    }
}

static void pm_PatchProgramTable()
{
    pm_DisplayXClientInfo(g_xClientInfo);
    g_hash_table_foreach(g_programInfo, pm_UpdateXClientReference, g_xClientInfo);
}

static void pm_UpdateXClientReference(gpointer key, gpointer value, gpointer user_data)
{
    int     index;
    erXClient_t *client;
    erApplicationInfo_t *theApplicationInfo = (erApplicationInfo_t *) value;
    GArray *clientInfo = (GArray *) user_data;

    CL_PMPRINTF("entry");

    if ((g_xClientInfo) && (theApplicationInfo))
    {
        // clear the obsolete client info
        theApplicationInfo->xclient = NULL;
        CL_PMPRINTF("App %s", theApplicationInfo->uaID);

        for (index = 0; index < g_xClientInfo->len; index++)
        {
            client = &g_array_index(clientInfo, erXClient_t, index);

            if (strcmp(theApplicationInfo->userApp->xResName, client->resName) == 0)
            {
                CL_PMPRINTF("Updating [%s] with resname [%s] to windowid [0x%x]",
                            theApplicationInfo->uaID,
                            client->resName,
                            (unsigned int)client->win);
                theApplicationInfo->xclient = client;
                break;
            }
        }
    }
    else
    {
        CL_ERRORPRINTF("g_xClientInfo or theApplicationInfo is NULL");
    }
}



/*
 * --------------------------------------------
 * 
 * Implement GTK-safe handling of Unix signals.
 *
 * --------------------------------------------
 */
#define PIPE_READ       0
#define PIPE_WRITE      1

static int signal_pipe[2];

typedef struct
{
    int    signo;   // signal number
    pid_t  pid;     // pid,    valid only with SIGCHLD
    int    status;  // status, valid only with SIGCHLD
} signal_data_t;

// Install unix signal handlers
static void signals_init()
{
    CL_LOGPRINTF("entry");

    int         i;
    long        fd_flags;
    GIOFlags    io_flags;
    GIOChannel* io_channel = NULL;
    GError*     error = NULL;

    // create pipe
    i = pipe(signal_pipe);
    g_assert(i == 0);

    // set write-end of pipe to non-blocking
    fd_flags = fcntl(signal_pipe[PIPE_WRITE], F_GETFL);
    g_assert(fd_flags != -1);
    i = fcntl(signal_pipe[PIPE_WRITE], F_SETFL, fd_flags | O_NONBLOCK);
    g_assert(i == 0);

    // convert read-end of pipe into a GIOChannel
    io_channel = g_io_channel_unix_new(signal_pipe[PIPE_READ]);

    // we only read raw binary data from the pipe, therefore clear any encoding on the channel
    g_io_channel_set_encoding(io_channel, NULL, &error);
    g_assert(error == NULL);

    // set the reading end also to non-blocking mode
    io_flags = g_io_channel_get_flags(io_channel);
    g_io_channel_set_flags(io_channel, io_flags | G_IO_FLAG_NONBLOCK, &error);
    g_assert(error == NULL);

    // register the reading end with the event loop
    g_io_add_watch(io_channel, G_IO_IN | G_IO_PRI, on_signal_gtk, NULL);

    // catch the unix SIGCHLD signal
    struct sigaction on_chld;
    memset(&on_chld, 0x00, sizeof(on_chld));
    on_chld.sa_handler = on_signal;
    on_chld.sa_flags   = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &on_chld, NULL);
}

// unix signal handler: write signal number into pipe
static void on_signal(int signal)
{
    int            bytes_written;
    int            status;
    signal_data_t  data;
    pid_t          pid;
    gboolean       done;
    gboolean       do_write;

    CL_LOGPRINTF("entry: signal [%d]", signal);
    
    done        = FALSE;
    data.signo  = signal;
    data.pid    = 0;
    data.status = 0;
    while (!done)
    {
        do_write = FALSE;
        if (signal == SIGCHLD)
        {
            // SIGCHLD signal: query child process and forward these to GTK thread

            // find out which process has ended
            pid = waitpid(-1, &status, WNOHANG);
            if (pid > 0)
            {
                CL_PMPRINTF("child died: pid [%d] exited [%d] signaled [%d]",
                             pid, WIFEXITED(status), WIFSIGNALED(status) );

                // process status received: check it
                if ( WIFEXITED(status)  ||  WIFSIGNALED(status) )
                {
                    // process has terminated: report to GTK thread
                    data.pid    = pid;
                    data.status = status;
                    do_write = TRUE;

                    // temporarily block all keys
                    // this prevents sending keys to an application that is no longer present
                    button_block_all_keys(TRUE);
                }
            }
            else
            {
                // no more process: stop querying processes
                done = TRUE;
            }
        }
        else
        {
            // other signal: forward this to GTK thread
            done = TRUE;
        }
        
        if (do_write)
        {
            bytes_written = write(signal_pipe[PIPE_WRITE], &data, sizeof(data));
            fsync(signal_pipe[PIPE_WRITE]);  // just to be sure
            if (bytes_written != sizeof(data))
            {
                CL_ERRORPRINTF("unix signal [%d] [%d] lost\n", data.signo, data.pid);
            }
        }
    }
}

// GTK signal handler: read signal from pipe and dispatch
static gboolean on_signal_gtk(GIOChannel* source, GIOCondition cond, gpointer p)
{
    CL_LOGPRINTF("entry");

    signal_data_t data;
    GIOStatus     status;             // save the reading status
    gsize         bytes_read;         // save the number of chars read
    gboolean      done;
    GError*       error = NULL;

    // Read from the pipe as long as data is available.
    // The reading end is in non-blocking mode, so once we have consumed all unix signals, 
    // the read returns G_IO_STATUS_AGAIN. 
    done = FALSE;
    while (done == FALSE)
    {
        status = g_io_channel_read_chars(source, (char*)&data, sizeof(data), &bytes_read, &error);
        if (status == G_IO_STATUS_NORMAL)
        {
            g_assert(error == NULL);  // no error if reading returns normal

            if (bytes_read == sizeof(data))
            {
                // Got the expected number of bytes, consider this a valid signal
                CL_LOGPRINTF("signal read from pipe: [%d] [%d] [%d]", data.signo, data.pid, data.status);
                if (data.signo == SIGCHLD)
                {
                    CL_LOGPRINTF("read a SIGCHLD signal");
                    on_sigchld(data.signo, data.pid, data.status);

                    // terminated application has been incorparated in our administration,
                    // so from now on we can allow sending keys to applications again.
                    // but first check if iLiad is waken up by RTC alarm
                    if (get_timedids_connect_reason() != connect_after_wakeup)
                    {
                        button_block_all_keys(FALSE);
                    }
                }
                else
                {
                    CL_ERRORPRINTF("Unexpected signal [%d] [%d]", data.signo, data.pid);
                }
            }
            else
            {
                // Incorrect number of bytes: discard garbage and keep fingers crossed
                CL_ERRORPRINTF("incomplete read from pipe: requested [%d] received [%d]", sizeof(data), bytes_read);
                done = TRUE;
            }
        }
        else if (status == G_IO_STATUS_AGAIN)
        {
            // Nothing more to read from pipe
            CL_LOGPRINTF("pipe empty");
            done = TRUE;
        }
        else
        {
            CL_ERRORPRINTF("unexpected return g_io_channel_read_chars [%d]", status);
            done = TRUE;
        }
    }
  
    return (TRUE);  // Please call again
}


////only used for debug reasons
//static void pm_DisplayExtensionInfo(GHashTable * g_extensionInfo)
//{
//    g_hash_table_foreach(g_extensionInfo, pm_DisplayExtInfo, NULL);
//    return;
//}

////only used for debug reasons
//static void pm_DisplayExtInfo(gpointer key, gpointer value, gpointer user_data)
//{
//    erExtInfo_t *theExtInfo = (erExtInfo_t *) value;
//
//    if (theExtInfo)
//    {
//        CL_PMPRINTF("");
//        CL_PMPRINTF("uaid %s", (NULL == theExtInfo->uaID) ? "NULL" : theExtInfo->uaID);
//        CL_PMPRINTF("icon %s", (NULL == theExtInfo->iconLocation) ? "NULL" : theExtInfo->iconLocation);
//        CL_PMPRINTF("ext %s", (NULL == key) ? "NULL" : (char *) key);
//    }
//}

////only used for debug reasons
//static void pm_DisplayProgramInfo(GHashTable * g_programInfo)
//{
//    g_hash_table_foreach(g_programInfo, pm_DisplayAppInfo, NULL);
//    return;
//}

//only used for debug reasons
//static void pm_DisplayAppInfo(gpointer key, gpointer value, gpointer user_data)
//{
//    erApplicationInfo_t *theApplicationInfo = (erApplicationInfo_t *) value;
//
//    if (theApplicationInfo)
//    {
//        CL_PMPRINTF("");
//        CL_PMPRINTF("uaid %s", (NULL == theApplicationInfo->uaID) ? "NULL" : theApplicationInfo->uaID);
//        CL_PMPRINTF("xResName %s", theApplicationInfo->userApp->xResName);
//        CL_PMPRINTF("szExec %s", theApplicationInfo->userApp->szExec);
//        CL_PMPRINTF("szArgMask %s", theApplicationInfo->userApp->szArgMask);
//        CL_PMPRINTF("szCategory %s", theApplicationInfo->userApp->szCategory);
//        CL_PMPRINTF("ipcChannel %d", theApplicationInfo->userApp->ipcChannel);
//        CL_PMPRINTF("pid %d", theApplicationInfo->pid);
//    }
//    else
//    {
//        CL_PMPRINTF("no value");
//    }
//}


/* Removed this unused function
static unsigned long pm_CheckWindowState(Display * dpy, Window window)
{
#define WM_STATE_ELEMENTS 1

    unsigned long retVal = 0;
    unsigned long *property = NULL;
    unsigned long nitems;
    unsigned long leftover;
    Atom    xa_WM_STATE, actual_type;
    int     actual_format;
    int     status;

    CL_PMPRINTF("entry");

    xa_WM_STATE = XInternAtom(dpy, "WM_STATE", False);

    status = XGetWindowProperty( dpy,
                                 window,
                                 xa_WM_STATE,
                                 0L,
                                 WM_STATE_ELEMENTS,
                                 False,
                                 xa_WM_STATE,
                                 &actual_type,
                                 &actual_format,
                                 &nitems,
                                 &leftover,
                                 (void*) &property );  // should be (unsigned char**) but that gives compiler warning

    if (!((status == Success) && (actual_type == xa_WM_STATE) && (nitems == WM_STATE_ELEMENTS)))
    {
        if (property)
        {
            XFree((char *) property);
            property = NULL;
            retVal = 0;
        }
    }

    if (property)
    {
        retVal = *property;
        XFree((char *) property);
    }

    return retVal;
}
*/
