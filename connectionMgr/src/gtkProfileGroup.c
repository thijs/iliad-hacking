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

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <liberdm/display.h>
#include <libergtk/ergtk.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "gtkSettingItem.h"
#include "gtkProfileEntry.h"
#include "gtkProfileGroup.h"
#include "background.h"
#include "erbusy.h"

// internally used routines
static void gtk_profile_group_class_init(GtkProfileGroupClass * klass);
static void gtk_profile_group_init(GtkProfileGroup * profileGroup);
static void gtk_profile_group_create(GtkProfileGroup * profileGroup,
                                 int nProfiles, profileGroupMode_t mode);
// static void gtk_profile_group_set_height(GtkProfileGroup *profileGroup);

static GtkWidget *create_list(GtkProfileGroup* profileGroup);

// signal handlers
static gboolean on_profile_group_name_button_press(GtkWidget* button, 
                                                   GdkEventButton* event, 
                                                   gpointer data);
static gboolean on_profile_group_edit_button_press(GtkWidget* button, 
                                                   GdkEventButton* event, 
                                                   gpointer data);

static void on_profile_group_selection_update(erGtkSelectionGroup *selection,
                                              gpointer button, 
                                              gpointer data);

// creates a new GtkProfileGroup widget
GtkWidget *gtk_profile_group_new(int nProfiles, profileGroupMode_t mode)
{
    GtkProfileGroup *profileGroup = NULL;

    profileGroup = (GtkProfileGroup*)g_object_new(GTK_PROFILE_GROUP_TYPE, 
                                                  NULL);
    gtk_profile_group_create(profileGroup, nProfiles, mode);

    return GTK_WIDGET(profileGroup);
}

void gtk_profile_group_set_mode(GtkProfileGroup* profileGroup, 
                                profileGroupMode_t mode)
{
    CN_LOGPRINTF("entry");
    
    profileGroupMode_t oldMode;

    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));
    g_return_if_fail((mode >= 0) && (mode < modeUndef_e));

    if (mode == deleteMode_e)
    {
        oldMode = gtk_profile_group_get_mode(profileGroup);
        if (oldMode == editMode_e)
        {
            profileGroup->mode = mode;
        }
    }
    else
    {
        profileGroup->mode = mode;
    }
}

profileGroupMode_t gtk_profile_group_get_mode(GtkProfileGroup* profileGroup)
{
    CN_LOGPRINTF("entry");
    
    profileGroupMode_t mode = modeUndef_e;
    
    g_return_val_if_fail(IS_GTK_PROFILE_GROUP(profileGroup), mode);
    
    mode = profileGroup->mode;
    
    return mode;
}

// returns type of this widget
GType gtk_profile_group_get_type(void)
{
    static GType profile_group_type = 0;

    if (!profile_group_type)
    {
        static const GTypeInfo  profile_group_info = {
            sizeof(GtkProfileGroupClass),               /* class_size */
            NULL,                                       /* base_init */
            NULL,                                       /* base_finalize */
            (GClassInitFunc) gtk_profile_group_class_init,/* class_init */
            NULL,                                       /* class_finalize */
            NULL,                                       /* class_data */
            sizeof(GtkProfileGroup),                    /* instance_size */
            0,                                          /* n_preallocs */
            (GInstanceInitFunc) gtk_profile_group_init, /* instance_init */
            0                                           /* value_table */
        };

        profile_group_type = g_type_register_static(GTK_TYPE_EVENT_BOX,
                                                    "ProfileGroup",
                                                    &profile_group_info,
                                                    0);
    }

    return profile_group_type;
}

// class_init:
static void gtk_profile_group_class_init(GtkProfileGroupClass * klass)
{
    // nothing needs to be done here
}

static void gtk_profile_group_init(GtkProfileGroup * profileGroup)
{
    CN_LOGPRINTF("entry");
}

//
// object hierarchy:
// profile_group
//    |--item(gtkSettingsItem)
//       |--background
//          |--alignment
//             |--vbox
//                |--dialup (vbox)
//                   |--dialupLabel
//                   |--dialupSigHbox
//                      |--dialupSigImage[5]
//                |--label
//                |--list(network profiles list)
//
static void gtk_profile_group_create(GtkProfileGroup *profileGroup,
                                     int nProfiles, 
                                     profileGroupMode_t mode)
{
    CN_LOGPRINTF("entry");

    GtkWidget* item;
    GtkWidget* background;
    GtkWidget* alignment;
    GtkWidget* vbox;
    GtkWidget* dialup;
    GtkWidget* dialupLabel;
    GtkWidget* dialupSigHbox;
    GtkWidget* dialupSigImage;
    GtkWidget* label;
    GtkWidget* list;

    CN_LOGPRINTF("nProfiles[%d] mode[%d]", nProfiles, mode);

    g_return_if_fail((mode == editMode_e) || (mode == connectMode_e));

    // set the values of profileGroup
    profileGroup->nProfiles = nProfiles;
    profileGroup->mode = mode;

    // item
    item = gtk_settingitem_new("");
    gtk_container_add(GTK_CONTAINER(profileGroup), item);
    gtk_widget_show(item);

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "profile_group_background");
    gtk_widget_set_size_request(background,
                                PROFILE_GROUP_WIDTH,
                                PROFILE_GROUP_HEIGHT);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), background);
    gtk_widget_show(background);

    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              PROFILE_GROUP_PADDING_TOP,
                              PROFILE_GROUP_PADDING_BOTTOM,
                              PROFILE_GROUP_PADDING_LEFT,
                              PROFILE_GROUP_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);
    gtk_widget_show(alignment);

    // vbox
    vbox = gtk_vbox_new(FALSE, PROFILE_GROUP_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    gtk_widget_show(vbox);

    // dialup
    dialup = gtk_vbox_new(FALSE, PROFILE_GROUP_DIALUP_SPACING);
    gtk_box_pack_start(GTK_BOX(vbox), dialup, FALSE, FALSE, 0);

    // dialupLabel
    dialupLabel = gtk_label_new("");
    gtk_widget_set_name (dialupLabel, "profile_group_info_text");
    gtk_widget_set_size_request(dialupLabel, 
            (-1), PROFILE_GROUP_DIALUP_LABEL_HEIGHT);
    gtk_label_set_justify (GTK_LABEL(dialupLabel), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(dialupLabel), 0.0, 0.0);
    gtk_box_pack_start (GTK_BOX (dialup), dialupLabel, FALSE, FALSE, 0);
    gtk_widget_show(dialupLabel);

    // signal quality images.
    dialupSigHbox = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(GTK_WIDGET(dialupSigHbox), 
                                PROFILE_GROUP_DIALUP_SIGIMG_WIDTH, 
                                PROFILE_GROUP_DIALUP_SIGIMG_HEIGHT);
    gtk_box_pack_start(GTK_BOX(dialup), dialupSigHbox, FALSE, FALSE, 0);
    gtk_widget_show(dialupSigHbox);

    // signal images
    int  i;
    char bgStyle[] = "profile_entry_signal0_background";
    for (i = 0; i < 5; i++)
    {
        dialupSigImage = gtk_event_box_new();
        gtk_widget_set_size_request(GTK_WIDGET(dialupSigImage), 
                                    PROFILE_GROUP_DIALUP_SIGIMG_WIDTH, 
                                    PROFILE_GROUP_DIALUP_SIGIMG_HEIGHT);
        snprintf(bgStyle, sizeof(bgStyle), 
                "profile_entry_signal%d_background", i);
        gtk_widget_set_name(GTK_WIDGET(dialupSigImage), bgStyle);
        gtk_box_pack_start(GTK_BOX(dialupSigHbox), dialupSigImage, 
                           FALSE, FALSE, 0);
        profileGroup->dialupSigImage[i] = dialupSigImage;    
    }

    // label
    label = gtk_label_new("");
    gtk_widget_set_name (label, "profile_group_info_text");
    gtk_widget_set_size_request(GTK_WIDGET(label), 
                                PROFILE_GROUP_INFO_WIDTH,
                                (-1) );
    gtk_label_set_justify (GTK_LABEL(label), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
    gtk_widget_show(label);

    // list
    list = create_list(profileGroup);
    gtk_box_pack_start(GTK_BOX(vbox), list, FALSE, FALSE, 0);
    gtk_widget_show(list);

    // set the values of profileGroup
    profileGroup->item = item;
    profileGroup->background = background;
    profileGroup->infoLabel = label;

    profileGroup->dialup = dialup;
    profileGroup->dialupLabel = dialupLabel;
    profileGroup->dialupSigHbox = dialupSigHbox;

//    gtk_profile_group_set_height(profileGroup);
 
    CN_LOGPRINTF("done");
}

//
// |--vbox
//    |--profile1...N(gtkProfileEntry)
//
static GtkWidget *create_list (GtkProfileGroup* profileGroup)
{
    GtkWidget *vbox;
    GtkWidget *profile;
    GtkWidget *selection;
    GtkWidget *nameButton, *editButton;
    GtkToggleButton **button_tbl;
    GtkProfileEntry **profileEntries;
    int               i;

    // malloc the memory
    CN_LOGPRINTF("profileGroup->nProfiles=%d", profileGroup->nProfiles);
    profileEntries = g_new0(GtkProfileEntry*, profileGroup->nProfiles);
    g_assert(profileEntries != NULL);
    button_tbl = g_new0(GtkToggleButton*, (profileGroup->nProfiles+1));
    g_assert(button_tbl != NULL);

    // vbox
    vbox = gtk_vbox_new(FALSE, PROFILE_GROUP_ENTRY_SPACING);

    // profile1...N
    for (i = 0; i < profileGroup->nProfiles; i++)
    {
        profile = gtk_profile_entry_new ();
        gtk_box_pack_start (GTK_BOX (vbox), profile, FALSE, FALSE, 0);
        profileEntries[i] = GTK_PROFILE_ENTRY(profile);
        
        editButton = gtk_profile_entry_get_editbutton(profileEntries[i]);
        nameButton = gtk_profile_entry_get_namebutton(profileEntries[i]);
        g_signal_connect(G_OBJECT(nameButton), "button-press-event", 
              G_CALLBACK(on_profile_group_name_button_press), profileGroup);
        g_signal_connect(G_OBJECT(editButton), "button-press-event", 
              G_CALLBACK(on_profile_group_edit_button_press), profileGroup);

        // selection
        if (profileGroup->mode == editMode_e)
        {
            gtk_profile_entry_show_edit_button(profileEntries[i], TRUE);
            button_tbl[i] = GTK_TOGGLE_BUTTON (editButton);
        }
        else
        {
            gtk_profile_entry_show_edit_button(profileEntries[i], FALSE);
            button_tbl[i] = GTK_TOGGLE_BUTTON (nameButton);
        }
    }

    // selection
    button_tbl[profileGroup->nProfiles] = NULL;
    selection = ergtk_selection_group_new (button_tbl);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(selection), 
                                      0, 1);

    g_signal_connect (G_OBJECT (selection), "selection-update",
          G_CALLBACK (on_profile_group_selection_update), profileGroup);

    // free the memory
    g_free(button_tbl);
    button_tbl = NULL;

    // set the values of profileGroup
    profileGroup->profileEntries = profileEntries;
    profileGroup->selection = selection;
    profileGroup->nDisplayed  = 0;

    return vbox;
}

// set the info/help text associated with this "profile list"
void gtk_profile_group_set_info_text(GtkProfileGroup * profileGroup,
                                     gchar * infoText)
{
    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));

    if (infoText)
    {
        CN_LOGPRINTF("infoLabel [%s]", infoText);

        gtk_label_set_text(GTK_LABEL(profileGroup->infoLabel), infoText);
        gtk_widget_show(profileGroup->infoLabel);
    }
}

// set the header label text
void gtk_profile_group_set_header_text(GtkProfileGroup * profileGroup,
                                       gchar * headerText)
{
    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));

    if (headerText)
    {
        CN_LOGPRINTF("headerLabel [%s]", headerText);

        gtk_settingitem_set_header_text(GTK_SETTINGITEM(profileGroup->item),
                                        headerText);
    }
}

void gtk_profile_group_set_dialup_text(GtkProfileGroup *profileGroup,
                                       gchar* text)
{
    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));

    if (text)
    {
        CN_LOGPRINTF("text [%s]", text);
        gtk_label_set_text(GTK_LABEL(profileGroup->dialupLabel), text);
    }
}

void gtk_profile_group_set_editbuttons_text(GtkProfileGroup *profileGroup,
                                            gchar* editText)
{
    int              i;
    GtkProfileEntry* profileEntry;

    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));

    if (editText)
    {
        CN_LOGPRINTF("editButton [%s]", editText);

        for ( i= 0; i < profileGroup->nProfiles; i++)
        {
            profileEntry = profileGroup->profileEntries[i];
            gtk_profile_entry_set_editbutton_text(profileEntry, editText);
        }
    }
}

// get the current selected profile index
gint gtk_profile_group_get_selected_profile_index(GtkProfileGroup *profileGroup)
{
    guint                profileIndex;
    erGtkSelectionGroup *selection;

    g_return_val_if_fail(IS_GTK_PROFILE_GROUP(profileGroup), -1);
    g_return_val_if_fail(ERGTK_IS_SELECTION_GROUP(profileGroup->selection), -1);

    selection = ERGTK_SELECTION_GROUP(profileGroup->selection);
    ergtk_selection_group_get_selected_buttons(selection, &profileIndex, 1);
    if ((profileIndex >= 0) && (profileIndex < profileGroup->nDisplayed))
    {
        CN_LOGPRINTF("profileIndex=%d", profileIndex);
        return profileIndex;
    }
    else
    {
        return (-1);
    }
}

// Get profile-item from the list
static GtkProfileEntry* gtk_profile_group_get_profile_entry(GtkProfileGroup *profileGroup,
                                                     gint profileIndex)
{
    CN_LOGPRINTF("entry [%p] [%d]", profileGroup, profileIndex);

    GtkProfileEntry *profileEntry = NULL;

    g_return_val_if_fail(IS_GTK_PROFILE_GROUP(profileGroup), NULL);

    CN_LOGPRINTF("entry profile[%d]", profileGroup->nProfiles);

    if ((profileIndex >= 0) && (profileIndex < profileGroup->nProfiles))
    {
        CN_LOGPRINTF("entry profileEntries[%p]", 
                     profileGroup->profileEntries);
        profileEntry = profileGroup->profileEntries[profileIndex];
    }

    CN_LOGPRINTF("ret[%p]", profileEntry);
    return profileEntry;
}

// set the status text for the 'profileIndex'th profileEntry
void gtk_profile_group_set_status_text(GtkProfileGroup* profileGroup,
                                       int profileIndex, 
                                       gchar* text, 
                                       gboolean flashing)
{
    GtkProfileEntry *profileEntry;

    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));
    g_return_if_fail((profileIndex >= 0) && (profileIndex < profileGroup->nProfiles));
    g_return_if_fail(text != NULL );

    profileEntry = gtk_profile_group_get_profile_entry(profileGroup, 
                                                       profileIndex);
    g_return_if_fail(profileEntry != NULL);
 
    gtk_profile_entry_set_status (profileEntry, text);
    gtk_profile_entry_set_status_flashing (profileEntry, flashing);
}

// display settings for the 'profileIndex'th profileEntry
void gtk_profile_group_display_settings(GtkProfileGroup* profileGroup,
                                        int profileIndex,
                                        const networkProfile_t* settings,
                                        gboolean resetStatus)
{
    CN_LOGPRINTF("entry");

    GtkProfileEntry *profileEntry;

    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));
    g_return_if_fail((profileIndex >= 0) && (profileIndex < profileGroup->nProfiles));
    g_return_if_fail(settings != NULL );
   
    profileEntry = gtk_profile_group_get_profile_entry(profileGroup, 
                                                       profileIndex);
    g_return_if_fail(profileEntry != NULL);

    if (resetStatus)
    {
        gtk_profile_entry_display_settings (profileEntry, settings, "");
    }
    else
    {
        gtk_profile_entry_display_settings (profileEntry, settings, NULL);
    }
    gtk_profile_entry_set_buttons_active(profileEntry, FALSE, FALSE);
}

void gtk_profile_group_show_dialup(GtkProfileGroup *profileGroup, 
                                   gboolean show)
{
    CN_LOGPRINTF("entry profileGroup[%p] show[%d]", profileGroup, show);

    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));
    
    if (show)
    {
        gtk_widget_show(profileGroup->dialup);
    }
    else
    {
        gtk_widget_hide(profileGroup->dialup);
    }
}

void gtk_profile_group_set_dialup_signal_image(GtkProfileGroup *profileGroup, 
                                               guint quality)
{
    CN_LOGPRINTF("entry profileGroup[%p] quality[%d]", 
                  profileGroup, quality);

    int i;

    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));
    g_return_if_fail((quality >= 0) && (quality <= 100));

    // hide all signal images firstly
    for (i = 0; i < 5; i++)
    {
        gtk_widget_hide(profileGroup->dialupSigImage[i]);
    }

    // show one correct signal image secondly
    if (quality <= 0)
    {
        gtk_widget_show(profileGroup->dialupSigImage[0]);
    }
    else if (quality <= 25)
    {
        gtk_widget_show(profileGroup->dialupSigImage[1]);
    }
    else if (quality <= 50)
    {
        gtk_widget_show(profileGroup->dialupSigImage[2]);
    }
    else if (quality <= 75)
    {
        gtk_widget_show(profileGroup->dialupSigImage[3]);
    }
    else
    {
        gtk_widget_show(profileGroup->dialupSigImage[4]);
    }
}

// show profile-entries from the list
void gtk_profile_group_show_profile_entries(GtkProfileGroup *profileGroup,
                                            guint nDisplayed)
{
    int i;
    CN_LOGPRINTF("entry nDisplayed[%d]", nDisplayed);
    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));

    i = 0;
    while (i < nDisplayed  &&  i < profileGroup->nProfiles)
    {
        gtk_widget_show(GTK_WIDGET(profileGroup->profileEntries[i]));
        i++;
    }
    while (i < profileGroup->nProfiles)
    {
        gtk_widget_hide(GTK_WIDGET(profileGroup->profileEntries[i]));
        i++;
    }

    // set the values of profileGroup
    profileGroup->nDisplayed = nDisplayed;

//    gtk_profile_group_set_height(profileGroup);
}

void gtk_profile_group_callback_on_buttons_clicked( GtkProfileGroup *profileGroup,
                             on_profile_group_selection_update_t* on_name,
                             on_profile_group_selection_update_t* on_edit)
{
    CN_LOGPRINTF("entry");
    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));

    profileGroup->callback_on_name = on_name;
    profileGroup->callback_on_edit = on_edit;
}

void gtk_profile_group_callback_on_buttons_press( GtkProfileGroup *profileGroup,
                           on_profile_group_button_press_t* on_name_press,
                           on_profile_group_button_press_t* on_edit_press)
{
    CN_LOGPRINTF("entry");
    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));

    profileGroup->callback_on_name_press = on_name_press;
    profileGroup->callback_on_edit_press = on_edit_press;
}

/*
static void gtk_profile_group_set_height(GtkProfileGroup *profileGroup)
{
    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));

    int height, listHeight;

    CN_LOGPRINTF("profileGroup->nDisplayed=%d", profileGroup->nDisplayed);
    if (profileGroup->nDisplayed > 0)
    {
        listHeight = profileGroup->nDisplayed * PROFILE_ENTRY_HEIGHT +
                     (profileGroup->nDisplayed - 1) * PROFILE_GROUP_ENTRY_SPACING;
    }
    else
    {
        listHeight = 0;
    }

    CN_LOGPRINTF("listHeight=%d", listHeight);

    height = PROFILE_GROUP_PADDING_TOP +
             PROFILE_GROUP_INFO_HEIGHT +
             PROFILE_GROUP_SPACING +
             listHeight +
             PROFILE_GROUP_PADDING_BOTTOM;

    CN_LOGPRINTF("height=%d", height);
    gtk_widget_set_size_request(GTK_WIDGET(profileGroup->background), (-1), height);
}
*/

////////////////////////////////////////////////////////////////////////////
// signal handlers
////////////////////////////////////////////////////////////////////////////
static int get_button_index(GtkProfileGroup* profileGroup, 
                            GtkWidget* button, 
                            gboolean edit)
{
    GtkProfileEntry*     profileEntry;
    GtkWidget*           nameEditBtn;
    gint                 i, profileIndex = -1;
 
    for (i = 0; i < profileGroup->nProfiles; i++)
    {
        profileEntry = profileGroup->profileEntries[i];
        if (edit)
        {
            nameEditBtn = gtk_profile_entry_get_editbutton(profileEntry);
        }
        else
        {
            nameEditBtn = gtk_profile_entry_get_namebutton(profileEntry);
        }

        if (nameEditBtn == button)
        {
            profileIndex = i;
            CN_LOGPRINTF("button press event: profileIndex=%d", 
                          profileIndex);
            break;
        }
    }

    return profileIndex;
}

static gboolean on_profile_group_name_button_press(GtkWidget* button, 
                                                   GdkEventButton* event, 
                                                   gpointer data)
{
    CN_LOGPRINTF("entry");

    GtkProfileGroup*     profileGroup;
    gint                 profileIndex = -1;
    gboolean             active;
    gboolean             ret = FALSE;

    g_return_val_if_fail(GTK_IS_TOGGLE_BUTTON(button), FALSE);
    g_return_val_if_fail(IS_GTK_PROFILE_GROUP(data), FALSE);

    profileGroup = GTK_PROFILE_GROUP(data);
    profileIndex = get_button_index(profileGroup, button, FALSE);
    g_return_val_if_fail(profileIndex != -1, FALSE);

    active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));
    switch (profileGroup->mode)
    {
        case editMode_e:
            // make the name buttons in edit mode un-clicked
            ret = TRUE;
            break;
        case connectMode_e:
            if (profileGroup->callback_on_name_press)
            {
                ret = profileGroup->callback_on_name_press(profileGroup, 
                                                           profileIndex, 
                                                           active);
            }
            break;
        case deleteMode_e:
            if (profileGroup->callback_on_name_press)
            {
                ret = profileGroup->callback_on_name_press(profileGroup, 
                                                           profileIndex, 
                                                           active);
            }
            break;
        default:
            break;
    }

    return ret;
}

static gboolean on_profile_group_edit_button_press(GtkWidget* button, 
                                                   GdkEventButton* event, 
                                                   gpointer data)
{
    CN_LOGPRINTF("entry");

    GtkProfileGroup*     profileGroup;
    gint                 profileIndex = -1;
    gboolean             active;
    gboolean             ret = FALSE;

    g_return_val_if_fail(GTK_IS_TOGGLE_BUTTON(button), FALSE);
    g_return_val_if_fail(IS_GTK_PROFILE_GROUP(data), FALSE);

    profileGroup = GTK_PROFILE_GROUP(data);
    profileIndex = get_button_index(profileGroup, button, TRUE);
    g_return_val_if_fail(profileIndex != -1, FALSE);

    active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));
    switch (profileGroup->mode)
    {
        case editMode_e:
            ret = FALSE;
            break;
        case connectMode_e:
            ret = TRUE;
            break;
        case deleteMode_e:
            ret = TRUE;
            break;
        default:
            break;
    }

    return ret;
}

static void on_profile_group_selection_update(erGtkSelectionGroup *selection,
                                              gpointer button,
                                              gpointer data)
{
    CN_LOGPRINTF("entry");

    GtkProfileGroup*     profileGroup;
    guint                profileIndex;
    GtkToggleButton*     btn;
    gboolean             active;

    g_return_if_fail(ERGTK_IS_SELECTION_GROUP(selection));
    g_return_if_fail(GTK_IS_TOGGLE_BUTTON(button));
    g_return_if_fail(IS_GTK_PROFILE_GROUP(data));

    profileGroup = GTK_PROFILE_GROUP(data);
    if (profileGroup->mode == deleteMode_e)
    {
        CN_WARNPRINTF("in delete mode, ignore selection-update signal");
    }
 
    btn = GTK_TOGGLE_BUTTON(button);
    active = gtk_toggle_button_get_active(btn);
    if (active)
    {
        ergtk_selection_group_get_selected_buttons(selection, 
                                                   &profileIndex, 1);
        g_return_if_fail((profileIndex >= 0) && (profileIndex < profileGroup->nDisplayed));

        if (profileGroup->mode == editMode_e)
        {
            if (profileGroup->callback_on_edit)
            {
                profileGroup->callback_on_edit(profileGroup, profileIndex);
            }
        }
        else
        {
            if (profileGroup->callback_on_name)
            {
                profileGroup->callback_on_name(profileGroup, profileIndex);
            }
        }
    }
}

// key-event received
guint on_profile_group_keypress(GtkProfileGroup* profileGroup, 
                                GdkEventKey * event)
{
    CN_LOGPRINTF ("entry");

    gboolean selected;
    gboolean displayUpdated = FALSE;
    guint    returnValue = 0;// return FALSE => default gtk handling

    switch (event->keyval)
    {
        case GDK_Down:
            // select next profile
            selected = gtk_profile_group_select_next_profile (profileGroup);
            if (!selected)
            {
                gtk_profile_group_select_first_profile (profileGroup);
            }
            displayUpdated = TRUE;
            // return TRUE => stop event handling
            returnValue = 1;
            break;

        case GDK_Up:
            // select previous profile
            selected = gtk_profile_group_select_previous_profile(profileGroup);
            if (!selected)
            {
                gtk_profile_group_select_last_profile (profileGroup);
            }
            displayUpdated = TRUE;
            //return TRUE => stop event handling
            returnValue = 1;
            break;

        case GDK_Return:// TODO
        default:
            CN_ERRORPRINTF ("-- case default");
            erbusy_off ();
    }

    // show the changes on screen
    if (displayUpdated)
    {
        display_update_request_screen_refresh (STATUS_ITEM_CHANGE, 
                                               WAVEFORM_TYPING);
    }

    return returnValue;
}

///////////////////////////////////////////////////////////////////////////////
//
// manage the indice of profiles
//
///////////////////////////////////////////////////////////////////////////////
void gtk_profile_group_select_first_profile (GtkProfileGroup* profileGroup)
{
    CN_LOGPRINTF ("entry");

    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));
    if (profileGroup->nDisplayed > 0)
    {
        gtk_profile_group_select_profile (profileGroup, 0);
    }
}

void gtk_profile_group_select_last_profile (GtkProfileGroup* profileGroup)
{
    CN_LOGPRINTF ("entry");

    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));
    if (profileGroup->nDisplayed > 0)
    {
        gtk_profile_group_select_profile (profileGroup, 
                                profileGroup->nDisplayed-1);
    }
}

gboolean gtk_profile_group_select_next_profile (GtkProfileGroup* profileGroup)
{
    CN_LOGPRINTF ("entry");

    erGtkSelectionGroup *selection;
    guint    profileIndex;
    gboolean selected = FALSE;    // no profile selected (yet)

    g_return_val_if_fail(IS_GTK_PROFILE_GROUP(profileGroup), FALSE);

    selection = ERGTK_SELECTION_GROUP(profileGroup->selection);
    ergtk_selection_group_get_selected_buttons(selection,
                                               &profileIndex, 1);
    if ((profileIndex >= 0) && (profileIndex < profileGroup->nDisplayed))
    {
        // select next profile
        if ((profileIndex + 1) < profileGroup->nDisplayed)
        {
            gtk_profile_group_select_profile(profileGroup, 
                                             profileIndex + 1);
            selected = TRUE;
        }
    }
    return selected;
}

gboolean gtk_profile_group_select_previous_profile (GtkProfileGroup* profileGroup)
{
    CN_LOGPRINTF ("entry");

    erGtkSelectionGroup *selection;
    guint    profileIndex;
    gboolean selected = FALSE;    // no profile selected (yet)

    g_return_val_if_fail(IS_GTK_PROFILE_GROUP(profileGroup), FALSE);

    selection = ERGTK_SELECTION_GROUP(profileGroup->selection);
    ergtk_selection_group_get_selected_buttons(selection, &profileIndex, 1);
    if ((profileIndex >= 0) && (profileIndex < profileGroup->nDisplayed))
    {
        // select previous profile
        if ((profileIndex - 1) > 0)
        {
            gtk_profile_group_select_profile(profileGroup, profileIndex-1);
            selected = TRUE;
        }
    }
    return selected;
}

void gtk_profile_group_select_profile (GtkProfileGroup* profileGroup,
                                       guint profileIndex)
{
    CN_LOGPRINTF ("entry: profileIndex [%d]", profileIndex);

    erGtkSelectionGroup *selection;
    guint            selectedIndex;
    GtkProfileEntry *profileEntry;

    g_return_if_fail(IS_GTK_PROFILE_GROUP(profileGroup));
    
    // specified profile must be valid range for current screen
    if ((profileIndex >= 0) && (profileIndex < profileGroup->nProfiles))
    {
        profileEntry = profileGroup->profileEntries[profileIndex];
        g_return_if_fail(profileEntry != NULL);

        if (profileGroup->mode == deleteMode_e)
        {
            gtk_profile_entry_set_buttons_active(profileEntry, TRUE, FALSE);
        }
        else
        {
            // get the current selection
            selection = ERGTK_SELECTION_GROUP(profileGroup->selection);
            ergtk_selection_group_get_selected_buttons(selection,
                                                       &selectedIndex, 1);
            if (profileIndex != selectedIndex)
            {
                if (profileGroup->mode == editMode_e)
                {
                    gtk_profile_entry_set_buttons_active(profileEntry, 
                                                         FALSE, TRUE);
                }
                else
                {
                    gtk_profile_entry_set_buttons_active(profileEntry, 
                                                         TRUE, FALSE);
                }
            }
        }
    }
}

void gtk_profile_group_unselect_profile (GtkProfileGroup* profileGroup,
                                         guint profileIndex)
{
    CN_LOGPRINTF ("entry");

    GtkProfileEntry* profileEntry;

    if ((profileIndex >= 0) && (profileIndex < profileGroup->nProfiles))
    {
        profileEntry = profileGroup->profileEntries[profileIndex];
        if (profileEntry)
        {
            gtk_profile_entry_set_buttons_active(profileEntry, 
                                                 FALSE, FALSE);
        }
    }
}

