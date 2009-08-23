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
 * \file erGtkEntry.c
 * \brief ereader gtk library - GtkEntry object adapted for ereader system
 *
 * Emits signal "changed-stable" shortly after the latest change of the entry text.
 * Emits signal "screen-refresh" when a screen refresh is needed for the entry object.
 * 
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 */

// system include files
#include <string.h>
#include <ctype.h>
#include <gtk/gtk.h>

// ereader library includes

// local includes
#include "ergtklog.h"
#include "erGtkEntry.h"
#include "erGtkEntryFilter.h"

// simulate overloading gtk_entry_set_text()
#undef gtk_entry_set_text


// local data
static GtkEntryClass* g_parent_class = NULL;

// class-specific signals for erGtkEntry
enum
{
    SIGNAL_CHANGED_STABLE = 0,       // changed + additional delay to wait for next signal
    SIGNAL_SCREEN_REFRESH,           // request screen refresh
    LAST_SIGNAL
};
static gint g_signals[LAST_SIGNAL];  // rely on the C compiler to initialise these to 0x00


// overloaded methods for base class
static void     ergtk_entry_grab_focus(GtkWidget* widget);

// overloaded signal handlers for base class
static void     ergtk_entry_notify(GObject* object, GParamSpec* pspec);
static gboolean ergtk_entry_focus_in(GtkWidget* widget, GdkEventFocus* event);
static gboolean ergtk_entry_focus_out(GtkWidget* widget, GdkEventFocus* event);
static gboolean ergtk_entry_focus(GtkWidget* widget, GtkDirectionType arg1);
static gboolean ergtk_entry_button_release(GtkWidget* widget, GdkEventButton* event);
static void     ergtk_entry_move_cursor(GtkEntry* widget, GtkMovementStep arg1, gint arg2, gboolean arg3);
static void     ergtk_entry_changed(GtkEditable* widget);

// implementation of virtual methods
static void     ergtk_entry_real_set_text(erGtkEntry* er_entry, const gchar* text);

// local functions
static void     ergtk_entry_class_init(erGtkEntryClass* klass);
static void     ergtk_entry_editable_init(GtkEditableClass*iface);
static void     ergtk_entry_init(erGtkEntry* input_entry);
static gboolean ergtk_entry_delayed_changed_or_movecursor(gpointer user_data);


GtkWidget* ergtk_entry_new()
{
    erGtkEntry* item = (erGtkEntry*) g_object_new(ERGTK_ENTRY_TYPE, NULL);

    return GTK_WIDGET(item);
}

void ergtk_entry_set_ipv4_filter(erGtkEntry* er_entry)
{
    g_return_if_fail(ERGTK_IS_ENTRY(er_entry));

    er_entry->filter = ipAddress_e;
    ipv4_filter_init(GTK_ENTRY(er_entry));
}

void ergtk_entry_set_integer_filter(erGtkEntry* er_entry)
{
    g_return_if_fail(ERGTK_IS_ENTRY(er_entry));

    er_entry->filter = integer_e;
    integer_filter_init(GTK_ENTRY(er_entry));
}

gboolean ergtk_entry_check_field(erGtkEntry *er_entry)
{
    GtkEntry    *entry;
    const gchar *text;
    gint         i, len, ret;
    gboolean     valid = FALSE;

    g_return_val_if_fail(ERGTK_IS_ENTRY(er_entry), FALSE);

    entry = GTK_ENTRY(er_entry);
    text = gtk_entry_get_text(entry);
    if (text && (text[0] != '\0'))
    {
        switch (er_entry->filter)
        {
            case ipAddress_e:
                ret = ipv4_filter_check_address(text);
                if (ret == ipv4Complete_e)
                {
                    valid = TRUE;
                }
                break;

            case integer_e:
                valid = TRUE;
                len = strlen(text);
                for (i = 0; i < len; i++)
                {
                    if (!isdigit(text[i]))
                    {
                        valid = FALSE;
                        break;
                    }
                }
                break;
            case string_e:
            default:
                valid = TRUE;
                break;
        }
    }

    LOGPRINTF("return %d", valid);
    return valid;
}

GType ergtk_entry_get_type(void)
{
    static GType class_type = 0;

    if (class_type == 0)
    {
        static const GTypeInfo class_info =
        {
            sizeof(erGtkEntryClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) ergtk_entry_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(erGtkEntry),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) ergtk_entry_init,
        };
        class_type = g_type_register_static(GTK_TYPE_ENTRY, "erGtkEntry", &class_info, 0);

        static const GInterfaceInfo editable_info =
        {
            (GInterfaceInitFunc)ergtk_entry_editable_init, /* interface_init */
            NULL, /* interface_finalize */
            NULL  /* interface_data */
        };
        g_type_add_interface_static(class_type, GTK_TYPE_EDITABLE, &editable_info);
    }

    return class_type;
}

// init class
static void ergtk_entry_class_init(erGtkEntryClass* klass)
{
    LOGPRINTF("entry");

    GObjectClass*   object_class = (GObjectClass*  )klass;
    GtkWidgetClass* widget_class = (GtkWidgetClass*)klass;
    GtkEntryClass*  entry_class  = (GtkEntryClass* )klass;

    // remember parent class struct, needed for chaining up to parent class
    g_parent_class = g_type_class_peek_parent(klass);

    // overload some virtual methods
    object_class->notify = ergtk_entry_notify;
    //
    widget_class->button_release_event = ergtk_entry_button_release;
    widget_class->focus_in_event       = ergtk_entry_focus_in;
    widget_class->focus_out_event      = ergtk_entry_focus_out;
    widget_class->focus                = ergtk_entry_focus;
    widget_class->grab_focus           = ergtk_entry_grab_focus;
    //
    entry_class->move_cursor = ergtk_entry_move_cursor;
    //
    klass->set_text          = ergtk_entry_real_set_text;

    // event to notify entry text has changed
    g_signals[SIGNAL_CHANGED_STABLE] = g_signal_new( "changed-stable",
                                                     G_OBJECT_CLASS_TYPE(object_class),
                                                     G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                                                     G_STRUCT_OFFSET(erGtkEntryClass, changed_stable),   // class closure
                                                     NULL,         // accumulator
                                                     NULL,         // accu data
                                                     gtk_marshal_VOID__VOID,
                                                     G_TYPE_NONE,  // return type
                                                     0,            // #params
                                                     0 );          // param types
    // event to notify screen refresh is needed
    g_signals[SIGNAL_SCREEN_REFRESH] = g_signal_new( "screen-refresh",
                                                     G_OBJECT_CLASS_TYPE(object_class),
                                                     G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE,
                                                     G_STRUCT_OFFSET(erGtkEntryClass, screen_refresh),   // class closure
                                                     NULL,         // accumulator
                                                     NULL,         // accu data
                                                     gtk_marshal_VOID__VOID,
                                                     G_TYPE_NONE,  // return type
                                                     0,            // #params
                                                     0 );          // param types
}

// init GtkEditable interface
static void ergtk_entry_editable_init(GtkEditableClass* iface)
{
    iface->changed = ergtk_entry_changed;
}

// init instance
static void ergtk_entry_init(erGtkEntry* er_entry)
{
    g_return_if_fail(ERGTK_IS_ENTRY(er_entry));

    GtkEntry*  entry  = (GtkEntry*) er_entry;
    GtkWidget* widget = (GtkWidget*)er_entry;
    
    // E-ink display requires a non-blinking cursur
    g_object_set(gtk_widget_get_settings(widget), "gtk-cursor-blink", FALSE, NULL);
    
    // set default size and attributes
    gtk_entry_set_editable(entry, TRUE);
    gtk_widget_set_size_request(widget, ERGTK_ENTRY_DEFAULT_WIDTH, ERGTK_ENTRY_DEFAULT_HEIGHT);
    gtk_entry_set_visibility(entry, TRUE);
   
    // instance-private data
    er_entry->timeout_id          = 0;
    er_entry->changed_pending     = FALSE;
    er_entry->changed_occurred    = FALSE;
    er_entry->movecursor_occurred = FALSE;
    er_entry->visible             = TRUE;
    er_entry->filter              = string_e;
}


// public methods
void ergtk_entry_set_text(erGtkEntry* er_entry, const gchar* text)
{
    g_return_if_fail(ERGTK_IS_ENTRY(er_entry));

    erGtkEntryClass* klass = ERGTK_ENTRY_GET_CLASS(er_entry);

    // chain to real method, which may be overloaded
    if (klass->set_text)
    {
        klass->set_text(er_entry, text);
    }
}

static void ergtk_entry_real_set_text(erGtkEntry* er_entry, const gchar* text)
{
    g_return_if_fail(ERGTK_IS_ENTRY(er_entry));
    g_return_if_fail(text != NULL);

    GtkEntry* entry = (GtkEntry*)er_entry;

    if (strcmp(text, gtk_entry_get_text(entry)) != 0)
    {
        // remember we expect a "changed" signal
        // Note: set changed_pending before calling parent class method,
        //       so we are sure that ergtk_entry_changed() callback reads it correctly.
        er_entry->changed_pending = TRUE;

        // chain to parent class
        gtk_entry_set_text(entry, text);
    }
}


// signal handlers
static void ergtk_entry_notify(GObject* object, GParamSpec* pspec)
{
    g_return_if_fail(ERGTK_IS_ENTRY(object));

    GtkWidget* widget = (GtkWidget*)object;

    // chain to parent class
    GObjectClass* parent_object_class = G_OBJECT_CLASS(g_parent_class);
    if (parent_object_class->notify)
    {
        parent_object_class->notify(object, pspec);
    }

    gboolean     b;
    const gchar* name = pspec->name;

    if (strcmp(name, "editable") == 0)
    {
        g_object_get(object, name, &b, NULL);
        gtk_widget_set_sensitive(widget, b);
    }
}

static gboolean ergtk_entry_focus_in(GtkWidget* widget, GdkEventFocus* event)
{
    g_return_val_if_fail(ERGTK_IS_ENTRY(widget), FALSE);

    gboolean    rc = FALSE;
    GtkEntry*   entry    = (GtkEntry*)widget;
    erGtkEntry* er_entry = (erGtkEntry*)widget;

    // chain to parent class
    if (g_parent_class->parent_class.focus_in_event)
    {
        rc = g_parent_class->parent_class.focus_in_event(widget, event);
    }

    // if protected, show content when empty
    er_entry->visible = gtk_entry_get_visibility(entry);
    if (er_entry->visible == FALSE)
    {
        const gchar* text = gtk_entry_get_text(entry);
        if (text[0] == '\0')
        {
            gtk_entry_set_visibility(entry, TRUE);
        }
    }
    
    return rc;
}

static gboolean ergtk_entry_focus_out(GtkWidget* widget, GdkEventFocus* event)
{
    g_return_val_if_fail(ERGTK_IS_ENTRY(widget), FALSE);

    gboolean    rc = FALSE;
    GtkEntry*   entry    = GTK_ENTRY(widget);
    erGtkEntry* er_entry = ERGTK_ENTRY(widget);

    // chain to parent class
    if (g_parent_class->parent_class.focus_out_event)
    {
        rc = g_parent_class->parent_class.focus_out_event(widget, event);
    }

    // de-select characters
    gtk_editable_select_region(GTK_EDITABLE(entry), -1, -1);

    // if protected, hide content
    if (er_entry->visible == FALSE)
    {
        gtk_entry_set_visibility(entry, FALSE);
    }
    
    return rc;
}

static gboolean ergtk_entry_focus(GtkWidget* widget, GtkDirectionType arg1)
{
    g_return_val_if_fail(ERGTK_IS_ENTRY(widget), FALSE);

    gboolean    rc = FALSE;
    erGtkEntry* er_entry = (erGtkEntry*)widget;

    // chain to parent class
    if (g_parent_class->parent_class.focus)
    {
        rc = g_parent_class->parent_class.focus(widget, arg1);
    }

    g_signal_emit(er_entry, g_signals[SIGNAL_SCREEN_REFRESH], 0, GTK_WIDGET(er_entry), NULL);
    return rc;
}

static void ergtk_entry_grab_focus(GtkWidget* widget)
{
    g_return_if_fail(ERGTK_IS_ENTRY(widget));

    // chain to parent class
    if (   gtk_widget_is_focus(widget) == FALSE
        && g_parent_class->parent_class.grab_focus )
    {
        g_parent_class->parent_class.grab_focus(widget);
    }
}

static void ergtk_entry_changed(GtkEditable* editable)
{
    g_return_if_fail(ERGTK_IS_ENTRY(editable));

    erGtkEntry* er_entry = (erGtkEntry*)editable;
    GtkEntry*   entry    = (GtkEntry*)editable;
    GtkWidget*  widget   = (GtkWidget*)editable;

    // for protected object, show content when empty
    if (   er_entry->visible == FALSE
        && gtk_widget_is_focus(widget) )
    {
        const gchar* text = gtk_entry_get_text(entry);
        if (text[0] == '\0')
        {
            gtk_entry_set_visibility(entry, TRUE);
        }
    }

    if (er_entry->changed_pending)
    {
        // changed event from ..set_text() -> ignore
        er_entry->changed_pending = FALSE;
    }
    else
    {
        // (re)start timer that will do the display update
        if (er_entry->timeout_id > 0)
        {
            g_source_remove(er_entry->timeout_id);
        }
        er_entry->timeout_id = g_timeout_add(300, ergtk_entry_delayed_changed_or_movecursor, er_entry);
        er_entry->changed_occurred = TRUE;
    }
}

static void ergtk_entry_move_cursor(GtkEntry* widget, GtkMovementStep arg1, gint arg2, gboolean arg3)
{
    g_return_if_fail(ERGTK_IS_ENTRY(widget));

    erGtkEntry* er_entry = (erGtkEntry*)widget;

    // chain to parent class
    if (g_parent_class->move_cursor)
    {
        g_parent_class->move_cursor(widget, arg1, arg2, arg3);
    }

    // (re)start timer that triggers ..-stable signal
    if (er_entry->timeout_id > 0)
    {
        g_source_remove(er_entry->timeout_id);
    }
    er_entry->timeout_id = g_timeout_add(300, ergtk_entry_delayed_changed_or_movecursor, er_entry);
    er_entry->movecursor_occurred = TRUE;
}

static gboolean ergtk_entry_button_release(GtkWidget* widget, GdkEventButton* event)
{
    g_return_val_if_fail(ERGTK_IS_ENTRY(widget), FALSE);

    gboolean    rc = FALSE;
    erGtkEntry* er_entry = (erGtkEntry*)widget;

    // chain to parent class
    if (g_parent_class->parent_class.button_release_event)
    {
        rc = g_parent_class->parent_class.button_release_event(widget, event);
    }
    
    g_signal_emit(er_entry, g_signals[SIGNAL_SCREEN_REFRESH], 0, GTK_WIDGET(er_entry), NULL);
    return rc;
}

static gboolean ergtk_entry_delayed_changed_or_movecursor(gpointer user_data)
{
    g_assert(ERGTK_IS_ENTRY(user_data));

    erGtkEntry* er_entry = (erGtkEntry*)user_data;

    // inform clients
    if (er_entry->movecursor_occurred)
    {
        er_entry->movecursor_occurred = FALSE; 
        g_signal_emit(er_entry, g_signals[SIGNAL_SCREEN_REFRESH], 0, GTK_WIDGET(er_entry), NULL);
    }
    if (er_entry->changed_occurred)
    {
        er_entry->changed_occurred = FALSE;
        g_signal_emit(er_entry, g_signals[SIGNAL_CHANGED_STABLE], 0, GTK_WIDGET(er_entry), NULL);
        g_signal_emit(er_entry, g_signals[SIGNAL_SCREEN_REFRESH], 0, GTK_WIDGET(er_entry), NULL);
    }

    er_entry->timeout_id = 0;
    return FALSE;  // FALSE = don't call again
}

