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
 * \file connectScreen.c
 * \brief connectionMgr - the entry of 'connect-to-server' screen
 *                        containing a connectScreenOptions 
 *                               and a connectScreenProfiles
 *                        screen handling and event processing
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <unistd.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <liberipc/eripctoolbar.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "connectionMgr.h"
#include "background.h"
#include "connectScreen.h"
#include "connectScreenOptions.h"
#include "connectScreenProfiles.h"
#include "connectScreenData.h"
#include "connectScan.h"
#include "connectPing.h"
#include "connectBackground.h"
#include "pagebar.h"
#include "erbusy.h"
#include "toolbar.h"

// notebook containing connect-to-server screens
static GtkWidget *g_connect_notebook = NULL;

// signal handlers
static void on_connect_switch_page (GtkNotebook * notebook,
                                    GtkNotebookPage * new_page,
                                    guint page_num, 
                                    gpointer data);


static void connect_show_back(void);
static void connect_show_keyboard(void);
static void connect_show_trashcan(void);

// create the connect-to-server screens 
//
// object hierarchy:
//   |--alignment
//      |--g_connect_notebook (gtkNotebook)
//         |--> page (connect-screen-options)
//         |--> page (connect-screen profiles)
//
GtkWidget *connect_screen_create (void)
{
    CN_LOGPRINTF ("entry");

    GtkWidget *alignment;
    GtkWidget *page;
    
    // alignment
    alignment = gtk_alignment_new (0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 
            0, 0, SCREEN_BORDER, 0);
    gtk_widget_show(alignment); 

    // connect_notebook
    g_connect_notebook = gtk_notebook_new ();
    gtk_notebook_set_show_tabs (GTK_NOTEBOOK (g_connect_notebook), FALSE);
    gtk_notebook_set_show_border (GTK_NOTEBOOK (g_connect_notebook), FALSE);
    gtk_container_add (GTK_CONTAINER (alignment), g_connect_notebook);

    // page: connect-screen-options
    page = connect_options_create ();
    gtk_notebook_append_page(GTK_NOTEBOOK (g_connect_notebook), page, NULL);

    // page: connect-screen profiles
    page = connect_profiles_create ();
    gtk_notebook_append_page(GTK_NOTEBOOK(g_connect_notebook), page, NULL);

    // notebook pages start counting from zero
    gtk_notebook_set_current_page (GTK_NOTEBOOK(g_connect_notebook), 0);
    gtk_widget_show (g_connect_notebook);

    g_signal_connect_after(g_connect_notebook, "switch-page",  
                    G_CALLBACK (on_connect_switch_page), NULL);

    return alignment;
}

void connect_screen_display(void)
{
    int page;

    CN_LOGPRINTF ("entry");

    page = connect_get_current_page(NULL);
    if (page == connectScreenOptions_e)
    {
        connect_options_display();
    }
    else if (page == connectScreenProfiles_e)
    {
        connect_profiles_display();
    }
}

// set the default text
void connect_screen_set_text(void)
{
    CN_LOGPRINTF ("entry");

    connect_options_set_text();
    connect_profiles_set_text();
}

// update the page bar
void connect_set_pagebar (void)
{
    CN_LOGPRINTF ("entry");

    //gint current_page;
    //gint page_num;

    if (!main_get_background ())
    {
        // a little overkill, 
        // but this ensures the pageBar always has the correct currentApp
        pagebar_reset ();

        // For connect screen only show one page.
        // This forces the user to navigate using the on-screen items
        pagebar_set_pagecount( 1 );
        pagebar_goto_page( 1 );
        pagebar_redraw();
    }
}

void connect_set_toolbar(void)
{
    CN_LOGPRINTF("entry");

    connect_show_back();
    connect_show_keyboard();
    connect_show_trashcan();
}

static void connect_show_back(void)
{
    screenPage_t curPage, backPage;

    CN_LOGPRINTF("entry");

    curPage.screen = cmgrScreenConnect_e;
    curPage.page = connect_get_current_page(NULL);
    
    cmgr_get_back_page(&curPage, &backPage);

    if ((backPage.screen == cmgrScreenUndef_e)
        || (backPage.page == -1))
    {
        toolbar_setIconState(iconID_back, iconState_grey);
    }
    else
    {
        toolbar_setIconState(iconID_back, iconState_normal);
    }
}

static void connect_show_keyboard(void)
{
    CN_LOGPRINTF("entry");
    show_keyboard(FALSE);
}

static void connect_show_trashcan(void)
{
    CN_LOGPRINTF("entry");
    toolbar_setIconState(iconID_trashcan, iconState_grey);
}

// signal handlers
static void on_connect_switch_page (GtkNotebook * notebook, 
                                           GtkNotebookPage * new_page,
                                           guint page_num, 
                                           gpointer data)
{
    CN_LOGPRINTF ("Entry, new_page [%d]", (int) page_num);

    connect_set_pagebar();
    connect_set_toolbar();
}

gint connect_get_current_page (gint * pagecount)
{
    GtkNotebook *notebook = GTK_NOTEBOOK(g_connect_notebook);

    // get number of pages, when requested.
    if (pagecount != NULL)
    {
        *pagecount = gtk_notebook_get_n_pages(notebook);
    }

    // return current page number, counting from 0.
    return gtk_notebook_get_current_page(notebook);
}

// key-event received
gboolean on_connect_keypress (GdkEventKey * event)
{
    guint ret = 0; // return FALSE => default gtk handling
    
    CN_LOGPRINTF ("entry");
  
    switch (connect_get_current_page(NULL))
    {
        case connectScreenOptions_e:
            ret = on_connect_options_keypress(event);
            break;
        case connectScreenProfiles_e:
            ret = on_connect_profiles_keypress(event);
            break;
        default:
            CN_ERRORPRINTF ("-- case default");
            erbusy_off ();
            break;
    }

    return ret;
}

void connect_goto_page(connectScreen_t newpage)
{
    GtkNotebook *notebook;
    gint curpage, pagecount;

    curpage = connect_get_current_page(&pagecount);
    if (curpage != newpage)
    {
        if (newpage >= 0 && newpage < pagecount)
        {
            CN_LOGPRINTF ("curpage %d-->newpage %d, pagecount %d", 
                           curpage, newpage, pagecount);
            notebook = GTK_NOTEBOOK(g_connect_notebook);
            gtk_notebook_set_current_page(notebook, newpage);
        }
    }
}

void on_connect_goto_page (gint newpage)
{
    CN_LOGPRINTF("entry newpage[%d]", newpage);
    connect_goto_page(newpage - 1);
}

void on_connect_icon_clicked(int iconID, int iconState)
{
    int page;

    CN_LOGPRINTF("entry iconID[%d] iconState[%d]", iconID, iconState);

    switch (iconID)
    {
        case iconID_back:
            page = connect_get_current_page(NULL);
            if (page == connectScreenProfiles_e) 
            {
                on_connect_profiles_icon_clicked(iconID, iconState);
            }
            break;
        default:
            break;
    }
}

