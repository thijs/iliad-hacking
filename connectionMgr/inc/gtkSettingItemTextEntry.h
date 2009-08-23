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
 * \file gtkSettingItemTextEntry.h
 * \brief connectionMgr - item containing one text entry field
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef __SETTING_ITEM_TEXT_ENTRY_H__
#define __SETTING_ITEM_TEXT_ENTRY_H__

#define SETTINGITEMTEXT_BORDER_TOP       0
#define SETTINGITEMTEXT_BORDER_BOTTOM    0
#define SETTINGITEMTEXT_BORDER_LEFT      13
#define SETTINGITEMTEXT_BORDER_RIGHT     13

#define SETTINGITEMTEXT_REQ_WIDTH        640
#define SETTINGITEMTEXT_REQ_HEIGHT       115

#define SETTINGITEMTEXT_LABEL_HEIGHT     50

#define SETTINGITEMTEXT_ENTRY_HEIGHT     30
#define SETTINGITEMTEXT_ENTRY_WIDTH      400

#define SETTINGITEMTEXT_BUTTON_WIDTH_SMALL 130
#define SETTINGITEMTEXT_BUTTON_WIDTH_LARGE 200
#define SETTINGITEMTEXT_BUTTON_PADDING     25

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>

#define GTK_SETTINGITEM_TEXTENTRY_TYPE (gtk_settingitem_textentry_get_type())
#define GTK_SETTINGITEM_TEXTENTRY(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                        GTK_SETTINGITEM_TEXTENTRY_TYPE, \
                                        GtkSettingItemTextEntry))
#define GTK_SETTINGITEM_TEXTENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                                GTK_SETTINGITEM_TEXTENTRY_TYPE, \
                                                GtkSettingItemTextEntryClass))
#define IS_GTK_SETTINGITEM_TEXTENTRY(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                           GTK_SETTINGITEM_TEXTENTRY_TYPE))
#define IS_GTK_SETTINGITEM_TEXTENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                                   GTK_SETTINGITEM_TEXTENTRY_TYPE))

typedef struct _GtkSettingItemTextEntry       GtkSettingItemTextEntry;
typedef struct _GtkSettingItemTextEntryClass  GtkSettingItemTextEntryClass;

typedef void on_textentry_changed_t(GtkWidget* item, const gchar* text);
typedef void on_textentry_buttonclicked_t(GtkWidget* item, 
                                          const gboolean button_active);

struct _GtkSettingItemTextEntry
{
    GtkEventBox                   eventbox;         // parent class

    // private
    GtkWidget*                    label;
    GtkWidget*                    entry;
    GtkWidget*                    button;
    on_textentry_changed_t*       callback_on_changed;
    on_textentry_buttonclicked_t* callback_on_buttonclicked;
};

struct _GtkSettingItemTextEntryClass
{
    GtkEventBoxClass  parent_class;
    
    void (*settingitem_textentry_enter) (GtkSettingItemTextEntry *item, 
                                         char* text);
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    // creates a new GtkSettingItemTextEntry widget 
    GtkWidget* gtk_settingitem_textentry_new(gboolean with_button);

    void gtk_settingitem_textentry_grabfocus(GtkSettingItemTextEntry *item);

    void gtk_settingitem_textentry_setlabel(GtkSettingItemTextEntry *item, 
                                            const gchar* text);

    void gtk_settingitem_textentry_settext(GtkSettingItemTextEntry *item, 
                                           const gchar* text);
    void gtk_settingitem_textentry_setvisibility(GtkSettingItemTextEntry* item, 
                                                 gboolean visible);
    void gtk_settingitem_textentry_setsensitive(GtkSettingItemTextEntry* item, 
                                                gboolean sensitive);

    void gtk_settingitem_textentry_show_button(GtkSettingItemTextEntry* item, 
                                               const gboolean show);
    void gtk_settingitem_textentry_set_buttontext(GtkSettingItemTextEntry *item, 
                                                  const gchar* text);
    void gtk_settingitem_textentry_set_buttonsize(GtkSettingItemTextEntry *item,
                                                  const gboolean large_button);
    void gtk_settingitem_textentry_set_buttonstate(GtkSettingItemTextEntry*item, 
                                                   const gboolean active);
    const gboolean gtk_settingitem_textentry_get_buttonactive(GtkSettingItemTextEntry* item);

    // callback on text entry changed
    void gtk_settingitem_textentry_callback_on_changed(GtkSettingItemTextEntry* item, 
                                            on_textentry_changed_t* callback);

    // callback on button clicked
    void gtk_settingitem_textentry_callback_on_buttonclicked(GtkSettingItemTextEntry* item, 
                                        on_textentry_buttonclicked_t* callback);

    // pointer points to the internally allocated storage 
    // in the widget and must
    // not be freed, modified or stored.
    const gchar* gtk_settingitem_textentry_gettext(GtkSettingItemTextEntry *item);

    // returns type of this widget 
    GType gtk_settingitem_textentry_get_type (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__SETTING_ITEM_TEXT_ENTRY_H__

