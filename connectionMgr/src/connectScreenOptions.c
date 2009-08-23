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
 * \file connectScreenOptions.c
 * \brief connectionMgr - the first page of 'connect-to-server' screen
 *                        the connection options configuration screen
 *                        containing 
 *                        a selection widget lets users choose 
 *                            which server(iDS/PC) connect to
 *                        a selection widget lets users choose 
 *                            the network type (wired/wireless/dialup)
 *                        a information widget gives some feedback to users 
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <unistd.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "commonData.h"
#include "connectScreenData.h"
#include "connectScan.h"
#include "connectPing.h"
#include "gtkSettingItem.h"
#include "widgetUtils.h"
#include "connectScreen.h"
#include "connectScreenOptions.h"
#include "connectScreenProfiles.h"
#include "background.h"
#include "connectionMgr.h"
#include "languages.h"
#include "erbusy.h"
#include "pagebar.h"

// vbox in which all connect-screen-options items are packed
static GtkWidget *g_connect_options = NULL;

// connect with
static GtkWidget *g_connect_with = NULL;

// selection: iDS or PC
static bkWdtLabelButton *g_server_type = NULL;

// selection: wired or wireless or dialup
static bkWdtLabelButton *g_network_type = NULL;

// information
static GtkWidget *g_info = NULL;
static GtkWidget *g_info_label = NULL;

static guint      g_scan_timeout_id = 0; 

static guint      g_scan_done_timeout_id = 0;

static gboolean   g_first_display = TRUE; 

// internally functions 
static GtkWidget *create_connect_with (void);
static GtkWidget *create_server_type (void);
static GtkWidget *create_network_type (void);
static GtkWidget *create_infomation (void);

static gboolean on_button_press(GtkWidget* button, 
                                GdkEventButton* event, 
                                gpointer data);
static void on_servertype_update (erGtkSelectionGroup * selection, 
                                  gpointer button, gpointer data);
static void on_networktype_update (erGtkSelectionGroup * selection, 
                                   gpointer button, gpointer data);

static void connect_options_update_info (scanStatus_t status);

////////////////////////////////////////////////////////////////////////////
// create GUI widgets
////////////////////////////////////////////////////////////////////////////

// 
// |--vbox
//    |--itemConnectWith
//    |--itemInfo
//
GtkWidget *connect_options_create (void)
{
    CN_LOGPRINTF ("entry");

    GtkWidget *vbox;
    GtkWidget *itemConnectWith;
    GtkWidget *itemInfo;

    vbox = gtk_vbox_new (FALSE, VBOX_SPACING);
    gtk_widget_show(vbox);
    g_connect_options = vbox;

    itemConnectWith = create_connect_with ();
    gtk_box_pack_start (GTK_BOX (vbox), itemConnectWith, FALSE, FALSE, 0);
    gtk_widget_show(itemConnectWith);

    itemInfo = create_infomation ();
    gtk_box_pack_start (GTK_BOX (vbox), itemInfo, FALSE, FALSE, 0);
    gtk_widget_show(itemInfo);

    return vbox;
}

//
// |--item(gtkSettingsItem)
//    |--vbox
//       |--server(composite widget)
//       |--network(composite widget)
//
static GtkWidget *create_connect_with (void)
{
    GtkWidget *item = NULL;
    GtkWidget *vbox = NULL;
    GtkWidget *server = NULL;
    GtkWidget *network = NULL;

    // item
    item = gtk_settingitem_new ("");
    gtk_widget_set_size_request (item, -1, -1);
    g_connect_with = item;

    // vbox
    vbox = gtk_vbox_new (FALSE, VBOX_SPACING);
    gtk_settingitem_add_details (GTK_SETTINGITEM (item), vbox);
    gtk_widget_show (vbox);

    // server
    server = create_server_type ();
    gtk_box_pack_start (GTK_BOX (vbox), server, FALSE, FALSE, 0);
    gtk_widget_show (server);

    // network
    network = create_network_type ();
    gtk_box_pack_start (GTK_BOX (vbox), network, FALSE, FALSE, 0);
    gtk_widget_show (network);

    return item;
}

//
// |--item (bkWdtLabelButton)  
//
static GtkWidget *create_server_type (void)
{
    bkWdtLabelButton *item;
    GtkWidget *widget;
   
    item = bk_wdt_label_button_new(2, FALSE, 1, 1);

    // signal handlers
    widget = GTK_WIDGET(item->details->buttonList[0]);
    g_signal_connect(G_OBJECT(widget), "button-press-event", 
                     G_CALLBACK(on_button_press), NULL);
   
    widget = GTK_WIDGET(item->details->buttonList[1]);
    g_signal_connect(G_OBJECT(widget), "button-press-event", 
                     G_CALLBACK(on_button_press), NULL);
    
    widget = item->details->selection;
    g_signal_connect (G_OBJECT (widget), "selection-update", 
                      G_CALLBACK (on_servertype_update), NULL);

    // global variables
    g_server_type = item;

    // return
    return item->background;
}

//
// |--item (bkWdtLabelButton)
//
static GtkWidget *create_network_type (void)
{
    bkWdtLabelButton *item;
    GtkWidget *widget;

    item = bk_wdt_label_button_new(3, FALSE, 1, 1);

    // signal handlers
    widget = GTK_WIDGET(item->details->buttonList[0]);
    g_signal_connect(G_OBJECT(widget), "button-press-event", 
                     G_CALLBACK(on_button_press), NULL);
  
    widget = GTK_WIDGET(item->details->buttonList[1]);
    g_signal_connect(G_OBJECT(widget), "button-press-event", 
                     G_CALLBACK(on_button_press), NULL);
      
    widget = GTK_WIDGET(item->details->buttonList[2]);
    g_signal_connect(G_OBJECT(widget), "button-press-event", 
                     G_CALLBACK(on_button_press), NULL);
  
    widget = item->details->selection;
    g_signal_connect (G_OBJECT (widget), "selection-update",
            G_CALLBACK (on_networktype_update), NULL);
    
    // global variables
    g_network_type = item;

    return item->background;
}

//
// |--item(gtkSettingsItem)
//    |--background(event box)
//       |--alignment
//          |--vbox
//             |--label
//
static GtkWidget *create_infomation (void)
{
    GtkWidget *item = NULL;
    GtkWidget *background = NULL;
    GtkWidget *alignment = NULL;
    GtkWidget *vbox = NULL;
    GtkWidget *label = NULL;

    // item
    item = gtk_settingitem_new ("");
    gtk_widget_set_size_request (item, -1, -1);
    gtk_widget_hide (item);
    g_info = item;
    
    // background
    background = gtk_event_box_new ();
    gtk_widget_set_name (background, "bk_grey_666_75");
    gtk_widget_set_size_request(background, 
            SITEM_BK_WIDTH, SITEM_BK_HEIGHT);
    gtk_settingitem_add_details (GTK_SETTINGITEM (item), background);
   
    // alignment
    alignment = gtk_alignment_new (0.0, 0.5, 0.0, 0.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment),
            PADDING_TOP, PADDING_BOTTOM, PADDING_LEFT, PADDING_RIGHT);
    gtk_container_add (GTK_CONTAINER (background), alignment);
    
    // vbox
    vbox = gtk_vbox_new (FALSE, VBOX_SPACING);
    gtk_container_add (GTK_CONTAINER (alignment), vbox);
   
    // label
    label = gtk_label_new ("");
    gtk_widget_set_name (label, "info_label");
    gtk_widget_set_size_request(GTK_WIDGET(label), 
            SITEM_BK_WIDTH - PADDING_LEFT - PADDING_RIGHT, -1);
    gtk_label_set_single_line_mode(GTK_LABEL(label), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
    g_info_label = label;

    gtk_widget_show_all (background);

    return item;
}

////////////////////////////////////////////////////////////////////////////
// set the correct text on screen
////////////////////////////////////////////////////////////////////////////
void connect_options_set_text (void)
{
    GtkWidget *widget;

    CN_LOGPRINTF ("entry");

    if (g_connect_with)
    {
        // selection iDS / PC
        gtk_settingitem_set_header_text (GTK_SETTINGITEM (g_connect_with), 
                                         _("Connection settings"));
    }

    if (g_server_type)
    {
        widget = g_server_type->details->label;
        gtk_label_set_text (GTK_LABEL (widget),
                            _("Select to connect to the iDS "
                              "(iRex Delivery Service) or to "
                              "your computer."));

        widget = GTK_WIDGET(g_server_type->details->buttonList[0]);
        gtk_button_set_label (GTK_BUTTON (widget), _("iDS"));
       
        widget = GTK_WIDGET(g_server_type->details->buttonList[1]);
        gtk_button_set_label (GTK_BUTTON (widget), _("Computer"));
    }

    if (g_network_type)
    {
        widget = g_network_type->details->label;
        gtk_label_set_text (GTK_LABEL (widget),
                            _("Select a connection type or accept "
                              "the default."));

        widget = GTK_WIDGET(g_network_type->details->buttonList[0]);
        gtk_button_set_label (GTK_BUTTON (widget), _("Wired"));
        
        widget = GTK_WIDGET(g_network_type->details->buttonList[1]);
        gtk_button_set_label (GTK_BUTTON (widget), _("Wireless"));

        widget = GTK_WIDGET(g_network_type->details->buttonList[2]);
        gtk_button_set_label (GTK_BUTTON (widget), _("Dial-up"));
    }
}

////////////////////////////////////////////////////////////////////////////
// according to the settings,
// make sure the related labels show correct text, 
// and select the correct button
////////////////////////////////////////////////////////////////////////////
void connect_options_display (void)
{
    CN_LOGPRINTF ("entry");

    GtkWidget*       widget;
    GtkToggleButton* button;
  
    if (g_server_type)
    {
        // Display choice iDS / Computer
        //   - select which button must be selected
        switch (connect_data_get_connect_type())
        {
            case cmgrConnectIDS_e:
                widget = GTK_WIDGET(g_server_type->details->buttonList[0]);
                break;
            case cmgrConnectPCShare_e:
                widget = GTK_WIDGET(g_server_type->details->buttonList[1]);
                break;
            default:
                // nothing selected (yet)
                widget = GTK_WIDGET(g_server_type->details->buttonList[0]);
                connect_data_set_connect_type(cmgrConnectIDS_e);
                break;
        }
        //   - and select this button
        //     Note: when the button is already selected, 
        //     we de-select then select again;
        //           this ensures the signal handlers are called 
        //           regardless the old button state.
        button = GTK_TOGGLE_BUTTON(widget);
        if (g_first_display && gtk_toggle_button_get_active(button))
        {
            // button is selected, so first deselect it 
            // to make sure signal handler is called
            gtk_toggle_button_set_active(button, FALSE);
        }
        gtk_toggle_button_set_active(button, TRUE);
    }

    if (g_network_type)
    {
        // Display choice wired / wireless / dialup
        switch (connect_data_get_network_type())
        {
            case wired_t:
                widget = GTK_WIDGET(g_network_type->details->buttonList[0]);
                break;
            case wireless_t:
                widget = GTK_WIDGET(g_network_type->details->buttonList[1]);
                break;
            case dialup_t:
                widget = GTK_WIDGET(g_network_type->details->buttonList[2]);
                break;
            default:
                // nothing selected (yet), using the default setting
                // default is wireless, unless the device does not have wireless, then the default is wired
                widget = GTK_WIDGET(g_network_type->details->buttonList[main_get_wifi_disabled() ? 0 : 1]);
                connect_data_set_network_type (main_get_wifi_disabled() ? wired_t : wireless_t);
                break;
        }
        //   - and select this button
        //     Note: when the button is already selected, 
        //     we de-select then select again;
        //           this ensures the signal handlers are called 
        //           regardless the old button state.
        button = GTK_TOGGLE_BUTTON(widget);
        if (g_first_display && gtk_toggle_button_get_active(button))
        {
            // button is selected, so first deselect it 
            // to make sure signal handler is called
            gtk_toggle_button_set_active(button, FALSE);
        }
        gtk_toggle_button_set_active(button, TRUE);
    }

    // force connect to iDS when no PC profile in registry
    const regPCProfile_t *pcshare = NULL;
    pcshare = connect_data_get_pcsharedata();
    if (pcshare == NULL
      || (pcshare->pcname == NULL || pcshare->pcname[0] == '\0')
      || (pcshare->sharename == NULL || pcshare->sharename[0] == '\0'))
    {
        CN_WARNPRINTF ("no PC profile in registry: force connect to iDS");
       
        if (g_server_type)
        {
            widget = GTK_WIDGET(g_server_type->details->buttonList[1]);
            gtk_widget_set_sensitive(widget, FALSE);
            
            widget = GTK_WIDGET(g_server_type->details->buttonList[0]);
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
        }

        connect_data_set_connect_type (cmgrConnectIDS_e);
    }
    
    if (main_get_wifi_disabled())
    {
        // set wifi button insensitive when wifi is disabled
        widget = GTK_WIDGET(g_network_type->details->buttonList[1]);
        gtk_widget_set_sensitive(widget, FALSE);
    }

    if (main_get_dialup_disabled())
    {
        // set dialup button insensitive when dialup is disabled
        widget = GTK_WIDGET(g_network_type->details->buttonList[2]);
        gtk_widget_set_sensitive(widget, FALSE);
    }

    connect_options_update_info(scanRunning_e);

    g_first_display = FALSE;
}

static void connect_options_update_info (scanStatus_t status)
{
    CN_LOGPRINTF("entry");

    connection_t networkType;
    char* cpTitle = NULL, *cpDetails = NULL;
    
    networkType = connect_data_get_network_type();
    switch(networkType)
    {
        case wired_t:
            cpTitle = _("Available wired networks");
            if (status == rescanNeed_e)
            {
                cpDetails = _("No wired networks found.\n"
                              "Click the Wired button above "
                              "to scan again.");
            }
            else if (status == scanRunning_e)
            {
                cpDetails = _("Please wait while the iLiad scans for "
                              "wired networks ...");
            }
            else
            {
                cpDetails = "";
            }
            break;
        case dialup_t:
            cpTitle = _("Available dial-up networks");
            if (status == rescanNeed_e)
            {
                cpDetails = _("No dial-up networks found.\n"
                              "Click the Dial-up button above "
                              "to scan again.");
            }
            else if (status == scanRunning_e)
            {
                cpDetails = _("Please wait while the iLiad scans for "
                              "dial-up networks ...");
            }
            else
            {
                cpDetails = "";
            }
            break;
        case wireless_t:
            cpTitle = _("Available wireless networks");
            if (status == rescanNeed_e)
            {
                cpDetails = _("No wireless networks found.\n"
                              "Click the Wireless button above "
                              "to scan again.");
            }
            else if (status == scanRunning_e)
            {
                cpDetails = _("Please wait while the iLiad is scanning "
                              "for wireless networks ...");
            }
            else
            {
                cpDetails = "";
            }
            break;
        default:
            CN_WARNPRINTF("Invalid network type[%d].", networkType);
            break;
    }

    // show the information wiget with the correct text 
    if (g_info)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_info), cpTitle);
        gtk_label_set_text(GTK_LABEL(g_info_label), cpDetails);
    }
}

////////////////////////////////////////////////////////////////////////////
// signal handlers after user changes his selection
////////////////////////////////////////////////////////////////////////////
static void remove_timeout_ids(void)
{
    if (g_scan_timeout_id > 0)
    {
        CN_WARNPRINTF("remove the old timeout function for scanning");
        g_source_remove(g_scan_timeout_id);
        g_scan_timeout_id = 0;
    }

    if (g_scan_done_timeout_id)
    {
        CN_WARNPRINTF("remove the old timeout function for scanning done");
        g_source_remove(g_scan_done_timeout_id);
        g_scan_done_timeout_id = 0;
    }
}

static gboolean on_button_press(GtkWidget* button, 
                                GdkEventButton* event, 
                                gpointer data)
{
    CN_LOGPRINTF("entry");
    
    if (connect_scan_freeze_ui(cmgr_get_scan_ctxt()))
    {
        CN_WARNPRINTF("Please wait when freezing UI ...");
        return TRUE;
    }

    return FALSE;
}

static gboolean delay_servertype_udpate(gpointer data)
{
    int                  selectedId;
    erGtkSelectionGroup *selection;
    scanContext_t       *ctxt;

    if (connect_scan_freeze_ui(cmgr_get_scan_ctxt()))
    {
        CN_WARNPRINTF("call me later ...");

        connect_options_update_info(scanAborting_e);
        display_update_request_screen_refresh(SETTING_ITEM_CHANGE, 
                                              WAVEFORM_TYPING);
        return TRUE;  // TRUE = please call again
    }

    selection = ERGTK_SELECTION_GROUP(data);
    ergtk_selection_group_get_selected_buttons(selection, &selectedId, 1);
    if (selectedId == 0)
    {
        // ids connection selected, set mode
        connect_data_set_connect_type (cmgrConnectIDS_e);
    }
    else if (selectedId == 1)
    {
        // computer connection selected, set mode
        connect_data_set_connect_type (cmgrConnectPCShare_e);
    }
 
    // update the GUI, give some feedback to users
    connect_options_update_info(scanRunning_e);
    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, 
                                          WAVEFORM_TYPING);
  
    connect_options_restore_scan_ctxt();
    ctxt = cmgr_get_scan_ctxt();
    connect_scan(ctxt, FALSE);
    
    g_scan_timeout_id = 0;

    return FALSE;  // FALSE = don't call me again
}

static void on_servertype_update (erGtkSelectionGroup * selection, 
                                  gpointer button, gpointer data)
{
    CN_LOGPRINTF ("entry selection[%p], button[%p], data[%p]", 
                   selection, button, data);
 
    GtkToggleButton* btn = GTK_TOGGLE_BUTTON(button);
    gboolean active = gtk_toggle_button_get_active(btn);

    if (active)
    {
        remove_timeout_ids();
        
        g_timeout_add(1000, delay_erbusy_blink, NULL);
        connect_scan_abort(cmgr_get_scan_ctxt());
        
        // wait for scan aborting done, then further actions
        g_scan_timeout_id = g_timeout_add(200,
                                delay_servertype_udpate, 
                                (gpointer)selection);
    }
}

static gboolean delay_networktype_udpate(gpointer data)
{
    int                  selectedId;
    erGtkSelectionGroup *selection;
    scanContext_t       *ctxt;

    if (connect_scan_freeze_ui(cmgr_get_scan_ctxt()))
    {
        CN_WARNPRINTF("call me later ...");
        connect_options_update_info(scanAborting_e);
        display_update_request_screen_refresh(SETTING_ITEM_CHANGE, 
                                              WAVEFORM_TYPING);
        return TRUE;  // TRUE = please call again
    }

    selection = ERGTK_SELECTION_GROUP(data);
    ergtk_selection_group_get_selected_buttons(selection, &selectedId, 1);
    if (selectedId == 0)
    {
        // wired selected
        connect_data_set_network_type(wired_t);
    }
    else if (selectedId == 1)
    {
        // wireless selected
        connect_data_set_network_type (wireless_t);
    }
    else if (selectedId == 2)
    {
        // dialup selected
        connect_data_set_network_type(dialup_t);
    }

    // update the GUI, give some feedback to users
    connect_options_update_info(scanRunning_e);
    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, 
                                          WAVEFORM_TYPING);

    connect_options_restore_scan_ctxt();
    ctxt = cmgr_get_scan_ctxt();
    connect_scan(ctxt, FALSE);

    g_scan_timeout_id = 0;

    return FALSE;  // FALSE = don't call me again
}

static void on_networktype_update (erGtkSelectionGroup * selection, 
                                   gpointer button, gpointer data)
{
    CN_LOGPRINTF ("entry selection[%p], button[%p], data[%p]", 
                   selection, button, data);

    GtkToggleButton* btn = GTK_TOGGLE_BUTTON(button);
    gboolean active = gtk_toggle_button_get_active(btn);

    if (active)
    {
        remove_timeout_ids();

        g_timeout_add(1000, delay_erbusy_blink, NULL);
        connect_scan_abort(cmgr_get_scan_ctxt());

        // wait for scan aborting done, then further actions
        g_scan_timeout_id = g_timeout_add(200, 
                                       delay_networktype_udpate, 
                                       (gpointer)selection);
    }
}

static gboolean connect_ping_during_idle(gpointer data)
{
    CN_LOGPRINTF("entry");

    pingContext_t *ctxt;
    gboolean delay = (gboolean)data;
    
    connect_profiles_restore_ping_ctxt();
    
    ctxt = cmgr_get_ping_ctxt();
    connect_ping_select_initial_profile(ctxt);
    connect_ping(ctxt, delay);
    
    return FALSE;
}

// using timeout to give a chance to cancel the scanning results
static gboolean on_delayed_scan_networks_done(gpointer data)
{
    int          nProfiles;
    connection_t networkType;
    gboolean     nextPage = FALSE;

    nProfiles = connect_data_get_n_network_profiles();
    networkType = connect_data_get_network_type();

    switch (networkType)
    {
        case wireless_t:
            if (nProfiles > 0)
            {
                nextPage = TRUE;
            }
            break;
        case dialup_t:
            nextPage = TRUE;
            break;
        case wired_t:
            nextPage = TRUE;
            break;
        default:
            break;
    }    

    if (nextPage)
    {
        display_update_increase_level(MAIN_WINDOW_EXPOSE_LEVEL);
   
        // go to the profiles page
        connect_profiles_display();
        connect_goto_page(connectScreenProfiles_e);

        if (nProfiles > 0)
        {
            // call connect_ping during idle 
            // in order to show the profiles page firstly, 
            // then start to ping
            gtk_idle_add(connect_ping_during_idle, (gpointer)TRUE);
        }
    }
    else
    {
        connect_options_update_info(rescanNeed_e);
        display_update_request_screen_refresh(SETTING_ITEM_CHANGE, 
                                              WAVEFORM_TYPING);
    }    

    g_scan_done_timeout_id = 0;
    return FALSE;
}

void connect_options_scan_networks_done(void)
{
    CN_LOGPRINTF("entry");

    g_scan_done_timeout_id = g_timeout_add(5000, 
                            on_delayed_scan_networks_done, 
                            NULL);
}

void connect_options_restore_scan_ctxt(void)
{
    scanContext_t *ctxt;
    connection_t networkType;
    char *ssidList;

    CN_LOGPRINTF("entry");
    
    ctxt = cmgr_get_scan_ctxt();
    if (ctxt)
    {
        connect_scan_ctxt_set_mode(ctxt, connectScan_e);
        
        networkType = connect_data_get_network_type();
        connect_scan_ctxt_set_network_type(ctxt, networkType);
        
        connect_scan_ctxt_set_done_callbacks(ctxt,
                    connect_data_init_network_profiles,
                    connect_options_scan_networks_done);

        ssidList = connect_data_get_hidden_ssid_list();
        connect_scan_ctxt_set_ssidlist(ctxt, ssidList);
        g_free(ssidList);
    }
}

gboolean on_connect_options_keypress(GdkEventKey *event)
{
    scanContext_t *ctxt;
    gboolean ret = FALSE;

    CN_LOGPRINTF("entry");

    switch (event->keyval)
    {
        case GDK_Return:
            // 'Enter' key is pressed
            ret = TRUE;
            break;

        case GDK_F5:
            // 'UP' key is pressed
            // goto the one top level page
            remove_timeout_ids();

            ctxt = cmgr_get_scan_ctxt();
            connect_scan_abort(ctxt);
            
            cmgr_up_page();
            
            ret = TRUE;
            break;

        default:
            erbusy_off();
    }
    
    return ret;
}

