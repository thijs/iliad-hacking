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
 * \file background.c
 * \brief setup application - E-reader settings application background creation/update                

 * <File description>
 * 
 */

#include <string.h>
#include <strings.h>

#include <gtk/gtk.h>

#include "setupLog.h"
#include "background.h"
#include "system.h"
#include "languages.h"

//global data
static GtkWidget *g_titleItem;
static GtkWidget *g_settingsArea;


// Object hierarchy:
//      parentWindow
//        |-- background (event-box)
//             |-- alignment
//                   |-- topLevelVBox (vbox)
//                         |-- title_background (event box)
//                         |     |-- alignment
//                         |         |-- g_titleItem (label)
//                         |-- g_settingsArea (event box)
//
GtkWidget *bg_create(GtkWidget * parentWindow)
{
    GtkWidget *background;
    GtkWidget *topLevelVBox;
    GtkWidget *title_background;
    GtkWidget *alignment;
    
    // background: allows to set a background color or pixmap
    background = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(background), "screen_background");
    gtk_container_add(GTK_CONTAINER(parentWindow), background);

    // alignement: defines screen area available for objects,
    //             also allows user to resize window, just in case
    alignment = gtk_alignment_new(0.0, 0, 1, 1);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), SETTINGS_BOX_BORDER, 1, SETTINGS_BOX_BORDER, SETTINGS_BOX_BORDER);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // topLevelVBox: orders objects top to bottom, not homogeneous, zero spacing
    topLevelVBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(alignment), topLevelVBox);

    // title_background
    title_background = gtk_event_box_new();
    gtk_widget_set_size_request(GTK_WIDGET(title_background), TITLE_MIN_WIDTH, TITLE_MIN_HEIGHT);
    gtk_widget_set_name(GTK_WIDGET(title_background), "title_background");
    gtk_box_pack_start(GTK_BOX(topLevelVBox), title_background, FALSE, FALSE, 0);

    // alignment
    alignment = gtk_alignment_new(0.0, 0.5, 1, 1);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), 0, 0, TITLE_BORDER_LEFT, TITLE_BORDER_RIGHT);
    gtk_container_add(GTK_CONTAINER(title_background), alignment);

    // g_titleItem: screen title text
    g_titleItem = gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(g_titleItem), 0, 0.5);
    gtk_widget_set_name(GTK_WIDGET(g_titleItem), "title_item");
    gtk_container_add(GTK_CONTAINER(alignment), g_titleItem);

    // g_settingsArea: area to place remaining objects
    g_settingsArea = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(g_settingsArea), "settings_area");
    gtk_box_pack_end(GTK_BOX(topLevelVBox), g_settingsArea, TRUE, TRUE, SETTINGS_ITEM_PADDING);

    gtk_widget_show_all(background);

    return g_settingsArea;
}

void bg_set_text(void)
{
    gtk_label_set_text(GTK_LABEL(g_titleItem), _("iLiad Settings"));
}

