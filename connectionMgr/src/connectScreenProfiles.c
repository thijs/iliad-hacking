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
 * \file connectScreenProfiles.c
 * \brief connectionMgr - the second page of 'connect-to-server' screen
 *                        the connection profiles screen
 *                        containing a profile list and a new profile entry
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#include <unistd.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <liberdm/connectionMgrConstants.h>
#include <libergtk/ergtk.h>
#include <liberipc/eripctoolbar.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "background.h"
#include "gtkSettingItem.h"
#include "gtkInfoItem.h"
#include "gtkProfileEntry.h"
#include "gtkProfileGroup.h"
#include "widgetUtils.h"
#include "connectScreenData.h"
#include "connectionMgr.h"
#include "connectScreen.h"
#include "connectScreenOptions.h"
#include "connectScreenProfiles.h"
#include "connectPing.h"
#include "connectScan.h"
#include "connectBackground.h"
#include "editScreenData.h"
#include "editScreen.h"
#include "modemUtils.h"
#include "erbusy.h"
#include "pagebar.h"
#include "toolbar.h"
#include "languages.h"

// vbox in which all connect-screen-profiles items are packed
static GtkWidget *g_connect_profiles = NULL;

// available network profile list
static GtkWidget *g_known_profiles = NULL;

// create a new profile
static GtkWidget *g_profile_editor = NULL;
static bkWdtLabelButton *g_label_button = NULL;

// information
static GtkWidget* g_info = NULL;

// internally used functions
static GtkWidget *create_known_profiles (void);
static GtkWidget *create_profile_editor (void);
static GtkWidget *create_info(void);

// signal handlers
static gboolean on_profile_entry_button_press(GtkProfileGroup* profileGroup, 
                                              guint profileIndex,
                                              gboolean active);

static void on_profile_editor_toggled (GtkWidget* button,
                                       GdkEventButton * event,
                                       gpointer data);

static gboolean delay_profile_entry_selected(gpointer data);
static void connect_profiles_reset_active_reg_index(void);
static gboolean connect_profiles_start_fillin_profile(void);
static gboolean on_delay_start_fillin_profile(gpointer data);
static void connect_profiles_update_new_profile(void);
static gboolean delayed_on_profile_editor_toggled(gpointer data);
static void connect_profiles_on_failed_all(void);

////////////////////////////////////////////////////////////////////////////
// create GUI widgets
////////////////////////////////////////////////////////////////////////////

// object hierarchy:
//     g_pingParms.pingStatus (gtkLabel)
//     g_connect_profiles (vbox)
//       |--g_known_profiles (GtkProfileGroup)
//       |--g_profile_editor(GtkSettingsItem)
//       |--g_info(gtkInfoItem)
// 
GtkWidget *connect_profiles_create (void)
{
    CN_LOGPRINTF ("entry");

    GtkWidget* vbox;
    GtkWidget* itemNetworks;
    GtkWidget* itemNewProfile;
    GtkWidget* itemInfo;

    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_widget_show(vbox);
    g_connect_profiles = vbox;

    // itemNetworks
    itemNetworks = create_known_profiles();
    gtk_box_pack_start(GTK_BOX(vbox), itemNetworks, FALSE, FALSE, 0);
    gtk_widget_show(itemNetworks);

    // itemNewProfile
    itemNewProfile = create_profile_editor();
    gtk_box_pack_start(GTK_BOX(vbox), itemNewProfile, FALSE, FALSE, 0);
    gtk_widget_show(itemNewProfile);

    // itemInfo
    itemInfo = create_info();
    gtk_box_pack_start(GTK_BOX(vbox), itemInfo, FALSE, FALSE, 0);
    gtk_widget_show(itemInfo);

    return g_connect_profiles;
}

//
//|--item(GtkProfileGroup)
//
static GtkWidget *create_known_profiles (void)
{
    GtkWidget* item;
    
    // item
    item = gtk_profile_group_new(MAX_PROFILES_PER_PAGE, connectMode_e);
    gtk_profile_group_show_dialup(GTK_PROFILE_GROUP(item), FALSE);
    gtk_profile_group_callback_on_buttons_press(GTK_PROFILE_GROUP(item), 
                                    on_profile_entry_button_press, NULL);
    g_known_profiles = item;

    return item;
}

//
// |--item(GtkSettingsItem)
//    |--widget (bkWdtLabelButton)
static GtkWidget *create_profile_editor (void)
{
    GtkWidget *item;
    bkWdtLabelButton *widget;
    GtkToggleButton  *button;

    // item
    item = gtk_settingitem_new("");

    // widget
    widget = bk_wdt_label_button_new(1, FALSE, 0, 1);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), widget->background);

    // signal handlers
    button = widget->details->buttonList[0];
    g_signal_connect(G_OBJECT(button), "toggled", 
                 G_CALLBACK(on_profile_editor_toggled), 0);

    // show widget
    gtk_widget_show(widget->background);

    // global variable
    g_profile_editor = item;
    g_label_button = widget;

    // return
    return item;
}

//
// |-item(gtkInfoItem)
//
static GtkWidget *create_info(void)
{
    GtkWidget* item;

    item = gtk_infoitem_new(FALSE);
    g_info = item;
    return item;
}

////////////////////////////////////////////////////////////////////////////
// set the correct text on screen
////////////////////////////////////////////////////////////////////////////
void connect_profiles_set_text (void)
{
    GtkProfileGroup *profileGroup;
    GtkWidget *widget;

    CN_LOGPRINTF ("entry");

    if (g_known_profiles)
    {
        profileGroup = GTK_PROFILE_GROUP(g_known_profiles);
        gtk_profile_group_set_dialup_text(profileGroup,
                        _("Signal strength of network:"));
    }

    if (g_profile_editor)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_profile_editor), 
                                        _("Setup a network profile"));
    }
    
    if (g_label_button)
    {
        widget = g_label_button->details->label;
        gtk_label_set_text (GTK_LABEL(widget),
                            _("You can create or edit a network profile "
                              "by clicking the button below."));

        widget = GTK_WIDGET(g_label_button->details->buttonList[0]);
        gtk_button_set_label (GTK_BUTTON(widget), _("Profile editor"));
    }
}

////////////////////////////////////////////////////////////////////////////
// display the related fields
////////////////////////////////////////////////////////////////////////////
static void connect_profiles_update_info(void)
{
    connection_t networkType;
    char* cpTitle = NULL, *cpDetails = NULL;
    char* cpInfo = NULL;
    GtkProfileGroup *profileGroup;
   
    networkType = connect_data_get_network_type();
    switch(networkType)
    {
        case wired_t:
            cpTitle = _("Available wired networks");
            cpDetails = _("No wired profiles exist, "
                          "please use the button below "
                          "to create a profile.");
            cpInfo = _("You need to connect your travel hub "
                       "and ethernet cable "
                       "to your iLiad.\n"
                       "The default profile uses DHCP and no proxy.");
            break;
        case dialup_t:
            cpTitle = _("Available dial-up networks");
            cpDetails = _("No dial-up profiles exist, "
                          "please use the button below "
                          "to create a profile.");
            cpInfo =  _("In order to use a dial-up network, "
                        "you need a CF modem.\n"
                        "Bluetooth dial-up connections "
                        "are not yet supported.");
            break;
        case wireless_t:
            cpTitle = _("Available wireless networks");
            cpDetails = _("No wireless profiles exist, "
                          "please use the button below "
                          "to create a profile.");
            cpInfo =  _("When the access point is secured with "
                        "an encryption key,"
                        " click on the corresponding network button "
                        "to open the configuration screen.\n"
                        "By clicking the 'Profile editor' button "
                        "you can create a network profile"
                        " for a non broadcasting (hidden) access point.");
            break;
        default:
            CN_WARNPRINTF("Invalid network type[%d].", networkType);
            break;
    }

    // show the information wiget with the correct text 
    if (g_known_profiles)
    {
        profileGroup = GTK_PROFILE_GROUP(g_known_profiles);
        
        gtk_profile_group_set_header_text(profileGroup, cpTitle);
        
        if (connect_data_get_n_network_profiles())
        {
            gtk_profile_group_set_info_text(profileGroup,
                                _("Accept the automatic search "
                                  "or select the network of your choice."));
        }
        else
        {
            gtk_profile_group_set_info_text(profileGroup, cpDetails);
        }
    }

    if (g_info)
    {
        gtk_infoitem_set_text(GTK_INFOITEM(g_info), cpInfo);
    }
}

void connect_profiles_display (void)
{
    CN_LOGPRINTF ("entry");
    
    GtkProfileGroup           *profileGroup;
    const networkProfile_t    *settings;
    int                       i, profileIndex;
    int                       nProfiles, nDisplayed;
    int                       quality;
    GtkToggleButton           *button;
 
    connect_profiles_update_info();

    // show the accurate profile entries
    profileGroup = GTK_PROFILE_GROUP(g_known_profiles);   
 
    // show dialup signal widgets or not
    switch (connect_data_get_network_type())
    {
        case wired_t:
        case wireless_t:
            gtk_profile_group_show_dialup(profileGroup, FALSE);
            break;
        case dialup_t:
            quality = get_modem_signal_strength() * 100 / 32;
            gtk_profile_group_set_dialup_signal_image(profileGroup, quality);
            gtk_profile_group_show_dialup(profileGroup, TRUE);
            break;
        default:
            break;
    }

    // display the profile list
    nProfiles = connect_data_get_n_network_profiles();
    nDisplayed = ((nProfiles < MAX_PROFILES_PER_PAGE) ? nProfiles: MAX_PROFILES_PER_PAGE);

    if (nDisplayed > 0)
    {
        for (i = 0; i < nDisplayed; i++)
        {
            settings = connect_data_get_network_profile(i);
            gtk_profile_group_display_settings(profileGroup, i, 
                                               settings, TRUE);
        }
    }
    gtk_profile_group_show_profile_entries(profileGroup, nDisplayed);

    // deselect all profile index
    profileIndex = gtk_profile_group_get_selected_profile_index(profileGroup);
    if (profileIndex != -1)
    {
        gtk_profile_group_unselect_profile(profileGroup, profileIndex);
    }

    // deselect the 'profile editor' button   
    if (g_label_button)
    {
        button = g_label_button->details->buttonList[0];
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
    }
}

void connect_profiles_select_profile(int profileIndex)
{
    CN_LOGPRINTF("entry");
    
    GtkProfileGroup*  profileGroup;

    profileGroup = GTK_PROFILE_GROUP(g_known_profiles);
    gtk_profile_group_select_profile(profileGroup, profileIndex);
    display_update_request_screen_refresh(STATUS_ITEM_CHANGE, 
                                          WAVEFORM_TYPING);
}

void connect_profiles_unselect_profile (void)
{
    CN_LOGPRINTF ("entry");

    GtkProfileGroup *profileGroup;
    gint             profileIndex;

    profileGroup = GTK_PROFILE_GROUP(g_known_profiles);  
    profileIndex = gtk_profile_group_get_selected_profile_index(profileGroup);
    if (profileIndex != -1)
    {
        gtk_profile_group_unselect_profile(profileGroup, profileIndex);
    }
}

void connect_profiles_update_status(int profileIndex, pingStatus_t status)
{
    CN_LOGPRINTF("entry");
    
    int               nProfiles;
    GtkProfileGroup*  profileGroup;
    gchar*            text;
    gboolean          flashing = FALSE;

    nProfiles = connect_data_get_n_network_profiles();
    if ((profileIndex >= 0) && (profileIndex < nProfiles))
    {
        switch (status)
        {
            case pingSucceeded_e:
                text = _("Connected");
                break;
            case pingFailed_e:
            case pingFailedPCShare_e:
            case pingFailedNetwork_e:
                text = _("Unsuccessful");
                break;
            case pingSkipped_e:
                text = _("Skipped");
                break;
            case pingNeedKey_e:
                text = _("Security key required");
                break;
            case pingAborted_e:
                text = _("Aborted");
                break;
            case pingAborting_e:
                text = _("Aborting...");
                flashing = TRUE;
                break;
            case pingConnecting_e:
                text = _("Connecting...");
                flashing = TRUE;
                break;
            default:
                text = "";
                break;
        }

        profileGroup = GTK_PROFILE_GROUP(g_known_profiles);
        gtk_profile_group_set_status_text(profileGroup, profileIndex, 
                                          text, flashing);
        display_update_request_screen_refresh (STATUS_ITEM_CHANGE, 
                                               WAVEFORM_TYPING);
    }
}

void connect_profiles_display_settings(int profileIndex, 
                                       gboolean resetStatus)
{
    GtkProfileGroup           *profileGroup;
    const networkProfile_t    *settings;
    int max, nProfiles, nDisplayed;
    
    CN_LOGPRINTF("entry profileIndex[%d]", profileIndex);

    max = MAX_PROFILES_PER_PAGE;
    nProfiles = connect_data_get_n_network_profiles();
    nDisplayed = ((nProfiles < max) ? nProfiles: max);
    if (nDisplayed > 0)
    {
        if ((profileIndex >= 0) && (profileIndex < nDisplayed))
        {
            profileGroup = GTK_PROFILE_GROUP(g_known_profiles);
            settings = connect_data_get_network_profile(profileIndex);
            gtk_profile_group_display_settings(profileGroup, profileIndex, 
                                               settings, resetStatus);
            display_update_request_screen_refresh(STATUS_ITEM_CHANGE, 
                                                  WAVEFORM_TYPING);
        }
    }
}

////////////////////////////////////////////////////////////////////////////
// signal handlers 
////////////////////////////////////////////////////////////////////////////

// call this function, when user uses stylus to select one profile
// select the profile he wants to start pinging
// or open the corresponding profile editor to input the encryption key 
//
// notes, 
// if the profile is in pinging press, 
// nothing happens but keep the button in selected status
static gboolean on_profile_entry_button_press(GtkProfileGroup* profileGroup, 
                                              guint profileIndex,
                                              gboolean active)
{
    pingContext_t* ctxt;
    gint pingingProfileIndex;

    CN_LOGPRINTF("entry");
    
    g_return_val_if_fail(IS_GTK_PROFILE_GROUP(profileGroup), FALSE);
    g_return_val_if_fail (profileIndex != -1, FALSE);
    
    ctxt = cmgr_get_ping_ctxt();
    if (connect_ping_freeze_ui(ctxt))
    {
        CN_WARNPRINTF("Please wait when freezing UI...");
        return TRUE;
    }

    // note: current state is de-active means it's going to be active
    if (active)
    {
        pingingProfileIndex = connect_ping_get_pinging_profile_index(ctxt);
        if (profileIndex == pingingProfileIndex)
        {
            CN_LOGPRINTF("button-press-event happens "
                         "on the current pinging button[%d]",
                         pingingProfileIndex);
            // return TRUE to prevent 
            // the standard button-press-event handler
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        // if already pinging, abort pinging firstly
        connect_ping_abort(ctxt);
       
        // wait for ping done, then further actions
        g_timeout_add(200, 
                delay_profile_entry_selected, 
                (gpointer)profileIndex);

        // return TRUE, to prevent standard button-press-event handler
        // select this button in delay_profile_entry_selected later
        return TRUE;
    }
}

static gboolean delay_profile_entry_selected(gpointer data)
{
    pingContext_t* ctxt;
    guint profileIndex = (guint)data;

    ctxt  = cmgr_get_ping_ctxt();
    if (connect_ping_freeze_ui(ctxt))
    {
        CN_WARNPRINTF("call me later...");
        return TRUE;  // TRUE = please call again
    }

    connect_profiles_reset_active_reg_index();

    // set the profile index of starting to ping or to edit
    connect_ping_set_pinging_profile_index_start(ctxt, profileIndex);

    // set the selected profile index
    connect_ping_set_selected_profile_index(ctxt, profileIndex);

    // update the ui
    connect_profiles_select_profile(profileIndex);

    if (!connect_profiles_start_fillin_profile())
    {
        // connecting using this profile
        connect_profiles_restore_ping_ctxt(); 
        connect_ping(ctxt, TRUE);
    }

    return FALSE;  // FALSE = don't call me again
}

static void connect_profiles_reset_active_reg_index(void)
{
    int i, nProfiles;
    networkProfile_t* settings = NULL;

    nProfiles = connect_data_get_n_network_profiles();
    for (i = 0; i < nProfiles; i++)
    {
        settings = (networkProfile_t*)connect_data_get_network_profile(i);
        g_return_if_fail(settings != NULL);
        if (settings->nRegProfiles > 0
            && settings->regSettingsList
            && settings->regIndexList
            && ((settings->nActiveIndex > 0) 
            && (settings->nActiveIndex < settings->nRegProfiles)))
        {
            settings->nActiveIndex = 0;
            connect_profiles_display_settings(i, FALSE);
        }
    }
}

// the selected profile
// (1) If it has corresponding entry in registry, nothing need to do.
// (2) If it has no corresponding entry in registry and it's non-secured, 
//     new a profile for it.
// (3) If it has no corresponding entry in registry and it's secured, 
//     launch the profile editor to fill in the encryption.
// return: TRUE  needn't open the profile editor, it can be used by ping
//         FALSE open the profile editor to fill in the encryption key
static gboolean connect_profiles_start_fillin_profile(void)
{
    pingContext_t *ctxt;
    gint profileIndex;
    const networkProfile_t* settings;
    regNetworkProfile_t *regSettings;
    connection_t networkType;
    gboolean needEdit = FALSE;
    GtkProfileGroup *profileGroup;

    CN_LOGPRINTF("entry");

    ctxt = cmgr_get_ping_ctxt();
    profileIndex = connect_ping_get_selected_profile_index(ctxt);
    g_return_val_if_fail(profileIndex != -1, FALSE);

    settings = connect_data_get_network_profile(profileIndex);
    g_return_val_if_fail(settings != NULL, FALSE);

    if ( (settings->nRegProfiles == 0) && settings->scanSettings)
    {
        if (settings->scanSettings->encryption == encr_none_t)
        {
            CN_LOGPRINTF("The %dth profile needs to save before ping.", 
                          profileIndex);
            if (!connect_data_reach_max_profiles())
            {
                // set the edit mode
                edit_set_mode(editModeConnectCreate_e);

                regSettings = network_spec_to_reg_network(settings->scanSettings);
                
                // set the network type for edit
                networkType = regSettings->connection;
                edit_data_set_network_type(networkType);
           
                // initilize the edit data
                edit_data_init_network_profiles();

                // set the edit mode to be wizard
                edit_set_wizard_mode(TRUE);

                // set the fields of widgets
                edit_set_network_settings(regSettings);

                // save it
                edit_save_profile();
                
                // update
                connect_profiles_update_new_profile();

                erRegFreeNetworkProfile(regSettings);
            }
        }
        else
        {
            CN_LOGPRINTF("The %dth profile needs encryption key.", 
                          profileIndex);

            if (!connect_data_reach_max_profiles())
            {
                needEdit = TRUE;
                
                profileGroup = GTK_PROFILE_GROUP(g_known_profiles);
                gtk_profile_group_set_status_text(profileGroup, 
                                             profileIndex, 
                                             _("Open network profile..."), 
                                             FALSE);
                display_update_request_screen_refresh(STATUS_ITEM_CHANGE, 
                                                      WAVEFORM_TYPING);

                g_timeout_add(500, 
                        on_delay_start_fillin_profile, 
                        (gpointer)settings);
            }
        }
    }

    return needEdit;
}

static gboolean on_delay_start_fillin_profile(gpointer data)
{
    networkProfile_t* settings;
    regNetworkProfile_t* regSettings;
    connection_t networkType;
    
    settings = (networkProfile_t*)data;
    if (settings)
    {
        CN_LOGPRINTF("Opening the profile editor...");

        cmgr_set_screen_mode(cmgrScreenEdit_e);

        // set the edit mode
        edit_set_mode(editModeConnectFillIn_e);
 
        regSettings = network_spec_to_reg_network(settings->scanSettings);
       
        // set the network type for edit
        networkType = regSettings->connection;
        edit_data_set_network_type(networkType);
   
        // initilize the edit data
        edit_data_init_network_profiles();

        // set the edit mode to be wizard
        edit_set_wizard_mode(TRUE);
        edit_set_back_overview(FALSE);

        // set the fields of widgets
        edit_set_network_settings(regSettings);
        
        // go to the wireless ext page 
        edit_goto_page(editScreenWirelessExt_e);
       
        erRegFreeNetworkProfile(regSettings);
        regSettings = NULL;
    }

    return FALSE;
}

static void connect_profiles_update_new_profile(void)
{
    networkProfile_t *settings;
    pingContext_t *ctxt;
    gint profileIndex;

    CN_LOGPRINTF("entry");

    ctxt = cmgr_get_ping_ctxt();
    profileIndex = connect_ping_get_selected_profile_index(ctxt);
    g_return_if_fail(profileIndex != -1);

    settings = (networkProfile_t*)connect_data_get_network_profile(profileIndex);
    g_return_if_fail(settings != NULL);

    if ( (settings->nRegProfiles == 0) && settings->scanSettings)
    {
        connect_data_update_network_profile_with_last(settings);
    }
}

static void on_profile_editor_toggled (GtkWidget* button,
                                       GdkEventButton * event,
                                       gpointer data)
{
    gboolean active;
    pingContext_t* ctxt;

    CN_LOGPRINTF("entry");

    // check if it is a "active event"
    active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));
    ctxt = cmgr_get_ping_ctxt();

    if (connect_ping_freeze_ui(ctxt))
    {
        CN_WARNPRINTF("Please wait when freezing UI...");
        if (active)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
        }
        return;
    }
    
    if (active)
    {
        // if already pinging, abort pinging firstly
        connect_ping_abort(ctxt);

        // show button selected, then start to launch profile editor
        g_timeout_add(500, delayed_on_profile_editor_toggled, data);
    }   
}

static gboolean delayed_on_profile_editor_toggled(gpointer data)
{
    pingContext_t* ctxt;
    connection_t networkType;
   
    erbusy_blink();

    ctxt = cmgr_get_ping_ctxt();
    if (connect_ping_freeze_ui(ctxt))
    {
        CN_LOGPRINTF("call me later...");
        return TRUE;
    }

    // set the connection mgr mode
    cmgr_set_screen_mode(cmgrScreenEdit_e);

    // set the edit mode
    edit_set_mode(editModeConnectNormal_e);

    // set the network type for edit
    networkType = connect_data_get_network_type();
    edit_data_set_network_type(networkType);
    
    // initilize the edit data
    edit_data_init_network_profiles();

    // go to the edit profiles page
    edit_goto_page(editScreenProfiles_e);

    // screen refresh
    display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL, 
                                          WAVEFORM_FULLSCREEN);

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////
// key event handlers
////////////////////////////////////////////////////////////////////////////
gboolean on_connect_profiles_keypress (GdkEventKey * event)
{
    pingContext_t *ctxt;
    gboolean ret = FALSE;

    CN_LOGPRINTF ("entry");

    switch (event->keyval)
    {
        case GDK_Return:
            // 'Enter' key is pressed
            ret = TRUE;
            break;

        case GDK_F5:
            // 'UP' key is pressed
            // goto the one top level page
            ctxt = cmgr_get_ping_ctxt();
            connect_ping_abort(ctxt);
            cmgr_up_page();
            ret = TRUE;
            break;

        case GDK_Down:
        case GDK_Up:
        case GDK_Page_Down:
        case GDK_Page_Up:
        default:
            erbusy_off();
    }
    
    return ret;
}

////////////////////////////////////////////////////////////////////////////
// toolbar icon is clicked
////////////////////////////////////////////////////////////////////////////
static gboolean on_delay_back_page(gpointer data)
{
    pingContext_t *ctxt;

    CN_LOGPRINTF("entry");

    ctxt = cmgr_get_ping_ctxt();
    if (connect_ping_freeze_ui(ctxt))
    {
        CN_WARNPRINTF("call me later...");
        return TRUE;// TRUE = please call again
    }

    toolbar_setIconState(iconID_back, iconState_selected);
    cmgr_add_on_toolbar_sync_action(cmgr_back_page);
    toolbar_synchronise();

    return FALSE;
}

void on_connect_profiles_icon_clicked(int iconID, int iconState)
{
    pingContext_t *ctxt;

    CN_LOGPRINTF("entry iconID[%d] iconState[%d]", iconID, iconState);

    switch (iconID)
    {
        case iconID_back:
            if (iconState == iconState_normal)
            {
                ctxt = cmgr_get_ping_ctxt();
                if (connect_ping_freeze_ui(ctxt))
                {
                    CN_WARNPRINTF("Please wait when freezing UI...");
                    return;
                }
                
                // aborting pinging firstly
                connect_ping_abort(ctxt);

                // wait for ping aborting doen, then back one page
                g_timeout_add(200, on_delay_back_page, NULL);
            }
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////
// some functions about ping context 
////////////////////////////////////////////////////////////////////////////
void connect_profiles_restore_ping_ctxt(void)
{
    pingContext_t *ctxt;
    cmgrConnectType_t connectType;

    CN_LOGPRINTF("entry");
    
    ctxt = cmgr_get_ping_ctxt();
    if (ctxt)
    {
        connect_ping_ctxt_set_mode(ctxt, connectPing_e);
        
        connectType =  connect_data_get_connect_type();
        connect_ping_ctxt_set_connect_type(ctxt, connectType);

        connect_ping_ctxt_set_done_callbacks(ctxt,
                        connect_background_on_connected,
                        connect_profiles_on_failed_all);

        connect_ping_ctxt_set_get_initial_profile(ctxt,
                    connect_background_get_initial_profile);

        connect_ping_ctxt_set_access_network_profiles_callbacks(ctxt,
                                    connect_data_get_n_network_profiles,
                                    connect_data_get_network_profile,
                                    connect_data_select_next_regprofile);

       connect_ping_ctxt_set_ui_callbacks(ctxt,
                           connect_profiles_update_status,
                           connect_profiles_display_settings,
                           connect_profiles_select_profile,
                           connect_profiles_unselect_profile);
    }
}

static void connect_profiles_on_failed_all(void)
{
    connect_profiles_unselect_profile();

    if (main_get_connect_after_reboot())
    {
        CN_LOGPRINTF("It's connection after rebooting, exit.");
        main_quit();
    }
}

