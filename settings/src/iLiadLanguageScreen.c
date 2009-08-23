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
 * \file iLiadLanguageScreen.c
 * \brief settings -Language Selection Screen 
 *
 * Providing GUI to let users select language. 
 * Notes: This screen is not one page of gtk_notebook. 
 * 
 */
#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <liberdm/display.h>
#include <libergtk/ergtk.h>

#include "iLiadLanguageData.h"
#include "iLiadLanguageScreen.h"
#include "languages.h"
#include "erbusy.h"
#include "displayStatus.h"
#include "setupLog.h"

// global variables 
static iLiad_locale_t*  g_locale_installed_tbl = NULL;
static guint            g_locale_installed_num = 0;
 
static gtkLanguage_t    g_select_language;

// internal functions
static void     on_language_destroy(GtkWidget * widget, gpointer data);
static gboolean on_language_expose(GtkWidget * widget, GdkEventExpose * event, gpointer data);
static gboolean on_language_key_press(GtkWidget* widget, GdkEventKey* event, gpointer data);

static void     on_language_button_press(GtkWidget * widget, GdkEventButton * event, gpointer data);
static void     on_language_selection_update(erGtkSelectionGroup * selection, gpointer button, gpointer data);

static void up_one_item(erGtkSelectionGroup* selection);
static void up_to_first_item(erGtkSelectionGroup* selection);
static void down_one_item(erGtkSelectionGroup* selection);
static void down_to_last_item(erGtkSelectionGroup* selection);
static gboolean confirm_current_item(gpointer data);

// widget hierarchy
// |--window
//     |--background(GtkEventBox)
//        |--alignment
//           |--vbox
//              |--button1
//              |--...
//              |--buttonN
GtkWidget* iLiad_language_screen_create(void)
{
    GtkWidget*  window;
    GtkWidget*  background;
    GtkWidget*  alignment;
    GtkWidget*  vbox;
    GtkWidget*  button;
    GtkWidget*  selection;
    gint        i, nbuttons; 
    GtkToggleButton* button_tbl[LANGUAGE_MAX_BUTTON_NUMBER + 1];
    gint        paddingTop, paddingBottom;

    g_locale_installed_num = find_installed_locales(&g_locale_installed_tbl);
    // calculate how many buttons to be created
    nbuttons = ( (g_locale_installed_num <= LANGUAGE_MAX_BUTTON_NUMBER) 
                  ? g_locale_installed_num : LANGUAGE_MAX_BUTTON_NUMBER);

    // window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), PACKAGE " " VERSION);
    gtk_window_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    gtk_widget_set_size_request(GTK_WIDGET(window), SCREEN_WIDTH, SCREEN_HEIGHT);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_fullscreen(GTK_WINDOW(window)); 
    g_select_language.window = window; 

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "language_background");
    gtk_widget_set_size_request(background, LANGUAGE_SCREEN_WIDTH, LANGUAGE_SCREEN_HEIGHT);
    gtk_container_add(GTK_CONTAINER(window), background);

    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    // calculate top padding and bottom padding
    paddingTop = (LANGUAGE_SCREEN_HEIGHT + LANGUAGE_SCREEN_SPACING
                     - (LANGUAGE_BUTTON_HEIGHT + LANGUAGE_SCREEN_SPACING) * nbuttons) / 2;
    paddingBottom = paddingTop;
    ST_LOGPRINTF("top padding = %d, bottom padding = %d", paddingTop, paddingBottom);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              paddingTop, paddingBottom,
                              LANGUAGE_SCREEN_PADDING_LEFT, LANGUAGE_SCREEN_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);
    
    // vbox
    vbox = gtk_vbox_new(FALSE, LANGUAGE_SCREEN_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    
    // button
    ST_LOGPRINTF("Creating %d buttons.", nbuttons);
    
    for (i = 0 ; i < nbuttons ; i++)
    {
        button = ergtk_toggle_button_new_with_label( g_locale_installed_tbl[i].long_name );
        button_tbl[i] = GTK_TOGGLE_BUTTON(button);
        gtk_widget_set_size_request(button, LANGUAGE_BUTTON_WIDTH, LANGUAGE_BUTTON_HEIGHT);
        gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
        g_signal_connect(G_OBJECT(button), "button_press_event", 
                G_CALLBACK(on_language_button_press), button);
    }
    button_tbl[i] = NULL;
    
    // selection
    selection = ergtk_selection_group_new(button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(selection), 0, 1);
    g_select_language.selection = selection;

    // signal handlers
    g_signal_connect(G_OBJECT(background), "expose-event", G_CALLBACK(on_language_expose), NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_language_destroy), &g_select_language);
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_language_key_press), &g_select_language);

    g_signal_connect(G_OBJECT(selection), "selection-update", 
            G_CALLBACK(on_language_selection_update), &g_select_language);

    gtk_widget_show_all(background);
    
    // return
    return window;
}

void iLiad_language_screen_display_data(void)
{
    gint        i;
    const char* locale;

    ST_LOGPRINTF("entry");

    // get the current language setting
    locale = iLiad_language_data_get();
   
    // get the index of button 
    for (i = 0 ; i < g_locale_installed_num ; i++)
    {
        if (strcmp(locale, g_locale_installed_tbl[i].code) == 0)
        {
            // found
            break;   // exit for
        }
    }
    
    if (i == g_locale_installed_num)
    {
        // not found
        ST_WARNPRINTF("Locale [%s] not installed.", locale);
    }
    else
    {
        // found
        GtkToggleButton* button = NULL;
        ST_LANGUAGEPRINTF("%s %s", locale, g_locale_installed_tbl[i].name);
        button = ergtk_selection_group_get_button((erGtkSelectionGroup*)g_select_language.selection, i);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(button));
    }
}

static void on_language_destroy(GtkWidget * widget, gpointer data)
{
    ST_LOGPRINTF("entry");

    // free memory  
    if (g_locale_installed_tbl)
    {
        free_installed_locales(&g_locale_installed_tbl, g_locale_installed_num);
        g_locale_installed_num = 0;
    }

    // quit gtk_main
    gtk_main_quit();
}

static gboolean on_language_expose(GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
    ST_LOGPRINTF("entry");
    
    int x, y, w, h;
    
    x = event->area.x;
    y = event->area.y;
    w = event->area.width;
    h = event->area.height;

    ST_LOGPRINTF("entry, x[%d], y[%d], w[%d], h[%d]", x, y, w, h);
 
    if ((0 == x) && (0 == y) && (SCREEN_WIDTH == w) &&(SCREEN_HEIGHT == h))
    {
        display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL, WAVEFORM_FULLSCREEN);
    }

    return FALSE;
}

static gboolean on_language_key_press(GtkWidget* widget, GdkEventKey* event, gpointer data)
{
    ST_LOGPRINTF("entry");

    gtkLanguage_t* language;
    erGtkSelectionGroup* selection;
    gboolean returnValue = FALSE;    // return FALSE => default gtk handling

    erbusy_off();
    
    language = (gtkLanguage_t*)data;
    g_return_val_if_fail(language != NULL, returnValue);
    selection = (erGtkSelectionGroup*)language->selection;
    g_assert(selection != NULL);

    switch (event->keyval)
    {
        case GDK_Up:
            ST_LOGPRINTF("Press UP one item key");
            up_one_item(selection);
            returnValue = TRUE;  // no further event handling
            break;
        case GDK_F4:
            ST_LOGPRINTF("Long press UP to the first key");
            up_to_first_item(selection);
            returnValue = TRUE;  // no further event handling
            break;

        case GDK_Down:
            ST_LOGPRINTF("Press DOWN one item key");
            down_one_item(selection);
            returnValue = TRUE;  // no further event handling
            break;
        case GDK_F3:
            ST_LOGPRINTF("Long press DOWN to the last key");
            down_to_last_item(selection); 
            returnValue = TRUE;  // no further event handling
            break;

        case GDK_Return:
        case GDK_F6:
            ST_LOGPRINTF("Press CONFIRM key");
            confirm_current_item((gpointer)TRUE);
            returnValue = TRUE;  // no further event handling
            break;

        case GDK_F5:
            ST_LOGPRINTF("Press TOP LEVEL key");
            // de-select all language keys
            ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(selection), 0, 0);
            // show buttons unselected, then quit without save ...
            g_timeout_add(500, confirm_current_item, (gpointer)FALSE);

            returnValue = TRUE; // no further event handling
            break;
            
        default:
            /* ignore */ 
            break;
    }
    return returnValue;
}

static void on_language_button_press(GtkWidget * widget, GdkEventButton * event, gpointer data)
{
    ST_LOGPRINTF("entry");
    
    gboolean pressed;  
    GtkToggleButton* button;

    button = GTK_TOGGLE_BUTTON(data);
    pressed = gtk_toggle_button_get_active(button);
    if (!pressed)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(button));
    
        // show button selected, then ...
        g_timeout_add(500, confirm_current_item, (gpointer)TRUE);
    }
    else
    {
        confirm_current_item((gpointer)TRUE);
    }
}

static void on_language_selection_update(erGtkSelectionGroup * selection, gpointer button, gpointer data)
{
    const gtkLanguage_t *language = (gtkLanguage_t*)data;
    int                  button_id = 0;

    ST_LOGPRINTF("entry");

    g_return_if_fail(selection != NULL);
    g_return_if_fail(data != NULL);

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(language->selection), &button_id, 1);
    if (button_id >= 0  &&  button_id < g_locale_installed_num)
    {
        iLiad_language_data_set(g_locale_installed_tbl[button_id].code);
    }

    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_TYPING);
}

static void up_one_item(erGtkSelectionGroup* selection)
{
    gint nButtons = ergtk_selection_group_get_length(selection);
    if (nButtons > 0)
    {
        int currentFocus = 0, newFocus = 0;
        GtkToggleButton* button= NULL;

        ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(selection), &currentFocus, 1);
        if ((currentFocus < 0) || (currentFocus >= g_locale_installed_num))
        {
            newFocus = 0;
        }
        else if (currentFocus == 0)
        {
            newFocus = nButtons - 1;
        }
        else
        {
            newFocus = currentFocus - 1;
        }
        
        button = ergtk_selection_group_get_button(selection, newFocus);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(button));
    }   
}

static void up_to_first_item(erGtkSelectionGroup* selection)
{
    gint nButtons = ergtk_selection_group_get_length(selection);
    if (nButtons > 0)
    {
        GtkToggleButton* button= NULL;
        button = ergtk_selection_group_get_button(selection, 0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(button));
    } 
}

static void down_one_item(erGtkSelectionGroup* selection)
{
    gint nButtons = ergtk_selection_group_get_length(selection);
    if (nButtons > 0)
    {
        int currentFocus = 0, newFocus = 0;
        GtkToggleButton* button= NULL;

        ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(selection), &currentFocus, 1);
        if ((currentFocus < 0) || (currentFocus >= g_locale_installed_num))
        {
            newFocus = 0;
        }
        else
        {
            newFocus = currentFocus + 1;
            if (newFocus >= nButtons)
            {
                newFocus = 0;
            }
        }

        button = ergtk_selection_group_get_button(selection, newFocus);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(button));
     }
}

static void down_to_last_item(erGtkSelectionGroup* selection)
{
    gint nButtons = ergtk_selection_group_get_length(selection);
    if (nButtons > 0)
    {
        GtkToggleButton* button= NULL;
        button = ergtk_selection_group_get_button(selection, nButtons-1);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(button));
    } 
}

static gboolean confirm_current_item(gpointer data)
{
    const gboolean save = (gboolean)data;
    
    erbusy_blink();
    if (save)
    {
        iLiad_language_data_store();
    }
    gtk_main_quit();

    return FALSE;
}
