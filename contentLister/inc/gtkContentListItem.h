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
 * \file gtkContentListItem.h
 * \brief content lister - GTK widget for the eReader content Lister item. 
 *                         The eReader content Lister item contains one 
 *                         thumb, 2 labels and a texfield                 
 * 
 * <File description>
 * 
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __CONTENT_LIST_ITEM_H__
#define __CONTENT_LIST_ITEM_H__


#define ITEM_MAX_THUMB_WIDTH        91
#define ITEM_MAX_THUMB_HEIGHT       115

#define HBOX_TEXT_THUMB_SPACING     5
#define HBOX_TEXT_TEXT_SPACING      4
#define HBOX_TEXT_BORDER            11
#define VBOX_TEXT_TEXT_SPACING      2
#define VBOX_TEXT_BORDER            7

#define THUMB_BOX_MIN_WIDTH         91
#define TEXT_BOX_MIN_WIDTH          570
#define CURSOR_BOX_MIN_WIDTH        (LISTER_BOX_BORDER - HBOX_TEXT_THUMB_SPACING)


#include <gtk/gtk.h>
#include <gdk/gdk.h>

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

#define GTK_CONTENT_LIST_ITEM_TYPE            (gtk_content_list_item_get_type ())
#define GTK_CONTENT_LIST_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_CONTENT_LIST_ITEM_TYPE, GtkContentListItem))
#define GTK_CONTENT_LIST_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_CONTENT_LIST_ITEM_TYPE, GtkContentListItemClass))
#define IS_GTK_CONTENT_LIST_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_CONTENT_LIST_ITEM_TYPE))
#define IS_GTK_CONTENT_LIST_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_CONTENT_LIST_ITEM_TYPE))

typedef struct _GtkContentListItem GtkContentListItem;
typedef struct _GtkContentListItemClass GtkContentListItemClass;

struct _GtkContentListItem
{
    GtkHBox box;            //container
    
    GtkWidget *textbox;
    GtkWidget *thumbbox;
    GtkWidget *cursorspace;
    GtkWidget *cursorbox;
    GtkWidget *image;
    GdkPixbuf *pixmap;      // image buffer, used when non icon thumb is used
    GtkWidget *vbox;
    GtkWidget *table;       // table container in order to display more information
    GtkWidget *title;       // first label
    GtkWidget *subtitle;    // second label
    GtkWidget *description; // text field
    GtkWidget *information; // additional information
    int     index;          // location in the page lister page
};

struct _GtkContentListItemClass
{
    GtkHBoxClass parent_class;
};

/**
 * creates a new GtkListItem widget
 *
 * @param index - the widget location on the lister page
 *
 * @returns reference to created widget
 */
GtkWidget *gtk_content_list_item_new(int index);


/**
 * returns type of GtkListItem widget
 *
 * @returns type
 */
GType gtk_content_list_item_get_type(void);

/**
 * Set title text of GtkListItem widget
 *
 * @param text
 *
 */
void gtk_content_list_item_set_title(GtkContentListItem * item, const gchar * text);

/**
 * Set subtitle text of GtkListItem widget
 *
 * @param item the GtkListItem widget
 * @param text
 *
 */
void gtk_content_list_item_set_subtitle(GtkContentListItem * item, const gchar * text);

/**
 * Set description text of GtkListItem widget
 *
 * @param item the GtkListItem widget
 * @param text
 *
 */
void gtk_content_list_item_set_description(GtkContentListItem * item, const gchar * text);

/**
 * Set thumb of GtkListItem widget
 *
 * @param item the GtkListItem widget
 * @param iconURL icon path 
 *
 */
void gtk_content_list_item_set_thumb(GtkContentListItem * item, const char* iconURL);

/**
 * Set thumb of GtkListItem widget
 *
 * @param item the GtkListItem widget
 * @param iconID icon identification
 *
 */
void gtk_content_list_item_set_icon_thumb(GtkContentListItem * item, unsigned int iconID);

/**
 * Hide thumb 
 *
 * @param item the GtkListItem widget
 *
 */
void gtk_content_list_item_hide_thumb(GtkContentListItem * item);

/**
 * Clear thumb and free allocated resources of the GtkListItem widget
 *
 * @param item the GtkListItem widget
 * @param show 
 *
 */
void gtk_content_list_item_show_cursor(GtkContentListItem * item, gboolean show);

/**
 * Set information text of GtkListItem widget
 *
 * @param item the GtkListItem widget
 * @param text
 *
 */
void gtk_content_list_item_set_information(GtkContentListItem * item, const gchar * text);

#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__CONTENT_LIST_ITEM_H__
