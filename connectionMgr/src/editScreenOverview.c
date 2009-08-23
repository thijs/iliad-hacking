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
 * \file editScreenOverview.c
 * \brief connectionMgr - "edit network configuration overview" screen
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include "config.h"
#include <string.h>

#include <gtk/gtk.h>

#include <liberdm/connectionMgrConstants.h>
#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "erbusy.h"
#include "connectionMgr.h"
#include "gtkSettingItem.h"
#include "gtkInfoItem.h"
#include "gtkProfileEntry.h"
#include "widgetUtils.h"
#include "commonData.h"
#include "connectScreenData.h"
#include "editScreenData.h"
#include "editScreen.h"
#include "editScreenOverview.h"
#include "editScreenWireless.h"
#include "editScreenWirelessExt.h"
#include "editScreenDialup.h"
#include "editScreenStatic.h"
#include "editScreenProxy.h"
#include "connectPing.h"
#include "connectBackground.h"
#include "connectionMgr.h"
#include "background.h"
#include "languages.h"

static GtkWidget *g_overview = NULL;

static bkProfileEntry   *g_profile_entry = NULL;
static bkWdtLabelButton *g_address = NULL;
static bkWdtLabelButton *g_proxy = NULL;
static bkWdtLabelEntry  *g_profilename = NULL;
static bkWdtLabelButton *g_buttons = NULL;

static GtkWidget *g_info = NULL;

static ip_address_mode_t edit_overview_get_address(void);
static gboolean edit_overview_get_proxy(void);
static const char *edit_overview_get_profilename(void);

static gboolean on_delayed_edit_goto_page(gpointer new_page);
static gboolean delayed_deactive_button(gpointer data);

static gboolean on_button_press(GtkWidget *button, 
                                GdkEventButton *event, 
                                gpointer data);
static gboolean on_profile_entry_button_press(GtkWidget *widget, 
                                              GdkEventButton *event, 
                                              gpointer data);
static void on_ip_address_mode_update(GtkWidget *selection, 
                                      gpointer button, 
                                      gpointer data);
static void on_proxy_onoff_update(GtkWidget *selection, 
                                  gpointer button, 
                                  gpointer data);
static void on_profilename_changed(GtkEntry *widget, gpointer data);
static void on_buttons_update(GtkWidget *selection, 
                              gpointer button, 
                              gpointer data);

static void on_test(void);
static void on_save(void);
static void on_saveconnect(void);

static void edit_overview_restore_ping_ctxt(gboolean toTest);
static void on_test_connected(int profileIndex);
static void on_test_failed(void);
static void on_connected(int profileIndex);
static void on_failed(void);
static void ui_test_update_status(int profileIndex, pingStatus_t status);
static void ui_update_status(int profileIndex, pingStatus_t status);
static void ui_display_settings(int profileIndex, gboolean resetStatus);
static void ui_select_profile(int profileIndex);
static void ui_unselect_all_profile(void);

static void edit_overview_freeze_ui(gboolean freeze);

///////////////////////////////////////////////////////////////////////////
// create widgets 
///////////////////////////////////////////////////////////////////////////

// |--item (gtkSettingItem)
//    |--vbox
//       |--profileEntry
//       |--ipAddressMode
//       |--proxyOnOff
//       |--profileName
//       |--buttons
//       |--info
//
GtkWidget* edit_overview_create(void)
{
    GtkWidget *item;
    GtkWidget *vbox;
    bkProfileEntry   *profileEntry;
    bkWdtLabelButton *ipAddressMode;
    bkWdtLabelButton *proxyOnOff;
    bkWdtLabelEntry  *profileName;
    bkWdtLabelButton *buttons;
    GtkWidget *info;
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    // item
    item = gtk_settingitem_new("");

    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), vbox);

    // profileEntry
    profileEntry = bk_profile_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), profileEntry->background, 
                       FALSE, FALSE, 0);

    // ipAddressMode
    ipAddressMode = bk_wdt_label_button_new(2, FALSE, 1, 1);
    gtk_box_pack_start(GTK_BOX(vbox), ipAddressMode->background, 
                       FALSE, FALSE, 0);

    // proxyOnOff
    proxyOnOff = bk_wdt_label_button_new(2, FALSE, 1, 1);
    gtk_box_pack_start(GTK_BOX(vbox), proxyOnOff->background, 
                       FALSE, FALSE, 0);

    // profilename
    profileName = bk_wdt_label_entry_new(string_e);
    gtk_box_pack_start(GTK_BOX(vbox), profileName->background, 
                       FALSE, FALSE, 0);

    // buttons
    buttons = bk_wdt_label_button_new(3, FALSE, 0, 1);
    gtk_box_pack_start(GTK_BOX(vbox), buttons->background, 
                       FALSE, FALSE, 0);

    // info
    info = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), info, FALSE, FALSE, 0);

    // signal handlers
    widget = profileEntry->background;
    g_signal_connect(G_OBJECT(widget), "button-press-event",
                 G_CALLBACK(on_button_press), (gpointer)inputDetails_e);
    widget = ipAddressMode->background;
    g_signal_connect(G_OBJECT(widget), "button-press-event",
                 G_CALLBACK(on_button_press), (gpointer)inputAddress_e);
    widget = proxyOnOff->background;
    g_signal_connect(G_OBJECT(widget), "button-press-event",
                 G_CALLBACK(on_button_press), (gpointer)inputProxy_e);
    widget = profileName->background;
    g_signal_connect(G_OBJECT(widget), "button-press-event",
             G_CALLBACK(on_button_press), (gpointer)inputProfileName_e);

    widget = gtk_profile_entry_get_namebutton(GTK_PROFILE_ENTRY(profileEntry->details));
    g_signal_connect(G_OBJECT(widget), "button-press-event", 
                     G_CALLBACK(on_profile_entry_button_press), NULL);

    widget = ipAddressMode->details->selection;
    g_signal_connect(G_OBJECT(widget), "selection-update",
                     G_CALLBACK(on_ip_address_mode_update), NULL);

    widget = proxyOnOff->details->selection;
    g_signal_connect(G_OBJECT(widget), "selection-update",
                     G_CALLBACK(on_proxy_onoff_update), NULL);

    widget = profileName->details->entry;
    g_signal_connect_after(G_OBJECT(widget), "changed-stable", 
                           G_CALLBACK(on_profilename_changed), NULL);

    widget = buttons->details->selection;
    g_signal_connect(G_OBJECT(widget), "selection-update",
                     G_CALLBACK(on_buttons_update), NULL);

    // show widgets
    gtk_widget_show(item);
    gtk_widget_show(vbox);
    gtk_widget_show(profileEntry->background);
    gtk_widget_show(ipAddressMode->background);
    gtk_widget_show(proxyOnOff->background);
    gtk_widget_show(profileName->background);
    gtk_widget_hide(profileName->details->button);
    gtk_widget_grab_focus(profileName->details->entry);
    gtk_widget_show(buttons->background);
    gtk_widget_show(info);

    // global variables
    g_overview = item;
    g_profile_entry = profileEntry;
    g_address = ipAddressMode;
    g_proxy = proxyOnOff;
    g_profilename = profileName;
    g_buttons = buttons;
    g_info = info;

    CN_LOGPRINTF("done");

    // return
    return item;
}

///////////////////////////////////////////////////////////////////////////
// set text 
///////////////////////////////////////////////////////////////////////////
void edit_overview_set_text(void)
{
    CN_LOGPRINTF("entry");
    
    GtkWidget *widget;

    if (g_overview)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_overview),
                                        _("Network profile"));
    }

    if (g_address)
    {
        widget = g_address->details->label;
        gtk_label_set_text(GTK_LABEL(widget),
                _("Select how your iLiad gets a network address."));

        widget = GTK_WIDGET(g_address->details->buttonList[0]);
        gtk_button_set_label(GTK_BUTTON(widget), _("Automatic"));
        
        widget = GTK_WIDGET(g_address->details->buttonList[1]);
        gtk_button_set_label(GTK_BUTTON(widget), _("Static"));
    }

    if (g_proxy)
    {
        widget = g_proxy->details->label;
        gtk_label_set_text(GTK_LABEL(widget),
                _("Select if there is a proxy server present."));

        widget = GTK_WIDGET(g_proxy->details->buttonList[0]);
        gtk_button_set_label(GTK_BUTTON(widget), _("No proxy"));

        widget = GTK_WIDGET(g_proxy->details->buttonList[1]);
        gtk_button_set_label(GTK_BUTTON(widget), _("Proxy")); 
    }

    if (g_profilename)
    {
        widget = g_profilename->details->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                _("You can personalize the profile name."));
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

    edit_overview_update_info_text(inputDetails_e);
}

void edit_overview_update_info_text(overviewStatus_t status)
{
    CN_LOGPRINTF("entry");

    g_return_if_fail(g_info != NULL);

    switch (status)
    {
        case inputDetails_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("A wireless or dial-up network requires "
                   "specific settings.\n"
                   "Hint: You can fill in the details "
                   "for them by clicking the profile entry."));
             break;
        case inputAddress_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("An automatic network address requires a DHCP server "
                  "to be present "
                  "in your network. A static network address requires "
                  "specific settings.\n"
                  "Hint: If you are not sure, select 'Automatic' "
                  "and 'No proxy'."));
           break;
        case inputProxy_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("A proxy server regulates internet traffic "
                  "and is usually installed "
                  "in company networks, "
                  "this requires additional settings.\n"
                  "Hint: If you are not sure, select 'No proxy'."));
            break;
        case inputProfileName_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("If you personalize the profile name, "
                  "it will be easy to recognize "
                  "in the iLiad's network profile list."));
            break;
        case toSaveConnect_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("The profile has been saved. "
                  "The iLiad is trying to connect to the server."));
            break;
        case toSave_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                    _("The profile will be saved."));
            break;
        case testing_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e);
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("The iLiad is testing your network profile "
                  "and is trying to make a connection to the server."));
            break;
        case testAborting_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                _("The iLiad is aborting the test."));
            break;
        case testAborted_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e);
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                _("The test has been aborted."));
            break;
        case testSucceeded_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconOk_e);
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("Connection to the server "
                  "has been established correctly.\n"
                  "Press the 'Save & Connect' button "
                  "to create a connection."));
            break;
        case testFailed_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconFailed_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("Connection to the server "
                      "can not be established correctly."));
            break;
        case connecting_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e);
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("The iLiad is trying to connect to the server."));
            break;
        case connectAborting_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                _("The iLiad is aborting the connection."));
            break;
        case connectAborted_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e);
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                _("The connection has been aborted."));
            break;
        case connectSucceeded_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconOk_e);
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("Connection to the server established."));
            break;
        case connectFailed_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconFailed_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("Connection to the server "
                      "can not be established correctly."));
            break;
        default:
            break;
    }
}

void edit_overview_update_profile(void)
{
    regNetworkProfile_t *settings;

    CN_LOGPRINTF("entry");

    settings = edit_get_network_settings();
    edit_overview_set_profile(settings);
    erRegFreeNetworkProfile(settings);
}


void edit_overview_show_profilename(void)
{
    connection_t networkType;

    CN_LOGPRINTF("entry");

    g_return_if_fail(g_profilename != NULL);
    
    networkType = edit_data_get_network_type();
    switch (networkType)
    {
        case wireless_t:
        case dialup_t:
            gtk_widget_hide(g_profilename->background);
            break;
        case wired_t:
        default:
            gtk_widget_show(g_profilename->background);
            break;
    }
}

void edit_overview_enable_save_button(gboolean enable)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry %d", enable);
    
    if (g_buttons)
    {
        widget = GTK_WIDGET(g_buttons->details->buttonList[1]);
        gtk_widget_set_sensitive(widget, enable);
    }
}

///////////////////////////////////////////////////////////////////////////
// get and set settings data 
///////////////////////////////////////////////////////////////////////////
void edit_overview_set_network_settings(const regNetworkProfile_t*settings)
{
    CN_LOGPRINTF("entry");
    
    g_return_if_fail(settings != NULL);

    edit_overview_set_profile(settings);
    edit_overview_set_address(settings->addressMode);
    edit_overview_set_proxy(settings->proxy);
    edit_overview_set_profilename(settings->name);
}

void edit_overview_set_profile(const regNetworkProfile_t* settings)
{
    connection_t mode;
    GtkWidget   *widget;

    CN_LOGPRINTF("entry");
     
    g_return_if_fail(settings != NULL);

    // show the current selection
    mode = settings->connection;
    switch (mode)
    {
        case wired_t:
            // wired profile: static ip allowed
            if (g_address)
            {
                widget = GTK_WIDGET(g_address->details->buttonList[1]);
                gtk_widget_set_sensitive(widget, TRUE);
            }
            break;
        case wireless_t:
        case dialup_t:
           // not a wired profile: force to DHCP and hide the static button
           if (g_address)
           {
               widget = GTK_WIDGET(g_address->details->buttonList[0]);
               gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), 
                                            TRUE);

               widget = GTK_WIDGET(g_address->details->buttonList[1]);
               gtk_widget_set_sensitive(widget, FALSE);
           }
           break;
        case connection_undefined_t:
        default:
            CN_ERRORPRINTF("Illegal wired mode [%d]", mode);
            g_assert_not_reached();
    }

    bk_profile_entry_display(g_profile_entry, settings);
}

void edit_overview_set_address(const ip_address_mode_t addressMode)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry");
   
    g_return_if_fail(g_address != NULL);
    
    switch (addressMode)
    {
        case dhcp_t:
        case ip_address_mode_undefined_t:
            widget = GTK_WIDGET(g_address->details->buttonList[0]);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
            break;
        case static_t:
            widget = GTK_WIDGET(g_address->details->buttonList[1]);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
            break;
        default:
            CN_ERRORPRINTF("Illegal dhcp mode [%d]", addressMode);
            g_assert_not_reached();
    }
}

void edit_overview_set_proxy(const gboolean use_proxy)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry: use_proxy [%d]", use_proxy);

    g_return_if_fail(g_proxy != NULL);

    if (use_proxy == FALSE)
    {
        widget = GTK_WIDGET(g_proxy->details->buttonList[0]);
    }
    else
    {
        widget = GTK_WIDGET(g_proxy->details->buttonList[1]);
    }

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
}

void edit_overview_set_profilename(const gchar* profilename)
{
    GtkWidget *widget;
    connection_t networkType;
    editMode_t mode;

    CN_LOGPRINTF("entry profilename[%s]", profilename);

    networkType = edit_data_get_network_type();
    switch (networkType)
    {
        case wired_t:
            if (g_profilename)
            {
                widget = g_profilename->details->entry;
                gtk_entry_set_text(GTK_ENTRY(widget), 
                        profilename ? profilename : "");
            }
            break;
        case wireless_t:
            mode = edit_get_mode();
            if (mode == editModeConnectFillIn_e)
            {
                if (g_profilename)
                {
                    widget = g_profilename->details->entry;
                    gtk_entry_set_text(GTK_ENTRY(widget), 
                            profilename ? profilename : "");
                }
            }
            else
            {
                edit_wireless_set_profilename(profilename);
            }
            break;
        case dialup_t:
            edit_dialup_set_profilename(profilename);
            break;
        default:
            break;
    }
}

void edit_overview_get_network_settings(regNetworkProfile_t* settings)
{
    const gchar *profilename;

    CN_LOGPRINTF("entry");

    g_return_if_fail(settings != NULL);
   
    // connection   
    settings->connection = edit_data_get_network_type();
  
    // address mode
    settings->addressMode = edit_overview_get_address();
    
    // proxy
    settings->proxy = edit_overview_get_proxy();
    
    // profile name
    profilename = edit_overview_get_profilename();
    g_free(settings->name);
    settings->name = g_strdup(profilename);
}

static ip_address_mode_t edit_overview_get_address(void)
{
    GtkWidget *widget;
    erGtkSelectionGroup *item;
    int nsel;
    ip_address_mode_t mode;

    CN_LOGPRINTF("entry");

    g_return_val_if_fail(g_address != NULL, dhcp_t);
    
    widget = g_address->details->selection;
    item = ERGTK_SELECTION_GROUP(widget);
    
    ergtk_selection_group_get_selected_buttons(item, &nsel, 1); 
    if (nsel == 0)
    {
        mode = dhcp_t;
    }
    else
    {
        mode = static_t;
    }

    CN_LOGPRINTF("return mode=%d", mode);
    return mode;
}

static gboolean edit_overview_get_proxy(void)
{
    GtkWidget *widget;
    erGtkSelectionGroup *item;
    int nsel;
    gboolean useproxy;

    CN_LOGPRINTF("entry");

    g_return_val_if_fail(g_proxy != NULL, FALSE);

    widget = g_proxy->details->selection;
    item = ERGTK_SELECTION_GROUP(widget);

    ergtk_selection_group_get_selected_buttons(item, &nsel, 1);
    if (nsel == 0)
    {
        useproxy = FALSE;
    }
    else
    {
        useproxy = TRUE;
    }

    CN_LOGPRINTF("return useproxy=%d", useproxy);
    return useproxy;
}

static const char *edit_overview_get_profilename(void)
{
    GtkWidget *widget;
    const char *profilename = NULL;
    connection_t networkType;
    editMode_t mode;

    CN_LOGPRINTF("entry");

    networkType = edit_data_get_network_type();
    switch (networkType)
    {
        case wired_t:
            if (g_profilename)
            {
                widget = g_profilename->details->entry;
                profilename = gtk_entry_get_text(GTK_ENTRY(widget));
            }
            break;
        case wireless_t:
            mode = edit_get_mode();
            if (mode == editModeConnectFillIn_e)
            {
                if (g_profilename)
                {
                    widget = g_profilename->details->entry;
                    profilename = gtk_entry_get_text(GTK_ENTRY(widget));
                }
            }
            else
            {
                profilename = edit_wireless_get_profilename();
            }
            break;
        case dialup_t:
            profilename = edit_dialup_get_profilename();
            break;
        default:
            break;
    }

    CN_LOGPRINTF("return profilename=%s", profilename);
    return profilename;
}

gboolean edit_overview_check_network_settings(void)
{
    editMode_t mode;
    connection_t networkType;
    ip_address_mode_t address;
    gboolean useproxy;
    erGtkEntry *profilename;
    gboolean valid = TRUE;

    CN_LOGPRINTF("entry");

    networkType =  edit_data_get_network_type();
    address = edit_overview_get_address();
    useproxy = edit_overview_get_proxy();
    profilename = ERGTK_ENTRY(g_profilename->details->entry);

    switch (networkType)
    {
        case wired_t:
            if ((address < 0) || (address >= ip_address_mode_undefined_t)
                || (!ergtk_entry_check_field(profilename)))
            {
                valid = FALSE;
            }
            break;
        case wireless_t:
            mode = edit_get_mode();
            if (mode == editModeConnectFillIn_e)
            {
                valid = edit_wireless_ext_check_network_settings();
            }
            else
            {
                valid = edit_wireless_check_network_settings();
            }
            break;
        case dialup_t:
            valid = edit_dialup_check_network_settings();
            break;
        default:
            CN_ERRORPRINTF("unknown networkType[%d]", networkType);
            g_assert_not_reached();
            break;
    }
    g_return_val_if_fail(valid == TRUE, valid);

    // check the static ip address
    if (address == static_t)
    {
        valid = edit_static_check_network_settings();
    }
    g_return_val_if_fail(valid == TRUE, valid);
    
    // check the proxy settings
    if (useproxy)
    {
        valid = edit_proxy_check_network_settings();
    }
    g_return_val_if_fail(valid == TRUE, valid);

    return valid;
}

///////////////////////////////////////////////////////////////////////////
// signal handlers
///////////////////////////////////////////////////////////////////////////
static gboolean on_button_press(GtkWidget* button, 
                                GdkEventButton* event, 
                                gpointer data)
{
    overviewStatus_t status = (overviewStatus_t)data;

    CN_LOGPRINTF("entry");
    
    edit_overview_update_info_text(status);

    display_update_request_screen_refresh (STATUS_ITEM_CHANGE, 
                                           WAVEFORM_TYPING);

    return FALSE;
}

static gboolean on_profile_entry_button_press(GtkWidget *widget, 
                                              GdkEventButton *event, 
                                              gpointer data)
{
    CN_LOGPRINTF("entry");

    GtkToggleButton *button;
    gboolean active;
    connection_t networktype;    
    editMode_t mode;

    button = GTK_TOGGLE_BUTTON(widget);
    active = gtk_toggle_button_get_active(button);
    if (!active)
    {
        if (!edit_get_in_set_network_settings())
        {
            erbusy_blink();

            networktype = edit_data_get_network_type();
            switch (networktype)
            {
                case wired_t:
                    // profile for wired, nothing need to do
                    break;
                case wireless_t:
                    edit_set_back_overview(TRUE);
                    // profile for wireless, open the edit wireless screen
                    mode = edit_get_mode();
                    if (mode == editModeConnectFillIn_e)
                    {
                        g_timeout_add(500, 
                                on_delayed_edit_goto_page, 
                                (gpointer)(editScreenWirelessExt_e + 1));

                    }
                    else
                    {
                        g_timeout_add(500, 
                                on_delayed_edit_goto_page, 
                                (gpointer)(editScreenWireless_e + 1));
                    }
                    break;
                case dialup_t:
                    edit_set_back_overview(TRUE);
                    // profile for dialup, open the edit dialup screen
                    g_timeout_add(500, 
                            on_delayed_edit_goto_page, 
                            (gpointer)(editScreenDialup_e + 1));
                    break;
                default:
                    break;
            }
        }

        // de-active the button
        g_timeout_add(500, delayed_deactive_button, button);
    }

    edit_overview_update_info_text(inputDetails_e);

    return FALSE;
}

// signal handler for page goto
static gboolean on_delayed_edit_goto_page(gpointer new_page)
{
    erbusy_blink();

    // switch page           
    on_edit_goto_page((guint)new_page);

    // don't call again
    return FALSE;  
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

static void on_ip_address_mode_update(GtkWidget* selection, 
                                      gpointer button, 
                                      gpointer data)
{
    // open the static screen
    gboolean active;
    int        nsel;
    erGtkSelectionGroup *sel;
    const char *profilename;
   
    CN_LOGPRINTF("entry");

    g_return_if_fail(g_address != NULL);

    active = gtk_toggle_button_get_active(button);
    if (active)
    {
        if (!edit_get_in_set_network_settings())
        {
            sel = ERGTK_SELECTION_GROUP(selection);
            ergtk_selection_group_get_selected_buttons(sel, &nsel, 1);
            if (nsel == 1)
            {
                erbusy_blink();

                // update the title for edit static screen 
                // before going to this page
                profilename = edit_overview_get_profilename();
                edit_static_set_title(profilename);

                g_timeout_add(500, 
                        on_delayed_edit_goto_page, 
                        (gpointer)(editScreenStatic_e + 1));
            }
        }
    }
    edit_overview_update_info_text(inputAddress_e);
}

static void on_proxy_onoff_update(GtkWidget* selection, 
                                  gpointer button, 
                                  gpointer data)
{
    // open the proxy screen
    gboolean   active;
    int        nsel;
    erGtkSelectionGroup *sel;
    const char* profilename;
   
    CN_LOGPRINTF("entry");

    g_return_if_fail(g_proxy != NULL);

    active = gtk_toggle_button_get_active(button);
    if (active)
    {
        if (!edit_get_in_set_network_settings())
        {
            sel = ERGTK_SELECTION_GROUP(selection);
            ergtk_selection_group_get_selected_buttons(sel, &nsel, 1);
            if (nsel == 1)
            {
                erbusy_blink();

                // update the title for edit static screen 
                // before going to this page
                profilename = edit_overview_get_profilename();
                edit_proxy_set_title(profilename);

                g_timeout_add(500, 
                        on_delayed_edit_goto_page, 
                        (gpointer)(editScreenProxy_e + 1));
            }
        }
    }

    edit_overview_update_info_text(inputProxy_e);
}

static void on_profilename_changed(GtkEntry* widget, gpointer data)
{
    CN_LOGPRINTF("entry");

    if (!edit_get_in_set_network_settings())
    {
        // update the profile entry as well
        edit_overview_update_profile();

        // update the info text    
        edit_overview_update_info_text(inputProfileName_e);
    }
}

static void on_buttons_update(GtkWidget* selection, gpointer button, gpointer data)
{
    gboolean   active;
    gboolean   valid;
    int        nsel;
    erGtkSelectionGroup *widget;
    gboolean toDeactive = TRUE;

    CN_LOGPRINTF("entry");

    active = gtk_toggle_button_get_active(button);
    valid = edit_overview_check_network_settings();
    if (active && valid)
    {
        widget = ERGTK_SELECTION_GROUP(g_buttons->details->selection);
        ergtk_selection_group_get_selected_buttons(widget, &nsel, 1);
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

// 'Test' button clicked
static void on_test(void)
{
    pingContext_t *ctxt;

    CN_LOGPRINTF("entry");

    edit_overview_freeze_ui(TRUE);

    edit_overview_restore_ping_ctxt(TRUE);

    ctxt = cmgr_get_ping_ctxt();
    connect_ping_select_initial_profile(ctxt);
    connect_ping(ctxt, TRUE); 
}

static gboolean on_delayed_save(gpointer data)
{
    GtkToggleButton *button;

    erbusy_blink();

    edit_save_profile();
    
    edit_goto_page(editScreenProfiles_e);

    // de-active the button
    if (g_buttons)
    {
        button = g_buttons->details->buttonList[1];
        gtk_toggle_button_set_active(button, FALSE);
    }
    
    return FALSE;
}

// 'Save' button clicked
static void on_save(void)
{
    CN_LOGPRINTF("entry");
   
    edit_overview_update_info_text(toSave_e);

    g_timeout_add(500, on_delayed_save, NULL);
}

static gboolean on_delayed_saveconnect(gpointer data)
{
    pingContext_t *ctxt;

    erbusy_blink();

    edit_overview_freeze_ui(TRUE);

    edit_overview_restore_ping_ctxt(FALSE);

    ctxt = cmgr_get_ping_ctxt();
    connect_ping_select_initial_profile(ctxt);
    connect_ping(ctxt, TRUE);

    return FALSE;
}

// 'Save&Connect' button clicked
static void on_saveconnect(void)
{
    CN_LOGPRINTF("entry");

    edit_overview_update_info_text(toSaveConnect_e);

    g_timeout_add(500, on_delayed_saveconnect, NULL);
}

void edit_overview_click_test(void)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    if (g_buttons)
    {
        widget = GTK_WIDGET(g_buttons->details->buttonList[0]);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
    }
}

void edit_overview_click_save(void)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    if (g_buttons)
    {
        widget = GTK_WIDGET(g_buttons->details->buttonList[1]);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
    }
}

void edit_overview_click_saveconnect(void)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    if (g_buttons)
    {
        widget = GTK_WIDGET(g_buttons->details->buttonList[2]);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
    }
}

///////////////////////////////////////////////////////////////////////////
// functions about ping context
///////////////////////////////////////////////////////////////////////////
static void edit_overview_restore_ping_ctxt(gboolean toTest)
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
        if (toTest)
        {
            connect_ping_ctxt_set_done_callbacks(ctxt, 
                        on_test_connected, on_test_failed);
        }
        else
        {
            connect_ping_ctxt_set_done_callbacks(ctxt, 
                                on_connected, on_failed);
        }

        // data access callbacks
        connect_ping_ctxt_set_get_initial_profile(ctxt, 
                            edit_get_initial_profile);
        connect_ping_ctxt_set_access_network_profiles_callbacks(ctxt,
                                    edit_get_n_network_profiles,
                                    edit_get_network_profile,
                                    edit_select_next_regprofile);

        // ui callbacks
        if (toTest)
        {
            connect_ping_ctxt_set_ui_callbacks(ctxt, 
                                        ui_test_update_status, 
                                        ui_display_settings,
                                        ui_select_profile,
                                        ui_unselect_all_profile);

        }
        else
        {
            connect_ping_ctxt_set_ui_callbacks(ctxt, 
                                        ui_update_status, 
                                        ui_display_settings,
                                        ui_select_profile,
                                        ui_unselect_all_profile);
        }
    }
}

static void deactive_button(void)
{
    erGtkSelectionGroup *item;
    int nsel;
    GtkToggleButton *button;

    // deactive the button
    if (g_buttons)
    {
        item = ERGTK_SELECTION_GROUP(g_buttons->details->selection);
        ergtk_selection_group_get_selected_buttons(item, &nsel, 1);
        if ((nsel >= 0) && (nsel <= 2)) 
        {
            button = g_buttons->details->buttonList[nsel];
            g_timeout_add(500, delayed_deactive_button, button);
        }
    }
}

static void on_test_connected(int profileIndex)
{
    CN_LOGPRINTF("entry");
   
    edit_overview_freeze_ui(FALSE);

    deactive_button();
}

static void on_test_failed(void)
{
    CN_LOGPRINTF("entry");

    edit_overview_freeze_ui(FALSE);

    deactive_button();
}

static void on_connected(int profileIndex)
{
    CN_LOGPRINTF("entry");

    edit_save_profile();

    edit_overview_freeze_ui(FALSE);

    deactive_button();

    edit_on_connected();    
}

static void on_failed(void)
{
    CN_LOGPRINTF("entry");

    edit_overview_freeze_ui(FALSE);

    deactive_button();
}

static void ui_test_update_status(int profileIndex, pingStatus_t status)
{
    overviewStatus_t state;

    CN_LOGPRINTF("entry");

    bk_profile_entry_update_status(g_profile_entry, status);

    switch (status)
    {
        case pingSucceeded_e:
            state = testSucceeded_e;
            break;
        case pingFailed_e:
        case pingFailedPCShare_e:
        case pingFailedNetwork_e:
            state = testFailed_e;
            break;
        case pingConnecting_e:
            state = testing_e;
            break;
        case pingAborting_e:
            state = testAborting_e;
            break;
        case pingAborted_e:
            state = testAborted_e;
            on_failed();
            break;
        default:
            state = undefinedStatus_e;
            break;
    }
    
    edit_overview_update_info_text(state);

    display_update_request_screen_refresh (STATUS_ITEM_CHANGE, 
                                           WAVEFORM_TYPING);
}

static void ui_update_status(int profileIndex, pingStatus_t status)
{
    overviewStatus_t state;

    CN_LOGPRINTF("entry");

    bk_profile_entry_update_status(g_profile_entry, status);

    switch (status)
    {
        case pingSucceeded_e:
            state = connectSucceeded_e;
            break;
        case pingFailed_e:
        case pingFailedPCShare_e:
        case pingFailedNetwork_e:
            state = connectFailed_e;
            break;
        case pingConnecting_e:
            state = connecting_e;
            break;
        case pingAborting_e:
            state = connectAborting_e;
            break;
        case pingAborted_e:
            state = connectAborted_e;
            on_failed();
            break;
        default:
            state = undefinedStatus_e;
            break;
    }
    
    edit_overview_update_info_text(state);

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

///////////////////////////////////////////////////////////////////////////
// key-press handlers
///////////////////////////////////////////////////////////////////////////
// signal handlers for key input
gboolean on_edit_overview_keypress(GdkEventKey *event)
{
    pingContext_t *ctxt;
    gboolean ret = FALSE;

    CN_LOGPRINTF("entry");
    
    ctxt = cmgr_get_ping_ctxt();

    switch (event->keyval)
    {
        case GDK_Return:
            // 'Enter' key is pressed
            if (!connect_ping_in_pinging(ctxt))
            {
                // simulate clicking 'Save & Connect' button
                edit_overview_click_saveconnect();
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
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////
// functions for freezing the ui when in ping-to-server state
///////////////////////////////////////////////////////////////////////////

// the way to freeze the whole ui:
// for gtk button, set the widget to be insensitive 
// for gtk entry, set the entry to be insensitive
// for toolbar icon, return immediately in handlers 
// for keys, return immediately in handlers

static void edit_overview_freeze_ui(gboolean freeze)
{
    GtkProfileEntry *item;
    editMode_t mode;
    connection_t networkType;
    gboolean sensitive;
    GtkWidget *widget;

    CN_LOGPRINTF("entry[%d]", freeze);

    sensitive = !freeze;

    // the name button of profile entry
    item = GTK_PROFILE_ENTRY(g_profile_entry->details);
    widget = gtk_profile_entry_get_namebutton(item);
    gtk_widget_set_sensitive(widget, sensitive);

    // ipaddress mode buttons
    widget = GTK_WIDGET(g_address->details->buttonList[0]);
    gtk_widget_set_sensitive(widget, sensitive);
    
    widget = GTK_WIDGET(g_address->details->buttonList[1]);
    gtk_widget_set_sensitive(widget, sensitive);

    // proxy buttons
    widget = GTK_WIDGET(g_proxy->details->buttonList[0]);
    gtk_widget_set_sensitive(widget, sensitive);
    
    widget = GTK_WIDGET(g_proxy->details->buttonList[1]);
    gtk_widget_set_sensitive(widget, sensitive);

    // profilename entry
    widget = g_profilename->details->entry;
    gtk_widget_set_sensitive(widget, sensitive);

    // buttons
    widget = GTK_WIDGET(g_buttons->details->buttonList[0]);
    gtk_widget_set_sensitive(widget, sensitive);
    
    widget = GTK_WIDGET(g_buttons->details->buttonList[1]);
    gtk_widget_set_sensitive(widget, sensitive);

    widget = GTK_WIDGET(g_buttons->details->buttonList[2]);
    gtk_widget_set_sensitive(widget, sensitive);
  
    if (sensitive)
    {
        // restore the previous in-sensitive widgets
        // static button
        networkType = edit_data_get_network_type();
        if ((networkType == wireless_t)
            || (networkType == dialup_t))
        {
            widget = GTK_WIDGET(g_address->details->buttonList[1]);
            gtk_widget_set_sensitive(widget, FALSE);
        }
        
        // save button
        mode = edit_get_mode();
        if ((mode == editModeConnectFillIn_e)
            || (mode == editModeConnectCreate_e)
            || (mode == editModeConnectNormal_e))
        {
            edit_overview_enable_save_button(FALSE);
        }
    }
}

