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
 * \file gtkSortWnd.c
 * \brief gtkSortWnd - eReader content lister- Sort Settings 
 * 
 * <File description>
 *  gtkSortWnd provides GUI to let users choose their sorting type, sorting direction
 *  
 * Copyright (C) 2006-2008 iRex Technologies BV.
 * All rights reserved.
 *
 */

#define _GNU_SOURCE

// system include files
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <gtk/gtk.h>

// ereader library includes
#include <liberdm/display.h>
#include <liberdm/erdm.h>
#include <libergtk/ergtk.h>
#include <liberipc/eripctoolbar.h>
#include <libermanifest/ermanifest.h>

// local includes
#include "contentListerLog.h"
#include "lister.h"
#include "gtkSortWnd.h"
#include "displayUpdate.h"
#include "languages.h"

// id for selection buttons
enum
{
    eFileNameButton = 0,
    eFileExtButton,
    eFileSizeButton,
    eDateButton,
    eTitleButton,
    eDescButton,
    eButtonUndefined
};

// label text per button, must be synchronised with enum above
static const gchar *button_text_tbl[eButtonUndefined];

#define nAscendingButton  0     // index of ascendingButton
#define nDescendingButton 1     // index of descendingButton

// local data
static GtkSortWnd g_sort_wnd;

// local functions
static GtkWidget *create_sort_type_widget(void);
static GtkWidget *create_sort_order_widget(void);
static GtkWidget *create_sort_finish_widget(void);

static void on_sort_type_selection_update(erGtkSelectionGroup* selection, gpointer data);

// widget layout
// sortTypeSelection
// |--background
//    |--alignment
//       |--vbox1
//          |--sortLabel
//          |--vbox2 
//             |--vbox3_1
//                |--sortTypeLabel
//                |--hbox
//                   |--fileNameButton
//                   |--fileExtButton
//                   |--fileSizeButton
//                   |--dateButton
//             |--vbox3_2
//                |--sortTagLabel
//                |--hbox
//                   |--titleButton
//                   |--descButton
static GtkWidget *create_sort_type_widget(void)
{
    int        i;
    GtkWidget *sortTypeSelection;       // use it to combine buttons to be radio buttons 
    GtkWidget *background;
    GtkWidget *alignment;
    GtkWidget *vbox1;
    GtkWidget *sortLabel;
    GtkWidget *vbox2;
    GtkWidget *vbox3_1;
    GtkWidget *sortTypeLabel;
    GtkWidget *hbox1;
    GtkWidget *vbox3_2;
    GtkWidget *sortTagLabel;
    GtkWidget *hbox2;
    GtkWidget *button;
    GtkToggleButton *button_tbl[eButtonUndefined + 1];

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "sort_type_background");
    gtk_widget_set_size_request(background, SORT_WND_SORTTYPE_BK_WIDTH, SORT_WND_SROTTYPE_BK_HEIGHT);
    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              SORT_WND_TOP_PADDING, SORT_WND_BOTTOM_PADDING,
                              SORT_WND_LEFT_PADDING, SORT_WND_RIGHT_PADDING);
    gtk_container_add(GTK_CONTAINER(background), alignment);
    // vbox1
    vbox1 = gtk_vbox_new(FALSE, SORT_WND_SPACING_V * 3);
    gtk_container_add(GTK_CONTAINER(alignment), vbox1);
    // sortLabel
    sortLabel = gtk_label_new("");
    gtk_widget_set_name(sortLabel, "sort_wnd_text_bold");
    gtk_misc_set_alignment(GTK_MISC(sortLabel), 0.0, 0.5);
    gtk_label_set_ellipsize(GTK_LABEL(sortLabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_justify(GTK_LABEL(sortLabel), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox1), sortLabel, FALSE, FALSE, 0);
    // vbox2
    vbox2 = gtk_vbox_new(FALSE, SORT_WND_SPACING_V * 3);
    gtk_box_pack_start(GTK_BOX(vbox1), vbox2, FALSE, FALSE, 0);
    // vbox3_1
    vbox3_1 = gtk_vbox_new(FALSE, SORT_WND_SPACING_V * 2);
    gtk_box_pack_start(GTK_BOX(vbox2), vbox3_1, FALSE, FALSE, 0);
    // sortTypeLabel
    sortTypeLabel = gtk_label_new("");
    gtk_widget_set_name(sortTypeLabel, "sort_wnd_text_normal");
    gtk_misc_set_alignment(GTK_MISC(sortTypeLabel), 0.0, 0.5);
    gtk_label_set_ellipsize(GTK_LABEL(sortTypeLabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_justify(GTK_LABEL(sortTypeLabel), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox3_1), sortTypeLabel, FALSE, FALSE, 0);
    // hbox1
    hbox1 = gtk_hbox_new(FALSE, SORT_WND_SPACING_H);
    gtk_box_pack_start(GTK_BOX(vbox3_1), hbox1, FALSE, FALSE, 0);
    // vbox3_2
    vbox3_2 = gtk_vbox_new(FALSE, SORT_WND_SPACING_V * 2);
    gtk_box_pack_start(GTK_BOX(vbox2), vbox3_2, FALSE, FALSE, 0);
    // sortTagLabel
    sortTagLabel = gtk_label_new("");
    gtk_widget_set_name(sortTagLabel, "sort_wnd_text_normal");
    gtk_misc_set_alignment(GTK_MISC(sortTagLabel), 0.0, 0.5);
    gtk_label_set_ellipsize(GTK_LABEL(sortTagLabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_justify(GTK_LABEL(sortTagLabel), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox3_2), sortTagLabel, FALSE, FALSE, 0);
    // hbox2
    hbox2 = gtk_hbox_new(FALSE, SORT_WND_SPACING_H);
    gtk_box_pack_start(GTK_BOX(vbox3_2), hbox2, FALSE, FALSE, 0);
    // create buttons
    for (i = 0 ; i < eButtonUndefined ; i++)
    {
        button = ergtk_toggle_button_new_with_label("");
        button_tbl[i] = GTK_TOGGLE_BUTTON(button);
        gtk_widget_set_size_request(button, SORT_WND_BUTTON_WIDTH, SORT_WND_BUTTON_HEIGHT);
        switch (i)
        {
            case eFileNameButton:
            case eFileExtButton:
            case eFileSizeButton:
            case eDateButton:
                gtk_box_pack_start(GTK_BOX(hbox1), button, FALSE, FALSE, 0);
                break;
            case eTitleButton:
            case eDescButton:
                gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 0);
                break;
            default:
                break;
        }
    }
    button_tbl[eButtonUndefined] = NULL;
    // add buttons to selection group
    sortTypeSelection = ergtk_selection_group_new(button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(sortTypeSelection), 1, 1);
    gtk_container_add(GTK_CONTAINER(sortTypeSelection), background);

    // show widgets
    gtk_widget_show_all(sortTypeSelection);
    // set global values
    g_sort_wnd.sortLabel = sortLabel;
    g_sort_wnd.sortTypeLabel = sortTypeLabel;
    g_sort_wnd.sortTypeSelection = sortTypeSelection;
    g_sort_wnd.sortTagLabel = sortTagLabel;
    // signals functions
    g_signal_connect(G_OBJECT(sortTypeSelection), "selection_update", G_CALLBACK(on_sort_type_selection_update), NULL);

    // return
    return sortTypeSelection;
}

// widget layout
// sortOrderSelection
// |--background
//    |--alignment
//       |--vbox
//          |--alignment_1
//             |--sortOrderLabel
//          |--hbox
//             |--selLabel
//             |--selSortLabel
//                |--hbox_1
//                   |--ascendingButton
//                   |--descendingButton
//
static GtkWidget *create_sort_order_widget(void)
{
    GtkWidget *sortOrderSelection; 
    GtkWidget *background;
    GtkWidget *alignment;
    GtkWidget *vbox;
    GtkWidget *alignment_1;
    GtkWidget *sortOrderLabel; 
    GtkWidget *hbox;
    GtkWidget *selLabel;      
    GtkWidget *selSortLabel;
    GtkWidget *hbox_1;
    GtkWidget *ascendingButton;
    GtkWidget *descendingButton;
    GtkToggleButton *button_tbl[3];

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "sort_order_background");
    gtk_widget_set_size_request(background, SORT_WND_SORTORDER_BK_WIDTH, SORT_WND_SORTORDER_BK_HEIGHT);
    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              SORT_WND_TOP_PADDING, SORT_WND_BOTTOM_PADDING,
                              SORT_WND_LEFT_PADDING, SORT_WND_RIGHT_PADDING);
    gtk_container_add(GTK_CONTAINER(background), alignment);
    // vbox
    vbox = gtk_vbox_new(FALSE, SORT_WND_SPACING_V * 3);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    // alignment_1
    alignment_1 = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), alignment_1, FALSE, FALSE, 0);
    // sortOrderLabel
    sortOrderLabel = gtk_label_new("");
    gtk_widget_set_name(sortOrderLabel, "sort_wnd_text_bold");
    gtk_misc_set_alignment(GTK_MISC(sortOrderLabel), 0.0, 0.5);
    gtk_widget_set_size_request(sortOrderLabel,
                                SORT_WND_SORTORDER_BK_WIDTH - SORT_WND_LEFT_PADDING - SORT_WND_RIGHT_PADDING, -1);
    gtk_label_set_ellipsize(GTK_LABEL(sortOrderLabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_justify(GTK_LABEL(sortOrderLabel), GTK_JUSTIFY_LEFT);
    gtk_container_add(GTK_CONTAINER(alignment_1), sortOrderLabel);
    // hbox
    hbox = gtk_hbox_new(FALSE, SORT_WND_SPACING_H);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    // selLabel
    selLabel = gtk_label_new("");
    gtk_widget_set_name(selLabel, "sort_wnd_text_normal");
    gtk_misc_set_alignment(GTK_MISC(selLabel), 0.0, 0.5);
    gtk_widget_set_size_request(selLabel, SORT_WND_BUTTON_WIDTH - 20, -1);
    gtk_label_set_ellipsize(GTK_LABEL(selLabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_justify(GTK_LABEL(selLabel), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(hbox), selLabel, FALSE, FALSE, 0);
    // selSortLabel
    selSortLabel = gtk_label_new("");
    gtk_widget_set_name(selSortLabel, "sort_wnd_text_bold");
    gtk_widget_set_size_request(selSortLabel, SORT_WND_BUTTON_WIDTH + 40, -1);
    gtk_misc_set_alignment(GTK_MISC(selSortLabel), 0.0, 0.5);
    gtk_label_set_ellipsize(GTK_LABEL(selSortLabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_justify(GTK_LABEL(selSortLabel), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(hbox), selSortLabel, FALSE, FALSE, 0);
    // hbox_1
    hbox_1 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), hbox_1, FALSE, FALSE, 25);
    // ascendingButton
    ascendingButton = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(ascendingButton, ASCENDING_BTN_WIDTH, ASCENDING_BTN_HEIGHT);
    gtk_widget_set_name(ascendingButton, "sort_order_ascending_button");
    gtk_box_pack_start(GTK_BOX(hbox_1), ascendingButton, FALSE, FALSE, 0);
    // descendingButton
    descendingButton = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(descendingButton, ASCENDING_BTN_WIDTH, ASCENDING_BTN_HEIGHT);
    gtk_widget_set_name(descendingButton, "sort_order_descending_button");
    gtk_box_pack_start(GTK_BOX(hbox_1), descendingButton, FALSE, FALSE, 0);
    // 
    button_tbl[0] = GTK_TOGGLE_BUTTON(ascendingButton);
    button_tbl[1] = GTK_TOGGLE_BUTTON(descendingButton);
    button_tbl[2] = NULL;
    // add buttons to selection group
    sortOrderSelection = ergtk_selection_group_new(button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(sortOrderSelection), 1, 1);
    gtk_container_add(GTK_CONTAINER(sortOrderSelection), background);

    // show widgets
    gtk_widget_show_all(background);

    // set global values
    g_sort_wnd.sortOrderLabel = sortOrderLabel;
    g_sort_wnd.selLabel = selLabel;
    g_sort_wnd.selSortLabel = selSortLabel;
    g_sort_wnd.sortOrderSelection = sortOrderSelection;

    // return
    return sortOrderSelection;
}

//
// widget layout
// background
//  |--alignment
//     |--finishLabel
//
static GtkWidget *create_sort_finish_widget(void)
{
    GtkWidget *background;
    GtkWidget *alignment;
    GtkWidget *finishLabel;
    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "sort_finish_background");
    gtk_widget_set_size_request(background, SORT_WND_FINISH_BK_WIDTH, SORT_WND_FINISH_BK_HEIGHT);
    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              SORT_WND_TOP_PADDING, SORT_WND_BOTTOM_PADDING,
                              SORT_WND_LEFT_PADDING, SORT_WND_RIGHT_PADDING);
    gtk_container_add(GTK_CONTAINER(background), alignment);
    // finishLabel
    finishLabel = gtk_label_new("");
    gtk_widget_set_name(finishLabel, "sort_wnd_text_bold");
    gtk_misc_set_alignment(GTK_MISC(finishLabel), 0.0, 0.5);
    gtk_widget_set_size_request(finishLabel,
                                (SORT_WND_FINISH_BK_WIDTH - SORT_WND_LEFT_PADDING - SORT_WND_RIGHT_PADDING),
                                (SORT_WND_FINISH_BK_HEIGHT - SORT_WND_TOP_PADDING - SORT_WND_BOTTOM_PADDING));
    gtk_label_set_ellipsize(GTK_LABEL(finishLabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_justify(GTK_LABEL(finishLabel), GTK_JUSTIFY_LEFT);
    gtk_container_add(GTK_CONTAINER(alignment), finishLabel);

    // global variables
    g_sort_wnd.finishLabel = finishLabel;
    
    // show widgets
    gtk_widget_show_all(background);
    
    // return
    return background;
}

static gboolean on_sort_wnd_expose(GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
    CL_LOGPRINTF("entry");
    display_update_request_screen_refresh(LISTER_EXPOSE_LEVEL);

    return FALSE;
}

// widget layout
// parent
// |-alignment
//   |--vbox
//      |--sortTypeWidget
//      |--sortOrderWidget
//      |--sortFinishWidget
//
GtkWidget *create_sort_wnd(GtkWidget * parent)
{
    GtkWidget *background;
    GtkWidget *alignment;
    GtkWidget *vbox;
    GtkWidget *sortTypeWidget;
    GtkWidget *sortOrderWidget;
    GtkWidget *sortFinishWidget;

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(background), "sort_wnd_background");
    gtk_widget_set_size_request(GTK_WIDGET(background), (-1), (-1));
    gtk_container_add(GTK_CONTAINER(parent), background);
    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), 0, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(background), alignment);
    // vbox
    vbox = gtk_vbox_new(FALSE, SORT_WND_SPACING_V);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    // sortTypeWidget
    sortTypeWidget = create_sort_type_widget();
    gtk_box_pack_start(GTK_BOX(vbox), sortTypeWidget, FALSE, FALSE, 0);
    // sortOrderWidget
    sortOrderWidget = create_sort_order_widget();
    gtk_box_pack_start(GTK_BOX(vbox), sortOrderWidget, FALSE, FALSE, 0);
    // sortFinishWidget
    sortFinishWidget = create_sort_finish_widget();
    gtk_box_pack_start(GTK_BOX(vbox), sortFinishWidget, FALSE, FALSE, 0);

    // global variables
    g_sort_wnd.window = parent;

    // show widgets
    gtk_widget_show_all(background);

    // signal
    g_signal_connect(G_OBJECT(g_sort_wnd.window), "expose-event", G_CALLBACK(on_sort_wnd_expose), NULL);

    // return 
    return background;
}

// Note: when changing languages settings, we must call this function again.
void sort_wnd_set_text(void)
{
    int              i;
    GtkToggleButton* button;

    // set names for sort options
    for (i = 0 ; i < eButtonUndefined ; i++)
    {
        switch (i)
        {
            case eFileNameButton:
                button_text_tbl[i] = _("File name");
                break;
            case eFileExtButton :
                button_text_tbl[i] = _("File extension");
                break;
            case eFileSizeButton:
                button_text_tbl[i] = _("File size");
                break;
            case eDateButton:
                button_text_tbl[i] = _("File date");
                break;
            case eTitleButton:
                button_text_tbl[i] = _("Title");
                break;
            case eDescButton:
                button_text_tbl[i] = _("Description");
                break;
            default:
                button_text_tbl[i] = "?";
        }
    }
    
    // sortTypeWidget
    if (g_sort_wnd.sortLabel)
    {
        gtk_label_set_text(GTK_LABEL(g_sort_wnd.sortLabel), _("Step 1: Please select a sort criteria"));
    }
    if (g_sort_wnd.sortTypeLabel)
    {
        gtk_label_set_text(GTK_LABEL(g_sort_wnd.sortTypeLabel), _("Sort content on file system attributes"));
    }
    if (g_sort_wnd.sortTypeSelection)
    {
        for (i = 0 ; i < eButtonUndefined ; i++)
        {
            button = ergtk_selection_group_get_button(ERGTK_SELECTION_GROUP(g_sort_wnd.sortTypeSelection), i);
            gtk_button_set_label(GTK_BUTTON(button), button_text_tbl[i]);
        }
    }
    if (g_sort_wnd.sortTagLabel)
    {
        gtk_label_set_text(GTK_LABEL(g_sort_wnd.sortTagLabel), _("Sort content on information tags"));
    }
   
    // sortOrderWidget
    if (g_sort_wnd.sortOrderLabel)
    {
        gtk_label_set_text(GTK_LABEL(g_sort_wnd.sortOrderLabel), _("Step 2: Please select ascending or descending"));
    }
    if (g_sort_wnd.selLabel)
    {
        gtk_label_set_text(GTK_LABEL(g_sort_wnd.selLabel), _("Selection"));
    }

    // sortFinishWidget
    if (g_sort_wnd.finishLabel)
    {
        gtk_label_set_text(GTK_LABEL(g_sort_wnd.finishLabel),
                           _("Step 3: Please click on the 'Sort' icon to confirm your selection"));
    }
}

void sort_wnd_get_sort_settings(mdsSortStruct_t * sortStruct)
{
    g_return_if_fail(sortStruct != NULL);
    g_return_if_fail(g_sort_wnd.sortTypeSelection != NULL);
    g_return_if_fail(g_sort_wnd.sortOrderSelection != NULL);

    gint nSelSortType;
    gint nSelSortOrder;

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(g_sort_wnd.sortTypeSelection),  &nSelSortType,  1);
    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(g_sort_wnd.sortOrderSelection), &nSelSortOrder, 1);

    switch (nSelSortType)
    {
        case eFileNameButton:
            sortStruct->ft = mdsFieldFsName;
            break;
        case eFileExtButton:
            sortStruct->ft = mdsFieldExtName;
            break;
        case eFileSizeButton:
            sortStruct->ft = mdsFieldSize;
            break;
        case eDateButton:
            sortStruct->ft = mdsFieldDate;
            break;
        case eTitleButton:
            sortStruct->ft = mdsFieldTitle;
            break;
        case eDescButton:
            sortStruct->ft = mdsFieldDescription;
            break;
        default:
            CL_WARNPRINTF("invalid nSelSortType [%d]", nSelSortType);
            sortStruct->ft = mdsFieldFsName;
            break;
    }
   
    switch (nSelSortOrder)
    {
        case nAscendingButton:
            sortStruct->ascending = 1;
            break;
        case nDescendingButton:
            sortStruct->ascending = 0;
            break;
        default:
            CL_WARNPRINTF("invalid nSelSortOrder [%d]", nSelSortOrder);
            sortStruct->ascending = 1;
            break;
    }
    CL_LOGPRINTF("return: sortStruct->ft = %d, sortStruct->ascending = %d", sortStruct->ft, sortStruct->ascending);
}

void sort_wnd_set_sort_settings(const mdsSortStruct_t * sortStruct)
{
    g_return_if_fail(sortStruct != NULL);
    g_return_if_fail(g_sort_wnd.sortTypeSelection != NULL);
    g_return_if_fail(g_sort_wnd.selSortLabel != NULL);
    g_return_if_fail(g_sort_wnd.sortOrderSelection != NULL);

    GtkToggleButton* button = NULL;

    gint nSelSortType  = eFileNameButton; 
    gint nSelSortOrder = nAscendingButton;

    switch (sortStruct->ft)
    {
        case mdsFieldFsName: 
            nSelSortType = eFileNameButton;
            break;
        case mdsFieldExtName:
            nSelSortType = eFileExtButton;
            break;
        case mdsFieldSize:
            nSelSortType = eFileSizeButton;
            break;
        case mdsFieldDate:
            nSelSortType = eDateButton;
            break;
        case mdsFieldTitle:
            nSelSortType = eTitleButton;
            break;
        case mdsFieldDescription:
            nSelSortType = eDescButton;
            break;
        default:
            nSelSortType = eFileNameButton;
            break;
    }
    switch (sortStruct->ascending)
    {
        case 1:
            nSelSortOrder = nAscendingButton;
            break;
        case 0:
            nSelSortOrder = nDescendingButton;
            break;
        default:
            nSelSortOrder = nAscendingButton;
            break;
    }

    if (g_sort_wnd.selSortLabel)
    {
        gtk_label_set_text(GTK_LABEL(g_sort_wnd.selSortLabel), _( button_text_tbl[nSelSortType] ));
    }

    button = ergtk_selection_group_get_button(ERGTK_SELECTION_GROUP(g_sort_wnd.sortTypeSelection), nSelSortType);
    if (button)
    {
        gtk_toggle_button_set_active(button, TRUE);
    }

    button = ergtk_selection_group_get_button(ERGTK_SELECTION_GROUP(g_sort_wnd.sortOrderSelection), nSelSortOrder);
    if (button)
    {
        gtk_toggle_button_set_active(button, TRUE);
    }
}

static void on_sort_type_selection_update(erGtkSelectionGroup* item, gpointer data)
{
    gint         button_id;
    const gchar* label_text = "";
    
    ergtk_selection_group_get_selected_buttons(item, &button_id, 1);

    switch (button_id)
    {
        case eFileNameButton:
        case eFileExtButton:
        case eFileSizeButton:
        case eDateButton:
        case eTitleButton:
        case eDescButton:
            label_text = _( button_text_tbl[button_id] );
            break;
        default:
            CL_WARNPRINTF("invalid button_id [%d]", button_id);
            break;
    }

    if (g_sort_wnd.selSortLabel)
    {
        gtk_label_set_text(GTK_LABEL(g_sort_wnd.selSortLabel), label_text);
    }
}

