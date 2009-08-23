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
 * \file connectScan.c
 * \brief connectionMgr - wireless: using scanThread to scan the networks 
 *                        the others: get the networks from registry 
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#include <gtk/gtk.h>
#include <glib.h>
#include <string.h>

#include "connectionMgrLog.h"
#include "scanThread.h"
#include "connectScan.h"
#include "background.h"
#include "erbusy.h"
    
static void connect_scan_ctxt_destory_results(scanContext_t* ctxt);
static void connect_scan_ctxt_empty_results(scanContext_t* ctxt);

static gboolean connect_scan_start(gpointer data);
static void     connect_scan_done (scanContext_t* ctxt);

static void wireless_scan_start(scanContext_t* ctxt);


static void     on_scan_status_changed (GObject *object, 
                                        GParamSpec *arg, 
                                        gpointer data);

scanContext_t* connect_scan_ctxt_new(void)
{
    scanContext_t* ctxt;
    GtkWidget* scanStatus;

    ctxt = g_new0(scanContext_t, 1);
    g_assert(ctxt != NULL);

    // initilize the scanParams
    ctxt->scanParms = g_new0(scan_thread_parms, 1);
    g_assert(ctxt->scanParms != NULL);

    scanStatus = gtk_label_new("scan-idle");
    g_signal_connect(scanStatus, "notify", 
            G_CALLBACK(on_scan_status_changed), ctxt);
    ctxt->scanParms->scanStatus = scanStatus;

    // set the default values
    connect_scan_ctxt_set_mode(ctxt, undefScanMode_e);
    connect_scan_ctxt_set_network_type(ctxt, connection_undefined_t);
    connect_scan_ctxt_set_done_callbacks(ctxt, NULL, NULL);

    connect_scan_ctxt_set_ssidlist(ctxt, "");
    connect_scan_ctxt_set_max_networks(ctxt, MAX_PROFILES_PER_PAGE);

    // the other values are set to be NULL or zero implicitly by g_new0

    return ctxt;
}

void connect_scan_ctxt_set_mode(scanContext_t* ctxt, scanMode_t mode)
{
    if (ctxt && (mode >= 0) && (mode < undefScanMode_e))
    {
        ctxt->mode = mode;
    }
}

void connect_scan_ctxt_set_network_type(scanContext_t* ctxt, 
                                    connection_t networkType)
{
    if (ctxt && (networkType >= 0) && (networkType < connection_undefined_t))
    {
        ctxt->networkType = networkType;
    }
}

void connect_scan_ctxt_set_done_callbacks(scanContext_t* ctxt, 
                                      scan_done_t* scan_done, 
                                      scan_done_after_t* scan_done_after)
{
    if (ctxt)
    {
        ctxt->scan_done = scan_done;
        ctxt->scan_done_after = scan_done_after;
    }
}

void connect_scan_ctxt_set_ssidlist(scanContext_t* ctxt, gchar* ssidList)
{
    if (ctxt && ctxt->scanParms && ssidList)
    {
        if (ctxt->scanParms->ssid)
        {
            g_free(ctxt->scanParms->ssid);
        }
        ctxt->scanParms->ssid = g_strdup(ssidList);
    }
}

void connect_scan_ctxt_set_max_networks(scanContext_t* ctxt, 
                                        int maxNetworks)
{
    network_spec_t *networks, *pNetwork;
    int             i;

    if (ctxt && ctxt->scanParms && (maxNetworks > 0))
    {
        // destory the old memory for networks
        connect_scan_ctxt_destory_results(ctxt);

        // malloc the new memory for networks
        networks = g_new0(network_spec_t, maxNetworks);
        g_assert(networks != NULL);
        for (i = 0; i < maxNetworks; i++)
        {
            pNetwork = &networks[i];
            pNetwork->encryption = encr_none_t;
        }
        ctxt->scanParms->networks = networks;
        ctxt->scanParms->networks_num = maxNetworks;
    }
}

void connect_scan_ctxt_destory(scanContext_t* ctxt)
{
    if (ctxt)
    {
        if (ctxt->scanParms)
        {
            /*
            if (ctxt->scanParms->scanStatus)
            {
                gtk_widget_destroy(ctxt->scanParms->scanStatus);
            }
            */
           
            if (ctxt->scanParms->ssid)
            {
                g_free(ctxt->scanParms->ssid);
            }
        }

        connect_scan_ctxt_destory_results(ctxt);

        g_free(ctxt->scanParms);
        g_free(ctxt);
    }
}

static void connect_scan_ctxt_destory_results(scanContext_t* ctxt)
{
    network_spec_t* networks, *pNetwork;
    int i, n;

    if (ctxt && ctxt->scanParms)
    {
        if (ctxt->scanParms->networks)
        {
            networks = ctxt->scanParms->networks;
            n = ctxt->scanParms->networks_num;

            for (i = 0; i < n; i++)
            {
                pNetwork = &networks[i];
                if (pNetwork->ssid)
                {
                    g_free(pNetwork->ssid);
                }
            }
            g_free(ctxt->scanParms->networks);
            ctxt->scanParms->networks = NULL;
            ctxt->scanParms->networks_num = 0;
            ctxt->scanParms->networks_found = 0;
        }
    }
}

static void connect_scan_ctxt_empty_results(scanContext_t* ctxt)
{
    network_spec_t* networks, *pNetwork;
    int i, n;

    if (ctxt && ctxt->scanParms)
    {
        if (ctxt->scanParms->networks)
        {
            networks = ctxt->scanParms->networks;
            n = ctxt->scanParms->networks_num;

            for (i = 0; i < n; i++)
            {
                pNetwork = &networks[i];
                if (pNetwork->ssid)
                {
                    g_free(pNetwork->ssid);
                    pNetwork->ssid = NULL;
                }
                pNetwork->encryption = encr_none_t;
                pNetwork->quality = 0;
            }

            ctxt->scanParms->networks_found = 0;
        }
    }
}

void connect_scan(scanContext_t* ctxt, gboolean delay)
{
    scanMode_t   mode;
    connection_t type;

    CN_LOGPRINTF("entry");

    g_return_if_fail(ctxt != NULL);
    
    mode = ctxt->mode;
    type = ctxt->networkType;
    if ((mode < 0) || (mode >= undefScanMode_e) 
        || (type < 0) || (type >= connection_undefined_t))
    {
        CN_ERRORPRINTF("Invalid arguments: mode[%d] type[%d]", mode, type);
        return;
    }

    if (ctxt->scanNetworks)
    {
        CN_WARNPRINTF("Please wait while scanning");
        return;
    }

    if (delay)
    {
        if (ctxt->scanTimeoutId > 0)
        {
            CN_LOGPRINTF("remove the old timeout function for scaning");
            g_source_remove(ctxt->scanTimeoutId);
            ctxt->scanTimeoutId = 0;
        }

        // begin to scan after 5s timeout
        if (ctxt->networkType == wireless_t)
        {
            ctxt->scanTimeoutId = g_timeout_add(200, 
                                                connect_scan_start, 
                                                (gpointer)ctxt);
        }
        else
        {
            ctxt->scanTimeoutId = g_timeout_add(5000, 
                                                connect_scan_start, 
                                                (gpointer)ctxt);
        }
    }
    else
    {
        connect_scan_start((gpointer)ctxt);
    }
}

static gboolean connect_scan_start(gpointer data)
{
    CN_LOGPRINTF("entry");

    scanContext_t* ctxt;

    ctxt = (scanContext_t*)data;
    g_return_val_if_fail(ctxt != NULL, FALSE);

    if (ctxt->mode != backgroundScan_e)
    {
        // busybar begins to blink
        erbusy_blink();
    }

    // set the flag to be TRUE
    ctxt->scanNetworks = TRUE;

    if (ctxt->networkType == wireless_t)
    {
        // start scanning wireless
        wireless_scan_start(ctxt);
    }
    else
    {   
        // get the network profiles according to the 'networkType'
        if (ctxt->scan_done)
        {
            ctxt->scan_done(ctxt->networkType, NULL, 0);
        }

        // finished scan networks
        connect_scan_done(ctxt);
    }

    ctxt->scanTimeoutId = 0;
    
    // don't call me again
    return FALSE; 
}

static void connect_scan_done (scanContext_t* ctxt)
{
    CN_LOGPRINTF("entry");
   
    g_return_if_fail(ctxt != NULL);

    if (ctxt->scanNetworks)
    {
        // set the flag back to FALSE
        ctxt->scanNetworks = FALSE;

        if (ctxt->scan_done_after)
        {
            ctxt->scan_done_after();
        }

        if (ctxt->mode != backgroundScan_e)
        {
            erbusy_off();
        }
    }
}

gboolean connect_scan_freeze_ui(scanContext_t* ctxt)
{
    g_return_val_if_fail(ctxt != NULL, FALSE);

    CN_LOGPRINTF("return freeze = %d", ctxt->scanAborting);
    return ctxt->scanAborting;
}

static gboolean delay_connect_scan_abort(gpointer data)
{
    scanContext_t* ctxt;
    gboolean       ret = TRUE;  // TRUE = please call again

    ctxt = (scanContext_t*)data;
    g_return_val_if_fail(ctxt != NULL, FALSE);

    // wait the scanThread aborting
    if (!scanThread_stopped())
    {
        CN_LOGPRINTF("call me later...");
    }
    else
    {
        ctxt->scanAborting = FALSE;
        
        // no more scan ongoing
        ctxt->scanNetworks = FALSE;

        ret = FALSE;  // FALSE = don't call me again
    }

    return ret;
}

void connect_scan_abort(scanContext_t* ctxt)
{
    CN_LOGPRINTF("entry");

    g_return_if_fail(ctxt != NULL);

    if (ctxt->scanNetworks)
    {
        CN_WARNPRINTF("Abort scanning...");

        if (ctxt->mode != backgroundScan_e)
        {
            erbusy_blink();
        }

        if (ctxt->networkType == wireless_t)
        {
            // stop the scan thread
            scanThread_stop();

            // wait for scanThread done
            ctxt->scanAborting = TRUE;
            g_timeout_add(200, delay_connect_scan_abort, (gpointer)ctxt);
        }
        else
        {
            // set the flag back to FALSE
            ctxt->scanNetworks = FALSE;
        }

        if (ctxt->mode != backgroundScan_e)
        {
            erbusy_off();
        }
    }
}

static void wireless_scan_start(scanContext_t* ctxt)
{
    gboolean ok;

    CN_LOGPRINTF("entry");

    g_return_if_fail(ctxt != NULL);

    // clear old network info
    connect_scan_ctxt_empty_results(ctxt);

    CN_LOGPRINTF("Scanning for ssid[%s]", 
            ctxt->scanParms ? ctxt->scanParms->ssid : ""); 

    // start scan thread
    ok = scanThread_start(ctxt->scanParms);
    if (ok && (ctxt->mode != backgroundScan_e))
    {
        erbusy_blink();
    }
}

// signal handler for scan status change
static void on_scan_status_changed(GObject *object, 
                                   GParamSpec *arg, 
                                   gpointer data)
{
    CN_LOGPRINTF("entry");

    scanContext_t* ctxt = (scanContext_t*)data;
    g_return_if_fail(ctxt != NULL);
    
    const char *name = arg->name;
    g_return_if_fail(strcmp(name,"label") == 0);

    const gchar* scanStatus = gtk_label_get_text(GTK_LABEL(object));
    CN_LOGPRINTF("scanStatus [%s]", scanStatus);

    if (strcmp(scanStatus, "scan-done-ok") == 0)
    {
        if (ctxt->mode != backgroundScan_e)
        {
            erbusy_off();
        }
       
        gtk_label_set_text(GTK_LABEL(ctxt->scanParms->scanStatus)
                /*GTK_LABEL(object)*/,"scan-idle");
        
        if (ctxt->scan_done)
        {
            ctxt->scan_done(wireless_t,
                            ctxt->scanParms->networks, 
                            ctxt->scanParms->networks_found);
        }

        connect_scan_done(ctxt);
    }
}

