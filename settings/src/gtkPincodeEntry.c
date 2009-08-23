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
 * \file gtkPincodeEntry.c
 * \brief content lister - Input Pincode Entry
 *
 * override the standard GtkEntry
 *
 * input pincode entry, limited between 4-8 digital numbers
 * characters are shown as '********'
 * call displayMgr to show the user's input to user on eink screen
 * 
 */

// system include files
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

// ereader library includes
#include <liberdm/display.h>
#include <liberdm/erdm.h>
#include <libergtk/ergtk.h>
#include <liberipc/eripctoolbar.h>

// local includes
#include "setupLog.h"
#include "displayStatus.h"
#include "gtkPincodeEntry.h"


// internally used routines forward declarations
static void gtk_pincode_entry_class_init(GtkPincodeEntryClass* klass);
static void gtk_pincode_entry_init(GtkPincodeEntry* input_entry);

static void on_gtk_pincode_entry_insert_text(GtkWidget* widget,
                                             const gchar* text, gint length, gint* position, gpointer user_data);


GtkWidget* gtk_pincode_entry_new()
{
    GtkPincodeEntry* pincode_entry = (GtkPincodeEntry*) g_object_new(GTK_PINCODE_ENTRY_TYPE, NULL);

    return GTK_WIDGET(pincode_entry);
}

GType gtk_pincode_entry_get_type(void)
{
    static GType pincode_entry_type = 0;

    if (!pincode_entry_type)
    {
        static const GTypeInfo pincode_entry_info = {
            sizeof(GtkPincodeEntryClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) gtk_pincode_entry_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(GtkPincodeEntry),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) gtk_pincode_entry_init,
        };

        pincode_entry_type = g_type_register_static(ERGTK_ENTRY_TYPE, "PincodeEntry", &pincode_entry_info, 0);
    }
    return pincode_entry_type;
}

// init class
static void gtk_pincode_entry_class_init(GtkPincodeEntryClass* klass)
{
    return;
}

// initialise a new object
static void gtk_pincode_entry_init(GtkPincodeEntry* pincode_entry)
{
    g_return_if_fail(pincode_entry != NULL);

    GtkWidget* widget = (GtkWidget*)pincode_entry;
    GtkEntry*  entry  = (GtkEntry* )pincode_entry;

    gtk_widget_set_size_request(widget, PINCODE_INPUT_ENTRY_WIDTH, PINCODE_INPUT_ENTRY_HEIGHT);
    gtk_entry_set_max_length(entry, PINCODE_UI_MAX_LENGTH);
    gtk_entry_set_visibility(entry, FALSE);

    gtk_signal_connect(GTK_OBJECT(entry), "insert_text", GTK_SIGNAL_FUNC(on_gtk_pincode_entry_insert_text), NULL);
}

// check text is valid pincode string
// a valid pincode string should be only digital numbers
// and it's length is limited between 4-8 characters(letters)
gboolean gtk_pincode_entry_check_text(GtkPincodeEntry* pincode_entry)
{
    const gchar *text = NULL;
    int i, len;
    gboolean entry_ok = FALSE;

    g_return_val_if_fail((pincode_entry!= NULL), FALSE);
    g_return_val_if_fail(IS_GTK_PINCODE_ENTRY(pincode_entry), FALSE);

    GtkEntry* entry = (GtkEntry*)pincode_entry;

    text = gtk_entry_get_text(entry);
    if (text == NULL)
    {
        text = "";
    }

    len = strlen(text);
    if (   len >= PINCODE_UI_MIN_LENGTH
        && len <= PINCODE_UI_MAX_LENGTH )
    {
        entry_ok = TRUE;
        for (i = 0; i < len; i++)
        {
            if ( !isdigit(text[i]) )
            {
                entry_ok = FALSE;
            }
        }
    }

    return entry_ok;
}

// signal callback functions about GtkPincodeEntry
// filter out the invalid characters
static void on_gtk_pincode_entry_insert_text(GtkWidget* widget,
                                             const gchar* text, gint length, gint* position, gpointer user_data)
{
    GtkEditable *editable = GTK_EDITABLE(widget);
    int i, count = 0;
    gchar *result = g_new(gchar, length);

    for (i = 0; i < length; i++)
    {
        if (!isdigit(text[i]))
            continue;
        result[count++] = text[i];
    }
    if (count > 0)
    {
        gtk_signal_handler_block_by_func(GTK_OBJECT(editable),
                                         GTK_SIGNAL_FUNC(on_gtk_pincode_entry_insert_text), user_data);
        gtk_editable_insert_text(editable, result, count, position);
        gtk_signal_handler_unblock_by_func(GTK_OBJECT(editable),
                                           GTK_SIGNAL_FUNC(on_gtk_pincode_entry_insert_text), user_data);
    }
    gtk_signal_emit_stop_by_name(GTK_OBJECT(editable), "insert_text");

    g_free(result);
}

