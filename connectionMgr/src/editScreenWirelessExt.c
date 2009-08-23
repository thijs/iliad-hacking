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

/* \file editScreenWireless.c
 * \brief connectionMgr - edit wireless screen 
 *
 *  "Wireless configuration" screen 
 *  when the profile editor is launched from connect
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include "config.h"
#include <string.h>
#include <gtk/gtk.h>

#include <liberdm/connectionMgrConstants.h>
#include <liberdm/display.h>
#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "erbusy.h"
#include "background.h"
#include "connectionMgr.h"
#include "gtkSettingItem.h"
#include "gtkInfoItem.h"
#include "gtkProfileEntry.h"
#include "widgetUtils.h"
#include "connectScreen.h"
#include "connectScreenData.h"
#include "editScreen.h"
#include "editScreenOverview.h"
#include "editScreenWireless.h"
#include "editScreenWirelessExt.h"
#include "commonData.h"
#include "connectPing.h"
#include "connectBackground.h"
#include "languages.h"

typedef enum
{
    inputEncrKey_e = 0,
    wrongEncrKey_e,
    advancedSettings_e,
    pcshareSettings_e,
    wlanExtConnecting_e,
    wlanExtConnected_e,
    wlanExtAborting_e,
    wlanExtAborted_e,
    undefWlanExtStatus_e
}wlanExtStatus_t;

static GtkWidget *g_wireless_ext = NULL;

static bkProfileEntry *g_profile_entry = NULL;

static GtkWidget *g_ssid_name = NULL;
static GtkWidget *g_ssid_value = NULL;
static GtkWidget *g_encrtype_name = NULL;
static GtkWidget *g_encrtype_value = NULL;
static wdtLabelEntry *g_encrkey = NULL;

static bkWdtLabelButton *g_advanced = NULL;

static GtkWidget *g_info = NULL;

static char *g_old_encrkey = NULL;

static GtkWidget *create_encrypt(void);

static void edit_wireless_ext_update_status(pingStatus_t status);

static void edit_wireless_ext_set_profile(const regNetworkProfile_t *settings);
static void edit_wireless_ext_set_ssid(const char* ssid);
static void edit_wireless_ext_set_encrtype(wireless_encr_type_t encrtype);
static void edit_wireless_ext_set_encrkey(const char* encrkey);

static const char* edit_wireless_ext_get_ssid(void);
static wireless_encr_type_t edit_wireless_ext_get_encrtype(void); 
static const char* edit_wireless_ext_get_encrkey(void);

static void edit_wireless_ext_backup_network_settings(void);

static void on_connect(GtkToggleButton *button, gpointer data);
static void on_advanced(GtkToggleButton *button, gpointer data);

static gboolean delayed_deactive_button(gpointer data);

static void edit_wireless_ext_restore_ping_ctxt(void);
static void on_connected(int profileIndex);
static void on_failed(void);
static void ui_update_status(int profileIndex, pingStatus_t status);
static void ui_display_settings(int profileIndex, gboolean resetStatus);
static void ui_select_profile(int profileIndex);
static void ui_unselect_all_profile(void);

static void edit_wireless_ext_freeze_ui(gboolean freeze);

//
// |--item (GtkSettingsItem)
//    |--vbox
//       |--profileEntry (bkProfileEntry)
//       |--encrypt      (composite widgets)
//       |--advanced     (bkWdtLabelButton)
//       |--info         (GtkInfoItem)
//
GtkWidget *edit_wireless_ext_create(void)
{
    GtkWidget *item;
    GtkWidget *vbox;
    bkProfileEntry *profileEntry;
    GtkWidget *encrypt;
    bkWdtLabelButton *advanced;
    GtkWidget *info;
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    // item
    item = gtk_settingitem_new("");

    // vbox
    vbox =gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), vbox);
    
    // profileEntry
    profileEntry = bk_profile_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), profileEntry->background, 
                       FALSE, FALSE, 0);

    // encrypt
    encrypt = create_encrypt();
    gtk_box_pack_start(GTK_BOX(vbox), encrypt, FALSE, FALSE, 0);
    
    // advanced
    advanced = bk_wdt_label_button_new(1, FALSE, 0, 1);
    gtk_box_pack_start(GTK_BOX(vbox), advanced->background, 
                       FALSE, FALSE, 0); 

    // info
    info = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), info, FALSE, FALSE, 0);

    // signal handlers
    widget = GTK_WIDGET(advanced->details->buttonList[0]);
    g_signal_connect_after(G_OBJECT(widget), "toggled", 
                           G_CALLBACK(on_advanced), NULL);
 
    // show widgets
    gtk_widget_show(item);
    gtk_widget_show(vbox);
    gtk_widget_show(profileEntry->background);
    gtk_widget_show(encrypt);
    gtk_widget_show(advanced->background);
    gtk_widget_show(info);

    // global variables
    g_wireless_ext = item;
    g_profile_entry = profileEntry;
    g_advanced =  advanced;
    g_info = info;

    // return
    return item;
}

// |--background
//    |--alignment
//       |--vbox
//          |--subvbox
//             |--hbox
//                |--ssidName
//                |--ssidValue
//             |--hbox
//                |--encrtypeName
//                |--encrtypeValue
//          |--encrkey
static GtkWidget *create_encrypt(void)
{
    GtkWidget *background;
    GtkWidget *alignment;
    GtkWidget *vbox;
    GtkWidget *subvbox;
    GtkWidget *hbox;
    GtkWidget *ssidName;
    GtkWidget *ssidValue;
    GtkWidget *encrtypeName;
    GtkWidget *encrtypeValue;
    wdtLabelEntry *encrkey;
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "bk_grey_666_168");
    gtk_widget_set_size_request(background, 
            BK_666_168_WIDTH, BK_666_168_HEIGHT);

    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              PADDING_TOP * 2, 
                              PADDING_BOTTOM * 2, 
                              PADDING_LEFT, 
                              PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING * 3);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    // subvbox
    subvbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_box_pack_start(GTK_BOX(vbox), subvbox, FALSE, FALSE, 0);

    // hbox
    hbox = gtk_hbox_new(FALSE, HBOX_SPACING);
    gtk_box_pack_start(GTK_BOX(subvbox), hbox, FALSE, FALSE, 0);

    // ssidName
    ssidName = gtk_label_new("");
    gtk_widget_set_name(ssidName, "info_label");
    gtk_box_pack_start(GTK_BOX(hbox), ssidName, FALSE, FALSE, 0);
    
    // ssidValue
    ssidValue = gtk_label_new("");
    gtk_widget_set_name(ssidValue, "info_label");
    gtk_box_pack_start(GTK_BOX(hbox), ssidValue, FALSE, FALSE, 0);

    // hbox
    hbox = gtk_hbox_new(FALSE, HBOX_SPACING);
    gtk_box_pack_start(GTK_BOX(subvbox), hbox, FALSE, FALSE, 0);

    // encrtypeName
    encrtypeName = gtk_label_new("");
    gtk_widget_set_name(encrtypeName, "info_label");
    gtk_box_pack_start(GTK_BOX(hbox), encrtypeName, FALSE, FALSE, 0);
    
    // encrtypeValue
    encrtypeValue = gtk_label_new("");
    gtk_widget_set_name(encrtypeValue, "info_label");
    gtk_box_pack_start(GTK_BOX(hbox), encrtypeValue, FALSE, FALSE, 0);

    // encrkey
    encrkey = wdt_label_entry_new(string_e);
    gtk_entry_set_visibility(GTK_ENTRY(encrkey->entry), FALSE); 
    gtk_box_pack_start(GTK_BOX(vbox), encrkey->parent, FALSE, FALSE, 0);

    // signal handlers
    widget = encrkey->button;
    g_signal_connect_after(G_OBJECT(widget), "toggled", 
                           G_CALLBACK(on_connect), NULL);

    // show widgets
    gtk_widget_show(alignment);
    gtk_widget_show(vbox);
    gtk_widget_show_all(subvbox);
    gtk_widget_show(encrkey->parent);
    gtk_widget_grab_focus(encrkey->entry);

    // global variables
    g_ssid_name = ssidName;
    g_ssid_value = ssidValue;
    g_encrtype_name = encrtypeName;
    g_encrtype_value = encrtypeValue;
    g_encrkey = encrkey;

    // return
    return background;
}

static void edit_wireless_ext_show_widgets(wlanExtStatus_t mode)
{
    CN_LOGPRINTF("entry mode[%d]", mode);
   
    g_return_if_fail(g_advanced != NULL);

    switch (mode)
    {
        case advancedSettings_e:
            gtk_widget_show(g_advanced->background);
            break;
        default:
            gtk_widget_hide(g_advanced->background);
            break;
    }
}

void edit_wireless_ext_set_text(void)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    if (g_wireless_ext)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_wireless_ext),
                                        _("Network profile"));
    }
    
    if (g_ssid_name)
    {
        gtk_label_set_text(GTK_LABEL(g_ssid_name), _("Network name (SSID):"));
    }
    
    if (g_encrtype_name)
    {
        gtk_label_set_text(GTK_LABEL(g_encrtype_name), _("Security type:"));
    }
    
    if (g_encrkey)
    {
        widget = g_encrkey->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                _("Enter the security key / passphrase:"));

        widget = g_encrkey->button;
        gtk_button_set_label(GTK_BUTTON(widget), _("Connect"));
    }

    if (g_advanced)
    {
        widget = g_advanced->details->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                _("Click the button for advanced settings."));
        widget = GTK_WIDGET(g_advanced->details->buttonList[0]);
        gtk_button_set_label(GTK_BUTTON(widget),  _("Advanced settings"));
    }
}

static void edit_wireless_ext_update_info_text(wlanExtStatus_t status)
{
    g_return_if_fail(g_info != NULL);

    CN_LOGPRINTF("entry");

    switch (status)
    {
        case inputEncrKey_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("If you do not know the security key "
                      "for this access point. "
                      "Please ask your network administrator."));
            break;
        case wrongEncrKey_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconFailed_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("Connection failed: Cannot connect to "
                      "the access point.\n"
                      "Check that the security key is correct "
                      "and for WPA check "
                      "that your router is set to PSK-TKIP mode. "
                      "If you do not know the settings "
                      "for this access point, "
                      "please ask your network administrator."));
            break;
        case advancedSettings_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconFailed_e); 
            // TODO
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("Connection failed: Cannot connect to the internet "
                      "or iDS.\n"
                      "Click on 'Advanced settings' "
                      "to set a proxy server.\n"
                      "Please ask your network administrator "
                      "if you do not know the correct settings "
                      "for this network."));
            break;
        case pcshareSettings_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconFailed_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("Connection failed: Cannot connect "
                      "to your computer.\n"
                      "Please check the 'Settings for Computer Connection' "
                      "in the iLiad Settings menu."));
            break;
        case wlanExtConnecting_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("The iLiad is trying "
                  "to connect to the server."));
            break;
         case wlanExtConnected_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconOk_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("Connection to the server "
                      "has been established."));
            break;
        case wlanExtAborting_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                _("The iLiad is aborting the connection."));
            break;
        case wlanExtAborted_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e);
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                _("The connection has been aborted."));
            break;
        default:
            break;
    }
}

void edit_wireless_ext_update_status(pingStatus_t status)
{
    CN_LOGPRINTF("entry");

    bk_profile_entry_update_status(g_profile_entry, status);
}

void edit_wireless_ext_set_network_settings(const regNetworkProfile_t *settings)
{
    const char *ssid = NULL;
    wireless_encr_type_t encrtype = encr_undefined_t;
    const char *encrkey = NULL;
    regWirelessSetting_t *wirelessSettings = NULL;

    CN_LOGPRINTF("entry");

    g_return_if_fail(settings != NULL);
    g_return_if_fail(settings->connection == wireless_t);
    
    wirelessSettings = settings->wirelessSettings;
    if (wirelessSettings)
    {
        ssid = wirelessSettings->SSID;
        encrtype =  wirelessSettings->encrType;
        encrkey = wirelessSettings->encrKey;
    }
    
    edit_wireless_ext_set_profile(settings);
    edit_wireless_ext_set_ssid(ssid);
    edit_wireless_ext_set_encrtype(encrtype);
    edit_wireless_ext_set_encrkey(encrkey);
    
    edit_wireless_ext_show_widgets(inputEncrKey_e);
    edit_wireless_ext_update_info_text(inputEncrKey_e);

    edit_wireless_ext_backup_network_settings();
}

static void edit_wireless_ext_set_profile(const regNetworkProfile_t *settings)
{
    CN_LOGPRINTF("entry");

    bk_profile_entry_display(g_profile_entry, settings);
}

static void edit_wireless_ext_set_ssid(const char *ssid)
{
    CN_LOGPRINTF("entry");

    if (g_ssid_value)
    {
        gtk_label_set_text(GTK_LABEL(g_ssid_value), ssid ? ssid : "");
    }
}

static void edit_wireless_ext_set_encrtype(wireless_encr_type_t encrtype)
{
    char *text;

    CN_LOGPRINTF("entry");

    if (g_encrtype_value)
    {
        switch (encrtype)
        {
            case encr_wep_t:
                text = "WEP";
                break;
            case encr_wpa_t:
                text = "WPA";
                break;
            case encr_none_t:
            default:
                text = "NONE";
                break;
        }
        gtk_label_set_text(GTK_LABEL(g_encrtype_value), text);
    }
}

static void edit_wireless_ext_set_encrkey(const char* encrkey)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    if (g_encrkey)
    {
        widget = g_encrkey->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), encrkey);
    }
}

void edit_wireless_ext_get_network_settings(regNetworkProfile_t *settings)
{
    regWirelessSetting_t *wirelessSettings;
    const char *ssid = NULL;
    const char *encrkey = NULL;

    CN_LOGPRINTF("entry");

    g_return_if_fail(settings != NULL);
    g_return_if_fail(settings->connection == wireless_t);

    wirelessSettings = settings->wirelessSettings;

    ssid = edit_wireless_ext_get_ssid();
    encrkey = edit_wireless_ext_get_encrkey();

    if (wirelessSettings)
    {
        g_free(wirelessSettings->SSID);
        g_free(wirelessSettings->encrKey);
        g_free(wirelessSettings);
        wirelessSettings = NULL;
        settings->wirelessSettings = NULL;
    }

    if (settings->connection == wireless_t)
    {
        wirelessSettings = g_new0(regWirelessSetting_t, 1);
        g_assert(wirelessSettings != NULL);
        wirelessSettings->SSID     = g_strdup(ssid);
        wirelessSettings->encrType = edit_wireless_ext_get_encrtype();
        wirelessSettings->encrKey  = g_strdup(encrkey);
        // it's a broadcast access point
        wirelessSettings->broadcast = TRUE;
        settings->wirelessSettings = wirelessSettings;
    }
}

static const char *edit_wireless_ext_get_ssid(void)
{
    const char *ssid = NULL;
    
    CN_LOGPRINTF("entry");
    if (g_ssid_value)
    {
        ssid = gtk_label_get_text(GTK_LABEL(g_ssid_value));
    }

    CN_LOGPRINTF("return ssid=%s", ssid);
    return ssid;
}

static wireless_encr_type_t edit_wireless_ext_get_encrtype(void)
{
    const char *value = NULL;
    wireless_encr_type_t encrtype = encr_undefined_t;
       
    CN_LOGPRINTF("entry");

    if (g_encrtype_value)
    {
        value = gtk_label_get_text(GTK_LABEL(g_encrtype_value));
        if (value)
        {
            if (strcmp(value, "NONE") == 0)
            {
                encrtype = encr_none_t;
            }
            else if (strcmp(value, "WEP") == 0)
            {
                encrtype = encr_wep_t;
            }
            else if (strcmp(value, "WPA") == 0)
            {
                encrtype = encr_wpa_t;
            }
        }
    }

    CN_LOGPRINTF("return encrtype=%d", encrtype);
    return encrtype;
}

static const char *edit_wireless_ext_get_encrkey(void)
{
    const char *encrkey = NULL;
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    if (g_encrkey)
    {
        widget = g_encrkey->entry;
        encrkey = gtk_entry_get_text(GTK_ENTRY(widget)); 
    }

    CN_LOGPRINTF("return encrkey=%s", encrkey);
    return encrkey;
}

gboolean edit_wireless_ext_check_network_settings(void)
{
    gboolean valid = TRUE;

    CN_LOGPRINTF("entry");
    if (!ergtk_entry_check_field(ERGTK_ENTRY(g_encrkey->entry)))
    {
        valid = FALSE;
    }

    return valid;
}

static void edit_wireless_ext_backup_network_settings(void)
{
    const char *encrkey;

    CN_LOGPRINTF("entry");

    g_free(g_old_encrkey);

    encrkey = edit_wireless_ext_get_encrkey();
    g_old_encrkey = g_strdup(encrkey);
}

void edit_wireless_ext_restore_network_settings(void)
{
    CN_LOGPRINTF("entry");

    edit_wireless_ext_set_encrkey(g_old_encrkey);
}

static gboolean on_delayed_connect(gpointer data)
{
    gboolean valid;
    pingContext_t *ctxt;
    GtkToggleButton *button;

    erbusy_blink();

    valid = edit_wireless_ext_check_network_settings();
    if (valid)
    {
        edit_wireless_ext_freeze_ui(TRUE);

        edit_wireless_ext_restore_ping_ctxt();

        ctxt = cmgr_get_ping_ctxt();
        connect_ping_select_initial_profile(ctxt);
        connect_ping(ctxt, TRUE);
    }
    else
    {
        button = GTK_TOGGLE_BUTTON(data);
        gtk_toggle_button_set_active(button, FALSE);
    }

    return FALSE;
}

static void on_connect(GtkToggleButton* button, gpointer data)
{
    gboolean active;

    CN_LOGPRINTF("entry");

    active = gtk_toggle_button_get_active(button);
    if (active)
    {
        g_timeout_add(500, on_delayed_connect, (gpointer)button);
    }
}

static gboolean on_delayed_advanced(gpointer data)
{
    gboolean valid;
    GtkToggleButton *button;

    erbusy_blink();

    valid = edit_wireless_ext_check_network_settings();
    if (valid)
    {
        edit_wireless_ext_backup_network_settings();

        edit_goto_page(editScreenOverview_e);
        edit_overview_update_info_text(inputProxy_e);
    }

    button = GTK_TOGGLE_BUTTON(data);
    gtk_toggle_button_set_active(button, FALSE);

    return FALSE;
}

static void on_advanced(GtkToggleButton* button, gpointer data)
{
    gboolean active;

    CN_LOGPRINTF("entry");

    active = gtk_toggle_button_get_active(button);
    if (active)
    {
        g_timeout_add(500, on_delayed_advanced, button);
    }
}

static gboolean delayed_deactive_button(gpointer data)
{
    GtkToggleButton *button = GTK_TOGGLE_BUTTON(data);
    
    if (button)
    {
        gtk_toggle_button_set_active(button, FALSE);
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////
// functions about ping context
////////////////////////////////////////////////////////////////////////////
static void edit_wireless_ext_restore_ping_ctxt(void)
{
    pingContext_t *ctxt;
    cmgrConnectType_t connectType;

    CN_LOGPRINTF("entry");
    
    ctxt = cmgr_get_ping_ctxt();
    if (ctxt)
    {
        // set mode and connect type
        connect_ping_ctxt_set_mode(ctxt, editPing_e);
        connectType = connect_data_get_connect_type();
        connect_ping_ctxt_set_connect_type(ctxt, connectType);
        
        // ping done callbacks
        connect_ping_ctxt_set_done_callbacks(ctxt, on_connected, on_failed);
       
        // data access callbacks
        connect_ping_ctxt_set_get_initial_profile(ctxt, 
                        edit_get_initial_profile);
        connect_ping_ctxt_set_access_network_profiles_callbacks(ctxt,
                                        edit_get_n_network_profiles,
                                        edit_get_network_profile,
                                        edit_select_next_regprofile);

        // ui callbacks
        connect_ping_ctxt_set_ui_callbacks(ctxt, 
                                    ui_update_status, 
                                    ui_display_settings,
                                    ui_select_profile,
                                    ui_unselect_all_profile);
    }
}

static void on_connected(int profileIndex)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");
   
    edit_save_profile();

    edit_on_connected();    

    edit_wireless_ext_freeze_ui(FALSE);

    if (g_encrkey)
    {
        button = GTK_TOGGLE_BUTTON(g_encrkey->button);
        g_timeout_add(500, delayed_deactive_button, button);
    }
}

static void on_failed(void)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");

    edit_wireless_ext_freeze_ui(FALSE);

    if (g_encrkey)
    {
        button = GTK_TOGGLE_BUTTON(g_encrkey->button);
        gtk_toggle_button_set_active(button, FALSE);
    }
}

static void ui_update_status(int profileIndex, pingStatus_t status)
{
    wlanExtStatus_t state;

    CN_LOGPRINTF("entry");
    
    edit_wireless_ext_update_status(status);

    switch (status)
    {
        case pingSucceeded_e:
            state = wlanExtConnected_e;
            break;
        case pingFailed_e:
            state = advancedSettings_e;
            break;
        case pingFailedPCShare_e:
            state = pcshareSettings_e;
            break;
        case pingFailedNetwork_e:
            state = wrongEncrKey_e;
            break;
        case pingConnecting_e:
            state = wlanExtConnecting_e;
            break;
        case pingAborting_e:
            state = wlanExtAborting_e;
            break;
        case pingAborted_e:
            state = wlanExtAborted_e;
            on_failed();
            break;
        default:
            state = undefWlanExtStatus_e;
            break;
    }

    edit_wireless_ext_show_widgets(state);
    edit_wireless_ext_update_info_text(state);

    display_update_request_screen_refresh (STATUS_ITEM_CHANGE, 
                                           WAVEFORM_TYPING);
}

static void ui_display_settings(int profileIndex, gboolean resetStatus)
{
    CN_LOGPRINTF("entry");
    // nothing to do
}

static void ui_select_profile(int profileIndex)
{
    GtkProfileEntry *profileEntry;
    
    CN_LOGPRINTF("entry");

    if (g_profile_entry)
    {
        profileEntry = GTK_PROFILE_ENTRY(g_profile_entry->details);
        gtk_profile_entry_set_buttons_active(profileEntry, TRUE, FALSE);

        display_update_request_screen_refresh (STATUS_ITEM_CHANGE, 
                                               WAVEFORM_TYPING);
    }
}

static void ui_unselect_all_profile(void)
{
    GtkProfileEntry *profileEntry;

    CN_LOGPRINTF("entry");

    if (g_profile_entry)
    {
        profileEntry = GTK_PROFILE_ENTRY(g_profile_entry->details);
        gtk_profile_entry_set_buttons_active(profileEntry, FALSE, FALSE);

        display_update_request_screen_refresh(STATUS_ITEM_CHANGE, 
                                              WAVEFORM_TYPING);
    }
}


////////////////////////////////////////////////////////////////////////////
// key enter 
////////////////////////////////////////////////////////////////////////////
gboolean on_edit_wireless_ext_keypress(GdkEventKey *event)
{
    pingContext_t *ctxt;
    GtkToggleButton *button;
    gboolean ret = FALSE;

    CN_LOGPRINTF("entry");
   
    ctxt = cmgr_get_ping_ctxt();
    switch (event->keyval)
    {
        case GDK_Return:
            // 'Enter' key is pressed
            if (!connect_ping_in_pinging(ctxt))
            {
                // simulate clicking the 'Connect' button
                if (g_encrkey)
                {
                    button = GTK_TOGGLE_BUTTON(g_encrkey->button);
                    gtk_toggle_button_set_active(button, TRUE);
                }
                ret = TRUE;
            }
            break;

        case GDK_F5:
            // 'UP' key is pressed
            // goto the one top level page
            connect_ping_abort(ctxt);
            cmgr_up_page();
            ret = TRUE;
            break;
        default:
            erbusy_off();
            break;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////
// functions for freezing the ui when in ping-to-server state
///////////////////////////////////////////////////////////////////////////

static void edit_wireless_ext_freeze_ui(gboolean freeze)
{
    gboolean sensitive;
    GtkWidget *widget;

    CN_LOGPRINTF("entry[%d]", freeze);

    sensitive = !freeze;
    
    // the encryption key
    widget = g_encrkey->entry;
    gtk_widget_set_sensitive(widget, sensitive);

    // the 'Connect' button
    widget = g_encrkey->button;
    gtk_widget_set_sensitive(widget, sensitive);

    // the 'Advanced settings' button
    widget = GTK_WIDGET(g_advanced->details->buttonList[0]);
    gtk_widget_set_sensitive(widget, sensitive);
}

