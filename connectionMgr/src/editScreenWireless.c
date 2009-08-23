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
 * \file 
 * \brief connectionMgr - "Wireless Configuration" screen.
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
#include "widgetUtils.h"
#include "connectScreen.h"
#include "connectScreenData.h"
#include "editScreen.h"
#include "editScreenOverview.h"
#include "editScreenWireless.h"
#include "connectScan.h"
#include "connectBackground.h"
#include "languages.h"

typedef enum
{
    inputSSID_e,
    inputEncrType_e,
    inputEncrKeyWEP_e,
    inputEncrKeyWPA_e,
    wlanInputProfileName_e,
    wlanConnecting_e,
    wlanConnected_e,
    wlanUnconnected_e,
    wlanAborting_e,
    wlanAborted_e,
    undefWlanStatus_e
}wlanStatus_t;

// screen objects
static GtkWidget* g_wireless = NULL;

static bkWdtLabelEntry  *g_ssid = NULL;
static bkWdtLabelButton *g_encrtype = NULL;
static bkWdtLabelEntry  *g_encrkey = NULL;
static bkWdtLabelEntry  *g_profilename = NULL;
static bkWdtLabelButton *g_buttons = NULL;

static GtkWidget *g_info = NULL;

static wlanStatus_t g_wlan_status = undefWlanStatus_e;

static network_spec_t* g_found_network = NULL;

static regWirelessSetting_t *g_old_settings = NULL;
static char *g_old_profilename = NULL;

static void edit_wireless_set_status(wlanStatus_t status);

static void on_ssid_changed(GtkEntry *widget, gpointer data);
static void on_ssid_next(GtkToggleButton *button, gpointer data);

static void on_encrtype_update(GtkWidget *selection, 
                               gpointer button, 
                               gpointer data);

static void on_encrkey_changed(GtkEntry *widget, gpointer data);

static void on_profilename_changed(GtkEntry *widget, gpointer data);
static void on_profilename_next(GtkToggleButton *button, gpointer data);

static void on_buttons_update(GtkWidget *selection, 
                              gpointer button, 
                              gpointer data);

static gboolean delayed_deactive_button(gpointer data);

static void on_test(void);
static void on_save(void);
static void on_saveconnect(void);

static void edit_wireless_restore_scan_ctxt(void);
static void scan_done(connection_t networkType, 
                  network_spec_t* pScanNetworks, 
                  int nScanNetworks);
static void scan_done_after(void);
static void edit_wireless_restore_ping_ctxt(void);
static void on_connected(int profileIndex);
static void on_failed(void);
static void ui_update_status(int profileIndex, pingStatus_t status);
static void ui_display_settings(int profileIndex, gboolean resetStatus);
static void ui_select_profile(int profileIndex);
static void ui_unselect_all_profile(void);

static void edit_wireless_freeze_ui(gboolean freeze);

///////////////////////////////////////////////////////////////////////////
// create widgets
///////////////////////////////////////////////////////////////////////////

// 
// |--item (GtkSettingsIem)
//    |--vbox
//       |--ssid        (bkWdtLabelEntry)
//       |--encrtype    (bkWdtLabelButton)
//       |--encrkey     (bkWdtLabelEntry)
//       |--profilename (bkWdtLabelEntry)
//       |--buttons     (bkWdtLabelButton)
//       |--info        (GtkInfoItem) 
//
GtkWidget *edit_wireless_create(void)
{
    GtkWidget *item;
    GtkWidget *vbox;
    bkWdtLabelEntry  *ssid;
    bkWdtLabelButton *encrtype;
    bkWdtLabelEntry  *encrkey;
    bkWdtLabelEntry  *profilename;
    bkWdtLabelButton *buttons;
    GtkWidget *info;
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    // item
    item = gtk_settingitem_new("");

    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), vbox);

    // ssid
    ssid = bk_wdt_label_entry_new(string_e);
    gtk_box_pack_start(GTK_BOX(vbox), ssid->background, FALSE, FALSE, 0);

    // encrtype
    encrtype = bk_wdt_label_button_new(3, TRUE, 0, 1);
    gtk_box_pack_start(GTK_BOX(vbox), encrtype->background, 
                       FALSE, FALSE, 0);

    // encrkey
    encrkey = bk_wdt_label_entry_new(string_e);
    gtk_entry_set_visibility(GTK_ENTRY(encrkey->details->entry), FALSE); 
    gtk_box_pack_start(GTK_BOX(vbox), encrkey->background, 
                       FALSE, FALSE, 0);

    // profilename
    profilename = bk_wdt_label_entry_new(string_e);
    gtk_box_pack_start(GTK_BOX(vbox), profilename->background, 
                       FALSE, FALSE, 0);

    // buttons
    buttons = bk_wdt_label_button_new(3, FALSE, 0, 1);
    gtk_box_pack_start(GTK_BOX(vbox), buttons->background, 
                       FALSE, FALSE, 0);

    // info
    info = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), info, FALSE, FALSE, 0);

    // signal handlers
    widget = ssid->details->entry;
    g_signal_connect_after(G_OBJECT(widget), "changed-stable", 
                           G_CALLBACK(on_ssid_changed), NULL);
    widget = ssid->details->button;
    g_signal_connect_after(G_OBJECT(widget), "toggled", 
                           G_CALLBACK(on_ssid_next), NULL);

    widget = GTK_WIDGET(encrtype->details->selection);
    g_signal_connect(G_OBJECT(widget), "selection-update",
                     G_CALLBACK(on_encrtype_update), NULL);
    
    widget = encrkey->details->entry;
    g_signal_connect_after(G_OBJECT(widget), "changed-stable", 
                           G_CALLBACK(on_encrkey_changed), NULL);

    widget = profilename->details->entry;
    g_signal_connect_after(G_OBJECT(widget), "changed-stable", 
                           G_CALLBACK(on_profilename_changed), NULL);
    widget = profilename->details->button;
    g_signal_connect_after(G_OBJECT(widget), "toggled", 
                           G_CALLBACK(on_profilename_next), NULL);

    widget = GTK_WIDGET(buttons->details->selection);
    g_signal_connect(G_OBJECT(widget), "selection-update",
                     G_CALLBACK(on_buttons_update), NULL);

    // show widgets
    gtk_widget_show(item);
    gtk_widget_show(vbox);
    gtk_widget_show(ssid->background);
    gtk_widget_show(encrtype->background);
    gtk_widget_show(encrkey->background);
    gtk_widget_hide(encrkey->details->button);
    gtk_widget_show(profilename->background);
    gtk_widget_show(buttons->background);
    gtk_widget_show(info);

    // global variables
    g_wireless = item;
    g_ssid = ssid;
    g_encrtype = encrtype;
    g_encrkey = encrkey;
    g_profilename = profilename;
    g_buttons = buttons;
    g_info = info;

    CN_LOGPRINTF("done");

    // return
    return item;
 }

static void edit_wireless_show_widgets(wlanStatus_t mode)
{
    wireless_encr_type_t encrtype;

    CN_LOGPRINTF("entry mode[%d]", mode);

    switch (mode)
    {
        case inputSSID_e:
            gtk_widget_show(g_ssid->background);
            gtk_widget_show(g_ssid->details->button);
            gtk_widget_hide(g_encrtype->background);
            gtk_widget_hide(g_encrkey->background);
            gtk_widget_hide(g_profilename->background);
            gtk_widget_hide(g_buttons->background);
            gtk_widget_grab_focus(g_ssid->details->entry);
            break;
        case inputEncrType_e:
            gtk_widget_show(g_ssid->background);
            gtk_widget_hide(g_ssid->details->button);
            gtk_widget_show(g_encrtype->background);
            gtk_widget_hide(g_encrkey->background);
            gtk_widget_hide(g_profilename->background);
            gtk_widget_hide(g_buttons->background);
            break;
        case inputEncrKeyWEP_e:
        case inputEncrKeyWPA_e:
            gtk_widget_show(g_ssid->background);
            gtk_widget_hide(g_ssid->details->button);
            gtk_widget_show(g_encrtype->background);
            gtk_widget_show(g_encrkey->background);
            gtk_widget_show(g_profilename->background);
            gtk_widget_show(g_buttons->background);
            gtk_widget_grab_focus(g_encrkey->details->entry);
            break;
        case wlanInputProfileName_e:
        case undefWlanStatus_e:
        default:
            gtk_widget_show(g_ssid->background);
            gtk_widget_hide(g_ssid->details->button);
            gtk_widget_show(g_encrtype->background); 
            encrtype = edit_wireless_get_encrtype();
            if (encrtype == encr_none_t)
            {
                gtk_widget_hide(g_encrkey->background);
            }
            else
            {
                gtk_widget_show(g_encrkey->background);
            }
            gtk_widget_show(g_profilename->background);
            gtk_widget_show(g_buttons->background);
            gtk_widget_grab_focus(g_profilename->details->entry);
            break;
    }
}

void edit_wireless_enable_save_button(gboolean enable)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry %d", enable);
    
    if (g_buttons)
    {
        widget = GTK_WIDGET(g_buttons->details->buttonList[1]);
        gtk_widget_set_sensitive(widget, enable);
    }
}

void edit_wireless_set_text (void)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    if (g_wireless)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_wireless), 
                                        _("Network profile"));
    }

    if (g_ssid)
    {
        widget = g_ssid->details->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                _("Type the network name (SSID) and click 'Next'."));
    
        widget = g_ssid->details->button;
        gtk_button_set_label(GTK_BUTTON(widget), _("Next"));
    }

    if (g_encrtype)
    {
        widget = g_encrtype->details->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                _("Select the security type of this wireless network."));

        widget = GTK_WIDGET(g_encrtype->details->buttonList[0]);
        gtk_button_set_label(GTK_BUTTON(widget), _("Off"));
        
        widget = GTK_WIDGET(g_encrtype->details->buttonList[1]);
        gtk_button_set_label(GTK_BUTTON(widget), _("WEP"));
        
        widget = GTK_WIDGET(g_encrtype->details->buttonList[2]);
        gtk_button_set_label(GTK_BUTTON(widget), _("WPA"));
    }

    if (g_encrkey)
    {
        widget = g_encrkey->details->label;
        gtk_label_set_text(GTK_LABEL(widget),
                _("Enter the security key / passphrase:"));
    }

    if (g_profilename)
    {
        widget = g_profilename->details->label;
        gtk_label_set_text(GTK_LABEL(widget),
                _("You can personalize the profile name."));

        widget = g_profilename->details->button;
        gtk_button_set_label(GTK_BUTTON(widget), _("Next"));
    }
    
    if (g_buttons)
    {
        widget = g_buttons->details->label;
        gtk_label_set_text(GTK_LABEL(widget),
                _("Select an action for this profile."));

        widget = GTK_WIDGET(g_buttons->details->buttonList[0]);
        gtk_button_set_label(GTK_BUTTON(widget), _("Test"));

        widget = GTK_WIDGET(g_buttons->details->buttonList[1]);
        gtk_button_set_label(GTK_BUTTON(widget), _("Save"));

        widget = GTK_WIDGET(g_buttons->details->buttonList[2]);
        gtk_button_set_label(GTK_BUTTON(widget), _("Save & Connect"));
    }    
}

static void edit_wireless_update_info_text(wlanStatus_t status)
{
    CN_LOGPRINTF("entry");

    g_return_if_fail(g_info != NULL);

    switch (status)
    {
        case inputSSID_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("If you do not know the network name "
                  "for this access point, "
                  "please ask your network administrator."));
            break;
        case inputEncrType_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("Click 'Off' for a non secured network "
                  "or choose the security type of "
                  "your network.\n"
                  "Both 'WEP' and 'WPA' requires a security key."));
            break;
        case inputEncrKeyWEP_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("For WEP, set your local router device to a HEX key.\n"
                  "Hint: If you do not know the security key "
                  "for this network, "
                  "please ask your network adminitrator."));
            break;
        case inputEncrKeyWPA_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("For WPA, set your router to PSK mode with TKIP.\n"
                  "Hint: If you do not know the security key "
                  "for this network, "
                  "please ask your network administrator."));
            break;
        case wlanInputProfileName_e:
        case undefWlanStatus_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("If you personalize the profile name, "
                  "it will be easy to recognize "
                  "in the iLiad's network profile list.\n"
                  "Click 'Next' for more advanced settings."));
            break;
        case wlanConnecting_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("The iLiad is trying to connect to the server."));
            break;
        case wlanConnected_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconOk_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("Connection to the server established."));
            break;
        case wlanUnconnected_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconFailed_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("Connection to server "
                      "can not been established correctly."));
            break;
        case wlanAborting_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                _("The iLiad is aborting the connection."));
            break;
        case wlanAborted_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e);
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                _("The connection has been aborted."));
            break;

        default:
            break;
    }
}

// return undefWlanStatus_e means all fields have been fill in
static wlanStatus_t edit_wireless_detect_status(void)
{
    erGtkEntry *ssid, *encrkey, *profilename;
    wireless_encr_type_t encrtype;
    wlanStatus_t status = undefWlanStatus_e;

    CN_LOGPRINTF("entry");

    ssid = ERGTK_ENTRY(g_ssid->details->entry);
    encrtype = edit_wireless_get_encrtype();
    profilename = ERGTK_ENTRY(g_profilename->details->entry);
    encrkey = ERGTK_ENTRY(g_encrkey->details->entry);

    if (!ergtk_entry_check_field(ssid))
    {
        status = inputSSID_e;
    }
    else if (encrtype == encr_undefined_t)
    {
        status = inputEncrType_e;
    }
    else if (encrtype == encr_none_t)
    {
        if (!ergtk_entry_check_field(profilename))
        {
            status = wlanInputProfileName_e;
        }
    }
    else if (!ergtk_entry_check_field(encrkey))
    {
        if (encrtype == encr_wep_t)
        {
            status = inputEncrKeyWEP_e;
        }
        else if (encrtype == encr_wpa_t)
        {
            status = inputEncrKeyWPA_e;
        }
    }
    else if (!ergtk_entry_check_field(profilename))
    {
        status = wlanInputProfileName_e;
    }

    CN_LOGPRINTF("return %d", status);
    return status;
}

void edit_wireless_determine_status(void)
{
    wlanStatus_t status;

    status = edit_wireless_detect_status();
    edit_wireless_set_status(status);
}

static gboolean on_delay_determine_status(gpointer data)
{
    CN_LOGPRINTF("entry");

    edit_wireless_determine_status();
    return FALSE;
}

void delay_edit_wireless_determine_status(void)
{
    g_idle_add(on_delay_determine_status, NULL);
}

static void edit_wireless_set_status(wlanStatus_t status)
{
    CN_LOGPRINTF("entry status[%d]", status);

    g_wlan_status = status;

    edit_wireless_show_widgets(status);
    edit_wireless_update_info_text(status);
}

///////////////////////////////////////////////////////////////////////////
// set/get functions
///////////////////////////////////////////////////////////////////////////

void edit_wireless_set_network_settings(const regNetworkProfile_t* settings)
{   
    gchar                *ssid        = NULL;
    wireless_encr_type_t encrtype    = encr_none_t;
    gchar                *encrkey     = NULL;
    gchar                *profilename = NULL;
    regWirelessSetting_t *wirelessSettings = NULL;
 
    CN_LOGPRINTF("entry");

    g_return_if_fail(settings != NULL);
    g_return_if_fail(settings->connection == wireless_t);
    
    profilename = settings->name;
    
    wirelessSettings = settings->wirelessSettings;
    if (wirelessSettings)
    {
        ssid = wirelessSettings->SSID;
        encrtype = wirelessSettings->encrType;
        encrkey = wirelessSettings->encrKey;
    }

    // set screen objects
    edit_wireless_set_ssid(ssid);
    edit_wireless_set_encrtype(encrtype);
    edit_wireless_set_encrkey(encrkey);
    edit_wireless_set_profilename(profilename);

    delay_edit_wireless_determine_status();

    // make a backup copy of the settings
    edit_wireless_backup_network_settings();
}

void edit_wireless_set_ssid(const char* ssid)
{
    GtkWidget* widget;

    CN_LOGPRINTF("entry ssid[%s]", ssid);

    if (g_ssid)
    {
        widget = g_ssid->details->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), ssid ? ssid : "");
    }
}

void edit_wireless_set_encrtype(const wireless_encr_type_t encrType)
{
    GtkWidget *widget;
    erGtkSelectionGroup *sel;
    int        nsel;

    CN_LOGPRINTF("entry: encrType [%d]", encrType);

    // set screen objects
    switch (encrType)
    {
        case encr_undefined_t:
            widget = g_encrtype->details->selection;
            sel = ERGTK_SELECTION_GROUP(widget);
            ergtk_selection_group_get_selected_buttons(sel, &nsel, 1);
            if ((nsel >= 0) && (nsel <= 2))
            {
                widget = GTK_WIDGET(g_encrtype->details->buttonList[nsel]);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), FALSE);
            }
            break;
        case encr_none_t:
            widget = GTK_WIDGET(g_encrtype->details->buttonList[0]);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
            break;
        case encr_wep_t:
            widget = GTK_WIDGET(g_encrtype->details->buttonList[1]);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
            break;
        case encr_wpa_t:
            widget = GTK_WIDGET(g_encrtype->details->buttonList[2]);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
            break;
        default:
            CN_ERRORPRINTF("Illegal encryption type [%d]", encrType);
            g_assert_not_reached();
    }
}

void edit_wireless_set_encrkey(const char* encrkey)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry encrkey[%s]", encrkey);

    if (g_encrkey)
    {
        widget = g_encrkey->details->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), encrkey ? encrkey : "");
    }
}

void edit_wireless_set_profilename(const gchar* profilename)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry profilename[%s]", profilename);

    if (g_profilename)
    {
        widget = g_profilename->details->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), 
                profilename ? profilename : "");
    }
}

void edit_wireless_get_network_settings(regNetworkProfile_t* settings)
{
    const char *ssid = NULL;
    const char *encrkey = NULL;
    const char *profilename = NULL;
    regWirelessSetting_t *wirelessSettings;

    CN_LOGPRINTF("entry");

    g_return_if_fail(settings != NULL);
    g_return_if_fail(settings->connection == wireless_t);
   
    // clear wireless wirelessSettings
    wirelessSettings = settings->wirelessSettings;
    if (wirelessSettings)
    {
        g_free(wirelessSettings->SSID);
        g_free(wirelessSettings->encrKey);
        g_free(wirelessSettings);
        wirelessSettings = NULL;
        settings->wirelessSettings = NULL;
    }

    // store new settings, if wireless selected
    if (settings->connection == wireless_t)
    {
        ssid = edit_wireless_get_ssid();
        encrkey = edit_wireless_get_encrkey();
        profilename = edit_wireless_get_profilename();
        
        g_free(settings->name);
        settings->name = g_strdup(profilename);

        wirelessSettings = g_new0(regWirelessSetting_t, 1);
        g_assert(wirelessSettings != NULL);
        wirelessSettings->SSID     = g_strdup(ssid);
        wirelessSettings->encrType = edit_wireless_get_encrtype();
        wirelessSettings->encrKey  = g_strdup(encrkey);
        // always treat it as non-broadcast access point
        wirelessSettings->broadcast = FALSE;
        settings->wirelessSettings = wirelessSettings;
    }
}

const char* edit_wireless_get_ssid(void)
{
    GtkWidget *widget;
    const char *ssid = NULL;

    if (g_ssid)
    {
        widget = g_ssid->details->entry;
        ssid = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return ssid=%s", ssid);
    return ssid;
}
 
wireless_encr_type_t edit_wireless_get_encrtype(void)
{
    GtkWidget *widget;
    erGtkSelectionGroup *sel;
    int        nsel;
    wireless_encr_type_t encrType;
 
    CN_LOGPRINTF("entry");

    g_return_val_if_fail(g_encrtype != NULL, encr_undefined_t);

    // get current value from screen object
    widget =  g_encrtype->details->selection;
    sel = ERGTK_SELECTION_GROUP(widget);
    ergtk_selection_group_get_selected_buttons(sel, &nsel, 1);
    switch (nsel)
    {
        case 0:
            encrType = encr_none_t;
            break;
        case 1:
            encrType = encr_wep_t;
            break;
        case 2:
            encrType = encr_wpa_t;
            break;
        default:
            encrType = encr_undefined_t;
            break;
    }

    CN_LOGPRINTF("return encrType=%d", encrType);
    return encrType;
}

const char* edit_wireless_get_encrkey(void)
{
    GtkWidget  *widget;
    const char *encrkey = NULL;

    CN_LOGPRINTF("entry");

    if (g_encrkey)
    {
        widget =  g_encrkey->details->entry;
        encrkey = gtk_entry_get_text(GTK_ENTRY(widget)); 
    }

    CN_LOGPRINTF("return encrkey=%s", encrkey);
    return encrkey;
}

const char* edit_wireless_get_profilename(void)
{
    GtkWidget  *widget;
    const char *profilename = NULL;

    CN_LOGPRINTF("entry");

    if (g_profilename)
    {
        widget = g_profilename->details->entry;
        profilename = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return profilename=%s", profilename);
    return profilename;
}

gboolean edit_wireless_check_network_settings(void)
{
    wlanStatus_t status;
    gboolean valid = TRUE;

    status = edit_wireless_detect_status();
    if (status != undefWlanStatus_e)
    {
        valid = FALSE;
    }
    
    CN_LOGPRINTF("return valid=%d", valid);
    return valid;
}

void edit_wireless_backup_network_settings(void)
{
    const char *ssid, *encrkey, *profilename;
    
    CN_LOGPRINTF("entry");

    ssid = edit_wireless_get_ssid();
    encrkey = edit_wireless_get_encrkey();
    profilename = edit_wireless_get_profilename();

    if (g_old_settings)
    {
        g_free(g_old_settings->SSID);
        g_free(g_old_settings->encrKey);
        g_free(g_old_settings);
    }

    g_old_settings = g_new0(regWirelessSetting_t, 1);
    g_assert(g_old_settings != NULL);
    g_old_settings->SSID = g_strdup(ssid);
    g_old_settings->encrType = edit_wireless_get_encrtype();
    g_old_settings->encrKey = g_strdup(encrkey);
    
    g_free(g_old_profilename);
    g_old_profilename = g_strdup(profilename);
}

void edit_wireless_restore_network_settings(void)
{
    char *ssid = NULL;
    char *encrkey = NULL;
    char *profilename = NULL; 
    
    CN_LOGPRINTF("entry");
     
    if (g_old_settings)
    {
       ssid = g_old_settings->SSID;
       encrkey = g_old_settings->encrKey;
    }

    if (g_old_profilename)
    {
        profilename = g_old_profilename;
    }

    edit_wireless_set_ssid(ssid);
    edit_wireless_set_encrtype(g_old_settings->encrType);
    edit_wireless_set_encrkey(encrkey);
    edit_wireless_set_profilename(profilename);
}

///////////////////////////////////////////////////////////////////////////
// signal handlers
///////////////////////////////////////////////////////////////////////////

static void on_ssid_changed(GtkEntry* widget, gpointer data)
{
    CN_LOGPRINTF("entry");

    edit_wireless_set_status(inputSSID_e);
}

// Click 'Next' to detect the encryption type for this ssid automatically.
static void on_ssid_next(GtkToggleButton* button, gpointer data)
{
    gboolean   active;
    const char *ssid;
    scanContext_t *ctxt;

    CN_LOGPRINTF("entry");

    active = gtk_toggle_button_get_active(button);
    if (active)
    {
        ssid = edit_wireless_get_ssid();
        if (ssid && ssid[0] != '\0')
        {
            edit_wireless_restore_scan_ctxt();
            ctxt = cmgr_get_scan_ctxt();
            connect_scan_ctxt_set_ssidlist(ctxt, (char*)ssid);
            connect_scan(ctxt, TRUE);
        }
        else
        {
            g_timeout_add(500, delayed_deactive_button, button);
        }
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

static void on_encrtype_update(GtkWidget* selection, 
                               gpointer button, 
                               gpointer data)
{
    int        nsel;
    erGtkSelectionGroup *widget;

    CN_LOGPRINTF("entry");

    g_return_if_fail(g_encrtype != NULL);

    widget = ERGTK_SELECTION_GROUP(selection);
    ergtk_selection_group_get_selected_buttons(widget, &nsel, 1);
    switch (nsel)
    {
        case 0:
            edit_wireless_set_status(wlanInputProfileName_e);
            break;
        case 1:
            edit_wireless_set_status(inputEncrKeyWEP_e);
            break;
        case 2:
            edit_wireless_set_status(inputEncrKeyWPA_e);
            break;
        default:
            // encrtype is not selected yet
            edit_wireless_set_status(inputEncrType_e);
            break;
    }
}

static void on_encrkey_changed(GtkEntry *widget, gpointer data)
{
    wireless_encr_type_t encrtype;

    CN_LOGPRINTF("entry");

    encrtype = edit_wireless_get_encrtype();
    switch (encrtype)
    {
        case encr_wep_t:
            edit_wireless_set_status(inputEncrKeyWEP_e);
            break;
        case encr_wpa_t:
            edit_wireless_set_status(inputEncrKeyWPA_e);
            break;
        case encr_none_t:
        default:
            break;
    }
}

static void on_profilename_changed(GtkEntry* widget, gpointer data)
{
    CN_LOGPRINTF("entry");

    edit_wireless_set_status(wlanInputProfileName_e);
}

static gboolean on_delayed_profilename_next(gpointer data)
{
    CN_LOGPRINTF("entry");

    erbusy_blink();

    edit_wireless_backup_network_settings();

    edit_goto_page(editScreenOverview_e);
    edit_overview_update_info_text(inputProxy_e);
    edit_overview_update_profile();

    return FALSE;
}

// Click 'Next' to goto the page of filling in the advanced settings.
static void on_profilename_next(GtkToggleButton* button, gpointer data)
{
    gboolean active;
    gboolean valid;

    CN_LOGPRINTF("entry");

    active = gtk_toggle_button_get_active(button);
    if (active)
    {
        valid = edit_wireless_check_network_settings();
        if (valid)
        {
            // use timeout to show button is in active state
            g_timeout_add(500, on_delayed_profilename_next, NULL);
        }
        else
        {
            edit_wireless_determine_status();
        }

        g_timeout_add(500, delayed_deactive_button, button);
    }
}

static void on_buttons_update(GtkWidget *selection, 
                              gpointer button, 
                              gpointer data)
{
    gboolean   active;
    gboolean   valid;
    int        nsel;
    GtkWidget *widget;
    erGtkSelectionGroup *sel;
    gboolean  toDeactive = TRUE;

    CN_LOGPRINTF("entry");

    active = gtk_toggle_button_get_active(button);
    valid = edit_wireless_check_network_settings();
    if (!valid)
    {
        edit_wireless_determine_status();
    }

    if (active && valid)
    {
        widget = g_buttons->details->selection;
        sel = ERGTK_SELECTION_GROUP(widget);
        ergtk_selection_group_get_selected_buttons(sel, &nsel, 1);

        switch (nsel)
        {
            case 0:
                // test button is selected
                on_test();
                toDeactive = FALSE;
                break;
            case 1:
                // save button is selected
                on_save();
                toDeactive = FALSE;
                break;
            case 2:
                // save&connect button is selected
                on_saveconnect();
                toDeactive = FALSE;
                break;
            default:
                break;
        }
    }

    if (active && toDeactive)
    {
        g_timeout_add(500, delayed_deactive_button, button);
    }
}

static gboolean on_delayed_test(gpointer data)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");

    erbusy_blink();

    edit_wireless_backup_network_settings();

    edit_goto_page(editScreenOverview_e);

    edit_overview_update_profile();
    
    // similuate clicking the 'Test' button
    edit_overview_click_test();

    if (g_buttons)
    {
        button = g_buttons->details->buttonList[0];
        gtk_toggle_button_set_active(button, FALSE);
    }

    return FALSE;
}

static void on_test(void)
{
    CN_LOGPRINTF("entry");

    // use timeout to show button is in active state
    g_timeout_add(500, on_delayed_test, NULL);
}

static gboolean on_delayed_save(gpointer data)
{
    GtkToggleButton *button;
    
    CN_LOGPRINTF("entry");

    erbusy_blink();

    edit_save_profile();

    edit_goto_page(editScreenProfiles_e);
    if (g_buttons)
    {
        button = g_buttons->details->buttonList[1];
        gtk_toggle_button_set_active(button, FALSE);
    }

    return FALSE;
}

static void on_save(void)
{
    CN_LOGPRINTF("entry");

    // use timeout to show button is in active state
    g_timeout_add(500, on_delayed_save, NULL);
}

static void on_saveconnect(void)
{
    pingContext_t *ctxt;

    CN_LOGPRINTF("entry");

    erbusy_blink();

    edit_wireless_freeze_ui(TRUE);

    edit_wireless_restore_ping_ctxt();

    ctxt = cmgr_get_ping_ctxt();
    connect_ping_select_initial_profile(ctxt);
    connect_ping(ctxt, TRUE);
}

///////////////////////////////////////////////////////////////////////////
// functions about scan context
///////////////////////////////////////////////////////////////////////////
static void edit_wireless_restore_scan_ctxt(void)
{
    scanContext_t *ctxt;

    CN_LOGPRINTF("entry");

    ctxt = cmgr_get_scan_ctxt();
    if (ctxt)
    {
        connect_scan_ctxt_set_mode(ctxt, editScan_e);
        connect_scan_ctxt_set_network_type(ctxt, wireless_t);
        connect_scan_ctxt_set_done_callbacks(ctxt, 
                        scan_done, scan_done_after);
    }
}

static void scan_done(connection_t networkType, 
                      network_spec_t* pScanNetworks, 
                      int nScanNetworks)
{
    int        i;
    const char *ssid;

    CN_LOGPRINTF("entry");

    g_found_network = NULL;

    ssid = edit_wireless_get_ssid();
    if (pScanNetworks && (nScanNetworks != 0) && ssid)
    {
        for (i = 0; i < nScanNetworks; i++)
        {
           if (pScanNetworks[i].ssid 
               && (strcmp(pScanNetworks[i].ssid, ssid) == 0))
           {
               CN_LOGPRINTF("found [%d]", i);
               g_found_network = &pScanNetworks[i];
               break;
           }
        }
    }
}

static void scan_done_after(void)
{
    GtkToggleButton *button;
    wireless_encr_type_t encrtype;
    
    CN_LOGPRINTF("entry");

    // de-active the button
    if (g_ssid)
    {
        button = GTK_TOGGLE_BUTTON(g_ssid->details->button);
        gtk_toggle_button_set_active(button, FALSE);
    }

    CN_ERRORPRINTF("%p", g_found_network);

    if (g_found_network)
    {
        encrtype = g_found_network->encryption;
    }
    else
    {
        encrtype = encr_undefined_t;
    }
    
    edit_wireless_set_encrtype(encrtype);
    
    edit_wireless_determine_status();
    display_update_request_screen_refresh(STATUS_ITEM_CHANGE, 
                                          WAVEFORM_TYPING);
}

///////////////////////////////////////////////////////////////////////////
// functions about ping context
///////////////////////////////////////////////////////////////////////////
static void edit_wireless_restore_ping_ctxt(void)
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
        connect_ping_ctxt_set_done_callbacks(ctxt, 
                            on_connected, on_failed);
       
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
 
    edit_wireless_freeze_ui(FALSE);

    if (g_buttons)
    {
        button = g_buttons->details->buttonList[2];
        g_timeout_add(500, delayed_deactive_button, button);
    }

    edit_on_connected();    
}

static void on_failed(void)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");

    edit_wireless_freeze_ui(FALSE);

    if (g_buttons)
    {
        button = g_buttons->details->buttonList[2];
        g_timeout_add(500, delayed_deactive_button, button);
    }
}

static void ui_update_status(int profileIndex, pingStatus_t status)
{
    wlanStatus_t state;

    CN_LOGPRINTF("entry");

    switch (status)
    {
        case pingSucceeded_e:
            state = wlanConnected_e;
            break;
        case pingFailed_e:
        case pingFailedPCShare_e:
        case pingFailedNetwork_e:
            state = wlanUnconnected_e;
            break;
        case pingConnecting_e:
            state = wlanConnecting_e;
            break;
        case pingAborting_e:
            state  = wlanAborting_e;
            break;
        case pingAborted_e:
            state = wlanAborted_e;
            on_failed();
            break;
        default:
            state = undefWlanStatus_e;
            break;
    }

    edit_wireless_update_info_text(state);
    display_update_request_screen_refresh(STATUS_ITEM_CHANGE, 
                                          WAVEFORM_TYPING);
}

static void ui_display_settings(int profileIndex, gboolean resetStatus)
{
    CN_LOGPRINTF("entry");
    // nothing to do
}

static void ui_select_profile(int profileIndex)
{
    CN_LOGPRINTF("entry");
    // nothing to do
}

static void ui_unselect_all_profile(void)
{
    CN_LOGPRINTF("entry");
    // nothing to do
}

static void edit_wireless_click_ssid_next(void)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");

    g_return_if_fail(g_ssid != NULL);
    
    button = GTK_TOGGLE_BUTTON(g_ssid->details->button);
    gtk_toggle_button_set_active(button, TRUE);
}

static void edit_wireless_click_encr_none(void)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");

    g_return_if_fail(g_encrtype != NULL);
    
    button = g_encrtype->details->buttonList[0];
    gtk_toggle_button_set_active(button, TRUE);
}

static void edit_wireless_click_saveconnect(void)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");

    g_return_if_fail(g_buttons != NULL);

    button = g_buttons->details->buttonList[2];
    gtk_toggle_button_set_active(button, TRUE);
}

gboolean on_edit_wireless_keypress(GdkEventKey *event)
{
    pingContext_t *ctxt;
    const char *encrkey = NULL;
    gboolean ret = FALSE;

    CN_LOGPRINTF("entry");

    ctxt = cmgr_get_ping_ctxt();
    switch (event->keyval)
    {
        case GDK_Return:
            // 'Enter' key is pressed
            if (!connect_ping_in_pinging(ctxt))
            {
                switch (g_wlan_status)
                {
                    case inputSSID_e:
                        // simulate clicking the 'Next' button 
                        // of g_ssid
                        edit_wireless_click_ssid_next();
                        break;
                    case inputEncrType_e:
                        // simulate clicking the encr_none_t button
                        edit_wireless_click_encr_none();
                        break;
                    case inputEncrKeyWEP_e:
                    case inputEncrKeyWPA_e:
                        encrkey = edit_wireless_get_encrkey();
                        if (encrkey && (encrkey[0] != '\0'))
                        {
                            // move to the next field(profile name)
                            edit_wireless_set_status(wlanInputProfileName_e);
                            display_update_request_screen_refresh(STATUS_ITEM_CHANGE, 
                                                              WAVEFORM_TYPING);

                        }
                        break;
                    case wlanInputProfileName_e:
                    case undefWlanStatus_e:
                        // simulate clicking the 'Save & Connect' button
                        edit_wireless_click_saveconnect();
                        break;
                    default:
                        break;
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
        case GDK_Down:
        case GDK_Up:
            // avoid the standard GTK behaviour for GtkEntries
            erbusy_off();
            ret = TRUE;
        default:
            erbusy_off();
    }
    
    return ret;
}

///////////////////////////////////////////////////////////////////////////
// functions for freezing the ui when in ping-to-server state
///////////////////////////////////////////////////////////////////////////

static void edit_wireless_freeze_ui(gboolean freeze)
{
    editMode_t mode;
    gboolean sensitive;
    GtkWidget *widget;

    CN_LOGPRINTF("entry[%d]", freeze);

    sensitive = !freeze;

    // the ssid
    widget = g_ssid->details->entry;
    gtk_widget_set_sensitive(widget, sensitive);

    // the encryption type
    widget = GTK_WIDGET(g_encrtype->details->buttonList[0]);
    gtk_widget_set_sensitive(widget, sensitive);

    widget = GTK_WIDGET(g_encrtype->details->buttonList[1]);
    gtk_widget_set_sensitive(widget, sensitive);

    widget = GTK_WIDGET(g_encrtype->details->buttonList[2]);
    gtk_widget_set_sensitive(widget, sensitive);

    // the encryption key
    widget = g_encrkey->details->entry;
    gtk_widget_set_sensitive(widget, sensitive);

    // the profilename
    widget = g_profilename->details->entry;
    gtk_widget_set_sensitive(widget, sensitive);

    widget = g_profilename->details->button;
    gtk_widget_set_sensitive(widget, sensitive);
    
    // the buttons
    widget = GTK_WIDGET(g_buttons->details->buttonList[0]);
    gtk_widget_set_sensitive(widget, sensitive);
    
    widget = GTK_WIDGET(g_buttons->details->buttonList[1]);
    gtk_widget_set_sensitive(widget, sensitive);

    widget = GTK_WIDGET(g_buttons->details->buttonList[2]);
    gtk_widget_set_sensitive(widget, sensitive);
 
    if (sensitive)
    {
        // restore the previous in-sensitive widgets
        // save button
        mode = edit_get_mode();
        if ((mode == editModeConnectFillIn_e)
            || (mode == editModeConnectCreate_e)
            || (mode == editModeConnectNormal_e))
        {
            edit_wireless_enable_save_button(FALSE);
        }
    }
}

