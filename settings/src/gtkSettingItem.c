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
 * \file gtkSettingItem.c
 * \brief setup application - empty setup item containing a header label and some item
 *                            information text.
 * <File description>
 * 
 */

#include <gtk/gtk.h>

#include <liberdm/display.h>

#include "setupLog.h"
#include "background.h"
#include "gtkSettingItem.h"


// internally used routines
static void gtk_settingitem_class_init(GtkSettingItemClass * klass);
static void gtk_settingitem_init(GtkSettingItem * settingitem);

// creates a new GtkSettingItem widget 
GtkWidget *gtk_settingitem_new(gchar * headerlabel)
{
    GtkSettingItem *settingItem;

    settingItem = (GtkSettingItem *) g_object_new(GTK_SETTINGITEM_TYPE, NULL);

    // set string values and init data fields

    if (headerlabel)
    {
        ST_LOGPRINTF("headerlabel %s", headerlabel);
        gtk_label_set_text(GTK_LABEL(settingItem->headerlabel), headerlabel);
    }
    return GTK_WIDGET(settingItem);
}

// returns type of this widget 
GType gtk_settingitem_get_type(void)
{
    static GType settingitem_type = 0;

    if (!settingitem_type)
    {
        static const GTypeInfo settingitem_info = {
            sizeof(GtkSettingItemClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) gtk_settingitem_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(GtkSettingItem),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) gtk_settingitem_init,
        };

        settingitem_type = g_type_register_static(GTK_TYPE_EVENT_BOX, "SettingItem", &settingitem_info, 0);
    }
    return settingitem_type;
}

// init class
static void gtk_settingitem_class_init(GtkSettingItemClass * klass)
{
    // nothing needs to be done here 
}

// create the composite widget - set the default values 
//
// object hierarchy:
//     settingitem (GtkSettingItem)
//       |-- vbox
//             |-- header_background (event-box)
//             |     |-- settingitem->headerlabel
//             |-- settingitem->infolabel
//             |-- settingitem->clientArea (event-box)
// 
static void gtk_settingitem_init(GtkSettingItem * settingitem)
{
    GtkWidget *vbox;
    GtkWidget *header_background;

    gtk_widget_set_name(GTK_WIDGET(settingitem), "settingitem_background");
    
    
    // create (not homogenous) vbox to layout the header label and the rest of the setting item
    vbox = gtk_vbox_new(FALSE, SETTINGITEM_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(settingitem), vbox);

    header_background = gtk_event_box_new();
    gtk_widget_set_size_request(GTK_WIDGET(header_background), SETTING_TITLE_MIN_WIDTH, SETTING_TITLE_MIN_HEIGHT);
    gtk_widget_set_name(GTK_WIDGET(header_background), "settingitem_headerbackground");
    gtk_box_pack_start(GTK_BOX(vbox), header_background, FALSE, FALSE, 0);

    // create header label 
    settingitem->headerlabel = gtk_label_new("");
    gtk_widget_set_name(GTK_WIDGET(settingitem->headerlabel), "settingitem_header");
    gtk_misc_set_alignment(GTK_MISC(settingitem->headerlabel), 0.03, 0.5);
    gtk_label_set_ellipsize(GTK_LABEL(settingitem->headerlabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_justify(GTK_LABEL(settingitem->headerlabel), GTK_JUSTIFY_LEFT);
    gtk_container_add(GTK_CONTAINER(header_background), settingitem->headerlabel);

    // create info label
    settingitem->infolabel = gtk_label_new("");
    gtk_widget_set_size_request(GTK_WIDGET(settingitem->infolabel), TITLE_MIN_WIDTH, -1 );
    gtk_widget_set_name(GTK_WIDGET(settingitem->infolabel), "settingitem_infotext");
    gtk_misc_set_alignment(GTK_MISC(settingitem->infolabel), 0.03, 0.5);
    // gtk_label_set_ellipsize(GTK_LABEL(settingitem->infolabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_justify(GTK_LABEL(settingitem->infolabel), GTK_JUSTIFY_LEFT);
    gtk_label_set_single_line_mode(GTK_LABEL(settingitem->infolabel), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(settingitem->infolabel), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(settingitem->infolabel), FALSE, FALSE, 0);

    // create client area
    settingitem->clientArea = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(settingitem->clientArea), "settingitem_clientArea_background");
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(settingitem->clientArea), TRUE, TRUE, 0);

    gtk_widget_show_all(vbox);

    // only show this widget when there is info text available
    gtk_widget_hide(settingitem->infolabel);
}


// set the info/help text associated with this "setting item"
void gtk_settingitem_set_info_text(GtkSettingItem * settingitem, gchar * infotext)
{
    g_return_if_fail(IS_GTK_SETTINGITEM(settingitem));

    if (infotext)
    {
        ST_LOGPRINTF("infolabel %s", infotext);
        gtk_label_set_text(GTK_LABEL(settingitem->infolabel), infotext);
        gtk_widget_show(settingitem->infolabel);
    }
}


// set the header label text
void gtk_settingitem_set_header_text(GtkSettingItem * settingitem, gchar * headertext)
{
    g_return_if_fail(IS_GTK_SETTINGITEM(settingitem));

    if (headertext)
    {
        ST_LOGPRINTF("headerlabel %s", headertext);
        gtk_label_set_text(GTK_LABEL(settingitem->headerlabel), headertext);
    }
}


// Fill client Area of the the settings item with details
void gtk_settingitem_add_details(GtkSettingItem * settingitem, GtkWidget * details)
{
    g_return_if_fail(IS_GTK_SETTINGITEM(settingitem));

    if (details)
    {
        ST_LOGPRINTF("add 0x%x", (int)details);
        gtk_container_add(GTK_CONTAINER(settingitem->clientArea), details);
    }
}
