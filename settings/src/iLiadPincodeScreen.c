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
 * \file iLiadPincodeScreen.c
 * \brief settings - Pincode Screen
 *
 * in Pincode Screen of settings, user can switch on or off PINCODE
 * also can set new PINCODE
 *
 */
#include <string.h>

#include <gtk/gtk.h>

#include <liberdm/display.h>
#include <libergtk/ergtk.h>
#include <liberregxml/erregapi.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "system.h"
#include "gtkInfoItem.h"
#include "gtkSettingItem.h"
#include "gtkPincodeEntry.h"
#include "iLiadPincodeScreen.h"
#include "iLiadPincodeData.h"
#include "iLiadUserData.h"
#include "languages.h"
#include "settings.h"


static GtkWidget *g_pincode_onoff_item = NULL;
static GtkWidget *g_pincode_onoff_infolabel = NULL;
static GtkWidget *g_pincode_onoff_selection = NULL;
static GtkWidget *g_pincode_button_on  = NULL;
static GtkWidget *g_pincode_button_off = NULL;
static guint g_pincode_off = -1;
static guint g_pincode_on  = -1;

static GtkWidget *g_pincode_select_item = NULL;
static GtkWidget *g_pincode_select_infolabel = NULL;
static GtkWidget *g_old_pincode_label = NULL;
static GtkWidget *g_new_pincode_label = NULL;
static GtkWidget *g_verify_pincode_label = NULL;
static GtkWidget *g_old_pincode_entry = NULL;
static GtkWidget *g_new_pincode_entry = NULL;
static GtkWidget *g_verify_pincode_entry = NULL;
static GtkWidget *g_pincode_confirm_button = NULL;
static GtkWidget *g_pincode_message_label = NULL;
static GtkWidget *g_pincode_additional_info = NULL;

// internal functions forward declarations
static GtkWidget *create_pincode_onoff_item(void);
static GtkWidget *create_select_pincode_item(void);

static GtkWidget *create_pincode_select_main_area();
static GtkWidget *create_pincode_labels_entries();
static GtkWidget *create_pincode_additional_info();

static void show_help_text(gboolean is_pincode_on);

static void iLiad_pincode_set_message(const gchar * text);

// callbacks
static void on_pincode_onoff_selection_update(erGtkSelectionGroup* selection, 
                                              gpointer button, 
                                              gpointer data);
static gboolean iLiad_pincode_get_on(void);

static void on_confirm_button_toggled_event(GtkToggleButton * button, gpointer ptr);
static gboolean delayed_on_confirm_button_toggled(gpointer user_data);
// lock_screen update
static void  iLiad_pincode_screen_toolbar_update(void);


//
// object hierarchy:
//     g_vbox
//       |-- g_pincode_onoff_item (GtkSettingItem)
//       |-- g_pincode_select_item (GtkSettingItem)
//
GtkWidget *iLiad_pincode_create(void)
{
    GtkWidget *vbox;
    GtkWidget *onoff_item;
    GtkWidget *select_item;

    vbox = gtk_vbox_new(FALSE, PINCODE_SCREEN_SPACING_V);

    onoff_item = create_pincode_onoff_item();
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(onoff_item), FALSE, FALSE, 0);
    select_item = create_select_pincode_item();
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(select_item), FALSE, FALSE, 0);

    gtk_widget_show(onoff_item);
    gtk_widget_show(select_item);
    gtk_widget_show(vbox);

    return vbox;
}

// object hierarchy:
//     g_pincode_onoff_item (GtkSettingItem)
//       |-- selection (erGtkSelectionGroup)
//             |-- alignment
//                   |--vbox
//                        |-- g_pincode_onoff_infolabel (GtkLabel)
//                        |-- hbox
//                              |-- g_pincode_button_on
//                              |-- g_pincode_button_off
//
static GtkWidget* create_pincode_onoff_item(void)
{
    GtkWidget*       item;
    GtkWidget*       selection;
    GtkWidget*       alignment;
    GtkWidget*       vbox;
    GtkWidget*       label;
    GtkWidget*       hbox;
    GtkToggleButton* button_tbl[3];

    // g_pincode_onoff_item
    item = gtk_settingitem_new("");
    gtk_widget_set_size_request(item, (-1), (-1));

    // selection --> see below

    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              PINCODE_SCREEN_ITEM_PADDING_TOP,
                              PINCODE_SCREEN_ITEM_PADDING_BOTTOM,
                              PINCODE_SCREEN_ITEM_PADDING_LEFT,
                              PINCODE_SCREEN_ITEM_PADDING_RIGHT);

    // vbox
    vbox = gtk_vbox_new(FALSE, PINCODE_SCREEN_SPACING_V * 2);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    
    // g_pincode_onoff_infolabel
    label = gtk_label_new("");
    gtk_widget_set_name(label, "pincode_screen_info_text");
    gtk_widget_set_size_request(label, PINCODE_ONOFF_ITEM_LABEL_WIDTH, (-1));
    gtk_label_set_single_line_mode(GTK_LABEL(label), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    // hbox
    hbox = gtk_hbox_new(FALSE, SMALL_BUTTON_SPACING);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    //     g_pincode_button_on
    g_pincode_button_on = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(g_pincode_button_on), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), g_pincode_button_on, FALSE, FALSE, 0);
    //     g_pincode_button_off
    g_pincode_button_off = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(g_pincode_button_off), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), g_pincode_button_off, FALSE, FALSE, 0);
    // add details to selection
    button_tbl[0] = GTK_TOGGLE_BUTTON(g_pincode_button_on );
    button_tbl[1] = GTK_TOGGLE_BUTTON(g_pincode_button_off);
    button_tbl[2] = NULL;
    g_pincode_on  = 0;  // selection-group counts from 0
    g_pincode_off = 1;
    // selection
    selection = ergtk_selection_group_new(button_tbl);
    gtk_widget_set_name(selection, "pincode_onoff_background");
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(selection), 1, 1);
    gtk_widget_set_size_request(GTK_WIDGET(selection), PINCODE_ONOFF_BK_WIDTH, PINCODE_ONOFF_BK_HEIGHT);
    g_signal_connect(G_OBJECT(selection), "selection-update", G_CALLBACK(on_pincode_onoff_selection_update), NULL);
    gtk_container_add(GTK_CONTAINER(selection), alignment);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), selection);

    gtk_widget_show_all(selection);

    g_pincode_onoff_item = item;
    g_pincode_onoff_infolabel = label;
    g_pincode_onoff_selection = selection;
    return item;
}

//
// widget layout:
//   g_pincode_select_item (GtkSettingItem)
//     |-- vbox
//           |-- background(GtkEventBox)
//           |     |-- alignment
//           |           |-- widget(create_pincode_select_main_area)
//           |-- pincode_additional_info
//
static GtkWidget *create_select_pincode_item(void)
{
    GtkWidget *item;
    GtkWidget *vbox;
    GtkWidget *background;
    GtkWidget *alignment;
    GtkWidget *widget;
    GtkWidget *info;

    // g_pincode_select_item
    item = gtk_settingitem_new("");
    gtk_widget_set_size_request(GTK_WIDGET(item), (-1), (-1));
    // vbox
    vbox = gtk_vbox_new(FALSE, PINCODE_SCREEN_SPACING_V);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), vbox);
    // background
    background = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), background, FALSE, FALSE, 0);
    gtk_widget_set_name (GTK_WIDGET (background), "pincode_select_background");
    gtk_widget_set_size_request(background, PINCODE_SELECT_BK_WIDTH, PINCODE_SELECT_BK_HEIGHT);
    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(background), alignment);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              PINCODE_SCREEN_ITEM_PADDING_TOP,
                              PINCODE_SCREEN_ITEM_PADDING_BOTTOM,
                              PINCODE_SCREEN_ITEM_PADDING_LEFT, PINCODE_SCREEN_ITEM_PADDING_RIGHT);
    widget = create_pincode_select_main_area();
    gtk_container_add(GTK_CONTAINER(alignment), widget);
    // additional info
    info = create_pincode_additional_info();
    gtk_box_pack_start(GTK_BOX(vbox), info, FALSE, FALSE, 0);
    //
    gtk_widget_show_all(vbox);

    g_pincode_select_item = item;

    return item;
}

// widget layout:
//   vbox
//     |-- label
//     |-- labels_entries  // components
//     |-- hbox
//           |-- button    // GtkToggleButton
//           |-- message   // GtkLabel
//
static GtkWidget *create_pincode_select_main_area()
{
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *labels_entries;
    GtkWidget *hbox;
    GtkWidget *button;
    GtkWidget *message;

    // vbox
    vbox = gtk_vbox_new(FALSE, PINCODE_SELECT_ITEM_INTER_SPACING1);
    // label
    label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    gtk_widget_set_name(label, "pincode_screen_info_text");
    gtk_widget_set_size_request(label, PINCODE_SELECT_ITEM_LABEL_WIDTH, (-1));
    gtk_label_set_single_line_mode(GTK_LABEL(label), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
    // labels_entries
    labels_entries = create_pincode_labels_entries();
    gtk_box_pack_start(GTK_BOX(vbox), labels_entries, FALSE, FALSE, 0);
    // hbox
    hbox = gtk_hbox_new(FALSE, PINCODE_SCREEN_SPACING_H);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    // button
    button = ergtk_toggle_button_new_with_label("");
    gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(button), FALSE, FALSE, 0);
    gtk_widget_set_size_request(button, PINCODE_BUTTON_WIDTH, PINCODE_BUTTON_HEIGHT);
    g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(on_confirm_button_toggled_event), NULL);
    // message
    message = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(message), FALSE, FALSE, 0);
    gtk_widget_set_name(message, "pincode_screen_info_text");
    gtk_widget_set_size_request(message, PINCODE_SELECT_MESSAGE_WIDTH, (-1));
    gtk_label_set_single_line_mode(GTK_LABEL(label), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);

    g_pincode_select_infolabel = label;
    g_pincode_confirm_button = button;
    g_pincode_message_label = message;
    return vbox;
}

// widget layout:
//   vbox
//     |-- hbox1
//     |     |-- label1   // GtkLabel
//     |     |-- entry1   // GtkPincodeEntry
//     |-- hbox2
//     |     |-- label2   // GtkLabel
//     |     |-- entry2   // GtkPincodeEntry
//     |-- hbox3/
//           |-- label3   // GtkLabel
//           |-- entry3   // GtkPincodeEntry
//
static GtkWidget *create_pincode_labels_entries()
{
    GtkWidget *vbox;
    GtkWidget *hbox1;
    GtkWidget *hbox2;
    GtkWidget *hbox3;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;

    vbox = gtk_vbox_new(FALSE, PINCODE_SELECT_ITEM_INTER_SPACING2);
    // hbox1
    hbox1 = gtk_hbox_new(FALSE, PINCODE_SCREEN_SPACING_H);
    gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 0);
    // label1
    label1 = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(hbox1), label1, FALSE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(label1), "pincode_screen_info_text");
    gtk_widget_set_size_request(label1, PINCODE_LABELS_WIDTH, PINCODE_LABELS_HEIGHT);
    gtk_label_set_justify(GTK_LABEL(label1), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(label1), 0.0, 0.0);
    // entry1
    entry1 = gtk_pincode_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox1), entry1, FALSE, FALSE, 0);
    // hbox2
    hbox2 = gtk_hbox_new(FALSE, PINCODE_SCREEN_SPACING_H);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 0);
    // label2
    label2 = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(hbox2), label2, FALSE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(label2), "pincode_screen_info_text");
    gtk_widget_set_size_request(label2, PINCODE_LABELS_WIDTH, PINCODE_LABELS_HEIGHT);
    gtk_label_set_justify(GTK_LABEL(label2), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(label2), 0.0, 0.0);
    // entry2
    entry2 = gtk_pincode_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox2), entry2, FALSE, FALSE, 0);
    // hbox3
    hbox3 = gtk_hbox_new(FALSE, PINCODE_SCREEN_SPACING_H);
    gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 0);
    // label3
    label3 = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(hbox3), label3, FALSE, FALSE, 0);
    gtk_widget_set_name(GTK_WIDGET(label3), "pincode_screen_info_text");
    gtk_widget_set_size_request(label3, PINCODE_LABELS_WIDTH, PINCODE_LABELS_HEIGHT);
    gtk_label_set_justify(GTK_LABEL(label3), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(label3), 0.0, 0.0);
    // entry3
    entry3 = gtk_pincode_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox3), entry3, FALSE, FALSE, 0);

    g_old_pincode_label = label1;
    g_new_pincode_label = label2;
    g_verify_pincode_label = label3;
    g_old_pincode_entry = entry1;
    g_new_pincode_entry = entry2;
    g_verify_pincode_entry = entry3;
    return vbox;
}


// widget layout:
//   g_pincode_additional_info (GtkInfoItem)
//
static GtkWidget *create_pincode_additional_info()
{
    ST_LOGPRINTF("entry");

    // g_background_item_info
    g_pincode_additional_info = gtk_infoitem_new(FALSE);

    // display
    gtk_widget_show(g_pincode_additional_info);

    return g_pincode_additional_info;
}


void iLiad_pincode_set_text(void)
{
    ST_LOGPRINTF("entry");

    GtkWidget *item;
    GtkWidget *selection;
    GtkWidget *label;
    GtkWidget *button;

    // g_pincode_onoff_item
    if (g_pincode_onoff_item)
    {
        item = g_pincode_onoff_item;
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(item), _("PIN Code"));
        if (g_pincode_onoff_infolabel)
        {
            label = g_pincode_onoff_infolabel;
            gtk_label_set_text(GTK_LABEL(label),
                               _("The PIN code protects your reader against unauthorized use.\n"
                                 "The code is requested each time the reader is switched on."   ));
        }
        if (g_pincode_onoff_selection)
        {
            selection = g_pincode_onoff_selection;
            gtk_button_set_label(GTK_BUTTON(g_pincode_button_on ), _("On" ));
            gtk_button_set_label(GTK_BUTTON(g_pincode_button_off), _("Off"));
        }
    }
    // g_pincode_select_item
    if (g_pincode_select_item)
    {
        item = g_pincode_select_item;
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(item), _("Select PIN code"));
        if (g_pincode_select_infolabel)
        {
            label = g_pincode_select_infolabel;
            gtk_label_set_text(GTK_LABEL(label), _("Please select a PIN code of 4 digits."));
        }
        if (g_old_pincode_label)
        {
            label = g_old_pincode_label;
            gtk_label_set_text(GTK_LABEL(label), _("Please enter current PIN code"));
        }
        if (g_new_pincode_label)
        {
            label = g_new_pincode_label;
            gtk_label_set_text(GTK_LABEL(label), _("Please enter new PIN code"));
        }
        if (g_verify_pincode_label)
        {
            label = g_verify_pincode_label;
            gtk_label_set_text(GTK_LABEL(label), _("Please verify new PIN code"));
        }

        if (g_pincode_confirm_button)
        {
            button = g_pincode_confirm_button;
            gtk_button_set_label(GTK_BUTTON(button), _("Confirm"));
        }

        if (g_pincode_message_label)
        {
            label = g_pincode_message_label;
            gtk_label_set_text(GTK_LABEL(label), "");
        }
        if (g_pincode_additional_info)
        {
            show_help_text(iLiad_pincode_data_is_pincode_on());
        }
    }
}

void show_help_text(gboolean is_pincode_on)
{
    gboolean show_warning_msg = FALSE;

    if (is_pincode_on)
    {
        regUserProfile_t* user_profile = get_user_profile_ptr();

        if ((NULL == user_profile) ||
            (NULL == user_profile->name) || (user_profile->name[0] == '\0') ||
            (NULL == user_profile->email) || (user_profile->email[0] == '\0'))
        {
            show_warning_msg = TRUE;
        }
    }
    
    // Show warning message when switched to "ON" by an unregistered user
    if (show_warning_msg)
    {
        gtk_infoitem_set_text( GTK_INFOITEM(g_pincode_additional_info),
                               _("Please create a user account at 'MyiRex' and fill in the account settings in your iLiad."));
    }
    else
    {
        gtk_infoitem_set_text( GTK_INFOITEM(g_pincode_additional_info),
                               _("Warning!\n"
                                 "You can only enter the PIN code with the iLiad's stylus. "
                                 "Make sure that you always have the stylus available "
                                 "after enabling the pin code functionality."));
    }
}

// display pincode data
void iLiad_pincode_display_data(void)
{
    const gboolean enable  = iLiad_pincode_data_is_pincode_on();

    if (enable)
    {
        ST_LOGPRINTF("PIN code on");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_pincode_button_on), TRUE);
    }
    else
    {
        ST_LOGPRINTF("PIN code off");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_pincode_button_off), TRUE);
    }
}

static gboolean iLiad_pincode_get_on(void)
{
    GtkWidget* selection = g_pincode_onoff_selection;
    gint       selection_id;
    gboolean   returnValue = FALSE;

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(selection), &selection_id, 1);

    if (g_pincode_on == selection_id)
    {
        ST_LOGPRINTF("PIN code on");
        returnValue = TRUE;
    }
    else if (g_pincode_off == selection_id)
    {
        ST_LOGPRINTF("PIN code off");
        returnValue = FALSE;
    }
    else
    {
        ST_ERRORPRINTF("invalid pincode on/off %d", selection_id);
        //terminate application if this point is reached
        g_assert_not_reached();
    }
    return returnValue;
}

static void iLiad_pincode_set_message(const gchar * text)
{
    GtkWidget *label;

    label = g_pincode_message_label;
    if (label && text)
    {
        gtk_label_set_text(GTK_LABEL(label), text);
        gtk_widget_show(label);
        display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_TYPING);
    }
}

// callback functions 
static void on_pincode_onoff_selection_update(erGtkSelectionGroup* selection, 
                                              gpointer button, 
                                              gpointer data)
{
    gboolean enable;

    enable = iLiad_pincode_get_on();

    ST_LOGPRINTF("pincode %s", enable == 0 ? "OFF" : "ON");

    show_help_text(enable);

    iLiad_pincode_data_set_pincode_on(enable);

    // update toolbar lock screen icon
    iLiad_pincode_screen_toolbar_update();

    // for pincode, store into sysset immediately
    iLiad_pincode_data_store();

    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_TYPING);
}

// confirm button selected
static void on_confirm_button_toggled_event(GtkToggleButton * button, gpointer ptr)
{
    // check if it is a "pressed event"
    gboolean pressed = gtk_toggle_button_get_active(button);
    if (pressed)
    {
        // show button selected, then start check pincode
        display_update_request_screen_refresh(TEXT_ENTRY_CHANGED_LEVEL, WAVEFORM_TYPING);
        g_timeout_add(500, delayed_on_confirm_button_toggled, ptr);
    }
}

static gboolean delayed_on_confirm_button_toggled(gpointer user_data)
{
    GtkWidget *button = g_pincode_confirm_button;

    gboolean check_old = FALSE;
    gboolean check_new = FALSE;
    gboolean check_verify = FALSE;

    const gchar *old_pincode = NULL;
    const gchar *new_pincode = NULL;
    const gchar *verify_pincode = NULL;

    gchar *pincode = NULL;

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);

    pincode = iLiad_pincode_data_get_pincode();
    if ((NULL == pincode) || (0 == strlen(pincode)))
    {
        check_old = TRUE;
    }
    else
    {
        check_old = gtk_pincode_entry_check_text(GTK_PINCODE_ENTRY(g_old_pincode_entry));
    }
    check_new    = gtk_pincode_entry_check_text(GTK_PINCODE_ENTRY(g_new_pincode_entry));
    check_verify = gtk_pincode_entry_check_text(GTK_PINCODE_ENTRY(g_verify_pincode_entry));
    if (FALSE == check_old || FALSE == check_new || FALSE == check_verify)
    {
        gtk_entry_set_text(GTK_ENTRY(g_old_pincode_entry), "");
        gtk_entry_set_text(GTK_ENTRY(g_new_pincode_entry), "");
        gtk_entry_set_text(GTK_ENTRY(g_verify_pincode_entry), "");
        iLiad_pincode_set_message(_("Illegal characters or illegal length, pincode unchanged"));
        return FALSE;
    }

    new_pincode    = gtk_entry_get_text(GTK_ENTRY(g_new_pincode_entry));
    verify_pincode = gtk_entry_get_text(GTK_ENTRY(g_verify_pincode_entry));
    if (strcmp(new_pincode, verify_pincode))
    {
        gtk_entry_set_text(GTK_ENTRY(g_old_pincode_entry), "");
        gtk_entry_set_text(GTK_ENTRY(g_new_pincode_entry), "");
        gtk_entry_set_text(GTK_ENTRY(g_verify_pincode_entry), "");

        iLiad_pincode_set_message(_("Pincode verification failed, pincode unchanged"));
        return FALSE;
    }
    if (NULL == pincode || 0 == strlen(pincode))
    {
        iLiad_pincode_data_set_pincode(new_pincode);
        iLiad_pincode_display_data();

        // for pincode, store into sysset immediately
        iLiad_pincode_data_store();

        iLiad_pincode_set_message(_("Pincode updated successfully"));
        // update toolbar lock screen icon
        iLiad_pincode_screen_toolbar_update();

        gtk_entry_set_text(GTK_ENTRY(g_old_pincode_entry), "");
        gtk_entry_set_text(GTK_ENTRY(g_new_pincode_entry), "");
        gtk_entry_set_text(GTK_ENTRY(g_verify_pincode_entry), "");

        return FALSE;
    }
    old_pincode = gtk_entry_get_text(GTK_ENTRY(g_old_pincode_entry));
    if (strcmp(old_pincode, pincode))
    {
        gtk_entry_set_text(GTK_ENTRY(g_old_pincode_entry), "");
        gtk_entry_set_text(GTK_ENTRY(g_new_pincode_entry), "");
        gtk_entry_set_text(GTK_ENTRY(g_verify_pincode_entry), "");

        iLiad_pincode_set_message(_("Current pincode incorrect, pincode unchanged"));
    }
    else
    {
        iLiad_pincode_data_set_pincode(new_pincode);
        iLiad_pincode_display_data();

        // for pincode, store into sysset immediately
        iLiad_pincode_data_store();

        iLiad_pincode_set_message(_("Pincode updated successfully"));
        // update toolbar lock screen icon
        iLiad_pincode_screen_toolbar_update();

        gtk_entry_set_text(GTK_ENTRY(g_old_pincode_entry), "");
        gtk_entry_set_text(GTK_ENTRY(g_new_pincode_entry), "");
        gtk_entry_set_text(GTK_ENTRY(g_verify_pincode_entry), "");
    }
    return FALSE;               // don't call me anymore
}

static void  iLiad_pincode_screen_toolbar_update(void)
{
    gboolean enable = FALSE;
    gchar *pincode = NULL;
    
    // update toolbar lock screen icon
    enable =  iLiad_pincode_data_is_pincode_on();
    pincode = iLiad_pincode_data_get_pincode();

    if (FALSE == enable || NULL == pincode || 0 == strlen(pincode))
    {
        show_lock_screen_icon(FALSE);
    }
    else
    {
        show_lock_screen_icon(TRUE);
    }
}
