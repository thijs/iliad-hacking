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
 * \file gtkInfoItem.c
 * \brief connectionMgr - setup item showing additional help or info text`
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <glib.h>
#include <gtk/gtk.h>

#include <liberdm/display.h>
#include <libergtk/ergtk.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "gtkSettingItem.h"
#include "gtkInfoItem.h"

#define LOGPRINTF CN_LOGPRINTF


// internally used routines
static void gtk_infoitem_class_init(GtkInfoItemClass * klass);
static void gtk_infoitem_init(GtkInfoItem * item);

// signal handlers
static void on_button_toggled(GtkToggleButton* button, gpointer data);


// creates a new GtkInfoItem widget 
GtkWidget *gtk_infoitem_new(gboolean with_button)
{
    GtkInfoItem* item;

    item = (GtkInfoItem*) g_object_new(GTK_INFOITEM_TYPE, NULL);

    if (!with_button)
    {
        gtk_widget_destroy(item->button);
        item->button = NULL;

        gtk_widget_set_size_request( item->label,
                             INFOITEM_TEXT_WIDTH + INFOITEM_BUTTON_WIDTH,
                             INFOITEM_TEXT_HEIGHT );
    }

    return GTK_WIDGET(item);
}

// returns type of this widget 
GType gtk_infoitem_get_type(void)
{
    static GType infoitem_type = 0;

    if (!infoitem_type)
    {
        static const GTypeInfo infoitem_info = {
            sizeof(GtkInfoItemClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) gtk_infoitem_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(GtkInfoItem),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) gtk_infoitem_init,
        };

        infoitem_type = g_type_register_static(GTK_TYPE_EVENT_BOX, 
                                               "InfoItem", 
                                               &infoitem_info, 
                                               0);
    }
    return infoitem_type;
}

// init class
static void gtk_infoitem_class_init(GtkInfoItemClass * klass)
{
    GtkObjectClass *object_class;

    object_class = (GtkObjectClass *) klass;
}


// create the composite widget - set the default values 
//
// object hierarchy:
//     item (GtkInfoItem)
//       |-- background (event box)
//             |-- alignment
//                   |-- hbox
//                         |-- item->icon (event box)
//                         |-- item->label
//                         |-- alignment
//                         |     |-- item->button
// 
static void gtk_infoitem_init(GtkInfoItem * item)
{
    GtkWidget *background;
    GtkWidget *alignment;
    GtkWidget *hbox;
    GtkWidget *widget;

    gtk_widget_set_size_request(GTK_WIDGET(item), 
            INFOITEM_REQ_WIDTH, INFOITEM_REQ_HEIGHT);


    // background
    widget = gtk_event_box_new();
    gtk_widget_set_name(widget, "infoitem_background");
    gtk_container_add(GTK_CONTAINER(item), widget);
    background = widget;

    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              INFOITEM_BORDER_TOP,
                              INFOITEM_BORDER_BOTTOM,
                              INFOITEM_BORDER_LEFT,
                              INFOITEM_BORDER_RIGHT );
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // hbox
    widget = gtk_hbox_new(FALSE, INFOITEM_HBOX_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), widget);
    hbox = widget;

    //     item->icon
    widget = gtk_event_box_new();
    gtk_widget_set_name(widget, "infoitem_icon");
    gtk_widget_set_size_request(widget, 
            INFOITEM_ICON_IMG_WIDTH, INFOITEM_ICON_IMG_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    item->icon = widget;

    //     item->label
    widget = gtk_label_new("");
    gtk_widget_set_name(widget, "infoitem_text");
    gtk_widget_set_size_request(widget, 
            INFOITEM_TEXT_WIDTH, INFOITEM_TEXT_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.0);
    gtk_label_set_single_line_mode(GTK_LABEL(widget), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(widget), TRUE);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    item->label = widget;

    //     alignment
    widget = gtk_alignment_new(1.0, 1.0, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    alignment = widget;

    //         item->button
    widget = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(widget, 
            INFOITEM_BUTTON_WIDTH, INFOITEM_BUTTON_HEIGHT);
    gtk_container_add(GTK_CONTAINER(alignment), widget);
    gtk_widget_show(widget);
    item->button = widget;
    //     signal callbacks
    g_signal_connect_after(G_OBJECT(widget), "toggled", 
            G_CALLBACK(on_button_toggled), item);

    // display
    gtk_widget_show_all(GTK_WIDGET(item));
}


// set/get functions
void gtk_infoitem_set_icon(GtkInfoItem *item, infoItemIcon_t icon)
{
    GtkWidget *widget;
    const char * name;
    
    g_return_if_fail((item != NULL) && IS_GTK_INFOITEM(item));
    
    widget = item->icon;
    g_return_if_fail(widget != NULL);

    switch (icon)
    {
        case iconInfo_e:
            name = "infoitem_icon";
            break;
        case iconOk_e:
            name = "infoitem_ok_icon";
            break;
        case iconFailed_e:
            name = "infoitem_failed_icon";
            break;
        default:
            name = "infoitem_icon";
            break;
    }

    gtk_widget_set_name(widget, name);
}

void gtk_infoitem_set_text(GtkInfoItem* item, const gchar* text)
{
    gtk_label_set_text(GTK_LABEL(item->label), text);
    gdk_flush();  // wait for label to copy text
}

void gtk_infoitem_set_buttontext(GtkInfoItem* item, const gchar* text)
{
    g_return_if_fail(item->button != NULL);
    gtk_button_set_label(GTK_BUTTON(item->button), text);
}

void gtk_infoitem_set_buttonstate(GtkInfoItem* item, const gboolean active)
{
    g_return_if_fail(item->button != NULL);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item->button), active);
}


// user callback functions
void gtk_infoitem_callback_on_buttonclicked(GtkInfoItem* item, 
                            on_infoitem_buttonclicked_t* callback)
{
    item->callback_on_buttonclicked = callback;
}


// local callback functions
static void on_button_toggled(GtkToggleButton* button, gpointer data)
{
    LOGPRINTF("entry");

    GtkInfoItem* item = GTK_INFOITEM(data);
    g_return_if_fail(item != NULL);

    gboolean active;
    
    // inform user that button has changed
    if (item->callback_on_buttonclicked)
    {
        active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));
        item->callback_on_buttonclicked(GTK_WIDGET(item), active);
    }
}

