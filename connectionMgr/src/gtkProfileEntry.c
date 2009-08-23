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
 * \file gtkProfileEntry.c
 * \brief connectionMgr - the network-profile entry
 *                        containing a button with profile name or SSID
 *                        a icon showing the wireless signal quality
 *                        a icon showing it's a profile with encryption
 *                        a label showing the status
 *                        a button to let user enter the profile editor
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#include <string.h>
#include <stdio.h>

#include <gtk/gtk.h>

#include <liberdm/display.h>
#include <libergtk/ergtk.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "gtkProfileEntry.h"

// local functions
static void gtk_profile_entry_class_init(GtkProfileEntryClass * klass);
static void gtk_profile_entry_init(GtkProfileEntry * profileEntry);

static void gtk_profile_entry_hide_signal_image(GtkProfileEntry *profileEntry);

// signal handlers
static gboolean on_flash_status(gpointer data);
   
// creates a new GtkProfileEntry widget 
GtkWidget *gtk_profile_entry_new(void)
{
    GtkProfileEntry *profileEntry;

    profileEntry = (GtkProfileEntry *) g_object_new(GTK_PROFILE_ENTRY_TYPE, NULL);

    gtk_widget_show_all(profileEntry->clientArea);
    gdk_flush();

    gtk_profile_entry_hide_signal_image(profileEntry);
    gtk_widget_hide(profileEntry->encrImage);
    gtk_widget_hide(profileEntry->editButton);

    return GTK_WIDGET(profileEntry);
}

// returns type of this widget 
GType gtk_profile_entry_get_type(void)
{
    static GType profile_entry_type = 0;

    if (!profile_entry_type)
    {
        static const GTypeInfo  profile_entry_info = 
        {
            sizeof(GtkProfileEntryClass),  /* class_size */
            NULL,                          /* base_init */
            NULL,                          /* base_finalize */
            (GClassInitFunc) gtk_profile_entry_class_init,/* class_init */
            NULL,                          /* class_finalize */
            NULL,                          /* class_data */
            sizeof(GtkProfileEntry),       /* instance_size */
            0,                             /* n_preallocs */
            (GInstanceInitFunc) gtk_profile_entry_init,  /* instance_init */
            0                              /* value_table */
        };
        profile_entry_type = g_type_register_static(GTK_TYPE_EVENT_BOX, 
                                                    "ProfileEntry", 
                                                    &profile_entry_info, 
                                                    0);
    }

    return profile_entry_type;
}

// class_init:
static void gtk_profile_entry_class_init(GtkProfileEntryClass * klass)
{
    // nothing needs to be done here 
}

// instance_init: create the composite widget - set the default values 
//
// object hierarchy:
// profileEntry
//   |--profileEntry->clientArea (hbox)
//      |--profileEntry->nameButton (button)
//      |--profileEntry->sigVbox (vbox)
//         |--sigImage[5]
//      |--profileEntry->encrVbox (vbox)
//         |--profileEntry->encrImage (event box)
//      |--profileEntry->status(label)
//      |--profileEntry->editButton (button)
static void gtk_profile_entry_init(GtkProfileEntry * profileEntry)
{
    CN_LOGPRINTF("entry");

    GtkWidget* clientArea;
    GtkWidget* nameButton;
    GtkWidget* sigVbox;
    GtkWidget* sigImage;
    GtkWidget* encrVbox;
    GtkWidget* encrImage;
    GtkWidget* status;
    GtkWidget* editButton;

    gtk_widget_set_size_request(GTK_WIDGET(profileEntry), 
                PROFILE_ENTRY_WIDTH, PROFILE_ENTRY_HEIGHT);
    // setting event box invisible can make it transparent
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(profileEntry), FALSE);

    // client area
    clientArea = gtk_hbox_new(FALSE, PROFILE_ENTRY_SPACING);
    gtk_widget_set_size_request(GTK_WIDGET(clientArea), 
                PROFILE_ENTRY_WIDTH, PROFILE_ENTRY_HEIGHT);

    gtk_container_add(GTK_CONTAINER(profileEntry), clientArea);
    profileEntry->clientArea = clientArea;

    // name button
    nameButton = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(nameButton), 
        PROFILE_ENTRY_NAME_BUTTON_WIDTH, PROFILE_ENTRY_NAME_BUTTON_HEIGHT);
    gtk_box_pack_start(GTK_BOX(clientArea), nameButton, FALSE, FALSE, 0);
    profileEntry->nameButton = nameButton;

    // signal quality images.
    sigVbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(GTK_WIDGET(sigVbox), 
        PROFILE_ENTRY_SIG_IMG_WIDTH, PROFILE_ENTRY_SIG_IMG_HEIGHT);
    gtk_box_pack_start(GTK_BOX(clientArea), sigVbox, FALSE, FALSE, 0);
    profileEntry->sigVbox = sigVbox;

    // signal images
    int  i;
    char bgStyle[] = "profile_entry_signal0_background";
    for (i = 0; i < 5; i++)
    {
        sigImage = gtk_event_box_new();
        gtk_widget_set_size_request(GTK_WIDGET(sigImage), 
            PROFILE_ENTRY_SIG_IMG_WIDTH, PROFILE_ENTRY_SIG_IMG_HEIGHT);
        snprintf(bgStyle, sizeof(bgStyle), 
                "profile_entry_signal%d_background", i);
        gtk_widget_set_name(GTK_WIDGET(sigImage), bgStyle);
        gtk_box_pack_start(GTK_BOX(sigVbox), sigImage, FALSE, FALSE, 0);
        profileEntry->sigImage[i] = sigImage;    
    }

    // encrVbox
    encrVbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(GTK_WIDGET(encrVbox), 
        PROFILE_ENTRY_ENCR_IMG_WIDTH, PROFILE_ENTRY_ENCR_IMG_HEIGHT);
    gtk_box_pack_start(GTK_BOX(clientArea), encrVbox, FALSE, FALSE, 0);
    profileEntry->encrVbox = encrVbox;

    // encrImage
    encrImage = gtk_event_box_new();
    gtk_widget_set_size_request(GTK_WIDGET(encrImage), 
        PROFILE_ENTRY_ENCR_IMG_WIDTH, PROFILE_ENTRY_ENCR_IMG_HEIGHT);
    gtk_widget_set_name(GTK_WIDGET(encrImage), 
            "profile_entry_encrimg_background");
    gtk_box_pack_start(GTK_BOX(encrVbox), encrImage, FALSE, FALSE, 0);
    profileEntry->encrImage = encrImage;

    // status
    status = gtk_label_new("");
    gtk_widget_set_size_request(GTK_WIDGET(status), 
        PROFILE_ENTRY_STATUS_WIDTH, PROFILE_ENTRY_STATUS_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(status), 0.02, 0.5);
    gtk_widget_set_name(GTK_WIDGET(status), "profile_entry_status");
    gtk_box_pack_start(GTK_BOX(clientArea), status, FALSE, FALSE, 0);
    profileEntry->status = status;
    
    // editButton
    editButton = ergtk_toggle_button_new_with_label("");
    gtk_widget_set_size_request(GTK_WIDGET(editButton), 
        PROFILE_ENTRY_EDIT_BUTTON_WIDTH, PROFILE_ENTRY_EDIT_BUTTON_HEIGHT);
    gtk_box_pack_end(GTK_BOX(clientArea), editButton, FALSE, FALSE, 0);
    profileEntry->editButton = editButton;

    // local data
    profileEntry->statusFlashing = FALSE;
    
    CN_LOGPRINTF("done");
}

// display network settings
void gtk_profile_entry_display_settings(GtkProfileEntry *profileEntry, 
                        const networkProfile_t *settings, gchar *status)
{
    CN_LOGPRINTF("entry [%p][%p][%s]", profileEntry, settings, status);

    int                 nActiveIndex;
    regNetworkProfile_t *pRegSettings = NULL;
    network_spec_t      *pScanSettings = NULL;
    char                *nwName;
    gboolean             encrypted;

    g_return_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry));

    if (settings)
    {
        if ((settings->nRegProfiles > 0)
            && settings->regSettingsList
            && ((settings->nActiveIndex >= 0) 
                && (settings->nActiveIndex < settings->nRegProfiles)))
        {
            nActiveIndex = settings->nActiveIndex;
            pRegSettings = settings->regSettingsList[nActiveIndex];
        }
        pScanSettings = settings->scanSettings;
    }
    
    if (pRegSettings)
    {
        nwName = pRegSettings->name;
    }
    else if (pScanSettings)
    {
        nwName = pScanSettings->ssid;
    }
    else
    {
        nwName = NULL;
    }

    if ((nwName == NULL) || (nwName[0] == '\0'))
    {
        gtk_profile_entry_set_namebutton_text(profileEntry, "");
        gtk_profile_entry_set_status(profileEntry, "");
    }
    else
    {
        // network name
        gtk_profile_entry_set_namebutton_text(profileEntry, nwName);

        // signal images
        if (pScanSettings)
        {
            gtk_profile_entry_set_signal_image(profileEntry, 
                                               pScanSettings->quality);
        }
        else if (pRegSettings && pRegSettings->connection == wireless_t)
        {
            gtk_profile_entry_set_signal_image(profileEntry, 0);
        }
        else
        {
            gtk_profile_entry_hide_signal_image(profileEntry);
        }
       
        // encryption image
        if (pScanSettings && (pScanSettings->encryption != encr_none_t))
        {
            encrypted = TRUE;
        }
        else if (pRegSettings 
                 && pRegSettings->wirelessSettings
                 && pRegSettings->wirelessSettings->encrType != encr_none_t)
        {
            encrypted = TRUE;
        }
        else
        {
            encrypted = FALSE;
        }
        
        gtk_profile_entry_show_encr_image(profileEntry, encrypted);
       
        // status
        gtk_profile_entry_set_status(profileEntry, status);
    }
}

// set profile name
void gtk_profile_entry_set_namebutton_text(GtkProfileEntry *profileEntry, 
                                           gchar *nameText)
{
    g_return_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry));

    if (nameText)
    {
        CN_LOGPRINTF("name: [%s]", nameText);
        gtk_button_set_label(GTK_BUTTON(profileEntry->nameButton), 
                             nameText);
    }
}

void gtk_profile_entry_set_signal_image(GtkProfileEntry *profileEntry, 
                                        gint quality)
{
    g_return_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry));
    g_return_if_fail((quality >= 0) && (quality <= 100));

    CN_LOGPRINTF("quality: [%d]", quality);

    // hide all signal images firstly
    gtk_profile_entry_hide_signal_image(profileEntry);

    // show one correct signal image secondly
    if (quality <= 0)
    {
        gtk_widget_show(profileEntry->sigImage[0]);
    }
    else if (quality <= 25)
    {
        gtk_widget_show(profileEntry->sigImage[1]);
    }
    else if (quality <= 50)
    {
        gtk_widget_show(profileEntry->sigImage[2]);
    }
    else if (quality <= 75)
    {
        gtk_widget_show(profileEntry->sigImage[3]);
    }
    else
    {
        gtk_widget_show(profileEntry->sigImage[4]);
    }
}

// set profile status
void gtk_profile_entry_set_status(GtkProfileEntry *profileEntry, 
                                  gchar *statusText)
{
    g_return_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry));

    if (statusText)
    {
        CN_LOGPRINTF("status: [%s]", statusText);
        gtk_label_set_text(GTK_LABEL(profileEntry->status), statusText);
        gtk_profile_entry_set_status_flashing(profileEntry, FALSE);
    }
}

void gtk_profile_entry_set_status_flashing(GtkProfileEntry *profileEntry,
                                           gboolean flashing)
{
    g_return_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry));

    CN_LOGPRINTF("flashing: [%d]", flashing);

    gtk_widget_set_state(GTK_WIDGET(profileEntry->status), 
                         GTK_STATE_NORMAL);

    if (flashing == TRUE && profileEntry->statusFlashing == FALSE)
    {
        g_timeout_add(1100, on_flash_status, profileEntry);
    }
    profileEntry->statusFlashing = flashing;
}

// Set button labels
void gtk_profile_entry_set_editbutton_text(GtkProfileEntry *profileEntry, 
                                           gchar* editText)
{
    g_return_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry));

    if (editText)
    {
        CN_LOGPRINTF("edit: [%s]", editText);
        gtk_button_set_label(GTK_BUTTON(profileEntry->editButton), 
                             editText);
    }
}

void gtk_profile_entry_set_buttons_active(GtkProfileEntry *profileEntry, 
                                          gboolean nameActive, 
                                          gboolean editActive)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry nameActive[%d]editActive[%d]", 
                  nameActive, editActive);

    g_return_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry));

    button = GTK_TOGGLE_BUTTON(profileEntry->nameButton);
    gtk_toggle_button_set_active(button, nameActive);

    button = GTK_TOGGLE_BUTTON(profileEntry->editButton);
    gtk_toggle_button_set_active(button, editActive);
}

// show/hide wigets
static void gtk_profile_entry_hide_signal_image(GtkProfileEntry *profileEntry)
{
    int i;
    
    g_return_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry));
    
    for (i = 0; i < 5; i++)
    {
        gtk_widget_hide(profileEntry->sigImage[i]);
    }
}

void gtk_profile_entry_show_encr_image(GtkProfileEntry *profileEntry, 
                                       gboolean show)
{
    CN_LOGPRINTF("entry show[%d]", show);

    g_return_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry));

    if (show)
    {
        gtk_widget_show(profileEntry->encrImage);
    }
    else
    {
        gtk_widget_hide(profileEntry->encrImage);
    }
}

void gtk_profile_entry_show_edit_button(GtkProfileEntry *profileEntry, 
                                        gboolean show)
{
    CN_LOGPRINTF("entry show[%d]", show);

    g_return_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry));

    if (show)
    {
        gtk_widget_show(profileEntry->editButton);
    }
    else
    {
        gtk_widget_hide(profileEntry->editButton);
    }
}

static gboolean on_flash_status(gpointer data)
{
    CN_LOGPRINTF("entry");

    GtkWidget *status;
    static int delay = 0;

    g_return_val_if_fail((data != NULL), FALSE);
    GtkProfileEntry* profileEntry = GTK_PROFILE_ENTRY(data);

    if (profileEntry->statusFlashing)
    {
        if (--delay < 0)
        {
            status = GTK_WIDGET(profileEntry->status);
            if (GTK_WIDGET_STATE(status) == GTK_STATE_NORMAL)
            {
                gtk_widget_set_state(status, GTK_STATE_ACTIVE);
                delay = 0;
            }
            else
            {
                gtk_widget_set_state(status, GTK_STATE_NORMAL);
                delay = 3;
            }
            
            display_update_request_screen_refresh(TEXT_ENTRY_CHANGED_LEVEL,
                                                  WAVEFORM_TYPING);
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

GtkWidget* gtk_profile_entry_get_namebutton(GtkProfileEntry* profileEntry)
{
    CN_LOGPRINTF("entry");
    g_return_val_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry), NULL);
    return profileEntry->nameButton;
}

GtkWidget* gtk_profile_entry_get_editbutton(GtkProfileEntry* profileEntry)
{
    CN_LOGPRINTF("entry");
    g_return_val_if_fail(IS_GTK_PROFILE_ENTRY(profileEntry), NULL);
    return profileEntry->editButton;
}

