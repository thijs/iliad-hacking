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
 * \file connectScan.h
 * \brief connectionMgr - for wireless, 
 *                            using scanThread to scan the networks 
 *                        for the others, 
 *                            get the networks from registry 
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _CONNECT_SCAN_H_
#define _CONNECT_SCAN_H_

#include <glib.h>
#include <liberregxml/erregapi.h>
#include "scanThread.h"

// what purpose of calling this scanning module
typedef enum
{
    backgroundScan_e = 0,
    connectScan_e,
    editScan_e,
    undefScanMode_e
}scanMode_t;

typedef enum
{
    scanRunning_e = 0,
    rescanNeed_e,       // no results returned, probably need to rescan
    scanOk_e,           // results returned
    scanAborting_e,
    undefScanStatus_e
}scanStatus_t;

typedef void scan_done_t(connection_t type, 
                         network_spec_t* pNetworks, 
                         int nNetworks);
typedef void scan_done_after_t(void);

typedef struct
{
    // for which purpose
    scanMode_t   mode;
    // which network type need to scan
    connection_t networkType;

    // store the result of scanning
    scan_thread_parms* scanParms;

    // callback functions
    scan_done_t*       scan_done;
    scan_done_after_t* scan_done_after; 
     
    // private data

    // the status of scanning
    // indicate whether entering the scanning networks status
    gboolean     scanNetworks;
    guint        scanTimeoutId;
    gboolean     scanAborting;
}scanContext_t;

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    scanContext_t* connect_scan_ctxt_new(void);

    void connect_scan_ctxt_set_mode(scanContext_t* ctxt, scanMode_t mode);
    void connect_scan_ctxt_set_network_type(scanContext_t* ctxt, 
                                            connection_t networkType);
    void connect_scan_ctxt_set_done_callbacks(scanContext_t* ctxt, 
                                      scan_done_t* scan_done, 
                                      scan_done_after_t* scan_done_after);
    void connect_scan_ctxt_set_ssidlist(scanContext_t* ctxt, 
                                        gchar* ssidList);
    void connect_scan_ctxt_set_max_networks(scanContext_t* ctxt, 
                                            int maxNetworks);
    
    void connect_scan_ctxt_destory(scanContext_t* ctxt);

    void connect_scan(scanContext_t* ctxt, gboolean delay);
    void connect_scan_abort(scanContext_t* ctxt);

    gboolean connect_scan_freeze_ui(scanContext_t* ctxt);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_CONNECT_SCAN_H_

