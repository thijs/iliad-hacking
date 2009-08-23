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
 * \file pingThread.h
 * \brief Check whether IDS server can be reached
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef __PINGTHREAD__H__
#define __PINGTHREAD__H__

#include <gtk/gtk.h>
#include <glib.h>
#include <liberregxml/erregapi.h> 

typedef enum
        {
            PING_IDLE = 0,   // ping-thread not running
            PING_RUNNING,    // ping-thread started and connecting to server
            PING_ABORTING,   // ping-thread running but not wanted anymore
            PING_UNDEFINED
        } ping_status_t;

typedef struct
        {
            const regNetworkProfile_t* settings; // network settings
            const regPCProfile_t* pcshare; // pcshare settings (optional)
            GtkWidget* pingStatus;  // label widget to store ping status in
            int err; // ping result: ecd_err_t or -CURL_error_code
        } ping_thread_parms;

#ifdef __cplusplus
extern  "C"
{
#endif /* __cplusplus */

    gboolean pingThread_start(ping_thread_parms* ping_parms);
    gboolean pingThread_stop(void);
    gboolean pingThread_wait(int timeout_sec);

    gboolean pingThread_stopped(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__PINGTHREAD_H__

