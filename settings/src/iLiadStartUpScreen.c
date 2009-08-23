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
 * \file iLiadStartUpScreen.c
 * \brief settings - users customized startup behaviour Screen 
 *
 * Providing GUI interface to let users input their customized startup behaviour. 
 *
 */
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include <liberdm/display.h>
#include <libergtk/ergtk.h>
#include <liberregxml/erregapi.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "gtkSettingItem.h"
#include "iLiadStartUpData.h"
#include "iLiadStartUpScreen.h"
#include "iLiadConfigData.h"
#include "settings.h"
#include "languages.h"


static const struct
             {
                 const gchar               *text;
                 const startup_behaviour_t  behaviour;
                 const gchar               *document_path;
             } g_button_info[STARTUP_BUTTON_NUMBER] 
              = {
                    { gettext_noop("Quick Start Guide")  , specificDocument_t  , QUICK_START_GUIDE_PATH },
                    { gettext_noop("Reference materials"), referenceMaterials_t, ""                     },
                    { gettext_noop("Device manager")     , deviceManager_t     , ""                     },
                    { gettext_noop("Recent documents")   , recentDocuments_t   , ""                     },
                    { gettext_noop("Last read document") , lastReadDocument_t  , ""                     },
                    { gettext_noop("News folder")        , newsFolder_t        , ""                     },
                    { gettext_noop("Books folder")       , booksFolder_t       , ""                     },
                    { gettext_noop("Documents folder")   , docsFolder_t        , ""                     },
                    { gettext_noop("Notes folder")       , notesFolder_t       , ""                     },
                };

// global variables
static gtkStartUp_t g_startup;

static pid_t g_xtscal_pid = 0;

// internal functions
static GtkWidget* gtk_behaviour_new(gtkBehaviour_t *behaviour);
static GtkWidget* gtk_calibration_new(gtkCalibration_t* calibration);

static void on_selection_update(erGtkSelectionGroup * selection, gpointer button, gpointer data);
static void on_calibration_button_toggled_event(GtkToggleButton * button, gpointer data);

// widget hierarchy
// |--vbox
//    |--startup(GtkSettingItem)
//    |--calibration(GtkSettingItem)
GtkWidget *iLiad_startup_screen_create(void)
{
    GtkWidget *vbox;
    GtkWidget *behaviour;
    GtkWidget *calibration;

    ST_LOGPRINTF("entry");

    memset(&g_startup, 0, sizeof(gtkStartUp_t));

    vbox = gtk_vbox_new(FALSE, STARTUP_ITEM_SPACING);
    g_startup.widget = vbox;

    behaviour = gtk_behaviour_new(&(g_startup.behaviour));
    gtk_box_pack_start(GTK_BOX(vbox), behaviour, FALSE, FALSE, 0);

    calibration = gtk_calibration_new(&(g_startup.calibration));
    gtk_box_pack_start(GTK_BOX(vbox), calibration, FALSE, FALSE, 0);

    // show widgets
    gtk_widget_show(vbox);

    return vbox;
}

// widget hierarchy
// |--item(GtkSettingItem)
//    |--background(GtkEventBox)
//       |--alignment
//          |--vbox1
//             |--label
//             |--hbox
//                |--vbox2
//                   |--button(Quick Start Quide)
//                   |--button(Reference Materials)
//                   |--button(Last Read Document)
//                   |--button(Recent Documents)
//                   |--button(Device Manager)
//                |--vbox3
//                   |--button(News Folder)
//                   |--button(Books Folder)
//                   |--button(Documents Folder)
//                   |--button(Notes Folder)
static GtkWidget* gtk_behaviour_new(gtkBehaviour_t *behaviour)
{
    GtkWidget* item;
    GtkWidget* background;
    GtkWidget *alignment;
    GtkWidget* vbox1, *vbox2, *vbox3;
    GtkWidget* label;
    GtkWidget* hbox;
    GtkWidget* button;
    GtkWidget* selection;
    GtkToggleButton* button_tbl[STARTUP_BUTTON_NUMBER + 1];
    gint     i;

    ST_LOGPRINTF("entry");
    g_return_val_if_fail(NULL != behaviour, NULL);
    
    // item
    item = gtk_settingitem_new("");
    gtk_widget_set_size_request(item, -1, -1);
    behaviour->item = item;
     
    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "startup_behaviour_background");
    gtk_widget_set_size_request(background, TITLE_MIN_WIDTH, STARTUP_BEHAVIOUR_HEIGHT);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), background);
    
    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              STARTUP_ITEM_PADDING_TOP, STARTUP_ITEM_PADDING_BOTTOM,
                              STARTUP_ITEM_PADDING_LEFT, STARTUP_ITEM_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);
   
    // vbox1
    vbox1 = gtk_vbox_new(FALSE, STARTUP_ITEM_SPACING*3);
    gtk_container_add(GTK_CONTAINER(alignment), vbox1);

    // label
    label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox1), label, FALSE, FALSE, 0);
    gtk_widget_set_name(label, "startup_info_text");
    gtk_widget_set_size_request(label, STARTUP_LABEL_WIDTH, (-1));
    gtk_label_set_single_line_mode(GTK_LABEL(label), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    behaviour->label = label;

    // hbox
    hbox = gtk_hbox_new(FALSE, (STARTUP_LABEL_WIDTH - STARTUP_BUTTON_WIDTH * 2) / 2);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox, FALSE, FALSE, 0);

    // vbox2
    vbox2 = gtk_vbox_new(FALSE, STARTUP_ITEM_SPACING*2);
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 0);

    // vbox3
    vbox3 = gtk_vbox_new(FALSE, STARTUP_ITEM_SPACING*2);
    gtk_box_pack_start(GTK_BOX(hbox), vbox3, FALSE, FALSE, 0);

    // buttons
    for (i = 0; i < STARTUP_BUTTON_NUMBER; i++)
    {
        button = ergtk_toggle_button_new_with_label("");
        gtk_widget_set_size_request(GTK_WIDGET(button), STARTUP_BUTTON_WIDTH, STARTUP_BUTTON_HEIGHT);
        if (i <= 4)
        {
            gtk_box_pack_start(GTK_BOX(vbox2), GTK_WIDGET(button), FALSE, FALSE, 0);
        }
        else
        {
            gtk_box_pack_start(GTK_BOX(vbox3), GTK_WIDGET(button), FALSE, FALSE, 0);
        }
        button_tbl[i] = GTK_TOGGLE_BUTTON(button);
        behaviour->buttons[i] = GTK_WIDGET(button);
    }
    button_tbl[i] = NULL;

    // selection group not part of widget hierarchy
    selection = ergtk_selection_group_new(button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(selection), 1, 1);
    g_signal_connect(G_OBJECT(selection), "selection-update", G_CALLBACK(on_selection_update), &g_startup);
    behaviour->selection = selection;

    // show widgets
    gtk_widget_show_all(background);
    gtk_widget_show(item);
    
    return item;
}

// widget hierarchy
// |--item(GtkSettingItem)
//    |--background(GtkEventBox)
//       |--alignment
//          |--vbox
//             |--label
//             |--hbox
//                |--button
static GtkWidget* gtk_calibration_new(gtkCalibration_t* calibration)
{
    GtkWidget* item;
    GtkWidget* background;
    GtkWidget* alignment;
    GtkWidget* vbox;
    GtkWidget* hbox;
    GtkWidget* label;
    GtkWidget* button;

    ST_LOGPRINTF("entry");
    g_return_val_if_fail(NULL != calibration, NULL);

    // item
    item = gtk_settingitem_new("");
    gtk_widget_set_size_request(item, -1, -1);
    calibration->item = item;
    
    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "startup_calibration_background");
    gtk_widget_set_size_request(background, TITLE_MIN_WIDTH, STARTUP_CALIBRATION_HEIGHT);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), background);
    
    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              STARTUP_ITEM_PADDING_TOP, STARTUP_ITEM_PADDING_BOTTOM,
                              STARTUP_ITEM_PADDING_LEFT, STARTUP_ITEM_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);
   
    // vbox
    vbox = gtk_vbox_new(FALSE, STARTUP_ITEM_SPACING*3);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    // label
    label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    gtk_widget_set_name(label, "startup_info_text");
    gtk_widget_set_size_request(label, STARTUP_LABEL_WIDTH, (-1));
    gtk_label_set_single_line_mode(GTK_LABEL(label), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    calibration->label = label;
 
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(hbox), FALSE, FALSE, 0);
 
    // button
    button = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(button), STARTUP_BUTTON_WIDTH, STARTUP_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(button), FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(on_calibration_button_toggled_event), NULL);
    calibration->button = GTK_WIDGET(button);

    // show widgets
    gtk_widget_show_all(background);
    gtk_widget_show(item);
   
    return item;
}

void iLiad_startup_screen_set_text(void)
{
    ST_LOGPRINTF("entry");
    int i;

    if (g_startup.widget)
    {
        gtkBehaviour_t* behaviour = &(g_startup.behaviour);
        gtkCalibration_t* calibration = &(g_startup.calibration);
       
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(behaviour->item), _("Start-up Settings"));
        gtk_label_set_text(GTK_LABEL(behaviour->label), _("Please select what to open when starting up your iLiad."));
        
        for (i = 0; i < STARTUP_BUTTON_NUMBER; i++)
        {
            gtk_button_set_label( GTK_BUTTON(behaviour->buttons[i]), _(g_button_info[i].text) );
        }
        
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(calibration->item), _("Stylus calibration"));
        gtk_label_set_text(GTK_LABEL(calibration->label), _("With this function you can re-calibrate your stylus."));
        gtk_button_set_label(GTK_BUTTON(calibration->button), _("Start Calibration"));
    }
}

void iLiad_startup_screen_display_data(void)
{
    startup_behaviour_t behaviour;
    GtkWidget*          button;
    int                 i;

    ST_LOGPRINTF("entry");

    behaviour = iLiad_startup_data_get_behaviour();
    for (i = 0 ; i < STARTUP_BUTTON_NUMBER ; i++)
    {
        if (g_button_info[i].behaviour == behaviour)
        {
            button = g_startup.behaviour.buttons[i];
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
            gtk_widget_grab_focus(button);
        }
    }
}

static void on_selection_update(erGtkSelectionGroup * selection, gpointer button, gpointer data)
{
    const gtkStartUp_t *startup = (gtkStartUp_t*)data;
    int                 button_id = 0;

    ST_LOGPRINTF("entry");
    g_return_if_fail(data != NULL);

    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(startup->behaviour.selection), &button_id, 1);
    iLiad_startup_data_set_behaviour(g_button_info[button_id].behaviour, g_button_info[button_id].document_path);
}

static void on_calibration_button_toggled_event(GtkToggleButton * button, gpointer data)
{
    pid_t pid;

    ST_LOGPRINTF("entry");

    // check if it is a "pressed event"
    gboolean pressed = gtk_toggle_button_get_active(button);
    if (pressed)
    {
        if ((pid = fork()) < 0)
        {
            ST_ERRORPRINTF("Can't fork child process!");
            return;
        }
        else if (pid == 0)
        {
            // Child process, exec xtscal
            if (execvp("/usr/bin/xtscal", NULL) < 0)
            {
                ST_ERRORPRINTF("execve error!");
                return;
            }
            // Should never be here
        }
        else
        {
            // Parent process
            g_xtscal_pid = pid;
        }
    }
}

pid_t get_xtscal_pid()
{
    return g_xtscal_pid;
}

void handle_xtscal_quit()
{
    // Remember child process gone
    g_xtscal_pid = 0;
    
    // Refresh screen
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_startup.calibration.button), FALSE);
    // display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL, WAVEFORM_FULLSCREEN);
}

void stop_xtscal()
{
    if (g_xtscal_pid != 0)
    {
        kill(g_xtscal_pid, SIGTERM);
    }
}

