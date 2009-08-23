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
 * \file iLiadConfigScreen.c
 * \brief setup application - E-reader config screen                

 * <File description>
 * 
 */

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

#include <gtk/gtk.h>

#include <libergtk/ergtk.h>

#include "setupLog.h"
#include "gtkInfoItem.h"
#include "gtkSettingItem.h"
#include "iLiadConfigScreen.h"
#include "iLiadConfigData.h"
#include "languages.h"
#include "background.h"

// Locales currently installed
static iLiad_locale_t*  g_locale_installed_tbl = NULL;
static guint            g_locale_installed_num = 0;
 
static GtkWidget* g_vbox = NULL;

static GtkWidget* g_calibration_item   = NULL;
static GtkWidget* g_calibration_button = NULL;

static GtkWidget* g_sound_item      = NULL;
static GtkWidget* g_sound_selection = NULL;
enum
{
    e_sound_on = 0,
    e_sound_off,
    e_sound_undefined
};
static GtkWidget* g_sound_buttons[e_sound_undefined];

static GtkWidget* g_keys_item      = NULL;
static GtkWidget* g_keys_selection = NULL;
enum
{
    e_keys_0_5_sec = 0,
    e_keys_1_0_sec,
    e_keys_1_5_sec,
    e_keys_2_0_sec,
    e_keys_undefined
};
static GtkWidget* g_keys_buttons[e_keys_undefined];

static GtkWidget* g_flipbar_direction_item      = NULL;
static GtkWidget* g_flipbar_direction_selection = NULL;
enum
{
    e_flipbar_direction_left = 0,
    e_flipbar_direction_right,
    e_flipbar_direction_undefined
};
static GtkWidget* g_flipbar_direction_buttons[e_flipbar_direction_undefined];

static GtkWidget* g_keybuffering_item      = NULL;
static GtkWidget* g_keybuffering_selection = NULL;
enum
{
    e_keybuffering_on = 0,
    e_keybuffering_off,
    e_keybuffering_undefined
};
static GtkWidget* g_keybuffering_buttons[e_keybuffering_undefined];

static GtkWidget*  g_language_item      = NULL;
static GtkWidget*  g_language_selection = NULL;
static GtkWidget** g_language_buttons   = NULL;

//internally used routines
static GtkWidget* create_calibration_item(void);
static GtkWidget* create_sound_item(void);
static GtkWidget* create_keys_item(void);
static GtkWidget* create_flipbar_direction_item(void);
static GtkWidget* create_keybuffering_item(void);
static GtkWidget* create_language_item(void);

//
// object hierarchy:
//     g_vbox
//       |-- g_sound_item (GtkSettingItem)
//       |-- g_keybuffering_item(GtkSettingItem)
//       |-- g_keys_item (GtkSettingItem)
//       |-- g_flipbar_direction_item(GtkSettingItem)
//       |-- g_calibration_item (GtkSettingItem)
//       |-- g_language_item (GtkSettingItem)
//
GtkWidget *iLiad_config_create(void)
{
    g_locale_installed_num = find_installed_locales(&g_locale_installed_tbl);

    g_vbox = gtk_vbox_new(FALSE,  CONFIGSCREEN_ITEM_SPACING);

    g_sound_item = create_sound_item();
    gtk_box_pack_start(GTK_BOX(g_vbox), GTK_WIDGET(g_sound_item), FALSE, FALSE, 0);

    g_keybuffering_item = create_keybuffering_item();
    gtk_box_pack_start(GTK_BOX(g_vbox), GTK_WIDGET(g_keybuffering_item), FALSE, FALSE, 0);

    g_keys_item = create_keys_item();
    gtk_box_pack_start(GTK_BOX(g_vbox), GTK_WIDGET(g_keys_item), FALSE, FALSE, 0);
    
    g_flipbar_direction_item = create_flipbar_direction_item();
    gtk_box_pack_start(GTK_BOX(g_vbox), GTK_WIDGET(g_flipbar_direction_item), FALSE, FALSE, 0);

    g_calibration_item = create_calibration_item();
    //TODO implement Wacom calibration
    //TODO gtk_box_pack_start(GTK_BOX(g_vbox), GTK_WIDGET(g_calibration_item), FALSE, FALSE, 0);

    g_language_item = create_language_item();
    gtk_box_pack_start(GTK_BOX(g_vbox), GTK_WIDGET(g_language_item), FALSE, FALSE, 0);

    gtk_widget_show(g_vbox);

    return g_vbox;
}


static GtkWidget* create_sound_item(void)
{
    int              i;
    GtkWidget*       alignment;
    GtkWidget*       hbox;
    GtkWidget*       button;
    GtkToggleButton* button_tbl[e_sound_undefined + 1];


    // widget hierarchy:
    //     g_sound_item (GtkSettingItem)
    //       |
    g_sound_item = gtk_settingitem_new("");
    gtk_widget_set_size_request(GTK_WIDGET(g_sound_item), (-1), (-1));
    //       |
    //       |-- alignment
    //             |
    alignment = gtk_alignment_new(0, 0.5, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), CONFIGSCREEN_INTRA_PADDING, CONFIGSCREEN_INTER_PADDING, 0, 0);
    gtk_settingitem_add_details(GTK_SETTINGITEM(g_sound_item), alignment);
    //             |
    //             |-- g_sound_selection
    //                   |-- hbox
    //                         |-- g_sound_buttons[..]
    //
    hbox = gtk_hbox_new(FALSE, SMALL_BUTTON_SPACING);
    //
    for (i = 0 ; i < e_sound_undefined ; i++)
    {
        button = ergtk_toggle_button_new_with_label("");
        button_tbl[i] = GTK_TOGGLE_BUTTON(button);
        gtk_widget_set_name(button, "button_without_border");
        gtk_widget_set_size_request(button, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
        gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

        g_sound_buttons[i] = GTK_WIDGET(button);
    }
    button_tbl[i] = NULL;
    //
    g_sound_selection = ergtk_selection_group_new(button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(g_sound_selection), 1, 1);
    g_signal_connect(G_OBJECT(g_sound_selection), "selection-update", G_CALLBACK(on_sound_selection_update), NULL);
    gtk_container_add(GTK_CONTAINER(g_sound_selection), hbox);
    gtk_container_add(GTK_CONTAINER(alignment), g_sound_selection);

    gtk_widget_show_all(g_sound_item);
    return g_sound_item;
}


// object hierarchy:
//     g_calibration_item (GtkSettingItem)
//       |-- alignment
//             |-- g_calibration_button (button)
//
static GtkWidget *create_calibration_item(void)
{
    GtkWidget *alignment;

    // g_calibration_item
    g_calibration_item = gtk_settingitem_new("");
    gtk_widget_set_size_request(GTK_WIDGET(g_calibration_item), (-1), (-1));

    // alignment
    alignment = gtk_alignment_new(0, 0.5, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), CONFIGSCREEN_INTRA_PADDING, CONFIGSCREEN_INTER_PADDING, 0, 0);
    gtk_settingitem_add_details(GTK_SETTINGITEM(g_calibration_item), alignment);

    // g_calibration_button
    g_calibration_button = gtk_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(g_calibration_button), SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
    gtk_container_add(GTK_CONTAINER(alignment), g_calibration_button);

    gtk_widget_show(g_calibration_button);
    gtk_widget_show(alignment);
    gtk_widget_show(g_calibration_item);

    return g_calibration_item;
}


static GtkWidget *create_keys_item(void)
{
    int              i;
    GtkWidget*       alignment;
    GtkWidget*       hbox;
    GtkWidget*       button;
    GtkToggleButton* button_tbl[e_keys_undefined + 1];


    // widget hierarchy:
    //     g_keys_item (GtkSettingItem)
    //       |
    g_keys_item = gtk_settingitem_new("");
    gtk_widget_set_size_request(GTK_WIDGET(g_keys_item), (-1), (-1));
    //       |
    //       |-- alignment
    //             |
    alignment = gtk_alignment_new(0, 0.5, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), CONFIGSCREEN_INTRA_PADDING, CONFIGSCREEN_INTER_PADDING, 0, 0);
    gtk_settingitem_add_details(GTK_SETTINGITEM(g_keys_item), alignment);
    //             |
    //             |-- g_keys_selection
    //                   |-- hbox
    //                         |-- g_keys_buttons[..]
    //
    hbox = gtk_hbox_new(FALSE, SMALL_BUTTON_SPACING);
    //
    for (i = 0 ; i < e_keys_undefined ; i++)
    {
        button = ergtk_toggle_button_new_with_label("");
        button_tbl[i] = GTK_TOGGLE_BUTTON(button);
        gtk_widget_set_name(button, "button_without_border");
        gtk_widget_set_size_request(button, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
        gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

        g_keys_buttons[i] = GTK_WIDGET(button);
    }
    button_tbl[i] = NULL;
    //
    g_keys_selection = ergtk_selection_group_new(button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(g_keys_selection), 1, 1);
    g_signal_connect(G_OBJECT(g_keys_selection), "selection-update", G_CALLBACK(on_keys_selection_update), NULL);
    gtk_container_add(GTK_CONTAINER(g_keys_selection), hbox);
    gtk_container_add(GTK_CONTAINER(alignment), g_keys_selection);

    gtk_widget_show_all(g_keys_item);
    return g_keys_item;
}

// object hierarchy:
//     g_keybuffering_item (GtkSettingItem)
//       |-- alignment
//             |-- g_keybuffering_selection (erGtkSelectionGroup)
//                   |-- g_keybuffering_on  (guint)
//                   |-- g_keybuffering_off (guint)
//
static GtkWidget *create_keybuffering_item(void)
{
    int              i;
    GtkWidget*       alignment;
    GtkWidget*       hbox;
    GtkWidget*       button;
    GtkToggleButton* button_tbl[e_keybuffering_undefined + 1];


    // widget hierarchy:
    //     g_keybuffering_item (GtkSettingItem)
    //       |
    g_keybuffering_item = gtk_settingitem_new("");
    gtk_widget_set_size_request(GTK_WIDGET(g_keybuffering_item), (-1), (-1));
    //       |
    //       |-- alignment
    //             |
    alignment = gtk_alignment_new(0, 0.5, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), CONFIGSCREEN_INTRA_PADDING, CONFIGSCREEN_INTER_PADDING, 0, 0);
    gtk_settingitem_add_details(GTK_SETTINGITEM(g_keybuffering_item), alignment);
    //             |
    //             |-- g_keybuffering_selection
    //                   |-- hbox
    //                         |-- g_keybuffering_buttons[..]
    //
    hbox = gtk_hbox_new(FALSE, SMALL_BUTTON_SPACING);
    //
    for (i = 0 ; i < e_keybuffering_undefined ; i++)
    {
        button = ergtk_toggle_button_new_with_label("");
        button_tbl[i] = GTK_TOGGLE_BUTTON(button);
        gtk_widget_set_name(button, "button_without_border");
        gtk_widget_set_size_request(button, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
        gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

        g_keybuffering_buttons[i] = GTK_WIDGET(button);
    }
    button_tbl[i] = NULL;
    //
    g_keybuffering_selection = ergtk_selection_group_new(button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(g_keybuffering_selection), 1, 1);
    g_signal_connect(G_OBJECT(g_keybuffering_selection), "selection-update", G_CALLBACK(on_keybuffering_selection_update), NULL);
    gtk_container_add(GTK_CONTAINER(g_keybuffering_selection), hbox);
    gtk_container_add(GTK_CONTAINER(alignment), g_keybuffering_selection);

    gtk_widget_show_all(g_keybuffering_item);
    return g_keybuffering_item;
}


static GtkWidget *create_flipbar_direction_item(void)
{
    int              i;
    GtkWidget*       alignment;
    GtkWidget*       hbox;
    GtkWidget*       button;
    GtkToggleButton* button_tbl[e_flipbar_direction_undefined + 1];


    // widget hierarchy:
    //     g_flipbar_direction_item (GtkSettingItem)
    //       |
    g_flipbar_direction_item = gtk_settingitem_new("");
    gtk_widget_set_size_request(GTK_WIDGET(g_flipbar_direction_item), (-1), (-1));
    //       |
    //       |-- alignment
    //             |
    alignment = gtk_alignment_new(0, 0.5, 0, 0);    
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), CONFIGSCREEN_INTRA_PADDING, CONFIGSCREEN_INTER_PADDING, 0, 0);
    gtk_settingitem_add_details(GTK_SETTINGITEM(g_flipbar_direction_item), alignment);
    //             |
    //             |-- g_flipbar_direction_item
    //                   |-- hbox
    //                         |-- g_flipbar_direction_buttons[..]
    //
    hbox = gtk_hbox_new(FALSE, SMALL_BUTTON_SPACING);
    //
    for (i = 0 ; i < e_flipbar_direction_undefined ; i++)
    {
        button = ergtk_toggle_button_new_with_label("");
        button_tbl[i] = GTK_TOGGLE_BUTTON(button);
        gtk_widget_set_name(button, "button_without_border");
        gtk_widget_set_size_request(button, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
        gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

        g_flipbar_direction_buttons[i] = GTK_WIDGET(button);
    }
    button_tbl[i] = NULL;
    //
    g_flipbar_direction_selection = ergtk_selection_group_new(button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(g_flipbar_direction_selection), 1, 1);
    g_signal_connect(G_OBJECT(g_flipbar_direction_selection), "selection-update", G_CALLBACK(on_flipbar_direction_selection_update), NULL);
    gtk_container_add(GTK_CONTAINER(g_flipbar_direction_selection), hbox);
    gtk_container_add(GTK_CONTAINER(alignment), g_flipbar_direction_selection);

    gtk_widget_show_all(g_flipbar_direction_item);
    return g_flipbar_direction_item;
}


static GtkWidget *create_language_item(void)
{
    int              i;
    GtkWidget*       alignment;
    GtkWidget*       vbox;
    GtkWidget*       hbox;
    GtkWidget*       button;
    GtkToggleButton* button_tbl[e_keys_undefined + 1];


    // widget hierarchy:
    //     g_language_item (GtkSettingItem)
    //       |
    g_language_item = gtk_settingitem_new("");
    gtk_widget_set_size_request(GTK_WIDGET(g_language_item), (-1), (-1));
    //       |
    //       |-- alignment
    //             |
    alignment = gtk_alignment_new(0, 0.5, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), CONFIGSCREEN_INTRA_PADDING, CONFIGSCREEN_INTER_PADDING, 0, 0);
    gtk_settingitem_add_details(GTK_SETTINGITEM(g_language_item), alignment);
    //             |
    //             |-- g_language_selection
    //                   |-- vbox
    //                         |-- hbox[..]
    //                               |-- g_language_buttons[..]
    //
    vbox = gtk_vbox_new(TRUE, LARGE_BUTTON_SPACING);
    hbox = NULL;  // avoid compiler warning: 'hbox' might be used uninitialized in this function
    g_language_buttons = g_new0(GtkWidget*, g_locale_installed_num);
    for (i = 0 ; i < g_locale_installed_num ; i++)
    {
        if ((i % LANGUAGE_BUTTONS_PER_LINE) == 0)
        {
            hbox = gtk_hbox_new(FALSE, SMALL_BUTTON_SPACING);
            gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
        }
        button = ergtk_toggle_button_new_with_label( g_locale_installed_tbl[i].name );
        button_tbl[i] = GTK_TOGGLE_BUTTON(button);
        gtk_widget_set_name(button, "button_without_border");
        gtk_widget_set_size_request(button, SMALL_BUTTON_WIDTH, SMALL_BUTTON_HEIGHT);
        gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

        g_language_buttons[i] = GTK_WIDGET(button);
    }
    button_tbl[i] = NULL;
    //
    g_language_selection = ergtk_selection_group_new(button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(g_language_selection), 1, 1);
    g_signal_connect(G_OBJECT(g_language_selection), "selection-update", G_CALLBACK(on_language_selection_update), NULL);
    gtk_container_add(GTK_CONTAINER(g_language_selection), vbox);
    gtk_container_add(GTK_CONTAINER(alignment), g_language_selection);

    gtk_widget_show_all(g_language_item);
    return g_language_item;
}

void iLiad_config_set_text(void)
{
    ST_LANGUAGEPRINTF("entry");

    // sound
    if (g_sound_item)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_sound_item), _("Sound"));
        gtk_settingitem_set_info_text(GTK_SETTINGITEM(g_sound_item),
                                      _("With this function you can enable or disable the sound of your iLiad."));
        gtk_button_set_label(GTK_BUTTON(g_sound_buttons[e_sound_on ]), _("On" ));
        gtk_button_set_label(GTK_BUTTON(g_sound_buttons[e_sound_off]), _("Off"));
    }

    // keybuffering
    if (g_keybuffering_item)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_keybuffering_item), _("Key buffering"));
        gtk_settingitem_set_info_text(GTK_SETTINGITEM(g_keybuffering_item),
                                      _("When you enable key buffering, the iLiad will remember the keys pressed and process them when the device is ready."));
        gtk_button_set_label(GTK_BUTTON(g_keybuffering_buttons[e_keybuffering_on ]), _("On" ));
        gtk_button_set_label(GTK_BUTTON(g_keybuffering_buttons[e_keybuffering_off]), _("Off"));
    }

    // keys
    if (g_keys_item)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_keys_item), _("Keys"));
        gtk_settingitem_set_info_text(GTK_SETTINGITEM(g_keys_item),
                                      _("With this function you can set the minimum duration for a long keypress."));
        gtk_button_set_label(GTK_BUTTON(g_keys_buttons[e_keys_0_5_sec]), _("0.5 seconds"));
        gtk_button_set_label(GTK_BUTTON(g_keys_buttons[e_keys_1_0_sec]), _("1 second")   );
        gtk_button_set_label(GTK_BUTTON(g_keys_buttons[e_keys_1_5_sec]), _("1.5 seconds"));
        gtk_button_set_label(GTK_BUTTON(g_keys_buttons[e_keys_2_0_sec]), _("2 seconds")  );
    }

    // flipbar direction
    if (g_flipbar_direction_item)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_flipbar_direction_item), _("Flipbar direction"));
        gtk_settingitem_set_info_text(GTK_SETTINGITEM(g_flipbar_direction_item),
                                      _("With this function you choose how to move the flipbar for advancing in a document."));
        gtk_button_set_label(GTK_BUTTON(g_flipbar_direction_buttons[e_flipbar_direction_left ]), _("Left") );
        gtk_button_set_label(GTK_BUTTON(g_flipbar_direction_buttons[e_flipbar_direction_right]), _("Right"));
    }

    // wacom
    if (g_calibration_item)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_calibration_item), _("Pen interface"));
        gtk_button_set_label(GTK_BUTTON(g_calibration_button), _("Calibration"));
    }

    // language
    if (g_language_item)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_language_item), _("Languages"));
        gtk_settingitem_set_info_text(GTK_SETTINGITEM(g_language_item),
                                      _("With this function you can set the language of the iLiad."));
    }
}

void iLiad_config_set_language(const gchar *locale)
{
    gint  i;

    for (i = 0 ; i < g_locale_installed_num ; i++)
    {
        if (strcmp(locale, g_locale_installed_tbl[i].code) == 0)
        {
            // found
            break;   // exit for
        }
    }
    if (i == g_locale_installed_num)
    {
        // not found
        iLiad_config_set_language(DEFAULT_LOCALE);
    }
    else
    {
        // found
        ST_LANGUAGEPRINTF("%s %s", locale, g_locale_installed_tbl[i].name);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_language_buttons[i]), TRUE);
    }
}

const gchar *iLiad_config_get_language(void)
{
    gint        currentLanguage;
    const gchar *returnValue = DEFAULT_LOCALE;

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(g_language_selection), &currentLanguage, 1);
    if (currentLanguage >= 0  &&  currentLanguage < g_locale_installed_num)
    {
        returnValue = g_locale_installed_tbl[currentLanguage].code;
    }

    return returnValue;
}

void iLiad_config_set_keys(guint timeout)
{
    if (timeout <= 500)
    {
        ST_LOGPRINTF("zero_point_five_sec");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_keys_buttons[e_keys_0_5_sec]), TRUE);
    }
    else if (timeout <= 1000)
    {
        ST_LOGPRINTF("one_sec");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_keys_buttons[e_keys_1_0_sec]), TRUE);
    }
    else if (timeout <= 1500)
    {
        ST_LOGPRINTF("one_point_five_sec");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_keys_buttons[e_keys_1_5_sec]), TRUE);
    }
    else
    {
        ST_LOGPRINTF("two_sec_sec");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_keys_buttons[e_keys_2_0_sec]), TRUE);
    }
}

guint iLiad_config_get_keys(void)
{
    gint    currentTimeOut = 0;
    guint   returnValue    = 500;

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(g_keys_selection), &currentTimeOut, 1);

    switch (currentTimeOut)
    {
        case e_keys_0_5_sec:
            ST_LOGPRINTF("zero_point_five_sec");
            returnValue = 500;
            break;
        case e_keys_1_0_sec:
            ST_LOGPRINTF("one_sec");
            returnValue = 1000;
            break;
        case e_keys_1_5_sec:
            ST_LOGPRINTF("one_point_five_sec");
            returnValue = 1500;
            break;
        case e_keys_2_0_sec:
            ST_LOGPRINTF("two_sec");
            returnValue = 2000;
            break;
        default:
            ST_ERRORPRINTF("invalid keys ID %d", currentTimeOut);
            g_assert_not_reached();
    }
    return returnValue;
}

void iLiad_config_set_volume(guint volume)
{
    if (0 == volume)
    {
        ST_LOGPRINTF("Sound off");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_sound_buttons[e_sound_off]), TRUE);
    }
    else
    {
        ST_LOGPRINTF("Sound on");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_sound_buttons[e_sound_on]), TRUE);
    }
}

guint iLiad_config_get_volume(void)
{
    gint    current_sound = 0;
    guint   returnValue   = 0;

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(g_sound_selection), &current_sound, 1);

    switch (current_sound)
    {
        case e_sound_off:
            ST_LOGPRINTF("sound off");
            returnValue = 0;
            break;
        case e_sound_on:
            ST_LOGPRINTF("sound on");
            returnValue = 1;
            break;
        default:
            ST_ERRORPRINTF("invalid sound ID %d", current_sound);
            g_assert_not_reached();
    }
    return returnValue;
}

void iLiad_config_set_keybuffering(guint volume)
{
    if (volume == 0)
    {
        ST_LOGPRINTF("KeyBuffering off");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_keybuffering_buttons[e_keybuffering_off]), TRUE);
    }
    else
    {
        ST_LOGPRINTF("KeyBuffering on");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_keybuffering_buttons[e_keybuffering_on]), TRUE);
    }
}

guint iLiad_config_get_keybuffering(void)
{
    gint    currentBuffering = 0;
    guint   returnValue      = 0;

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(g_keybuffering_selection), &currentBuffering, 1);

    switch (currentBuffering)
    {
        case e_keybuffering_on:
            ST_LOGPRINTF("KeyBuffering on");
            returnValue = 1;
            break;
        case e_keybuffering_off:
            ST_LOGPRINTF("KeyBuffering off");
            returnValue = 0;
            break;
        default:
            ST_ERRORPRINTF("invalid keyBuffering ID %d", currentBuffering);
            g_assert_not_reached();
    }
    return returnValue;
}

void iLiad_config_set_flipbar_direction(gboolean left)
{
    if (left)
    {
        ST_LOGPRINTF("Flipbar Left");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_flipbar_direction_buttons[e_flipbar_direction_left]), TRUE);
    }
    else
    {
        ST_LOGPRINTF("Flipbar Right");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_flipbar_direction_buttons[e_flipbar_direction_right]), TRUE);
    }
}

gboolean iLiad_config_get_flipbar_direction(void)
{
    gint       currentDirection = 0;
    gboolean   returnValue      = 0;

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(g_flipbar_direction_selection), &currentDirection, 1);

    switch (currentDirection)
    {
        case e_flipbar_direction_left:
            ST_LOGPRINTF("Flipbar Left");
            returnValue = TRUE;
            break;
        case e_flipbar_direction_right:
            ST_LOGPRINTF("Flipbar Right");
            returnValue = FALSE;
            break;
        default:
            ST_ERRORPRINTF("invalid flipbarDirection ID %d", currentDirection);
            g_assert_not_reached();
    }
    return returnValue;
}


