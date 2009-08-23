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
 * \file gtkSettingItemTextEntry.c
 * \brief setup application - setup item containing one text entry field
 *
 * <File description>
 * 
 */

#include <gtk/gtk.h>

#include <liberdm/display.h>
#include <libergtk/ergtk.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "gtkSettingItem.h"
#include "gtkSettingItemTextEntry.h"

#define LOGPRINTF ST_LOGPRINTF

enum
{
    ENTER_SELECTED,             // signals emitted when the user selects enter on the keyboard
    LAST_SIGNAL,
};

static gint settingitem_textentry_signals[LAST_SIGNAL] = { 0 };


// internally used routines
static void gtk_settingitem_textentry_class_init(GtkSettingItemTextEntryClass * klass);
static void gtk_settingitem_textentry_init(GtkSettingItemTextEntry * item);

// callback functions
static gboolean on_focus_out(GtkWidget* widget, GdkEventFocus* event, gpointer data);
static void     on_validate(GtkSettingItemTextEntry* item);
static gboolean on_entry_button_press(GtkWidget* widget, GdkEventButton* event, gpointer data);
static void     on_changed_stable(GtkEditable* widget, gpointer data);



// creates a new GtkSettingItem widget 
GtkWidget *gtk_settingitem_textentry_new()
{
    GtkSettingItemTextEntry *settingItem;

    settingItem = (GtkSettingItemTextEntry *) g_object_new(GTK_SETTINGITEM_TEXTENTRY_TYPE, NULL);

    return GTK_WIDGET(settingItem);
}

// returns type of this widget 
GType gtk_settingitem_textentry_get_type(void)
{
    static GType settingitem_textentry_type = 0;

    if (!settingitem_textentry_type)
    {
        static const GTypeInfo settingitem_textentry_info = {
            sizeof(GtkSettingItemTextEntryClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) gtk_settingitem_textentry_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(GtkSettingItemTextEntry),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) gtk_settingitem_textentry_init,
        };

        settingitem_textentry_type = g_type_register_static(GTK_TYPE_EVENT_BOX, "SettingItemTextEntry", &settingitem_textentry_info, 0);
    }
    return settingitem_textentry_type;
}

// init class
static void gtk_settingitem_textentry_class_init(GtkSettingItemTextEntryClass * klass)
{
    GtkObjectClass *object_class;

    object_class = (GtkObjectClass *) klass;

    // event to notify page update 

    settingitem_textentry_signals[ENTER_SELECTED] =
        g_signal_new("settingitem_textentry_enter",
                     G_OBJECT_CLASS_TYPE(object_class),
                     G_SIGNAL_RUN_LAST,
                     G_STRUCT_OFFSET(GtkSettingItemTextEntryClass, settingitem_textentry_enter),
                     NULL, NULL, gtk_marshal_VOID__STRING, G_TYPE_NONE, 1, G_TYPE_CHAR);
}


// create the composite widget - set the default values 
//
// object hierarchy:
//     item (GtkSettingItemTextEntry)
//       |-- item_background (event-box)
//             |-- alignment
//                   |-- vbox
//                         |-- item->label
//                         |-- hbox
//                               |-- item->entry
//                               |-- item->entry_info (label)
// 
static void gtk_settingitem_textentry_init(GtkSettingItemTextEntry * item)
{
    GtkWidget *item_background;
    GtkWidget *alignment;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *widget;

    gtk_widget_set_size_request(GTK_WIDGET(item), SETTINGITEMTEXT_REQ_WIDTH, SETTINGITEMTEXT_REQ_HEIGHT);

    // item_background
    item_background = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(item_background), "settingitem_te_background");
    gtk_container_add(GTK_CONTAINER(item), item_background);
    
    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              SETTINGITEMTEXT_BORDER_TOP,
                              SETTINGITEMTEXT_BORDER_BOTTOM,
                              SETTINGITEMTEXT_BORDER_LEFT,
                              SETTINGITEMTEXT_BORDER_RIGHT );
    gtk_container_add(GTK_CONTAINER(item_background), alignment);

    // vbox
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(vbox, -1, -1);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    
    // item->label
    widget = gtk_label_new("");
    gtk_widget_set_name(widget, "settingitem_te_label");
    gtk_widget_set_size_request(widget, SETTINGITEMTEXT_REQ_WIDTH, SETTINGITEMTEXT_LABEL_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.6);
    gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    item->label = widget;
    
    // hbox
    hbox = gtk_hbox_new(FALSE, SETTINGITEMTEXT_HBOX_SPACING);
    gtk_widget_set_size_request(hbox, -1, -1);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    
    // item->entry
    widget = ergtk_entry_new();
    gtk_widget_set_size_request(widget, SETTINGITEMTEXT_ENTRY_WIDTH, SETTINGITEMTEXT_ENTRY_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    item->entry = widget;
    //     signal callbacks
    g_signal_connect_after( G_OBJECT(widget), "focus-out-event",    G_CALLBACK(on_focus_out),          item);
    g_signal_connect(       G_OBJECT(widget), "button-press-event", G_CALLBACK(on_entry_button_press), item);
    g_signal_connect_after( G_OBJECT(widget), "changed-stable",     G_CALLBACK(on_changed_stable),     item);

    // item->entry_info
    widget = gtk_label_new("");
    gtk_widget_set_name(widget, "settingitem_te_label");
    gtk_widget_set_size_request(widget, -1, SETTINGITEMTEXT_LABEL_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.5);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    item->entry_info = widget;

    // display
    gtk_widget_show_all(GTK_WIDGET(item));

    // instance-private data
    item->callback_on_button_press = NULL;
    item->callback_on_changed      = NULL;
    item->callback_on_validate     = NULL;
}

// set/get functions
void gtk_settingitem_textentry_setlabel(GtkSettingItemTextEntry* item, const gchar* text)
{
    gtk_label_set_text(GTK_LABEL(item->label), text);
}

void gtk_settingitem_textentry_setinfo(GtkSettingItemTextEntry* item, const gchar* text)
{
    gtk_label_set_text(GTK_LABEL(item->entry_info), text);
}

void gtk_settingitem_textentry_settext(GtkSettingItemTextEntry* item, const gchar * text)
{
    gtk_entry_set_text(GTK_ENTRY(item->entry), text);
    on_validate(item);
}

const gchar* gtk_settingitem_textentry_gettext(GtkSettingItemTextEntry* item)
{
    return gtk_entry_get_text(GTK_ENTRY(item->entry));
}

void gtk_settingitem_textentry_setvisibility(GtkSettingItemTextEntry* item, gboolean visible)
{
    gtk_entry_set_visibility(GTK_ENTRY(item->entry), visible);
}

void gtk_settingitem_textentry_setentrywidth(GtkSettingItemTextEntry* item, guint width)
{
    gtk_widget_set_size_request(GTK_WIDGET(item->entry), width, SETTINGITEMTEXT_ENTRY_HEIGHT);
}

void gtk_settingitem_textentry_callback_on_changed(GtkSettingItemTextEntry* item, on_textentry_changed_t* callback)
{
    item->callback_on_changed = callback;
}

void gtk_settingitem_textentry_callback_on_button_press(GtkSettingItemTextEntry* item, on_textentry_button_press_t* callback)
{
    item->callback_on_button_press = callback;
}

void gtk_settingitem_textentry_callback_on_validate(GtkSettingItemTextEntry* item, on_textentry_validate_t* callback)
{
    item->callback_on_validate = callback;
}

void gtk_settingitem_textentry_grabfocus(GtkSettingItemTextEntry *item)
{
    ST_LOGPRINTF("entry");
    gtk_widget_grab_focus(item->entry);
}

// callback functions
static gboolean on_focus_out(GtkWidget* widget, GdkEventFocus* event, gpointer data)
{
    LOGPRINTF("entry");

    // validate & fix content
    on_validate( GTK_SETTINGITEM_TEXTENTRY(data) );

    return FALSE;
}

static void on_validate(GtkSettingItemTextEntry* item)
{
    LOGPRINTF("entry");

    gchar* new_text = NULL;

    // validate & fix content
    if (item->callback_on_validate)
    {
        item->callback_on_validate( GTK_WIDGET(item),
                                    gtk_entry_get_text(GTK_ENTRY(item->entry)),
                                    &new_text );
        if (new_text)
        {
            gtk_entry_set_text(GTK_ENTRY(item->entry), new_text);
            g_free(new_text);
            new_text = NULL;
        }
    }
}

static gboolean on_entry_button_press(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
    LOGPRINTF("entry");

    GtkSettingItemTextEntry* item = GTK_SETTINGITEM_TEXTENTRY(data);
    if (IS_GTK_SETTINGITEM_TEXTENTRY(item))
    {
        if (item->callback_on_button_press)
        {
            item->callback_on_button_press( GTK_WIDGET(item) );
        }
    }

    return FALSE;  // continue with GTK signal handling
}

static void on_changed_stable(GtkEditable* widget, gpointer data)
{
    LOGPRINTF("entry");

    GtkSettingItemTextEntry* item = GTK_SETTINGITEM_TEXTENTRY(data);
    g_return_if_fail(item != NULL);

    // report item text has changed
    if (item->callback_on_changed)
    {
        item->callback_on_changed( GTK_WIDGET(item),
                                   gtk_entry_get_text(GTK_ENTRY(item->entry)) );
    }
}

