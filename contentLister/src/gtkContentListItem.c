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
 * \file gtkContentListItem.c
 * \brief content lister - GTK widget for the eReader content Lister item                  
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#include <string.h>

#include <liberdm/display.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "gtkContentListItem.h"
#include "icons.h"
// some size values 
#include "lister.h"

#define MAX_CHARACTERS  52  // max characters per line

// internally used routines
static void gtk_content_list_item_class_init(GtkContentListItemClass * klass);
static void gtk_content_list_item_init(GtkContentListItem * list_item);
static int  get_string_lines(const gchar * string);

// creates a new GtkContentListItem widget 
GtkWidget *gtk_content_list_item_new(index)
{
    GtkContentListItem *listItem;

    listItem = (GtkContentListItem *) g_object_new(GTK_CONTENT_LIST_ITEM_TYPE, NULL);

    CL_LOGPRINTF("listItem [%p]", listItem);
    listItem->index = index;

    return GTK_WIDGET(listItem);
}

// returns type of this widget 
GType gtk_content_list_item_get_type(void)
{
    static GType content_list_item_type = 0;

    if (!content_list_item_type)
    {
        static const GTypeInfo content_list_item_info = {
            sizeof(GtkContentListItemClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) gtk_content_list_item_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(GtkContentListItem),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) gtk_content_list_item_init,
        };

        content_list_item_type = g_type_register_static(GTK_TYPE_HBOX, "ListItem", &content_list_item_info, 0);
    }
    return content_list_item_type;
}

// init class
static void gtk_content_list_item_class_init(GtkContentListItemClass * klass)
{
    // nothing needs to be done here 
}

// create the composite widget - set the default values 
//
// object hierarchy:
//     list_item (GtkContentListItem, spacing=HBOX_TEXT_THUMB_SPACING=5)
//       |-- list_item->cursorspace (GtkEventBox, width=CURSOR_BOX_MIN_WIDTH=46, height=LISTER_ITEM_HEIGHT=115)
//       |     |-- list_item->cursorbox (GtkEventBox)
//       |-- list_item->thumbbox (GtkEventBox, width=THUMB_BOX_MIN_WIDTH=91, height=LISTER_ITEM_HEIGHT=115)
//       |     |-- list_item->image (GtkImage)
//       |-- list_item->textbox (GtkEventBox, width=TEXT_BOX_MIN_WIDTH=570, height=LISTER_ITEM_HEIGHT=115)
//       |     |-- alignment
//       |           |-- list_item->vbox (GtkVBox change to table)
//       |                 |-- list_item->title (GtkLabel)
//       |                 |-- list_item->subtitle (GtkLabel)
//       |                 |-- list_item->description (GtkLabel)
//       |                 |-- list_item->information (GtkLabel)
//       |-- list_item->pixmap = NULL
// 
static void gtk_content_list_item_init(GtkContentListItem * list_item)
{
    GtkWidget   *alignment;

    // not homogenous => no expand of the widgets added to the box - CONTENT_HBOX_SPACE pixels spacing
    gtk_box_set_homogeneous(GTK_BOX(list_item), FALSE);
    gtk_box_set_spacing(GTK_BOX(list_item), HBOX_TEXT_THUMB_SPACING);

    list_item->cursorspace = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(list_item->cursorspace), "listCursorItem_background");
    gtk_widget_set_size_request(GTK_WIDGET(list_item->cursorspace), CURSOR_BOX_MIN_WIDTH, LISTER_ITEM_HEIGHT);
    gtk_box_pack_start(GTK_BOX(list_item), list_item->cursorspace, FALSE, FALSE, 0);
    list_item->cursorbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(list_item->cursorbox), "listCursorItem");
    gtk_container_add(GTK_CONTAINER(list_item->cursorspace), list_item->cursorbox);

    list_item->thumbbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(list_item->thumbbox), "listThumbItem_background");
    gtk_widget_set_size_request(GTK_WIDGET(list_item->thumbbox), THUMB_BOX_MIN_WIDTH, LISTER_ITEM_HEIGHT);
    gtk_box_pack_start(GTK_BOX(list_item), list_item->thumbbox, FALSE, FALSE, 0);

    list_item->textbox = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(list_item->textbox), "listTextItem_background");
    gtk_widget_set_size_request(GTK_WIDGET(list_item->textbox), TEXT_BOX_MIN_WIDTH, LISTER_ITEM_HEIGHT);
    gtk_box_pack_start(GTK_BOX(list_item), list_item->textbox, FALSE, FALSE, 0);

    list_item->image = gtk_image_new();
    gtk_container_add(GTK_CONTAINER(list_item->thumbbox), list_item->image);
    list_item->pixmap = NULL;

    // content string data 
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_widget_set_size_request(GTK_WIDGET(alignment), TEXT_BOX_MIN_WIDTH, LISTER_ITEM_HEIGHT);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), VBOX_TEXT_BORDER, VBOX_TEXT_BORDER, HBOX_TEXT_BORDER, HBOX_TEXT_BORDER);
    gtk_container_add(GTK_CONTAINER(list_item->textbox), alignment);

    // file bar layout
    // table 6x6
    list_item->table = gtk_table_new (6, 6, TRUE);
    // list_item->vbox = gtk_vbox_new(FALSE, VBOX_TEXT_TEXT_SPACING);
    // gtk_widget_set_size_request(GTK_WIDGET(list_item->vbox), TEXT_BOX_MIN_WIDTH, (-1));
    // gtk_container_add(GTK_CONTAINER(alignment), list_item->vbox);
    gtk_widget_set_size_request(GTK_WIDGET(list_item->table), TEXT_BOX_MIN_WIDTH, (-1));
    gtk_table_set_col_spacings((GtkTable *)list_item->table, 10);
    gtk_container_add(GTK_CONTAINER(alignment), list_item->table);
    

    list_item->title = gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(list_item->title), 0.0, 0.0);
    //gtk_label_set_ellipsize(GTK_LABEL(list_item->title), PANGO_ELLIPSIZE_END);
    //gtk_label_set_justify(GTK_LABEL(list_item->title), GTK_JUSTIFY_LEFT);
    gtk_widget_set_name(GTK_WIDGET(list_item->title), "list_item_title");
    // gtk_box_pack_start(GTK_BOX(list_item->vbox), list_item->title, FALSE, FALSE, 0);
    gtk_table_attach_defaults(GTK_TABLE(list_item->table), list_item->title, 0, 6, 0, 2);

    list_item->subtitle = gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(list_item->subtitle), 0.0, 0.0);
    //gtk_label_set_ellipsize(GTK_LABEL(list_item->title), PANGO_ELLIPSIZE_END);
    //gtk_label_set_justify(GTK_LABEL(list_item->subtitle), GTK_JUSTIFY_LEFT);
    gtk_widget_set_name(GTK_WIDGET(list_item->subtitle), "list_item_subtitle");
    // gtk_box_pack_start(GTK_BOX(list_item->vbox), list_item->subtitle, FALSE, FALSE, 0);
    gtk_table_attach_defaults(GTK_TABLE(list_item->table), list_item->subtitle, 0, 6, 2, 3 );    

    list_item->description = gtk_label_new("");
    gtk_widget_set_name(GTK_WIDGET(list_item->description), "list_item_description");
    gtk_misc_set_alignment(GTK_MISC(list_item->description), 0.0, 0.0);
    //gtk_label_set_ellipsize(GTK_LABEL(list_item->description), PANGO_ELLIPSIZE_END);
    gtk_widget_set_size_request(GTK_WIDGET(list_item->description), TEXT_BOX_MIN_WIDTH * 4 / 5, -1 );
    gtk_label_set_single_line_mode(GTK_LABEL(list_item->description), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(list_item->description), TRUE);
    gtk_table_attach_defaults(GTK_TABLE(list_item->table), list_item->description, 0, 5, 3, 6);    
    
    list_item->information = gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(list_item->information), 0.0, 0.0);
    //gtk_label_set_ellipsize(GTK_LABEL(list_item->information), PANGO_ELLIPSIZE_END);
    //gtk_label_set_justify(GTK_LABEL(list_item->information), GTK_JUSTIFY_LEFT);
    gtk_widget_set_name(GTK_WIDGET(list_item->information), "list_item_information");
    // gtk_box_pack_start(GTK_BOX(list_item->vbox), list_item->subtitle, FALSE, FALSE, 0);
    gtk_table_attach_defaults(GTK_TABLE(list_item->table), list_item->information, 5, 6, 3, 6);    

    gtk_widget_show_all(GTK_WIDGET(list_item));
}

void gtk_content_list_item_set_title(GtkContentListItem * item, const gchar * text)
{
    g_return_if_fail(IS_GTK_CONTENT_LIST_ITEM(item));
    g_return_if_fail(text != NULL);

    CL_LOGPRINTF("text %s", text);

    gtk_label_set_text(GTK_LABEL(item->title), text);
}

void gtk_content_list_item_set_subtitle(GtkContentListItem * item, const gchar * text)
{
    g_return_if_fail(IS_GTK_CONTENT_LIST_ITEM(item));
    g_return_if_fail(text != NULL);

    CL_LOGPRINTF("text %s", text);

    gtk_label_set_text(GTK_LABEL(item->subtitle), text);
}

void gtk_content_list_item_set_description(GtkContentListItem * item, const gchar * text)
{
    g_return_if_fail(IS_GTK_CONTENT_LIST_ITEM(item));
    g_return_if_fail(text != NULL);
    CL_LOGPRINTF("text %s", text);
    
    // check length
    if (strlen(text) >= 2 * MAX_CHARACTERS || get_string_lines(text) >= 2)
    {
        // can not handle multi lines, TODO
        gtk_label_set_ellipsize(GTK_LABEL(item->description), PANGO_ELLIPSIZE_END);
    }
    else
    {
        gtk_label_set_ellipsize(GTK_LABEL(item->description), PANGO_ELLIPSIZE_NONE);
    }
    gtk_label_set_text(GTK_LABEL(item->description), text);
}

void gtk_content_list_item_set_information(GtkContentListItem * item, const gchar * text)
{
    g_return_if_fail(IS_GTK_CONTENT_LIST_ITEM(item));
    g_return_if_fail(text != NULL);
    CL_LOGPRINTF("text %s", text);
    gtk_label_set_text(GTK_LABEL(item->information), text);
}

void gtk_content_list_item_set_thumb(GtkContentListItem * item, const char *iconURL)
{
    GdkPixbuf *pixbuf = NULL;
    
    g_return_if_fail(IS_GTK_CONTENT_LIST_ITEM(item));
    g_return_if_fail(NULL != iconURL);

    pixbuf = icons_load(iconURL);
    if (pixbuf)
    {
        // free the old one 
        if (item->pixmap)
        {
            icons_unload(item->pixmap);
            item->pixmap = NULL;
        }
        // set the new one
        item->pixmap = pixbuf;

        gtk_image_set_from_pixbuf(GTK_IMAGE(item->image), pixbuf);
       
        gtk_widget_show(GTK_WIDGET(item->image));
    }
    else
    {
        gtk_widget_hide(GTK_WIDGET(item->image));
    }
    return;
}

void gtk_content_list_item_set_icon_thumb(GtkContentListItem * item, unsigned int iconID)
{
    GdkPixbuf *pixbuf;

    g_return_if_fail(IS_GTK_CONTENT_LIST_ITEM(item));

    pixbuf = icons_get(iconID);

    if (pixbuf)
    {
        gtk_image_set_from_pixbuf(GTK_IMAGE(item->image), pixbuf);
        gtk_widget_show(GTK_WIDGET(item->image));
    }
    else
    {
        gtk_widget_hide(GTK_WIDGET(item->image));
    }
    return;
}

void gtk_content_list_item_hide_thumb(GtkContentListItem * item)
{
    g_return_if_fail(IS_GTK_CONTENT_LIST_ITEM(item));
    gtk_widget_hide(GTK_WIDGET(item->image));
}

void gtk_content_list_item_show_cursor(GtkContentListItem * item, gboolean show)
{
    g_return_if_fail(IS_GTK_CONTENT_LIST_ITEM(item));

    CL_CURSORPRINTF("entry %s", (show == TRUE) ? "TRUE" : "FALSE");

    if (show)
    {
        gtk_widget_show(item->cursorbox);
    }
    else
    {
        gtk_widget_hide(item->cursorbox);
    }
}

int  get_string_lines(const gchar * string)
{
    int count = 0;
    if (NULL == string)
    {
        return count;
    }
   
    while(string && *string != 0)
    {
        string = strchr(string, '\n');
        if (string)
        { 
            ++count; ++string; continue;
        }
        return count;
    }
    return count;
}
