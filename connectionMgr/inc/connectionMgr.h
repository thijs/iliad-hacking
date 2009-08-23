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

#ifndef _CONNECTION_MGR_H_
#define _CONNECTION_MGR_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>
#include <liberregxml/erregapi.h>
#ifdef ECDC_LIBRARY
#include "ecdc.h"
#else
#include "ecdc_err.h"
#endif
#include "commonData.h"
#include "connectScan.h"
#include "connectPing.h"
#include "connectScreen.h"
#include "editScreen.h"

// connection manager constants
#define COMMAND_WIRED           "wired.sh"
#define COMMAND_IFUP_WIRED      "wired.sh start"

#define COMMAND_WIRELESS        "wireless.sh"
#define COMMAND_IFUP_WIRELESS   "wireless.sh start"
#define COMMAND_IFDOWN_ALL      "cifs.sh umount ; wireless.sh stop ; wired.sh stop ; dialup.sh stop"
#define COMMAND_SCAN_WIRELESS   "wireless.sh scan"
#define COMMAND_STATUS_WIRELESS "wireless.sh status"
#define COMMAND_DIALUP          "dialup.sh"
#define COMMAND_STATUS_DIALUP   "dialup.sh status"

#define INTERFACE_WIRED         "eth0"
#define INTERFACE_WIRELESS      "wlan0"

#define INTERFACES_FILE_TMP     "/tmp/interfaces.tmp"
#define RESOLVE_CONF_FILE_TMP   "/tmp/resolv.conf.tmp"
#define HTTP_PROXY_FILE_TMP     "/tmp/http_proxy.tmp"

#define COMMAND_MOUNT_SAMBA     "cifs.sh"

// Note: the options passed to the program, are hardcoded
#define DOWNLOADMGR_EXECUTABLE   "/usr/bin/downloadMgr"
#define PCSHAREMGR_EXECUTABLE    "/usr/bin/pcshareMgr"

// Screen-set actually displayed
typedef enum
{
    cmgrScreenConnect_e = 0,
    cmgrScreenEdit_e,
    cmgrScreenUndef_e
} cmgrScreen_t;

typedef struct
{
    cmgrScreen_t screen;
    int          page;
}screenPage_t;

typedef void (*on_tbsync_action_t) (void);

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    // command-line options
    gboolean          main_get_background(void);
    cmgrConnectType_t main_get_connect_type(void);
    gboolean          main_get_edit_only(void);
    gboolean          main_get_use_last_connected(void);
    gboolean          main_get_connect_after_reboot(void);
    gboolean          main_get_wifi_disabled(void);
    gboolean          main_get_dialup_disabled(void);
    char*             main_get_screen_title(void);

    // screen management
    void         cmgr_set_screen_mode(cmgrScreen_t mode);
    cmgrScreen_t cmgr_get_screen_mode(void);

    GtkWidget* cmgr_screens_create(void);
    void       cmgr_set_pagebar();

    void cmgr_add_on_toolbar_sync_action(on_tbsync_action_t action);

    guint on_cmgr_keypress(GtkWidget *widget, 
                           GdkEventKey *event, 
                           gpointer data);

    void main_start_child_process(void);
    void main_quit(void);

    void show_keyboard(gboolean show);

    // UP/BACK one page
    void cmgr_get_back_page(const screenPage_t *curPage, 
                            screenPage_t *backPage);
    void cmgr_up_page(void);
    void cmgr_back_page(void);

    // IPC
    gboolean cmgrInstallIpcServer(void);
    gboolean cmgrStartIpcClientSetup(void);

    // registry
    void         prepare_registry_write( void);
    void         do_registry_write(void);

    // enable/disable network
    void cmgr_scan_ctxt_init(void);
    void cmgr_scan_ctxt_destory(void);
    scanContext_t *cmgr_get_scan_ctxt(void);
    void cmgr_ping_ctxt_init(void);
    void cmgr_ping_ctxt_destory(void);
    pingContext_t *cmgr_get_ping_ctxt(void);

    void connect_disable_network(void);
    ecd_err_t connect_enable_network(const regNetworkProfile_t *setting);
    void connect_enable_network_abort(void);

    void main_set_exit_value(int exitValue);
    gboolean on_cmgr_quit(gpointer data);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_CONNECTION_MGR_H_



