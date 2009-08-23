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
 * \file scanThread.h
 * \brief Scan for available wireless networks
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef __SCANTHREAD__H__
#define __SCANTHREAD__H__

#include <gtk/gtk.h>
#include <glib.h>
#include <liberregxml/erregapi.h> 

typedef enum
        {
            SCAN_IDLE = 0, // scan-thread not running
            SCAN_RUNNING,  // scan-thread started and connecting to server
            SCAN_ABORTING, // scan-thread running but not wanted anymore
            SCAN_UNDEFINED
        } scan_status_t;

typedef struct
        {
            gchar*               ssid;
            wireless_encr_type_t encryption;
            int                  quality;
        } network_spec_t;

typedef struct
        {
            gchar* ssid; // ssid to scan for, amongst others
            network_spec_t* networks; // networks found
            guint networks_num;    // input: number of entries in networks[]
            guint networks_found;  // output: number of entries 
                                   // filled in networks[]
            GtkWidget* scanStatus; // label widget to store scan status in
        } scan_thread_parms;

#ifdef __cplusplus
extern  "C"
{
#endif /* __cplusplus */

    gboolean scanThread_start(scan_thread_parms* scan_parms);
    gboolean scanThread_stop(void);
    gboolean scanThread_wait(int timeout_sec);
    gboolean scanThread_stopped(void);

    // datatypes functions
    void free_network_spec(network_spec_t* pNetwork);
    network_spec_t *dup_network_spec(network_spec_t* pNetwork);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__SCANTHREAD_H__
