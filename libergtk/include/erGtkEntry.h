/*
 * This file is part of libergtk.
 *
 * libergtk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libergtk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file erGtkEntry.h
 * \brief ereader gtk library - GtkEntry object adapted for ereader system
 *
 * Emits signal "changed-stable" shortly after the latest change of the entry text.
 * Emits signal "screen-refresh" when a screen refresh is needed for the entry object.
 * 
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 */

#ifndef __ERGTK_ENTRY_H__
#define __ERGTK_ENTRY_H__
#ifdef __cplusplus
extern "C"
{
#endif

#define ERGTK_ENTRY_DEFAULT_WIDTH 150   // value from the designed screen
#define ERGTK_ENTRY_DEFAULT_HEIGHT 30

#define ERGTK_ENTRY_TYPE                 (ergtk_entry_get_type())
#define ERGTK_ENTRY(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), ERGTK_ENTRY_TYPE, erGtkEntry))
#define ERGTK_IS_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), ERGTK_ENTRY_TYPE))

#define ERGTK_ENTRY_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST((klass),  ERGTK_ENTRY_TYPE, erGtkEntryClass))
#define ERGTK_ENTRY_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS((obj),  ERGTK_ENTRY_TYPE, erGtkEntryClass))
#define ERGTK_IS_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE((klass),  ERGTK_ENTRY_TYPE))

// simulate overloading of gtk_entry_set_text()
// unfortunately the GtkEntry class did not implement this a virtual method
#define gtk_entry_set_text(entry, text)                         \
    if ( ERGTK_IS_ENTRY(entry) )                                \
        ergtk_entry_set_text( (erGtkEntry*)(entry), (text) );   \
    else                                                        \
        gtk_entry_set_text( (entry), (text) );

typedef enum
{
    ipAddress_e = 0,
    integer_e,
    string_e
}entryFilter_e;

typedef struct _erGtkEntry      erGtkEntry;
typedef struct _erGtkEntryClass erGtkEntryClass;

struct _erGtkEntry
{
    GtkEntry     parent;                // parent class

    // private:
    guint        timeout_id;            // timeout handler for "stable" signals
    gboolean     changed_pending;       //   "changed" event expected from ..set_text()
    gboolean     changed_occurred;      //   "changed" event has occurred
    gboolean     movecursor_occurred;   //   "move-cursor" event has occurred
    gboolean     visible;               // false = protect text entry by displaying '***'
    entryFilter_e filter;
};

struct _erGtkEntryClass
{
    GtkEntryClass parent_class;

    // virtual methods
    void (*set_text)           (erGtkEntry* er_entry, const gchar* text);
    
    // signal handlers
    void (*changed_stable)     (erGtkEntry* er_entry);
    void (*screen_refresh)     (erGtkEntry* er_entry);
};

/**
 * creates a new erGtkEntry widget
 *
 * @param - 
 *
 * @returns reference to created widget
 */
GtkWidget* ergtk_entry_new();

/**
 * get type of erGtkEntry widget
 *
 * @returns type
 */
GType ergtk_entry_get_type(void);

/**
 * set the user input text
 * @param er_entry the erGtkEntry widget
 * @param text the user input text
 *
 * @returns - the user input text
 */
void ergtk_entry_set_text(erGtkEntry* er_entry, const gchar * text);

void ergtk_entry_set_ipv4_filter(erGtkEntry *er_entry);

void ergtk_entry_set_integer_filter(erGtkEntry *er_entry);

gboolean ergtk_entry_check_field(erGtkEntry *er_entry);

#ifdef __cplusplus
}
#endif

#endif                          /* __ERGTK_ENTRY_H__ */
