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
 * \file editScreenStatic.c
 * \brief connectionMgr - "Static IP address" screen
 
 * Copyright (C) 2007 iRex Technologies BV.
 * 
 */

#include "config.h"
#include <string.h>
#include <gtk/gtk.h>
#include <liberregxml/erregapi.h>

#include "connectionMgrLog.h"
#include "connectionMgr.h"
#include "displayStatus.h"
#include "erbusy.h"
#include "background.h"
#include "gtkSettingItem.h"
#include "gtkInfoItem.h"
#include "editScreen.h"
#include "editScreenOverview.h"
#include "widgetUtils.h"
#include "editScreenStatic.h"
#include "languages.h"

typedef enum
{
    inputAddr_e = 0,
    inputNetmask_e,
    inputGateway_e,
    inputDns_e,
    undefStatus_e
}staticStatus_t;

static GtkWidget *g_static = NULL;

static wdtLabelEntry *g_address = NULL;
static wdtLabelEntry *g_netmask = NULL;
static wdtLabelEntry *g_gateway = NULL;
static wdtLabelEntry *g_dns = NULL;

static GtkWidget *g_info = NULL;

// Create a backup of the settings, need it when user "Cancel"s his changes.
static regIpSetting_t *g_old_settings = NULL;

static void on_next(GtkToggleButton *button, gpointer data);

static void edit_static_set_address(const char *address);
static void edit_static_set_netmask(const char *netmask);
static void edit_static_set_gateway(const char *gateway);
static void edit_static_set_dns(const char *dns);

static const char *edit_static_get_address(void);
static const char *edit_static_get_netmask(void);
static const char *edit_static_get_gateway(void);
static const char *edit_static_get_dns(void);

static void edit_static_backup_network_settings(void);


// 
// object hierarchy:
//   |--item (gtkSettingItem)
//      |--topVbox
//         |--background (event box)
//            |--alignment
//               |--vbox
//                  |--address (wdtLabelEntry)
//                  |--netmask (wdtLabelEntry)
//                  |--gateway (wdtLabelEntry)
//                  |--dns     (wdtLabelEntry)
//      |--info
//       
GtkWidget *edit_static_create(void)
{
    GtkWidget *item;
    GtkWidget *topVbox;
    GtkWidget *background;
    GtkWidget *alignment;
    GtkWidget *vbox;
    wdtLabelEntry *address;
    wdtLabelEntry *netmask;
    wdtLabelEntry *gateway;
    wdtLabelEntry *dns;
    GtkWidget *info;
    GtkWidget *widget;

    CN_LOGPRINTF("entry");

    // item
    item = gtk_settingitem_new("");

    // topVbox
    topVbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), topVbox);
 
    // background 
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "bk_grey_666_273");
    gtk_widget_set_size_request(background, 
            BK_666_273_WIDTH, BK_666_273_HEIGHT);
    gtk_box_pack_start(GTK_BOX(topVbox), background, FALSE, FALSE, 0);

    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
            PADDING_TOP, PADDING_BOTTOM, PADDING_LEFT, PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);
    
    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    // address
    address = wdt_label_entry_new(ipAddress_e);
    gtk_box_pack_start(GTK_BOX(vbox), address->parent, FALSE, FALSE, 0);

    // netmask
    netmask = wdt_label_entry_new(ipAddress_e);
    gtk_box_pack_start(GTK_BOX(vbox), netmask->parent, FALSE, FALSE, 0);

    // gateway
    gateway = wdt_label_entry_new(ipAddress_e);
    gtk_box_pack_start(GTK_BOX(vbox), gateway->parent, FALSE, FALSE, 0);

    // dns
    dns = wdt_label_entry_new(ipAddress_e);
    gtk_box_pack_start(GTK_BOX(vbox), dns->parent, FALSE, FALSE, 0);
 
    // info item
    info = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(topVbox), info, FALSE, FALSE, 0);

    // signal handlers
    widget = dns->button;
    g_signal_connect_after(G_OBJECT(widget), "toggled", 
                           G_CALLBACK(on_next), NULL);
 
    // show the widgets
    gtk_widget_show(item);
    gtk_widget_show(topVbox);
    gtk_widget_show(background);
    gtk_widget_show(alignment);
    gtk_widget_show(vbox);
    gtk_widget_show(address->parent);
    gtk_widget_hide(address->button);
    gtk_widget_show(netmask->parent);
    gtk_widget_hide(netmask->button);
    gtk_widget_show(gateway->parent);
    gtk_widget_hide(gateway->button);
    gtk_widget_show(dns->parent);
    gtk_widget_show(info);
 
    // set the global variables
    g_static = item;
    g_address = address;
    g_netmask = netmask;
    g_gateway = gateway;
    g_dns = dns;
    g_info = info;

    CN_LOGPRINTF("done");

    // return
    return item;
}

void edit_static_set_text(void)
{
    GtkWidget* widget = NULL;

    CN_LOGPRINTF("entry");
    
    if (g_static)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_static),
                                        _("Network profile"));
    }
    
    if (g_address)
    {
        widget = g_address->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                _("Enter the iLiad's IP address:"));
    }
    
    if (g_netmask)
    {
        widget = g_netmask->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                _("Enter the iLiad's netmask:"));
    }
    
    if (g_gateway)
    {
        widget = g_gateway->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                _("Enter the default gateway's IP address:"));
   }

    if (g_dns)
    {
        widget = g_dns->label;
        gtk_label_set_text(GTK_LABEL(widget), 
                _("Enter the DNS name server's IP address:"));
        
        widget = g_dns->button;
        gtk_button_set_label(GTK_BUTTON(widget), _("Next"));
    }

    if (g_info)
    {
        gtk_infoitem_set_text(GTK_INFOITEM(g_info),
                _("If you do not know the settings for this network, "
                  "please ask your network administrator."));
    }
}

static staticStatus_t edit_static_detect_status(void)
{
    staticStatus_t status = undefStatus_e;

    if (!ergtk_entry_check_field(ERGTK_ENTRY(g_address->entry)))
    {
        status = inputAddr_e;
    }
    else if (!ergtk_entry_check_field(ERGTK_ENTRY(g_netmask->entry)))
    {
        status = inputNetmask_e;
    }
    else if (!ergtk_entry_check_field(ERGTK_ENTRY(g_gateway->entry)))
    {
        status = inputGateway_e;
    }
    else if (!ergtk_entry_check_field(ERGTK_ENTRY(g_dns->entry)))
    {
        status = inputDns_e;
    }

    CN_LOGPRINTF("return %d", status);
    return status;
}

static void edit_static_determine_status(void)
{
    staticStatus_t status;
    GtkWidget *widget = NULL;

    status = edit_static_detect_status();
    switch (status)
    {
        case inputAddr_e:
            widget = g_address->entry;
            break;
        case inputNetmask_e:
            widget = g_netmask->entry;
            break;
        case inputGateway_e:
            widget = g_gateway->entry;
            break;
        case inputDns_e:
            widget = g_dns->entry;
            break;
        default:
            widget = g_address->entry;
            break;
    }

    if (widget)
    {
        gtk_widget_grab_focus(widget);
    }
}

// set/get functions
void edit_static_set_network_settings(const regNetworkProfile_t *settings)
{
    gchar *profilename = NULL;
    gchar *address = NULL;
    gchar *netmask = NULL;
    gchar *gateway = NULL;
    gchar *dns     = NULL;
    regIpSetting_t* ipSettings;

    CN_LOGPRINTF("entry [%p]", settings);

    g_return_if_fail(settings != NULL);
    // g_return_if_fail(settings->addressMode == static_t);

    profilename = settings->name;

    ipSettings = settings->ipSettings;
    if (ipSettings)
    {
        address = ipSettings->address;
        netmask = ipSettings->netmask;
        gateway = ipSettings->gateway;
        dns = ipSettings->dns;
    }
 
    edit_static_set_title(profilename);
    edit_static_set_address(address);
    edit_static_set_netmask(netmask);
    edit_static_set_gateway(gateway);
    edit_static_set_dns(dns);

    edit_static_determine_status();

    // a backup copy of settings
    edit_static_backup_network_settings();
}

void edit_static_set_title(const char *profilename)
{
    char *title = NULL;

    CN_LOGPRINTF("entry[%s]", profilename);
    
    if (g_static)
    {
        if (profilename)
        {
            title = g_strdup_printf(_("Network profile: %s/static"), 
                                    profilename);
        }
        else
        {
            title = g_strdup(_("Network profile: static"));
        }

        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_static), title);

        g_free(title);
    }
}

static void edit_static_set_address(const char *address)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry %s", address);

    if (g_address)
    {
        widget = g_address->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), address ? address : "");
    }
}

static void edit_static_set_netmask(const char *netmask)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry [%s]", netmask);

    if (g_netmask)
    {
        widget = g_netmask->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), netmask ? netmask : "");
    }
}

static void edit_static_set_gateway(const char *gateway)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry [%s]", gateway);

    if (g_gateway)
    {
        widget = g_gateway->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), gateway ? gateway : "");
    }
}

static void edit_static_set_dns(const char *dns)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry [%s]", dns);

    if (g_dns)
    {
        widget = g_dns->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), dns ? dns : "");
    }
}

void edit_static_get_network_settings(regNetworkProfile_t *settings)
{
    const gchar *address, *netmask, *gateway, *dns;
    regIpSetting_t *ipSettings;

    CN_LOGPRINTF("entry");

    g_return_if_fail(settings != NULL);
    g_return_if_fail(settings->addressMode == static_t);

    // clear static IP settings
    ipSettings = settings->ipSettings;
    if (ipSettings)
    {
        g_free(ipSettings->address);
        g_free(ipSettings->netmask);
        g_free(ipSettings->gateway);
        g_free(ipSettings->dns);
        g_free(ipSettings);
        ipSettings = NULL;
        settings->ipSettings = NULL;
    }

    // get current value from screen objects
    address = edit_static_get_address();
    netmask = edit_static_get_netmask();
    gateway = edit_static_get_gateway();
    dns = edit_static_get_dns();

    // store new settings, if static IP selected
    if (settings->addressMode == static_t)
    {
        ipSettings = g_new0(regIpSetting_t, 1);
        if (ipSettings)
        {
            ipSettings->address = g_strdup(address);
            ipSettings->netmask = g_strdup(netmask);
            ipSettings->gateway = g_strdup(gateway);
            ipSettings->dns     = g_strdup(dns    );
        }
        settings->ipSettings = ipSettings;
    }
}

static const char *edit_static_get_address(void)
{
    GtkWidget *widget;
    const char *address = NULL;

    if (g_address)
    {
        widget = g_address->entry;
        address = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return address=%s", address);

    return address;
}

static const char *edit_static_get_netmask(void)
{
    GtkWidget *widget;
    const char *netmask = NULL;

    if (g_netmask)
    {
        widget = g_netmask->entry;
        netmask = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return netmask=%s", netmask);
    return netmask;
}

static const char *edit_static_get_gateway(void)
{
    GtkWidget *widget;
    const char *gateway = NULL;

    if (g_gateway)
    {
        widget = g_gateway->entry;
        gateway = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return gateway=%s", gateway);
    return gateway;
}

static const char *edit_static_get_dns(void)
{
    GtkWidget *widget;
    const char *dns = NULL;

    if (g_dns)
    {
        widget = g_dns->entry;
        dns = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return dns=%s", dns);
    return dns;
}

gboolean edit_static_check_network_settings(void)
{
    staticStatus_t status;
    gboolean valid = TRUE;

    CN_LOGPRINTF("entry");
    
    status = edit_static_detect_status();
    if (status != undefStatus_e)
    {
        valid = FALSE;
    }

    CN_LOGPRINTF("return valid=%d", valid);
    return valid;
}

// store the settings in order to restore it later
// when go back to the previous page(e.g overview)
static void edit_static_backup_network_settings(void)
{
    const char *address, *netmask, *gateway, *dns;

    CN_LOGPRINTF("entry");

    address = edit_static_get_address();
    netmask = edit_static_get_netmask();
    gateway = edit_static_get_gateway();
    dns = edit_static_get_dns();

    if (g_old_settings)
    {
        g_free(g_old_settings->address);
        g_free(g_old_settings->netmask);
        g_free(g_old_settings->gateway);
        g_free(g_old_settings->dns);
        g_free(g_old_settings);
    }

    g_old_settings = g_new0(regIpSetting_t, 1);
    g_assert(g_old_settings != NULL);
    g_old_settings->address = g_strdup(address);
    g_old_settings->netmask = g_strdup(netmask);
    g_old_settings->gateway = g_strdup(gateway);
    g_old_settings->dns = g_strdup(dns);
}

void edit_static_restore_network_settings(void)
{
    char *address = NULL;
    char *netmask = NULL;
    char *gateway = NULL;
    char *dns = NULL; 
    
    CN_LOGPRINTF("entry");
     
    if (g_old_settings)
    {
       address = g_old_settings->address;
       netmask = g_old_settings->netmask;
       gateway = g_old_settings->gateway;
       dns = g_old_settings->dns;
    }

    edit_static_set_address(address);
    edit_static_set_netmask(netmask);
    edit_static_set_gateway(gateway);
    edit_static_set_dns(dns);
}

void edit_static_screen_leave(void)
{
    gboolean valid = TRUE;

    CN_LOGPRINTF("entry");

    valid = edit_static_check_network_settings();

    // force overview screen to DHCP when no static settings
    if (!valid)
    {
        edit_overview_set_address(dhcp_t);
    }
}

// signal handlers
static gboolean on_delayed_next(gpointer data)
{
    gboolean valid;
    GtkToggleButton *button;

    erbusy_blink();

    valid = edit_static_check_network_settings();
    if (valid)
    {
        // before leave this page, backup the settings
        edit_static_backup_network_settings();

        edit_goto_page(editScreenOverview_e);
        edit_overview_update_info_text(inputProxy_e);
    }
    else
    {
        edit_static_determine_status();
    }

    button = GTK_TOGGLE_BUTTON(data);
    gtk_toggle_button_set_active(button, FALSE);

    return FALSE;
}

static void on_next(GtkToggleButton *button, gpointer data)
{
    gboolean active;

    CN_LOGPRINTF("entry");
 
    active = gtk_toggle_button_get_active(button);
    if (active)
    {
        g_timeout_add(500, on_delayed_next, button);
    }
}

// signal handlers for key input
gboolean on_edit_static_keypress(GdkEventKey *event)
{
    GtkToggleButton *button;
    gboolean ret = FALSE;

    CN_LOGPRINTF("entry");
   
    switch (event->keyval)
    {
        case GDK_Return:
            // 'Enter' key is pressed
            // simulate clicking the 'Next' button
            if (g_dns)
            {
                button = GTK_TOGGLE_BUTTON(g_dns->button);
                gtk_toggle_button_set_active(button, TRUE);
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
            // avoid the standard GTK behaviour for GtkEntries
            erbusy_off();
            ret = TRUE;

        default:
            erbusy_off();
            break;
    }

    return ret;
}

