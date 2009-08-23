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
 * \file gtkInfoItem.h
 * \brief connectionMgr - item showing additional help or info text
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef __INFO_ITEM_H__
#define __INFO_ITEM_H__

#define INFOITEM_BORDER_TOP         10
#define INFOITEM_BORDER_BOTTOM      10
#define INFOITEM_BORDER_LEFT        13
#define INFOITEM_BORDER_RIGHT       13

#define INFOITEM_REQ_WIDTH          666
#define INFOITEM_REQ_HEIGHT         115

#define INFOITEM_BUTTON_WIDTH       100
#define INFOITEM_BUTTON_HEIGHT      27


#define INFOITEM_HBOX_SPACING       13

#define INFOITEM_ICON_IMG_WIDTH     75
#define INFOITEM_ICON_IMG_HEIGHT    102

#define INFOITEM_TEXT_WIDTH         (INFOITEM_REQ_WIDTH \
                                        - INFOITEM_BORDER_LEFT \
                                        - INFOITEM_ICON_IMG_WIDTH \
                                        - INFOITEM_HBOX_SPACING \
                                        - 1 /* don't ask me why */ \
                                        - INFOITEM_HBOX_SPACING \
                                        - INFOITEM_BUTTON_WIDTH \
                                        - INFOITEM_BORDER_RIGHT  )
#define INFOITEM_TEXT_HEIGHT        (INFOITEM_REQ_HEIGHT \
                                        - INFOITEM_BORDER_TOP \
                                        - INFOITEM_BORDER_BOTTOM)


#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define GTK_INFOITEM_TYPE            (gtk_infoitem_get_type())
#define GTK_INFOITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                      GTK_INFOITEM_TYPE, GtkInfoItem))
#define GTK_INFOITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                      GTK_INFOITEM_TYPE, GtkInfoItemClass))
#define IS_GTK_INFOITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                      GTK_INFOITEM_TYPE))
#define IS_GTK_INFOITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                      GTK_INFOITEM_TYPE))

typedef struct _GtkInfoItem       GtkInfoItem;
typedef struct _GtkInfoItemClass  GtkInfoItemClass;

typedef void on_infoitem_buttonclicked_t(GtkWidget* item, 
                                         const gboolean button_active);

struct _GtkInfoItem
{
    GtkEventBox                  eventbox;  // parent class
    GtkWidget*                   icon; 
    GtkWidget*                   label;
    GtkWidget*                   button;
    on_infoitem_buttonclicked_t* callback_on_buttonclicked;
};

struct _GtkInfoItemClass
{
    GtkEventBoxClass  parent_class;
};

typedef enum
{
    iconInfo_e = 0,
    iconOk_e,
    iconFailed_e,
}infoItemIcon_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    GType gtk_infoitem_get_type (void);


    // creates a new GtkInfoItem widget 
    GtkWidget* gtk_infoitem_new(gboolean with_button);

    // set info text
    void gtk_infoitem_set_icon(GtkInfoItem *item, infoItemIcon_t icon);
    void gtk_infoitem_set_text(GtkInfoItem *item, const gchar* text);
    void gtk_infoitem_set_buttontext(GtkInfoItem* item, const gchar* text);
    void gtk_infoitem_set_buttonstate(GtkInfoItem* item, 
                                      const gboolean active);
    void gtk_infoitem_callback_on_buttonclicked(GtkInfoItem* item, 
                             on_infoitem_buttonclicked_t* callback);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__INFO_ITEM_H__

