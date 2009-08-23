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
 * \file editScreenProxy.h
 * \brief connectionMgr - "Proxy Server" screen
 *
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
#include "editScreenProxy.h"
#include "languages.h"

typedef enum
{
    inputHost_e = 0,
    inputPort_e,
    undefStatus_e 
}proxyStatus_t;

// screen objects
static GtkWidget *g_proxy = NULL;

static wdtLabelEntry *g_host = NULL;
static wdtLabelEntry *g_port = NULL;

static GtkWidget *g_info = NULL;

// Create a backup of the settings, need it when user "Cancel"s his changes.
static regProxySetting_t *g_old_settings = NULL;

static void edit_proxy_set_host(const char *proxyhost);
static void edit_proxy_set_port(const char *proxyport);

static const char *edit_proxy_get_host(void);
static const char *edit_proxy_get_port(void);

static void edit_proxy_backup_network_settings(void);

static void on_next(GtkToggleButton *button, gpointer data);

// object hierarchy:
// |--item (gtkSettingItem)
//    |--topVbox;
//       |--background
//          |--alignment
//             |--vbox
//                |--host (wdtLabelEntry)
//                |--port (wdtLabelEntry)
//    |--info(gtkInfoItem)
//       
GtkWidget *edit_proxy_create(void)
{
    CN_LOGPRINTF("entry");

    GtkWidget *item = NULL;
    GtkWidget *topVbox = NULL;
    GtkWidget *background = NULL;
    GtkWidget *alignment = NULL;
    GtkWidget *vbox = NULL;
    wdtLabelEntry *host = NULL;
    wdtLabelEntry *port = NULL;
    GtkWidget *info = NULL;
    GtkWidget *widget = NULL;

    CN_LOGPRINTF("entry");

    // item
    item = gtk_settingitem_new("");

    // topVbox
    topVbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_settingitem_add_details(GTK_SETTINGITEM(item), topVbox);
 
    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "bk_grey_666_140");
    gtk_widget_set_size_request(background, 
            BK_666_140_WIDTH, BK_666_140_HEIGHT);
    gtk_box_pack_start(GTK_BOX(topVbox), background, FALSE, FALSE, 0);

    // alignment
    alignment = gtk_alignment_new(0.0, 0.0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
            PADDING_TOP, PADDING_BOTTOM, PADDING_LEFT, PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);
 
    // vbox
    vbox = gtk_vbox_new(FALSE, VBOX_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    // host
    host = wdt_label_entry_new(string_e);
    gtk_box_pack_start(GTK_BOX(vbox), host->parent, FALSE, FALSE, 0);
    
    // port
    port = wdt_label_entry_new(integer_e);
    gtk_box_pack_start(GTK_BOX(vbox), port->parent, FALSE, FALSE, 0);

    // info item 
    info = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(topVbox), info, FALSE, FALSE, 0);

    // signal handlers
    widget = port->button;
    g_signal_connect_after(G_OBJECT(widget), "toggled", 
            G_CALLBACK(on_next), NULL);
 
    // show the widgets
    gtk_widget_show(item);
    gtk_widget_show(topVbox);
    gtk_widget_show(background);
    gtk_widget_show(alignment);
    gtk_widget_show(vbox);
    gtk_widget_show(host->parent);
    gtk_widget_hide(host->button);
    gtk_widget_show(port->parent);
    gtk_widget_show(info);
 
    // global variables
    g_proxy = item;
    g_host = host;
    g_port = port;
    g_info = info;
  
    CN_LOGPRINTF("done");

    // return
    return item;
}

void edit_proxy_set_text(void)
{
    GtkWidget *widget = NULL;

    CN_LOGPRINTF("entry");
  
    if (g_proxy)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_proxy), 
                                        _("Network profile"));
    }

    if (g_host)
    {
        widget = g_host->label;
        gtk_label_set_text(GTK_LABEL(widget),
                _("Enter the name or address of the HTTP proxy server:"));

    }
    
    if (g_port)
    {
        widget = g_port->label;
        gtk_label_set_text(GTK_LABEL(widget),
                _("Enter the port number of the HTTP proxy server:"));
        widget = g_port->button;
        gtk_button_set_label(GTK_BUTTON(widget), _("Next"));
    }
    
    if (g_info)
    {
        gtk_infoitem_set_text( GTK_INFOITEM(g_info),
            _("Do not include the protocol (e.g. http://) "
              "and proxy number (e.g. 8080) "
              "in the name or address of the HTTP proxy server.\n"
              "Hint: If you do not know the proxy settings "
              "for this network, "
              "please ask your network administrator."));
    }
}

static proxyStatus_t edit_proxy_detect_status(void)
{
    proxyStatus_t status;

    CN_LOGPRINTF("entry");

    if (!ergtk_entry_check_field(ERGTK_ENTRY(g_host->entry)))
    {
        status = inputHost_e;
    }
    else if (!ergtk_entry_check_field(ERGTK_ENTRY(g_port->entry)))
    {
        status = inputPort_e;
    }
    else
    {
        status = undefStatus_e;
    }

    CN_LOGPRINTF("return %d", status);
    return status;
}

static void edit_proxy_determine_status(void)
{
    proxyStatus_t status;
    GtkWidget *widget = NULL;

    CN_LOGPRINTF("entry");

    status = edit_proxy_detect_status();
    switch (status)
    {
        case inputHost_e:
            widget = g_host->entry;
            break;
        case inputPort_e:
            widget = g_port->entry;
            break;
        case undefStatus_e:
            widget = g_host->entry;
            break;
        default:
            break;
    }

    if (widget)
    {
        gtk_widget_grab_focus(widget);
    }
}
// set/get functions
void edit_proxy_set_network_settings(const regNetworkProfile_t *settings)
{
    char *profilename = NULL;
    char *proxyhost = NULL;
    char *proxyport = NULL;
    regProxySetting_t *proxySettings;

    CN_LOGPRINTF("entry");

    g_return_if_fail(settings != NULL);
    // g_return_if_fail(settings->proxy == TRUE);

    profilename = settings->name;

    proxySettings = settings->proxySettings;
    if (proxySettings)
    {
        proxyhost = proxySettings->address;
        proxyport = proxySettings->port;
    }

    edit_proxy_set_title(profilename);
    edit_proxy_set_host(proxyhost);
    edit_proxy_set_port(proxyport);

    edit_proxy_determine_status();

    // a backup copy of settings
    edit_proxy_backup_network_settings();
}

void edit_proxy_set_title(const char *profilename)
{
    char *title = NULL;

    CN_LOGPRINTF("entry[%s]", profilename);

    if (g_proxy)
    {
        if (profilename)
        {
            title = g_strdup_printf(_("Network profile: %s/proxy"), 
                                    profilename);
        }
        else
        {
            title = g_strdup(_("Network profile: proxy"));
        }

        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_proxy), title);

        g_free(title);
    }
}

static void edit_proxy_set_host(const char *proxyhost)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry [%s]", proxyhost);

    if (g_host)
    {
        widget = g_host->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), proxyhost ? proxyhost : "");
    }
}

static void edit_proxy_set_port(const char *proxyport)
{
    GtkWidget *widget;

    CN_LOGPRINTF("entry [%s]", proxyport);
    if (g_port)
    {
        widget = g_port->entry;
        gtk_entry_set_text(GTK_ENTRY(widget), proxyport ? proxyport: "");
    }
}

void edit_proxy_get_network_settings(regNetworkProfile_t *settings)
{
    const gchar *proxyhost = NULL;
    const gchar *proxyport = NULL;
    regProxySetting_t *proxySettings = NULL;

    CN_LOGPRINTF("entry [%p]", settings);

    g_return_if_fail(settings != NULL);
    g_return_if_fail(settings->proxy == TRUE);

    // clear proxy settings
    proxySettings = settings->proxySettings;
    if (proxySettings)
    {
        g_free(proxySettings->address);
        g_free(proxySettings->port);
        g_free(proxySettings);
        proxySettings = NULL;
        settings->proxySettings = NULL;
    }

    // get current value from screen objects
    proxyhost = edit_proxy_get_host();
    proxyport = edit_proxy_get_port();
  
    // store new settings, if proxy selected
    if (settings->proxy == TRUE)
    {
        proxySettings = g_new0(regProxySetting_t, 1);
        if (proxySettings)
        {
            proxySettings->address = g_strdup(proxyhost);
            proxySettings->port    = g_strdup(proxyport);
        }
        settings->proxySettings = proxySettings;
    }
}

static const char *edit_proxy_get_host(void)
{
    GtkWidget *widget;
    const char *proxyhost = NULL;

    if (g_host)
    {
        widget = g_host->entry;
        proxyhost = gtk_entry_get_text(GTK_ENTRY(widget));
    }

    CN_LOGPRINTF("return proxyhost=%s", proxyhost);
    return proxyhost;
}

static const char *edit_proxy_get_port(void)
{
    GtkWidget *widget;
    const char *proxyport = NULL;

    if (g_port)
    {
        widget = g_port->entry;
        proxyport = gtk_entry_get_text(GTK_ENTRY(widget));
    }       
    
    CN_LOGPRINTF("return proxyport=%s", proxyport);
    return proxyport;
}


gboolean edit_proxy_check_network_settings(void)
{
    proxyStatus_t status;
    gboolean valid = TRUE;

    CN_LOGPRINTF("entry");
    
    status = edit_proxy_detect_status();
    if (status != undefStatus_e)
    {
        valid = FALSE;
    }

    CN_LOGPRINTF("return valid=%d", valid);
    return valid;
}

static void edit_proxy_backup_network_settings(void)
{
    const char *proxyhost, *proxyport;

    CN_LOGPRINTF("entry");

    proxyhost = edit_proxy_get_host();
    proxyport = edit_proxy_get_port();

    if (g_old_settings)
    {
        g_free(g_old_settings->address);
        g_free(g_old_settings->port);
        g_free(g_old_settings);
    }

    g_old_settings = g_new0(regProxySetting_t, 1);
    g_assert(g_old_settings != NULL);
    g_old_settings->address = g_strdup(proxyhost);
    g_old_settings->port = g_strdup(proxyport);
}

void edit_proxy_restore_network_settings(void)
{
    char *host = NULL;
    char *port = NULL; 
    
    CN_LOGPRINTF("entry");
     
    if (g_old_settings)
    {
       host = g_old_settings->address;
       port = g_old_settings->port;
    }
    
    edit_proxy_set_host(host);
    edit_proxy_set_port(port);
}

void edit_proxy_screen_leave(void)
{
    gboolean valid = TRUE;

    CN_LOGPRINTF("entry");

    valid = edit_proxy_check_network_settings();
    // force overview screen to no-proxy when proxy settings are empty
    if (!valid)
    {
        edit_overview_set_proxy(FALSE);
    }
}

static gboolean on_delayed_next(gpointer data)
{
    gboolean valid;
    GtkToggleButton *button;

    CN_LOGPRINTF("entry");
    
    erbusy_blink();

    valid = edit_proxy_check_network_settings();
    if (valid)
    {
        edit_proxy_backup_network_settings();
        edit_goto_page(editScreenOverview_e);
    }
    else
    {
        edit_proxy_determine_status();
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

gboolean on_edit_proxy_keypress(GdkEventKey *event)
{
    GtkToggleButton *button;
    gboolean ret = FALSE;

    CN_LOGPRINTF("entry");

    switch (event->keyval)
    {
        case GDK_Return:
            // 'Enter' key is pressed
            // simulate clicking the 'Next' button
            if (g_port)
            {
                button = GTK_TOGGLE_BUTTON(g_port->button);
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

