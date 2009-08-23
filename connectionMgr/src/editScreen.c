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
 * \file editScreen.c
 * \brief connectionMgr - the entry of 'profile-editor' screen
 *                        containing a editScreenProfiles
 *                               and a editScreenSearch
 *                               and a editScreenOverview
 *                               and a editScreenWireless
 *                               and a editScreenWirelessExt
 *                               and a editScreenStatic
 *                               and a editScreenProxy
 *                        screen handling and event processing
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <liberdm/display.h>
#include <liberdm/connectionMgrConstants.h>
#include <liberregxml/erregapi.h>
#include <liberipc/eripctoolbar.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "erbusy.h"
#include "pagebar.h"
#include "toolbar.h"
#ifdef ECDC_LIBRARY
#include "ecdctypes.h"
#else
#include "ecdc_err.h"
#endif
#include "commonData.h"
#include "editScreenData.h"
#include "connectionMgr.h"
#include "background.h"
#include "connectScreen.h"
#include "connectScreenProfiles.h"
#include "connectScreenData.h"
#include "connectBackground.h"
#include "editScreen.h"
#include "editScreenProfiles.h"
#include "editScreenSearch.h"
#include "editScreenOverview.h"
#include "editScreenWireless.h"
#include "editScreenWirelessExt.h"
#include "editScreenDialup.h"
#include "editScreenStatic.h"
#include "editScreenProxy.h"
#include "connectPing.h"
#include "languages.h"

// screen object
static GtkWidget *g_edit_notebook = NULL;

// a flag indicates the profile editor is launched from where
static editMode_t g_edit_mode = editModeNormal_e;

// a flag indicates whether it's for editing an exsiting profile 
// or creating a new profile  
static gboolean g_wizard_mode = FALSE;

// a flag indicates whether allow to back overview page 
// from wireless/dialup/wireless_ext page
static gboolean g_back_overview = FALSE;

// a flag indicates it's in setting network settings
static gboolean g_in_set_network_settings = FALSE;

// current network settings
static networkProfile_t* g_nw_settings = NULL;

// signal handlers
static void on_edit_screen_destroy(GtkObject* object, gpointer data);
static void on_edit_switch_page(GtkNotebook* notebook, 
                                GtkNotebookPage* new_page, 
                                guint page_num, 
                                gpointer data);

// toolbar
static void edit_show_back(void);
static void edit_show_keyboard(void);

//
// Create all edit screens 
// return the container (notebook) where the edit screens are stored 
//
// object hierarchy:
// alignment
//    |--vbox
//       |--g_edit_notebook (gtkNotebook)
//          |--> page (edit_profiles screen)
//          |--> page (edit_search screen)
//          |--> page (edit_overview screen)
//          |--> page (edit_wireless screen)
//          |--> page (edit_wireless_ext screen)
//          |--> page (edit_static screen)
//          |--> page (edit_proxy screen)
//
GtkWidget *edit_screen_create(void)
{
    GtkWidget* alignment;
    GtkWidget* vbox;
    GtkWidget* page;
    GtkWidget* widget;

    CN_LOGPRINTF("entry");
    
    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), 
            0, 0, SCREEN_BORDER_LEFT, 0);
    g_signal_connect(alignment, "destroy", 
            G_CALLBACK(on_edit_screen_destroy), NULL);
    gtk_widget_show(alignment);
    
    // vbox
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    gtk_widget_show(vbox);

    // g_edit_notebook
    widget = gtk_notebook_new();
    gtk_widget_set_size_request(widget, -1, -1);
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(widget), FALSE);
    gtk_notebook_set_show_border(GTK_NOTEBOOK(widget), FALSE);
    g_edit_notebook = widget;

    page = edit_profiles_create();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_edit_notebook), page, NULL);

    page = edit_search_create();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_edit_notebook), page, NULL);

    page = edit_overview_create();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_edit_notebook), page, NULL);

    page = edit_wireless_create();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_edit_notebook), page, NULL);

    page = edit_wireless_ext_create();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_edit_notebook), page, NULL);

    page = edit_dialup_create();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_edit_notebook), page, NULL);

    page = edit_static_create();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_edit_notebook), page, NULL);

    page = edit_proxy_create();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_edit_notebook), page, NULL);

    // note book pages start counting from zero
    gtk_notebook_set_current_page(GTK_NOTEBOOK(g_edit_notebook), 0);
    g_signal_connect_after(g_edit_notebook, "switch-page", 
                           G_CALLBACK(on_edit_switch_page), NULL); 

    gtk_box_pack_start(GTK_BOX(vbox), g_edit_notebook, FALSE, FALSE, 0);
    gtk_widget_show(g_edit_notebook);

    return alignment;
}

void edit_screen_set_text(void)
{
    CN_LOGPRINTF("entry");

    edit_profiles_set_text();
    edit_search_set_text();
    edit_overview_set_text();
    edit_wireless_set_text();
    edit_wireless_ext_set_text();
    edit_dialup_set_text();
    edit_static_set_text();
    edit_proxy_set_text();
}

void edit_screen_display(void)
{
    int page;

    CN_LOGPRINTF("entry");
    
    page = edit_get_current_page(NULL);
    if (page == editScreenProfiles_e)
    {
        edit_profiles_display();
    }
}

editMode_t edit_get_mode(void)
{
    CN_LOGPRINTF("return mode[%d]", g_edit_mode);
    
    return g_edit_mode;
}

void edit_set_mode(editMode_t mode)
{
    CN_LOGPRINTF("entry mode[%d]", mode);

    if ((mode >= 0) && (mode < editModeUndef_e))
    {
        g_edit_mode = mode;

        bg_display_title();

        switch (mode)
        {
            case editModeConnectFillIn_e:
            case editModeConnectCreate_e:
            case editModeConnectNormal_e:
                edit_overview_enable_save_button(FALSE);
                edit_wireless_enable_save_button(FALSE);
                edit_dialup_enable_save_button(FALSE);
                break;
            case editModeNormal_e:
                edit_overview_enable_save_button(TRUE);
                edit_wireless_enable_save_button(TRUE);
                edit_dialup_enable_save_button(TRUE);
            default:
                break;
        }
    }
}

// get/set the flags
gboolean edit_get_wizard_mode(void)
{
    CN_LOGPRINTF("return mode[%d]", g_wizard_mode);

    return g_wizard_mode;
}

void edit_set_wizard_mode(gboolean mode)
{
    CN_LOGPRINTF("entry: mode [%d]", mode);
    
    g_wizard_mode = mode;
}

// get/set the network settings
gboolean edit_get_in_set_network_settings(void)
{
    CN_LOGPRINTF("return [%d]", g_in_set_network_settings);

    return g_in_set_network_settings;
}

void edit_set_in_set_network_settings(gboolean value)
{
    CN_LOGPRINTF("entry [%d]", value);

    g_in_set_network_settings = value;
}

void edit_set_back_overview(gboolean back)
{
    CN_LOGPRINTF("entry [%d]", back);
    g_back_overview = back;
}

gboolean edit_get_back_overview(void)
{
    CN_LOGPRINTF("return %d", g_back_overview);
    return g_back_overview;
}

regNetworkProfile_t* edit_get_network_settings(void)
{
    regNetworkProfile_t* settings;
    editMode_t mode;

    CN_LOGPRINTF("entry");
    
    settings = g_new0(regNetworkProfile_t, 1);
    g_assert(settings != NULL);

    edit_overview_get_network_settings(settings);
    
    if (settings->connection == wireless_t)
    {
        mode = edit_get_mode();
        if (mode == editModeConnectFillIn_e)
        {
            edit_wireless_ext_get_network_settings(settings);
        }
        else
        {
            edit_wireless_get_network_settings(settings);
        }
    }
    else if (settings->connection == dialup_t)
    {
        edit_dialup_get_network_settings(settings);
    }

    if (settings->addressMode == static_t)
    {
        edit_static_get_network_settings(settings);
    }
    
    if (settings->proxy)
    {
        edit_proxy_get_network_settings(settings);
    }

    return settings;
}

void edit_set_network_settings(const regNetworkProfile_t* settings)
{
    editMode_t mode;

    CN_LOGPRINTF("entry");

    g_return_if_fail(settings != NULL);

    edit_set_in_set_network_settings(TRUE);

    CN_LOGPRINTF("settings->connection=%d", settings->connection);

    edit_overview_set_network_settings(settings);
 
    if (settings->connection == wireless_t)
    {
        mode = edit_get_mode();
        if (mode == editModeConnectFillIn_e)
        {
            edit_wireless_ext_set_network_settings(settings);
        }
        else
        {
            edit_wireless_set_network_settings(settings);
        }
    }
    else if (settings->connection == dialup_t)
    {
        edit_dialup_set_network_settings(settings);
    }

    edit_static_set_network_settings(settings);
    edit_proxy_set_network_settings(settings);

    edit_set_in_set_network_settings(FALSE);
}

void edit_restore_network_settings(int page)
{
    CN_LOGPRINTF("entry page[%d]", page);

    switch (page)
    {
        case editScreenProfiles_e:
        case editScreenSearch_e:
        case editScreenOverview_e:
            // back icon is grey out at these pages
            // no need to restore the settings
            break;
        case editScreenWireless_e:
            edit_wireless_restore_network_settings();
            break;
        case editScreenWirelessExt_e:
            edit_wireless_ext_restore_network_settings();
            break;
        case editScreenDialup_e:
            edit_dialup_restore_network_settings();
            break;
        case editScreenStatic_e:
            edit_static_restore_network_settings();
            break;
        case editScreenProxy_e:
            edit_proxy_restore_network_settings();
            break;
        case editScreenUndefined_e:
        default:
            break;
    }
}

// save the current profile setting
void edit_save_profile(void)
{
    int                  profileIndex = -1;
    regNetworkProfile_t* settings = NULL;
    networkProfile_t     network;

    CN_LOGPRINTF("entry");
     
    // get network settings displayed on screen 
    // and store these in the edit-screen data
    settings = edit_get_network_settings();
    g_return_if_fail(settings != NULL);

    network.regSettingsList = g_new0(regNetworkProfile_t*, 1);
    g_assert(network.regSettingsList != NULL);
    network.regSettingsList[0] = settings;
    network.regIndexList = NULL;
    network.nRegProfiles = 1;
    network.nActiveIndex = 0;

    // get the profileIndex to be saved
    profileIndex = edit_profiles_get_edited_profile_index();    

    // save the network profile into registry
    edit_data_set_network_profile(profileIndex, &network);

    // free the memory
    erRegFreeNetworkProfile(settings);
    g_free(network.regSettingsList);

    // save to registry
    edit_data_store_network_profiles();
}

// switch page management
gint edit_get_current_page(gint *pagecount)
{
    GtkNotebook *notebook = GTK_NOTEBOOK(g_edit_notebook);

    // Get number of pages, when requested.
    if (pagecount != NULL)
    {
        *pagecount = gtk_notebook_get_n_pages(notebook);
    }

    // Return current page number, counting from 0.
    return gtk_notebook_get_current_page(notebook);
}

// goto specified page, pages count from 0
void edit_goto_page(gint new_page)
{
    gint    current_page;
    gint    pagecount;

    current_page = edit_get_current_page(&pagecount);

    CN_LOGPRINTF("new_page %d - pagecount %d", new_page, pagecount);
    
    // Only call leave() functions when we really leave the current page
    if (current_page != new_page)
    {
        // finishing touch on current page
        switch (current_page)
        {
            case editScreenStatic_e:
                edit_static_screen_leave();
                break;
            case editScreenProxy_e:
                edit_proxy_screen_leave();
                break;
            default:
                /* ignore */ ;
        }
    }
    else
    {
        // We need to remove the blinking led
        // because there will be no page_change callback
        // of the GTK_NOTEBOOK
        erbusy_off();
    }

    // goto new page
    if (new_page >= 0  &&  new_page < pagecount)
    {
        CN_LOGPRINTF("switch to new_page %d", new_page);
        
        switch (new_page)
        {
            case editScreenOverview_e:
                edit_overview_show_profilename();
                break;
            case editScreenProfiles_e:
                edit_profiles_display();
                break;
            default:
                break;
        }

        gtk_notebook_set_current_page(GTK_NOTEBOOK(g_edit_notebook), 
                                      new_page);         
    }
}

// update the page bar
void edit_set_pagebar(void)
{
    // a little overkill, 
    // but this ensures the pageBar always has the correct currentApp
    pagebar_reset();   

    // For edit screen only show one page.
    // This forces the user to navigate using the on-screen items
    pagebar_set_pagecount( 1 );
    pagebar_goto_page( 1 );
    pagebar_redraw();
}

// toolbar and keyboard management
void edit_set_toolbar(void)
{
    CN_LOGPRINTF("entry");

    edit_show_back();
    edit_show_keyboard();
    edit_show_trashcan();
}

static void edit_show_back(void)
{
    screenPage_t curPage, backPage;

    CN_LOGPRINTF("entry");

    curPage.screen = cmgrScreenEdit_e;
    curPage.page = edit_get_current_page(NULL);
    
    cmgr_get_back_page(&curPage, &backPage);

    if ((backPage.screen == cmgrScreenUndef_e)
        || (backPage.page == -1))
    {
        toolbar_setIconState(iconID_back, iconState_grey);
    }
    else
    {
        toolbar_setIconState(iconID_back, iconState_normal);
    }
}

static void edit_show_keyboard(void)
{
    connection_t networkType;

    switch (edit_get_current_page(NULL))
    {
        case editScreenOverview_e:
            networkType = edit_data_get_network_type();
            if (networkType == wired_t)
            {
                show_keyboard(TRUE);
            }
            else
            {
                show_keyboard(FALSE);
            }
            break;
        case editScreenWireless_e:
        case editScreenWirelessExt_e:
        case editScreenDialup_e:
        case editScreenStatic_e:
        case editScreenProxy_e:
            show_keyboard(TRUE);
            break;
        case editScreenSearch_e:
        case editScreenProfiles_e:
            show_keyboard(FALSE);
            break;
        default:
            /* ignore */ ;
    }
}

void edit_show_trashcan(void)
{
    CN_LOGPRINTF("entry");

    switch (edit_get_current_page(NULL))
    {
        case editScreenProfiles_e:
            if (edit_data_get_n_network_profiles() > 0)
            {
                toolbar_setIconState(iconID_trashcan, iconState_normal);
            }
            else
            {
                toolbar_setIconState(iconID_trashcan, iconState_grey);
            }
            break;

        case editScreenSearch_e:
        case editScreenOverview_e:
        case editScreenWireless_e:
        case editScreenWirelessExt_e:
        case editScreenDialup_e:
        case editScreenStatic_e:
        case editScreenProxy_e:
            toolbar_setIconState(iconID_trashcan, iconState_grey);
            break;
        default:
            /* ignore */ ;
    }
}

// signal handlers 
static void on_edit_screen_destroy(GtkObject* object, gpointer data)
{
    CN_LOGPRINTF("entry");
    show_keyboard(FALSE);
}

static void on_edit_switch_page(GtkNotebook* notebook, 
                                GtkNotebookPage* new_page, 
                                guint page_num, 
                                gpointer data)
{
    CN_LOGPRINTF("Entry, new_page [%d]", (int)page_num);
    
    edit_set_pagebar();
    edit_set_toolbar();
}

void on_edit_goto_page(gint new_page)
{
    CN_LOGPRINTF("entry: %d", new_page);
    
    edit_goto_page(new_page - 1);  // pagebar counts from 1
}

gboolean on_edit_keypress(GdkEventKey* event)
{
    gboolean ret = FALSE;    // return FALSE => default gtk handling

    CN_LOGPRINTF("entry: keyval [0x%02X]", event->keyval);

    switch (edit_get_current_page(NULL) )
    {
        case editScreenProfiles_e:
            ret = on_edit_profiles_keypress(event);
            break;
        case editScreenSearch_e:
            ret = on_edit_search_keypress(event);
            break;
        case editScreenOverview_e:
            ret = on_edit_overview_keypress(event);
            break;
        case editScreenWireless_e:
            ret = on_edit_wireless_keypress(event);
            break;
        case editScreenWirelessExt_e:
            ret = on_edit_wireless_ext_keypress(event);
            break;
        case editScreenProxy_e:
            ret = on_edit_proxy_keypress(event);
            break;
        case editScreenStatic_e:
            ret = on_edit_static_keypress(event);
            break;
        case editScreenDialup_e:
            ret = on_edit_dialup_keypress(event);
            break;
        default:
            erbusy_off();
    }

    return ret;
}

void on_edit_icon_clicked(int iconID, int iconState)
{
    pingContext_t *ctxt;
    int page;

    CN_LOGPRINTF("entry iconID[%d] iconState[%d]", iconID, iconState);

    page = edit_get_current_page(NULL);
    switch (iconID)
    {
        case iconID_back:
            ctxt = cmgr_get_ping_ctxt();
            if (!connect_ping_in_pinging(ctxt)
                 && (iconState == iconState_normal))
            {
                toolbar_setIconState(iconID_back, iconState_selected);
                cmgr_add_on_toolbar_sync_action(cmgr_back_page);
                toolbar_synchronise();
            }
            break;
        case iconID_trashcan:
            if (page == editScreenProfiles_e)
            {
                on_edit_profiles_icon_clicked(iconID, iconState);
            }
            break;
        default:
            break;
    }
}

//
// Connect Ping
// 

int edit_get_initial_profile(void)
{
    CN_LOGPRINTF("entry");
    return 0;
}

int edit_get_n_network_profiles(void)
{
    CN_LOGPRINTF("entry");
    return 1;
}

const networkProfile_t* edit_get_network_profile(int profileIndex)
{
    regNetworkProfile_t* settings; 

    CN_LOGPRINTF("entry");
    if (g_nw_settings)
    {
        common_data_destroy_network_profiles(g_nw_settings, 1);
    }

    g_nw_settings = g_new0(networkProfile_t, 1);
    g_assert(g_nw_settings != NULL);

    settings = edit_get_network_settings();
    
    g_nw_settings->scanSettings = NULL;
   
    g_nw_settings->regSettingsList = g_new0(regNetworkProfile_t*, 1);
    g_nw_settings->regSettingsList[0] = (regNetworkProfile_t*)settings; 
    g_nw_settings->regIndexList = g_new0(int, 1);
    g_nw_settings->regIndexList[0] = -1;
    g_nw_settings->nRegProfiles = 1;
    g_nw_settings->nActiveIndex = 0;

    return (const networkProfile_t*)g_nw_settings;
}

gboolean edit_select_next_regprofile(int profileIndex)
{
    CN_LOGPRINTF("entry");
    return FALSE;
}

void edit_on_connected(void)
{
    int profileIndex;
    int nProfiles;
    int regIndex = -1;

    CN_LOGPRINTF("entry");

    // get the connected profile's index in the registry
    profileIndex = edit_profiles_get_edited_profile_index();
    CN_LOGPRINTF("profileIndex[%d]", profileIndex);

    if (profileIndex == -1)
    {
        // it's creating a new profile, 
        // the index in registry should be the last one
        nProfiles = common_data_get_n_profiles();
        if (nProfiles > 0)
        {
            regIndex = nProfiles - 1;
        }
    }
    else
    {
        regIndex = edit_data_get_reg_index(profileIndex); 
    }
    CN_LOGPRINTF("regIndex[%d]", regIndex);

    // set the last connected profile
    connect_data_set_last_connected_profile(regIndex);

    // start child process, if specified
    main_start_child_process();
}


