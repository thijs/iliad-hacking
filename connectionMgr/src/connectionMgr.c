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
 * \file connectionMgr.c
 * \brief connectionMgr - Main routines to create an environment
 *                       for the Network-connection application 
 *                       and to handle key strokes
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#include <glib/gutils.h>
#include <gtk/gtk.h>

#include <libergtk/ergtk.h>
#include <liberipc/eripcconnect.h>
#include <liberipc/eripctoolbar.h>
#include <liberregxml/erregapi.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include <liberdm/connectionMgrConstants.h>
#include "erbusy.h"
#include "pagebar.h"
#include "toolbar.h"
#include "connectScreenData.h"
#include "connectionMgr.h"
#include "connectScreen.h"
#include "connectScreenOptions.h"
#include "editScreen.h"
#include "background.h"
#include "modemUtils.h"

// Screens to be displayed or hidden
static GtkWidget*   g_connectScreens = NULL;
static GtkWidget*   g_editScreens = NULL;
static cmgrScreen_t g_screen_mode = cmgrScreenUndef_e;

// pid of network enable script
static scanContext_t *g_scan_ctxt = NULL;
static pingContext_t *g_ping_ctxt = NULL;

static volatile int enable_pid = 0;

// pending actions on toolbar_synchronise event
static on_tbsync_action_t on_toolbar_sync_actions[] = { NULL, NULL };

static guint g_show_idle_handler_id = 0;
static guint g_hide_idle_handler_id = 0;

// Local functions
static void on_toolbar_synchronised(void);

static void     on_goto_page(gint new_page);
static gboolean hook_screen_refresh_typing(GSignalInvocationHint *ihint, 
                                           guint n_param_values, 
                                           const GValue *param_values, 
                                           gpointer user_data);

static gboolean on_idle_show_keyboard(gpointer data);
static gboolean on_idle_hide_keyboard(gpointer data);

// IPC
#define SERVER_BUFFER_SIZE  1024
static erServerChannel_t theServerChannel;
static erClientChannel_t g_ipc_channel_setup;
static void connectMessageReceived(gpointer data, 
                                   gint source_fd, 
                                   GdkInputCondition condition);

///////////////////////////////////////////////////////////////////////////
// create widgets and event handlers
///////////////////////////////////////////////////////////////////////////

// Create the connection-manager screens 
// keep screens hidden until cmgr_set_screen_mode() is called
//
// object hierarchy:
//     screens (vbox)
//       |-- g_connect_screens
//       |-- g_edit_screens
//
GtkWidget *cmgr_screens_create(void)
{
    GtkWidget* screens;
    guint      signal_id;

    CN_LOGPRINTF("entry");

    // screens
    screens = gtk_vbox_new(FALSE, 0);

    // g_connectScreens
    g_connectScreens = connect_screen_create();
    connect_screen_set_text();
    gtk_box_pack_start(GTK_BOX(screens), g_connectScreens, 
                       FALSE, FALSE, 0);
    gtk_widget_hide(g_connectScreens);

    // g_editScreens
    g_editScreens = edit_screen_create();
    edit_screen_set_text();
    gtk_box_pack_start(GTK_BOX(screens), g_editScreens, FALSE, FALSE, 0);
    gtk_widget_hide(g_editScreens);

    // install class-specific screen update handlers
    //   erGtkEntry
    signal_id = g_signal_lookup("screen-refresh", ERGTK_ENTRY_TYPE);
    g_assert(signal_id > 0);
    g_signal_add_emission_hook(signal_id, 
                               0, 
                               hook_screen_refresh_typing, 
                               (gpointer)TEXT_ENTRY_CHANGED_LEVEL, 
                               NULL);
    //   erGtkToggleButton
    signal_id = g_signal_lookup("toggled", ERGTK_TOGGLE_BUTTON_TYPE);
    g_assert(signal_id > 0);
    g_signal_add_emission_hook(signal_id, 
                                0, 
                                hook_screen_refresh_typing, 
                                (gpointer)SETTING_ITEM_CHANGE, 
                                NULL);

    gtk_widget_show(screens);
    return screens;
}

void cmgr_set_screen_mode(cmgrScreen_t mode)
{
    CN_LOGPRINTF("entry: mode [%d] g_screen_mode [%d]", 
                  mode, g_screen_mode);

    if (mode != g_screen_mode)
    {
        switch (mode)
        {
            case cmgrScreenConnect_e:
                g_screen_mode = cmgrScreenConnect_e;
                
                bg_display_title();

                gtk_widget_hide(g_editScreens);
                gtk_widget_show(g_connectScreens);

                connect_set_pagebar();
                connect_set_toolbar();
                
                connect_screen_display();
                break;

            case cmgrScreenEdit_e:
                g_screen_mode = cmgrScreenEdit_e;

                bg_display_title();

                gtk_widget_hide(g_connectScreens);
                gtk_widget_show(g_editScreens);

                edit_set_toolbar();
                edit_set_pagebar();
                
                edit_screen_display();
                break;

            default:
                CN_LOGPRINTF("Illegal screen mode [%d]", mode);
        }
    }
}


cmgrScreen_t cmgr_get_screen_mode(void)
{
    CN_LOGPRINTF("entry: g_screen_mode [%d]", g_screen_mode);
    return g_screen_mode;
}

void cmgr_set_toolbar(void)
{
    CN_LOGPRINTF("entry: g_screen_mode [%d]", g_screen_mode);

    switch (g_screen_mode)
    {
        case cmgrScreenConnect_e:
            connect_set_toolbar();
            break;

        case cmgrScreenEdit_e:
            edit_set_toolbar();
            break;

        default:
            CN_LOGPRINTF("Illegal screen g_screen_mode [%d]", 
                          g_screen_mode);
    }

}

void cmgr_set_pagebar(void)
{
    CN_LOGPRINTF("entry: g_screen_mode [%d]", g_screen_mode);
    
    switch (g_screen_mode)
    {
        case cmgrScreenConnect_e:
            connect_set_pagebar();
            break;

        case cmgrScreenEdit_e:
            edit_set_pagebar();
            break;

        default:
            CN_LOGPRINTF("Illegal screen g_screen_mode [%d]", 
                          g_screen_mode);
    }
}

// Key-event received
guint on_cmgr_keypress(GtkWidget *widget, 
                                 GdkEventKey *event, 
                                 gpointer data)
{
    guint   returnValue = 0;    // return FALSE => default gtk handling

    CN_LOGPRINTF("entry");

    switch (cmgr_get_screen_mode())
    {
        case cmgrScreenConnect_e:
            returnValue = on_connect_keypress(event);
            break;
        case cmgrScreenEdit_e:
            returnValue = on_edit_keypress(event);
            break;
        default:
            /* ignore */ ;
    }
    
    return returnValue;
}

static void on_cmgr_icon_clicked(int iconID, int iconState)
{
    cmgrScreen_t mode;

    CN_LOGPRINTF("entry iconID[%d] iconState[%d]", iconID, iconState);

    mode = cmgr_get_screen_mode();
    switch (mode)
    {
        case cmgrScreenEdit_e:
            on_edit_icon_clicked(iconID, iconState); 
            break;
        case cmgrScreenConnect_e:
            on_connect_icon_clicked(iconID, iconState);
            break;
        default:
            break;
    }
}

static void on_cmgr_toolbar_synchronised(void)
{
    CN_LOGPRINTF("entry");

    on_toolbar_synchronised();
}

void cmgr_add_on_toolbar_sync_action(on_tbsync_action_t action)
{
    int elementSize, arraySize;
    int i, sz;

    CN_LOGPRINTF("entry");
    
    g_assert(action != NULL);

    elementSize = sizeof(on_toolbar_sync_actions[0]);
    arraySize = sizeof(on_toolbar_sync_actions);
    sz = arraySize / elementSize;
    
    for (i = 0; i < sz; i++)
    {
        if (on_toolbar_sync_actions[i] == NULL)
        {
            on_toolbar_sync_actions[i] = action;
            return;
        }
    }

    g_assert_not_reached();
}

static void on_toolbar_synchronised(void)
{
    int elementSize, arraySize;
    int i, sz;

    CN_LOGPRINTF("entry");

    display_update_request_screen_refresh(TEXT_ENTRY_CHANGED_LEVEL, 
                                          WAVEFORM_TYPING);

    elementSize = sizeof(on_toolbar_sync_actions[0]);
    arraySize = sizeof(on_toolbar_sync_actions);
    sz = arraySize / elementSize;

    // call functions for registered on_toolbar_sync_actions
    for (i = 0; i < sz; i++)
    {
        if (on_toolbar_sync_actions[i] != NULL)
        {
            on_toolbar_sync_actions[i] ();
            on_toolbar_sync_actions[i] = NULL;
        }
    }
}

static void on_goto_page(gint new_page)
{
    CN_LOGPRINTF("entry");

    switch (cmgr_get_screen_mode())
    {
        case cmgrScreenConnect_e:
            on_connect_goto_page(new_page);
            break;
        case cmgrScreenEdit_e:
            on_edit_goto_page(new_page);
            break;
        default:
            /* ignore */ ;
    }
}

static gboolean hook_screen_refresh_typing(GSignalInvocationHint *ihint, 
                                           guint n_param_values, 
                                           const GValue *param_values, 
                                           gpointer user_data)
{
    int level = (int)user_data;

    CN_LOGPRINTF("entry: level [%d]", level);

    display_update_request_screen_refresh(level, WAVEFORM_TYPING);

    return TRUE;  // TRUE = continue gtk signal handling
}

///////////////////////////////////////////////////////////////////////////
// keyboard handling
///////////////////////////////////////////////////////////////////////////
static void keyboard_remove_idle_handler_ids(void)
{
    if (g_show_idle_handler_id)
    {
        CN_DMPRINTF("\nRemoving the old show keyboard idle handler.\n");
       
        g_source_remove(g_show_idle_handler_id);
        g_show_idle_handler_id = 0;
    }

    if (g_hide_idle_handler_id)
    {
        CN_DMPRINTF("\nRemoving the old hide keyboard idle handler.\n");

        g_source_remove(g_hide_idle_handler_id);
        g_hide_idle_handler_id = 0;
    }

    display_update_remove_idle_handler_id();
}

static gboolean keyboard_is_running(void)
{
    const char *cmd = "ps -ef | grep matchbox-keyboard | grep -v grep";
    gboolean running = FALSE;
    
    if (system(cmd) == 0)
    {
        running = TRUE;
    }

    CN_LOGPRINTF("return %d", running);
    return running;
}

// show keyboard:
//   wait for GTK objects updated then show keyboard
//   block display updates while updating GTK objects, rely on keyboard
//   display update
static void do_show_keyboard()
{

    // remove the old display update, 
    // keyboard will refresh the screen later
    keyboard_remove_idle_handler_ids();

    // block screen updates until keyboard shows
    display_update_increase_level(KEYBOARD_SHOW_HIDE_LEVEL);
    
    // update GTK objects then show keyboard,
    // rely on display update by keyboard
    g_show_idle_handler_id = g_idle_add(on_idle_show_keyboard, NULL);
}

static gboolean on_idle_show_keyboard(gpointer data)
{
    gboolean running = keyboard_is_running();

    toolbar_setIconState(iconID_keyboard, iconState_selected);

    if (running)
    {
        // from show to show, keyboard doesn't refresh the screen,
        // do it on connectionMgr side
        CN_DMPRINTF("\nfrom show to show, " 
                    "rely on cmgr refresh the screen\n");
        display_update_request_screen_refresh(KEYBOARD_SHOW_HIDE_LEVEL, 
                                              WAVEFORM_FULLSCREEN);
    }
    else
    {
        CN_DMPRINTF("\nfrom hide to show, "
                    "rely on keyboard refresh the screen\n");
        // allow display updates again
        display_update_decrease_level(LOWEST_LEVEL);
    }

    g_show_idle_handler_id = 0;
    return FALSE;               // don't call me again
}

// hide keyboard:
//   wait for GTK objects updated then show keyboard
//   block display updates while updating GTK objects, rely on keyboard
//   display update
static void do_hide_keyboard(int iconState)
{
    g_assert((iconState == iconState_grey) 
            || (iconState == iconState_normal));

    // remove the old display update
    // keyboard will refresh the screen later
    keyboard_remove_idle_handler_ids();

    // block screen updates until keyboard hides
    display_update_increase_level(KEYBOARD_SHOW_HIDE_LEVEL);

    // update GTK objects then hide keyboard, 
    // rely on display update by keyboard
    g_hide_idle_handler_id = g_idle_add(on_idle_hide_keyboard, 
                                        (gpointer) iconState);
}

static gboolean on_idle_hide_keyboard(gpointer data)
{
    gboolean running = keyboard_is_running();
    int iconState = (int)data;

    toolbar_setIconState(iconID_keyboard, iconState);

    if (!running)
    {
        // from hide to hide, keyboard doesn't do refresh the screen,
        // do it on connectionMgr side
        CN_DMPRINTF("\nfrom hide to hide, " 
                      "rely on cmgr refresh the screen\n");
        display_update_request_screen_refresh(KEYBOARD_SHOW_HIDE_LEVEL, 
                                         WAVEFORM_FULLSCREEN);
    }
    else
    {
        CN_DMPRINTF("\nfrom show to hide, "
                    "rely on keyboard refresh the screen\n");
        // allow display updates again
        display_update_decrease_level(LOWEST_LEVEL);
    }

    g_hide_idle_handler_id = 0;
    return FALSE;               // don't call me again
}

// show/hide the on-screen keyboard
void show_keyboard(gboolean show)
{
    CN_LOGPRINTF("entry: show [%d]", show);

    if (show)
    {
        // keyboard requested
        do_show_keyboard();
    }
    else
    {
        // keyboard not requested
        do_hide_keyboard(iconState_grey);
    }
}

///////////////////////////////////////////////////////////////////////////
// functions about IPC
///////////////////////////////////////////////////////////////////////////
gboolean cmgrInstallIpcServer()
{
    int     ret;
    int     fd = -1;
    int     returnValue;

    CN_IPCPRINTF("entry");

    ret = erIpcOpenServerChannel(ER_CONNECT_CHANNEL, &theServerChannel);

    if (ret != (-1))
    {
        fd = erIpcGetServerFd(theServerChannel);

        CN_IPCPRINTF("erIpcGetServerFd returned %d\n", fd);

        // gtk specific mechanism to monitor input file descriptor.
        returnValue = gdk_input_add(fd, 
                                    GDK_INPUT_READ, 
                                    connectMessageReceived, 
                                    (gpointer) theServerChannel);

        CN_IPCPRINTF("gdk_input_add returned %d", returnValue);
        return TRUE;
    }
    else
    {
        CN_ERRORPRINTF("Could not open server channel");
        return FALSE;
    }
}

gboolean connectStartIpcClientSetup()
{
    int ret;

    CN_LOGPRINTF("entry\n");

    ret = erIpcStartClient(ER_SETUP_CHANNEL, &g_ipc_channel_setup);
    CN_IPCPRINTF("erIpcStartClient returned %d\n", ret);

    return (ret == 0) ? TRUE : FALSE;
}

// 
// This callback is invoked when there is data to read 
// on file descriptor source_fd.
// The channel info is passed via parameter 'data'
//
static void connectMessageReceived(gpointer data, 
                                   gint source_fd, 
                                   GdkInputCondition condition)
{
    char    szBuffer[SERVER_BUFFER_SIZE];
    int     nBuf = SERVER_BUFFER_SIZE;
    int     page;
    
    erIpcCmd_t command;
    //st_ContentType_e contentType;

    erIpcGetMessage((erServerChannel_t) data, szBuffer, &nBuf);

    CN_IPCPRINTF("Received %s\n", szBuffer);

    if (cnParseCommand(szBuffer, &command) >= 0)
    {
        switch (command.cc)
        {
            case ccCnGotoPage:
                page = atoi(command.arg[0]);
                CN_IPCPRINTF("Connect Goto Page: %d\n", page);
                gdk_threads_enter();
                on_goto_page(page);
                gdk_threads_leave();
                break;
           
            case ccCnToolbar:
                {
                    int iconID    = atoi(command.arg[0]);
                    int iconState = atoi(command.arg[1]);
                    CN_LOGPRINTF("ccCnToolbarIcon: iconID [%d] iconState [%d]", 
                                  iconID, iconState);
                    gdk_threads_enter();
                    on_cmgr_icon_clicked(iconID, iconState);
                    gdk_threads_leave();
                }
                break;

            case ccCnToolbarSync:
                    gdk_threads_enter();
                    CN_LOGPRINTF("ccCnToolbarSync");
                    on_cmgr_toolbar_synchronised();
                    gdk_threads_leave();
                break;

            default:
                CN_IPCPRINTF("UNKNOWN COMMAND %d", command.cc);
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
// functions about network
///////////////////////////////////////////////////////////////////////////
void cmgr_scan_ctxt_init(void)
{
    CN_LOGPRINTF("entry");

    cmgr_scan_ctxt_destory();
    g_scan_ctxt = connect_scan_ctxt_new();
}

void cmgr_scan_ctxt_destory(void)
{
    CN_LOGPRINTF("entry");

    connect_scan_ctxt_destory(g_scan_ctxt);
    g_scan_ctxt = NULL;
}

scanContext_t *cmgr_get_scan_ctxt(void)
{
    CN_LOGPRINTF("return %p", g_scan_ctxt);
    return g_scan_ctxt;
}

void cmgr_ping_ctxt_init(void)
{
    CN_LOGPRINTF("entry");

    cmgr_ping_ctxt_destory();
    g_ping_ctxt = connect_ping_ctxt_new();
}

void cmgr_ping_ctxt_destory(void)
{
    CN_LOGPRINTF("entry");

    connect_ping_ctxt_destory(g_ping_ctxt);
    g_ping_ctxt = NULL;
}

pingContext_t *cmgr_get_ping_ctxt(void)
{
    CN_LOGPRINTF("return %p", g_ping_ctxt);
    return g_ping_ctxt;
}

void connect_disable_network()
{
    // disable all interfaces
    system(COMMAND_IFDOWN_ALL);
}

void connect_enable_network_abort()
{
    // stop network enable, when needed
    if (enable_pid > 0)
    {
        CN_WARNPRINTF("Sending SIGTERM to enable_pid [%d]", enable_pid);
        kill(enable_pid, SIGTERM);
    }
}

static gboolean is_hex_key(const char *key)
{
    int i, len;
    gboolean ret = TRUE;

    if ((key == NULL) 
        || ((len = strlen(key)) == 0))
    {
        return TRUE;
    }

    for (i = 0; i < len; i++)
    {
        if (!isxdigit(key[i]))
        {
            ret = FALSE;
            break;
        }
    }

    CN_LOGPRINTF("return %d", ret);
    return ret;
}

static void add_key_prefix(const char *pkey, char **ppkey)
{
    if (is_hex_key(pkey))
    {
       *ppkey = g_strdup(pkey);
    }
    else
    {
        *ppkey = g_strdup_printf("s:%s", pkey);
    }

    CN_LOGPRINTF("output[%s]", *ppkey);
}

ecd_err_t connect_enable_network(const regNetworkProfile_t *regNetworkProfile)
{
    ecd_err_t err = ECD_FAILURE;
    char *command[128];
    int arg = 0;
    char *freelist[128];
    int free_arg = 0;
    int rc;
    struct in_addr address;
    struct in_addr netmask;
    struct in_addr broadcast;
    char *encrkey;

    CN_LOGPRINTF("entry");
    g_return_val_if_fail((regNetworkProfile != NULL), err);

    if (regNetworkProfile->connection != dialup_t)
    {
        FILE* fd_iface  = NULL;
        FILE* fd_resolv = NULL;
        FILE* fd_proxy  = NULL;

        // Create temporary files
        fd_iface  = fopen(INTERFACES_FILE_TMP,   "w");
        fd_resolv = fopen(RESOLVE_CONF_FILE_TMP, "w");
        fd_proxy  = fopen(HTTP_PROXY_FILE_TMP,   "w");
        g_return_val_if_fail((   fd_iface  != NULL
                              && fd_resolv != NULL
                              && fd_proxy  != NULL), err);

        // interface name
        fputs("iface ", fd_iface);
    
        switch (regNetworkProfile->connection)
        {
            case wired_t:
                fputs(INTERFACE_WIRED, fd_iface);
                break;
            case wireless_t:
                fputs(INTERFACE_WIRELESS, fd_iface);
                break;
            default:
                CN_ERRORPRINTF("Illegal setting->connection [%d]", 
                               (int)regNetworkProfile->connection);
                return err;
        }

        fputs(" inet ", fd_iface);  //type TCP/IP

        // static or dynamic IP address
        switch (regNetworkProfile->addressMode)
        {
            case dhcp_t:
                fputs("dhcp ", fd_iface);
                break;

            case static_t:
                // static IP address etc.
                fputs(  "static ", fd_iface);
                fputs("\n  address ", fd_iface); 
                fputs(regNetworkProfile->ipSettings->address, fd_iface);
                fputs("\n  netmask ", fd_iface); 
                fputs(regNetworkProfile->ipSettings->netmask, fd_iface);
                fputs("\n  gateway ", fd_iface); 
                fputs(regNetworkProfile->ipSettings->gateway, fd_iface);

                // static DNS server address
                fputs("nameserver ", fd_resolv);
                fputs(regNetworkProfile->ipSettings->dns, fd_resolv);
                break;

            default:
                CN_ERRORPRINTF("Illegal setting->addresMode [%d]", 
                                (int)regNetworkProfile->addressMode);
                return err;
        }

        // Put proxy setting in environment
        if (regNetworkProfile->proxySettings == NULL)
        {
            CN_WARNPRINTF("No proxy needs to be specified");
            g_unsetenv("http_proxy");
            g_unsetenv("https_proxy");
        }
        else
        {
            char proxyString[256];

            snprintf(proxyString, sizeof(proxyString), 
                    "http://%s:%s", 
                    regNetworkProfile->proxySettings->address, 
                    regNetworkProfile->proxySettings->port);
            fputs(proxyString, fd_proxy);
            CN_WARNPRINTF("Setting proxy to %s", proxyString);

            g_setenv("http_proxy", proxyString, 1);
            g_setenv("https_proxy", proxyString, 1);
        }

        // close temporary files
        fputc('\n', fd_iface);   fclose(fd_iface);
        fputc('\n', fd_resolv);  fclose(fd_resolv);
        fputc('\n', fd_proxy);   fclose(fd_proxy);
    }

    // command-line to start network interface
    switch (regNetworkProfile->connection)
    {
        case wired_t:
            err = ECD_WIRED_ERROR;
            arg = 0;
            command[arg++] = COMMAND_WIRED;
            command[arg++] = "start";
            if (regNetworkProfile->addressMode == static_t)
            {
                //inet_aton(regNetworkProfile->ipSettings->address, &address);
                //inet_aton(regNetworkProfile->ipSettings->netmask, &netmask);
                address.s_addr = inet_addr(regNetworkProfile->ipSettings->address);
                netmask.s_addr = inet_addr(regNetworkProfile->ipSettings->netmask);
                broadcast.s_addr = address.s_addr | ~netmask.s_addr;

                command[arg++] = "static";
                command[arg++] = regNetworkProfile->ipSettings->address;
                command[arg++] = regNetworkProfile->ipSettings->netmask;
                command[arg++] = freelist[free_arg++] = strdup(inet_ntoa(broadcast));
                command[arg++] = regNetworkProfile->ipSettings->gateway;
                command[arg++] = regNetworkProfile->ipSettings->dns;
            }
            else
            {
                command[arg++] = "dhcp";
            }
            break;

        case wireless_t:
            err = ECD_WIRELESS_ERROR;
            arg = 0;
            command[arg++] = COMMAND_WIRELESS;
            command[arg++] = "start";
            command[arg++] = regNetworkProfile->wirelessSettings->SSID;

            switch (regNetworkProfile->wirelessSettings->encrType)
            {
                case encr_none_t:
                    /* ignore */ ;
                    break;
                case encr_wep_t:
                    command[arg++] = "enc";
                    encrkey = regNetworkProfile->wirelessSettings->encrKey;
                    add_key_prefix(encrkey, &freelist[free_arg]);
                    command[arg++] = freelist[free_arg];
                    free_arg++;
                   break;
                case encr_wpa_t:
                    command[arg++] = "wpa";
                    encrkey = regNetworkProfile->wirelessSettings->encrKey;
                    command[arg++] = encrkey;
                    break;
                default:
                    // TODO: support these settings
                    CN_ERRORPRINTF("Support the ascii settings");
                    break;
            }
            break; // end wireless_t

        case dialup_t:
            err = ECD_DIALUP_ERROR;
            arg = 0;
            command[arg++] = COMMAND_DIALUP;
            command[arg++] = "start";
            command[arg++] = regNetworkProfile->dialupSettings->phone;
            command[arg++] = regNetworkProfile->dialupSettings->user;
            command[arg++] = regNetworkProfile->dialupSettings->password;
            break; // end dialup_t

        default:
            /* ignore, has been tested before */ ;
    }
  
    // now execute the command, at last
    command[arg] = NULL;
    if ((enable_pid = fork()) == 0)
    {
        printf("Execvp returned %d\n", execvp(command[0], command));
        printf("execvp error: %d %s", errno, strerror(errno));
        exit(1);
    }
    waitpid(enable_pid, &rc, 0);
    if (WIFEXITED(rc) && WEXITSTATUS(rc) == 0)
    {
        err = ECD_NO_ERROR;
    }
    enable_pid = 0;

    for (free_arg--; free_arg >= 0; free(freelist[free_arg--]));

    FILE *fd = fopen("/tmp/profile_name.tmp", "w");
    fputs(regNetworkProfile->name, fd);
    fputs("\n", fd);
    fclose(fd);
    
    CN_LOGPRINTF("done: err [%d]", err);
    return err;
}

///////////////////////////////////////////////////////////////////////////
// UP and BACK one page
///////////////////////////////////////////////////////////////////////////

// get one top level screen page
// return {cmgrScreenUndef_e, -1} means exit
static void cmgr_get_up_page(const screenPage_t *curPage, 
                             screenPage_t *upPage)
{
    CN_LOGPRINTF("entry [%p][%p]", curPage, upPage);

    g_return_if_fail(curPage != NULL);
    g_return_if_fail(upPage != NULL);

    if ((curPage->screen == cmgrScreenConnect_e)
         && (curPage->page == connectScreenOptions_e))
    {
        upPage->screen = cmgrScreenUndef_e;
        upPage->page = -1;
    }
    else if ((curPage->screen == cmgrScreenConnect_e)
            && (curPage->page == connectScreenProfiles_e))
    {
        upPage->screen = cmgrScreenUndef_e;
        upPage->page = -1;
    }
    else if ((curPage->screen == cmgrScreenEdit_e)
            && (curPage->page == editScreenProfiles_e))
    {
        if (main_get_edit_only())
        {    
            upPage->screen = cmgrScreenUndef_e;
            upPage->page = -1;
        }
        else
        {
            upPage->screen = cmgrScreenConnect_e;
            upPage->page = connectScreenOptions_e;
        }
    }
    else if ((curPage->screen == cmgrScreenEdit_e)
            && (curPage->screen == editScreenSearch_e))
    { 
        upPage->screen = cmgrScreenEdit_e;
        upPage->page = editScreenProfiles_e;
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenWireless_e))
    {
        upPage->screen = cmgrScreenEdit_e;
        upPage->page = editScreenProfiles_e;
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenOverview_e)) 
    {
        upPage->screen = cmgrScreenEdit_e;
        upPage->page = editScreenProfiles_e;
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenDialup_e))
    {
        upPage->screen = cmgrScreenEdit_e;
        upPage->page = editScreenProfiles_e;
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenProxy_e))
    {
        upPage->screen = cmgrScreenEdit_e;
        upPage->page = editScreenProfiles_e;
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenStatic_e))
    {
        upPage->screen = cmgrScreenEdit_e;
        upPage->page = editScreenProfiles_e;
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenWirelessExt_e))
    {
        upPage->screen = cmgrScreenEdit_e;
        upPage->page = editScreenProfiles_e;
    }
    else
    {
        upPage->screen = cmgrScreenUndef_e;
        upPage->page = -1;
    }
    CN_LOGPRINTF("upPage[%d,%d]", upPage->screen, upPage->page);
}

// get the previous screen page
// return {cmgrScreenUndef_e, -1} means disable back 
void cmgr_get_back_page(const screenPage_t *curPage, 
                        screenPage_t *backPage)
{
    CN_LOGPRINTF("entry [%p][%p]", curPage, backPage);
    
    g_return_if_fail(curPage != NULL);
    g_return_if_fail(backPage != NULL);

    if ((curPage->screen == cmgrScreenConnect_e)
       && (curPage->page == connectScreenOptions_e))
    {
         backPage->screen = cmgrScreenUndef_e;
         backPage->page = -1;
    }
    else if ((curPage->screen == cmgrScreenConnect_e) 
            && (curPage->page == connectScreenProfiles_e))
    {
        backPage->screen = cmgrScreenConnect_e;
        backPage->page = connectScreenOptions_e;
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenProfiles_e))
    { 
        backPage->screen = cmgrScreenUndef_e;
        backPage->page = -1; 
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenSearch_e))
    { 
        backPage->screen = cmgrScreenUndef_e;
        backPage->page = -1;
    }
    else if ((curPage->screen == cmgrScreenEdit_e)
            && (curPage->page == editScreenWireless_e))
    {
        if (edit_get_back_overview())
        {
            backPage->screen = cmgrScreenEdit_e;
            backPage->page = editScreenOverview_e;
        }
        else 
        {
            if (edit_get_wizard_mode())
            {
                backPage->screen = cmgrScreenEdit_e;
                backPage->page = editScreenSearch_e;
            }
            else
            {
                backPage->screen = cmgrScreenUndef_e;
                backPage->page = -1; 
            }
        }
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenOverview_e))
    {
        backPage->screen = cmgrScreenUndef_e;
        backPage->page = -1; 
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenDialup_e))
    {
        if (edit_get_back_overview())
        {
            backPage->screen = cmgrScreenEdit_e;
            backPage->page = editScreenOverview_e;
        }
        else
        {
            backPage->screen = cmgrScreenUndef_e;
            backPage->page = -1; 
        }
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenProxy_e))
    {
        backPage->screen = cmgrScreenEdit_e; 
        backPage->page = editScreenOverview_e;
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenStatic_e))
    {
        backPage->screen = cmgrScreenEdit_e;
        backPage->page = editScreenOverview_e;
    }
    else if ((curPage->screen == cmgrScreenEdit_e) 
            && (curPage->page == editScreenWirelessExt_e))
    {
        if (edit_get_back_overview())
        {
            backPage->screen = cmgrScreenEdit_e;
            backPage->page = editScreenOverview_e;
        }
        else
        {
            backPage->screen = cmgrScreenUndef_e; 
            backPage->page = -1; 
        }
    }
    else
    {
        backPage->screen = cmgrScreenUndef_e;
        backPage->page = -1;
    }
    CN_LOGPRINTF("backPage[%d,%d]", backPage->screen, backPage->page);
}

// get the current screen page
static void cmgr_get_cur_screen_page(screenPage_t *page)
{
    g_return_if_fail(page != NULL);

    page->screen = cmgr_get_screen_mode();
    switch (page->screen)
    {
        case cmgrScreenConnect_e:
            page->page = connect_get_current_page(NULL);
            break;
        case cmgrScreenEdit_e:
            page->page = edit_get_current_page(NULL);
            break;
        default:
            page->page = -1;
            break;
    }
}

// switch screen page
static void cmgr_goto_screen_page(const screenPage_t *page)
{
    scanContext_t* ctxt;

    CN_LOGPRINTF("entry [%p]", page);
    
    cmgr_set_screen_mode(page->screen); 
    switch (page->screen)
    {
        case cmgrScreenConnect_e:
            connect_goto_page(page->page);

            // for connectScreenOptions, scan automatically
            if (page->page == connectScreenOptions_e)
            {
                connect_options_display();

                connect_options_restore_scan_ctxt();
                ctxt = cmgr_get_scan_ctxt();
                connect_scan(ctxt, TRUE);
            }
            break;
        case cmgrScreenEdit_e:
            edit_goto_page(page->page);
            break;
        default:
            break;
    }
}

// goto one top level page
void cmgr_up_page(void)
{
    screenPage_t curPage, upPage;
    editMode_t mode;

    CN_LOGPRINTF("entry");
 
    cmgr_get_cur_screen_page(&curPage);
    cmgr_get_up_page(&curPage, &upPage);
    
    if ((upPage.screen == cmgrScreenUndef_e)
        || (upPage.page == -1))
    {
        main_quit();
    }
    else
    {
        cmgr_goto_screen_page(&upPage);

        // The behaviour of up page to editScreenProfiles_e 
        // will lead to the profile editor is not only 
        // for filling in encryption key for access point.
        // So here, need to change the edit mode 
        // to be editModeConnectNormal_e
        if (upPage.screen == cmgrScreenEdit_e
            && upPage.page == editScreenProfiles_e)
        {
            mode = edit_get_mode();
            if ((mode == editModeConnectFillIn_e) 
                || (mode == editModeConnectCreate_e))
            {
                edit_set_mode(editModeConnectNormal_e);
            }
        }
    }
}

// back to the previous page
static gboolean on_delayed_cmgr_back_page(gpointer data)
{
    screenPage_t curPage, backPage;

    CN_LOGPRINTF("entry");

    erbusy_blink();

    cmgr_get_cur_screen_page(&curPage);
    cmgr_get_back_page(&curPage, &backPage);

    // before back, restore the previous settings
    switch (curPage.screen)
    {
        case cmgrScreenEdit_e:
            edit_restore_network_settings(curPage.page);
            break;
        case cmgrScreenConnect_e:
            // nothing need to do
            break;
        default:
            break;
    }

    cmgr_goto_screen_page(&backPage);

    return FALSE;
}

void cmgr_back_page(void)
{
    // wait for a while to show back icon active state
    g_timeout_add(500, on_delayed_cmgr_back_page, NULL);
}

