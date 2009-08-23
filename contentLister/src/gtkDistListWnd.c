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
 * \file distListWnd.c
 * \brief distListWnd 
 * 
 *  distListWnd provides GUI to let users choose the persons who users want to send to 
 *  
 * Copyright (C) 2007-2008 iRex Technologies B.V.
 * All rights reserved.
 */


// system includes
#include <gtk/gtk.h>

// ereader library includes
#include <liberdm/display.h>
#include <libergtk/ergtk.h>
#include <libermanifest/ermanifest.h>

// local includes
#include "contentListerLog.h"
#include "displayUpdate.h"
#include "erbusy.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "erMdsContent.h"
#include "gtkDistListWnd.h"
#include "pagebar.h"

#include "languages.h"


// local data
static GtkWidget* g_parent_widget   = NULL;
static GtkWidget* g_notebook        = NULL;
static GtkWidget* g_lastpage_vbox   = NULL;
static guint      g_lastpage_height = 0;

// local functions
static void dist_list_wnd_create_notebook(void);
static void dist_list_wnd_notebook_append_page(void);
static void dist_list_wnd_update_pagebar(void);

// signal handlers
static void on_switch_page(GtkNotebook *notebook, GtkNotebookPage *new_page, guint page_num, gpointer data);
static void on_selection_update(erGtkSelectionGroup *item, gpointer user_data);


// public functions
void create_dist_list_wnd(GtkWidget *parent)
{
    CL_LOGPRINTF("entry");

    g_parent_widget = parent;
    dist_list_wnd_create_notebook();
}

void dist_list_wnd_clear(void)
{
    CL_LOGPRINTF("entry: g_notebook [%p]", g_notebook);

    if (g_notebook)
    {
        gtk_widget_destroy(g_notebook);
        dist_list_wnd_create_notebook();
    }
}

erGtkSelectionGroup* dist_list_wnd_add_list(const gchar*  display_as,
                                            const gchar*  title,
                                            const gchar*  instruction,
                                            const gchar** item_tbl)
{
    CL_LOGPRINTF("entry: title [%s]", title);

    GtkWidget*    list          = NULL;
    GtkWidget*    master_list   = NULL;
    GtkWidget*    lastpage_vbox = g_lastpage_vbox;
    guint         list_height   = 0;
    guint         list_items    = 0;
    guint         next_item     = 0;
    guint         available_height;

    // determine display_as for selection list
    erGtkSelectionList_displayAs_e list_display_as;
    if (strcmp(display_as, "radiobutton") == 0)
    {
        list_display_as = esl_RadioButton;
    }
    else if (strcmp(display_as, "textbutton") == 0)
    {
        list_display_as = esl_TextButton;
    }
    else
    {
        list_display_as = esl_Checklist;
    }

    // calculate space available for selection list
    if (lastpage_vbox == NULL)
    {
        available_height = DIST_LIST_WND_HEIGHT_SMALL;
    }
    else
    {
        if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(g_notebook)) <= 1)
        {
            available_height = DIST_LIST_WND_HEIGHT_SMALL;
        }
        else
        {
            available_height = DIST_LIST_WND_HEIGHT_BIG;
        }
        available_height = available_height - g_lastpage_height - DIST_LIST_WND_V_SPACING;
    }

    // create selection list
    list = ergtk_selection_list_new( list_display_as,
                                     title,
                                     instruction,
                                     item_tbl,
                                     available_height,
                                     &list_items,
                                     &list_height );
    if (list == NULL)
    {
        // list cannot start on current page
        lastpage_vbox    = NULL;
        available_height = DIST_LIST_WND_HEIGHT_BIG;
        list = ergtk_selection_list_new( list_display_as,
                                         title,
                                         instruction,
                                         item_tbl,
                                         available_height,
                                         &list_items,
                                         &list_height );
    }

    // add list to notebook, continue on next pages when needed
    master_list = list;
    while (list)
    {
        g_signal_connect(G_OBJECT(list), "selection-update", G_CALLBACK(on_selection_update), NULL);
        
        // add selection list to notebook
        if (lastpage_vbox == NULL)
        {
            dist_list_wnd_notebook_append_page();
            lastpage_vbox = g_lastpage_vbox;

            g_lastpage_height += list_height;
        }
        else
        {
            g_lastpage_height += DIST_LIST_WND_V_SPACING + list_height;
        }
        gtk_box_pack_start(GTK_BOX(lastpage_vbox), list, FALSE, FALSE, 0);
        next_item += list_items;

        // create additional selection list, if needed
        if (item_tbl[next_item] == NULL)
        {
            // all items done, no continuation list needed
            list = NULL;
        }
        else
        {
            // item(s) left, start continuation list on new page
            lastpage_vbox    = NULL;
            available_height = DIST_LIST_WND_HEIGHT_BIG;
            
            list = ergtk_selection_list_new_from_master( ERGTK_SELECTION_LIST(master_list),
                                                         next_item,
                                                         available_height,
                                                         &list_items,
                                                         &list_height );
        }
    }

    return ERGTK_SELECTION_GROUP(master_list);
}

void dist_list_wnd_page_forward(guint offset)
{
    gint last_page;
    gint old_page;
    gint new_page;

    if (g_notebook)
    {
        last_page = gtk_notebook_get_n_pages(GTK_NOTEBOOK(g_notebook)) - 1;
        old_page  = gtk_notebook_get_current_page(GTK_NOTEBOOK(g_notebook));
        if (old_page < last_page)
        {
            new_page = old_page + offset;
            if (new_page > last_page)
            {
                new_page = last_page;
            }
            gtk_notebook_set_current_page(GTK_NOTEBOOK(g_notebook), new_page);
        }
        else
        {
            erbusy_off();
        }
    }
}

void dist_list_wnd_page_back(guint offset)
{
    gint first_page = 0;
    gint old_page;
    gint new_page;

    if (g_notebook)
    {
        old_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(g_notebook));
        if (old_page > first_page)
        {
            new_page = old_page - offset;
            if (new_page < first_page)
            {
                new_page = first_page;
            }
            gtk_notebook_set_current_page(GTK_NOTEBOOK(g_notebook), new_page);
        }
        else
        {
            erbusy_off();
        }
    }
}

gboolean dist_list_wnd_is_visible(void)
{
    if (g_parent_widget  &&  GTK_WIDGET_VISIBLE(g_parent_widget))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


// local functions
static void dist_list_wnd_create_notebook(void)
{
    GtkWidget *widget;

    if (g_parent_widget)
    {
        // g_parent_widget
        //   |-- g_notebook (GtkNotebook)
        //         |
        widget = gtk_notebook_new();
        gtk_widget_set_size_request(widget, DIST_LIST_WND_WIDTH, -1);
        gtk_notebook_set_show_tabs(GTK_NOTEBOOK(widget), FALSE);
        gtk_notebook_set_show_border(GTK_NOTEBOOK(widget), FALSE);
        g_signal_connect_after(widget, "switch-page",   G_CALLBACK(on_switch_page), NULL);
        g_signal_connect_after(widget, "destroy-event", G_CALLBACK(gtk_widget_destroyed), &g_notebook);
        gtk_container_add(GTK_CONTAINER(g_parent_widget), widget);
        gtk_widget_show(widget);
        g_notebook = widget;

        // no selection lists yet
        g_lastpage_vbox   = NULL;
        g_lastpage_height = 0;
    }
}

static void dist_list_wnd_notebook_append_page(void)
{
    GtkWidget *widget;

    // g_notebook (GtkNotebook)
    //   |-- g_lastpage_vbox (one for each page)
    //         |-- ...
    widget = gtk_vbox_new(FALSE, DIST_LIST_WND_V_SPACING);
    gtk_widget_show(widget);  // Note: MUST show widget before appending to notebook
    gtk_notebook_append_page(GTK_NOTEBOOK(g_notebook), widget, NULL);
    g_lastpage_vbox = widget;

    // no selection lists yet
    g_lastpage_height = 0;
}

void dist_list_wnd_goto_page(gint new_page)
{
    gint old_page;

    if (g_notebook)
    {
        old_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(g_notebook));
        new_page--;  // notebook counts 0 ..

        if (new_page != old_page)
        {
            gtk_notebook_set_current_page(GTK_NOTEBOOK(g_notebook), new_page);
        }
        else
        {
            dist_list_wnd_update_pagebar();
        }
    }
}

static void dist_list_wnd_update_pagebar()
{
    const gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(g_notebook));
    const gint num_pages    = gtk_notebook_get_n_pages(GTK_NOTEBOOK(g_notebook));

    CL_LOGPRINTF("entry: page [%d] num_pages [%d]", current_page, num_pages);

    pagebar_reset();   // send pageBar events to us
    pagebar_set_pagecount(num_pages);
    pagebar_goto_page(current_page + 1);  // pagebar counts from 1
    pagebar_redraw();
}


// signal handlers
static void on_switch_page(GtkNotebook *notebook, GtkNotebookPage *new_page, guint page_num, gpointer data)
{
    CL_LOGPRINTF("entry: new_page [%d]", (int)page_num);

    dist_list_wnd_update_pagebar();

    if (page_num == 0)
    {
        ctrl_show_lister_area();
    }
    else
    {
        ctrl_hide_lister_area();
    }

    display_update_request_screen_refresh(LISTER_EXPOSE_LEVEL);
}

static void on_selection_update(erGtkSelectionGroup *item, gpointer user_data)
{
    CL_LOGPRINTF("entry");

    display_update_request_screen_refresh(TEXTENTRY_CHANGED_LEVEL);
}

