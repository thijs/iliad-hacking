/*
 * This file is part of settings.
 *
 * settings is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * settings is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

/**
 * \file main.c
 * \brief setup application - Main routines to create an environment for the E-reader settings application                 
 *                            and to handle key strokes
 * <File description>
 * 
 */

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <liberdm/display.h>
#include <libergtk/ergtk.h>

#include <liberregxml/erregapi.h>


#include "config.h"
#include "displayStatus.h"
#include "setupLog.h"
#include "background.h"
#include "settings.h"
#include "settingsData.h"
#include "iLiadUserData.h"
#include "iLiadConfigData.h"
#include "iLiadBgConnectData.h"
#include "iLiadPCConnectData.h"
#include "iLiadArcLocData.h"
#include "iLiadStartUpData.h"
#include "iLiadStartUpScreen.h"
#include "iLiadTimediDSData.h"
#include "system.h"
#include "iLiadPincodeData.h"
#include "iLiadLanguageData.h"
#include "iLiadLanguageScreen.h"
#include "erbusy.h"
#include "pagebar.h"
#include "toolbar.h"
#include "languages.h"


//callback routines
static void     on_sigterm(int signo);
static void     on_sigchld(int signo);
static void     on_main_window_destroy(GtkWidget * widget, gpointer data);
static gboolean on_main_window_expose_event(GtkWidget * widget, GdkEventExpose * event, gpointer p);
static gboolean on_main_window_focus_out_event(GtkWidget * widget, GdkEventFocus * event, gpointer p);

static gboolean g_select_language_only = FALSE;

static void parse_arguments(int argc, char **argv)
{
    static const char *usage_text =
                        "\n"
                        "usage: %s [options]\n"
                        "\n"
                        "options:\n"
                        "    --help\n"
                        "        Print help text and quit\n"
                        "    --select-language-only\n"
                        "       Show language settings screen only\n"
                        "\n";
    int i;

    for (i = 0 ; i < argc ; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            printf(usage_text, argv[0]);
            _exit(0);
        }
        else if(strcmp(argv[i], "--select-language-only") == 0)
        {
            ST_LOGPRINTF("%s is only for language selection", argv[0]);
            g_select_language_only = TRUE;
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
        ST_ERRORPRINTF("erRegRWLockInit fails with return code [%d]", b);
        g_assert_not_reached();
    }
    
    // acquire read-only lock
    b = erRegReadLock();
    if (b == FALSE)
    {
        ST_ERRORPRINTF("erRegReadLock fails with return code [%d]", b);
        g_assert_not_reached();
    }
    
    // load registry sections used
    regLoad = erRegLoad(regBasis_t);
    if (regLoad == loadError_t)
    {
        ST_ERRORPRINTF("erRegLoad(regBasis_t) fails with return code [%d]", regLoad);
        g_assert_not_reached();
    }
   
    regLoad = erRegLoad(regNWProfiles_t);
    if (regLoad == loadError_t)
    {
        ST_ERRORPRINTF("erRegLoad(regNWProfiles_t_t) fails with return code [%d]", regLoad);
        g_assert_not_reached();
    }

    // release lock, all data now in memory
    erRegUnlock();
}

// release registry data from memory
static void release_registry(void)
{
    // unload registry sections
    erRegUnload(regBasis_t);
    erRegUnload(regNWProfiles_t);

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
        ST_ERRORPRINTF("erRegWriteLock fails with return code [%d]", b);
        g_assert_not_reached();
    }
    
    // reload section(s) that may be written
    erRegUnload(regBasis_t);
    regLoad = erRegLoad(regBasis_t);
    if (regLoad == loadError_t)
    {
        ST_ERRORPRINTF("erRegLoad(regBasis_t) fails with return code [%d]", regLoad);
        g_assert_not_reached();
    }

    erRegUnload(regNWProfiles_t);
    regLoad = erRegLoad(regNWProfiles_t);
    if (regLoad == loadError_t)
    {
        ST_ERRORPRINTF("erRegLoad(regNWProfiles_t) fails with return code [%d]", regLoad);
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
        ST_ERRORPRINTF("erRegStore fails with return code [%d]", b);
    }

    // release lock
    erRegUnlock();
}

int main(int argc, char **argv)
{
    GtkWidget *window=NULL;
    GtkWidget *settingsArea=NULL;
    GtkWidget *settingsScreens=NULL;

    parse_arguments(argc, argv);

    // init threads - needed for screen refresh idle handling 
    g_thread_init(NULL);
    gdk_threads_init();

    // open the RC file associate with this program
    gtk_rc_parse(DATA_DIR "/setup.rc");
    ST_LOGPRINTF("rc file %s", DATA_DIR "/setup.rc");

    gtk_init(&argc, &argv);

    load_registry();
    erbusy_init();
    erbusy_blink();
 
    if (g_select_language_only)
    {
        iLiad_language_data_init();
    }
    else
    {
        settings_data_init();
        languagesInit();
        pagebar_init();
        toolbar_init();
        setupInstallIpcServer();
    }

    // => only request a refresh on the main window expose event
    display_update_increase_level(MAIN_WINDOW_EXPOSE_LEVEL);

    if (g_select_language_only)
    {
        window = iLiad_language_screen_create();
        iLiad_language_screen_display_data();
        gtk_widget_show(window);
    }
    else
    {
        // create the main, top level, window 
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), PACKAGE " " VERSION);
        gtk_window_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_container_set_border_width(GTK_CONTAINER(window), 0);
        gtk_widget_set_size_request(GTK_WIDGET(window), SCREEN_WIDTH, SCREEN_HEIGHT - TOOLBAR_HEIGTH - PAGEBAR_HEIGHT);
        gtk_window_set_modal(GTK_WINDOW(window), TRUE);
        gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

        g_signal_connect(      G_OBJECT(window), "destroy",         G_CALLBACK(on_main_window_destroy),         NULL);
        g_signal_connect_after(G_OBJECT(window), "expose-event",    G_CALLBACK(on_main_window_expose_event),    NULL);
        g_signal_connect(      G_OBJECT(window), "key-press-event", G_CALLBACK(on_settings_keypress),           NULL);
        g_signal_connect(      G_OBJECT(window), "focus-out-event", G_CALLBACK(on_main_window_focus_out_event), NULL);
        
        // create and display the application background and screens
        settingsArea = bg_create(window);
        bg_set_text();
        settingsScreens = settings_create();
        gtk_container_add(GTK_CONTAINER(settingsArea), settingsScreens);
      
        gtk_widget_show(window);
        display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL, WAVEFORM_FULLSCREEN);
    }

    // catch the TERM signal
    signal(SIGTERM, on_sigterm);
    if (!g_select_language_only) 
    {
        signal(SIGCHLD, on_sigchld); 
    }

    gdk_threads_enter();
    gtk_main();
   
    display_update_increase_level(NO_DISPLAY_UPDATE_LEVEL); // no more screen updates
   
    if (!g_select_language_only)
    {
        gtk_widget_hide(settingsArea);  // force a focus-out event on all objects
    }
    
    gdk_threads_leave();

    if (!g_select_language_only)
    {
        // store settings when changed
        iLiad_user_data_store();
        iLiad_config_data_store();
        iLiad_autoconnect_data_store();
        iLiad_pc_connect_data_store();
        iLiad_pincode_data_store();
        iLiad_archive_location_data_store();
        iLiad_startup_data_store();
        iLiad_timedids_data_store();
    }

    release_registry();

    return 0;
}

// signal handlers
static void on_sigterm(int signo)
{
    ST_WARNPRINTF("entry settings: signo [%d]", signo);

    // stop calibration process
    stop_xtscal();

    // stop main process
    gdk_threads_enter();
    gtk_main_quit();
    gdk_threads_leave();
}

static void on_sigchld(int signo)
{
    pid_t pid;
    int status;

    signal(SIGCHLD, on_sigchld);
    if ((pid = wait(&status)) < 0)
    {
        ST_ERRORPRINTF("wait failed!");
        return;
    }
    if (pid == get_xtscal_pid())
    {
        // xtscal quits
        handle_xtscal_quit();
    }
}

static void on_main_window_destroy(GtkWidget * widget, gpointer data)
{
    ST_LOGPRINTF("entry");
    
    gtk_main_quit();
}

static gboolean on_main_window_expose_event(GtkWidget * widget, GdkEventExpose * event, gpointer p)
{
    ST_LOGPRINTF("entry");

    display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL, WAVEFORM_FULLSCREEN);
    return FALSE;
}


static gboolean on_main_window_focus_out_event(GtkWidget * widget, GdkEventFocus * event, gpointer p)
{
    ST_LOGPRINTF("entry");

    settings_data_store_currentpage();
    return FALSE;
}
