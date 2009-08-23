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
 * \file editScreenSearch.c
 * \brief connectionMgr - "Search or Manually" screen
 
 * Copyright (C) 2007 iRex Technologies BV.
 * 
 */

#include "config.h"
#include <string.h>
#include <gtk/gtk.h>
#include <libergtk/ergtk.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "erbusy.h"
#include "background.h"
#include "gtkSettingItem.h"
#include "gtkInfoItem.h"
#include "gtkProfileGroup.h"
#include "connectScreenData.h"
#include "editScreen.h"
#include "widgetUtils.h"
#include "editScreenProfiles.h"
#include "editScreenWireless.h"
#include "editScreenSearch.h"
#include "connectScan.h"
#include "connectionMgr.h"
#include "languages.h"

static GtkWidget *g_search = NULL;
static bkWdtLabelButton *g_options = NULL;
static GtkWidget *g_known_profiles = NULL;
static GtkWidget *g_info = NULL;

// the network profiles which will be shown on the edit search screen
static networkProfile_t *g_network_profiles = NULL;
static int               g_n_network_profiles = 0;

static GtkWidget *create_known_profiles (void);

static void on_options_update(GtkWidget *selection, 
                              gpointer button, 
                              gpointer data);
static void on_profile_entry_selected (GtkProfileGroup *profileGroup, 
                                       guint profileIndex);

static int edit_search_get_n_network_profiles(void);
static const networkProfile_t *edit_search_get_network_profile(int profileIndex);
static void edit_search_destroy_network_profiles(void);

static void edit_search_init_network_profiles(connection_t networkType, 
                                          network_spec_t *pScanNetworks, 
                                          int nScanNetworks);
static void edit_search_scan_networks_done(void);

static void edit_search_restore_scan_ctxt(void);

// object hierarchy
// |--item (gtkSettingItem)
//    |--vbox
//       |--options     (bkWdtLabelButton)
//       |--profilelist (gtkProfileGroup)
//       |--info        (gtkInfoItem)
//
GtkWidget *edit_search_create(void)
{
    GtkWidget *item;
    GtkWidget *vbox;
    bkWdtLabelButton *options;
    GtkWidget *profilelist;
    GtkWidget *info;
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    // item
    item = gtk_settingitem_new("");

    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), vbox);

    // options
    options = bk_wdt_label_button_new(2, FALSE, 0, 1);
    gtk_box_pack_start(GTK_BOX(vbox), options->background, FALSE, FALSE, 0);
    
    // profilelist
    profilelist = create_known_profiles();
    gtk_box_pack_start(GTK_BOX(vbox), profilelist, FALSE, FALSE, 0);
    
    // info
    info = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), info, FALSE, FALSE, 0);

    // signal handlers
    widget = options->details->selection;
    g_signal_connect (G_OBJECT (widget), "selection-update", 
                      G_CALLBACK (on_options_update), NULL);

    // show widgets
    gtk_widget_show(item);
    gtk_widget_show(vbox);
    gtk_widget_show(options->background);
    gtk_widget_show(profilelist);
    gtk_widget_show(info);

    // global variables
    g_search = item;
    g_options = options;
    g_info = info;

    CN_LOGPRINTF("done");

    // return
    return item;
}

//
//|--item(GtkProfileGroup)
//
static GtkWidget *create_known_profiles (void)
{
    GtkWidget* item;
   
    CN_LOGPRINTF("entry");

    // item
    item = gtk_profile_group_new(MAX_PROFILES_PER_PAGE, connectMode_e);
    gtk_profile_group_show_dialup(GTK_PROFILE_GROUP(item), FALSE);
    gtk_profile_group_callback_on_buttons_clicked(GTK_PROFILE_GROUP(item),
                                          on_profile_entry_selected, NULL);
    g_known_profiles = item;

    return item;
}

void edit_search_set_text(void)
{
    GtkProfileGroup *profileGroup;
    GtkWidget *widget = NULL;

    CN_LOGPRINTF("entry");

    if (g_search)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_search), 
                                        _("Create new wireless network"));
    }

    if (g_options)
    {
        widget = g_options->details->label;
        gtk_label_set_text(GTK_LABEL(widget),
                _("Select a way to create the network."));

        widget = GTK_WIDGET(g_options->details->buttonList[0]);
        gtk_button_set_label(GTK_BUTTON(widget), _("Search"));

        widget = GTK_WIDGET(g_options->details->buttonList[1]);
        gtk_button_set_label(GTK_BUTTON(widget), _("Manually"));
    }
    
    if (g_known_profiles)
    {
        profileGroup = GTK_PROFILE_GROUP(g_known_profiles);
        gtk_profile_group_set_header_text(profileGroup,
                                          _("Network profiles"));
    }

    if (g_info)
    {
        gtk_infoitem_set_text(GTK_INFOITEM(g_info),
        _("When you click on 'Search' the device will scan for networks.\n"
          "Clicking 'Manually' requires all the settings."));
    }
}

static void edit_search_update_profilelist_info(scanStatus_t status)
{
   GtkProfileGroup *item;
   char            *info;

   CN_LOGPRINTF("entry");

   if (g_known_profiles)
   {
       item = GTK_PROFILE_GROUP(g_known_profiles);
       switch (status)
       {
           case scanRunning_e:
               info = _("Please wait while the iLiad scans "
                        "for wireless networks ....");
               break;
           case scanAborting_e:
               info = _("Aborting scan ...");
               break;
           case rescanNeed_e:
               info =  _("No wireless networks detected.");
               break;
           case scanOk_e:
               info =  _("Please select the network you wish to create.");
               break;
           default:
               info = "";
               break;
       }
       
       gtk_profile_group_set_info_text(item, info);
   }
}

static void edit_search_display_profilelist(void)
{
    int i, nProfiles, nDisplayed, max;
    GtkProfileGroup *profileGroup;
    const networkProfile_t *settings;

    g_return_if_fail(g_known_profiles != NULL);
    
    nProfiles = edit_search_get_n_network_profiles();
    max = MAX_PROFILES_PER_PAGE;
    nDisplayed = ((nProfiles < max) ? nProfiles: max);

    profileGroup = GTK_PROFILE_GROUP(g_known_profiles);

    // display the profile list
    if (nDisplayed > 0)
    {
        for (i = 0; i < nDisplayed; i++)
        {
            settings = edit_search_get_network_profile(i);
            gtk_profile_group_display_settings(profileGroup, i, 
                                               settings, TRUE);
        }
    }
    gtk_profile_group_show_profile_entries(profileGroup, nDisplayed);
}

void edit_search_restore_screen(void)
{
    GtkToggleButton *button;

    // Search and Manually buttons
    button = g_options->details->buttonList[0];
    gtk_toggle_button_set_active(button, FALSE);

    button = g_options->details->buttonList[0];
    gtk_toggle_button_set_active(button, FALSE);

    // the profilelist
    edit_search_destroy_network_profiles();
    edit_search_display_profilelist();
    edit_search_update_profilelist_info(undefScanStatus_e);
}

static gboolean on_delayed_search(gpointer data)
{
    scanContext_t       *ctxt;

    erbusy_blink();

    edit_search_restore_scan_ctxt();
    ctxt = cmgr_get_scan_ctxt();
    connect_scan(ctxt, FALSE);

    return FALSE;
}

static gboolean on_delayed_manually(gpointer data)
{
    char            *profilename;
    GtkToggleButton *button;
      
    erbusy_blink();
       
    edit_wireless_set_ssid("");
    edit_wireless_set_encrtype(encr_undefined_t);
 
    profilename = edit_profiles_default_profilename();
    edit_wireless_set_profilename(profilename);
    g_free(profilename);

    // backup the wireless settings
    edit_wireless_backup_network_settings();

    delay_edit_wireless_determine_status();

    edit_goto_page(editScreenWireless_e);

    // de-active the button
    button = g_options->details->buttonList[1];
    gtk_toggle_button_set_active(button, FALSE);

    return FALSE;
}

static void on_options_update(GtkWidget *selection, 
                              gpointer button, 
                              gpointer data)
{
    erGtkSelectionGroup *item;
    int                  nsel;

    CN_LOGPRINTF("entry");

    item = ERGTK_SELECTION_GROUP(selection);
    ergtk_selection_group_get_selected_buttons(item, &nsel, 1);
    switch (nsel)
    {
        case 0: // 'Search' button is selected
            // empty the old stuff before searching again
            edit_search_destroy_network_profiles();
            edit_search_display_profilelist();
            edit_search_update_profilelist_info(scanRunning_e);
            g_timeout_add(500, on_delayed_search, NULL);
            break;
        case 1:
            // 'Manually' button is selected
            g_timeout_add(500, on_delayed_manually, NULL);
            break;
        default:
            break;
    }
}

static gboolean on_delayed_profile_entry_selected(gpointer data)
{
    int profileIndex = (int)data;
   
    const networkProfile_t *pNetwork = NULL;
    network_spec_t *pScan = NULL; 
 
    const gchar *ssid = "";
    wireless_encr_type_t encrtype = encr_undefined_t;

    GtkProfileGroup *profileGroup;

    CN_LOGPRINTF("entry");

    erbusy_blink();

    if ((profileIndex >= 0) && (profileIndex < g_n_network_profiles))
    {
        pNetwork = edit_search_get_network_profile(profileIndex);
        pScan = pNetwork->scanSettings;

        // ssid
        ssid = pScan->ssid;
        edit_wireless_set_ssid(ssid);

        encrtype = pScan->encryption;
        edit_wireless_set_encrtype(encrtype);

        // profilename
        edit_wireless_set_profilename(ssid);

        // backup the wireless settings
        edit_wireless_backup_network_settings();

        delay_edit_wireless_determine_status();
        
        edit_goto_page(editScreenWireless_e);

        profileGroup = GTK_PROFILE_GROUP(g_known_profiles);
        gtk_profile_group_unselect_profile(profileGroup, profileIndex);  
    }

    return FALSE;
}

static void on_profile_entry_selected (GtkProfileGroup* profileGroup, 
                                       guint profileIndex)
{
    CN_LOGPRINTF ("entry");

    // wait for a while to show button selected
    g_timeout_add(500, 
            on_delayed_profile_entry_selected, 
            (gpointer)profileIndex);
}

static void edit_search_restore_scan_ctxt(void)
{
    scanContext_t *ctxt;
    char *ssidList;

    CN_LOGPRINTF("entry");

    ctxt = cmgr_get_scan_ctxt(); 
    if (ctxt)
    {
        connect_scan_ctxt_set_mode(ctxt, editScan_e);
        connect_scan_ctxt_set_network_type(ctxt, wireless_t);
        connect_scan_ctxt_set_done_callbacks(ctxt, 
                         edit_search_init_network_profiles, 
                         edit_search_scan_networks_done);
       
        ssidList = connect_data_get_hidden_ssid_list();
        connect_scan_ctxt_set_ssidlist(ctxt, ssidList);
        g_free(ssidList);
    }
}

static void edit_search_init_network_profiles(connection_t networkType, 
                                       network_spec_t *pScanNetworks, 
                                       int nScanNetworks)
{
    int i;
    networkProfile_t *pNetwork;

    CN_LOGPRINTF("entry");

    // empty the old results
    edit_search_destroy_network_profiles();

    // calculate the network profiles
    if (pScanNetworks && (nScanNetworks != 0))
    {
        g_n_network_profiles = nScanNetworks;
        g_network_profiles = g_new0(networkProfile_t, nScanNetworks);
        g_return_if_fail(g_network_profiles != NULL);

        for (i = 0; i < nScanNetworks; i++)
        {
            pNetwork = &g_network_profiles[i];
            pNetwork->scanSettings = dup_network_spec(&pScanNetworks[i]);
            pNetwork->regSettingsList = NULL;
            pNetwork->regIndexList = NULL;
            pNetwork->nRegProfiles = 0;
            pNetwork->nActiveIndex = -1;
        }
    }
}

static void edit_search_destroy_network_profiles(void)
{
    CN_LOGPRINTF("entry");

    common_data_destroy_network_profiles(g_network_profiles, 
                                         g_n_network_profiles);
    g_network_profiles = NULL;
    g_n_network_profiles = 0;
}

static int edit_search_get_n_network_profiles(void)
{
    CN_LOGPRINTF("entry [%d]", g_n_network_profiles);

    return g_n_network_profiles;
}

static const networkProfile_t *edit_search_get_network_profile(int profileIndex)
{
    CN_LOGPRINTF("entry profileIndex[%d]", profileIndex);

    networkProfile_t *ret = NULL;

    g_return_val_if_fail(g_network_profiles != NULL, NULL);
    g_return_val_if_fail(profileIndex < g_n_network_profiles, NULL);
    
    ret = &g_network_profiles[profileIndex];

    CN_LOGPRINTF("ret=%p", ret);

    return ret;
}

static void edit_search_scan_networks_done(void)
{
    int nProfiles;
    GtkToggleButton *button;
   
    CN_LOGPRINTF("entry");

    edit_search_display_profilelist();

    nProfiles = edit_search_get_n_network_profiles();
    if (nProfiles > 0)
    {
        // update the information text
        edit_search_update_profilelist_info(scanOk_e);
    }
    else
    {
        edit_search_update_profilelist_info(rescanNeed_e);
    }

    // de-active the 'Search' button when scan done
    if (g_options)
    {
        button = g_options->details->buttonList[0];
        gtk_toggle_button_set_active(button, FALSE);
    }

    display_update_request_screen_refresh(STATUS_ITEM_CHANGE, 
                                          WAVEFORM_TYPING);
}

gboolean on_edit_search_keypress(GdkEventKey *event)
{
    GtkToggleButton *button;
    gboolean ret = FALSE;

    CN_LOGPRINTF("entry");
    
    switch (event->keyval)
    {
        case GDK_Return:
            // 'Enter' key is pressed
            // simulate clicking the 'Manually' button
            if (g_options)
            {
                button = g_options->details->buttonList[1];
                gtk_toggle_button_set_active(button, TRUE);
            }
            ret = TRUE;
            break;

        case GDK_F5:
            // 'UP' key is pressed
            // goto the one top level page
            cmgr_up_page();
            ret = TRUE;
            break;

        case GDK_Down:
        case GDK_Up:
        case GDK_Page_Down:
        case GDK_Page_Up:
        default:
            erbusy_off();
            break;
    }

    return ret;
}

