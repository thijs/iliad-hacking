/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file lister.c
 * \brief content lister - eReader content Lister page creation                  
 * 
 * <File description>
 *  The eReader content Lister page contains a predefined number
 *  of gtkContentListItems. Which are created and updated with the following API
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
#include <config.h>

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include <gtk/gtk.h>

#include <libermanifest/ermanifest.h>
#include <liberdm/display.h>

#include "contentListerLog.h"
#include "gtkContentListItem.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "displayUpdate.h"
#include "icons.h"
#include "programManager.h"
#include "languages.h"

#define MAX_INT_LEN     50  // integer string buffer length

static lsLister_t g_lister;

// internally used routines
static void    ls_listItem_press_event(GtkWidget * widget, GdkEventButton * event, gpointer user_data);

//callback routines
static gboolean ls_lister_expose_event(GtkWidget * widget, GdkEventExpose * event, ContentLister * theContentLister);

static void formatSize(const int bytes, char * str, const int length);
static void formatFree(const int kbytes, char * str, const int length);


lsLister_t *lsInit(GtkWidget * topLevelWidget, gpointer data)
{
    GtkWidget *alignment;
    GtkWidget *listVBox;
    GtkWidget *listItem;
    GtkWidget *titleBackground;
    GtkWidget *titleContainer;

    int     index;

    // object hierarchy:
    //     topLevelWidget
    //       |-- listVBox
    //             |
    listVBox = gtk_vbox_new(FALSE, LIST_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(topLevelWidget), listVBox);
    gtk_widget_show(listVBox);
    //             |
    //             |-- alignment
    //             |     |-- titleBackground
    //             |           |
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), 0, 0, LISTER_BOX_BORDER, 0);
    gtk_box_pack_start(GTK_BOX(listVBox), alignment, FALSE, FALSE, 0);
    gtk_widget_show(alignment);
    //
    titleBackground = gtk_event_box_new();
    gtk_widget_set_size_request(GTK_WIDGET(titleBackground), TITLE_MIN_WIDTH, TITLE_MIN_HEIGHT);
    gtk_widget_set_name(GTK_WIDGET(titleBackground), "title_background");
    gtk_container_add(GTK_CONTAINER(alignment), titleBackground);
    g_signal_connect(G_OBJECT(titleBackground), "expose-event", G_CALLBACK(ls_lister_expose_event), NULL);
    gtk_widget_show(titleBackground);
    //             |           |
    //             |           |-- titleContainer (GtkHBox)
    //             |                 |-- g_lister.titleItem (GtkLabel)
    //             |                 |-- g_lister.titleLocation (GtkLabel)
    //             |
    titleContainer = gtk_hbox_new(FALSE, 10);
    gtk_widget_set_size_request(GTK_WIDGET(titleContainer), TITLE_MIN_WIDTH, TITLE_MIN_HEIGHT);    
    gtk_container_add(GTK_CONTAINER(titleBackground), titleContainer);
    gtk_widget_show(titleContainer);
    //
    g_lister.titleItem = gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(g_lister.titleItem), 0, 0.5);
    gtk_widget_set_name(GTK_WIDGET(g_lister.titleItem), "title_item");
    gtk_box_pack_start (GTK_BOX (titleContainer), g_lister.titleItem, FALSE, FALSE, 10);
    gtk_widget_show(g_lister.titleItem);
    //
    g_lister.titleLocation = gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(g_lister.titleLocation), 1.0, 0.5);
    gtk_label_set_ellipsize(GTK_LABEL(g_lister.titleLocation), PANGO_ELLIPSIZE_START);
    gtk_widget_set_name(GTK_WIDGET(g_lister.titleLocation), "title_location");
    gtk_box_pack_start (GTK_BOX (titleContainer), g_lister.titleLocation, TRUE, TRUE, 10);
    gtk_widget_show(g_lister.titleLocation);
    //             |
    //             |-- g_lister.listItems[0] (GtkContentListItem)
    //             |--   ...
    //             |-- g_lister.listItems[..] (GtkContentListItem)
    //
    for (index = 0; index < MAX_ITEMS_ON_ONE_PAGE; index++)
    {
        listItem = gtk_content_list_item_new(index);
        gtk_content_list_item_show_cursor(GTK_CONTENT_LIST_ITEM(listItem), FALSE);
        gtk_box_pack_start(GTK_BOX(listVBox), listItem, FALSE, FALSE, 0);
        g_signal_connect(G_OBJECT(listItem), "button_press_event", G_CALLBACK(ls_listItem_press_event), data);

        g_lister.listItems[index] = listItem;
    }

    return &g_lister;
}

GtkWidget * lsGetListerItem(lsLister_t * lister, int index)
{
    g_assert(lister != NULL);
    g_assert(index >= 0  &&  index < MAX_ITEMS_ON_ONE_PAGE);

    return lister->listItems[index];
}


void lsUpdatePage(lsLister_t * lister, clDisplayItem_t * items, 
                int count, int focus, gchar * title, const gchar *path)
{
    int       index = 0;
    GtkWidget *listerItem;
    char      *location;
    char      str[MAX_INT_LEN];

    //update the title
    if (title)
    {
        CL_LISTERPRINTF("set title %s", title);
        gtk_label_set_text(GTK_LABEL(lister->titleItem), title);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(lister->titleItem), "");
    }
    
    if (path)
    {
        // update the location information
        gtk_label_set_text(GTK_LABEL(lister->titleLocation), path);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(lister->titleLocation), "");
    }


    // update and display used lister items, hide the others
    for (index = 0 ; index < MAX_ITEMS_ON_ONE_PAGE ; index++)
    {
        listerItem = lister->listItems[index];

        if (index < count)
        {
            // update and display used lister items   
            CL_LISTERPRINTF("update and display listerItem %p - index %d", listerItem, index);
            gtk_content_list_item_set_title(GTK_CONTENT_LIST_ITEM(listerItem), items[index].szTitle);
            gtk_content_list_item_set_subtitle(GTK_CONTENT_LIST_ITEM(listerItem), items[index].szSubTitle);
            gtk_content_list_item_set_description(GTK_CONTENT_LIST_ITEM(listerItem), items[index].szDescription);

            str[0] = '\0';
            if (ERCL_INVALID_SIZE != items[index].size)
            {
                if (items[index].fit == mdsFitStorage)
                {
                    formatFree(items[index].size, str, MAX_INT_LEN);
                }
                else
                {
                    formatSize(items[index].size, str, MAX_INT_LEN);
                }
            }
            gtk_content_list_item_set_information(GTK_CONTENT_LIST_ITEM(listerItem), str);

            // set icon
            if (ctrl_location_is_outbox(path))
            {
                CL_LISTERPRINTF("clIconOutboxItem");
                gtk_content_list_item_set_icon_thumb(GTK_CONTENT_LIST_ITEM(listerItem), clOutboxIcon);
            }
            else if (items[index].iconID == clIconUrlDefined)
            {
                CL_LISTERPRINTF("clIconUrlDefined");
                gtk_content_list_item_set_thumb(GTK_CONTENT_LIST_ITEM(listerItem), items[index].clIconURL);
            }
            else if (   (items[index].iconID == clUnknownIcon)
                     && ((location = pm_getIcon(items[index].szFileExt)) != NULL) )
            {
                CL_LISTERPRINTF("known extension => location");
                gtk_content_list_item_set_thumb(GTK_CONTENT_LIST_ITEM(listerItem), location);
            }
            else
            {
                gtk_content_list_item_set_icon_thumb(GTK_CONTENT_LIST_ITEM(listerItem), items[index].iconID);
            }

            // set cursor
            if (index == focus)
            {
                CL_LISTERPRINTF("focus widget %d", focus);
                //temp do update here until history story becomes clear in future releases
                gtk_content_list_item_show_cursor(GTK_CONTENT_LIST_ITEM(listerItem), TRUE);
            }
            else
            {
                gtk_content_list_item_show_cursor(GTK_CONTENT_LIST_ITEM(listerItem), FALSE);
            }

            gtk_widget_show(listerItem);
        }
        else
        {
            CL_LISTERPRINTF("hide and clear listerItem %p - index %d", listerItem, index);
            gtk_content_list_item_set_title(GTK_CONTENT_LIST_ITEM(listerItem), "");
            gtk_content_list_item_set_subtitle(GTK_CONTENT_LIST_ITEM(listerItem), "");
            gtk_content_list_item_set_description(GTK_CONTENT_LIST_ITEM(listerItem), "");
            gtk_content_list_item_set_information(GTK_CONTENT_LIST_ITEM(listerItem), "");        
            gtk_content_list_item_hide_thumb(GTK_CONTENT_LIST_ITEM(listerItem));

            gtk_widget_hide(listerItem);
        }
    }

    CL_SCREENPRINTF("");
    display_update_increase_level(LISTER_EXPOSE_LEVEL);
}

// no screen update needed for ereader 
void ls_listItem_press_event(GtkWidget * widget, GdkEventButton * event, gpointer data)
{
    GtkContentListItem *listItem = (GtkContentListItem *) widget;

    CL_LISTERPRINTF("index %d", listItem->index);

    ctrl_listItem_clicked(listItem->index, data);
}


gboolean ls_lister_expose_event(GtkWidget * widget, GdkEventExpose * event, ContentLister * theContentLister)
{
    CL_SCREENPRINTF("entry");
    display_update_request_screen_refresh(LISTER_EXPOSE_LEVEL);
    return FALSE;
}


void formatSize(const int bytes, char * str, const int length)
{
    const int   MB = 1024 * 1024;
    const int   KB = 1024;
    const char *title = _("Size");

    int ret = 0;
    if (bytes >= MB)
    {
        ret = bytes / MB; 
        snprintf(str, length, "%s:\n%d.%d %s", title, ret, (bytes - ret * MB) * 10 / MB, _("MB"));
    }
    else if (bytes >= KB)
    {
        ret = bytes / KB;
        snprintf(str, length, "%s:\n%d.%d %s", title, ret, (bytes - ret * KB) * 10 / KB, _("KB"));
    }    
    else
    {
        snprintf(str, length, "%s:\n%d %s", title, bytes, _("Bytes"));
    }
}


void formatFree(const int kbytes, char * str, const int length)
{
    const int   KB = 1024;
    const char *title = _("Free");

    int ret = kbytes / KB; 
    snprintf(str, length, "%s:\n%d %s", title, ret, _("MB"));
}

