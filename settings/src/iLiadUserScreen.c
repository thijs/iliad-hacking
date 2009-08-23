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
 * \file iLiadUserScreen.c
 * \brief setup application - E-reader config screen                

 * <File description>
 * 
 */

#include <gtk/gtk.h>

#include <libergtk/ergtk.h>

#include "setupLog.h"
#include "background.h"
#include "gtkInfoItem.h"
#include "gtkSettingItem.h"
#include "gtkSettingItemTextEntry.h"
#include "iLiadUserScreen.h"
#include "iLiadUserData.h"
#include "languages.h"

static GtkWidget *g_top_level_box = NULL;

static GtkWidget *g_name_item = NULL;
static GtkWidget *g_name_item_entry = NULL;

static gtkAccountSettings_t* g_account_settings = NULL;

static GtkWidget *g_info_item = NULL;

// internally used routines
static GtkWidget *create_name_item(void);
static void create_account_settings(gtkAccountSettings_t* acc);

// callback functions
static void on_account_email_changed(GtkEditable* widget, gpointer data);
static void on_account_password_changed(GtkEditable* widget, gpointer data);

// object hierarchy:
//     g_top_level_box
//       |-- g_name_item (GtkSettingItem)
//       |-- g_account_settings (GtkSettingItem)
//
GtkWidget *iLiad_user_create(void)
{
    g_top_level_box = gtk_vbox_new(FALSE, USERSCREEN_ITEM_SPACING);

    g_name_item = create_name_item();
    gtk_box_pack_start(GTK_BOX(g_top_level_box), GTK_WIDGET(g_name_item), FALSE, FALSE, 0);

    g_account_settings = g_new0(gtkAccountSettings_t, 1);
    create_account_settings(g_account_settings);
    gtk_box_pack_start(GTK_BOX(g_top_level_box), GTK_WIDGET(g_account_settings->settingitem), FALSE, FALSE, 0);

    g_info_item = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(g_top_level_box), g_info_item, FALSE, FALSE, 0);

    gtk_widget_show(g_top_level_box);

    return g_top_level_box;
}

// object hierarchy:
//     g_name_item (GtkSettingItem)
//       |-- g_name_item_entry (GtkSettingItemTextEntry)
//
static GtkWidget *create_name_item(void)
{
    GtkWidget *widget;

    // g_name_item
    g_name_item = gtk_settingitem_new("");
    gtk_widget_set_size_request(GTK_WIDGET(g_name_item), (-1), USERNAME_ITEM_REQ_HEIGHT);

    // g_name_item_entry
    widget = gtk_settingitem_textentry_new();
    gtk_settingitem_add_details(GTK_SETTINGITEM(g_name_item), widget);
    g_name_item_entry = widget;

    // signal callback
    gtk_settingitem_textentry_callback_on_changed(GTK_SETTINGITEM_TEXTENTRY(widget), on_user_name_changed);

    // display
    gtk_widget_show(g_name_item_entry);
    gtk_widget_show(g_name_item);

    return g_name_item;
}

// object hierarchy
//     settingitem (GtkSettingItem)
//       |-- background (event-box)
//           |-- alignment
//               |-- vbox
//                   |-- label
//                   |-- hbox
//                       |-- entry
//                   |-- label
//                   |-- hbox
//                       |-- entry
static void create_account_settings(gtkAccountSettings_t* acc)
{
    GtkWidget* settingitem;
    GtkWidget* background;
    GtkWidget* alignment;
    GtkWidget* vbox;
    GtkWidget* hbox;
    GtkWidget* label;
    GtkWidget* entry;

    ST_LOGPRINTF("entry");

    // settingitem
    settingitem = gtk_settingitem_new("");
    gtk_widget_set_size_request(settingitem, -1, -1);
    acc->settingitem = settingitem;

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "user_account_settings_background");
    gtk_widget_set_size_request(background, TITLE_MIN_WIDTH, USERSCREEN_ACC_HEIGHT);
    gtk_settingitem_add_details(GTK_SETTINGITEM(settingitem), background);

    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              USERSCREEN_ITEM_PADDING_TOP,
                              USERSCREEN_ITEM_PADDING_BOTTOM,
                              USERSCREEN_ITEM_PADDING_LEFT,
                              USERSCREEN_ITEM_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    vbox = gtk_vbox_new(FALSE, ACCOUNT_SETTINGS_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    label = gtk_label_new("");
    gtk_widget_set_name(label, "settingitem_te_label");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.6);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, USERSCREEN_ITEM_SPACING);
    acc->label_email = label;

    hbox = gtk_hbox_new(FALSE, SETTINGITEMTEXT_HBOX_SPACING);
    gtk_widget_set_size_request(hbox, -1, -1);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    entry = ergtk_entry_new();
    gtk_widget_set_size_request(entry, SETTINGITEMTEXT_ENTRY_WIDTH, SETTINGITEMTEXT_ENTRY_HEIGHT);
    gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);
    acc->entry_email = entry;
                 
    label = gtk_label_new("");
    gtk_widget_set_name(label, "settingitem_te_label");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.6);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, USERSCREEN_ITEM_SPACING);
    acc->label_password = label;

    hbox = gtk_hbox_new(FALSE, SETTINGITEMTEXT_HBOX_SPACING);
    gtk_widget_set_size_request(hbox, -1, -1);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
            
    entry = ergtk_entry_new();
    gtk_widget_set_size_request(entry, SETTINGITEMTEXT_ENTRY_WIDTH, SETTINGITEMTEXT_ENTRY_HEIGHT);
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
    gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);
    acc->entry_password = entry;
    
    g_signal_connect(GTK_OBJECT(acc->entry_email),    "changed-stable", G_CALLBACK(on_account_email_changed),    NULL);
    g_signal_connect(GTK_OBJECT(acc->entry_password), "changed-stable", G_CALLBACK(on_account_password_changed), NULL);

    gtk_widget_show_all(background);
    gtk_widget_show(settingitem);
}

void iLiad_user_set_text(void)
{
    ST_LOGPRINTF("entry");

    // name
    if (g_name_item)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_name_item), _("Name"));
        gtk_settingitem_textentry_setlabel(GTK_SETTINGITEM_TEXTENTRY(g_name_item_entry), _("Please enter your name"));
    }

    // Email address and password
    if (g_account_settings)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_account_settings->settingitem), _("MyiRex account settings"));
        gtk_label_set_text(GTK_LABEL(g_account_settings->label_email),  _("Please enter the email address used to create your 'MyiRex' account"));
        gtk_label_set_text(GTK_LABEL(g_account_settings->label_password),  _("Please enter your 'MyiRex' password"));
    }
    
    if (g_info_item)
    {
       // Information text
        gtk_infoitem_set_text(GTK_INFOITEM(g_info_item),
                              _("\nTo connect to iRex Delivery Service (iDS) you need to create an account."
                                "\nVisit https://myirex.irexnet.com/ and follow the instructions."));
    }
}

// callback functions

static void on_account_email_changed(GtkEditable* widget, gpointer data)
{
    GtkWidget* item = g_account_settings->entry_email;
    on_user_email_changed(item, gtk_entry_get_text(GTK_ENTRY(item)));
}

static void on_account_password_changed(GtkEditable* widget, gpointer data)
{
    GtkWidget* item = g_account_settings->entry_password;
    on_user_password_changed(item, gtk_entry_get_text(GTK_ENTRY(item)));
}

// set/get functions
void iLiad_user_set_name(const char* name)
{
    g_return_if_fail(name != NULL);

    gtk_settingitem_textentry_settext(GTK_SETTINGITEM_TEXTENTRY(g_name_item_entry), name);
}

void iLiad_user_set_email(const char* email)
{
    g_return_if_fail(email != NULL);
    
    gtk_entry_set_text(GTK_ENTRY(g_account_settings->entry_email), email);
}

void iLiad_user_set_password(const char* password)
{
    g_return_if_fail(password != NULL);

    gtk_entry_set_text(GTK_ENTRY(g_account_settings->entry_password), password);
}

