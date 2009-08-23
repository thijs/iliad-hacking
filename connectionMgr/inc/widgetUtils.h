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
 * \file widgetUtils.h
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

#ifndef _WIDGET_UTILS_H_
#define _WIDGET_UTILS_H_

#include <glib.h>
#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>
#include "connectPing.h" 

typedef struct _wdtLabelEntry
{
    GtkWidget *parent; // the top container, GtkVBox
    GtkWidget *label;  // GtkLabel
    GtkWidget *entry;  // erGtkEntry or erGtkEntryIPV4 
    GtkWidget *button; // GtkToggleButton
} wdtLabelEntry;

typedef struct _wdtLabelButton
{
    GtkWidget *parent;    // the top container, GtkVBox
    GtkWidget *label;     // GtkLabel
    GtkWidget *selection; // erGtkSelectionGroup
    int              nbutton;     // the nubmer of buttons
    GtkToggleButton **buttonList; // the list of GtkToggleButton
} wdtLabelButton;

typedef struct _bkWdtLabelEntry
{
    GtkWidget *background;   // GtkEventBox
    wdtLabelEntry *details;
}bkWdtLabelEntry;

typedef struct _bkWdtLabelButton
{
    GtkWidget *background;  // GtkEventBox
    wdtLabelButton *details;
}bkWdtLabelButton;

typedef struct _bkProfileEntry
{
    GtkWidget *background;  // GtkEventBox
    GtkWidget *details; 
}bkProfileEntry;

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    // Create wdtLabelEntry.
    wdtLabelEntry *wdt_label_entry_new (entryFilter_e filter);

    // Create wdtLabelButton 
    wdtLabelButton *wdt_label_button_new (int nbutton, 
                                          gboolean smallButton,
                                          int minsel, 
                                          int maxsel);

    // Create bkWdtLabelEntry.
    bkWdtLabelEntry *bk_wdt_label_entry_new(entryFilter_e filter);

    // Create bkWdtLabelButton.
    bkWdtLabelButton *bk_wdt_label_button_new(int nbutton, 
                                              gboolean smallButton,
                                              int minsel, 
                                              int maxsel);

    // Create bkProfileEntry.
    bkProfileEntry *bk_profile_entry_new(void);

    // Display settings
    void bk_profile_entry_display(const bkProfileEntry *item, 
                                  const regNetworkProfile_t *settings);

    // Update the status
    void bk_profile_entry_update_status(const bkProfileEntry *item, 
                                        pingStatus_t status);

#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif //_WIDGET_UTILS_H_
