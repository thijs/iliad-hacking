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
 * \file editScreenDialup.c
 * \brief connectionMgr - "Dialup" screen
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include "config.h"
#include <string.h>
#include <gtk/gtk.h>
#include <libergtk/ergtk.h>
#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "background.h"
#include "connectionMgr.h"
#include "gtkSettingItem.h"
#include "gtkInfoItem.h"
#include "widgetUtils.h"
#include "editScreen.h"
#include "editScreenDialup.h"
#include "editScreenOverview.h"
#include "connectScreenData.h"
#include "connectPing.h"
#include "erbusy.h"
#include "languages.h"

typedef enum
{
    inputPhone_e = 0,
    inputUser_e,
    inputPassword_e,
    dialupInputProfileName_e,
    dialupConnecting_e,
    dialupSucceeded_e,
    dialupFailed_e,
    dialupAborting_e,
    dialupAborted_e,
    undefStatus_e
}dialupStatus_t;

GtkWidget *g_dialup = NULL;

wdtLabelEntry *g_phone = NULL;
wdtLabelEntry *g_user = NULL;
wdtLabelEntry *g_password = NULL;

bkWdtLabelEntry  *g_profilename = NULL;
bkWdtLabelButton *g_buttons = NULL;

GtkWidget *g_info = NULL;

// Create a backup of the settings, need it when user "Cancel"s his changes.
static regDialupSetting_t *g_old_settings = NULL;
static char *g_old_profilename = NULL;

static GtkWidget *create_settings(void);

static void edit_dialup_update_info_text(dialupStatus_t status);

static void edit_dialup_set_phone(const char *phone); 
static void edit_dialup_set_user(const char *user);
static void edit_dialup_set_password(const char *password);
 
static const char *edit_dialup_get_password(void);
static const char *edit_dialup_get_user(void);
static const char *edit_dialup_get_phone(void);

static void edit_dialup_backup_network_settings(void);

static void on_changed(GtkEntry *widget, gpointer data);
static void on_next(GtkToggleButton *button, gpointer data);
static void on_buttons_update(GtkWidget *selection, 
                              gpointer button, 
                              gpointer data);

static gboolean delayed_deactive_button(gpointer data);

static void on_test(void);
static void on_save(void);
static void on_saveconnect(void);

static void edit_dialup_restore_ping_ctxt(void);

static void on_connected(int profileIndex);
static void on_failed(void);
static void ui_update_status(int profileIndex, pingStatus_t status);
static void ui_display_settings(int profileIndex, gboolean resetStatus);
static void ui_select_profile(int profileIndex);
static void ui_unselect_all_profile(void);

static void edit_dialup_freeze_ui(gboolean freeze);

// 
// object hierarchy:
// |--item (gtkSettingItem)
//    |--vbox (GtkVBox)
//       |--settings (composite widgets)
//       |--profilename (bkWdtLabelEntry)
//       |--buttons (bkWdtLabelButton)
//       |--info (gtkInfoItem)
//
GtkWidget *edit_dialup_create(void)
{
    GtkWidget *item = NULL;
    GtkWidget *vbox = NULL;
    GtkWidget *settings = NULL;
    bkWdtLabelEntry *profilename = NULL;
    bkWdtLabelButton *buttons = NULL;
    GtkWidget *info = NULL;
    GtkWidget *widget = NULL;

    CN_LOGPRINTF("entry");

    // item
    item = gtk_settingitem_new("");
 
    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), vbox);

    // settings
    settings = create_settings();
    gtk_box_pack_start(GTK_BOX(vbox), settings, FALSE, FALSE, 0);

    // profilename
    profilename = bk_wdt_label_entry_new(string_e);
    gtk_box_pack_start(GTK_BOX(vbox), profilename->background, 
                       FALSE, FALSE, 0);
   
    // buttons
    buttons = bk_wdt_label_button_new(3, FALSE, 0, 1);
    gtk_box_pack_start(GTK_BOX(vbox), buttons->background, FALSE, FALSE, 0);

    // info
    info = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), info, FALSE, FALSE, 0);

    // signal handlers
    widget = profilename->details->entry;
    g_signal_connect_after(G_OBJECT(widget), "changed-stable", 
               G_CALLBACK(on_changed), (gpointer)dialupInputProfileName_e);
    widget = profilename->details->button;
    g_signal_connect_after(G_OBJECT(widget), "toggled", 
                           G_CALLBACK(on_next), NULL);
   
    widget = buttons->details->selection;
    g_signal_connect(G_OBJECT(widget), "selection-update",
                     G_CALLBACK(on_buttons_update), NULL);

    // show widgets
    gtk_widget_show(vbox);
    gtk_widget_show(settings);
    gtk_widget_show(profilename->background);
    gtk_widget_show(buttons->background);
    gtk_widget_show(info);
    gtk_widget_show(item);

    // set global variables
    g_dialup = item;
    g_profilename = profilename;
    g_buttons = buttons;
    g_info = info;

    CN_LOGPRINTF("done");

    // return
    return item;
}


// |--background
//    |--alignment
//       |--vbox
//          |--phone(wdtLabelEntry)
//          |--user(wdtLabelEntry)
//          |--password(wdtLabelEntry)
//
static GtkWidget *create_settings(void)
{
    GtkWidget *background = NULL;
    GtkWidget *alignment = NULL;
    GtkWidget *vbox = NULL;
    wdtLabelEntry *phone = NULL;
    wdtLabelEntry *user = NULL;
    wdtLabelEntry *password = NULL;
    GtkWidget *widget = NULL;

    CN_LOGPRINTF("entry");

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "bk_grey_666_245");
    gtk_widget_set_size_request(background, 
            BK_666_245_WIDTH, BK_666_245_HEIGHT);

    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
            PADDING_TOP, PADDING_BOTTOM, PADDING_LEFT,PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    
    // phone
    phone = wdt_label_entry_new(string_e);
    gtk_box_pack_start(GTK_BOX(vbox), phone->parent, FALSE, FALSE, 0); 
    
    // user
    user = wdt_label_entry_new(string_e);
    gtk_box_pack_start(GTK_BOX(vbox), user->parent, FALSE, FALSE, 0);
    
    // password
    password = wdt_label_entry_new(string_e);
    gtk_box_pack_start(GTK_BOX(vbox), password->parent, FALSE, FALSE, 0);

    // signal handlers
    widget = phone->entry;
    g_signal_connect_after(G_OBJECT(widget), "changed-stable", 
                           G_CALLBACK(on_changed), (gpointer)inputPhone_e);
    widget = user->entry;
    g_signal_connect_after(G_OBJECT(widget), "changed-stable", 
                           G_CALLBACK(on_changed), (gpointer)inputUser_e);
    widget = password->entry;
    g_signal_connect_after(G_OBJECT(widget), "changed-stable", 
                       G_CALLBACK(on_changed), (gpointer)inputPassword_e);
        
    // show widgets
    gtk_widget_show(alignment);
    gtk_widget_show(vbox);
    gtk_widget_show(phone->parent);
    gtk_widget_hide(phone->button);
    gtk_widget_show(user->parent);
    gtk_widget_hide(user->button);
    gtk_widget_show(password->parent);
    gtk_widget_hide(password->button);

    // global variables
    g_phone = phone;
    g_user = user;
    g_password = password;

    // return
    return background;
}

void edit_dialup_enable_save_button(gboolean enable)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry %d", enable);
    
    if (g_buttons)
    {
        widget = GTK_WIDGET(g_buttons->details->buttonList[1]);
        gtk_widget_set_sensitive(widget, enable);
    }
}

void edit_dialup_set_text(void)
{
    GtkWidget *widget = NULL;

    CN_LOGPRINTF("entry");

    if (g_dialup)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_dialup),
                                        _("Network profile"));
    }

    if (g_phone)
    {
        widget = g_phone->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                _("Type the phone number which you get from your ISP:"));
    }
   
    if (g_user)
    {
        widget = g_user->label;
        gtk_label_set_text(GTK_LABEL(widget), _("Type your username:"));
    }
    
    if (g_password)
    {
        widget = g_password->label;
        gtk_label_set_text(GTK_LABEL(widget), _("Type your password:"));
    }
    
    if (g_profilename)
    {
        widget = g_profilename->details->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                           _("You can personalize the profile name."));
        widget = g_profilename->details->button;
        gtk_button_set_label(GTK_BUTTON(widget), _("Next"));
    }

    if (g_buttons)
    {
        widget = g_buttons->details->label;
        gtk_label_set_text(GTK_LABEL(widget),
            _("Select an action for this profile."));

       widget = GTK_WIDGET(g_buttons->details->buttonList[0]);
       gtk_button_set_label(GTK_BUTTON(widget), _("Test"));
       widget = GTK_WIDGET(g_buttons->details->buttonList[1]);
       gtk_button_set_label(GTK_BUTTON(widget), _("Save"));
       widget = GTK_WIDGET(g_buttons->details->buttonList[2]);
       gtk_button_set_label(GTK_BUTTON(widget), _("Save & Connect"));
    }
}

static void edit_dialup_update_info_text(dialupStatus_t status)
{
    CN_LOGPRINTF("entry");

    g_return_if_fail(g_info != NULL);

    switch (status)
    {
        case inputPhone_e:
        case inputUser_e:
        case inputPassword_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("If you do not know the settings for this network, "
                      "please ask your Internet Service Provider "
                      "for detailed information."));
            break;
        case dialupInputProfileName_e:
        case undefStatus_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("If you personalize the profile name, "
                  "it will be easy to recognize "
                  "in the iLiad's network profile list.\n"
                  "Click 'Next' for more advanced settings."));
            break;
        case dialupConnecting_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("The iLiad is trying to connect to the server."));
            break;
        case dialupSucceeded_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconOk_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("Connection to the server established."));
             break;
        case dialupFailed_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconFailed_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                    _("Connection to server "
                      "can not been established correctly."));
           break;
        case dialupAborting_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e); 
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                _("The iLiad is aborting the connection."));
            break;
        case dialupAborted_e:
            gtk_infoitem_set_icon(GTK_INFOITEM(g_info), iconInfo_e);
            gtk_infoitem_set_text(GTK_INFOITEM(g_info), 
                _("The connection has been aborted."));
            break;
 
        default:
            break;
    }
}

// return undefStatus_e means all fields have been fill in
static dialupStatus_t edit_dialup_detect_status(void)
{
    erGtkEntry *phone, *user, *password, *profilename;
    dialupStatus_t status = undefStatus_e;

    CN_LOGPRINTF("entry");

    phone = ERGTK_ENTRY(g_phone->entry);
    user = ERGTK_ENTRY(g_user->entry);
    password = ERGTK_ENTRY(g_password->entry);
    profilename = ERGTK_ENTRY(g_profilename->details->entry);

    if (!ergtk_entry_check_field(phone))
    {
        status = inputPhone_e;
    }
    else if (!ergtk_entry_check_field(user))
    {
        status = inputUser_e;
    }
    else if (!ergtk_entry_check_field(password))
    {
        status = inputPassword_e;
    }
    else if (!ergtk_entry_check_field(profilename))
    {
        status = inputProfileName_e;
    }

    CN_LOGPRINTF("return %d", status);
    return status;
}

static void edit_dialup_determine_status(void)
{
    dialupStatus_t status;
    GtkWidget *widget = NULL;

    status = edit_dialup_detect_status();
    switch (status)
    {
        case inputPhone_e:
            widget = g_phone->entry;
            break;
        case inputUser_e:
            widget = g_user->entry;
            break;
        case inputPassword_e:
            widget = g_password->entry;
            break;
        case inputProfileName_e:
        case undefStatus_e:
            widget = g_profilename->details->entry;
            break;
        default:
            break;
    }
    
    if (widget)
    {
        gtk_widget_grab_focus(widget);
    }

    edit_dialup_update_info_text(status);
}

void edit_dialup_set_network_settings(const regNetworkProfile_t *settings)
{
    regDialupSetting_t *dialupSettings = NULL;
    char *phone = "";
    char *user = "";
    char *password = "";
    char *profilename = "";

    CN_LOGPRINTF("entry [%p]", settings);

    g_return_if_fail(settings != NULL);
    g_return_if_fail(settings->connection == dialup_t);

    // get the values
    profilename = settings->name;
    dialupSettings = settings->dialupSettings;
    if (dialupSettings)
    {
        phone = dialupSettings->phone;
        user = dialupSettings->user;
        password = dialupSettings->password;
    }

    // set the text for screen objects
    edit_dialup_set_phone(phone);
    edit_dialup_set_user(user);
    edit_dialup_set_password(password);
    edit_dialup_set_profilename(profilename);

    edit_dialup_determine_status();

    // make a backup copy of the settings
    edit_dialup_backup_network_settings();
}

static void edit_dialup_backup_network_settings(void) 
{
    const char *phone, *user, *password, *profilename;

    CN_LOGPRINTF("entry");

    phone = edit_dialup_get_phone();
    user = edit_dialup_get_user();
    password = edit_dialup_get_password();
    profilename = edit_dialup_get_profilename();

    if (g_old_settings)
    {
        g_free(g_old_settings->phone);
        g_free(g_old_settings->user);
        g_free(g_old_settings->password);
        g_free(g_old_settings);
    }

    g_old_settings = g_new0(regDialupSetting_t, 1);
    g_assert(g_old_settings != NULL);
    g_old_settings->phone = g_strdup(phone);
    g_old_settings->user = g_strdup(user);
    g_old_settings->password = g_strdup(password);
    
    g_free(g_old_profilename);
    g_old_profilename = g_strdup(profilename);
}

static void edit_dialup_set_phone(const char *phone) 
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry [%s]", phone);

    if (g_phone)
    {
        widget = g_phone->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), phone ? phone : ""); 
    }
}

static void edit_dialup_set_user(const char* user)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry [%s]", user);

    if (g_user)
    {
        widget = g_user->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), user ? user : "");
    }
} 

static void edit_dialup_set_password(const char *password)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry [%s]", password);

    if (g_password)
    {
        widget = g_password->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), password ? password : "");
    }
}

void edit_dialup_set_profilename(const char *profilename)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry [%s]", profilename);

    if (g_profilename)
    {
        widget = g_profilename->details->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), 
                           profilename ? profilename: "");
    }
}

void edit_dialup_get_network_settings(regNetworkProfile_t *settings)
{
    regDialupSetting_t *dialupSettings;
    const gchar *phone, *user, *password, *profilename;

    CN_LOGPRINTF("entry [%p]", settings);

    g_return_if_fail(settings != NULL);
    g_return_if_fail(settings->connection == dialup_t);

    // free the old settings
    dialupSettings = settings->dialupSettings;
    if (dialupSettings)
    {
        g_free(dialupSettings->phone);
        g_free(dialupSettings->user);
        g_free(dialupSettings->password);
        g_free(dialupSettings);
        dialupSettings = NULL;
        settings->dialupSettings = NULL;
    }
   
    // store the new settings
    if (settings->connection == dialup_t)
    {
        profilename = edit_dialup_get_profilename();
        g_free(settings->name);
        settings->name = g_strdup(profilename);

        dialupSettings = g_new0(regDialupSetting_t, 1);
        if (dialupSettings)
        {
            phone = edit_dialup_get_phone();
            user = edit_dialup_get_user();
            password = edit_dialup_get_password();

            dialupSettings->phone = g_strdup(phone);
            dialupSettings->user = g_strdup(user);
            dialupSettings->password = g_strdup(password);
           
            settings->dialupSettings = dialupSettings;
        }
    }
}

static const char *edit_dialup_get_phone(void)
{
    GtkWidget *widget;
    const char *phone = NULL;

    if (g_phone)
    {
        widget = g_phone->entry;
        phone = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return phone=%s", phone);
    return phone;
}

static const char *edit_dialup_get_user(void)
{
    GtkWidget *widget;
    const char *user = NULL;

    if (g_user)
    {
        widget = g_user->entry;
        user = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return user=%s", user);
    return user;
}

static const char *edit_dialup_get_password(void)
{
    GtkWidget *widget;
    const char *password = NULL;

    if (g_password)
    {
        widget = g_password->entry;
        password = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return password[%s]", password);
    return password;
}

const char *edit_dialup_get_profilename(void)
{
    GtkWidget *widget;
    const char *profilename = NULL;

    if (g_profilename)
    {
        widget = g_profilename->details->entry;
        profilename = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return profilename=%s", profilename);
    return profilename;
}

gboolean edit_dialup_check_network_settings(void)
{
    dialupStatus_t status;
    gboolean valid = TRUE;

    CN_LOGPRINTF("entry");

    status = edit_dialup_detect_status();
    if ((status != inputUser_e)
        && (status != inputPassword_e)
        && (status != undefStatus_e))
    {
        valid = FALSE;
    }

    CN_LOGPRINTF("return valid=%d", valid);
    return valid;
}

void edit_dialup_restore_network_settings(void)
{
    char *phone = NULL;
    char *user = NULL;
    char *password = NULL;
    char *profilename = NULL; 
    
    CN_LOGPRINTF("entry");
     
    if (g_old_settings)
    {
       phone = g_old_settings->phone;
       user = g_old_settings->user;
       password = g_old_settings->password;
    }
    if (g_old_profilename)
    {
        profilename = g_old_profilename;
    }
    
    edit_dialup_set_phone(phone);
    edit_dialup_set_user(user);
    edit_dialup_set_password(password);
    edit_dialup_set_profilename(profilename);

}

// signal hanlders
static void on_changed(GtkEntry *widget, gpointer data)
{
    CN_LOGPRINTF("entry");

    dialupStatus_t status = (dialupStatus_t)data;
    edit_dialup_update_info_text(status);
    if (status == inputProfileName_e)
    {
        edit_overview_update_profile();
    }
}

static gboolean on_delayed_next(gpointer data)
{
    CN_LOGPRINTF("entry");
    
    erbusy_blink();

    edit_dialup_backup_network_settings();

    edit_goto_page(editScreenOverview_e);
    edit_overview_update_info_text(inputProxy_e);
    edit_overview_update_profile();

    return FALSE;
}

static void on_next(GtkToggleButton *button, gpointer data)
{
    gboolean active;
    gboolean valid;

    CN_LOGPRINTF("entry");

    active = gtk_toggle_button_get_active(button);
    if (active)
    {
        valid = edit_dialup_check_network_settings();
        if (valid)
        {
            g_timeout_add(500, on_delayed_next, NULL);
        }
        else
        {
            edit_dialup_determine_status();
        }

        // wait for a while to deactive the button 
        g_timeout_add(500, delayed_deactive_button, button);
    }
}

static gboolean delayed_deactive_button(gpointer data)
{
    GtkToggleButton *button = GTK_TOGGLE_BUTTON(data);
    
    if (button)
    {
        gtk_toggle_button_set_active(button, FALSE);
    }

    return FALSE;
}

static void on_buttons_update(GtkWidget *selection, 
                              gpointer button, 
                              gpointer data)
{
    gboolean   active;
    gboolean   valid;
    int        nsel;
    GtkWidget  *widget;
    erGtkSelectionGroup *sel;
    gboolean toDeactive = TRUE;

    CN_LOGPRINTF("entry");

    active = gtk_toggle_button_get_active(button);
    valid = edit_dialup_check_network_settings();
    if (!valid)
    {
        edit_dialup_determine_status();
    }

    if (active && valid)
    {
        widget = g_buttons->details->selection;
        sel = ERGTK_SELECTION_GROUP(widget);
        ergtk_selection_group_get_selected_buttons(sel, &nsel, 1);

        switch (nsel)
        {
            case 0:
                // test button is selected
                on_test();
                toDeactive = FALSE;
                break;
            case 1:
                // save button is selected
                on_save();
                toDeactive = FALSE;
                break;
            case 2:
                // save&connect button is selected
                on_saveconnect();
                toDeactive = FALSE;
                break;
            default:
                break;
        }
    }

    if (active && toDeactive)
    {
        g_timeout_add(500, delayed_deactive_button, button);
    }
}

static gboolean on_delayed_test(gpointer data)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");

    erbusy_blink();

    edit_dialup_backup_network_settings();

    edit_overview_update_profile();
    edit_goto_page(editScreenOverview_e);
    // similuate clicking the 'Test' button
    edit_overview_click_test();
   
    if (g_buttons)
    {
        button = g_buttons->details->buttonList[0];
        gtk_toggle_button_set_active(button, FALSE);
    }

    return FALSE;
}

static void on_test(void)
{
    CN_LOGPRINTF("entry");

    g_timeout_add(500, on_delayed_test, NULL);
}

static gboolean on_delayed_save(gpointer data)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");

    erbusy_blink();

    edit_save_profile();
    edit_goto_page(editScreenProfiles_e);

    if (g_buttons)
    {
        button = g_buttons->details->buttonList[1];
        gtk_toggle_button_set_active(button, FALSE);
    }

    return FALSE;
}

static void on_save(void)
{
    CN_LOGPRINTF("entry");

    g_timeout_add(500, on_delayed_save, NULL);
}

static void on_saveconnect(void)
{
    pingContext_t *ctxt;

    CN_LOGPRINTF("entry");

    edit_dialup_freeze_ui(TRUE);

    edit_dialup_restore_ping_ctxt();

    ctxt = cmgr_get_ping_ctxt();
    if (ctxt)
    {
        connect_ping_select_initial_profile(ctxt);
        connect_ping(ctxt, TRUE);
    }
}

static void edit_dialup_restore_ping_ctxt(void)
{
    pingContext_t *ctxt;
    cmgrConnectType_t connectType;

    CN_LOGPRINTF("entry");
    
    ctxt = cmgr_get_ping_ctxt();
    if (ctxt)
    {
        // set mode and connect type
        connect_ping_ctxt_set_mode(ctxt, editPing_e);
        connectType = connect_data_get_connect_type();
        connect_ping_ctxt_set_connect_type(ctxt, connectType);
        
        // ping done callbacks
        connect_ping_ctxt_set_done_callbacks(ctxt, on_connected, on_failed);
       
        // data access callbacks
        connect_ping_ctxt_set_get_initial_profile(ctxt, 
                        edit_get_initial_profile);
        connect_ping_ctxt_set_access_network_profiles_callbacks(ctxt,
                                        edit_get_n_network_profiles,
                                        edit_get_network_profile,
                                        edit_select_next_regprofile);

        // ui callbacks
        connect_ping_ctxt_set_ui_callbacks(ctxt, 
                                    ui_update_status, 
                                    ui_display_settings,
                                    ui_select_profile,
                                    ui_unselect_all_profile);
    }
}

static void on_connected(int profileIndex)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");
  
    edit_dialup_freeze_ui(FALSE);

    if (g_buttons)
    {
        button = g_buttons->details->buttonList[2];
        g_timeout_add(500, delayed_deactive_button, button);
    }

    edit_save_profile();

    edit_on_connected();    
}

static void on_failed(void)
{
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");

    edit_dialup_freeze_ui(FALSE);

    if (g_buttons)
    {
        button = g_buttons->details->buttonList[2];
        g_timeout_add(500, delayed_deactive_button, button);
    }
}

static void ui_update_status(int profileIndex, pingStatus_t status)
{
    dialupStatus_t state;

    CN_LOGPRINTF("entry");

    switch (status)
    {
        case pingSucceeded_e:
            state = dialupSucceeded_e;
            break;
        case pingFailed_e:
        case pingFailedPCShare_e:
        case pingFailedNetwork_e:
            state = dialupFailed_e;
            break;
        case pingConnecting_e:
            state = dialupConnecting_e;
            break;
        case pingAborting_e:
            state = dialupAborting_e;
            break;
        case pingAborted_e:
            state = dialupAborted_e;
            on_failed();
            break;
        default:
            state = undefStatus_e;
            break;
    }

    edit_dialup_update_info_text(state);

    display_update_request_screen_refresh(STATUS_ITEM_CHANGE, 
                                          WAVEFORM_TYPING);
}

static void ui_display_settings(int profileIndex, gboolean resetStatus)
{
    CN_LOGPRINTF("entry");
    // nothing to do
}

static void ui_select_profile(int profileIndex)
{
    CN_LOGPRINTF("entry");
    // nothing to do
}

static void ui_unselect_all_profile(void)
{
    CN_LOGPRINTF("entry");
    // nothing to do
}

gboolean on_edit_dialup_keypress(GdkEventKey *event)
{
    pingContext_t *ctxt;
    GtkToggleButton *button;
    gboolean ret = FALSE;

    CN_LOGPRINTF("entry");
    
    ctxt = cmgr_get_ping_ctxt();
    switch (event->keyval)
    {
        case GDK_Return:
            // 'Enter' key is pressed
            if (!connect_ping_in_pinging(ctxt))
            {
                // simulate clicking the 'Save & Connect' button
                if (g_buttons)
                {
                    button = GTK_TOGGLE_BUTTON(g_buttons->details->buttonList[2]);
                    gtk_toggle_button_set_active(button, TRUE);
                }
                ret = TRUE;
            }
            break;

        case GDK_F5:
            // 'UP' key is pressed
            // goto the one top level page
            connect_ping_abort(ctxt);
            cmgr_up_page();
            ret = TRUE;
            break;
        case GDK_Down:
        case GDK_Up:
            // avoid the standard GTK behaviour for GtkEntries
            erbusy_off();
            ret = TRUE;
            break;
        default:
            erbusy_off();
            break;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////
// functions for freezing the ui when in ping-to-server state
///////////////////////////////////////////////////////////////////////////

static void edit_dialup_freeze_ui(gboolean freeze)
{
    editMode_t mode;
    gboolean sensitive;
    GtkWidget *widget;

    CN_LOGPRINTF("entry[%d]", freeze);

    sensitive = !freeze;

    // the phone
    widget = g_phone->entry;
    gtk_widget_set_sensitive(widget, sensitive);
    // the user
    widget = g_user->entry;
    gtk_widget_set_sensitive(widget, sensitive);
    // the password
    widget = g_password->entry;
    gtk_widget_set_sensitive(widget, sensitive);

    // the profilename
    widget = g_profilename->details->entry;
    gtk_widget_set_sensitive(widget, sensitive);

    widget = g_profilename->details->button;
    gtk_widget_set_sensitive(widget, sensitive);
    
    // the buttons
    widget = GTK_WIDGET(g_buttons->details->buttonList[0]);
    gtk_widget_set_sensitive(widget, sensitive);
    
    widget = GTK_WIDGET(g_buttons->details->buttonList[1]);
    gtk_widget_set_sensitive(widget, sensitive);

    widget = GTK_WIDGET(g_buttons->details->buttonList[2]);
    gtk_widget_set_sensitive(widget, sensitive);
 
    if (sensitive)
    {
        // restore the previous in-sensitive widgets
        // save button
        mode = edit_get_mode();
        if ((mode == editModeConnectFillIn_e)
            || (mode == editModeConnectCreate_e)
            || (mode == editModeConnectNormal_e))
        {
            edit_dialup_enable_save_button(FALSE);
        }
    }
}

