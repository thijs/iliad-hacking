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
 * \file widgetUtils.c
 * \brief connectionMgr - Some utilities of creating widgets.
 * 
 * These widgets include:
 * (1) The wdtLabelEntry contains one label at first line,
 *     and one text entry and one button at second line.
 * (2) The wdtLabelButton contains one label at first line,
 *     and some small or normal buttons at second line, 
 *     and these buttons only can be selected exclusively.
 * (3) The bkWdtLabelEntry contains one wdtLabelEntry in background.
 * (4) The bkWdtLabelButton contains one wdtLabelButton in background. 
 * (5) The bkProfileEntry contains one gtkProfileEntry in background.
 *
 * Annotation: wdt is the abbrev of widget, bk is the abbrev of background.
 *
 * Copyright (C) 2007 iRex Technologies BV.
 * 
 */
#include "config.h"

#include <glib.h>
#include <gtk/gtk.h>

#include <libergtk/ergtk.h>

#include "connectionMgrLog.h"
#include "background.h"
#include "gtkProfileEntry.h"
#include "widgetUtils.h"
#include "languages.h"

// Create wdtLabelEntry.
//
// objects hierarchy
// wdtLabelEntry
//    |--vbox
//       |--label
//       |--hbox
//          |--entry
//          |--button
//
wdtLabelEntry *wdt_label_entry_new (entryFilter_e filter)
{
    GtkWidget *vbox = NULL;
    GtkWidget *label = NULL;
    GtkWidget *hbox = NULL;
    GtkWidget *entry = NULL;
    GtkWidget *button = NULL;
    wdtLabelEntry *item = NULL;
    int spacing = 0;

    CN_LOGPRINTF("entry filter[%d]", filter);

    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);

    // label
    label = gtk_label_new("");
    gtk_widget_set_name(label, "info_label");
    gtk_widget_set_size_request(label, LABEL_WIDTH, LABEL_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
   
    // hbox
    spacing = SITEM_BK_WIDTH - PADDING_LEFT - PADDING_RIGHT 
              - ENTRY_WIDTH - SBUTTON_WIDTH;
    hbox = gtk_hbox_new(FALSE, spacing);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
   
    // entry
    entry = ergtk_entry_new();
    if (filter == ipAddress_e)
    {
        ergtk_entry_set_ipv4_filter(ERGTK_ENTRY(entry));
    }
    else if (filter == integer_e)
    {
        ergtk_entry_set_integer_filter(ERGTK_ENTRY(entry));
    }
    
    gtk_widget_set_size_request(entry, ENTRY_WIDTH, ENTRY_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);
    
    // button
    button = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(button, SBUTTON_WIDTH, SBUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    // show widgets
    gtk_widget_show(label);
    gtk_widget_show_all(hbox);

    // item 
    item = g_new0(wdtLabelEntry, 1);
    g_assert(item != NULL);
    item->parent = vbox;
    item->label = label;
    item->entry = entry;
    item->button = button;

    // return value
    return item;
}

// Create wdtLabelButton 
//
// objects hierarchy
// wdtLabelButton
//    |--vbox
//       |--label
//       |--hbox
//          |--buttonList[](buttons controlled by erGtkSelectionGroup)
//
wdtLabelButton *wdt_label_button_new (int nbutton, gboolean smallButton, 
                                      int minsel, int maxsel)
{
    GtkWidget *vbox = NULL;
    GtkWidget *label = NULL;
    GtkWidget *hbox = NULL;
    GtkWidget *button = NULL;
    GtkToggleButton **buttonList = NULL;
    GtkWidget *selection = NULL;
    wdtLabelButton *item = NULL;
    int i;
    int spacing = 0, maxSpacing;
    int buttonWidth, buttonHeight;

    CN_LOGPRINTF("entry nbutton[%d] smallButton[%d] minsel[%d] maxsel[%d]",
                  nbutton, smallButton, minsel, maxsel);

    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);

    // label
    label = gtk_label_new("");
    gtk_widget_set_name(label, "info_label");
    gtk_widget_set_size_request(label, LABEL_WIDTH, LABEL_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    // button size
    if (smallButton)
    {
        buttonWidth = SBUTTON_WIDTH;
        buttonHeight = SBUTTON_HEIGHT;
    }
    else
    {
        buttonWidth = BUTTON_WIDTH;
        buttonHeight = BUTTON_HEIGHT;
    }

    // the spacing among buttons
    if (nbutton > 1)
    {
        spacing = (SITEM_BK_WIDTH - PADDING_LEFT - PADDING_RIGHT 
                   - buttonWidth * nbutton)
                  / (nbutton -1);
    }
    maxSpacing = (SITEM_BK_WIDTH - PADDING_LEFT - PADDING_RIGHT 
                  - BUTTON_WIDTH * 3) / 2; 
    if (spacing > maxSpacing)
    {
        spacing = maxSpacing;
    }

    // hbox
    hbox = gtk_hbox_new(FALSE, spacing);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
   
    // buttonList
    buttonList = g_new0(GtkToggleButton*, nbutton+1);
    g_assert(buttonList != NULL);

    // button
    for (i = 0; i < nbutton; i++)
    {
        button = ergtk_toggle_button_new_with_label("");
        gtk_widget_set_size_request(button, buttonWidth, buttonHeight);
        gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
        buttonList[i] = GTK_TOGGLE_BUTTON(button);
    }
    buttonList[i] = NULL;

    // selection
    selection = ergtk_selection_group_new(buttonList);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(selection), 
                                      minsel, maxsel);

    // show widgets
    gtk_widget_show(label);
    gtk_widget_show_all(hbox);

    // item 
    item = g_new0(wdtLabelButton, 1);
    g_assert(item != NULL);

    item->parent = vbox;
    item->label = label;
    item->selection = selection;
    item->buttonList = buttonList;
    item->nbutton = nbutton;

    // return value
    return item;
}

// Create bkWdtLabelEntry.
//
// objects hierarchy
// bkWdtLabelEntry
//    |--background
//       |--alignment
//          |--details(wdtLabelEntry)
//
bkWdtLabelEntry *bk_wdt_label_entry_new(entryFilter_e filter)
{
    GtkWidget *background = NULL;
    GtkWidget *alignment = NULL;
    wdtLabelEntry *details = NULL;
    bkWdtLabelEntry *item = NULL;

    CN_LOGPRINTF("entry filter[%d]", filter);

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "bk_grey_666_75");
    gtk_widget_set_size_request(background, 
            SITEM_BK_WIDTH, SITEM_BK_HEIGHT);

    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
            PADDING_TOP, PADDING_BOTTOM, PADDING_LEFT, PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // details
    details = wdt_label_entry_new(filter);
    g_assert(details != NULL);
    gtk_container_add(GTK_CONTAINER(alignment), details->parent);

    // show widgets
    gtk_widget_show(alignment);
    gtk_widget_show(details->parent);

    // item 
    item = g_new0(bkWdtLabelEntry, 1);
    g_assert(item != NULL);
    item->background = background;
    item->details = details;

    // return value
    return item;
}

// Create bkWdtLabelButton.
//
// objects hierarchy
// bkWdtLabelButton
//    |--background
//       |--alignment
//          |--details(wdtLabelButton)
//
bkWdtLabelButton *bk_wdt_label_button_new(int nbutton, gboolean smallButton,
                                          int minsel, int maxsel)
{
    GtkWidget *background = NULL;
    GtkWidget *alignment = NULL; 
    wdtLabelButton *details = NULL;
    bkWdtLabelButton *item = NULL;

    CN_LOGPRINTF("entry nbutton[%d] smallButton[%d] minsel[%d] maxsel[%d]",
                  nbutton, smallButton, minsel, maxsel);

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "bk_grey_666_75");
    gtk_widget_set_size_request(background, 
            SITEM_BK_WIDTH, SITEM_BK_HEIGHT);

    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
            PADDING_TOP, PADDING_BOTTOM, PADDING_LEFT, PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // details
    details = wdt_label_button_new(nbutton, smallButton, minsel, maxsel);
    g_assert(details != NULL);
    gtk_container_add(GTK_CONTAINER(alignment), details->parent);

    // show widgets
    gtk_widget_show(alignment);
    gtk_widget_show(details->parent);

    // item 
    item = g_new0(bkWdtLabelButton, 1);
    g_assert(item != NULL);
    item->background = background;
    item->details = details;

    // return value
    return item;
}

// Create bkProfileEntry.
//
// objects hierarchy
// bkProfileEntry
//    |--background
//       |--alignment
//          |--details(gtkProfileEntry)
//
bkProfileEntry *bk_profile_entry_new(void)
{
    GtkWidget *background = NULL;
    GtkWidget *alignment = NULL; 
    GtkWidget *details = NULL;
    bkProfileEntry *item = NULL;

    CN_LOGPRINTF("entry");

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "bk_grey_666_75");
    gtk_widget_set_size_request(background, 
            SITEM_BK_WIDTH, SITEM_BK_HEIGHT);

    // alignment
    alignment = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
            PADDING_TOP, PADDING_BOTTOM, PADDING_LEFT, PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // details
    details = gtk_profile_entry_new();
    g_assert(details != NULL);
    gtk_container_add(GTK_CONTAINER(alignment), details);

    // show widgets
    gtk_widget_show(alignment);
    gtk_widget_show(details);

    // item 
    item = g_new0(bkProfileEntry, 1);
    g_assert(item != NULL);
    item->background = background;
    item->details = details;

    // return value
    return item;
}

void bk_profile_entry_display(const bkProfileEntry *item, 
                              const regNetworkProfile_t *settings)
{
    GtkWidget        *widget;
    networkProfile_t nwSettings;

    g_return_if_fail(item != NULL);
    g_return_if_fail(settings != NULL);

    // display the profile entry
    nwSettings.scanSettings = NULL;
   
    nwSettings.regSettingsList = g_new0(regNetworkProfile_t*, 1);
    g_return_if_fail(nwSettings.regSettingsList != NULL);
    
    nwSettings.regSettingsList[0] = (regNetworkProfile_t*)settings; 
    nwSettings.regIndexList = NULL;
    nwSettings.nRegProfiles = 1;
    nwSettings.nActiveIndex = 0;

    widget = item->details;
    gtk_profile_entry_display_settings(GTK_PROFILE_ENTRY(widget), 
                                       &nwSettings, "");
    
    g_free(nwSettings.regSettingsList);
    nwSettings.regSettingsList = NULL;
}

void bk_profile_entry_update_status(const bkProfileEntry* item, 
                                    pingStatus_t status)
{
    gchar     *text;
    gboolean  flashing = FALSE;
    GtkWidget *widget;

    g_return_if_fail(item != NULL);

    switch (status)
    {
        case pingSucceeded_e:
            text = _("Connected");
            break;
        case pingFailed_e:
        case pingFailedPCShare_e:
        case pingFailedNetwork_e:
            text = _("Unsuccessful");
            break;
        case pingSkipped_e:
            text = ("Skipped");
            break;
        case pingNeedKey_e:
            text = _("Security key required");
            break;
        case pingAborted_e:
            text = _("Aborted");
            break;
        case pingAborting_e:
            text = _("Aborting...");
            flashing = TRUE;
            break;
        case pingConnecting_e:
            text = _("Connecting...");
            flashing = TRUE;
            break;
        default:
            text = "";
            break;
    }

    // display the profile entry
    widget = item->details;
    gtk_profile_entry_set_status(GTK_PROFILE_ENTRY(widget), text);
    gtk_profile_entry_set_status_flashing(GTK_PROFILE_ENTRY(widget), 
                                          flashing);
}

