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
 * \file gtkProfileGroup.c
 * \brief connectionMgr - the network-profile group
 *                        containing 
 *                        a header label 
 *                        a information label 
 *                        a list of gtkProfileEntry 
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _PROFILE_GROUP_H_
#define _PROFILE_GROUP_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>
#include "gtkProfileEntry.h"

#define PROFILE_GROUP_WIDTH               666
#define PROFILE_GROUP_HEIGHT              547

#define PROFILE_GROUP_SPACING             27

#define PROFILE_GROUP_PADDING_TOP         10
#define PROFILE_GROUP_PADDING_BOTTOM      10  
#define PROFILE_GROUP_PADDING_LEFT        13
#define PROFILE_GROUP_PADDING_RIGHT       13 

#define PROFILE_GROUP_DIALUP_LABEL_WIDTH   666
#define PROFILE_GROUP_DIALUP_LABEL_HEIGHT  27

#define PROFILE_GROUP_DIALUP_SPACING       12
#define PROFILE_GROUP_DIALUP_SIGIMG_WIDTH  37
#define PROFILE_GROUP_DIALUP_SIGIMG_HEIGHT 27

#define PROFILE_GROUP_INFO_WIDTH          640
#define PROFILE_GROUP_INFO_HEIGHT         27

#define PROFILE_GROUP_ENTRY_SPACING       12  

#define GTK_PROFILE_GROUP_TYPE  (gtk_profile_group_get_type ())
#define GTK_PROFILE_GROUP(obj)  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                 GTK_PROFILE_GROUP_TYPE, GtkProfileGroup))
#define GTK_PROFILE_GROUP_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                        GTK_PROFILE_GROUP_TYPE, \
                                        GtkProfileGroupClass))
#define IS_GTK_PROFILE_GROUP(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                   GTK_PROFILE_GROUP_TYPE))
#define IS_GTK_PROFILE_GROUP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                           GTK_PROFILE_GROUP_TYPE))

typedef enum
{
    editMode_e = 0,
    connectMode_e,
    deleteMode_e,
    modeUndef_e
}profileGroupMode_t;

typedef struct _GtkProfileGroup       GtkProfileGroup;
typedef struct _GtkProfileGroupClass  GtkProfileGroupClass;

typedef void on_profile_group_selection_update_t(GtkProfileGroup* profileGroup, 
                                                 guint profileIndex);
typedef gboolean on_profile_group_button_press_t(GtkProfileGroup* profileGroup, 
                                                 guint profileIndex,
                                                 gboolean active);

struct _GtkProfileGroup
{
    GtkEventBox        eventbox;         // parent class
    GtkWidget*         item;             // gtkSettingsItem 
    GtkWidget*         background;
    GtkWidget*         dialup;           // vbox, container of 
                                         // the dialup signal widgets 
    GtkWidget*         dialupLabel; 
    GtkWidget*         dialupSigHbox;    
    GtkWidget*         dialupSigImage[5];
    GtkWidget*         infoLabel;
    GtkWidget*         selection; // use erGtkSelectionGroup 
                                  // to manage the list of GtkProfileEntry  
    profileGroupMode_t mode;                                  
    guint              nProfiles;
    guint              nDisplayed;
    GtkProfileEntry**  profileEntries;

    on_profile_group_selection_update_t* callback_on_name;
    on_profile_group_selection_update_t* callback_on_edit;
    on_profile_group_button_press_t*     callback_on_name_press;
    on_profile_group_button_press_t*     callback_on_edit_press;
};

struct _GtkProfileGroupClass
{
    GtkEventBoxClass parent_class;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    // creates a new GtkProfileGroup widget with 'nProfiles' GtkProfileEntry
    GtkWidget* gtk_profile_group_new(int nProfiles, 
                                     profileGroupMode_t mode);

    // set/get the GtkProfileGroup mode
    void gtk_profile_group_set_mode(GtkProfileGroup* profileGroup, 
                                    profileGroupMode_t mode);
    profileGroupMode_t gtk_profile_group_get_mode(GtkProfileGroup* profileGroup);

    // set the header label text
    void gtk_profile_group_set_header_text(GtkProfileGroup *profileGroup, 
                                           gchar* headerText);

    // set the info/help text associated with dialup signal 
    void gtk_profile_group_set_dialup_text(GtkProfileGroup *profileGroup,
                                           gchar* text);

    // set the info/help text associated with this "profile list"
    void gtk_profile_group_set_info_text(GtkProfileGroup *profileGroup, 
                                         gchar* infoText); 

    // set all edit buttons's text
    void gtk_profile_group_set_editbuttons_text(GtkProfileGroup *profileGroup,
                                                gchar* editText);

    void gtk_profile_group_set_status_text(GtkProfileGroup* profileGroup,
                                           int profileIndex, 
                                           gchar* text, 
                                           gboolean flashing);

    // display settings for the 'profileIndex'th profileEntry
    void gtk_profile_group_display_settings(GtkProfileGroup* profileGroup,
                                        int profileIndex,
                                        const networkProfile_t* settings,
                                        gboolean resetStatus);

    // get the current selected profile index
    gint gtk_profile_group_get_selected_profile_index(GtkProfileGroup *profileGroup);

    // show the dialup signal widget 
    void gtk_profile_group_show_dialup(GtkProfileGroup *profileGroup, 
                                       gboolean show);

    // display the dialup signal quality
    void gtk_profile_group_set_dialup_signal_image(GtkProfileGroup *profileGroup, 
                                                   guint quality);

    // show profile-entries from the list
    void gtk_profile_group_show_profile_entries(GtkProfileGroup *profileGroup, 
                                                guint nDisplayed);

    // set callback functions
    void gtk_profile_group_callback_on_buttons_clicked( GtkProfileGroup *profileGroup,
                             on_profile_group_selection_update_t* on_name,
                             on_profile_group_selection_update_t* on_edit);

    void gtk_profile_group_callback_on_buttons_press( GtkProfileGroup *profileGroup,
                         on_profile_group_button_press_t* on_name_press,
                         on_profile_group_button_press_t* on_edit_press);

    guint on_profile_group_keypress (GtkProfileGroup* profileGroup, GdkEventKey * event);

    void gtk_profile_group_select_first_profile (GtkProfileGroup* profileGroup);
    void gtk_profile_group_select_last_profile (GtkProfileGroup* profileGroup);
    gboolean gtk_profile_group_select_next_profile (GtkProfileGroup* profileGroup);
    gboolean gtk_profile_group_select_previous_profile (GtkProfileGroup* profileGroup);

    void gtk_profile_group_select_profile (GtkProfileGroup* profileGroup,
                                           guint profileIndex);
    void gtk_profile_group_unselect_profile (GtkProfileGroup* profileGroup, 
                                             guint profileIndex);

    // returns type of this widget 
    GType gtk_profile_group_get_type (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_PROFILE_GROUP_H_

