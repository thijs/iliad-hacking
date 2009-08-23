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


#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "connectionMgrLog.h"
#ifdef ECDC_LIBRARY
#include "ecdc.h"
#else
#include "ecdc_err.h"
#endif
#include "connectScreenData.h"
#include "connectionMgr.h"
#include "pingThread.h"
#include "system.h"

static volatile GThread* g_ping_thread = NULL;
static volatile gboolean g_ping_abort  = FALSE;


static gpointer pingThread(gpointer arg)
{
#ifdef ECDC_LIBRARY
    pecd_connection_t pecd_connection;
#endif
    gboolean connected = FALSE;

    ping_thread_parms *parms = (ping_thread_parms*)arg;
#ifdef ECDC_LIBRARY
    gchar *redirectUrl = connect_data_get_ecd_url();
#else
    gchar *redirectUrl = NULL;
#endif
    gchar *errmsg;

    int   i;
    int   rc;
    int   argc;
    char* argv[10];


    CN_LOGPRINTF("entry");

    gdk_threads_enter();
    gtk_label_set_text(GTK_LABEL(parms->pingStatus), "ping-started");
    gdk_threads_leave();

    parms->err = connect_enable_network(parms->settings);
    if (   g_ping_abort == FALSE
        && parms->err == ECD_NO_ERROR)
    {
        if (parms->pcshare == NULL)
        {
#ifdef ECDC_LIBRARY
            // connect to iDS
            pecd_connection = ecd_connection_create(redirectUrl, 
                                                    DUMMY_EUI64, 
                                                    DUMMY_PRODUCT_ID);
            if (pecd_connection)
            {
                if (ecd_ping(pecd_connection))
                {
                    connected = TRUE;
                }
                else
                {
                    connect_disable_network();
                }
                parms->err = ecd_errno(pecd_connection);
                ecd_connection_destroy(pecd_connection);
            }
#else
			connected = TRUE;
#endif
        }
        else
        {
            // connect to PC share
            argc = 0;
            argv[argc++] = COMMAND_MOUNT_SAMBA;
            argv[argc++] = "mount";
            argv[argc++] = parms->pcshare->pcname ? parms->pcshare->pcname    : "";
            argv[argc++] = parms->pcshare->sharename ? parms->pcshare->sharename : "";
            argv[argc++] = parms->pcshare->workgroup ? parms->pcshare->workgroup : "";
            if (    parms->pcshare->username
                &&  parms->pcshare->username[0] != '\0' )
            {
                argv[argc++] = parms->pcshare->username;
                argv[argc++] = parms->pcshare->password;
            }
            argv[argc] = NULL;
            g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );

            CN_LOGPRINTF("execute command [%s]", argv[0]);
            for (i = 0 ; i < argc ; i++)
            {
                CN_LOGPRINTF("    argv[%d] = [%s]", i, argv[i]);
            }
            rc = fork_exec(argc, argv);
            CN_LOGPRINTF("fork_exec [%s] returned [%d]", argv[0], rc);
            if (rc == 0)
            {
                connected = TRUE;
            }
            else
            {
                CN_ERRORPRINTF("Execute [%s] failed - "
                               "fork_exec returns [%d]", 
                               argv[0], rc);
                connect_disable_network();
                parms->err = ECD_NO_CONNECTION;
            }
        }
    }

    g_free(redirectUrl);

    // tell GTK thread we are done ...
    gdk_threads_enter();
    {
        g_ping_thread = NULL;
        if (gtk_main_level() == 0)
        {
            CN_WARNPRINTF("    -- oops, gtk_main has quit!");
        }
        else
        {
            if (connected)
            {
                gtk_label_set_text(GTK_LABEL(parms->pingStatus), 
                                   "ping-done-ok");
            }
            else
            {
                if (g_ping_abort)
                {
                    errmsg = "ping-done-abort";
                }
                else
                {
                    if (  ((parms->settings->connection == wireless_t) 
                            && (parms->err == ECD_WIRELESS_ERROR))
                       || ((parms->settings->connection == wired_t)
                            && (parms->err == ECD_WIRED_ERROR))
                       || ((parms->settings->connection == dialup_t)
                            && (parms->err == ECD_DIALUP_ERROR))  )
                    {
                        errmsg = "ping-network-error";
                    }
                    else
                    {
                        if (parms->pcshare)
                        {
                            errmsg = "ping-pcshare-error";
                        }
                        else
                        {
                            errmsg = "ping-done-error";
                        }
                    }
                }
                gtk_label_set_text(GTK_LABEL(parms->pingStatus), errmsg);
            }
        }
    }
    gdk_threads_leave();
    
    // ... and quit
    CN_LOGPRINTF("exit, connected [%d]", connected);
    return (gpointer)connected;
}


gboolean pingThread_start(ping_thread_parms* ping_parms)
{
    GThread* thread = NULL;
    GError*  error = NULL;

    CN_LOGPRINTF("entry");
    if (g_ping_thread)
    {
        CN_ERRORPRINTF("ping thread already running");
        return FALSE;
    }

    // create the ping thread
    g_ping_abort = FALSE;
    thread = g_thread_create(pingThread, ping_parms, FALSE, &error);
    if (error != NULL)
    {
        CN_ERRORPRINTF("Failed to create ping thread - %s [%d]", 
                       error->message, error->code);
        g_free(error);
        error = NULL;
    }
    else
    {
        g_ping_thread = thread;
    }

    return TRUE;
}


gboolean pingThread_stop()
{
    CN_LOGPRINTF("entry");

    if (g_ping_thread)
    {
        // stop the ping thread
        g_ping_abort = TRUE;

        // stop connect action, when running
        connect_enable_network_abort();

        // stop any process spawned by pingThread
        pid_t pid = get_forkexec_child_pid();
        if (pid > 0)
        {
            kill(pid, SIGTERM);
        }

        return TRUE;
    }
    else
    {
        CN_ERRORPRINTF("ping thread not running");
        return FALSE;
    }
}


gboolean pingThread_wait(int timeout_sec)
{
    int max_ticks = 2 * timeout_sec;

    CN_LOGPRINTF("entry");
    
    if (g_ping_thread)
    {
        // sorry don't like busy-wait, 
        // but g_thread_join has no timeout option
        while (g_ping_thread  &&  --max_ticks >= 0)
        {
            g_usleep(500*1000L);
        }
        return TRUE;
    }
    else
    {
        CN_ERRORPRINTF("ping thread not running");
        return FALSE;
    }
}

gboolean pingThread_stopped(void)
{
    CN_LOGPRINTF("entry");

    if (g_ping_thread)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

