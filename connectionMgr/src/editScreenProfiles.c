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
 * \file editScreenProfiles.c
 * \brief connectionMgr - the first page of 'edit' screen
 *                        the edit profiles screen
 *                        containing a new profile entry 
 *                               and a profile list(registry)
 *
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <unistd.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>
#include <liberipc/eripctoolbar.h>

#include "connectionMgrLog.h"
#include "connectionMgr.h"
#include "gtkSettingItem.h"
#include "gtkProfileEntry.h"
#include "gtkProfileGroup.h"
#include "widgetUtils.h"
#include "commonData.h"
#include "editScreenData.h"
#include "editScreen.h"
#include "editScreenProfiles.h"
#include "editScreenOverview.h"
#include "editScreenSearch.h"
#include "background.h"
#include "displayStatus.h"
#include "erbusy.h"
#include "pagebar.h"
#include "toolbar.h"
#include "languages.h"

// data for listitem select timer
typedef void on_item_selected_t(const int* index_tbl);
static struct
{
    int       iconID;           // toolbar icon
    gboolean  confirm_with_icon;// confirm selection by clicking icon again?
    guint     ticks;            // for timer
    guint     timerID;          // for timer
    gboolean  active;           // is selection mode active?
    gboolean  icon_clicked;     // is selected icon clicked again?
    gboolean  item_selected[MAX_PROFILES_PER_PAGE];
    on_item_selected_t* on_item_selected; // callback on item selected
} g_select = { ICON_ID_NOTUSED, FALSE, 0, 0, FALSE, FALSE, {}, NULL };

// vbox in which all edit-screen-profiles items are packed
static GtkWidget *g_edit_profiles = NULL;

// selection: wired or wireless or dialup
static GtkWidget *g_networktype = NULL;
static bkWdtLabelButton *g_nwtype_labelbutton = NULL;

// new profile
static GtkWidget *g_new_profile = NULL;
static bkWdtLabelButton *g_newprofile_labelbutton = NULL;

// list of profiles
static GtkWidget *g_known_profiles = NULL;

static gboolean   g_first_display = TRUE; 

static GtkWidget* create_new_profile(void);
static GtkWidget* create_known_profiles(void);

static GtkWidget *create_network_type (void);

// signal handlers
static void on_networktype_update (erGtkSelectionGroup * selection, 
                                   gpointer button, gpointer data);
static void on_new_profile_toggled (GtkWidget* button,
                                    GdkEventButton * event,
                                    gpointer data);
static void on_profile_entry_selected (GtkProfileGroup *profileGroup, 
                                       guint profileIndex);

static void edit_profile(guint profileIndex);

static void on_icon_clicked(int iconID, int iconState);

static void begin_select_delete_profiles(int iconID, 
                                 gboolean confirm_with_icon, 
                                 on_item_selected_t* callback_on_selected);

static gboolean on_end_select_delete_profiles_timer(gpointer data);
static void select_delete_profiles_stop(gboolean restore_toolbar);
static void unselect_delete_profiles(void);
static gboolean on_delete_profile_selected(GtkProfileGroup* profileGroup, 
                                           guint index, gboolean active);
static void delete_profiles(const int* index_tbl);

////////////////////////////////////////////////////////////////////////////
// create GUI widgets
////////////////////////////////////////////////////////////////////////////

// object hierarchy:
//  vbox
//     |--networkTypeItem
//     |--newProfileItem
//     |--knownProfilesItem
GtkWidget *edit_profiles_create(void)
{
    CN_LOGPRINTF("entry");

    GtkWidget *vbox;
    GtkWidget *networkTypeItem;
    GtkWidget *newProfileItem;
    GtkWidget *knownProfilesItem;

    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_widget_show(vbox);
    g_edit_profiles = vbox;

    // networkTypeItem
    networkTypeItem = create_network_type();
    gtk_box_pack_start(GTK_BOX(vbox), networkTypeItem, FALSE, FALSE, 0);
    gtk_widget_show(networkTypeItem);

    // newProfileItem
    newProfileItem = create_new_profile();
    gtk_box_pack_start(GTK_BOX(vbox), newProfileItem, FALSE, FALSE, 0);
    gtk_widget_show(newProfileItem);

    // knownProfilesItem
    knownProfilesItem = create_known_profiles();
    gtk_box_pack_start(GTK_BOX(vbox), knownProfilesItem, FALSE, FALSE, 0);
    gtk_widget_show(knownProfilesItem);

    return vbox;
}

// |--item(gtkSettingItem)
//    |--widget
static GtkWidget *create_new_profile(void)
{
    GtkWidget *item;
    bkWdtLabelButton *widget;
    GtkToggleButton *button;

    // item
    item = gtk_settingitem_new("");

    // widget
    widget = bk_wdt_label_button_new(1, FALSE, 0, 1);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), widget->background);
    
    // signal handlers
    button = widget->details->buttonList[0];
    g_signal_connect(G_OBJECT(button), "toggled", 
                     G_CALLBACK(on_new_profile_toggled), 0);
    
    // show widgets
    gtk_widget_show(widget->background);

    // global variables
    g_new_profile = item;
    g_newprofile_labelbutton = widget;

    // return value
    return item;
}

//
// |--item(gtkSettingItem)
//    |--widget (bkWdtLabelButton)
static GtkWidget *create_network_type (void)
{
    GtkWidget *item = NULL;
    bkWdtLabelButton *widget;
    GtkWidget *selection;

    // item
    item = gtk_settingitem_new("");
    
    // widget
    widget = bk_wdt_label_button_new(3, FALSE, 1, 1);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), widget->background);

    // signal handlers
    selection = widget->details->selection;
    g_signal_connect (G_OBJECT (selection), "selection-update",
            G_CALLBACK (on_networktype_update), NULL);

    // show widget
    gtk_widget_show(widget->background);

    // global variables
    g_networktype = item;
    g_nwtype_labelbutton = widget;

    // return
    return item;
}

//
//|--item(gtkSettingsItem)
//
static GtkWidget *create_known_profiles (void)
{
    GtkWidget* item;
 
    // item
    item = gtk_profile_group_new(MAX_PROFILES_PER_PAGE, editMode_e);
    gtk_profile_group_show_dialup(GTK_PROFILE_GROUP(item), FALSE);

    gtk_profile_group_callback_on_buttons_press(GTK_PROFILE_GROUP(item), 
                                         on_delete_profile_selected, NULL);
    gtk_profile_group_callback_on_buttons_clicked(GTK_PROFILE_GROUP(item),
                                          NULL, on_profile_entry_selected);
    g_known_profiles = item;

    return item;
}

////////////////////////////////////////////////////////////////////////////
// set the correct text on screen
////////////////////////////////////////////////////////////////////////////
void edit_profiles_set_text (void)
{
    GtkWidget *widget;
    GtkProfileGroup *profileGroup;

    CN_LOGPRINTF ("entry");

    if (g_networktype)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_networktype), 
                                        _("Show network profiles for"));
    }
    
    if (g_nwtype_labelbutton)
    {
        widget = g_nwtype_labelbutton->details->label;
        gtk_label_set_text (GTK_LABEL(widget), 
                _("Select the type of network profiles to list below."));

        widget = GTK_WIDGET(g_nwtype_labelbutton->details->buttonList[0]);
        gtk_button_set_label (GTK_BUTTON (widget), _("Wired"));

        widget = GTK_WIDGET(g_nwtype_labelbutton->details->buttonList[1]);
        gtk_button_set_label (GTK_BUTTON (widget), _("Wireless"));

        widget = GTK_WIDGET(g_nwtype_labelbutton->details->buttonList[2]);
        gtk_button_set_label (GTK_BUTTON (widget), _("Dial-up"));
    }

    if (g_new_profile)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_new_profile), 
                                        _("Network profiles"));
    }

    if (g_newprofile_labelbutton)
    {
        widget = g_newprofile_labelbutton->details->label;
        gtk_label_set_text (GTK_LABEL (widget), 
                _("Create a new network profile:"));

        widget = GTK_WIDGET(g_newprofile_labelbutton->details->buttonList[0]);
        gtk_button_set_label (GTK_BUTTON (widget), _("Create new profile"));
    }

    if (g_known_profiles)
    {
        profileGroup = GTK_PROFILE_GROUP(g_known_profiles);
        gtk_profile_group_set_header_text(profileGroup,
                                         _("Network profiles"));
        gtk_profile_group_set_info_text (profileGroup,
                            _("Stored network profiles on your iLiad: "));
        gtk_profile_group_set_editbuttons_text(profileGroup,
                                               _("Edit"));
    }
}

////////////////////////////////////////////////////////////////////////////
// display the related fields
////////////////////////////////////////////////////////////////////////////
void edit_profiles_display (void)
{
    CN_LOGPRINTF ("entry");
 
    GtkProfileGroup           *profileGroup;
    networkProfile_t          *settings;
    int                       i, profileIndex;
    int                       nProfiles, nDisplayed;

    GtkWidget*                widget;
    GtkToggleButton*          button;

    if (g_nwtype_labelbutton)
    {
        // display choice wired / wireless / dialup
        switch (edit_data_get_network_type())
        {
            case wired_t:
                widget = GTK_WIDGET(g_nwtype_labelbutton->details->buttonList[0]);
                break;
            case wireless_t:
                widget = GTK_WIDGET(g_nwtype_labelbutton->details->buttonList[1]);
                break;
            case dialup_t:
                widget = GTK_WIDGET(g_nwtype_labelbutton->details->buttonList[2]);
                break;
            default:
                // nothing selected (yet), using the default setting: wired
                widget = GTK_WIDGET(g_nwtype_labelbutton->details->buttonList[0]);
                edit_data_set_network_type (wired_t);
                break;
        }

        //   - and select this button
        //     Note: when the button is already selected, 
        //     we de-select then select again;
        //           this ensures the signal handlers are called 
        //           regardless the old button state.
        button = GTK_TOGGLE_BUTTON(widget);
        if (g_first_display && gtk_toggle_button_get_active(button))
        {
            // button is selected, so first deselect it 
            // to make sure signal handler is called
            gtk_toggle_button_set_active(button, FALSE);
        }
        gtk_toggle_button_set_active(button, TRUE);
        
        // Disable buttons for WiFi and/or Dialup based on hardware presence
        if (main_get_wifi_disabled())
        {
            // set wifi button insensitive when wifi is disabled
            widget = GTK_WIDGET(g_nwtype_labelbutton->details->buttonList[1]);
            gtk_widget_set_sensitive(widget, FALSE);
        }

        if (main_get_dialup_disabled())
        {
            // set dialup button insensitive when dialup is disabled
            widget = GTK_WIDGET(g_nwtype_labelbutton->details->buttonList[2]);
            gtk_widget_set_sensitive(widget, FALSE);
        }
    }

    if (g_newprofile_labelbutton)
    {
        button = g_newprofile_labelbutton->details->buttonList[0];

        // grey out the 'Create new profile' button if needed
        if (edit_data_reach_max_profiles())
        {
            gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
        }
        else
        {
            gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE);
        }

        // deselect the 'create new profile' button
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
    }

    // show the corresponding profile entries  
    profileGroup = GTK_PROFILE_GROUP(g_known_profiles);   
    nProfiles = edit_data_get_n_network_profiles();
    CN_LOGPRINTF("nProfiles[%d]", nProfiles);

    nDisplayed = ((nProfiles < MAX_PROFILES_PER_PAGE) ? nProfiles: MAX_PROFILES_PER_PAGE);
    if (nDisplayed > 0)
    {
        for (i = 0; i < nDisplayed; i++)
        {
            settings = (networkProfile_t*)edit_data_get_network_profile(i);
            gtk_profile_group_display_settings(profileGroup, i, 
                                               settings, TRUE);
        }
    }
    gtk_profile_group_show_profile_entries(profileGroup, nDisplayed);

    // deselect all profile index
    profileIndex = gtk_profile_group_get_selected_profile_index(profileGroup);
    if (profileIndex != -1)
    {
        gtk_profile_group_unselect_profile(profileGroup, profileIndex);
    }

    // update the trashcan icon as well
    edit_show_trashcan();


    g_first_display = FALSE;
}

////////////////////////////////////////////////////////////////////////////
// signal handlers 
////////////////////////////////////////////////////////////////////////////
static void on_networktype_update (erGtkSelectionGroup * selection, 
                                   gpointer button, gpointer data)
{
    CN_LOGPRINTF("entry selection[%p], button[%p], data[%p]", 
                 selection, button, data);

    GtkToggleButton* btn;
    gboolean active;
    int nsel;

    btn = GTK_TOGGLE_BUTTON(button);
    active = gtk_toggle_button_get_active(btn);
    if (active)
    {

        erbusy_blink();
 
        // stop the delete mode
        select_delete_profiles_stop(TRUE);

        ergtk_selection_group_get_selected_buttons(selection, &nsel, 1);
        if (nsel == 0)
        {
            // wired selected
            edit_data_set_network_type(wired_t);
        }
        else if (nsel == 1)
        {
            // wireless selected
            edit_data_set_network_type (wireless_t);
        }
        else if (nsel == 2)
        {
            // dialup selected
            edit_data_set_network_type(dialup_t);
        }

        // update the edit data 
        edit_data_init_network_profiles();

        // update the GUI
        edit_profiles_display();
    }
}

static gboolean delayed_on_new_profile_toggled(gpointer data)
{
    GtkToggleButton *button;

    erbusy_blink();
    edit_profiles_create_profile();
    
    button = g_newprofile_labelbutton->details->buttonList[0];
    gtk_toggle_button_set_active(button, FALSE);

    return FALSE;
}

static void on_new_profile_toggled (GtkWidget* button,
                                    GdkEventButton * event,
                                    gpointer data)
{
    gboolean pressed;

    CN_LOGPRINTF ("entry");

    // check if it is a "pressed event"
    pressed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));
    if (pressed)
    {
        // stop the delete mode
        select_delete_profiles_stop(TRUE);

        // wait for a while to show button selected
        g_timeout_add(500, delayed_on_new_profile_toggled, data);
    }
}

static gboolean delayed_on_profile_entry_selected(gpointer data)
{
    int profileIndex = (int)data;

    erbusy_blink();
    edit_profile(profileIndex);
    return FALSE;
}

static void on_profile_entry_selected (GtkProfileGroup* profileGroup, 
                                       guint profileIndex)
{
    CN_LOGPRINTF ("entry");

    // wait for a while to show button selected
    g_timeout_add(500, 
            delayed_on_profile_entry_selected, 
            (gpointer)profileIndex);
}

gboolean on_edit_profiles_keypress(GdkEventKey *event)
{
    CN_LOGPRINTF("entry");

    GtkToggleButton *button;
    gboolean ret = FALSE;
     
    switch (event->keyval)
    {
        case GDK_Return:
            // 'Enter' key is pressed
            // simulate clicking the 'Create new profile' button
            if (!edit_data_reach_max_profiles() && g_newprofile_labelbutton)
            {
                button = g_newprofile_labelbutton->details->buttonList[0];
                gtk_toggle_button_set_active(button, TRUE);
            }
            else
            {
                erbusy_off();
            }
            ret = TRUE;
            break;

        case GDK_F5:
            // 'UP' key is pressed
            // goto the one top level page
            cmgr_up_page();
            ret = TRUE;
            break;

        case GDK_Down:
        case GDK_Up:
        case GDK_Page_Down:
        case GDK_Page_Up:
        default:
            erbusy_off();
            break;
    }
    
    return ret;
}

void on_edit_profiles_icon_clicked(int iconID, int iconState)
{
    CN_LOGPRINTF("entry iconID[%d] iconState[%d]", iconID, iconState);

    on_icon_clicked(iconID, iconState);
}

// start editing the specified network profile
static void edit_profile(guint profileIndex)
{
    const networkProfile_t *settings;
    regNetworkProfile_t* pRegSettings = NULL;

    CN_LOGPRINTF("entry: profileIndex [%d]", profileIndex);

    edit_set_wizard_mode(FALSE);
    edit_set_back_overview(FALSE);

    // get network settings and show in screen objects
    settings = edit_data_get_network_profile(profileIndex);

    if ((settings->nRegProfiles > 0)
        && settings->regSettingsList
        && ((settings->nActiveIndex >= 0) 
            && (settings->nActiveIndex < settings->nRegProfiles)))
    {
        pRegSettings = settings->regSettingsList[settings->nActiveIndex];
    }

    if (pRegSettings)
    {
        edit_set_network_settings(pRegSettings);

        switch (pRegSettings->connection)
        {
            case wired_t:
                edit_goto_page(editScreenOverview_e);
                break;
            case wireless_t:
                edit_goto_page(editScreenWireless_e);
                break;
            case dialup_t:
                edit_goto_page(editScreenDialup_e);
                break;
            default:
                break;
        }
    }
    else
    {
        // oops, no settings: create a new profile
        edit_profiles_create_profile();
    }
}

char *edit_profiles_default_profilename(void)
{
    connection_t networkType;
    int          nProfiles;
    char        *name = NULL;

    networkType = edit_data_get_network_type();
    nProfiles = edit_data_get_n_network_profiles();
   
    switch (networkType)
    {
        case wired_t:
            name = g_strdup_printf(_("Wired %d"), nProfiles+1);
            break;
        case wireless_t:
            name = g_strdup_printf(_("Wireless %d"), nProfiles+1);
            break;
        case dialup_t:
            name = g_strdup_printf(_("Dial-up %d"), nProfiles+1);
            break;
        default:
            name = g_strdup("");
            break;
    }

    CN_LOGPRINTF("return %s", name);
    return name;
}

void edit_profiles_create_profile(void)
{
    connection_t networkType;
    regNetworkProfile_t* settings;

    CN_LOGPRINTF("entry");

    networkType = edit_data_get_network_type();
    if ((networkType >= 0) && (networkType < connection_undefined_t))
    {
        edit_set_wizard_mode(TRUE);
        edit_set_back_overview(FALSE);

        // get network settings and show in screen objects
        settings = g_new0(regNetworkProfile_t, 1);
        g_assert(settings != NULL);

        settings->name = edit_profiles_default_profilename();
        settings->connection  = networkType;
        settings->proxy       = FALSE;
        settings->addressMode = dhcp_t;
        
        edit_set_network_settings(settings);
        
        erRegFreeNetworkProfile(settings);

        // show the correct screen
        switch (networkType)
        {
            case wired_t:
                edit_goto_page(editScreenOverview_e);
                break;
            case wireless_t:
                edit_search_restore_screen();
                edit_goto_page(editScreenSearch_e);
                break;
            case dialup_t:
                edit_goto_page(editScreenDialup_e);
                break;
            default:
                edit_goto_page(editScreenOverview_e);
                break;
        }
    }
}

int edit_profiles_get_edited_profile_index(void)
{
    CN_LOGPRINTF("entry");

    int              profileIndex;
    GtkProfileGroup* profileGroup; 
    
    if (edit_get_wizard_mode())
    {
        profileIndex = -1;
    }
    else
    {
        profileGroup = GTK_PROFILE_GROUP(g_known_profiles);
        profileIndex = gtk_profile_group_get_selected_profile_index(profileGroup);
    }

    CN_LOGPRINTF("profileIndex[%d]", profileIndex);

    return profileIndex;
}

///////////////////////////////////////////////////////////////////////////////
//
// deleting profiles(support multi-selection)
//
///////////////////////////////////////////////////////////////////////////////
static void on_icon_clicked(int iconID, int iconState)
{
    CN_LOGPRINTF("entry: iconID [%d] iconState [%d]", iconID, iconState);

    if (   g_select.active
        && iconID    == g_select.iconID
        && iconState == iconState_selected)
    {
        // icon clicked again -> listitem select procedure ends
        // Note: ipc messages like toolbar icon clicked are received 
        //       on a separate thread, therefore the current
        //       function is NOT called on the GTK thread, 
        //       so the order of events cannot be guaranteed.
        //       More precisely: pen-click listitem, 
        //       pen-click toolbar icon sometimes is received 
        //       in the inverse order.
        //       To avoid these issues we set a flag, 
        //       which is checked by the timer function 
        //       who then ends the select.
        g_select.icon_clicked = TRUE;
    }
    else
    {
        // icon actions for icon state normal
        if (iconState == iconState_normal)
        {
            switch (iconID)
            {
                case iconID_trashcan:
                    begin_select_delete_profiles(iconID_trashcan, 
                                                  TRUE, 
                                                  delete_profiles);
                    break;
                default:
                    break;  // ignore
            }
        }
    }
}


static void begin_select_delete_profiles(int iconID, 
                             gboolean confirm_with_icon, 
                             on_item_selected_t* callback_on_selected)
{
    GtkProfileGroup* profileGroup;

    CN_LOGPRINTF("entry: iconID [%d]", iconID);

    g_assert(callback_on_selected != NULL);

    profileGroup = GTK_PROFILE_GROUP(g_known_profiles);

    // set the delete mode 
    gtk_profile_group_set_mode(profileGroup, deleteMode_e);

    // remember the settings
    g_select.iconID            = iconID;
    g_select.on_item_selected  = callback_on_selected;
    g_select.confirm_with_icon = confirm_with_icon;

    // update toolbar
    toolbar_disableUpdate();
    toolbar_setIconState(iconID_trashcan, iconState_grey);
    toolbar_setIconState(iconID, iconState_selected);
    toolbar_enableUpdate();
    
    // de-select profiles
    unselect_delete_profiles();
    
    // start time-out
    g_select.ticks        = ITEM_SELECT_TIMEOUT_TICKS;
    g_select.timerID      = g_timeout_add(500, 
                                    on_end_select_delete_profiles_timer, 
                                    0);
    g_select.icon_clicked = FALSE;
    g_select.active       = TRUE;

    // update screen when toolbar icons ok
    toolbar_synchronise();
}

static gboolean on_end_select_delete_profiles_timer(gpointer data)
{
    int      i;
    int      item_idx[MAX_PROFILES_PER_PAGE + 1];
    int*     ip;
    gboolean call_me_again = FALSE;

    CN_LOGPRINTF("entry");

    if (g_select.active)
    {
        if (g_select.icon_clicked)
        {
            // listitem select procedure ends
            if (g_select.on_item_selected != NULL)
            {
                // report the currently selected item(s)
                ip = item_idx;
                for (i = MAX_PROFILES_PER_PAGE - 1 ; i >= 0 ; i--)
                {
                    if (g_select.item_selected[i])
                    {
                        *ip = i;
                        ip++;
                    }
                }
                *ip = -1;
                g_select.on_item_selected(item_idx);
                select_delete_profiles_stop(g_select.confirm_with_icon);
            }
            else
            {
                select_delete_profiles_stop(TRUE);
            }
        }
        else if (g_select.ticks > 0)
        {
            // decrement time-out counter
            g_select.ticks--;
            call_me_again = TRUE;
        }
        else
        {
            // abort select mode
            select_delete_profiles_stop(TRUE);
            toolbar_synchronise();
        }
    }

    return call_me_again;
}

static void select_delete_profiles_stop(gboolean restore_toolbar)
{
    GtkProfileGroup* profileGroup;

    CN_LOGPRINTF("entry");

    profileGroup = GTK_PROFILE_GROUP(g_known_profiles);

    // set the delete mode 
    gtk_profile_group_set_mode(profileGroup, editMode_e);

    if (g_select.active)
    {
        // stop time-out
        gtk_timeout_remove(g_select.timerID);
        g_select.active = FALSE;

        // unselect all profiles
        unselect_delete_profiles();

        if (restore_toolbar)
        {
           // update toolbar
            toolbar_disableUpdate();
            edit_show_trashcan();
            toolbar_enableUpdate();
            toolbar_synchronise();
        }
    }
}

static void unselect_delete_profiles(void)
{
    int i;

    GtkProfileGroup* profileGroup = GTK_PROFILE_GROUP(g_known_profiles);

    for (i = 0 ; i < MAX_PROFILES_PER_PAGE; i++)
    {
        if (g_select.item_selected[i])
        {
            gtk_profile_group_unselect_profile(profileGroup, i);
            g_select.item_selected[i] = FALSE;
        }
    }
}

static gboolean on_delete_profile_selected(GtkProfileGroup* profileGroup, 
                                           guint index, gboolean active)
{
    CN_LOGPRINTF("Entry: index [%d]", index);

    if (g_select.active)
    {
        // notes, active means current state is active, 
        // it's going to be de-active.
        // select mode: blink or hide cursor for this item
        // if (g_select.item_selected[index])
        if (active)
        {
            g_select.item_selected[index] = FALSE;
            gtk_profile_group_unselect_profile(profileGroup, index);
        }
        else
        {
            g_select.item_selected[index] = TRUE;
            gtk_profile_group_select_profile(profileGroup, index);
            if ( !g_select.confirm_with_icon )
            {
                g_select.icon_clicked = TRUE;
            }
        }

        // restart the select timeout
        g_select.ticks = ITEM_SELECT_TIMEOUT_TICKS;

    }
    return TRUE;
}

static void delete_profiles(const int* index_tbl)
{
    int              itemCount;
    int              index;
    const int*       indexPtr;
    gboolean         deleted = FALSE;

    CN_LOGPRINTF("entry: index [%d]", index_tbl[0]);

    itemCount = edit_data_get_n_network_profiles();
    for (indexPtr = index_tbl ; *indexPtr >= 0 ; indexPtr++)
    {
        index = *indexPtr;

        if (index >= 0  &&  index < itemCount)
        {
            CN_LOGPRINTF("index [%d] itemCount [%d]", index, itemCount);

            // Trashcan mode: delete the item
            erbusy_blink();
            edit_data_remove_network_profile(index);
            deleted = TRUE;
        }
    }

    if (deleted)
    {
        // save to registry
        edit_data_store_network_profiles();

        // update the the list network profiles in the meanwhile
        edit_data_init_network_profiles();
        edit_profiles_display();
    }
}

