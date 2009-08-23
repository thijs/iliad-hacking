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
 * \file background.c
 * \brief connectionMgr - background creation/update
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <gtk/gtk.h>

#include "connectionMgrLog.h"
#include "connectionMgr.h"
#include "background.h"
#include "languages.h"

// global data
static GtkWidget *g_titleItem = NULL;
static GtkWidget *g_settingsArea = NULL;

// local functions
static GtkWidget *bg_screen_create(GtkWidget *parentWindow);

GtkWidget *bg_create(GtkWidget *parentWindow)
{
    GtkWidget *returnValue;

    returnValue = bg_screen_create(parentWindow);
    bg_set_text();

    return returnValue;
}

//
// Object hierarchy:
//      parentWindow
//        |-- background (event-box)
//             |-- screen_alignment
//                   |-- topLevelVBox (vbox)
//                         |-- title_hbox
//                         |     |-- title_padding (event box)
//                         |     |-- title_background (event box)
//                         |           |-- g_titleItem (label)
//                         |-- g_settingsArea (event box)
//
static GtkWidget *bg_screen_create(GtkWidget *parentWindow)
{
    GtkWidget *background;
    GtkWidget *topLevelVBox;
    GtkWidget *title_background;
    GtkWidget *title_hbox;
    GtkWidget *title_padding;
    GtkWidget *screen_alignment;

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(background), "screen_background");
    gtk_container_add(GTK_CONTAINER(parentWindow), background);

    // screen_alignment
    screen_alignment = gtk_alignment_new(0.0, 0.0, 1.0, 1.0);
    gtk_widget_set_name(GTK_WIDGET(screen_alignment), "screen_background");
    gtk_alignment_set_padding(GTK_ALIGNMENT(screen_alignment),
                              SCREEN_BORDER_TOP,
                              SCREEN_BORDER_BOTTOM,
                              1,
                              SCREEN_BORDER_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), screen_alignment);

    // topLevelVBox
    topLevelVBox = gtk_vbox_new(FALSE, SCREEN_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(screen_alignment), topLevelVBox);

    // title_hbox
    title_hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(topLevelVBox), title_hbox, FALSE, FALSE, 0);

    // title_padding
    title_padding = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(title_padding), "title_padding");
    gtk_widget_set_size_request(GTK_WIDGET(title_padding), 
                                SCREEN_BORDER_LEFT, -1);
    gtk_box_pack_start(GTK_BOX(title_hbox), title_padding, FALSE, FALSE, 0);

    // title_background
    title_background = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(title_background), "title_background");
    gtk_widget_set_size_request(GTK_WIDGET(title_background), 
                                TITLE_MIN_WIDTH, TITLE_MIN_HEIGHT);
    gtk_box_pack_start(GTK_BOX(title_hbox), title_background, 
                       FALSE, FALSE, 0);

    // g_titleItem: screen title text
    g_titleItem = gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(g_titleItem), 0.0, 0.5);
    gtk_misc_set_padding(GTK_MISC(g_titleItem), BACKGROUND_TITLE_OFFSET, 0);
    gtk_widget_set_name(GTK_WIDGET(g_titleItem), "title_item");
    gtk_container_add(GTK_CONTAINER(title_background), g_titleItem);

    // g_settingsArea: area to place remaining objects
    g_settingsArea = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(g_settingsArea), "settings_area");
    gtk_box_pack_end(GTK_BOX(topLevelVBox), g_settingsArea, TRUE, TRUE, 0);

    gtk_widget_show_all(background);

    return g_settingsArea;
}

void bg_set_text(void)
{
    char *title;

    title = main_get_screen_title();
    if (title == NULL)
    {
        title = _("iLiad network profiles");
    }
    
    CN_LOGPRINTF("Title [%s]\n", title);
    gtk_label_set_text(GTK_LABEL(g_titleItem), title);
}

// call it when change the cmgrScreen_t or editMode_t
void bg_display_title(void)
{
    cmgrScreen_t screen;
    editMode_t   mode;
    const gchar *title;

    screen = cmgr_get_screen_mode();
    switch (screen)
    {
        case cmgrScreenConnect_e:
            title = _("iLiad network connection");
            break;
        case cmgrScreenEdit_e:
            mode = edit_get_mode();
            if (mode == editModeConnectFillIn_e)
            {
                title = _("iLiad network configuration");
            }
            else
            {
                title = _("iLiad profile editor");
            }
            break;
        default:
            title = _("iLiad network profiles");
            break;
    }

    CN_LOGPRINTF("Title [%s]\n", title);
    gtk_label_set_text(GTK_LABEL(g_titleItem), title);
}

