/*
 * This file is part of pcshareMgr.
 *
 * pcshareMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pcshareMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file main.c
 * \brief pcshareMgr - PC Share Manager application startup
 * 
 * <File description>
 *
 */

/* pcshareMgr - A GTK based application on the eReader platform
 * Copyright (C) 2006 iRex Technologies BV
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

#include <gtk/gtk.h>

#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>
#include <liberregxml/erregapi.h>

#include "config.h"
#include "languages.h"
#include "control.h"
#include "systemcalls.h"
#include "logging.h"
#include "displayUpdate.h"
#include "sharethread.h"
#include "main.h"

// ipc channels
static erClientChannel_t contentListerChannel = NULL;

// command options
static gboolean g_background = FALSE;


// local functions
static void parse_arguments(int argc, char **argv);

// callback routines
static void     on_sigterm(int signo);

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
        else if (strncmp(argv[i], "--", 2) == 0)
        {
            printf("\nInvalid option: %s\n", argv[i]);
            printf(usage_text, argv[0]);
            _exit(1);
        }
    }
}

gboolean main_get_background()
{
    DL_LOGPRINTF("entry");
    return g_background;
}

void main_report_new_content()
{
    DL_LOGPRINTF("entry");
    clNewContent(contentListerChannel);
}

// read registry data into memory
static void load_registry(void)
{
    gboolean       b;
    regLoad_t      regLoad;
    
    b = erRegRWLockInit();
    if (b == FALSE)
    {
        DL_ERRORPRINTF("erRegRWLockInit fails with return code [%d]", b);
        g_assert_not_reached();
    }
    
    b = erRegReadLock();
    if (b == FALSE)
    {
        DL_ERRORPRINTF("erRegReadLock fails with return code [%d]", b);
        g_assert_not_reached();
    }
    
    regLoad = erRegLoad(regBasis_t);
    if (regLoad == loadError_t)
    {
        DL_ERRORPRINTF("erRegLoad(regBasis_t) fails with return code [%d]", regLoad);
        g_assert_not_reached();
    }
    
    // release lock, all data now in memory
    erRegUnlock();
}

// release registry data from memory
static void release_registry(void)
{
    erRegUnload(regBasis_t);
    erRegRWLockDestroy();
}


/*
* Terminate the main loop.
*/
static void on_destroy(GtkWidget * widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    GtkWidget*  window;
    shareMgr_t* theShareMgr;
    int         nRet;
    gboolean    ok;

    // catch the SIGTERM signal
    struct sigaction on_term;
    memset(&on_term, 0x00, sizeof(on_term));
    on_term.sa_handler = on_sigterm;
    sigaction(SIGTERM, &on_term, NULL);

    DL_WARNPRINTF(PACKAGE_STRING);

    parse_arguments(argc, argv);

    // minimum battery charge required for background mode
    if (g_background)
    {
        if (get_battery_charge() <= BATTERY_MINCHARGE_BACKGROUND)
        {
            DL_ERRORPRINTF("Insufficient battery [%d] for background PC connect -> quit", get_battery_charge());
            return NO_ERROR;
        }
    }
    
    /* init threads */
    g_thread_init(NULL);
    gdk_threads_init();

    // open the RC file associated with this program (re-use the downloadMgr one)
    gtk_rc_parse(DATA_DIR "/downloadMgr.rc");
    DL_LOGPRINTF("rc file %s", DATA_DIR "/downloadMgr.rc");

    gtk_init(&argc, &argv);

    load_registry();
    languagesInit();
    
    // create/init the channel to communicate with the content lister
    nRet = erIpcStartClient(ER_CONTENTLISTER_CHANNEL, &contentListerChannel);
    if (nRet != 0)
    {
        DL_ERRORPRINTF("erIpcStartClient returned %d", nRet);
        contentListerChannel = NULL;
    }

    // create the main, top level, window 
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), PACKAGE " " VERSION);
    gtk_window_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    // gtk_widget_set_size_request(GTK_WIDGET(window), SCREEN_WIDTH, SCREEN_HEIGHT);
    gtk_window_set_modal(GTK_WINDOW(window), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_fullscreen(GTK_WINDOW(window)); // Fullscreen overlap taskbars

    theShareMgr = ctrlInit(window);

    if (theShareMgr)
    {
        // Connect the destroy event of the window with our on_destroy function
        // When the window is about to be destroyed we get a notification and
        // stop the main GTK loop
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_destroy), NULL);

        // make sure that everything, window and label, are visible 
        if ( !g_background )
        {
            gtk_widget_show(window);
        }
    }

    // Start the download thread
    ok = shareThread_start((gpointer)theShareMgr);
    if (!ok)
    {
        printf("%s %s: ", __FILE__, __FUNCTION__);
        perror("Could not create thread\n");
    }

    // run the GTK main loop
    DL_LOGPRINTF("Before gtk_main");
    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();
    nRet = theShareMgr->returnVal;
    DL_LOGPRINTF("exitValue after gtk_main [%d]", nRet);

    // GTK done, abort any copying that might be running
    shareThread_stop();
    shareThread_wait(15);

    DL_WARNPRINTF("PC Share Manager quitting with return value \"%d\"", nRet);
    ctrlDestroy(theShareMgr);

    release_registry();

    return nRet;
}

// signal handlers
static void on_sigterm(int signo)
{
    DL_WARNPRINTF("entry pcshareMgr");

    if (gtk_main_level() > 0)
    {
        // stop main process
        gdk_threads_enter();
        DL_WARNPRINTF("stop GTK main");
        gtk_main_quit();
        gdk_threads_leave();
    }
    else
    {
        // not in gtk_main, probably in initialisation phase
        // just quit
        DL_WARNPRINTF("_exit(1)");
        _exit(1);
    }
}

/* vimlovers, you are great! vi:set ts=4 sw=4 ai et: */
