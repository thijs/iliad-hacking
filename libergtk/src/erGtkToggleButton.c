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
 * \file erGtkToggleButton.c
 * \brief ereader gtk library - GtkToggleButton object adapted for ereader system
 *
 * Changes colour of the button's child object as a fucntion of the button state.
 * 
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 */

// system include files
#include <string.h>
#include <gtk/gtk.h>

// ereader library includes

// local includes
#include "ergtklog.h"
#include "erGtkToggleButton.h"


// local data
static GtkToggleButtonClass* g_parent_class = NULL;

// size to style conversion table
// Note: entries must be sorted ascending on (1) x_size then (2) y_size
static const struct
             {
                 guint        width;
                 guint        height;
                 const gchar* style;
             } g_style_tbl[]
                = {
                    { 130, 27, "erGtkToggleButton_130_27" },
                    { 150, 27, "erGtkToggleButton_150_27" },
                    { 200, 27, "erGtkToggleButton_200_27" },
                    { 310, 27, "erGtkToggleButton_310_27" },
                    { 640, 44, "erGtkToggleButton_640_44" },
                    {   0,  0, ""                         }
                  };
#define DEFAULT_STYLE_INDEX  0
#define STYLE_PREFIX         "erGtkToggleButton"
#define STYLE_PREFIX_LEN     strlen(STYLE_PREFIX)

// overloaded methods for base class

// overloaded signal handlers for base class
static void     ergtk_toggle_button_notify(GObject* object, GParamSpec* pspec);
static gboolean ergtk_toggle_button_expose_event(GtkWidget* widget, GdkEventExpose* event);
static void     ergtk_toggle_button_toggled(GtkToggleButton* toggleButton);

// implementation of virtual methods

// local functions
static void     ergtk_toggle_button_class_init(erGtkToggleButtonClass* klass);
static void     ergtk_toggle_button_init(erGtkToggleButton* input_entry);
static void     fix_label_colour(erGtkToggleButton* toggleButton);


GtkWidget* ergtk_toggle_button_new()
{
    erGtkToggleButton* item = (erGtkToggleButton*) g_object_new(ERGTK_TOGGLE_BUTTON_TYPE, NULL);

    GtkWidget* widget = (GtkWidget*)item;
    GTK_WIDGET_UNSET_FLAGS(widget, GTK_CAN_FOCUS);
    
    return widget;
}

GtkWidget* ergtk_toggle_button_new_with_label(const gchar *label)
{
    erGtkToggleButton* item = (erGtkToggleButton*) g_object_new(ERGTK_TOGGLE_BUTTON_TYPE, "label", label, NULL);
    
    GtkWidget* widget = (GtkWidget*)item;
    GTK_WIDGET_UNSET_FLAGS(widget, GTK_CAN_FOCUS);

    return widget;
}

GType ergtk_toggle_button_get_type(void)
{
    static GType class_type = 0;

    if (class_type == 0)
    {
        static const GTypeInfo class_info =
        {
            sizeof(erGtkToggleButtonClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) ergtk_toggle_button_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(erGtkToggleButton),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) ergtk_toggle_button_init,
        };
        class_type = g_type_register_static(GTK_TYPE_TOGGLE_BUTTON, "erGtkToggleButton", &class_info, 0);
    }

    return class_type;
}

// init class
static void ergtk_toggle_button_class_init(erGtkToggleButtonClass* klass)
{
    LOGPRINTF("entry");

    GObjectClass*         object_class       = (GObjectClass*        )klass;
    GtkWidgetClass*       widget_class       = (GtkWidgetClass*      )klass;
    GtkToggleButtonClass* toggleButton_class = (GtkToggleButtonClass*)klass;

    // remember parent class struct, needed for chaining up to parent class
    g_parent_class = g_type_class_peek_parent(klass);

    // overload some virtual methods
    object_class->notify        = ergtk_toggle_button_notify;
    // 
    widget_class->expose_event  = ergtk_toggle_button_expose_event;
    // 
    toggleButton_class->toggled = ergtk_toggle_button_toggled;
}

// init instance
static void ergtk_toggle_button_init(erGtkToggleButton* item)
{
    LOGPRINTF("entry");
    g_return_if_fail(ERGTK_IS_TOGGLE_BUTTON(item));

    GtkWidget* widget = (GtkWidget*)item;
    
    // set default size
    gtk_widget_set_size_request( widget,
                                 g_style_tbl[DEFAULT_STYLE_INDEX].width,
                                 g_style_tbl[DEFAULT_STYLE_INDEX].height );
}


// overloaded methods


// signal handlers
static void ergtk_toggle_button_notify(GObject* object, GParamSpec* pspec)
{
    g_return_if_fail(ERGTK_IS_TOGGLE_BUTTON(object));

    int          i;
    gint         width;
    const gchar* name = pspec->name;
    GtkWidget*   widget = (GtkWidget*)object;
    const gchar* widget_name;

    // chain to parent class
    GObjectClass* parent_object_class = G_OBJECT_CLASS(g_parent_class);
    if (parent_object_class->notify)
    {
        parent_object_class->notify(object, pspec);
    }

    if (strcmp(name, "width-request") == 0)
    {
        // set name and rc-style as a function of width
        g_object_get(object, name, &width, NULL);
        widget_name = gtk_widget_get_name(widget);
        if (   strcmp(widget_name, "erGtkToggleButton") == 0
            || strncmp(widget_name, STYLE_PREFIX, STYLE_PREFIX_LEN) == 0 )
        {
            i = 0;
            while (width > g_style_tbl[i].width  &&  g_style_tbl[i+1].width != 0)
            {
                i++;
            }
            gtk_widget_set_name(widget, g_style_tbl[i].style);
        }
    }
}

static gboolean ergtk_toggle_button_expose_event(GtkWidget* widget, GdkEventExpose* event)
{
    g_return_val_if_fail(ERGTK_IS_TOGGLE_BUTTON(widget), FALSE);

    // chain to parent class
    GtkWidgetClass* parent_widget_class = GTK_WIDGET_CLASS(g_parent_class);
    if (parent_widget_class->expose_event)
    {
        parent_widget_class->expose_event(widget, event);
    }

    fix_label_colour(ERGTK_TOGGLE_BUTTON(widget));

    return FALSE;  // continue event handling
}

static void ergtk_toggle_button_toggled(GtkToggleButton* toggleButton)
{
    g_return_if_fail(ERGTK_IS_TOGGLE_BUTTON(toggleButton));

    // chain to parent class
    if (g_parent_class->toggled)
    {
        g_parent_class->toggled(toggleButton);
    }

    fix_label_colour(ERGTK_TOGGLE_BUTTON(toggleButton));
}

static void fix_label_colour(erGtkToggleButton* erToggleButton)
{

    // update text colour for button label
    // when the (invisible) cursor is on the button, the text label remains in state PRELIGHT
    // we now change the PRELIGHT colour the one that goes with the button state
    gboolean     active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(erToggleButton));
    GtkWidget*   label  = gtk_bin_get_child(GTK_BIN(erToggleButton));
    GtkStyle*    style  = gtk_widget_get_style(label);
    GtkStateType betterstate;
    if (active)
    {
        betterstate = GTK_STATE_ACTIVE;
    }
    else
    {
        betterstate = GTK_STATE_NORMAL;
    }
    
    // set colour for prelight style
    // Note: only call gtk_widget_modify_fg() when needed, to avoid an avalanche of expose events
    if (memcmp( &(style->fg[GTK_STATE_PRELIGHT]), &(style->fg[betterstate]), sizeof(style->fg[0])) != 0)
    {
        gtk_widget_modify_fg(label, GTK_STATE_PRELIGHT, &(style->fg[betterstate]));
    }
}

