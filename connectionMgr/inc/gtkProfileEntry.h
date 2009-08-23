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
 * \file gtkProfileEntry.h
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
#ifndef _GTK_PROFILE_ENTRY_H_
#define _GTK_PROFILE_ENTRY_H_

#include <gtk/gtk.h>
#include <glib.h>
#include "commonData.h"

#define PROFILE_ENTRY_WIDTH                    666
#define PROFILE_ENTRY_HEIGHT                   27

#define PROFILE_ENTRY_SPACING                  20

#define PROFILE_ENTRY_NAME_BUTTON_WIDTH        310
#define PROFILE_ENTRY_NAME_BUTTON_HEIGHT       27

#define PROFILE_ENTRY_SIG_IMG_WIDTH            37 
#define PROFILE_ENTRY_SIG_IMG_HEIGHT           27  

#define PROFILE_ENTRY_ENCR_IMG_WIDTH           40 
#define PROFILE_ENTRY_ENCR_IMG_HEIGHT          27  

#define PROFILE_ENTRY_STATUS_WIDTH             (-1)
#define PROFILE_ENTRY_STATUS_HEIGHT            27

#define PROFILE_ENTRY_EDIT_BUTTON_WIDTH        130
#define PROFILE_ENTRY_EDIT_BUTTON_HEIGHT       27  

#define GTK_PROFILE_ENTRY_TYPE (gtk_profile_entry_get_type ())
#define GTK_PROFILE_ENTRY(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                GTK_PROFILE_ENTRY_TYPE, GtkProfileEntry))
#define GTK_PROFILE_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                        GTK_PROFILE_ENTRY_TYPE, \
                                        GtkProfileEntryClass))
#define IS_GTK_PROFILE_ENTRY(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                   GTK_PROFILE_ENTRY_TYPE))
#define IS_GTK_PROFILE_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                           GTK_PROFILE_ENTRY_TYPE))

typedef struct _GtkProfileEntry       GtkProfileEntry;
typedef struct _GtkProfileEntryClass  GtkProfileEntryClass;

struct _GtkProfileEntry
{
    GtkEventBox   eventbox;      //parent class

    // private
    GtkWidget*    clientArea;  // hbox, container of all widgets
    GtkWidget*    nameButton;  // button, show profile name or SSID on it
    GtkWidget*    sigVbox;     // vbox, container of signal quality images 
    GtkWidget*    sigImage[5]; // event box, one image stands for 
                               // one signal quality level
    GtkWidget*    encrVbox;      // vbox, container of encryption image  
    GtkWidget*    encrImage;     // event box, encryption image
    GtkWidget*    status;        // label, show status text on it
    gboolean      statusFlashing;// status flashing or not 
    GtkWidget*    editButton; // button, click it to launch profile editor
};

struct _GtkProfileEntryClass
{
    GtkEventBoxClass parent_class;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    // creates a new GtkProfileEntry widget 
    GtkWidget* gtk_profile_entry_new(void);

    // fill client area with profile settings
    void gtk_profile_entry_display_settings(GtkProfileEntry *profileEntry, 
                                        const networkProfile_t *settings, 
                                        gchar *status);

    void gtk_profile_entry_set_namebutton_text(GtkProfileEntry *profileEntry, 
                                               gchar *nameText);
    void gtk_profile_entry_set_signal_image(GtkProfileEntry *profileEntry, 
                                            gint quality);
    void gtk_profile_entry_set_status(GtkProfileEntry *profileEntry, 
                                      gchar *statusText);
    void gtk_profile_entry_set_status_flashing(GtkProfileEntry *profileEntry, 
                                               gboolean flashing);
    void gtk_profile_entry_set_editbutton_text(GtkProfileEntry *profileEntry, 
                                               gchar* editText);

    // show/hide widgets
    void gtk_profile_entry_show_encr_image(GtkProfileEntry *profileEntry, 
                                           gboolean show);
    void gtk_profile_entry_show_edit_button(GtkProfileEntry *profileEntry, 
                                            gboolean show);

    // set widget status
    void gtk_profile_entry_set_buttons_active(GtkProfileEntry *profileEntry, 
                                              gboolean nameActive, 
                                              gboolean editActive);

    GtkWidget* gtk_profile_entry_get_namebutton(GtkProfileEntry *profileEntry);
    GtkWidget* gtk_profile_entry_get_editbutton(GtkProfileEntry *profileEntry);

    // returns type of this widget 
    GType gtk_profile_entry_get_type (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_GTK_PROFILE_ENTRY_H_

