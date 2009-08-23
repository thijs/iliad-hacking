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
* \file gtkSearchWnd.c
* \brief gtkSearchWnd - eReader search options window
* 
* <File description>
*  The eReader content metadata editor provides gtk widgets to enable user change 
*  content title and description. Which are created and updated with the following API
*/

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2006-2008 iRex Technologies
 * All rights reserved.
 *
 */

#define _GNU_SOURCE

// system includes
#include <string.h>
#include <sys/types.h>

// gtk includes
#include <gdk/gdkkeysyms.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtk.h>

// ereader library includs
#include <liberdm/display.h>
#include <liberdm/erdm.h>
#include <liberipc/eripctoolbar.h>
#include <libermanifest/ermanifest.h>
#include <libergtk/erGtkEntry.h>
#include <libergtk/erGtkToggleButton.h>

// local includes
#include "contentListerLog.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "displayUpdate.h"
#include "erbusy.h"
#include "erMdsContent.h"
#include "gtkSearchWnd.h"
#include "languages.h"
#include "pagebar.h"
#include "search_files.h"
#include "toolbar.h"


// local data
static swSearch_t  g_wnd;

// local functions
static gboolean delayed_on_searchbutton_toggled(gpointer data);


//////////////////////////////////////////////////////////////////////////
// event handlers
//////////////////////////////////////////////////////////////////////////

static void on_changed(GtkEditable * widget, gpointer data)
{
    CL_SEARCHPRINTF("entry");

    // check for illegal characters in search pattern
    const char* illegal_chars = ILLEGAL_FILENAME_CHARS;
    const char* pattern = gtk_entry_get_text(GTK_ENTRY(widget));
    //
    char* new_pattern = alloca( strlen(pattern) + 1 );
    strcpy(new_pattern, pattern);
    //
    gboolean found = FALSE;
    char* cp = new_pattern;
    while (*cp)
    {
        if ( strchr(illegal_chars, *cp) )
        {
            *cp = '-';
            found = TRUE;
        }
        cp++;
    }
    if (found)
    {
        CL_SEARCHPRINTF("illegal pattern [%s] new pattern [%s]", pattern, new_pattern);
        gtk_entry_set_text(GTK_ENTRY(widget), new_pattern);
    }
}

static gboolean on_expose(GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
    CL_SEARCHPRINTF("entry");

    gtk_widget_grab_focus(GTK_WIDGET(g_wnd.patternEntry));

    // select all characters, cursor at end of input field
    gtk_editable_select_region(GTK_EDITABLE(g_wnd.patternEntry), 0, -1);

    display_update_request_screen_refresh(LISTER_EXPOSE_LEVEL);

    return FALSE;
}


// search button selected
static void on_searchbutton_toggled_event(GtkToggleButton * button, gpointer ptr)
{
    CL_SEARCHPRINTF("entry");
    
    gboolean active = gtk_toggle_button_get_active(button);
    if (active)
    {
        erbusy_blink();

        // wait for button shown selected, then start seach
        g_timeout_add(200, delayed_on_searchbutton_toggled, button);
    }
}

static gboolean delayed_on_searchbutton_toggled(gpointer data)
{
    CL_SEARCHPRINTF("entry");

    GtkToggleButton* button = GTK_TOGGLE_BUTTON(data);
    const char*      pattern = "";
    
    // get search string
    if (g_wnd.patternEntry)
    {
        pattern = gtk_entry_get_text(GTK_ENTRY(g_wnd.patternEntry));
    }
    
    // get current location, start search
    char  root[ERMDS_MAX_FILENAME_SIZE] = "";
    const char* sCurrPath = ctrl_get_abscurr_location(root, sizeof(root));    
    execSearch(sCurrPath, pattern);

    // show the search result
    display_update_increase_level(LISTER_EXPOSE_LEVEL);
    gtk_toggle_button_set_active(button, FALSE);
    ctrl_hide_search_wnd();
    ctrl_display_search_result();

    return FALSE;  // don't call me anymore
}

//////////////////////////////////////////////////////////////////////////
// helper construction function
//////////////////////////////////////////////////////////////////////////
// object hierarchy
//     parent
//       |-- g_wnd.patternTag (gtk_label)
//       |-- g_wnd.patternEntry (gtk_entry)
//       |-- g_wnd.patternExpl (gtk_label)
//
static void createPatternItem(GtkWidget* parent)
{
    GtkWidget* widget;

    CL_SEARCHPRINTF("entry");    

    // pattern tag
    widget = gtk_label_new("");
    gtk_widget_set_name(GTK_WIDGET(widget), "search_wnd_text_bold");
    gtk_widget_set_size_request(GTK_WIDGET(widget), SEARCH_ITEM_WIDTH, SEARCH_TAG_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(widget), 0, 1.0);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_wnd.patternTag = widget;

    // patternEdit
    widget = ergtk_entry_new();
    gtk_widget_set_size_request(widget, SEARCH_ITEM_WIDTH, SEARCH_NAME_HEIGHT);
    g_signal_connect_after(G_OBJECT(widget), "changed-stable", G_CALLBACK(on_changed), NULL);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);    
    gtk_widget_show(widget);
    g_wnd.patternEntry = widget;

    // pattern explanation
    widget = gtk_label_new("");
    gtk_widget_set_name(GTK_WIDGET(widget), "search_wnd_text");
    gtk_widget_set_size_request(GTK_WIDGET(widget), SEARCH_ITEM_WIDTH, SEARCH_NAME_EXPL_HEIGHT);
    gtk_label_set_single_line_mode(GTK_LABEL(widget), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(widget), TRUE);
    gtk_misc_set_alignment(GTK_MISC(widget), 0, 0.0);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_wnd.patternExpl = widget;
}

// object hierarchy:
//     parent
//       |-- g_wnd.searchTag (gtk_label)
//       |-- alignment
//             |-- g_wnd.searchButton (gtk_toggle_button)
//
static void createSearchButtonItem(GtkWidget* parent)
{
    GtkWidget* widget;
    GtkWidget* alignment;

    CL_SEARCHPRINTF("entry");

    // pattern tag
    widget = gtk_label_new("");
    gtk_widget_set_name(GTK_WIDGET(widget), "search_wnd_text_bold");
    gtk_widget_set_size_request(GTK_WIDGET(widget), SEARCH_ITEM_WIDTH, SEARCH_TAG_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(widget), 0, 1.0);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_wnd.searchTag = widget;

    // alignment
    widget = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
    gtk_widget_set_size_request(GTK_WIDGET(widget), -1, SEARCH_BUTTON_AREA_HEIGHT);
    gtk_container_add(GTK_CONTAINER(parent), widget);
    gtk_widget_show(widget);
    alignment = widget;

    // button    
    widget = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(widget), SEARCH_BUTTON_WIDTH, SEARCH_BUTTON_HEIGHT);
    g_signal_connect(G_OBJECT(widget), "toggled", G_CALLBACK(on_searchbutton_toggled_event), NULL);
    gtk_container_add(GTK_CONTAINER(alignment), widget);
    gtk_widget_show(widget);
    g_wnd.searchButton = widget;
}

// object hierarchy
//     parent
//       |-- background (event-box)
//             |-- alignment
//                   |-- vbox
//                         |-- pattern item
//                         |-- button item
//
GtkWidget* create_search_wnd(GtkWidget* parent)
{
    CL_SEARCHPRINTF("entry");

    // main window
    g_wnd.window = parent;
    g_signal_connect(G_OBJECT(g_wnd.window), "expose-event", G_CALLBACK(on_expose), NULL);

    // background
    GtkWidget* background = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(background), "search_wnd_background");
    gtk_widget_set_size_request(GTK_WIDGET(background), SEARCH_BK_IMG_WIDTH, SEARCH_BK_IMG_HEIGHT);
    gtk_container_add(GTK_CONTAINER(parent), background);
    gtk_widget_show(background);
        
    // alignment
    GtkWidget* alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding( GTK_ALIGNMENT(alignment), 
                               SEARCH_VBORDER, 
                               SEARCH_VBORDER, 
                               SEARCH_HBORDER, 
                               SEARCH_HBORDER );
    gtk_container_add(GTK_CONTAINER(background), alignment);
    gtk_widget_show(alignment);

    // vbox
    GtkWidget*vbox = gtk_vbox_new(FALSE, SEARCH_WND_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    gtk_widget_show(vbox);
   
    // pattern item
    createPatternItem(vbox);

    // search item
    createSearchButtonItem(vbox);

    return background;
}

// set fixed screen texts
void search_wnd_set_text()
{
    CL_SEARCHPRINTF("entry");

    // pattern item
    gtk_label_set_text( GTK_LABEL(g_wnd.patternTag),  _("Step 1: Please enter the search pattern") );
    gtk_label_set_text( GTK_LABEL(g_wnd.patternExpl), _("Multiple search strings can be separated with a space.\n"
                                                        "Search strings containing spaces should be enclosed in double quotes (\").\n"
                                                        "Narrow your search results by clicking the 'Search' icon again.") );

    // search item
    gtk_label_set_text( GTK_LABEL(g_wnd.searchTag), _("Step 2: Please click on the 'Search' button to start your enquiry") );
    gtk_button_set_label( GTK_BUTTON(g_wnd.searchButton), _("Search") );
}

