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
 * \file iLiadAutoconnectScreen.c
 * \brief setup application - E-reader config screen                

 * <File description>
 * 
 */

#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include <libergtk/ergtk.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "background.h"
#include "gtkInfoItem.h"
#include "gtkSettingItem.h"
#include "gtkSettingItemTextEntry.h"
#include "iLiadBgConnectScreen.h"
#include "iLiadBgConnectData.h"
#include "languages.h"


static GtkWidget* g_vbox = NULL;

static GtkWidget* g_wlan_onoff = NULL;
static GtkWidget* g_wlan_onoff_selection = NULL;
static GtkWidget* g_wlan_onoff_label = NULL;
static GtkWidget* g_wlan_button_on  = NULL;
static GtkWidget* g_wlan_button_off = NULL;
static guint      g_wlan_on  = -1;
static guint      g_wlan_off = -1;

static GtkWidget* g_background_idspc = NULL;
static GtkWidget* g_background_idspc_selection = NULL;
static GtkWidget* g_background_idspc_label = NULL;
static GtkWidget* g_background_button_ids = NULL;
static GtkWidget* g_background_button_pc  = NULL;
static guint      g_background_ids = -1;
static guint      g_background_pc  = -1;

static GtkWidget* g_background_onoff = NULL;
static GtkWidget* g_background_onoff_selection = NULL;
static GtkWidget* g_background_onoff_label = NULL;
static GtkWidget* g_background_button_on  = NULL;
static GtkWidget* g_background_button_off = NULL;
static guint      g_background_on  = -1;
static guint      g_background_off = -1;

static GtkWidget* g_background_interval = NULL;

static GtkWidget* g_background_info = NULL;

// internally used routines
static GtkWidget* create_wlan_onoff(void);
static GtkWidget* create_background_idspc(void);
static GtkWidget* create_background_onoff(void);
static GtkWidget* create_background_interval(void);
static GtkWidget* create_background_info(void);
//static gint       strtime2int(const gchar* text);
gboolean   iLiad_background_get_on(void);

// callback functions
static void on_wlan_onoff_selection_update(erGtkSelectionGroup* selection, 
                                           gpointer button, 
                                           gpointer data);
static void on_background_idspc_selection_update(erGtkSelectionGroup* selection, 
                                                 gpointer button, 
                                                 gpointer data);
static void on_background_onoff_selection_update(erGtkSelectionGroup* selection, 
                                                 gpointer button, 
                                                 gpointer data);

static void on_interval_validate(GtkWidget* item, const gchar* text, gchar** new_text);


// object hierarchy:
//     g_vbox
//       |-- g_wlan_onoff
//       |-- g_background_idspc
//       |-- vbox
//       |     |-- g_background_onoff
//       |     |-- g_background_interval
//       |-- g_background_info
//
GtkWidget* iLiad_autoconnect_create(void)
{
    GtkWidget* vbox;

    ST_LOGPRINTF("entry");

    g_vbox = gtk_vbox_new(FALSE, BACKGROUNDSCREEN_ITEM_SPACING);

    g_wlan_onoff = create_wlan_onoff();
    gtk_box_pack_start(GTK_BOX(g_vbox), GTK_WIDGET(g_wlan_onoff), FALSE, FALSE, 0);

    g_background_idspc = create_background_idspc();
    gtk_box_pack_start(GTK_BOX(g_vbox), GTK_WIDGET(g_background_idspc), FALSE, FALSE, 0);

    vbox = gtk_vbox_new(FALSE, SETTINGITEM_ITEM_SPACING);
    gtk_box_pack_start(GTK_BOX(g_vbox), GTK_WIDGET(vbox), FALSE, FALSE, 0);
    gtk_widget_show(vbox);

    g_background_onoff = create_background_onoff();
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(g_background_onoff), FALSE, FALSE, 0);

    g_background_interval = create_background_interval();
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(g_background_interval), FALSE, FALSE, 0);

    g_background_info = create_background_info();
    gtk_box_pack_start(GTK_BOX(g_vbox), GTK_WIDGET(g_background_info), FALSE, FALSE, 0);

    gtk_widget_show(g_vbox);

    return g_vbox;
}

// object hierarchy:
//     g_wlan_onoff (GtkSettingItem)
//       |-- g_wlan_onoff_selection (erGtkSelectionGroup)
//             |-- alignment
//                   |--vbox
//                        |-- g_wlan_onoff_label (GtkLabel)
//                        |-- hbox
//                              |-- g_wlan_button_on
//                              |-- g_wlan_button_off
//
static GtkWidget* create_wlan_onoff(void)
{
    GtkWidget*       widget;
    GtkWidget*       alignment;
    GtkWidget*       vbox;
    GtkWidget*       hbox;
    GtkToggleButton* button_tbl[3];

    // g_wlan_onoff
    widget = gtk_settingitem_new("");
    gtk_widget_set_size_request(widget, (-1), (-1));
    gtk_widget_show(widget);
    g_wlan_onoff = widget;

    // g_wlan_onoff_selection --> see below

    // alignment
    widget = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(widget),
                              BACKGROUND_ITEM_PADDING_TOP,
                              BACKGROUND_ITEM_PADDING_BOTTOM,
                              BACKGROUND_ITEM_PADDING_LEFT,
                              BACKGROUND_ITEM_PADDING_RIGHT);
    gtk_widget_show(widget);
    alignment = widget;

    // vbox
    widget = gtk_vbox_new(FALSE, 2 * BACKGROUNDSCREEN_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), widget);
    gtk_widget_show(widget);
    vbox = widget;
    
    // g_wlan_onoff_label
    widget = gtk_label_new("");
    gtk_widget_set_name(widget, "background_screen_info_text");
    gtk_widget_set_size_request(widget, BACKGROUND_ONOFF_LABEL_WIDTH, (-1));
    gtk_label_set_single_line_mode(GTK_LABEL(widget), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(widget), TRUE);
    gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_wlan_onoff_label = widget;

    // hbox
    widget = gtk_hbox_new(FALSE, SMALL_BUTTON_SPACING);
    gtk_container_add(GTK_CONTAINER(vbox), widget);
    gtk_widget_show(widget);
    hbox = widget;
    //     g_wlan_button_on
    widget = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(widget), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_wlan_button_on = widget;
    //     g_wlan_button_off
    widget = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(widget), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_wlan_button_off = widget;
    // add details to selection
    button_tbl[0] = GTK_TOGGLE_BUTTON(g_wlan_button_on);
    button_tbl[1] = GTK_TOGGLE_BUTTON(g_wlan_button_off );
    button_tbl[2] = NULL;
    g_wlan_on = 0;  // selection-group counts from 0
    g_wlan_off  = 1;
    // g_wlan_onoff_selection
    widget = ergtk_selection_group_new(button_tbl);
    gtk_widget_set_name(widget, "background_idspc_background");
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(widget), 1, 1);
    gtk_widget_set_size_request(GTK_WIDGET(widget), -1, BACKGROUND_IDSPC_REQ_HEIGHT);
    g_signal_connect(G_OBJECT(widget), "selection-update", 
                     G_CALLBACK(on_wlan_onoff_selection_update), NULL);
    gtk_container_add(GTK_CONTAINER(widget), alignment);
    gtk_settingitem_add_details(GTK_SETTINGITEM(g_wlan_onoff), widget);
    gtk_widget_show(widget);
    g_wlan_onoff_selection = widget;

    return g_wlan_onoff;
}


// object hierarchy:
//     g_background_idspc (GtkSettingItem)
//       |-- g_background_idspc_selection (erGtkSelectionGroup)
//             |-- alignment
//                   |--vbox
//                        |-- g_background_idspc_label (GtkLabel)
//                        |-- hbox
//                              |-- g_background_button_ids
//                              |-- g_background_button_pc
//
static GtkWidget* create_background_idspc(void)
{
    GtkWidget*       widget;
    GtkWidget*       alignment;
    GtkWidget*       vbox;
    GtkWidget*       hbox;
    GtkToggleButton* button_tbl[3];

    // g_background_idspc
    widget = gtk_settingitem_new("");
    gtk_widget_set_size_request(widget, (-1), (-1));
    gtk_widget_show(widget);
    g_background_idspc = widget;

    // g_background_idspc_selection --> see below

    // alignment
    widget = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(widget),
                              BACKGROUND_ITEM_PADDING_TOP,
                              BACKGROUND_ITEM_PADDING_BOTTOM,
                              BACKGROUND_ITEM_PADDING_LEFT,
                              BACKGROUND_ITEM_PADDING_RIGHT);
    gtk_widget_show(widget);
    alignment = widget;

    // vbox
    widget = gtk_vbox_new(FALSE, 2 * BACKGROUNDSCREEN_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), widget);
    gtk_widget_show(widget);
    vbox = widget;
    
    // g_background_idspc_label
    widget = gtk_label_new("");
    gtk_widget_set_name(widget, "background_screen_info_text");
    gtk_widget_set_size_request(widget, BACKGROUND_ONOFF_LABEL_WIDTH, (-1));
    gtk_label_set_single_line_mode(GTK_LABEL(widget), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(widget), TRUE);
    gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_background_idspc_label = widget;

    // hbox
    widget = gtk_hbox_new(FALSE, SMALL_BUTTON_SPACING);
    gtk_container_add(GTK_CONTAINER(vbox), widget);
    gtk_widget_show(widget);
    hbox = widget;
    //     g_background_button_ids
    widget = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(widget), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_background_button_ids = widget;
    //     g_background_button_pc
    widget = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(widget), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_background_button_pc = widget;
    // add details to selection
    button_tbl[0] = GTK_TOGGLE_BUTTON(g_background_button_ids);
    button_tbl[1] = GTK_TOGGLE_BUTTON(g_background_button_pc );
    button_tbl[2] = NULL;
    g_background_ids = 0;  // selection-group counts from 0
    g_background_pc  = 1;
    // g_background_idspc_selection
    widget = ergtk_selection_group_new(button_tbl);
    gtk_widget_set_name(widget, "background_idspc_background");
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(widget), 1, 1);
    gtk_widget_set_size_request(GTK_WIDGET(widget), -1, BACKGROUND_IDSPC_REQ_HEIGHT);
    g_signal_connect(G_OBJECT(widget), "selection-update", G_CALLBACK(on_background_idspc_selection_update), NULL);
    gtk_container_add(GTK_CONTAINER(widget), alignment);
    gtk_settingitem_add_details(GTK_SETTINGITEM(g_background_idspc), widget);
    gtk_widget_show(widget);
    g_background_idspc_selection = widget;

    return g_background_idspc;
}



// object hierarchy:
//     g_background_onoff (GtkSettingItem)
//       |-- g_background_onoff_selection
//             |-- alignment
//                   |--vbox
//                        |-- g_background_onoff_label (GtkLabel)
//                        |-- hbox
//                              |-- g_background_button_on
//                              |-- g_background_button_off
//
static GtkWidget* create_background_onoff(void)
{
    GtkWidget*       widget;
    GtkWidget*       alignment;
    GtkWidget*       vbox;
    GtkWidget*       hbox;
    GtkToggleButton* button_tbl[3];

    // g_background_onoff
    widget = gtk_settingitem_new("");
    gtk_widget_set_size_request(widget, (-1), (-1));
    gtk_widget_show(widget);
    g_background_onoff = widget;

    // g_background_onoff_selection --> see below

    // alignment
    widget = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(widget),
                              BACKGROUND_ITEM_PADDING_TOP,
                              BACKGROUND_ITEM_PADDING_BOTTOM,
                              BACKGROUND_ITEM_PADDING_LEFT,
                              BACKGROUND_ITEM_PADDING_RIGHT);
    gtk_widget_show(widget);
    alignment = widget;

    // vbox
    widget = gtk_vbox_new(FALSE, 2 * BACKGROUNDSCREEN_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), widget);
    gtk_widget_show(widget);
    vbox = widget;
    
    // g_background_onoff_label
    widget = gtk_label_new("");
    gtk_widget_set_name(widget, "background_screen_info_text");
    gtk_widget_set_size_request(widget, BACKGROUND_ONOFF_LABEL_WIDTH, (-1));
    gtk_label_set_single_line_mode(GTK_LABEL(widget), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(widget), TRUE);
    gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_background_onoff_label = widget;

    // hbox
    widget = gtk_hbox_new(FALSE, SMALL_BUTTON_SPACING);
    gtk_container_add(GTK_CONTAINER(vbox), widget);
    gtk_widget_show(widget);
    hbox = widget;
    //     g_background_button_on
    widget = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(widget), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_background_button_on = widget;
    //     g_background_button_off
    widget = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(widget), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_background_button_off = widget;
    // add details to selection
    button_tbl[0] = GTK_TOGGLE_BUTTON(g_background_button_on );
    button_tbl[1] = GTK_TOGGLE_BUTTON(g_background_button_off);
    button_tbl[2] = NULL;
    g_background_on  = 0;  // selection-group counts from 0
    g_background_off = 1;
    // g_background_onoff_selection
    widget = ergtk_selection_group_new(button_tbl);
    gtk_widget_set_name(widget, "background_onoff_background");
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(widget), 1, 1);
    gtk_widget_set_size_request(GTK_WIDGET(widget), -1, BACKGROUND_ONOFF_REQ_HEIGHT);
    g_signal_connect(G_OBJECT(widget), "selection-update", G_CALLBACK(on_background_onoff_selection_update), NULL);
    gtk_container_add(GTK_CONTAINER(widget), alignment);
    gtk_settingitem_add_details(GTK_SETTINGITEM(g_background_onoff), widget);
    gtk_widget_show(widget);
    g_background_onoff_selection = widget;

    return g_background_onoff;
}

// object hierarchy:
//     g_background_interval (GtkSettingItemTextEntry)
//
static GtkWidget* create_background_interval(void)
{
    ST_LOGPRINTF("entry");

    GtkWidget *widget;

    // g_background_interval_entry
    widget = gtk_settingitem_textentry_new();
    gtk_settingitem_textentry_setentrywidth(GTK_SETTINGITEM_TEXTENTRY(widget), BACKGROUND_INTERVAL_ENTRY_WIDTH);
    gtk_widget_show(widget);
    g_background_interval = widget;
    //     signal callback
    gtk_settingitem_textentry_callback_on_validate(GTK_SETTINGITEM_TEXTENTRY(widget), on_interval_validate);

    return g_background_interval;
}


// object hierarchy:
//     g_background_info (GtkInfoItem)
//
static GtkWidget* create_background_info(void)
{
    ST_LOGPRINTF("entry");

    // g_background_info
    g_background_info = gtk_infoitem_new(FALSE);

    // display
    gtk_widget_show(g_background_info);

    return g_background_info;
}


void iLiad_autoconnect_set_text(void)
{
    gchar *cp;
    
    ST_LOGPRINTF("entry");

    // wlan onoff  
    if (g_wlan_onoff)
    {
        gtk_settingitem_set_header_text( GTK_SETTINGITEM(g_wlan_onoff),
                                         _("Wireless connection preferences")   );
        gtk_label_set_text (GTK_LABEL(g_wlan_onoff_label),
                            _("Select whether to automatically connect to "
                              "a non-secured (open) wireless network.") );
        gtk_button_set_label(GTK_BUTTON(g_wlan_button_on ), _("On") );
        gtk_button_set_label(GTK_BUTTON(g_wlan_button_off), _("Off"));
    }

    // background-connect iDS/pc selection
    if (g_background_idspc)
    {
        gtk_settingitem_set_header_text( GTK_SETTINGITEM(g_background_idspc),
                                         _("Background connect")   );
        gtk_label_set_text (GTK_LABEL(g_background_idspc_label),
                            _("Select whether to download content from the iDS or a computer.") );
        gtk_button_set_label(GTK_BUTTON(g_background_button_ids), _("iDS")     );
        gtk_button_set_label(GTK_BUTTON(g_background_button_pc ), _("Computer"));
    }

    // background-connect enable
    if (g_background_onoff)
    {
        gtk_settingitem_set_header_text( GTK_SETTINGITEM(g_background_onoff),
                                         _("Automatic background connect")   );
        gtk_label_set_text (GTK_LABEL(g_background_onoff_label),
                            _("Enabling \"Automatic background connect\" "
                              "causes the iLiad to receive or send content items "
                              "at a fixed interval.") );
        gtk_button_set_label(GTK_BUTTON(g_background_button_on ), _("On") );
        gtk_button_set_label(GTK_BUTTON(g_background_button_off), _("Off"));
    }

    // background-connect interval
    if (g_background_interval)
    {
        gtk_settingitem_textentry_setlabel(GTK_SETTINGITEM_TEXTENTRY(g_background_interval),
                                           _("Enter the time interval for the background connect.") );
        cp = g_strdup_printf( _("minutes (min. %d, max. %d)"), BACKGROUND_MIN_INTERVAL, BACKGROUND_MAX_INTERVAL);
        gtk_settingitem_textentry_setinfo(GTK_SETTINGITEM_TEXTENTRY(g_background_interval), cp);
        g_free(cp);
    }
    
    // background-connect help text
    if (g_background_info)
    {
        gtk_infoitem_set_text( GTK_INFOITEM(g_background_info),
                               _("During the background connection you can operate the iLiad as usual.\n"
                                 "\n"
                                 "Warning!\n"
                                 "Background connect will empty your battery faster.") );
    }
}

void iLiad_ac_set_wlan_onoff(const gboolean enable)
{
    ST_LOGPRINTF("entry: enable [%d]", enable);

    if (enable)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_wlan_button_on), TRUE);
    }
    else
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_wlan_button_off), TRUE);
    }
}

static gboolean iLiad_background_get_wlan_onoff(void)
{
    erGtkSelectionGroup *selection;
    gint selection_id;
    gboolean returnValue = TRUE;  // none selected

    selection = ERGTK_SELECTION_GROUP(g_wlan_onoff_selection);
    ergtk_selection_group_get_selected_buttons(selection, &selection_id, 1);

    if (g_wlan_on == selection_id)
    {
        ST_LOGPRINTF("enable connect with open wireless network");
        returnValue = TRUE;
    }
    else if (g_wlan_off == selection_id)
    {
        ST_LOGPRINTF("disable connect with open wireless network");
        returnValue = FALSE;
    }
    else
    {
        ST_ERRORPRINTF("invalid wlan on/off [%d]", selection_id);
        //terminate application if this point is reached
        g_assert_not_reached();
    }

    return returnValue;
}

void iLiad_ac_set_background_idspc(const gint connect_to)
{
    ST_LOGPRINTF("entry: connect_to [%d]", connect_to);

    switch (connect_to)
    {
        case 0:
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_background_button_ids), TRUE);
            break;
        case 1:
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_background_button_pc), TRUE);
            break;
        default:
            ST_ERRORPRINTF("Illegal connect_to [%d]", connect_to);
    }
}

static gint iLiad_background_get_idspc(void)
{
    gint selection_id;
    gint returnValue = -1;  // none selected

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(g_background_idspc_selection), &selection_id, 1);

    if (g_background_ids == selection_id)
    {
        ST_LOGPRINTF("connect_to iDS");
        returnValue = 0;
    }
    else if (g_background_pc == selection_id)
    {
        ST_LOGPRINTF("connect_to PC-share");
        returnValue = 1;
    }
    else
    {
        ST_ERRORPRINTF("invalid background ids/pc [%d]", selection_id);
        //terminate application if this point is reached
        g_assert_not_reached();
    }
    return returnValue;
}

// set/get functions
void iLiad_ac_set_background_onoff(const gboolean enable)
{
    ST_LOGPRINTF("entry: enable [%d]", enable);

    if (enable)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_background_button_on), TRUE);
        gtk_widget_show(g_background_interval);
        gtk_settingitem_textentry_grabfocus(GTK_SETTINGITEM_TEXTENTRY(g_background_interval));
        gtk_widget_hide(g_background_info);
    }
    else
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_background_button_off), TRUE);
        gtk_widget_hide(g_background_interval);
        gtk_widget_show(g_background_info);
    }
}

static gboolean iLiad_background_get_onoff(void)
{
    gint selection_id;
    gboolean returnValue = FALSE;

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(g_background_onoff_selection), &selection_id, 1);

    if (g_background_on == selection_id)
    {
        ST_LOGPRINTF("PIN code on");
        returnValue = TRUE;
    }
    else if (g_background_off == selection_id)
    {
        ST_LOGPRINTF("PIN code off");
        returnValue = FALSE;
    }
    else
    {
        ST_ERRORPRINTF("invalid background on/off %d", selection_id);
        //terminate application if this point is reached
        g_assert_not_reached();
    }
    return returnValue;
}

void iLiad_ac_set_background_interval(const gint interval)
{
    ST_LOGPRINTF("entry: interval [%d]", interval);

    char buf[10];
    snprintf(buf, sizeof(buf), "%d", interval);

    gtk_settingitem_textentry_settext(GTK_SETTINGITEM_TEXTENTRY(g_background_interval), buf);
}

static void on_wlan_onoff_selection_update(erGtkSelectionGroup* selection, 
                                           gpointer button,
                                           gpointer data)
{
    gboolean enable = iLiad_background_get_wlan_onoff();

    ST_LOGPRINTF("enable [%d]", enable);

    // store new value
    iLiad_ac_wlan_onoff_changed(enable);

    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_FULLSCREEN);
}

static void on_background_idspc_selection_update(erGtkSelectionGroup* selection, 
                                                 gpointer button,
                                                 gpointer data)
{
    guint connect_to = iLiad_background_get_idspc();

    ST_LOGPRINTF("connect-to [%d]", connect_to);

    // store new value
    iLiad_ac_background_idspc_changed(connect_to);

    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_FULLSCREEN);
}

// callback functions
static void on_background_onoff_selection_update(erGtkSelectionGroup* selection, 
                                                 gpointer button, 
                                                 gpointer data)
{
    gboolean enable = iLiad_background_get_onoff();

    ST_LOGPRINTF("background %s", enable == 0 ? "OFF" : "ON");

    // show/hide interval setting
    if (enable)
    {
        gtk_widget_show(g_background_interval);
        gtk_settingitem_textentry_grabfocus(GTK_SETTINGITEM_TEXTENTRY(g_background_interval));
        gtk_widget_hide(g_background_info);
    }
    else
    {
        gtk_widget_hide(g_background_interval);
        gtk_widget_show(g_background_info);
    }

    // store new value
    iLiad_ac_background_onoff_changed(enable);

    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_FULLSCREEN);
}

static void on_interval_validate(GtkWidget* item, const gchar* text, gchar** new_text)
{
    ST_LOGPRINTF("entry: item [%p] text [%s]", item, text);

    char  buf[10];
    gint  time_value;

    time_value = atoi(text);

    // validate & fix background interval time
    if (time_value < 0)
    {
        // illegal time
        time_value = BACKGROUND_DEFAULT_INTERVAL;
    }
    else if (time_value < BACKGROUND_MIN_INTERVAL)
    {
        time_value = BACKGROUND_MIN_INTERVAL;
    }
    else if (time_value > BACKGROUND_MAX_INTERVAL)
    {
        time_value = BACKGROUND_MAX_INTERVAL;
    }
    snprintf(buf, sizeof(buf), "%d", time_value);

    // report fixed interval, if needed
    if ( strcmp(buf, text) != 0 )
    {
        *new_text = strdup(buf);
    }

    // store new interval
    iLiad_ac_background_interval_changed(time_value);
}

