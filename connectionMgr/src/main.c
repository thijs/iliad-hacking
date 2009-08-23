/*
 * This file is part of connectionMgr.
 *
 * connectionMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * connectionMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file main.c
 * \brief connectionMgr - Main routines to create an environment 
 *                        for the E-reader Network-connection application 
 *                        and to handle key strokes
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include "config.h"

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <liberipc/eripctoolbar.h>
#include <liberdm/connectionMgrConstants.h>
#include <liberdm/display.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "background.h"
#include "connectScreenData.h"
#include "connectionMgr.h"
#include "connectionMgrData.h"
#include "connectScreen.h"
#include "editScreen.h"
#include "connectBackground.h"
#include "connectScan.h"
#include "pingThread.h"
#include "scanThread.h"
#include "languages.h"
#include "erbusy.h"
#include "pagebar.h"
#include "system.h"
#include "toolbar.h"

// command options
static gboolean g_connect_after_reboot = FALSE;
static gboolean g_stay_connected = FALSE;
static gboolean g_edit_only = FALSE;
static gboolean g_background = FALSE;
static gboolean g_content_only = FALSE;
static gchar*   g_content_uuid = NULL;
static gboolean g_use_last_connected = FALSE;
static gboolean g_wifi_disabled = FALSE;
static gboolean g_dialup_disabled = TRUE;
static char*    g_screen_title = NULL;

// local data
static int        g_exitValue = CONNECT_ERROR;
static GtkWidget* g_main_window = NULL;

// connection type as specified in command-line parameters
static cmgrConnectType_t g_connect_type = cmgrConnectUnknown_e;  
static int   exec_argc = 0;
static char* exec_argv[10];
static pid_t g_exec_pid = 0; // pid on which exec_argv command is running

// local functions
static void     parse_arguments(int argc, char **argv);
static gboolean delayed_start_child_process(gpointer user_data);

// callback routines
static void     on_sigterm(int signo);
static void     on_sigchld(int signo);
static void     on_main_window_destroy(GtkWidget * widget, gpointer data);
static gboolean on_main_window_expose_event(GtkWidget * widget, 
                                            GdkEventExpose * event, 
                                            gpointer p);
static gboolean on_main_window_focus_out_event(GtkWidget * widget, 
                                               GdkEventFocus * event, 
                                               gpointer p);


// parse command-line arguments
static void parse_arguments(int argc, char **argv)
{
    static const char *usage_text =
                        "\n"
                        "usage: %s [options]\n"
                        "\n"
                        "options:\n"
                        "    --help\n"
                        "        Print help text and quit\n"
                        "    --background\n"
                        "        Operate in background, no GUI\n"
                        "    --connect-ids\n"
                        "        Connect to iDS\n"
                        "    --connect-pc\n"
                        "        Connect to PC share\n"
                        "    --content-only\n"
                        "        Download content items, skip software upgrades\n"
                        "    --content-uuid <uuid>\n"
                        "        Together with --content-only, download only this content item\n"
                        "    --edit-only\n"
                        "        Edit profiles only, do connect automatically\n"
                        "    --use-last-connected\n"
                        "        Use last connected profile first\n"
                        "    --stay-connected\n"
                        "        Keep network enabled when exiting connection manager\n"
                        "    --connect-after-reboot\n"
                        "        Connect automatically after rebooting\n"
                        "    --title <text>\n"
                        "        <text> = screen title\n"
                        "    --execute\n"
                        "        When connected to IDS, execute rest of the arguments as a new command\n"
                        "\n";
    int i;

    // parse connectionMgr options
    for (i = 0 ; i < argc ; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            printf(usage_text, argv[0]);
            _exit(0);
        }
        else if(strcmp(argv[i], "--background") == 0)
        {
            g_background = TRUE;
        }
        else if(strcmp(argv[i], "--connect-ids") == 0)
        {
            g_connect_type = cmgrConnectIDS_e;
        }
        else if(strcmp(argv[i], "--connect-pc") == 0)
        {
            g_connect_type = cmgrConnectPCShare_e;
        }
        else if(strcmp(argv[i], "--content-only") == 0)
        {
            g_content_only = TRUE;
        }
        else if(strcmp(argv[i], "--content-uuid") == 0)
        {
            if (++i < argc)
            {
                g_content_uuid = argv[i];
            }
            else
            {
                i--;  // oops
            }
        }
        else if(strcmp(argv[i], "--edit-only") == 0)
        {
            g_edit_only = TRUE;
        }
        else if(strcmp(argv[i], "--use-last-connected") == 0)
        {
            g_use_last_connected = TRUE;
        }
        else if(strcmp(argv[i], "--stay-connected") == 0)
        {
            g_stay_connected = TRUE;
        }
        else if (strcmp(argv[i], "--connect-after-reboot") == 0)
        {
            g_connect_after_reboot = TRUE;
        }
        else if (strcmp(argv[i], "--title") == 0)
        {
            if (++i < argc)
            {
                g_screen_title = argv[i];
            }
            else
            {
                i--;  // oops
            }
        }
        else if (strcmp(argv[i], "--execute") == 0)
        {
            i++;
            if ( argc - i >= sizeof(exec_argv)/sizeof(exec_argv[0]) )
            {
                printf("\nToo many arguments for option [%s]\n", argv[i]);
                printf(usage_text, argv[0]);
                _exit(1);
            }

            exec_argc = 0;
            while (i < argc)
            {
                exec_argv[exec_argc++] = argv[i++];
            }
            exec_argv[exec_argc] = NULL;
        }
        else if (strncmp(argv[i], "--", 2) == 0)
        {
            printf("\nInvalid option: %s\n", argv[i]);
            printf(usage_text, argv[0]);
            _exit(1);
        }
    }
}

// read registry data into memory
static void load_registry(void)
{
    gboolean       b;
    regLoad_t      regLoad;
    
    // init registry locking
    b = erRegRWLockInit();
    if (b == FALSE)
    {
        CN_ERRORPRINTF("erRegRWLockInit fails with return code [%d]", b);
        g_assert_not_reached();
    }
    
    // acquire read-only lock
    b = erRegReadLock();
    if (b == FALSE)
    {
        CN_ERRORPRINTF("erRegReadLock fails with return code [%d]", b);
        g_assert_not_reached();
    }
    
    // load registry sections used
    regLoad = erRegLoad(regBasis_t);
    if (regLoad == loadError_t)
    {
        CN_ERRORPRINTF("erRegLoad(regBasis_t) fails with return code [%d]", 
                        regLoad);
        g_assert_not_reached();
    }
    regLoad = erRegLoad(regNWProfiles_t);
    if (regLoad == loadError_t)
    {
        CN_ERRORPRINTF("erRegLoad(regNWProfiles_t) "
                       "fails with return code [%d]", 
                       regLoad);
        g_assert_not_reached();
    }
    
    // release lock, all data now in memory
    erRegUnlock();
}

// release registry data from memory
static void release_registry(void)
{
    // unload registry sections
    erRegUnload(regNWProfiles_t);
    erRegUnload(regBasis_t);

    // discard registry locking
    erRegRWLockDestroy();
}

// prepare registry for writing: lock + reload
void prepare_registry_write( void)
{
    gboolean       b;
    regLoad_t      regLoad;
    
    // acquire write lock
    b = erRegWriteLock();
    if (b == FALSE)
    {
        CN_ERRORPRINTF("erRegWriteLock fails with return code [%d]", b);
        g_assert_not_reached();
    }
    
    // reload section(s) that may be written
    erRegUnload(regNWProfiles_t);
    regLoad = erRegLoad(regNWProfiles_t);
    if (regLoad == loadError_t)
    {
        CN_ERRORPRINTF("erRegLoad(regNWProfiles_t) fails "
                       "with return code [%d]", 
                       regLoad);
        g_assert_not_reached();
    }
}

// write modified registry: write + unlock
void do_registry_write(void)
{
    gboolean       b;
    
    // verify write lock
    g_assert(lock_write == erRegGetLockState());
    
    // save registry
    b = erRegStore();
    if (b == FALSE)
    {
        CN_ERRORPRINTF("erRegStore fails with return code [%d]", b);
    }

    // release lock
    erRegUnlock();
}

void get_wifi_and_dialup_status(void)
{
    int status;
    
    CN_LOGPRINTF("Checking status of WiFi...");
    status = system(COMMAND_STATUS_WIRELESS);
    if (WEXITSTATUS(status) == 0)
    {
        g_wifi_disabled = FALSE;
    }
    else
    {
        g_wifi_disabled = TRUE;
    }
    CN_LOGPRINTF("WiFi present? [%d]", g_wifi_disabled);
    
    CN_LOGPRINTF("Checking status of Dialup...");
    status = system(COMMAND_STATUS_DIALUP);
    if (WEXITSTATUS(status) == 0)
    {
        g_dialup_disabled = FALSE;
    }
    else
    {
        g_dialup_disabled = TRUE;
    }
    CN_LOGPRINTF("Dialup present? [%d]", g_dialup_disabled);
}

int main(int argc, char **argv)
{
    GtkWidget* clientArea = NULL;
    GtkWidget* screens = NULL;
    GtkWidget* widget;

    scanContext_t *ctxt;

    parse_arguments(argc, argv);
    
    // catch the SIGTERM signal
    struct sigaction on_term;
    memset(&on_term, 0x00, sizeof(on_term));
    on_term.sa_handler = on_sigterm;
    sigaction(SIGTERM, &on_term, NULL);

    // catch the SIGCHLD signal
    struct sigaction on_chld;
    memset(&on_chld, 0x00, sizeof(on_chld));
    on_chld.sa_handler = on_sigchld;
    on_chld.sa_flags   = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &on_chld, NULL);

    // init threads - needed for screen refresh idle handling 
    g_thread_init(NULL);
    gdk_threads_init();

    // open the RC file associate with this program
    gtk_rc_parse(DATA_DIR "/connectionMgr.rc");
    CN_LOGPRINTF("rc file %s", DATA_DIR "/connectionMgr.rc");

    gtk_init(&argc, &argv);
    
    // check for WiFi and Dialup hardware status
    get_wifi_and_dialup_status();

    load_registry();
    cmgr_data_init();
    languagesInit();

    // note: set connect type only 
    // after connection mgr data has been initialized
    if (g_connect_type != cmgrConnectUnknown_e)
    {
        CN_WARNPRINTF("g_connect_type [%d]", g_connect_type);
        connect_data_set_connect_type(g_connect_type);
    }

    // init the context for scan and ping
    cmgr_scan_ctxt_init();
    cmgr_ping_ctxt_init();

    if (!g_background)
    {
        erbusy_init();
        pagebar_init();
        toolbar_init();
        cmgrInstallIpcServer();

        // => only request a refresh on the main window expose event
        display_update_increase_level(MAIN_WINDOW_EXPOSE_LEVEL);

        // create the main, top level, window 
        widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(widget), PACKAGE " " VERSION);
        gtk_window_position(GTK_WINDOW(widget), GTK_WIN_POS_CENTER);
        gtk_container_set_border_width(GTK_CONTAINER(widget), 0);
        gtk_widget_set_size_request(GTK_WIDGET(widget), 
                                    SCREEN_WIDTH, CLIENT_AREA);
        gtk_window_set_modal(GTK_WINDOW(widget), TRUE);
        gtk_window_set_resizable(GTK_WINDOW(widget), FALSE);
        g_signal_connect(G_OBJECT(widget), "destroy", 
                G_CALLBACK(on_main_window_destroy), NULL);
        g_signal_connect_after(G_OBJECT(widget), "expose-event", 
                           G_CALLBACK(on_main_window_expose_event), NULL);
        g_signal_connect(G_OBJECT(widget), "focus-out-event", 
                         G_CALLBACK(on_main_window_focus_out_event), NULL);
        g_signal_connect(G_OBJECT(widget), "key-press-event", 
                         G_CALLBACK(on_cmgr_keypress), NULL);
        gtk_widget_show(widget);
        g_main_window = widget;

        // create and display the application background and screens
        clientArea = bg_create(g_main_window);
        screens = cmgr_screens_create();
        gtk_container_add(GTK_CONTAINER(clientArea), screens);

        // show the correct screen
        if (main_get_edit_only())
        {
            cmgr_set_screen_mode(cmgrScreenEdit_e);
            edit_set_mode(editModeNormal_e);
        }
        else
        {
            cmgr_set_screen_mode(cmgrScreenConnect_e);
        }
    }
    else
    {
        connect_background_restore_scan_ctxt();
        ctxt = cmgr_get_scan_ctxt();
        connect_scan(ctxt, FALSE);
    }

    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();

    CN_WARNPRINTF("exitValue after gtk_main [%d]", g_exitValue);

    // save connection manager settings
    connect_data_store();

    // wait for pingThread and scanThread to end
    pingThread_stop();
    pingThread_wait(30);
    scanThread_stop();
    scanThread_wait(30);

    cmgr_scan_ctxt_destory();
    cmgr_ping_ctxt_destory();
 
    // disconnect, unless requested otherwise
    if (!g_stay_connected)
    {
        connect_disable_network();
    }

    release_registry();

    CN_WARNPRINTF("exit connectionMgr [%d]", g_exitValue);
    return g_exitValue;
}

void main_quit(void)
{
    CN_LOGPRINTF("entry");

    if (g_exec_pid > 0)
    {
        // stop child process
        CN_WARNPRINTF("stop child process [%d]", g_exec_pid);
        kill(g_exec_pid, SIGTERM);
    }
    else if (gtk_main_level() > 0)
    {
        // no child process: stop gtk engine
        CN_WARNPRINTF("stop GTK main");
        gtk_main_quit();
    }
    else
    {
        // no child process, no in gtk_main, 
        // probably in initialisation phase: just quit
        CN_WARNPRINTF("_exit(1)");
        _exit(1);
    }
}


// Execute command that may have been specified on command-line.
// This function is called from the gtk thread, 
// it starts a child process then returns.
void main_start_child_process(void)
{
    if (!g_background)
    {
       // no more screen updates from connection manager
       display_update_increase_level(NO_DISPLAY_UPDATE_LEVEL);
       // show_keyboard(FALSE);
       toolbar_setIconState(iconID_keyboard, iconState_grey);
    }

    g_idle_add(delayed_start_child_process, NULL);
}

static gboolean delayed_start_child_process(gpointer user_data)
{
    int   i;
    pid_t pid;

    CN_LOGPRINTF("entry");

    // execute command, when specified
    if (exec_argc == 0)
    {
        // no command yet: determine command to execute
        if (connect_data_get_connect_type() == cmgrConnectIDS_e)
        {
            exec_argv[exec_argc++] = DOWNLOADMGR_EXECUTABLE;
            if (g_background)
            {
                exec_argv[exec_argc++] = "--background";
            }
            if (g_content_only)
            {
                exec_argv[exec_argc++] = "--content-only";
            }
            if (g_content_uuid)
            {
                exec_argv[exec_argc++] = "--content-uuid";
                exec_argv[exec_argc++] = g_content_uuid;
            }
        }
        else if (connect_data_get_connect_type() == cmgrConnectPCShare_e)
        {
            exec_argv[exec_argc++] = PCSHAREMGR_EXECUTABLE;
            if (g_background)
            {
                exec_argv[exec_argc++] = "--background";
            }
        }
    }

    // execute command
    if (exec_argc > 0)
    {
        exec_argv[exec_argc] = NULL;
        g_assert( exec_argc < (sizeof(exec_argv)/sizeof(exec_argv[0])) );

        CN_WARNPRINTF("execute command [%s]", exec_argv[0]);
        for (i = 0 ; i < exec_argc ; i++)
        {
            CN_WARNPRINTF("    argv[%d] = [%s]", i, exec_argv[i]);
        }

        pid = fork_exec_no_wait(exec_argc, exec_argv);
        if (pid > 0)
        {
            g_exec_pid = pid;
        }
        else
        {
            CN_ERRORPRINTF("Execute [%s] failed "
                           "- fork_exec_no_wait returns [%d]", 
                           exec_argv[0], pid);
            g_exitValue = CONNECT_ERROR;
        }
    }

    return FALSE;  // don't call me again
}


gboolean main_get_background()
{
    CN_LOGPRINTF("entry");
    return g_background;
}

cmgrConnectType_t main_get_connect_type()
{
    CN_LOGPRINTF("return [%d]", g_connect_type);
    return g_connect_type;
}

gboolean main_get_edit_only()
{
    CN_LOGPRINTF("return [%d]", g_edit_only);
    return g_edit_only;
}

gboolean main_get_use_last_connected()
{
    CN_LOGPRINTF("return [%d]", g_use_last_connected);
    return g_use_last_connected;
}

gboolean main_get_connect_after_reboot(void)
{
    CN_LOGPRINTF("return [%d]", g_connect_after_reboot);
    return g_connect_after_reboot;
}

gboolean main_get_wifi_disabled(void)
{
    CN_LOGPRINTF("return [%d]", g_wifi_disabled);
    return g_wifi_disabled;
}

gboolean main_get_dialup_disabled(void)
{
    CN_LOGPRINTF("return [%d]", g_dialup_disabled);
    return g_dialup_disabled;
}

char* main_get_screen_title()
{
    CN_LOGPRINTF("return [%s]", g_screen_title);
    return g_screen_title;
}


// signal handlers
static void on_sigterm(int signo)
{
    CN_WARNPRINTF("    -- entry connectionMgr, my pid [%d]", getpid());

    // stop main process, prepare to quit application
    gdk_threads_enter();
    main_quit();
    gdk_threads_leave();

    show_keyboard(FALSE);
    g_stay_connected = FALSE;

    CN_WARNPRINTF("    -- leave connectionMgr");
}

static void on_sigchld(int signo)
{
    CN_WARNPRINTF("    -- entry connectionMgr");

    // check which process has died and free resources
    pid_t pid;
    int   status;
    while ( (pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        CN_WARNPRINTF("pid [%d]", pid);
        if (pid == g_exec_pid)
        {
            g_exec_pid = 0;
            if ( WIFEXITED(status) )
            {
                g_exitValue = WEXITSTATUS(status);
            }
            main_quit();
        }
    }

    CN_WARNPRINTF("    -- leave connectionMgr");
}

static void on_main_window_destroy(GtkWidget * widget, gpointer data)
{
    CN_WARNPRINTF("entry");

    //clean up or store stuff before the app ends
    gtk_main_quit();
}

static gboolean on_main_window_expose_event(GtkWidget * widget, 
                                            GdkEventExpose * event, 
                                            gpointer p)
{
    CN_LOGPRINTF("entry");
    display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL, 
                                          WAVEFORM_FULLSCREEN);
    
    if (!main_get_edit_only())
    {
        gtk_idle_add(delay_erbusy_blink, NULL);
    }
   
    return FALSE;
}

static gboolean on_main_window_focus_out_event(GtkWidget *widget, 
                                               GdkEventFocus *event, 
                                               gpointer p)
{
    CN_LOGPRINTF("entry");

    return FALSE;
}

