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
 * \file iLiadTimediDSScreen.c
 * \brief settings - iLiad timed iDS connection setup screen
 *
 * User interface for user to set timed iDS connection
 *
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <libergtk/ergtk.h>
#include "gtkInfoItem.h"
#include "gtkSettingItem.h"
#include "setupLog.h"
#include "iLiadTimediDSData.h"
#include "iLiadTimediDSScreen.h"
#include "languages.h"

// Global variables
static gtk_timedids_screen_t* g_timedids_screen;
static regTimedIds_t* g_cur_settings = NULL;

// Internal used functions
static GtkWidget* create_enable_timedids(gtk_enable_timedids_t* enable_timedids);
static GtkWidget* create_time_settings(gtk_time_settings_t* settime);
static void update_timeset(void);

// Signal handlers
static void on_selection_update(erGtkSelectionGroup * selection, gpointer data);
static void on_checked_software_update(GtkToggleButton* button, gpointer data);
static void on_checked_switch_off(GtkToggleButton* button, gpointer data);
static void on_hour_entry_insert_text(GtkEditable *editable,
                                      gchar *new_text,
                                      gint new_text_length,
                                      gint *position,
                                      gpointer user_data);
static void on_minute_entry_insert_text(GtkEditable *editable,
                                        gchar *new_text,
                                        gint new_text_length,
                                        gint *position,
                                        gpointer user_data);
static gboolean on_hour_entry_losefocus(GtkWidget *widget, GdkEventFocus *event, gpointer user_data);
static gboolean on_minute_entry_getfocus(GtkWidget *widget, GdkEventFocus *event, gpointer user_data);
static gboolean on_minute_entry_losefocus(GtkWidget *widget, GdkEventFocus *event, gpointer user_data);

// widget hierarchy
// |--vbox
//    |--enable_timedids (GtkSettingItem)
//    |--settime         (GtkSettingItem)
//    |--info
GtkWidget* iLiad_timedids_screen_create(void)
{
    GtkWidget* vbox;
    GtkWidget* enable_timedids;
    GtkWidget* settime;
    GtkWidget* info;

    ST_LOGPRINTF("entry");
    
    vbox = gtk_vbox_new(FALSE, TIMEDIDS_ITEM_SPACING);

    g_timedids_screen = g_new0(gtk_timedids_screen_t, 1);
    
    enable_timedids = create_enable_timedids(&g_timedids_screen->enable_timedids);
    gtk_box_pack_start(GTK_BOX(vbox), enable_timedids, FALSE, FALSE, 0);

    settime = create_time_settings(&g_timedids_screen->settime);
    gtk_box_pack_start(GTK_BOX(vbox), settime, FALSE, FALSE, 0);

    info = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), info, FALSE, FALSE, 0);
    g_timedids_screen->infoitem = info;
    
    // show widgets
    gtk_widget_show(vbox);
    return vbox;
}

// widget hierarchy
// |--settingitem (GtkSettingItem)
//    |--background (GtkEventBox)
//       |--alignment
//          |--vbox
//             |--label
//             |--hbox
//                |--button
//                |--button
GtkWidget* create_enable_timedids(gtk_enable_timedids_t* enable_timedids)
{
    GtkWidget* settingitem;
    GtkWidget* background;
    GtkWidget* alignment;
    GtkWidget* vbox;
    GtkWidget* label;
    GtkWidget* hbox;
    GtkWidget* button;
    GtkToggleButton* button_tbl[TIME_OPTIONS];
    GtkWidget* selection;

    ST_LOGPRINTF("entry");
    
    // settingitem
    settingitem = gtk_settingitem_new("");
    gtk_widget_set_size_request(settingitem, -1, -1);
    enable_timedids->settingitem = settingitem;

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "timedids_enable_background");
    gtk_widget_set_size_request(background, TITLE_MIN_WIDTH, ENABLE_TIMEDIDS_HEIGHT);
    gtk_settingitem_add_details(GTK_SETTINGITEM(settingitem), background);

    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
            TIMEDIDS_SCREEN_ITEM_PADDING_TOP,
            TIMEDIDS_SCREEN_ITEM_PADDING_BOTTOM,
            TIMEDIDS_SCREEN_ITEM_PADDING_LEFT,
            TIMEDIDS_SCREEN_ITEM_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // vbox
    vbox = gtk_vbox_new(FALSE, ENABLE_TIMEDIDS_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    // label
    label = gtk_label_new("");
    gtk_widget_set_name(label, "timedids_info_text");
    gtk_label_set_single_line_mode(GTK_LABEL(label), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_widget_set_size_request(label, ENABLE_TIMEDIDS_LABEL_WIDTH, -1);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    enable_timedids->label_desc = label;

    // hbox
    hbox = gtk_hbox_new(FALSE, SMALL_BUTTON_SPACING);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(hbox), FALSE, FALSE, 0);

    // button
    button = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(button), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
    enable_timedids->button_timedids_on = GTK_TOGGLE_BUTTON(button);
    button_tbl[0] = enable_timedids->button_timedids_on;

    // button
    button = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(button), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
    enable_timedids->button_timedids_off = GTK_TOGGLE_BUTTON(button);
    button_tbl[1] = enable_timedids->button_timedids_off;
    
    button_tbl[2] = NULL;
    // selection group not part of widget hierarchy
    selection = ergtk_selection_group_new(button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(selection), 1, 1);
    g_signal_connect(G_OBJECT(selection), "selection-update", G_CALLBACK(on_selection_update), NULL);
    
    gtk_widget_show_all(background);
    gtk_widget_show(settingitem);

    return settingitem;    
}

// widget hierarchy
// |--settingitem (GtkSettingItem)
//    |--background (GtkEventBox)
//       |--alignment
//          |--vbox
//             |--label
//             |--hbox
//                |--label   "Connect at"
//                |--entry   "Time option 1: hour"
//                |--label   ":"
//                |--entry   "Time option 1: minute"
//             |--hbox
//                |--label   "Connect at"
//                |--entry   "Time option 2: hour"
//                |--label   ":"
//                |--entry   "Time option 2: minute"
//                ...
//             |--checkbutton
//             |--checkbutton
GtkWidget* create_time_settings(gtk_time_settings_t* settime)
{
    int        i;
    GtkWidget* settingitem;
    GtkWidget* background;
    GtkWidget* alignment;
    GtkWidget* vbox;
    GtkWidget* label;
    GtkWidget* hbox;
    GtkWidget* entry;
    GtkWidget* check;
    
    ST_LOGPRINTF("entry");
    
    // settingitem
    settingitem = gtk_settingitem_new("");
    gtk_widget_set_size_request(settingitem, -1, -1);
    settime->settingitem = settingitem;

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "timedids_settime_background");
    gtk_widget_set_size_request(background, TITLE_MIN_WIDTH, TIME_SETTINGS_HEIGHT);
    gtk_settingitem_add_details(GTK_SETTINGITEM(settingitem), background);

    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
            TIMEDIDS_SCREEN_ITEM_PADDING_TOP,
            TIMEDIDS_SCREEN_ITEM_PADDING_BOTTOM,
            TIMEDIDS_SCREEN_ITEM_PADDING_LEFT,
            TIMEDIDS_SCREEN_ITEM_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // vbox
    vbox = gtk_vbox_new(FALSE, TIME_SETTINGS_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    // label
    label = gtk_label_new("");
    gtk_widget_set_name(label, "timedids_info_text");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    settime->label_desc = label;

    for (i=0; i<TIME_OPTIONS; i++)
    {
        // create a hbox to contain time entrys
        hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(hbox), FALSE, FALSE, 0);

        // label "Connect at"
        label = gtk_label_new("");
        gtk_widget_set_name(label, "timedids_info_text");
        gtk_widget_set_size_request(label, TIME_SETTINGS_CONNECTAT_WIDTH, -1);
        gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
        gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
        settime->time_entries[i].label_connectat = label;

        // hour entry
        entry = ergtk_entry_new();
        gtk_widget_set_size_request(entry, TIME_SETTINGS_ENTRY_WIDTH, TIME_SETTINGS_ENTRY_HEIGHT);
        gtk_entry_set_alignment(GTK_ENTRY(entry), 1.0);
        gtk_entry_set_max_length(GTK_ENTRY(entry), 2);
        g_signal_connect(GTK_OBJECT(entry), "insert_text", G_CALLBACK(on_hour_entry_insert_text), (gpointer)i);
        g_signal_connect(GTK_OBJECT(entry), "focus-out-event", G_CALLBACK(on_hour_entry_losefocus), (gpointer)i);
        gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);
        settime->time_entries[i].entry_hour = entry;

        // label colon
        label = gtk_label_new(":");
        gtk_widget_set_name(label, "timedids_info_text");
        gtk_widget_set_size_request(label, TIME_SETTINGS_COLON_WIDTH, -1);
        gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

        // minute entry
        entry = ergtk_entry_new();
        gtk_widget_set_size_request(entry, TIME_SETTINGS_ENTRY_WIDTH, TIME_SETTINGS_ENTRY_HEIGHT);
        gtk_entry_set_max_length(GTK_ENTRY(entry), 2);
        g_signal_connect(GTK_OBJECT(entry), "insert-text", G_CALLBACK(on_minute_entry_insert_text), (gpointer)i);
        g_signal_connect(GTK_OBJECT(entry), "focus-out-event", G_CALLBACK(on_minute_entry_losefocus), (gpointer)i);
        g_signal_connect(GTK_OBJECT(entry), "focus-in-event", G_CALLBACK(on_minute_entry_getfocus), (gpointer)i);
        gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);
        settime->time_entries[i].entry_minute = entry;
    }

    // check_software_update
    check = gtk_check_button_new_with_label("");
    gtk_box_pack_start(GTK_BOX(vbox), check, FALSE, FALSE, 0);
    g_signal_connect(check, "toggled", G_CALLBACK(on_checked_software_update), NULL);
    settime->check_software_update = check;
    
    // check_auto_shutdown
    check = gtk_check_button_new_with_label("");
    gtk_box_pack_start(GTK_BOX(vbox), check, FALSE, FALSE, 0);
    g_signal_connect(check, "toggled", G_CALLBACK(on_checked_switch_off), NULL);
    settime->check_auto_shutdown = check;
    
    gtk_widget_show_all(background);
    gtk_widget_show(settingitem);

    return settingitem;
}

void iLiad_timedids_screen_set_text(void)
{
    ST_LOGPRINTF("entry");
    if (g_timedids_screen == NULL)
    {
        return;
    }
    
    gtk_enable_timedids_t* enable_timedids = &g_timedids_screen->enable_timedids;
    gtk_time_settings_t* settime = &g_timedids_screen->settime;

    gtk_settingitem_set_header_text(GTK_SETTINGITEM(enable_timedids->settingitem),
            _("Time to connect to iDS"));
    gtk_label_set_text(GTK_LABEL(enable_timedids->label_desc),
            _("Enabling \"time to connection\" causes the iLiad to download content from the iDS at a set time."));
    
    gtk_button_set_label(GTK_BUTTON(enable_timedids->button_timedids_on), _("On"));
    gtk_button_set_label(GTK_BUTTON(enable_timedids->button_timedids_off), _("Off"));

    gtk_settingitem_set_header_text(GTK_SETTINGITEM(settime->settingitem),
            _("Set the time"));
    gtk_label_set_text(GTK_LABEL(settime->label_desc), _("The iLiad will connect to iDS at the set time:"));

    int i;
    for (i=0; i<TIME_OPTIONS; i++)
    {
        gtk_label_set_text(GTK_LABEL(settime->time_entries[i].label_connectat), _("Connect at"));
    }

    gtk_button_set_label(GTK_BUTTON(settime->check_software_update),
            _("Include software downloads while connected to iDS."));
    gtk_button_set_label(GTK_BUTTON(settime->check_auto_shutdown),
            _("Switch off again after waking up the device."));
    
    // Information text
    gtk_infoitem_set_text(GTK_INFOITEM(g_timedids_screen->infoitem),
            _("When the iLiad is switched off, it will 'wake-up' to download content from the iDS."));
}

void iLiad_timedids_screen_data_display(void)
{
    ST_LOGPRINTF("entry");
    
    gtk_enable_timedids_t* enable_timedids = &g_timedids_screen->enable_timedids;
    gtk_time_settings_t*   settime         = &g_timedids_screen->settime;

    g_cur_settings = get_current_timedids_settings();

    // Enable timed-iDS or not
    if (g_cur_settings->enable)
    {
        gtk_toggle_button_set_active(enable_timedids->button_timedids_on, TRUE);
    }
    else
    {
        gtk_toggle_button_set_active(enable_timedids->button_timedids_off, TRUE);
    }

    // Time set
    int i;
    char hh[4], mm[4];
    for (i=0; i<g_cur_settings->timeCnt; i++)
    {
        snprintf(hh, 4, "%02d", g_cur_settings->timeSet[i] / 3600);
        gtk_entry_set_text(GTK_ENTRY(settime->time_entries[i].entry_hour), hh);

        snprintf(mm, 4, "%02d", g_cur_settings->timeSet[i] / 60 % 60);
        gtk_entry_set_text(GTK_ENTRY(settime->time_entries[i].entry_minute), mm);
    }
    
    // Include software updates while connected to iDS
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settime->check_software_update),
            g_cur_settings->swUpdate);
    
    // Switch off iLiad again after waking iLiad up
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settime->check_auto_shutdown),
            g_cur_settings->switchOff);
}

void iLiad_timedids_handle_page_switch(void)
{
    // If disabled timed-iDS, toggle button
    gtk_enable_timedids_t* enable_timedids = &g_timedids_screen->enable_timedids;

    if (!g_cur_settings->enable)
    {
        gtk_toggle_button_set_active(enable_timedids->button_timedids_off, TRUE);
    }
}

void on_selection_update(erGtkSelectionGroup * selection, gpointer data)
{
    int index = 0;
    
    ST_LOGPRINTF("entry");
    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(selection), &index, 1);

    if (index == 0)
    {
        // User click the "on" button
        g_cur_settings->enable = TRUE;
        gtk_widget_show(g_timedids_screen->settime.settingitem);
    }
    else
    {
        // User click the "off" button
        g_cur_settings->enable = FALSE;
        gtk_widget_hide(g_timedids_screen->settime.settingitem);
    }
}

void on_checked_software_update(GtkToggleButton* button, gpointer data)
{
    ST_LOGPRINTF("entry");

    g_cur_settings->swUpdate = gtk_toggle_button_get_active(button);
}

void on_checked_switch_off(GtkToggleButton* button, gpointer data)
{
    ST_LOGPRINTF("entry");

    g_cur_settings->switchOff = gtk_toggle_button_get_active(button);
}

void on_hour_entry_insert_text(GtkEditable *editable,
                               gchar *new_text,
                               gint new_text_length,
                               gint *position,
                               gpointer user_data)
{
    int  count = 0;
    char szTmp[8] = {0};
    char valid_chars[3] = {0};
    char *firstchar = NULL;
    GtkWidget *widget = GTK_WIDGET(editable);

    ST_LOGPRINTF("entry");

    // i for loop indicator, j for the current char numbers in szTmp
    int i = 0, j = 0;

    if (*position >= 2)
    {
        gtk_signal_emit_stop_by_name(GTK_OBJECT (editable), "insert_text");
        return;
    }

    if (*position == 1)
    {
        firstchar = gtk_editable_get_chars(editable, 0, 1);
        szTmp[j++] = firstchar[0];
        g_free(firstchar);
    }

    for (i=0; i<new_text_length; i++)
    {
        if (isdigit(new_text[i]))
        {
            // Check that the digit is valid
            szTmp[j++] = new_text[i];
            if (atoi(szTmp) <= 24)
            {
                // Valid char
                valid_chars[count++] = new_text[i];
            }
        }
    }

    if (count > 0)
    {
        gtk_signal_handler_block_by_func(GTK_OBJECT(editable),
                GTK_SIGNAL_FUNC(on_hour_entry_insert_text),
                user_data);
        gtk_editable_insert_text(editable, valid_chars, count, position);
        gtk_signal_handler_unblock_by_func(GTK_OBJECT(editable),
                GTK_SIGNAL_FUNC(on_hour_entry_insert_text),
                user_data);

        // after second digit move focus to minutes entry
        firstchar = gtk_editable_get_chars(editable, 0, -1);  // get all characters
        if (strlen(firstchar) == 2)
        {
            for (i=0; i<TIME_OPTIONS; i++)
            {
                if (g_timedids_screen->settime.time_entries[i].entry_hour == widget)
                {
                    gtk_widget_grab_focus(g_timedids_screen->settime.time_entries[i].entry_minute);
                }
            }
        }
    }
    
    gtk_signal_emit_stop_by_name(GTK_OBJECT (editable), "insert_text");
}

void on_minute_entry_insert_text(GtkEditable *editable,
                         gchar *new_text,
                         gint new_text_length,
                         gint *position,
                         gpointer user_data)
{
    ST_LOGPRINTF("entry");
    
    int  count = 0;
    char szTmp[8] = {0};
    char valid_chars[3] = {0};
    char *firstchar = NULL;

    // First check if hour entry is empty
    int index = (int)user_data;
    GtkWidget* entry_hour = g_timedids_screen->settime.time_entries[index].entry_hour;
    const char* hour_str = gtk_entry_get_text(GTK_ENTRY(entry_hour));

    // Any input in minute entry is not allowed while hour entry is empty
    if (strlen(hour_str) == 0 || *position >= 2)
    {
        gtk_signal_emit_stop_by_name(GTK_OBJECT (editable), "insert_text");
        return;
    }

    int i = 0, j = 0;
    if (*position == 1)
    {
        firstchar = gtk_editable_get_chars(editable, 0, 1);
        szTmp[j++] = firstchar[0];
        g_free(firstchar);
    }

    for (i=0; i<new_text_length; i++)
    {
        if (isdigit(new_text[i]))
        {
            // Check that the digit is valid
            szTmp[j++] = new_text[i];
            if (atoi(szTmp) < 60)
            {
                // Valid char
                valid_chars[count++] = new_text[i];
            }
        }
    }

    if (count > 0)
    {
        gtk_signal_handler_block_by_func(GTK_OBJECT(editable),
                GTK_SIGNAL_FUNC(on_minute_entry_insert_text),
                user_data);
        gtk_editable_insert_text(editable, valid_chars, count, position);
        gtk_signal_handler_unblock_by_func(GTK_OBJECT(editable),
                GTK_SIGNAL_FUNC(on_minute_entry_insert_text),
                user_data);
    }
    
    gtk_signal_emit_stop_by_name(GTK_OBJECT (editable), "insert_text");
}

gboolean on_hour_entry_losefocus(GtkWidget *widget, GdkEventFocus *event, gpointer user_data)
{
    ST_LOGPRINTF("entry");
    
    const char* str = gtk_entry_get_text(GTK_ENTRY(widget));
    int len = strlen(str);
    int index = (int)user_data;
    GtkWidget* entry_minute = g_timedids_screen->settime.time_entries[index].entry_minute;

    if (len == 0)
    {
        // Hour entry is empty, empty minute entry too
        gtk_entry_set_text(GTK_ENTRY(entry_minute), "");
    }
    else if (atoi(str) > 23)
    {
        gtk_entry_set_text(GTK_ENTRY(widget), "00");
    }
    else
    {
        char hh[4] = {0};
        snprintf(hh, sizeof(hh), "%.2d", atoi(str));
        gtk_entry_set_text(GTK_ENTRY(widget), hh);
    }

    if (len > 0)
    {
        str = gtk_entry_get_text(GTK_ENTRY(entry_minute));
        if (strlen(str) == 0)
        {
            gtk_entry_set_text(GTK_ENTRY(entry_minute), "00");
        }
        gtk_editable_select_region(GTK_EDITABLE(entry_minute), 0, -1);
    }

    update_timeset();
    return FALSE;
}

gboolean on_minute_entry_getfocus(GtkWidget *widget, GdkEventFocus *event, gpointer user_data)
{
    ST_LOGPRINTF("entry");

    int index = (int)user_data;
    GtkWidget* entry_hour = g_timedids_screen->settime.time_entries[index].entry_hour;
    
    if (strlen(gtk_entry_get_text(GTK_ENTRY(entry_hour))) == 0)
    {
        // Hour entry is empty, empty minute entry and set focus on hour entry
        gtk_entry_set_text(GTK_ENTRY(widget), "");
        gtk_widget_grab_focus(entry_hour);
    }

    return FALSE;
}

gboolean on_minute_entry_losefocus(GtkWidget *widget, GdkEventFocus *event, gpointer user_data)
{
    ST_LOGPRINTF("entry");

    const char* str = gtk_entry_get_text(GTK_ENTRY(widget));
    int index = (int)user_data;
    GtkWidget* entry_hour = g_timedids_screen->settime.time_entries[index].entry_hour;
    
    str = gtk_entry_get_text(GTK_ENTRY(widget));
    
    if (strlen(gtk_entry_get_text(GTK_ENTRY(entry_hour))) == 0)
    {
        // Hour entry is empty, empty minute entry too
        gtk_entry_set_text(GTK_ENTRY(widget), "");
    }
    else if (atoi(str) > 59)
    {
        gtk_entry_set_text(GTK_ENTRY(widget), "59");
    }
    else
    {
        char mm[4] = {0};
        snprintf(mm, sizeof(mm), "%.2d", atoi(str));
        gtk_entry_set_text(GTK_ENTRY(widget), mm);
    }

    update_timeset();
    return FALSE;
}

void update_timeset(void)
{
    ST_LOGPRINTF("entry");

    g_cur_settings->timeCnt = 0;
    int i;
    for (i=0; i<TIME_OPTIONS; i++)
    {
        GtkWidget* entry_hour = g_timedids_screen->settime.time_entries[i].entry_hour;
        GtkWidget* entry_minute = g_timedids_screen->settime.time_entries[i].entry_minute;
        
        const char* str_hour = gtk_entry_get_text(GTK_ENTRY(entry_hour));
        const char* str_minute = gtk_entry_get_text(GTK_ENTRY(entry_minute));

        if (strlen(str_hour) > 0 && strlen(str_minute) > 0)
        {
            g_cur_settings->timeSet[g_cur_settings->timeCnt++] =
                atoi(str_hour) * 3600 + atoi(str_minute) * 60;
        }
    }

    if (0 == g_cur_settings->timeCnt)
    {
        g_cur_settings->enable = FALSE;
    }
}

