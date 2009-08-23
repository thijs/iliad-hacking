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
 * \file gtkSettingItemTextEntry.c
 * \brief connectionMgr - setup item containing one text entry field
 *
 * <File description>
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

//#include <string.h>

#include <gtk/gtk.h>

#include <libergtk/ergtk.h>
#include <liberdm/display.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "gtkSettingItem.h"
#include "gtkSettingItemTextEntry.h"

#define LOGPRINTF CN_LOGPRINTF

enum
{
    // signals emitted when the user selects enter on the keyboard
    ENTER_SELECTED, 
    LAST_SIGNAL,
};

static gint settingitem_textentry_signals[LAST_SIGNAL] = { 0 };


// internally used routines
static void gtk_settingitem_textentry_class_init(GtkSettingItemTextEntryClass * klass);
static void gtk_settingitem_textentry_init(GtkSettingItemTextEntry * item);
static void send_button_press(GtkSettingItemTextEntry * item);

// callback functions
static void on_changed(GtkEntry* widget, gpointer data);
static void on_button_toggled(GtkToggleButton* button, gpointer data);


// creates a new GtkSettingItem widget 
GtkWidget *gtk_settingitem_textentry_new(gboolean with_button)
{
    GtkSettingItemTextEntry *settingItem;

    settingItem = (GtkSettingItemTextEntry *) g_object_new(GTK_SETTINGITEM_TEXTENTRY_TYPE, 
                                                           NULL);

    if (!with_button)
    {
        // remove button object
        gtk_widget_destroy(settingItem->button);
        settingItem->button = NULL;
    }

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

        settingitem_textentry_type = g_type_register_static(GTK_TYPE_EVENT_BOX, 
                                            "SettingItemTextEntry", 
                                            &settingitem_textentry_info, 
                                            0);
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
                     G_STRUCT_OFFSET(GtkSettingItemTextEntryClass, 
                                     settingitem_textentry_enter),
                     NULL, 
                     NULL, 
                     gtk_marshal_VOID__STRING, 
                     G_TYPE_NONE, 
                     1, 
                     G_TYPE_CHAR);
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
//                               |-- item->button
// 
static void gtk_settingitem_textentry_init(GtkSettingItemTextEntry * item)
{
    GtkWidget *item_background;
    GtkWidget *alignment;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *widget;

    gtk_widget_set_size_request(GTK_WIDGET(item), 
            SETTINGITEMTEXT_REQ_WIDTH, SETTINGITEMTEXT_REQ_HEIGHT);

    // item_background
    item_background = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(item_background), 
                        "settingitem_te_background");
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
    gtk_widget_set_size_request(widget, 
            SETTINGITEMTEXT_REQ_WIDTH, SETTINGITEMTEXT_LABEL_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.6);
    gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    item->label = widget;
    
    // hbox
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(hbox, -1, -1);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    
    // item->entry
    widget = ergtk_entry_new();
    gtk_widget_set_size_request(widget, 
            SETTINGITEMTEXT_ENTRY_WIDTH, SETTINGITEMTEXT_ENTRY_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    item->entry = widget;
    //     signal callbacks
    g_signal_connect_after(G_OBJECT(widget), "changed-stable", 
                           G_CALLBACK(on_changed), item);

    // item->button
    widget = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(widget, 
            SETTINGITEMTEXT_BUTTON_WIDTH_SMALL, -1);
    gtk_box_pack_start(GTK_BOX(hbox), widget, 
            FALSE, FALSE, SETTINGITEMTEXT_BUTTON_PADDING);
    item->button = widget;
    //     signal callbacks
    g_signal_connect_after(G_OBJECT(widget), "toggled", 
            G_CALLBACK(on_button_toggled), item);

    // display
    gtk_widget_show_all(GTK_WIDGET(item));

    // instance-private data
    item->callback_on_changed = NULL;
}


void gtk_settingitem_textentry_grabfocus(GtkSettingItemTextEntry *item)
{
    LOGPRINTF("entry");
    gtk_widget_grab_focus(item->entry);
}


// set/get functions
void gtk_settingitem_textentry_setlabel(GtkSettingItemTextEntry* item, 
                                        const gchar* text)
{
    gtk_label_set_text(GTK_LABEL(item->label), text);
}

void gtk_settingitem_textentry_settext(GtkSettingItemTextEntry* item, 
                                       const gchar * text)
{
    gtk_entry_set_text(GTK_ENTRY(item->entry), text);
}

const gchar* gtk_settingitem_textentry_gettext(GtkSettingItemTextEntry* item)
{
    return gtk_entry_get_text(GTK_ENTRY(item->entry));
}

void gtk_settingitem_textentry_setvisibility(GtkSettingItemTextEntry* item, 
                                             gboolean visible)
{
    gtk_entry_set_visibility(GTK_ENTRY(item->entry), visible);
}

void gtk_settingitem_textentry_setsensitive(GtkSettingItemTextEntry* item, 
                                            gboolean sensitive)
{
    gtk_widget_set_sensitive(item->entry, sensitive);
}

void gtk_settingitem_textentry_show_button(GtkSettingItemTextEntry* item, 
                                           const gboolean show)
{
    g_return_if_fail(item->button != NULL);

    if (show)
    {
        gtk_widget_show(item->button);
    }
    else
    {
        gtk_widget_hide(item->button);
    }
}

void gtk_settingitem_textentry_set_buttontext(GtkSettingItemTextEntry*item,
                                              const gchar* text)
{
    g_return_if_fail(item->button != NULL);
    gtk_button_set_label(GTK_BUTTON(item->button), text);
}

void gtk_settingitem_textentry_set_buttonsize(GtkSettingItemTextEntry*item,
                                              const gboolean large_button)
{
    g_return_if_fail(item->button != NULL);

    if (large_button)
    {
        gtk_widget_set_size_request(item->button, 
                SETTINGITEMTEXT_BUTTON_WIDTH_LARGE, -1);
    }
    else
    {
        gtk_widget_set_size_request(item->button, 
                SETTINGITEMTEXT_BUTTON_WIDTH_SMALL, -1);
    }
}

void gtk_settingitem_textentry_set_buttonstate(GtkSettingItemTextEntry*item,
                                               const gboolean active)
{
    g_return_if_fail(item->button != NULL);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item->button), active);
}

const gboolean gtk_settingitem_textentry_get_buttonactive(GtkSettingItemTextEntry* item)
{
    g_return_val_if_fail((item->button != NULL), FALSE);
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(item->button));
}


// user callback functions
void gtk_settingitem_textentry_callback_on_buttonclicked(GtkSettingItemTextEntry* item, 
        on_textentry_buttonclicked_t* callback)
{
    item->callback_on_buttonclicked = callback;
}

void gtk_settingitem_textentry_callback_on_changed(GtkSettingItemTextEntry* item, 
        on_textentry_changed_t* callback)
{
    item->callback_on_changed = callback;
}

static void send_button_press(GtkSettingItemTextEntry* item)
{
    LOGPRINTF("entry");
    
    // simulate button-press-event
    static GdkEventButton event;
    event.type = GDK_BUTTON_PRESS;
    gboolean result = FALSE;
    g_signal_emit_by_name(item, "button-press-event", GTK_WIDGET(item), 
            &event, NULL, &result);
}


// callback functions
static void on_changed(GtkEntry* widget, gpointer data)
{
    LOGPRINTF("entry");

    GtkSettingItemTextEntry* item = GTK_SETTINGITEM_TEXTENTRY(data);
    g_return_if_fail(item != NULL);

    // item text is stable: inform application and update screen
    send_button_press(item);
    if (item->callback_on_changed)
    {
        item->callback_on_changed( GTK_WIDGET(data),
                           gtk_entry_get_text(GTK_ENTRY(item->entry)) );
    }
}

static void on_button_toggled(GtkToggleButton* button, gpointer data)
{
    LOGPRINTF("entry");

    GtkSettingItemTextEntry* item = GTK_SETTINGITEM_TEXTENTRY(data);
    g_return_if_fail(item != NULL);

    gboolean active;
    
    // inform user that button has changed
    if (item->callback_on_buttonclicked)
    {
        active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));
        item->callback_on_buttonclicked(GTK_WIDGET(item), active);
    }
}

