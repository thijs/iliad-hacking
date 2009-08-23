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

#include <string.h>
#include <stdlib.h>

#include "connectionMgrLog.h"
#include "connectionMgr.h"
#include "scanThread.h"

#define MAX_NETWORKS  50

static volatile GThread* g_scan_thread = NULL;
static volatile gboolean g_scan_abort  = FALSE;


static int compare_networks(const void* p1, const void* p2);
static int quality_to_integer(const char*string);

static gpointer scanThread(gpointer arg)
{
    int      i;
    char*    cp;
    char*    line;
    gboolean b;
    gchar*   p_stdout = NULL;
    guint    networks_found = 0;

    scan_thread_parms* parms = (scan_thread_parms*)arg;
    network_spec_t*    networks = NULL;


    CN_LOGPRINTF("entry");

    gdk_threads_enter();
    {
        // clean networks table, just to be sure
        for (i = 0 ; i < parms->networks_num ; i++)
        {
            g_free(parms->networks[i].ssid);
        }
        parms->networks_found = 0;

        // local array of networks found
        networks = g_new0(network_spec_t, MAX_NETWORKS);
        g_assert(networks != NULL);

        // tell GTK thread we are starting
        gtk_label_set_text(GTK_LABEL(parms->scanStatus), "scan-started");
    }
    gdk_threads_leave();

    // scan for networks
    fflush(stdout);

    char cmd[100];
    snprintf(cmd, sizeof(cmd), "%s \"%s\"", 
            COMMAND_SCAN_WIRELESS, parms->ssid);

    b = g_spawn_command_line_sync(cmd, &p_stdout, NULL, NULL, NULL);
    if (b == FALSE)
    {
        CN_ERRORPRINTF("g_spawn_command_line_sync failed on command [%s]", 
                       cmd);
    }
    else
    {
        for ( line = strtok(p_stdout, "\n") ;
              line != NULL ;
              line = strtok(NULL, "\n")      )
        {
            CN_LOGPRINTF("line [%s]", line);

            // One line received specifying a wireless network, syntax:
            // Network: "<ssid-string>" <security-type> <quality>
            char *ssid = NULL;
            char *security = NULL;
            char *quality = NULL;
            wireless_encr_type_t encr_type = encr_none_t;

            // precaution because string functions probably not thread-safe
            gdk_threads_enter();
            {
                // parse string into tokens
                // line = 1st token = 'Networks:'
                cp = strchr(line, ' ');
                if (cp  &&  cp[1] == '"')
                {
                    *cp = '\0';
                    
                    // ssid = 2nd token = '"<ssid-string>"'
                    // Note: "<ssid-string>" 
                    //       may contain white-space characters
                    // Note: strip quotes from "<ssid-string>"
                    ssid = cp + 2;
                    cp = strchr(ssid, '"');
                    if (cp  &&  cp[1] == ' ')
                    {
                        *cp = '\0';

                        // security = 3rd token = '[free|wep|wpa]'
                        security = cp + 2;
                        cp = strchr(security, ' ');
                        if (cp)
                        {
                            *cp = '\0';
                            if (strcmp(security, "free") == 0)
                            {
                                encr_type = encr_none_t;
                            }
                            else if (strcmp(security, "wep") == 0)
                            {
                                encr_type = encr_wep_t;
                            }
                            else if (strcmp(security, "wpa") == 0)
                            {
                                encr_type = encr_wpa_t;
                            }
                            else
                            {
                                encr_type = encr_undefined_t;
                            }

                            // quality = 4th token = 'xx/yy'
                            quality = cp + 1;
                            cp = strchr(quality, ' ');
                            if (cp)
                            {
                                *cp = '\0';
                            }
                        }
                    }
                }
                
                // check syntax and store network details
                if (   strcmp(line, "Network:") == 0
                    && ssid     != NULL
                    && security != NULL
                    && quality  != NULL )
                {
                    i = networks_found++;
                    if (i < MAX_NETWORKS)
                    {
                        networks[i].ssid = strdup(ssid);
                        networks[i].encryption = encr_type;
                        networks[i].quality = quality_to_integer(quality);
                        CN_LOGPRINTF("Network: ssid [%s] "
                                     "security [%s] [%d] quality [%s]", 
                                     ssid, security, encr_type, quality);
                    }
                    else
                    {
                        // oops
                        networks_found--;
                        CN_LOGPRINTF("Too many networks: "
                                     "MAX_NETWORKS = [%d] "
                                     "networks_found = [%d]", 
                                     MAX_NETWORKS, networks_found);
                    }
                }
            }
            gdk_threads_leave();
        }

        // sort networks and report to GTK thread
        qsort(networks, 
              networks_found, 
              sizeof(networks[0]), 
              compare_networks);
        for (i = 0 ; i < networks_found && i < parms->networks_num ; i++)
        {
            parms->networks[i] = networks[i];
            parms->networks_found++;
        }
        for ( ; i < networks_found ; i++)
        {
            g_free(networks[i].ssid);
        }
    }
    g_free(p_stdout);
    p_stdout = NULL;

    // tell GTK thread we are done ...
    gdk_threads_enter();
    {
        g_scan_thread = NULL;
        if (gtk_main_level() == 0)
        {
            CN_WARNPRINTF("    -- oops, gtk_main has quit!");
        }
        else
        {
            if (g_scan_abort)
            {
                gtk_label_set_text(GTK_LABEL(parms->scanStatus), 
                                   "scan-done-abort");
            }
            else
            {
                gtk_label_set_text(GTK_LABEL(parms->scanStatus), 
                                   "scan-done-ok");
            }
        }
    }
    gdk_threads_leave();
    
    // ... and quit
    CN_LOGPRINTF("exit");
    g_free(networks);
    return 0;
}


gboolean scanThread_start(scan_thread_parms* scan_parms)
{
    GThread* thread = NULL;
    GError*  error = NULL;

    CN_LOGPRINTF("entry");
    if (g_scan_thread)
    {
        CN_ERRORPRINTF("scan thread already running");
        return FALSE;
    }

    // create the scan thread
    g_scan_abort = FALSE;
    thread = g_thread_create(scanThread, scan_parms, FALSE, &error);
    if (error != NULL)
    {
        CN_ERRORPRINTF("Failed to create scan thread - %s [%d]", 
                       error->message, error->code);
        g_free(error);
        error = NULL;
    }
    else
    {
        g_scan_thread = thread;
    }

    return TRUE;
}


gboolean scanThread_stop()
{
    CN_LOGPRINTF("entry");

    if (g_scan_thread)
    {
        // stop the scan thread
        g_scan_abort = TRUE;

        return TRUE;
    }
    else
    {
        CN_ERRORPRINTF("scan thread not running");
        return FALSE;
    }
}


gboolean scanThread_wait(int timeout_sec)
{
    int max_ticks = 2 * timeout_sec;

    CN_LOGPRINTF("entry");
    if (g_scan_thread)
    {
        // sorry don't like busy-wait, 
        // but g_thread_join has no timeout option
        while (g_scan_thread  &&  --max_ticks >= 0)
        {
            g_usleep(500*1000L);
        }

        return TRUE;
    }
    else
    {
        CN_ERRORPRINTF("scan thread not running");
        return FALSE;
    }
}

gboolean scanThread_stopped(void)
{
    CN_LOGPRINTF("entry");

    if (g_scan_thread)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static int compare_networks(const void* p1, const void* p2)
{
    const network_spec_t* left  = p1;
    const network_spec_t* right = p2;
    int   ret = 0;

    if (left->quality != right->quality)
    {
        // 1st sort key: signal quality
        ret = left->quality - right->quality;
    }
    else if (left->encryption != right->encryption)
    {
        // 2st sort key: encryption type
        if (left->encryption == encr_none_t)
        {
            ret = -1;
        }
        if (right->encryption == encr_none_t)
        {
            ret = 1;
        }

        if (left->encryption == encr_wep_t)
        {
            ret = -1;
        }
        if (right->encryption == encr_wep_t)
        {
            ret = 1;
        }

        if (left->encryption == encr_wpa_t)
        {
            ret = -1;
        }
        if (right->encryption == encr_wpa_t)
        {
            ret = 1;
        }
        
        if (left->encryption == encr_undefined_t)
        {
            ret = -1;
        }
        if (right->encryption == encr_undefined_t)
        {
            ret = 1;
        }
    }
    else
    {
        // 3rd sort key: network name
        ret = g_ascii_strcasecmp(left->ssid, right->ssid);
    }
   
    CN_LOGPRINTF("ret=%d", ret);

    // make the result of qsort to be descending
    ret = 0 - ret; 
    return ret;
}

// convert string in format 'xx/yy' into integer
static int quality_to_integer(const char*string)
{
    char *cp, *cpx, *cpy=NULL;
    int   x,  y=0;
    int   integer;
    
    integer = 0;
    if (string)
    {
        cpx = g_strdup(string);
        if (cpx)
        {
            cp = strchr(cpx, '/');
            if (cp)
            {
                cp[0] = '\0';
                cpy = &cp[1];
            }
            
            x = atoi(cpx);
            if (cpy)
            {
                y = atoi(cpy);
            }

            CN_LOGPRINTF("x=%d,y=%d", x, y); 
            
            if (y != 0)
            {
                integer = x * 100 / y;
            }
           
            g_free(cpx);
            cpx = NULL;
        }
    }

    CN_LOGPRINTF("string[%s]-->integer[%d]", string, integer);
    return integer;
}

void free_network_spec(network_spec_t* pNetwork)
{
    if (pNetwork)
    {
        g_free(pNetwork->ssid);
        g_free(pNetwork);
    }
}

network_spec_t *dup_network_spec(network_spec_t* pNetwork)
{
    network_spec_t *pDupNetwork = NULL;
    if (pNetwork)
    {
        pDupNetwork = g_new0(network_spec_t, 1);
        if (pDupNetwork)
        {
            pDupNetwork->ssid = g_strdup(pNetwork->ssid);
            pDupNetwork->encryption = pNetwork->encryption;
            pDupNetwork->quality = pNetwork->quality;
        }
    }
    return pDupNetwork;
}

