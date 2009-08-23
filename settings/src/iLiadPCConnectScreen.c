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
 * \file iLiadPCConnectScreen.c
 * \brief settings application - screen handling and event processing for screen "pc connect"
 * 
 */

#define _GNU_SOURCE

#include "config.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "gtkSettingItem.h"
#include "gtkSettingItemTextEntry.h"
#include "gtkInfoItem.h"
#include "iLiadPCConnectScreen.h"
#include "iLiadPCConnectData.h"
#include "languages.h"


// screen modes
typedef enum
{
    e_screen_show_all,
    e_screen_enter_pcname,
    e_screen_enter_sharename,
    e_screen_enter_workgroup,
    e_screen_enter_user_password,
} screen_mode_t;

static screen_mode_t g_screen_mode = e_screen_show_all;

// screen objects
static GtkWidget *g_edit_pc_connect = NULL;

static GtkWidget *g_pcname = NULL;
static GtkWidget *g_pcname_info = NULL;

static GtkWidget *g_sharename = NULL;
static GtkWidget *g_sharename_info = NULL;

static GtkWidget *g_workgroup = NULL;
static GtkWidget *g_workgroup_info = NULL;

static GtkWidget *g_user_password = NULL;
static GtkWidget *g_user_password_info = NULL;
static GtkWidget *g_user_password_label = NULL;
static GtkWidget *g_user_label = NULL;
static GtkWidget *g_user_entry = NULL;
static GtkWidget *g_password_label = NULL;
static GtkWidget *g_password_entry = NULL;

// local functions
static GtkWidget* create_user_password_widget(void);
static void       iLiad_pc_connect_set_screenmode(const screen_mode_t mode);
static gboolean   pcname_check_split(const gchar* pcname_old,    gchar** pcname_new,
                                     const gchar* workgroup_old, gchar** workgroup_new);

// signal handlers
static void on_pcname_changed(GtkWidget* widget, const gchar* value);
static void on_pcname_validate(GtkWidget* widget, const gchar* text, gchar** newtext);
static void on_pcname_button_press(GtkWidget* widget);
static void on_pcname_info_button_clicked(GtkWidget* widget, const gboolean button_active);

static void on_sharename_changed(GtkWidget* widget, const gchar* value);
static void on_sharename_button_press(GtkWidget* widget);
static void on_sharename_info_button_clicked(GtkWidget* widget, const gboolean button_active);

static void on_workgroup_changed(GtkWidget* widget, const gchar* value);
static void on_workgroup_validate(GtkWidget* widget, const gchar* text, gchar** newtext);
static void on_workgroup_button_press(GtkWidget* widget);
static void on_workgroup_info_button_clicked(GtkWidget* widget, const gboolean button_active);

static void on_username_changed(GtkEditable* editable, gpointer user_data);
static void on_password_changed(GtkEditable* editable, gpointer user_data);
static gboolean on_user_password_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
static void on_user_password_info_button_clicked(GtkWidget*widget, const gboolean button_active);


// object hierarchy:
//     g_edit_pc_connect (gtkSettingItem)
//       |--top_vbox
//          |--vbox
//             |--g_pcname (gtkSettingItemTextEntry)
//             |--g_pcname_info (gtkInfoItem)
//          |--vbox
//             |--g_sharename (gtkSettingItemTextEntry)
//             |--g_sharename_info (gtkInfoItem)
//          |--vbox
//             |--g_workgroup (gtkSettingItemTextEntry)
//             |--g_workgroup_info (gtkInfoItem)
//          |--vbox
//             |--g_user_password
//             |--g_user_password_info
//          |--g_confirm_widget
//       
GtkWidget *iLiad_pc_connect_screen_create(void)
{
    GtkWidget *item;
    GtkWidget *top_vbox;
    GtkWidget *vbox;
    GtkWidget *pcname;
    GtkWidget *pcname_info;
    GtkWidget *sharename;
    GtkWidget *sharename_info;
    GtkWidget *workgroup;
    GtkWidget *workgroup_info;
    GtkWidget *user_password;
    GtkWidget *user_password_info;

    ST_LOGPRINTF("entry");

    // g_edit_pc_connect
    item = gtk_settingitem_new("");
    gtk_widget_set_size_request(item, PC_CONNECT_SCREEN_WIDTH, -1);
    // top_vbox
    top_vbox = gtk_vbox_new(FALSE, PC_CONNECT_SCREEN_SPACING_V);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), top_vbox);
    gtk_widget_show(top_vbox);
    // vbox
    vbox = gtk_vbox_new(FALSE, 1);
    gtk_box_pack_start(GTK_BOX(top_vbox), vbox, FALSE, FALSE, 0);
    gtk_widget_show(vbox);
    // pcname
    pcname = gtk_settingitem_textentry_new();
    gtk_widget_set_size_request(pcname, -1, PC_CONNECT_PCNAME_HEIGHT); 
    gtk_box_pack_start(GTK_BOX(vbox), pcname, FALSE, FALSE, 0);
    // pcname_info
    pcname_info = gtk_infoitem_new(TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), pcname_info, FALSE, FALSE, 0);
    // vbox
    vbox = gtk_vbox_new(FALSE, 1);
    gtk_box_pack_start(GTK_BOX(top_vbox), vbox, FALSE, FALSE, 0);
    gtk_widget_show(vbox);
    // sharename
    sharename = gtk_settingitem_textentry_new();
    gtk_widget_set_size_request(sharename, -1, PC_CONNECT_SHARENAME_HEIGHT); 
    gtk_box_pack_start(GTK_BOX(vbox), sharename, FALSE, FALSE, 0);
    // sharename_info
    sharename_info = gtk_infoitem_new(TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), sharename_info, FALSE, FALSE, 0);
    // vbox
    vbox = gtk_vbox_new(FALSE, 1);
    gtk_box_pack_start(GTK_BOX(top_vbox), vbox, FALSE, FALSE, 0);
    gtk_widget_show(vbox);
    // workgroup
    workgroup = gtk_settingitem_textentry_new();
    gtk_widget_set_size_request(workgroup, -1, PC_CONNECT_WORKGROUP_HEIGHT); 
    gtk_box_pack_start(GTK_BOX(vbox), workgroup, FALSE, FALSE, 0);
    // workgroup_info
    workgroup_info = gtk_infoitem_new(TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), workgroup_info, FALSE, FALSE, 0);
    // vbox
    vbox = gtk_vbox_new(FALSE, 1);
    gtk_box_pack_start(GTK_BOX(top_vbox), vbox, FALSE, FALSE, 0);
    gtk_widget_show(vbox);
    // user_password widget
    user_password = create_user_password_widget();
    gtk_box_pack_start(GTK_BOX(vbox), user_password, FALSE, FALSE, 0);
    // user_password_info
    user_password_info = gtk_infoitem_new(TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), user_password_info, FALSE, FALSE, 0);

    // signals functions
    gtk_settingitem_textentry_callback_on_changed(GTK_SETTINGITEM_TEXTENTRY(pcname), on_pcname_changed);
    gtk_settingitem_textentry_callback_on_validate(GTK_SETTINGITEM_TEXTENTRY(pcname), on_pcname_validate);
    gtk_settingitem_textentry_callback_on_button_press(GTK_SETTINGITEM_TEXTENTRY(pcname), on_pcname_button_press);
    gtk_infoitem_callback_on_buttonclicked(GTK_INFOITEM(pcname_info), on_pcname_info_button_clicked);

    gtk_settingitem_textentry_callback_on_changed(GTK_SETTINGITEM_TEXTENTRY(sharename), on_sharename_changed);
    gtk_settingitem_textentry_callback_on_button_press(GTK_SETTINGITEM_TEXTENTRY(sharename), on_sharename_button_press);
    gtk_infoitem_callback_on_buttonclicked(GTK_INFOITEM(sharename_info), on_sharename_info_button_clicked);

    gtk_settingitem_textentry_callback_on_changed(GTK_SETTINGITEM_TEXTENTRY(workgroup), on_workgroup_changed);
    gtk_settingitem_textentry_callback_on_validate(GTK_SETTINGITEM_TEXTENTRY(workgroup), on_workgroup_validate);
    gtk_settingitem_textentry_callback_on_button_press(GTK_SETTINGITEM_TEXTENTRY(workgroup), on_workgroup_button_press);
    gtk_infoitem_callback_on_buttonclicked(GTK_INFOITEM(workgroup_info), on_workgroup_info_button_clicked);

    gtk_infoitem_callback_on_buttonclicked(GTK_INFOITEM(user_password_info), on_user_password_info_button_clicked);

   // global variables 
    g_edit_pc_connect = item;
    g_pcname = pcname;
    g_pcname_info = pcname_info;
    g_sharename = sharename;
    g_sharename_info = sharename_info;
    g_workgroup = workgroup;
    g_workgroup_info = workgroup_info;
    g_user_password = user_password;
    g_user_password_info = user_password_info;
    // show hide widgets
    gtk_widget_show(item);
    gtk_widget_show(pcname);
    gtk_widget_hide(pcname_info);
    gtk_widget_show(sharename);
    gtk_widget_hide(sharename_info);
    gtk_widget_show(workgroup);
    gtk_widget_hide(workgroup_info);
    gtk_widget_show(user_password);
    gtk_widget_hide(user_password_info);

    ST_LOGPRINTF("done");
    // return
    return g_edit_pc_connect;
}

//
// widget layout
//  |--background
//     |--alignment
//        |--vbox
//           |--user_password_label
//           |--hbox
//              |--vbox
//                 |--user_label
//                 |--user_entry
//              |--vbox
//                 |--password_label
//                 |--password_entry
//
static GtkWidget* create_user_password_widget()
{
    GtkWidget *background;
    GtkWidget *alignment;
    GtkWidget *vbox;
    GtkWidget *user_password_label;
    GtkWidget *hbox;
    GtkWidget *user_label;
    GtkWidget *user_entry;
    GtkWidget *password_label;
    GtkWidget *password_entry;

    ST_LOGPRINTF("entry");

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "pc_connect_user_password_widget");
    gtk_widget_set_size_request(GTK_WIDGET(background),
                                PC_CONNECT_USER_PASSWORD_BK_WIDTH, PC_CONNECT_USER_PASSWORD_BK_HEIGHT);
    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              PC_CONNECT_SCREEN_PADDING_TOP,  PC_CONNECT_SCREEN_PADDING_BOTTOM,
                              PC_CONNECT_SCREEN_PADDING_LEFT, PC_CONNECT_SCREEN_PADDING_RIGHT  );
    gtk_container_add(GTK_CONTAINER(background), alignment);
    gtk_widget_show(alignment);
    // vbox
    vbox = gtk_vbox_new(FALSE, PC_CONNECT_SCREEN_USER_PASSWORD_SPACING_V);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    gtk_widget_show(vbox);
    // user_password_label
    user_password_label = gtk_label_new("");
    gtk_widget_set_name(user_password_label, "pc_connect_screen_info_text");
    gtk_widget_set_size_request(user_password_label, (-1), (-1));
    gtk_misc_set_alignment(GTK_MISC(user_password_label), 0.0, 0.5);
    gtk_label_set_justify(GTK_LABEL(user_password_label), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox), user_password_label, FALSE, FALSE, 0);
    gtk_widget_show(user_password_label);
    // hbox
    hbox = gtk_hbox_new(FALSE, PC_CONNECT_SCREEN_USER_PASSWORD_SPACING_H);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    gtk_widget_show(hbox);
    // vbox
    vbox = gtk_vbox_new(FALSE, PC_CONNECT_SCREEN_USER_PASSWORD_SPACING_V);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
    gtk_widget_show(vbox);
    // user_label
    user_label = gtk_label_new("");
    gtk_widget_set_name(user_label, "pc_connect_screen_info_text");
    gtk_widget_set_size_request(user_label, PC_CONNECT_INPUT_ENTRY_WIDTH, (-1));
    gtk_misc_set_alignment(GTK_MISC(user_label), 0.0, 0.5);
    gtk_label_set_justify(GTK_LABEL(user_label), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox), user_label, FALSE, FALSE, 0);
    gtk_widget_show(user_label);
    // user_entry
    user_entry = ergtk_entry_new();
    gtk_widget_set_size_request(user_entry, PC_CONNECT_INPUT_ENTRY_WIDTH, PC_CONNECT_INPUT_ENTRY_HEIGHT);
    gtk_box_pack_start(GTK_BOX(vbox), user_entry, FALSE, FALSE, 0);
    gtk_widget_show(user_entry);
    // vbox
    vbox = gtk_vbox_new(FALSE, PC_CONNECT_SCREEN_USER_PASSWORD_SPACING_V);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
    gtk_widget_show(vbox);
    // password_label
    password_label = gtk_label_new("");
    gtk_widget_set_name(password_label, "pc_connect_screen_info_text");
    gtk_widget_set_size_request(password_label, PC_CONNECT_INPUT_ENTRY_WIDTH, (-1));
    gtk_misc_set_alignment(GTK_MISC(password_label), 0.0, 0.5);
    gtk_label_set_justify(GTK_LABEL(password_label), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox), password_label, FALSE, FALSE, 0);
    gtk_widget_show(password_label);
    // password_entry
    password_entry = ergtk_entry_new();
    gtk_widget_set_size_request(password_entry, PC_CONNECT_INPUT_ENTRY_WIDTH, PC_CONNECT_INPUT_ENTRY_HEIGHT);
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 0);
    gtk_widget_show(password_entry);
    
    // signals functions
    g_signal_connect(GTK_OBJECT(user_entry),     "changed-stable",     G_CALLBACK(on_username_changed),           NULL);
    g_signal_connect(GTK_OBJECT(password_entry), "changed-stable",     G_CALLBACK(on_password_changed),           NULL);
    g_signal_connect(GTK_OBJECT(user_entry),     "button-press-event", G_CALLBACK(on_user_password_button_press), NULL);
    g_signal_connect(GTK_OBJECT(password_entry), "button-press-event", G_CALLBACK(on_user_password_button_press), NULL);

    // global variables
    g_user_password_label = user_password_label;
    g_user_label          = user_label;
    g_user_entry          = user_entry;
    g_password_label      = password_label;
    g_password_entry      = password_entry;
    
    return background;
}

void iLiad_pc_connect_screen_set_text(void)
{
    ST_LOGPRINTF("entry");

    gchar *info_proceed = _("Proceed");

    // title
    if (g_edit_pc_connect)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_edit_pc_connect),
                                        _("iLiad settings / Setting for computer connection "));
    }
    // pcname
    if (g_pcname)
    {
        gtk_settingitem_textentry_setlabel(GTK_SETTINGITEM_TEXTENTRY(g_pcname),
                                           _("STEP 1: Please type the name of your computer."));
    }
    if (g_pcname_info)
    {
        gtk_infoitem_set_text(GTK_INFOITEM(g_pcname_info),
                              _("You can create a connection between the iLiad"
                                " and your computer by entering the specifications"
                                " of your PC.\n"
                                "The \"full computer name\" is located at the system"
                                " properties of your computer."));
        gtk_infoitem_set_buttontext(GTK_INFOITEM(g_pcname_info), info_proceed);
    }
    // sharename
    if (g_sharename)
    {
        gtk_settingitem_textentry_setlabel(GTK_SETTINGITEM_TEXTENTRY(g_sharename),
                                           _("STEP 2: Please enter the name of the \"shared\" destination folder."));
    }
    if (g_sharename_info)
    {
        gtk_infoitem_set_text(GTK_INFOITEM(g_sharename_info),
                              _("Create a destination folder on your computer. In the folder properties"
                                " you can \"share this folder\"."));
        gtk_infoitem_set_buttontext(GTK_INFOITEM(g_sharename_info), info_proceed);
    }
    // workgroup
    if (g_workgroup)
    {
        gtk_settingitem_textentry_setlabel(GTK_SETTINGITEM_TEXTENTRY(g_workgroup),
                                           _("STEP 3: Please enter the domain group / work group of your computer."));
    }
    if (g_workgroup_info)
    {
        gtk_infoitem_set_text(GTK_INFOITEM(g_workgroup_info),
                              _("When the computer is part of a business network, you can enter the domain group.\n"
                                "In most cases you can leave this entry field blank."));
        gtk_infoitem_set_buttontext(GTK_INFOITEM(g_workgroup_info), info_proceed);
    }
    if (g_user_password_label)
    {
         gtk_label_set_text(GTK_LABEL(g_user_password_label), _("STEP 4: Please enter the username and password of your computer."));
    }
    if (g_user_label)
    {
         gtk_label_set_text(GTK_LABEL(g_user_label), _("User name"));
    }
    if (g_password_label)
    {
         gtk_label_set_text(GTK_LABEL(g_password_label), _("Password"));
    }
    if (g_user_password_info)
    {
        gtk_infoitem_set_text(GTK_INFOITEM(g_user_password_info),
                              _("You can leave this entry field blank when you didn't create a user name and password."));
        gtk_infoitem_set_buttontext(GTK_INFOITEM(g_user_password_info), info_proceed);
    }

    ST_LOGPRINTF("done");
}

void iLiad_pc_connect_screen_display_settings()
{
    ST_LOGPRINTF("entry");

    const regPCProfile_t *setting = iLiad_pc_connect_get_profiledata(0);

    // set screen objects
    if (NULL == setting)
    {
        gtk_settingitem_textentry_settext(GTK_SETTINGITEM_TEXTENTRY(g_pcname),    "");
        gtk_settingitem_textentry_settext(GTK_SETTINGITEM_TEXTENTRY(g_sharename), "");
        gtk_settingitem_textentry_settext(GTK_SETTINGITEM_TEXTENTRY(g_workgroup), "");
        gtk_entry_set_text(GTK_ENTRY(g_user_entry),     "");
        gtk_entry_set_text(GTK_ENTRY(g_password_entry), "");
    }
    else
    {
        gtk_settingitem_textentry_settext(GTK_SETTINGITEM_TEXTENTRY(g_pcname),    setting->pcname   );
        gtk_settingitem_textentry_settext(GTK_SETTINGITEM_TEXTENTRY(g_sharename), setting->sharename);
        gtk_settingitem_textentry_settext(GTK_SETTINGITEM_TEXTENTRY(g_workgroup), setting->workgroup);
        gtk_entry_set_text(GTK_ENTRY(g_user_entry),     setting->username);
        gtk_entry_set_text(GTK_ENTRY(g_password_entry), setting->password);
    }

    // set screen mode
    iLiad_pc_connect_set_screenmode(e_screen_show_all);
}


static void iLiad_pc_connect_set_screenmode(const screen_mode_t mode)
{
    ST_LOGPRINTF("entry: mode [%d]", mode);

    if (mode != g_screen_mode)
    {
        g_screen_mode = mode;

        if (mode == e_screen_show_all)
        {
            gtk_widget_set_size_request(g_pcname,        PC_CONNECT_SCREEN_WIDTH, PC_CONNECT_PCNAME_HEIGHT          );
            gtk_widget_set_size_request(g_sharename,     PC_CONNECT_SCREEN_WIDTH, PC_CONNECT_SHARENAME_HEIGHT       );
            gtk_widget_set_size_request(g_workgroup,     PC_CONNECT_SCREEN_WIDTH, PC_CONNECT_WORKGROUP_HEIGHT       );
            gtk_widget_set_size_request(g_user_password, PC_CONNECT_SCREEN_WIDTH, PC_CONNECT_USER_PASSWORD_BK_HEIGHT);
        }
        else
        {
            gtk_widget_set_size_request(g_pcname,        PC_CONNECT_SCREEN_WIDTH, PC_CONNECT_PCNAME_HEIGHT           - 15);
            gtk_widget_set_size_request(g_sharename,     PC_CONNECT_SCREEN_WIDTH, PC_CONNECT_SHARENAME_HEIGHT        - 15);
            gtk_widget_set_size_request(g_workgroup,     PC_CONNECT_SCREEN_WIDTH, PC_CONNECT_WORKGROUP_HEIGHT        - 15);
            gtk_widget_set_size_request(g_user_password, PC_CONNECT_SCREEN_WIDTH, PC_CONNECT_USER_PASSWORD_BK_HEIGHT - 15);
        }

        switch (mode)
        {
            case e_screen_enter_pcname:
                gtk_widget_show(g_pcname);
                gtk_settingitem_textentry_grabfocus(GTK_SETTINGITEM_TEXTENTRY(g_pcname));
                gtk_widget_show(g_pcname_info);
                gtk_widget_hide(g_sharename);
                gtk_widget_hide(g_sharename_info);
                gtk_widget_hide(g_workgroup);
                gtk_widget_hide(g_workgroup_info);
                gtk_widget_hide(g_user_password);
                gtk_widget_hide(g_user_password_info);
                break;

            case e_screen_enter_sharename:
                gtk_widget_show(g_pcname);
                gtk_widget_hide(g_pcname_info);
                gtk_widget_show(g_sharename);
                gtk_settingitem_textentry_grabfocus(GTK_SETTINGITEM_TEXTENTRY(g_sharename));
                gtk_widget_show(g_sharename_info);
                gtk_widget_hide(g_workgroup);
                gtk_widget_hide(g_workgroup_info);
                gtk_widget_hide(g_user_password);
                gtk_widget_hide(g_user_password_info);
              break;

            case e_screen_enter_workgroup:
                gtk_widget_show(g_pcname);
                gtk_widget_hide(g_pcname_info);
                gtk_widget_show(g_sharename);
                gtk_widget_hide(g_sharename_info);
                gtk_widget_show(g_workgroup);
                gtk_settingitem_textentry_grabfocus(GTK_SETTINGITEM_TEXTENTRY(g_workgroup));
                gtk_widget_show(g_workgroup_info);
                gtk_widget_hide(g_user_password);
                gtk_widget_hide(g_user_password_info);
                break;

            case e_screen_enter_user_password:
                gtk_widget_show(g_pcname);
                gtk_widget_hide(g_pcname_info);
                gtk_widget_show(g_sharename);
                gtk_widget_hide(g_sharename_info);
                gtk_widget_show(g_workgroup);
                gtk_widget_hide(g_workgroup_info);
                gtk_widget_show(g_user_password);
                gtk_widget_show(g_user_password_info);
                gtk_widget_grab_focus(g_user_entry);
                break;
          
            case e_screen_show_all:
                gtk_widget_show(g_pcname);
                gtk_widget_hide(g_pcname_info);
                gtk_widget_show(g_sharename);
                gtk_widget_hide(g_sharename_info);
                gtk_widget_show(g_workgroup);
                gtk_widget_hide(g_workgroup_info);
                gtk_widget_show(g_user_password);
                gtk_widget_hide(g_user_password_info);
                break;

            default:
                /* ignore */ ;
        }
        display_update_request_screen_refresh(SETUP_PAGE_LAYOUT_CHANGE, WAVEFORM_FULLSCREEN);
    }
}

// signal handlers for pcname
static void on_pcname_changed(GtkWidget* widget, const gchar* value)
{
    ST_LOGPRINTF("entry");

    iLiad_pc_connect_set_profile_field(0, e_pcfield_pcname, value);
}

static void on_pcname_button_press(GtkWidget* widget)
{
    ST_LOGPRINTF("entry");
    iLiad_pc_connect_set_screenmode(e_screen_enter_pcname);
}

static void on_pcname_info_button_clicked(GtkWidget* widget, gboolean button_active)
{
    GtkInfoItem *item = GTK_INFOITEM(widget);
    ST_LOGPRINTF("entry");

    if (button_active)
    {
        gtk_infoitem_set_buttonstate(item, FALSE);
        iLiad_pc_connect_set_screenmode(e_screen_enter_sharename);
    }
}

static void on_pcname_validate(GtkWidget* widget, const gchar* text, gchar** newtext)
{
    gboolean       do_split;
    const gchar*   pcname_old = text;
    gchar*         pcname_new = NULL;
    const gchar*   workgroup_old = gtk_settingitem_textentry_gettext(GTK_SETTINGITEM_TEXTENTRY(g_workgroup));
    gchar*         workgroup_new = NULL;

    ST_LOGPRINTF("entry: text [%s]", text);

    // check for domain name in pcname
    do_split = pcname_check_split(pcname_old, &pcname_new, workgroup_old, &workgroup_new);
    if (do_split)
    {
        gtk_settingitem_textentry_settext(GTK_SETTINGITEM_TEXTENTRY(g_workgroup), workgroup_new);
        iLiad_pc_connect_set_profile_field(0, e_pcfield_pcname,    pcname_new   );
        iLiad_pc_connect_set_profile_field(0, e_pcfield_workgroup, workgroup_new);
        g_free(workgroup_new);
    }
    *newtext = pcname_new;
}

static gboolean pcname_check_split(const gchar* pcname_old,    gchar** pcname_new, 
                                   const gchar* workgroup_old, gchar** workgroup_new)
{
    gboolean       do_split = FALSE;  // return
    gchar*         cp;
    struct in_addr ip_addr;

    ST_LOGPRINTF("entry");

    // check for domain name in pcname
    cp = strchr(pcname_old, '.');
    if (cp  &&  inet_aton(pcname_old, &ip_addr) == 0)
    {
        // pcname seems to contain domain name and is not a valid IP address
        if (   workgroup_old[0] == '\0'
            || strcmp(cp + 1, workgroup_old) == 0 )
        {
            // workgroup is empty or pcname ends on workgroup:
            //     strip domain from pcname and move it to workgroup
            *pcname_new = g_strdup(pcname_old);
            cp = strchr(*pcname_new, '.');
            *cp = '\0';
            *workgroup_new = g_strdup(cp + 1);
            // report split required
            do_split = TRUE;
        }
    }
    return do_split;
}

// signal handlers for sharename
static void on_sharename_changed(GtkWidget* widget, const gchar* value)
{
    ST_LOGPRINTF("entry");

    iLiad_pc_connect_set_profile_field(0, e_pcfield_sharename, value);
}

static void on_sharename_button_press(GtkWidget* widget)
{
    ST_LOGPRINTF("entry");
    iLiad_pc_connect_set_screenmode(e_screen_enter_sharename);
}

static void on_sharename_info_button_clicked(GtkWidget* widget, gboolean button_active)
{
    GtkInfoItem *item = GTK_INFOITEM(widget);
    ST_LOGPRINTF("entry");

    if (button_active)
    {
        gtk_infoitem_set_buttonstate(item, FALSE);
        iLiad_pc_connect_set_screenmode(e_screen_enter_workgroup);
    }
}

// signal handlers for workgroup 
static void on_workgroup_changed(GtkWidget* widget, const gchar* value)
{
    ST_LOGPRINTF("entry");

    iLiad_pc_connect_set_profile_field(0, e_pcfield_workgroup, value);
}

static void on_workgroup_validate(GtkWidget* widget, const gchar* text, gchar** newtext)
{
    gboolean       do_split;
    const gchar*   pcname_old = gtk_settingitem_textentry_gettext(GTK_SETTINGITEM_TEXTENTRY(g_pcname));
    gchar*         pcname_new = NULL;
    const gchar*   workgroup_old = text;
    gchar*         workgroup_new = NULL;

    ST_LOGPRINTF("entry: text [%s]", text);

    // check for domain name in pcname
    do_split = pcname_check_split(pcname_old, &pcname_new, workgroup_old, &workgroup_new);
    if (do_split)
    {
        gtk_settingitem_textentry_settext(GTK_SETTINGITEM_TEXTENTRY(g_pcname), pcname_new);
        iLiad_pc_connect_set_profile_field(0, e_pcfield_pcname,    pcname_new   );
        iLiad_pc_connect_set_profile_field(0, e_pcfield_workgroup, workgroup_new);
        g_free(pcname_new);
    }
    *newtext = workgroup_new;
}

static void on_workgroup_button_press(GtkWidget* widget)
{
    ST_LOGPRINTF("entry");
    iLiad_pc_connect_set_screenmode(e_screen_enter_workgroup);
}

static void on_workgroup_info_button_clicked(GtkWidget* widget, gboolean button_active)
{
    GtkInfoItem *item = GTK_INFOITEM(widget);
    ST_LOGPRINTF("entry");

    if (button_active)
    {
        gtk_infoitem_set_buttonstate(item, FALSE);
        iLiad_pc_connect_set_screenmode(e_screen_enter_user_password);
    }
}

// signal handlers for username/password
static void on_username_changed(GtkEditable* editable, gpointer user_data)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(ERGTK_IS_ENTRY(editable));

    GtkEntry*    entry = GTK_ENTRY(editable);
    const gchar* value = gtk_entry_get_text(entry);

    iLiad_pc_connect_set_profile_field(0, e_pcfield_username, value);
}

static void on_password_changed(GtkEditable* editable, gpointer user_data)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(ERGTK_IS_ENTRY(editable));

    GtkEntry*    entry = GTK_ENTRY(editable);
    const gchar* value = gtk_entry_get_text(entry);

    iLiad_pc_connect_set_profile_field(0, e_pcfield_password, value);
}

static gboolean on_user_password_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    ST_LOGPRINTF("entry");
    iLiad_pc_connect_set_screenmode(e_screen_enter_user_password);

    return FALSE;  // continue GTK signal handling
}

static void on_user_password_info_button_clicked(GtkWidget *widget, const gboolean button_active)
{
    GtkInfoItem *item = GTK_INFOITEM(widget);
    ST_LOGPRINTF("entry");

    if (button_active)
    {
        gtk_infoitem_set_buttonstate(item, FALSE);
        iLiad_pc_connect_set_screenmode(e_screen_show_all);
    }
}

