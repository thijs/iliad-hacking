/*
 * This file is part of pcshareMgr.
 *
 * pcshareMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pcshareMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file view.c
 * \brief view - GUI of the PC Share Manager application                  
 * 
 * <File description>
 *
 */

/* pcshareMgr - A GTK based PC Share application on the eReader platform
 * Copyright (C) 2006 iRex Technologies BV
 *
 */

#include <gtk/gtk.h>
#include <string.h>
#include <liberdm/display.h>
#include "languages.h"
#include "logging.h"
#include "view.h"
#include "displayUpdate.h"

// global data
static GtkWidget* g_error_background;
static GtkWidget* g_error_icon;
static GtkWidget* g_error_text;
static GtkWidget* g_error_confirm;
static GtkWidget* g_progress;
static GtkWidget* g_info;

static int        g_progressCount = 1;

//callbacks
gboolean view_expose_event(GtkWidget* widget, GdkEventExpose* event, gpointer data);
gboolean on_error_button_pressed(GtkWidget* widget, GdkEventButton* event, gpointer data);

static on_error_confirmed_t* g_on_error_confirmed = NULL;



// create and show the background
// create but hide the error information
//
// object hierarchy:
//     topLevelWindow
//       |-- viewbackground (event box)
//             |-- alignment
//                   |-- vbox
//                         |-- textbackground (event box)
//                         |     |-- hbox
//                         |           |-- g_info (GtkLabel)
//                         |           |-- g_progress (GtkLabel)
//                         |-- g_error_background (event box)
//                               |-- hbox
//                                     |-- alignment
//                                     |     |-- g_error_icon (event box)
//                                     |-- error_vbox
//                                           |-- g_error_text (GtkLabel)
//                                           |-- hbox
//                                                 |-- g_error_confirm (GtkLabel)
//     
void viewInit(GtkWidget * topLevelWindow)
{
    GtkWidget *hbox;
    GtkWidget *alignment;
    GtkWidget *vbox;
    GtkWidget *viewbackground;
    GtkWidget *textbackground;
    GtkWidget *error_vbox;
    GtkWidget *widget;

    DL_LOGPRINTF("entry");

    // viewbackground
    widget = gtk_event_box_new();
    gtk_widget_set_name(widget, "downloadMgr_background");
    gtk_container_add(GTK_CONTAINER(topLevelWindow), widget);
    viewbackground = widget;
    //     signal handlers
    g_signal_connect(G_OBJECT(viewbackground), "expose-event", G_CALLBACK(view_expose_event), NULL);

    // alignment
    widget = gtk_alignment_new(0, 0, 1, 1);
    gtk_alignment_set_padding(GTK_ALIGNMENT(widget), TEXT_BORDER_TOP, TEXT_BORDER_BOTTOM, TEXT_BORDER_LEFT, TEXT_BORDER_RIGHT);
    gtk_container_add(GTK_CONTAINER(viewbackground), widget);
    alignment = widget;
    
    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    // textbackground
    widget = gtk_event_box_new();
    gtk_widget_set_name(widget, "downloadMgr_textbackground");
    gtk_widget_set_size_request(widget, TEXT_BG_WIDTH, TEXT_BG_HEIGHT);
    gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    textbackground = widget;

    //     hbox
    hbox = gtk_hbox_new(FALSE, VIEW_TEXT_SPACING);
    gtk_widget_set_size_request(hbox, TEXT_BG_WIDTH, TEXT_BG_HEIGHT);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), HBOX_TEXT_BORDER);
    gtk_container_add(GTK_CONTAINER(textbackground), hbox);

    //         g_info
    widget = gtk_label_new("");
    gtk_widget_set_name(widget, "downloadMgr_info_text");
    gtk_widget_set_size_request(widget, INFO_TEXT_WIDTH, (-1));
    gtk_label_set_ellipsize(GTK_LABEL(widget), PANGO_ELLIPSIZE_END);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.5);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    g_info = widget;

    //         g_progress
    widget = gtk_label_new("");
    gtk_widget_set_name(widget, "downloadMgr_progress_text");
    gtk_label_set_ellipsize(GTK_LABEL(widget), PANGO_ELLIPSIZE_END);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.5);
    gtk_box_pack_start(GTK_BOX(hbox), widget, TRUE, TRUE, 0);
    g_progress = widget;


    // g_error_background
    widget = gtk_event_box_new();
    gtk_widget_set_name(widget, "downloadMgr_error_background");
    gtk_widget_set_size_request(widget, ERROR_BG_WIDTH, ERROR_BG_HEIGHT);
    gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    g_error_background = widget;
    //     signal handlers
    g_signal_connect(G_OBJECT(g_error_background), "button-press-event", G_CALLBACK(on_error_button_pressed), NULL);

    //     hbox
    hbox = gtk_hbox_new(FALSE, VIEW_TEXT_SPACING);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), HBOX_TEXT_BORDER);
    gtk_container_add(GTK_CONTAINER(g_error_background), hbox);

    //     alignment
    widget = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_set_size_request(widget, ERRORICON_BG_WIDTH, ERRORICON_BG_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    alignment = widget;
    
    //         g_error_icon
    widget = gtk_event_box_new();
    gtk_widget_set_name(widget, "downloadMgr_error_icon");
    gtk_widget_set_size_request(widget, ERRORICON_IMG_WIDTH, ERRORICON_IMG_HEIGHT);
    gtk_container_add(GTK_CONTAINER(alignment), widget);
    g_error_icon = widget;

    //     error_vbox
    widget = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    error_vbox = widget;

    //         g_error_text
    widget = gtk_label_new("");
    gtk_widget_set_name(widget, "downloadMgr_error_text");
    gtk_widget_set_size_request(widget, ERRORTEXT_WIDTH, ERRORTEXT_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.0);
    gtk_label_set_single_line_mode(GTK_LABEL(widget), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(widget), TRUE);
    gtk_box_pack_start(GTK_BOX(error_vbox), widget, FALSE, FALSE, 0);
    g_error_text = widget;

    //         hbox
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 0);
    gtk_container_add(GTK_CONTAINER(error_vbox), hbox);

    //         g_error_confirm
    widget = gtk_label_new("");
    gtk_widget_set_name(widget, "downloadMgr_error_confirm");
    gtk_widget_set_size_request(widget, ERRORCONFIRM_WIDTH, -1);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.5, 0.5);
    gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 0);
    g_error_confirm = widget;


    // show download screen, but not yet the error section
    gtk_widget_show_all(viewbackground);
    gdk_flush();
    gtk_widget_hide(g_error_background);
}

// set static screen texts
void viewSetText(void)
{
    gtk_label_set_text(GTK_LABEL(g_error_confirm), _("Please click here to confirm."));
}


// show/update the information
void viewShowInfo(char *info)
{
    DL_LOGPRINTF("entry %s", (info == NULL) ? "NULL" : info);

    gtk_label_set_text(GTK_LABEL(g_info), info);

    gtk_widget_show(g_info);
    display_update_request_screen_refresh(TEXT_UPDATE_LEVEL, (gpointer) WAVEFORM_TYPING);
}

// clear the information
void viewHideInfo(void)
{
    DL_LOGPRINTF("entry");
    gtk_label_set_text(GTK_LABEL(g_info), "");

    display_update_request_screen_refresh(TEXT_UPDATE_LEVEL, (gpointer) WAVEFORM_TYPING);
}

// show/update the progress 
void viewShowProgress(void)
{
    gchar text[10];

    DL_LOGPRINTF("entry %d", g_progressCount);
    
    memset(text, '+', g_progressCount);
    text[g_progressCount] = '\0';

    gtk_label_set_text(GTK_LABEL(g_progress), text);
    gtk_widget_show(g_progress);

    if (g_progressCount == VIEW_MAX_DOTS)
    {
        g_progressCount = 1;
    }
    else
    {
        g_progressCount++;
    }

    display_update_request_screen_refresh(TEXT_UPDATE_LEVEL, (gpointer) WAVEFORM_BW);
}

// clear the progress
void viewHideProgress(void)
{
    DL_LOGPRINTF("entry");
    g_progressCount = 1;
    gtk_label_set_text(GTK_LABEL(g_progress), "");
    display_update_request_screen_refresh(TEXT_UPDATE_LEVEL, (gpointer) WAVEFORM_TYPING);
}

// show/update the error message
static gboolean on_delayed_update_screen(gpointer p)
{
    display_update_request_screen_refresh(TEXT_EXPOSE_LEVEL, (gpointer) WAVEFORM_TYPING);
    return FALSE;
}

void viewShowError(const char *msg)
{
    DL_LOGPRINTF("entry %s", (msg == NULL) ? "NULL" : msg);

    gtk_label_set_text(GTK_LABEL(g_error_text), msg);

    gtk_widget_show(g_error_background);

    // it's a weird world, but calling display_update_request_screen_refresh right now does not work
    // maybe this because it is called from the ecdcthread and objects are created in the main thread
    // anyway, adding a slight delay works fine and also separates the "downloading" display from the "error"
    g_timeout_add(1000, on_delayed_update_screen, 0);
}

// clear the error window
void viewHideError(void)
{
    DL_LOGPRINTF("entry");
    gtk_label_set_text(GTK_LABEL(g_error_text), "");

    gtk_widget_hide(g_error_background);
    // display update implicit because main window gets fully exposed now
}


// signal handlers
gboolean view_expose_event(GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
    DL_LOGPRINTF("entry");

    display_update_request_screen_refresh(VIEW_WINDOW_EXPOSE_LEVEL, (gpointer) WAVEFORM_FULLSCREEN);
    return FALSE;
}


gboolean on_error_button_pressed(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
    DL_LOGPRINTF("entry");

    if (g_on_error_confirmed)
    {
        g_on_error_confirmed();
    }
    else
    {
        viewHideError();
    }
    return FALSE;
}


void view_callback_on_error_confirmed(on_error_confirmed_t* callback)
{
    DL_LOGPRINTF("entry");

    g_on_error_confirmed = callback;
}


