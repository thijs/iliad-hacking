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
 * \file iLiadArcLocScreen.c
 * \brief settings - Archive Location Screen 
 *
 * Providing GUI interface to let users input their personalize archive location. 
 *
 */

#include <string.h>
#include <gtk/gtk.h>

#include <liberdm/display.h>
#include <libergtk/ergtk.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "gtkInfoItem.h"
#include "gtkSettingItem.h"
#include "iLiadArcLocData.h"
#include "iLiadArcLocScreen.h"
#include "settings.h"
#include "languages.h"

// global variables
static GtkWidget*  g_archive_settingitem = NULL;
static GtkWidget*  g_export_settingitem = NULL;
static gtkArcLoc_t g_archive_locs[arcTypeUndefined] = {
                                                        { arcNews  },
                                                        { arcBooks },
                                                        { arcDocs  },
                                                        { arcNotes },
                                                      };
static gtkArcLoc_t g_export_loc;
static GtkWidget*  g_info_item = NULL;

// internal functions
static GtkWidget *gtk_archive_location_new(gtkArcLoc_t *arcLoc);
static GtkWidget *iLiad_archive_location_create_buttons(gtkArcLoc_t *arcLoc);
static void on_selection_update(erGtkSelectionGroup * selection, gpointer button, gpointer data);

// widget hierarchy
// |--top_vbox
//    |--item (GtkSettingItem)
//       |--vbox
//          |--g_archive_locs[arcNews]
//          |--g_archive_locs[arcBooks]
//          |--g_archive_locs[arcDocs]
//          |--g_archive_locs[arcNotes]
//    |--item (GtkSettingItem)
//       |--g_archive_locs[arcExport]
//    |--g_info_item
GtkWidget *iLiad_archive_location_create(void)
{
    int        i;
    GtkWidget *item;
    GtkWidget *top_vbox;
    GtkWidget *vbox;
    GtkWidget *widget;

    ST_LOGPRINTF("entry");

    // Top level vbox
    top_vbox = gtk_vbox_new(FALSE, ARC_LOC_SCR_SPACING_V);

    // g_archive_settingitem
    item = gtk_settingitem_new("");
    gtk_widget_set_size_request(item, -1, -1);
    gtk_box_pack_start(GTK_BOX(top_vbox), item, FALSE, FALSE, 0);
    g_archive_settingitem = item;

    // vbox
    vbox = gtk_vbox_new(FALSE, SETTINGITEM_ITEM_SPACING);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), vbox);
    
    // g_archive_locs[..]
    for (i = 0 ; i < sizeof(g_archive_locs) / sizeof(g_archive_locs[0]) ; i++)
    {
        widget = gtk_archive_location_new(&g_archive_locs[i]);
        gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    }

    // g_export_settingitem
    item = gtk_settingitem_new("");
    gtk_widget_set_size_request(item, -1, -1);
    gtk_box_pack_start(GTK_BOX(top_vbox), item, FALSE, FALSE, 0);
    g_export_settingitem = item;

    widget = gtk_archive_location_new(&g_export_loc);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), widget);
    gtk_widget_show_all(widget);

    // g_info_item
    widget = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(top_vbox), widget, FALSE, FALSE, 0);
    g_info_item = widget;
   
    // show widgets
    gtk_widget_show_all(vbox);
    gtk_widget_show(g_archive_settingitem);
    gtk_widget_show(g_export_settingitem);
    gtk_widget_show(top_vbox);
    return top_vbox;
}

static GtkWidget *gtk_archive_location_new(gtkArcLoc_t *arcLoc)
{
    GtkWidget *background;
    GtkWidget *alignment;
    GtkWidget *vbox;
    GtkWidget *title;
    GtkWidget *subTitle;
    GtkWidget *widget;

    ST_LOGPRINTF("entry");
    g_return_val_if_fail(arcLoc, NULL);

    // widget hierarchy
    // ----------------
    //     background 
    //       |
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "archive_location_background");
    gtk_widget_set_size_request(background, ARC_LOC_ITEM_BK_W, ARC_LOC_ITEM_BK_H);
   //       |
    //       |-- alignment
    //             |-- vbox
    //                   |
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              ARC_LOC_SCR_ITEM_PADDING_T, ARC_LOC_SCR_ITEM_PADDING_B,
                              ARC_LOC_SCR_ITEM_PADDING_L, ARC_LOC_SCR_ITEM_PADDING_R);
    gtk_container_add(GTK_CONTAINER(background), alignment);
    //
    vbox = gtk_vbox_new(FALSE, ARC_LOC_SCR_SPACING_V);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    //                   |
    //                   |-- title
    title = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 0);
    gtk_widget_set_name(title, "archive_location_title");
    gtk_widget_set_size_request(title, ARC_LOC_ITEM_LABEL_W, (-1));
    gtk_label_set_single_line_mode(GTK_LABEL(title), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(title), FALSE);
    gtk_label_set_justify(GTK_LABEL(title), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(title), 0.0, 0.0);
    arcLoc->title = title;
    //                   |
    //                   |-- subTitle
    subTitle = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), subTitle, FALSE, FALSE, 0);
    gtk_widget_set_name(subTitle, "archive_location_info_text");
    gtk_widget_set_size_request(subTitle, ARC_LOC_ITEM_LABEL_W, (-1));
    gtk_label_set_single_line_mode(GTK_LABEL(subTitle), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(subTitle), TRUE);
    gtk_label_set_justify(GTK_LABEL(subTitle), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(subTitle), 0.0, 0.0);
    arcLoc->subTitle = subTitle;
    //                   |
    //                   |-- buttons
    widget = iLiad_archive_location_create_buttons(arcLoc);
    gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    
    // return
    return background;
}

static GtkWidget *iLiad_archive_location_create_buttons(gtkArcLoc_t *arcLoc)
{
    int i;
    GtkWidget *hbox;
    GtkWidget *button;
    GtkWidget *selection;
    GtkToggleButton *button_tbl[ARC_LOC_BTN_NUM + 1];

    ST_LOGPRINTF("entry");

    // widget hierarchy
    // ----------------
    //     hbox
    //       |-- xx_buttons[..]
    //
    hbox = gtk_hbox_new(FALSE, ARC_LOC_BTN_SPACING_H);
    for (i = 0; i < ARC_LOC_BTN_NUM; i++)
    {
        button = ergtk_toggle_button_new_with_label("");
        button_tbl[i] = GTK_TOGGLE_BUTTON(button);
        gtk_widget_set_size_request(GTK_WIDGET(button), ARC_LOC_BTN_W, ARC_LOC_BTN_H);
        gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(button), FALSE, FALSE, 0);

        arcLoc->buttons[i] = GTK_WIDGET(button);
    }
    button_tbl[i] = NULL;
    //
    // selection grout not part of widget hierarchy
    selection = ergtk_selection_group_new(button_tbl);
    gtk_widget_set_name(selection, "archive_location_background");
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(selection), 1, 1);
    g_signal_connect(G_OBJECT(selection), "selection-update", G_CALLBACK(on_selection_update), arcLoc);
    arcLoc->selection = selection;

    return hbox;
}

void iLiad_archive_location_set_text(void)
{
    int           i;
    gtkArcLoc_t  *arcLoc;
    GtkWidget   **button_tbl;

    ST_LOGPRINTF("entry");

    if (g_archive_settingitem)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_archive_settingitem), _("Archive locations"));
        // news
        arcLoc = &g_archive_locs[arcNews];
        gtk_label_set_text(GTK_LABEL(arcLoc->title),    _("Newspapers"));
        gtk_label_set_text(GTK_LABEL(arcLoc->subTitle), _("Select a memory type for the 'NEWS' button."));
        // books
        arcLoc = &g_archive_locs[arcBooks];
        gtk_label_set_text(GTK_LABEL(arcLoc->title),    _("Books"));
        gtk_label_set_text(GTK_LABEL(arcLoc->subTitle), _("Select a memory type for the 'BOOKS' button."));
        // docs
        arcLoc = &g_archive_locs[arcDocs];
        gtk_label_set_text(GTK_LABEL(arcLoc->title),    _("Documents"));
        gtk_label_set_text(GTK_LABEL(arcLoc->subTitle), _("Select a memory type for the 'DOCS' button."));
        // notes
        arcLoc = &g_archive_locs[arcNotes];
        gtk_label_set_text(GTK_LABEL(arcLoc->title),    _("Notes"));
        gtk_label_set_text(GTK_LABEL(arcLoc->subTitle), _("Select a memory type for the 'NOTES' button."));

        // button labels
        for (i = 0 ; i < sizeof(g_archive_locs) / sizeof(g_archive_locs[0]) ; i++)
        {
            button_tbl = g_archive_locs[i].buttons;
            gtk_button_set_label(GTK_BUTTON(button_tbl[arcLocMain]), _("Internal memory"));
            gtk_button_set_label(GTK_BUTTON(button_tbl[arcLocCard]), _("MMC card"       ));
            gtk_button_set_label(GTK_BUTTON(button_tbl[arcLocUSB ]), _("USB stick"      ));
            gtk_button_set_label(GTK_BUTTON(button_tbl[arcLocCF  ]), _("CF card"        ));
        }
    }

    if (g_export_settingitem)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_export_settingitem), _("USB Connection"));
        
        // Export
        arcLoc = &g_export_loc;
        gtk_label_set_text(GTK_LABEL(arcLoc->title),    _("Export via USB"));
        gtk_label_set_text(GTK_LABEL(arcLoc->subTitle), _("Select a memory type to be exported to the PC via the USB cable."));

        button_tbl = g_export_loc.buttons;
        gtk_button_set_label(GTK_BUTTON(button_tbl[arcLocMain]), _("Internal memory"));
        gtk_button_set_label(GTK_BUTTON(button_tbl[arcLocCard]), _("MMC card"       ));
        gtk_button_set_label(GTK_BUTTON(button_tbl[arcLocUSB ]), _("USB stick"      ));
        gtk_button_set_label(GTK_BUTTON(button_tbl[arcLocCF  ]), _("CF card"        ));
    }

    if (g_info_item)
    {
        // info
        gtk_infoitem_set_text( GTK_INFOITEM(g_info_item),
                               _("\nWhen you select a memory type for an archive key, "
                                 "the iLiad will use this as storage for newly downloaded items from iDS.") );
    }
}

void iLiad_archive_location_display_data(void)
{
    int           i;
    arcType_e     type;
    arcLocType_e  location;
    GtkWidget    *button;

    ST_LOGPRINTF("entry");

    for (i = 0 ; i < sizeof(g_archive_locs) / sizeof(g_archive_locs[0]) ; i++)
    {
        type = g_archive_locs[i].arcType;
        location = iLiad_archive_location_data_get(type);

        button = g_archive_locs[i].buttons[location];
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    }

    location = iLiad_export_location_data_get();
    button = g_export_loc.buttons[location];
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
}

// callbacks
static void on_selection_update(erGtkSelectionGroup * selection, gpointer button, gpointer data)
{
    const gtkArcLoc_t *arcLoc = (gtkArcLoc_t*)data;

    int          button_id = 0;
    arcLocType_e location;


    ST_LOGPRINTF("entry");
    g_return_if_fail(data != NULL);

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(arcLoc->selection), &button_id, 1);
    if (button_id >= 0  &&  button_id < arcLocUndefined)
    {
        location = button_id;
    }
    else
    {
        location = arcLocMain;
    }

    if (arcLoc == &g_export_loc)
    {
        iLiad_export_location_data_set(location);
    }
    else
    {
        iLiad_archive_location_data_set(arcLoc->arcType, location);
    }
}
