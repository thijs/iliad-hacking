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
 * \file settings.c
 * \brief setup application - E-reader settings application screen creation and handling                

 * <File description>
 * 
 */

#include <sys/wait.h>
#include <signal.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <libergtk/ergtk.h>
#include <liberipc/eripcsetup.h>
#include <liberipc/eripctoolbar.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "settings.h"
#include "erbusy.h"
#include "pagebar.h"
#include "toolbar.h"
#include "settingsData.h"
#include "iLiadConfigScreen.h"
#include "iLiadPincodeScreen.h"
#include "system.h"
#include "iLiadPincodeData.h"
#include "iLiadConfigData.h"
#include "iLiadUserScreen.h"
#include "iLiadUserData.h"
#include "iLiadBgConnectScreen.h"
#include "iLiadBgConnectData.h"
#include "iLiadPCConnectData.h"
#include "iLiadPCConnectScreen.h"
#include "iLiadArcLocData.h"
#include "iLiadArcLocScreen.h"
#include "iLiadDateTimeScreen.h"
#include "iLiadStartUpScreen.h"
#include "iLiadStartUpData.h"
#include "iLiadDeviceOverview.h"
#include "iLiadTimediDSScreen.h"
#include <signal.h>

// notebook widget
static GtkWidget *g_settings = NULL;

// local functions
static void update_pagebar(void);

// Callback functions
static void     on_goto_page(gint new_page);
static void     on_settings_switch_page(GtkNotebook *notebook, GtkNotebookPage *new_page, guint page_num, gpointer data);
static gboolean hook_screen_refresh_typing(GSignalInvocationHint *ihint, guint n_param_values, const GValue *param_values, gpointer user_data);

// IPC
#define SERVER_BUFFER_SIZE  1024
static erServerChannel_t theServerChannel;
static void setupMessageReceived(gpointer data, gint source_fd, GdkInputCondition condition);


//Create all settings screens 
//return the container (notebook) where the settings screens are stored 
GtkWidget *settings_create(void)
{
    GtkWidget* page;
    guint      signal_id;

    ST_LOGPRINTF("entry");

    g_settings = gtk_notebook_new();
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(g_settings), FALSE);
    gtk_notebook_set_show_border(GTK_NOTEBOOK(g_settings), FALSE);
    g_signal_connect_after(g_settings, "switch-page", G_CALLBACK(on_settings_switch_page), NULL);

    page = iLiad_startup_screen_create();
    iLiad_startup_screen_set_text();
    iLiad_startup_screen_display_data();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_settings), page, NULL);

    page = iLiad_config_create();
    iLiad_config_set_text();
    iLiad_config_data_display();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_settings), page, NULL);

    page = iLiad_archive_location_create();
    iLiad_archive_location_set_text();
    iLiad_archive_location_display_data();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_settings), page, NULL);

    page = iLiad_user_create();
    iLiad_user_set_text();
    iLiad_user_data_display();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_settings), page, NULL);

    page = iLiad_pincode_create();
    iLiad_pincode_set_text();
    iLiad_pincode_display_data();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_settings), page, NULL);

    page = iLiad_pc_connect_screen_create();
    iLiad_pc_connect_screen_set_text();
    iLiad_pc_connect_screen_display_settings();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_settings), page, NULL);

    page = iLiad_autoconnect_create();
    iLiad_autoconnect_set_text();
    iLiad_autoconnect_data_display();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_settings), page, NULL);

    page = iLiad_timedids_screen_create();
    iLiad_timedids_screen_set_text();
    iLiad_timedids_screen_data_display();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_settings), page, NULL);
    
    page = iLiad_date_time_create();
    iLiad_date_time_set_text();
    iLiad_date_time_display_data();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_settings), page, NULL);
                    
    page = iLiad_device_overview_create();
    iLiad_device_overview_set_text();
    iLiad_device_overview_display_data();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_settings), page, NULL);

    // install class-specific screen update handlers
    //   erGtkEntry
    signal_id = g_signal_lookup("screen-refresh", ERGTK_ENTRY_TYPE);
    g_assert(signal_id > 0);
    g_signal_add_emission_hook(signal_id, 0, hook_screen_refresh_typing, (gpointer)TEXT_ENTRY_CHANGED_LEVEL, NULL);
    //   erGtkToggleButton
    signal_id = g_signal_lookup("toggled", ERGTK_TOGGLE_BUTTON_TYPE);
    g_assert(signal_id > 0);
    g_signal_add_emission_hook(signal_id, 0, hook_screen_refresh_typing, (gpointer)SETTING_ITEM_CHANGE, NULL);

    // note book pages start counting from zero
    gtk_notebook_set_current_page(GTK_NOTEBOOK(g_settings), 0);
    gtk_widget_show(g_settings);

    update_pagebar();

    return g_settings;
}


gint settings_get_current_page(gint *pagecount)
{
    // Get number of pages, when requested.
    if (pagecount != NULL)
    {
        *pagecount = gtk_notebook_get_n_pages(GTK_NOTEBOOK(g_settings));
    }

    // Return current page number, counting from 0.
    return gtk_notebook_get_current_page(GTK_NOTEBOOK(g_settings));
}


// Goto the next page of the currently displayed settings screen notebook
void settings_page_down(gint count)
{
    gint    page;
    gint    pagecount;

    page = settings_get_current_page(&pagecount);
    page = page + count;
    if (page >= pagecount)
    {
        page = pagecount - 1; // counts from 0
    }

    ST_LOGPRINTF("page %d - pagecount %d", page, pagecount);
    on_goto_page(page+1);  // pagebar counts from 1
}

// Goto the previous page of the currently displayed settings screen notebook
void settings_page_up(gint count)
{
    gint    page;

    page = settings_get_current_page(NULL);
    if (page > 0)
    {
        page = page - count;
        if (page < 0)
        {
            page = 0;
        }

        ST_LOGPRINTF("page %d", page);
        on_goto_page(page+1);  // pagebar counts from 1
    }
}

gboolean on_settings_keypress(GtkWidget* widget, GdkEventKey* event, gpointer data)
{
    gboolean returnValue = FALSE;    // return FALSE => default gtk handling
    gboolean changed = FALSE;

    ST_LOGPRINTF("entry");
    erbusy_off();

    changed = iLiad_config_data_change_flipbar_direction();
    
    switch (event->keyval)
    {
        case GDK_Page_Down:
            if (changed)
                settings_page_up(1);
            else
                settings_page_down(1);
            returnValue = TRUE;  // no further event handling
            break;

        case GDK_Page_Up:
            if (changed)
                settings_page_down(1);
            else
                settings_page_up(1);
            returnValue = TRUE;  // no further event handling
            break;

        case GDK_F1:
            if (changed)
                settings_page_up(5);
            else
                settings_page_down(5);
            returnValue = TRUE;  // no further event handling
            break;

        case GDK_F2:
            if (changed)
                settings_page_down(5);
            else
                settings_page_up(5);
            returnValue = TRUE;  // no further event handling
            break;

        case GDK_F5:
            if (get_xtscal_pid() != 0)
            {
                stop_xtscal();
            }
            else
            {
                erbusy_blink();
                gtk_main_quit();
                returnValue = TRUE; // no further event handling
            }
            break;
            
        default:
            /* ignore */ ;
    }

    return returnValue;
}

// ////////////////////////////////////////////////////////////////////////
// keyboard handling

static gboolean settings_on_idle_show_keyboard(gpointer data);
static gboolean settings_on_idle_hide_keyboard(gpointer data);

// show keyboard:
//   wait for GTK objects updated then show keyboard
//   block display updates while updating GTK objects, rely on keyboard
//   display update
static void settings_show_keyboard()
{
    erbusy_blink();

    // block screen updates until keyboard shows
    display_update_increase_level(KEYBOARD_SHOW_HIDE_LEVEL);

    // update GTK objects then show keyboard,
    // rely on display update by keyboard
    g_idle_add(settings_on_idle_show_keyboard, NULL);

}
static gboolean settings_on_idle_show_keyboard(gpointer data)
{
    // request keyboard but first disable keyboard,
    // this makes sure the keyboard (re-)appears and does a display update
    // toolbar_setIconState(iconID_keyboard, iconState_normal);
    toolbar_setIconState(iconID_keyboard, iconState_selected);

    // allow display updates again
    display_update_decrease_level(LOWEST_LEVEL);

    return FALSE;               // don't call me again
}

// hide keyboard:
//   wait for GTK objects updated then show keyboard
//   block display updates while updating GTK objects, rely on keyboard
//   display update
static void settings_hide_keyboard(int iconState)
{
    g_assert((iconState == iconState_grey) || (iconState == iconState_normal));

    erbusy_blink();

    // block screen updates until keyboard hides
    display_update_increase_level(KEYBOARD_SHOW_HIDE_LEVEL);

    // update GTK objects then hide keyboard, rely on display update by
    // keyboard
    g_idle_add(settings_on_idle_hide_keyboard, (gpointer) iconState);
}

static gboolean settings_on_idle_hide_keyboard(gpointer data)
{
    int iconState = (int)data;

    // hide keyboard but first show keyboard,
    // this makes sure the keyboard (re-)disappears and does a display update
    // toolbar_setIconState(iconID_keyboard, iconState_selected);
    toolbar_setIconState(iconID_keyboard, iconState);

    display_update_request_screen_refresh(KEYBOARD_SHOW_HIDE_LEVEL, WAVEFORM_FULLSCREEN);

    return FALSE;               // don't call me again
}


// show/hide the on-screen keyboard
void show_keyboard(gboolean show)
{
    static gboolean show_previous = FALSE;
    ST_LOGPRINTF("entry: show [%d]", show);

    if (show)
    {
        // keyboard requested
        settings_show_keyboard();
        if (show_previous)
        {
            display_update_decrease_level(SETUP_SCREEN_EXPOSE_LEVEL);
        }
    }
    else
    {
        // keyboard not requested
        settings_hide_keyboard(iconState_grey);
    }
    show_previous = show;
}

void  show_lock_screen_icon(gboolean enable)
{
     if (FALSE == enable)
     {
         toolbar_setIconState(iconID_lock_screen, iconState_grey);
     }
     else
     {
         toolbar_setIconState(iconID_lock_screen, iconState_normal);
     }
}

static void on_settings_switch_page(GtkNotebook *notebook, GtkNotebookPage *new_page, guint page_num, gpointer data)
{
    ST_LOGPRINTF("Entry: new_page [%d]", (int)page_num);

    switch (page_num)
    {
        case iLiad_user_screen_t:
        case iLiad_pc_connect_screen_t:
        case iLiad_autoconnect_screen_t:
        case iLiad_pincode_screen_t:
        case iLiad_timedids_screen_t:
            show_keyboard(TRUE);
            break;
        default:
            show_keyboard(FALSE);
    }
    
    update_pagebar();
    display_update_request_screen_refresh(SETUP_SCREEN_EXPOSE_LEVEL, WAVEFORM_FULLSCREEN);
}


gboolean setupInstallIpcServer()
{
    int     returnValue;
    int     fd = -1;

    ST_IPCPRINTF("entry");

    returnValue = erIpcOpenServerChannel(ER_SETUP_CHANNEL, &theServerChannel);

    if (returnValue != (-1))
    {
        fd = erIpcGetServerFd(theServerChannel);

        ST_IPCPRINTF("erIpcGetServerFd returned %d\n", fd);

        // gtk specific mechanism to monitor input file descriptor.
        returnValue = gdk_input_add(fd, GDK_INPUT_READ, setupMessageReceived, (gpointer) theServerChannel);

        ST_IPCPRINTF("gdk_input_add returned %d", returnValue);
        return TRUE;
    }
    else
    {
        ST_ERRORPRINTF("Could not open server channel");
        return FALSE;
    }
}


static void update_pagebar()
{
    // update the page bar
    gint current_page;
    gint page_num;

    current_page = settings_get_current_page(&page_num);
    ST_LOGPRINTF("current_page %d - page_num %d", current_page, page_num);

    pagebar_reset();   // send pageBar events to us
    pagebar_set_pagecount(page_num);
    pagebar_goto_page(current_page + 1);  // pagebar counts from 1
    pagebar_redraw();
}


static void on_goto_page(gint new_page)
{
    gint    current_page;
    gint    pagecount;

    current_page = settings_get_current_page(&pagecount);

    ST_LOGPRINTF("new_page %d - pagecount %d", new_page, pagecount);

    // Note that gtk_notebook counts pages from 0, pagebar counts from 1
    if (new_page > 0  &&  new_page <= pagecount)
    {
        ST_LOGPRINTF("switch to new_page %d", new_page);

        // There are some additional task to do when page switching
        // When switching from timed-iDS screen, then we must disable timed-iDS if all time sets are empty;
        // When switching to date time screen, the screen must be refreshed to show the current time
        if (current_page == iLiad_timedids_screen_t)
        {
            iLiad_timedids_handle_page_switch();
        }

        if (new_page-1 == iLiad_datetime_screen_t)
        {
            iLiad_date_time_handle_refresh();
        }

        // when go to new page, it's possible there's entry in the new page
        // increase level to prevent 'on_entry_focus_in' call display update
        // we do like this, because we expect screen refreshing relys on keyboard
        display_update_increase_level(SETUP_SCREEN_EXPOSE_LEVEL);
        gtk_notebook_set_current_page(GTK_NOTEBOOK(g_settings), new_page-1);

        settings_data_store_page(current_page);
        
    }
}

static gboolean hook_screen_refresh_typing(GSignalInvocationHint *ihint, guint n_param_values, const GValue *param_values, gpointer user_data)
{
    int level = (int)user_data;

    ST_LOGPRINTF("entry: level [%d]", level);

    display_update_request_screen_refresh(level, WAVEFORM_TYPING);

    return TRUE;  // TRUE = continue gtk signal handling
}

// 
// This callback is invoked when there is data to read on file descriptor source_fd.
// The channel info is passed via parameter 'data'
//
static void setupMessageReceived(gpointer data, gint source_fd, GdkInputCondition condition)
{
    char    szBuffer[SERVER_BUFFER_SIZE];
    int     nBuf = SERVER_BUFFER_SIZE;
    int     page;
    
    erIpcCmd_t command;

    erIpcGetMessage((erServerChannel_t) data, szBuffer, &nBuf);

    ST_IPCPRINTF("Received %s\n", szBuffer);

    if (stParseCommand(szBuffer, &command) >= 0)
    {
        switch (command.cc)
        {
            case ccStGotoPage:
                page = atoi(command.arg[0]);
                ST_IPCPRINTF("Setup Goto Page: %d\n", page);
                gdk_threads_enter();
                erbusy_blink();
                on_goto_page(page);
                gdk_threads_leave();
                break;

            default:
                ST_IPCPRINTF("UNKNOWN COMMAND %d", command.cc);
                break;
        }
    }
}
