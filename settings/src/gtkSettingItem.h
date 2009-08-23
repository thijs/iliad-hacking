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
 * \file gtkSettingItem.h
 * \brief setup application - empty setup item containing a header label and some item
 *                            information text.
 * <File description>
 * 
 */

#ifndef __SETTING_ITEM_H__
#define __SETTING_ITEM_H__

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define SMALL_BUTTON_SPACING  15
#define LARGE_BUTTON_SPACING  7
#define SMALL_BUTTON_HEIGHT   27
#define SMALL_BUTTON_WIDTH    ((666 - (3*SMALL_BUTTON_SPACING))/4)
#define LARGE_BUTTON_HEIGHT   27
#define LARGE_BUTTON_WIDTH    170

#define SETTINGITEM_ITEM_SPACING    5
#define SETTING_TITLE_MIN_WIDTH     666
#define SETTING_TITLE_MIN_HEIGHT    27
#define SETTING_INFO_MIN_HEIGHT     17

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_SETTINGITEM_TYPE            (gtk_settingitem_get_type ())
#define GTK_SETTINGITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_SETTINGITEM_TYPE, GtkSettingItem))
#define GTK_SETTINGITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_SETTINGITEM_TYPE, GtkSettingItemClass))
#define IS_GTK_SETTINGITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_SETTINGITEM_TYPE))
#define IS_GTK_SETTINGITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_SETTINGITEM_TYPE))

typedef struct _GtkSettingItem       GtkSettingItem;
typedef struct _GtkSettingItemClass  GtkSettingItemClass;

struct _GtkSettingItem
{
    GtkEventBox eventbox;   
    GtkWidget* headerlabel;
    GtkWidget* infolabel;
    GtkWidget* clientArea;
};

struct _GtkSettingItemClass
{
    GtkEventBoxClass parent_class;
};

// creates a new GtkSettingItem widget 
GtkWidget* gtk_settingitem_new(gchar* headerlabel);

// set the info/help text associated with this "setting item"
void gtk_settingitem_set_info_text(GtkSettingItem *settingitem, gchar* infotext); 

// set the header label text
void gtk_settingitem_set_header_text(GtkSettingItem *settingitem, gchar* headertext);

// Fill client Area of the the settings item with details
void gtk_settingitem_add_details(GtkSettingItem *settingitem, GtkWidget* details);

// returns type of this widget 
GType gtk_settingitem_get_type (void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__SETTING_ITEM_H__
