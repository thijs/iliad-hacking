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
 * \file iLiadDeviceOverview.c
 * \brief settings - Device detail overview
 *
 * To allow Service and users to get more and neccessary device details
 *
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/utsname.h>
#include <gtk/gtk.h>

#include <liberdm/display.h>
#include <libergtk/ergtk.h>
#include "gtkSettingItem.h"
#include "setupLog.h"
#include "iLiadDeviceOverview.h"
#include "languages.h"
#include "system.h"

// global variables
static gtk_dev_overview* g_dev_overview_screen = NULL;

// internal functions
static GtkWidget* gtk_dev_info_new(gtkDevInfo_t* dev);
static GtkWidget* gtk_ack_new(gtkAck_t* ack);
static void gtk_insert_separator(GtkBox *vbox);

// widget hierarchy
// |--vbox
//    |--dev (GtkSettingItem)
//    |--ack (GtkSettingItem)
GtkWidget *iLiad_device_overview_create(void)
{
    GtkWidget* vbox;
    GtkWidget* dev;
    GtkWidget* ack;
    
    ST_LOGPRINTF("entry");
    g_dev_overview_screen = g_new0(gtk_dev_overview, 1);

    // Top level vbox
    vbox = gtk_vbox_new(FALSE, DEV_OVERVIEW_ITEM_SPACING);
    g_dev_overview_screen->top_level_vbox = vbox;
    
    dev = gtk_dev_info_new(&g_dev_overview_screen->gtk_dev_info);
    gtk_box_pack_start(GTK_BOX(vbox), dev, FALSE, FALSE, 0);

    ack = gtk_ack_new(&g_dev_overview_screen->gtk_acknowledgement);
    gtk_box_pack_start(GTK_BOX(vbox), ack, FALSE, FALSE, 0);

    // show widgets
    gtk_widget_show(vbox);
    return vbox;
}

void gtk_insert_separator(GtkBox *vbox)
{
    GtkWidget* hbox;

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(hbox), FALSE, FALSE, 0);
    gtk_widget_set_size_request(hbox, TITLE_MIN_WIDTH, DEV_OVERVIEW_SEPARATOR_HEIGHT);
}

// widget hierarchy
// |--settingitem (GtkSettingItem)
//    |--background (GtkEventBox)
//       |--alignment
//          |--vbox
//             |--hbox
//             |--hbox
//             |--hbox
//             |--hbox
//             |--hbox
//             |--hbox
//             |--hbox
//             |--hbox
//             |--hbox
//             |--hbox
//             |--hbox
//             |--hbox
//             |--hbox
GtkWidget* gtk_dev_info_new(gtkDevInfo_t* dev)
{
    GtkWidget* settingitem;
    GtkWidget* background;
    GtkWidget* alignment;
    GtkWidget* vbox;
    GtkWidget* hbox;
    GtkWidget* label;

    gint i = 0;

    ST_LOGPRINTF("entry");
    
    g_return_val_if_fail(NULL != dev, NULL);

    // settingitem
    settingitem = gtk_settingitem_new("");
    gtk_widget_set_size_request(settingitem, -1, -1);
    dev->settingitem = settingitem;

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "device_info_background");
    gtk_widget_set_size_request(background, TITLE_MIN_WIDTH, DEVICE_INFO_HEIGHT);
    gtk_settingitem_add_details(GTK_SETTINGITEM(settingitem), background);

    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              DEVICE_OVERVIEW_ITEM_PADDING_TOP, DEVICE_OVERVIEW_ITEM_PADDING_BOTTOM,
                              DEVICE_OVERVIEW_ITEM_PADDING_LEFT, DEVICE_OVERVIEW_ITEM_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    vbox = gtk_vbox_new(FALSE, DEV_OVERVIEW_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    for (i=0; i<DEV_INFO_ITEM_NUMBER; i++)
    {
        if (i == 6 || i == 9)
        {
            gtk_insert_separator(GTK_BOX(vbox));
        }

        hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(hbox), FALSE, FALSE, 0);

        label = gtk_label_new("");
        gtk_widget_set_name(label, "device_overview_info_text");
        gtk_widget_set_size_request(label, DEVICE_INFO_LABEL_WIDTH, -1);
        gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
        gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
        dev->label_item_title[i] = label;

        dev->label_item_value[i] = gtk_label_new("");
        gtk_widget_set_name(dev->label_item_value[i], "device_overview_info_text");
        gtk_box_pack_start(GTK_BOX(hbox), dev->label_item_value[i], FALSE, FALSE, 0);
    }

    gtk_widget_show_all(background);
    gtk_widget_show(settingitem);
    return settingitem;
}

// widget hierarchy
// |--settingitem (GtkSettingItem)
//    |--background (GtkEventBox)
//       |--alignment
//          |--vbox
//             |--label
//             |--label
//             |--label
GtkWidget* gtk_ack_new(gtkAck_t* ack)
{
    GtkWidget* settingitem;
    GtkWidget* background;
    GtkWidget* alignment;
    GtkWidget* vbox;
    GtkWidget* label;
    gint i = 0;
    
    ST_LOGPRINTF("entry");

    g_return_val_if_fail(NULL != ack, NULL);

    // settingitem
    settingitem = gtk_settingitem_new("");
    gtk_widget_set_size_request(settingitem, -1, -1);
    ack->settingitem = settingitem;

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "device_ack_background");
    gtk_widget_set_size_request(background, TITLE_MIN_WIDTH, DEVICE_ACK_HEIGHT);
    gtk_settingitem_add_details(GTK_SETTINGITEM(settingitem), background);

    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              DEVICE_OVERVIEW_ITEM_PADDING_TOP, DEVICE_OVERVIEW_ITEM_PADDING_BOTTOM,
                              DEVICE_OVERVIEW_ITEM_PADDING_LEFT, DEVICE_OVERVIEW_ITEM_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    vbox = gtk_vbox_new(FALSE, DEV_OVERVIEW_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    label = gtk_label_new("");
    gtk_widget_set_name(label, "device_overview_info_text");
    gtk_label_set_single_line_mode(GTK_LABEL(label), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_widget_set_size_request(label, ACK_DESCRIPTION_LABEL_WIDTH, -1);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    ack->label_description = label;

    for (i=0; i<DEV_ACK_ITEM_NUMBER; i++)
    {
        ack->label_ack_item[i] = gtk_label_new("");
        gtk_widget_set_name(ack->label_ack_item[i], "device_overview_info_text");
        gtk_misc_set_alignment(GTK_MISC(ack->label_ack_item[i]), 0.0, 0.0);
        gtk_box_pack_start(GTK_BOX(vbox), ack->label_ack_item[i], FALSE, FALSE, 0);
    }

    gtk_widget_show_all(background);
    gtk_widget_show(settingitem);
    return settingitem;
}

void iLiad_device_overview_set_text(void)
{
    gtkDevInfo_t* dev;
    gtkAck_t*     ack;

    ST_LOGPRINTF("entry");

    if (g_dev_overview_screen)
    {
        dev = &g_dev_overview_screen->gtk_dev_info;
        ack = &g_dev_overview_screen->gtk_acknowledgement;

        gtk_settingitem_set_header_text(GTK_SETTINGITEM(dev->settingitem), _("Device Identification"));

        gtk_label_set_text(GTK_LABEL(dev->label_item_title[0]),  _("Product type:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[1]),  _("Language set:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[2]),  _("OEM code:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[3]),  _("Country code:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[4]),  _("PCBA revision:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[5]),  _("EPD ID:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[6]),  _("Product ID:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[7]),  _("Wired MAC address:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[8]),  _("Wireless MAC address:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[9]),  _("Software version:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[10]), _("Software build number:"));
        gtk_label_set_text(GTK_LABEL(dev->label_item_title[11]), _("Kernel version:"));

        gtk_settingitem_set_header_text(GTK_SETTINGITEM(ack->settingitem), _("Acknowledgements"));

        gtk_label_set_text(GTK_LABEL(ack->label_description),
                _("iRex Technologies wants to thank the following people for contributing to the iLiad software:"));

        char tmp_str[128];
        snprintf(tmp_str, 128, _("German translation: %s"), "Henning Angelo Wolter");
        gtk_label_set_text(GTK_LABEL(ack->label_ack_item[0]), tmp_str);

        snprintf(tmp_str, 128, _("Stylus calibration: %s"), "Jay Kuri, Rob Mills, John Harker");
        gtk_label_set_text(GTK_LABEL(ack->label_ack_item[1]), tmp_str);
    }
}

void iLiad_device_overview_display_data(void)
{
    gtkDevInfo_t* dev = &g_dev_overview_screen->gtk_dev_info;

    char   tmp[20];
    struct utsname uname_buf;
    
    ST_LOGPRINTF("entry");
    if (g_dev_overview_screen->top_level_vbox)
    {
        gtk_label_set_text(GTK_LABEL(dev->label_item_value[0]), get_sysset_fa_model());

        sprintf(tmp, "%03d", get_sysset_fa_region());
        gtk_label_set_text(GTK_LABEL(dev->label_item_value[1]), tmp);

        gtk_label_set_text(GTK_LABEL(dev->label_item_value[2]), get_sysset_fa_oem_code());
        gtk_label_set_text(GTK_LABEL(dev->label_item_value[3]), get_sysset_fa_country_code());

        sprintf(tmp, "%d", get_sysset_pa_board_revision());
        gtk_label_set_text(GTK_LABEL(dev->label_item_value[4]), tmp);
        gtk_label_set_text(GTK_LABEL(dev->label_item_value[5]), get_sysset_epd_id());

        gtk_label_set_text(GTK_LABEL(dev->label_item_value[6]), get_sysset_product_id());
        gtk_label_set_text(GTK_LABEL(dev->label_item_value[7]), get_wired_macAddress());
        gtk_label_set_text(GTK_LABEL(dev->label_item_value[8]), get_wireless_macAddress());

        FILE *fp = fopen(VERSION_FILE, "r");
        if (NULL != fp)
        {
            char version[40];
            fgets(version, sizeof(version)-1, fp);
            fclose(fp);
            char *new_line = strchr(version, '\n');
            if (new_line)
            {
                *new_line = '\0';
            }
            
            gtk_label_set_text(GTK_LABEL(dev->label_item_value[9]), version);
        }

        fp = fopen(BUILD_FILE, "r");
        if (NULL != fp)
        {
            char build_number[40];
            fgets(build_number, sizeof(build_number)-1, fp);
            fclose(fp);
            char *new_line = strchr(build_number, '\n');
            if (new_line)
            {
                *new_line = '\0';
            }

            gtk_label_set_text(GTK_LABEL(dev->label_item_value[10]), build_number);
        }

        uname(&uname_buf);
        char *p = NULL;
        for (p = uname_buf.version; !isdigit(*p); p++);
        int kernel_build_number = atoi(p);

        char kernel_version[32];
        snprintf(kernel_version, 32, "%s #%d", uname_buf.release, kernel_build_number);
        gtk_label_set_text(GTK_LABEL(dev->label_item_value[11]), kernel_version);
    }
}

