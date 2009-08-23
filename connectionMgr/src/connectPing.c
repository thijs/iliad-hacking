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
 * \file connectPing.c
 * \brief connectionMgr -According to the results of scanning networks,
 *                       use the profile one by one to ping 
 *                       until it succeeds.
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#include <string.h>

#include <gtk/gtk.h>
#include <glib.h>

#include <liberdm/connectionMgrConstants.h>

#include "connectionMgrLog.h"
#include "connectScreenData.h"
#include "connectScreenProfiles.h"
#include "pingThread.h"
#include "connectPing.h"
#include "connectionMgr.h"

static gboolean connect_ping_start (gpointer data);
static void connect_ping_done (pingContext_t *ctxt, pingStatus_t result);
static void on_ping_status_changed (GObject  *object, 
                                    GParamSpec *arg1, 
                                    gpointer data);

static gboolean connect_ping_select_profile(pingContext_t *ctxt, 
                                            int profileIndex);
static void connect_ping_unselect_profile (pingContext_t *ctxt);

pingContext_t *connect_ping_ctxt_new(void)
{
    pingContext_t *ctxt;
    GtkWidget *pingStatus;

    CN_LOGPRINTF("entry");

    ctxt = g_new0(pingContext_t, 1);
    g_assert(ctxt != NULL);

    // set the default values
    connect_ping_ctxt_set_mode(ctxt, undefPingMode_e);
    connect_ping_ctxt_set_connect_type(ctxt, cmgrConnectUnknown_e);
    connect_ping_ctxt_set_done_callbacks(ctxt, NULL, NULL);

    ctxt->selectedProfileIndex = -1;
    ctxt->pingingProfileIndexStart = -1;
    ctxt->pingingProfileIndex = -1;
    
    // initilize the pingParms 
    ctxt->pingParms = g_new0(ping_thread_parms, 1);
    g_assert(ctxt->pingParms != NULL);

    // ctxt->pingParms->pingStatus
    pingStatus = gtk_label_new ("ping-idle");
    g_signal_connect(pingStatus, "notify", 
                     G_CALLBACK (on_ping_status_changed), ctxt);
    ctxt->pingParms->pingStatus = pingStatus;
 
    // the other values are set to be NULL or zero implicitly by g_new0

    return ctxt;
}

void connect_ping_ctxt_set_mode(pingContext_t* ctxt, pingMode_t mode)
{
    if (ctxt && (mode >= 0) && (mode < undefPingMode_e))
    {
        ctxt->mode = mode;
    }
}

void connect_ping_ctxt_set_connect_type(pingContext_t* ctxt, 
                                        cmgrConnectType_t connectType)
{
    if (ctxt && (connectType >= 0) && (connectType < cmgrConnectUndef_e))
    {
        ctxt->connectType = connectType;
    }
}

void connect_ping_ctxt_set_get_initial_profile(pingContext_t* ctxt, 
                           get_initial_profile_t* get_initial_profile)
{
    if (ctxt)
    {
        ctxt->get_initial_profile = get_initial_profile;
    }
}

void connect_ping_ctxt_set_access_network_profiles_callbacks(pingContext_t *ctxt,
                        get_n_network_profiles_t *get_n_network_profiles,
                        get_network_profile_t *get_network_profile,
                        select_next_regprofile_t *select_next_regprofile)
{
    if (ctxt)
    {
        ctxt->get_n_network_profiles = get_n_network_profiles;
        ctxt->get_network_profile = get_network_profile;
        ctxt->select_next_regprofile = select_next_regprofile;
    }
}

void connect_ping_ctxt_set_ui_callbacks(pingContext_t* ctxt,
                        ui_update_status_t* ui_update_status,
                        ui_display_settings_t* ui_display_settings,
                        ui_select_profile_t* ui_select_profile,
                        ui_unselect_all_profile_t* ui_unselect_all_profile)
{
    if (ctxt)
    {
        ctxt->ui_update_status = ui_update_status;
        ctxt->ui_display_settings = ui_display_settings;
        ctxt->ui_select_profile = ui_select_profile;
        ctxt->ui_unselect_all_profile = ui_unselect_all_profile;
    }
}

void connect_ping_ctxt_set_done_callbacks(pingContext_t* ctxt, 
                                          on_connected_t* on_connected,
                                          on_failed_all_t* on_failed_all)
{
    if (ctxt)
    {
       ctxt->on_connected = on_connected;
       ctxt->on_failed_all = on_failed_all;
    }
}

static void free_network_pc_settings(ping_thread_parms *pingParms)
{
    regNetworkProfile_t *settings;
    regPCProfile_t *pcshare;

    if (pingParms)
    {
       settings = (regNetworkProfile_t*)pingParms->settings;
       erRegFreeNetworkProfile(settings);
       pingParms->settings = NULL;
           
       pcshare = (regPCProfile_t*)pingParms->pcshare;
       erRegFreePCProfile(pcshare);
       pingParms->pcshare = NULL;
    }
}

void connect_ping_ctxt_destory(pingContext_t* ctxt)
{
    ping_thread_parms *pingParms;

   if (ctxt)
   {
       if (ctxt->pingParms)
       {
           pingParms = ctxt->pingParms;
           free_network_pc_settings(pingParms);
           /*
           if (pingParms->pingStatus)
           {
               gtk_widget_destroy(pingParms->pingStatus);
           }
           */

           g_free(pingParms);
       }
       g_free(ctxt);
   }
}

////////////////////////////////////////////////////////////////////////////
// manage the indice of profiles 
////////////////////////////////////////////////////////////////////////////
int connect_ping_get_pinging_profile_index_start(pingContext_t* ctxt)
{
    g_return_val_if_fail(ctxt != NULL, -1);

    CN_LOGPRINTF("entry profileIndex=%d", ctxt->pingingProfileIndexStart);

    return ctxt->pingingProfileIndexStart;
}

void connect_ping_set_pinging_profile_index_start(pingContext_t* ctxt, 
                                                  int profileIndex)
{
    g_return_if_fail(ctxt != NULL);

    CN_LOGPRINTF("entry[%d]", profileIndex);
 
    ctxt->pingingProfileIndexStart = profileIndex;
}

int connect_ping_get_pinging_profile_index(pingContext_t* ctxt)
{
    g_return_val_if_fail(ctxt != NULL, -1);

    CN_LOGPRINTF("entry profileIndex=%d", ctxt->pingingProfileIndex);
 
    return ctxt->pingingProfileIndex;
}

void connect_ping_set_pinging_profile_index(pingContext_t* ctxt, 
                                            int profileIndex)
{
    g_return_if_fail(ctxt != NULL);

    CN_LOGPRINTF("entry profileIndex[%d]", profileIndex);
    
    ctxt->pingingProfileIndex = profileIndex;
}

int connect_ping_get_selected_profile_index(pingContext_t* ctxt)
{
    g_return_val_if_fail(ctxt != NULL, -1);

    CN_LOGPRINTF("entry profileIndex=%d", ctxt->selectedProfileIndex);
    
    return ctxt->selectedProfileIndex;
}

void connect_ping_set_selected_profile_index(pingContext_t* ctxt, 
                                             int profileIndex)
{
    g_return_if_fail(ctxt != NULL);

    CN_LOGPRINTF("entry profileIndex[%d]", profileIndex);
    
    ctxt->selectedProfileIndex = profileIndex;
}

static void connect_ping_select_first_profile(pingContext_t* ctxt)
{
    int nProfiles = 0;

    CN_LOGPRINTF("entry");

    g_return_if_fail(ctxt != NULL);
    
    if (ctxt->get_n_network_profiles)
    { 
        nProfiles = ctxt->get_n_network_profiles();
    }

    if (nProfiles > 0)
    {
        connect_ping_select_profile(ctxt, 0);
    }
}

/*
static void connect_ping_select_last_profile(pingContext_t* ctxt)
{
    CN_LOGPRINTF("entry");

    int nProfiles = 0;
   
    if (ctxt->get_n_network_profiles)
    {
        nProfiles = ctxt->get_n_network_profiles();
    }
    if (nProfiles > 0)
    {
        connect_ping_select_profile(ctxt, nProfiles);
    }
}*/

static gboolean connect_ping_select_next_profile(pingContext_t* ctxt)
{
    CN_LOGPRINTF("entry");

    gint  profileIndex;
    guint nProfiles;
    gboolean selected;

    g_return_val_if_fail(ctxt != NULL, FALSE);

    selected = FALSE;
    profileIndex = connect_ping_get_selected_profile_index(ctxt);
    if (ctxt->get_n_network_profiles)
    {
        nProfiles = ctxt->get_n_network_profiles();
    }
    else
    {
        nProfiles = 0;
    }
    if ((profileIndex >= 0) && (profileIndex < nProfiles))
    {
        if ((profileIndex + 1) < nProfiles)
        {
            connect_ping_select_profile(ctxt, profileIndex+1);
            selected = TRUE;
        }
    }

    return selected;
}

/*
static gboolean connect_ping_select_previous_profile(pingContext_t* ctxt)
{
    CN_LOGPRINTF("entry");

    gint  profileIndex;
    guint nProfiles;
    gboolean selected;
   
    g_return_val_if_fail(ctxt != NULL, FALSE);

    selected = FALSE;
    profileIndex = connect_ping_get_selected_profile_index(ctxt);
    if (ctxt->get_n_network_profiles)
    {
        nProfiles = ctxt->get_n_network_profiles();
    }
    else
    {
        nProfiles = 0;
    }

    if ((profileIndex >= 0) && (profileIndex < nProfiles))
    {
        if ((profileIndex - 1) > 0)
        {
            connect_ping_select_profile(ctxt, profileIndex - 1);
            selected = TRUE;
        }
    }

    return selected;
}
*/

static gboolean connect_ping_select_profile(pingContext_t* ctxt,  
                                            int profileIndex)
{
    CN_LOGPRINTF("entry");

    guint nProfiles;
    gboolean selected;
    
    g_return_val_if_fail(ctxt != NULL, FALSE);

    selected = FALSE;
    if (ctxt->get_n_network_profiles)
    {
        nProfiles = ctxt->get_n_network_profiles();
    }
    else
    {
        nProfiles = 0;
    }
    if ((profileIndex >= 0) && (profileIndex < nProfiles))
    {
        connect_ping_set_selected_profile_index(ctxt, profileIndex);
        selected = TRUE;
    }
    return selected;
}

// call this function only once
void connect_ping_select_initial_profile (pingContext_t* ctxt)
{
    CN_LOGPRINTF ("entry");

    int profileIndex = 0;
    cmgrConnectType_t connectType;
    
    g_return_if_fail(ctxt != NULL);

    // select initial profile to use when connecting
    connectType = ctxt->connectType;
    if (connectType == cmgrConnectUnknown_e)
    {
        CN_WARNPRINTF("Unknown connect type[%d].", connectType);

        // edit mode of iDS/PC unknown: no profile selected
        connect_ping_unselect_profile (ctxt);
    }
    else
    {
        if (ctxt->get_initial_profile)
        {
            profileIndex = ctxt->get_initial_profile();
        }
        
        connect_ping_select_profile(ctxt, profileIndex);                
        connect_ping_set_pinging_profile_index_start(ctxt, profileIndex);      

        if (ctxt->mode != backgroundPing_e && ctxt->ui_select_profile)
        {
            ctxt->ui_select_profile(profileIndex);
        }
    }
}

static void connect_ping_unselect_profile (pingContext_t* ctxt)
{
    CN_LOGPRINTF ("entry");

    g_return_if_fail(ctxt != NULL);

    connect_ping_set_selected_profile_index(ctxt, -1);
}


////////////////////////////////////////////////////////////////////////////
// connect-to-server one profile by one
////////////////////////////////////////////////////////////////////////////
void connect_ping(pingContext_t* ctxt, gboolean delay)
{
    CN_LOGPRINTF ("entry delay[%d]", delay);

    g_return_if_fail(ctxt != NULL);

    if (ctxt->pingServer)
    {
        CN_WARNPRINTF("Please wait while pinging...");
        return;
    }

    if (delay)
    {
        if (ctxt->pingTimeoutId > 0)
        {
            CN_LOGPRINTF("remove the old timeout function for pinging");
            g_source_remove(ctxt->pingTimeoutId);
            ctxt->pingTimeoutId = 0;
        }

        // begin to ping after 'delay' timeout
        ctxt->pingTimeoutId = g_timeout_add(delay, 
                                            connect_ping_start, 
                                            (gpointer)ctxt);
    }
    else
    {
        connect_ping_start(ctxt);
    }
}

gboolean connect_ping_in_pinging(pingContext_t * ctxt)
{
    g_return_val_if_fail(ctxt, FALSE);
    
    CN_LOGPRINTF("return [%d]", ctxt->pingServer);
    return ctxt->pingServer;
}

// start connecting to server, using the currently selected profile
static gboolean connect_ping_start (gpointer data)
{
    pingContext_t*          ctxt;
    int                     profileIndex;
    gboolean                ok;
    const networkProfile_t* settings = NULL;
    regNetworkProfile_t*    regSettings = NULL;
    const regPCProfile_t *pcshare = NULL;

    ctxt = (pingContext_t*)data;
    g_return_val_if_fail(ctxt != NULL, FALSE);

    // set the ctxt->pingParms parameters
    profileIndex = connect_ping_get_selected_profile_index(ctxt);
    CN_LOGPRINTF ("entry: selected profile [%d]", profileIndex);
    g_return_val_if_fail (profileIndex != -1, FALSE);

    if (ctxt->get_network_profile)
    {
        settings = ctxt->get_network_profile(profileIndex);
    }
    g_return_val_if_fail(settings != NULL, FALSE);

    // free the old network settings and pcshare settings
    free_network_pc_settings(ctxt->pingParms);

    // get the network settings and pcshare settings if needed
    if (settings->nRegProfiles > 0
        && settings->regSettingsList
        && settings->regIndexList
        && ((settings->nActiveIndex >= 0) 
        && (settings->nActiveIndex < settings->nRegProfiles)))
    {
        regSettings = settings->regSettingsList[settings->nActiveIndex];
        ctxt->pingParms->settings = erRegDupNetworkProfile (regSettings);
        CN_LOGPRINTF("settings->regIndexList[%d]=%d", 
                      settings->nActiveIndex,
                      settings->regIndexList[settings->nActiveIndex]);
    }
    else
    {
        CN_LOGPRINTF("settings->scanSettings->encryption=%d", 
                     settings->scanSettings->encryption);
        if (connect_data_get_ac_wlan() 
            && settings->scanSettings 
            && settings->scanSettings->encryption == encr_none_t)
        {
            CN_LOGPRINTF("here");

            regSettings = network_spec_to_reg_network(settings->scanSettings);
            ctxt->pingParms->settings = erRegDupNetworkProfile(regSettings);
            erRegFreeNetworkProfile(regSettings);
            regSettings = NULL;
        }
    }

    // set the flag to be TRUE
    ctxt->pingServer = TRUE;
    connect_ping_set_pinging_profile_index(ctxt, profileIndex);
    
    if (ctxt->pingParms->settings)
    {
        if (ctxt->mode != backgroundPing_e && ctxt->ui_update_status)
        {
            ctxt->ui_update_status(profileIndex, pingConnecting_e);
        }

        pcshare = common_data_get_pcsharedata(0);
        if (pcshare && ctxt->connectType == cmgrConnectPCShare_e)
        {
            ctxt->pingParms->pcshare = erRegDupPCProfile (pcshare);
        }
        else
        {
            ctxt->pingParms->pcshare = NULL;
        }

        // start Ping thread
        pcshare = ctxt->pingParms->pcshare;
        CN_WARNPRINTF ("profile [%s] pcshare [%s] [%s]",
                        ctxt->pingParms->settings->name,
                        pcshare ? pcshare->pcname : "",
                        pcshare ? pcshare->sharename : "");
        ok = pingThread_start(ctxt->pingParms);
        if (ok)
        {
            CN_LOGPRINTF("Succeeded to start ping thread");
        }
        else
        {
            CN_ERRORPRINTF ("Failed to start ping thread");
        }
    }
    else
    {
        CN_LOGPRINTF("The %dth profile is the one by scanning,"
                     " but it has no corresponding profile in registry. "
                     "Skipped it. It can be used by changing the settings "
                     "or filling in the encryption key. ", 
                     profileIndex);
        if (settings->scanSettings 
            && settings->scanSettings->encryption == encr_none_t)
        {
            connect_ping_done(ctxt, pingSkipped_e);
        }
        else
        {
            connect_ping_done(ctxt, pingNeedKey_e);
        }
    }

    ctxt->pingTimeoutId = 0;
    return FALSE;
}

gboolean connect_ping_freeze_ui(pingContext_t* ctxt)
{
    g_return_val_if_fail(ctxt != NULL, FALSE);

    CN_LOGPRINTF("return freeze = %d", ctxt->pingAborting);
    return ctxt->pingAborting;
}

static gboolean delay_connect_ping_abort(gpointer data)
{
    pingContext_t* ctxt = (pingContext_t*)data;
    g_return_val_if_fail(ctxt != NULL, FALSE);

    gboolean ret = TRUE;  // TRUE = please call again
    gint profileIndex = connect_ping_get_pinging_profile_index(ctxt);

    // wait the pingThread aborting
    if (!pingThread_stopped())
    {
        CN_LOGPRINTF("call me later...");
    }
    else
    {
        ctxt->pingAborting = FALSE;

        if (ctxt->mode != backgroundPing_e && ctxt->ui_update_status)
        {
            // update the ui
            ctxt->ui_update_status(profileIndex, pingAborted_e);
        }
        
        // no more ping ongoing
        ctxt->pingServer = FALSE;
        connect_ping_set_pinging_profile_index(ctxt, -1);

        ret = FALSE;  // FALSE = don't call me again
    }

    return ret;
}

// stop connecting to server
void connect_ping_abort (pingContext_t* ctxt)
{
    CN_LOGPRINTF ("entry");
   
    g_return_if_fail(ctxt != NULL);

    if (ctxt->pingServer)
    {
        CN_WARNPRINTF("Abort pinging...");

        gint profileIndex = connect_ping_get_pinging_profile_index(ctxt);

        if (ctxt->mode != backgroundPing_e && ctxt->ui_update_status)
        {
            // update the ui
            ctxt->ui_update_status(profileIndex, pingAborting_e);
        }
        
        // stop the pingThread
        pingThread_stop();

        // wait for pingThread done
        ctxt->pingAborting = TRUE;
        g_timeout_add(200, delay_connect_ping_abort, (gpointer)ctxt);
    }
}

// connecting to server is done
static void connect_ping_done (pingContext_t* ctxt, pingStatus_t result)
{
    GtkWidget *pingStatus;
    int profileIndex, startIdx;
    gboolean same = TRUE;
    gboolean selected;

    CN_LOGPRINTF ("entry");

    g_return_if_fail(ctxt != NULL);

    if (ctxt->pingServer)
    {
        // set the ctxt->pingParms parameters
        pingStatus = ctxt->pingParms->pingStatus;
        gtk_label_set_text(GTK_LABEL(pingStatus), "ping-idle");

        // free the ctxt->pingParms parameters
        free_network_pc_settings(ctxt->pingParms);

        // update the ui
        profileIndex = connect_ping_get_pinging_profile_index(ctxt);
        g_return_if_fail(profileIndex != -1);

        if (ctxt->mode != backgroundPing_e && ctxt->ui_update_status)
        {
            ctxt->ui_update_status(profileIndex, result);
        }
   
        if (result == pingSucceeded_e)
        {
            ctxt->pingServer = FALSE;
            connect_ping_set_pinging_profile_index(ctxt, -1);
           
            if (ctxt->mode != backgroundPing_e 
                && ctxt->ui_unselect_all_profile)
            {
                ctxt->ui_unselect_all_profile();
            }
 
            if (ctxt->on_connected)
            {
                ctxt->on_connected(profileIndex);
            }
        }
        else
        {
            if (ctxt->select_next_regprofile 
                && !ctxt->select_next_regprofile(profileIndex))
            {
                // connect using next profile
                 selected = connect_ping_select_next_profile(ctxt);
                 if (!selected)
                 {
                     connect_ping_select_first_profile(ctxt);
                 }            
                 same = FALSE;
            }
            
             // update the ui
             profileIndex = connect_ping_get_selected_profile_index(ctxt);
             CN_LOGPRINTF ("entry: selected profile [%d]", profileIndex);
             
             if (ctxt->mode != backgroundPing_e)
             {
                 if (same && ctxt->ui_display_settings)
                 {
                     ctxt->ui_display_settings(profileIndex, TRUE);
                 }
                 
                 if (ctxt->ui_select_profile)
                 {
                     ctxt->ui_select_profile(profileIndex);
                 }
             }

            // set the flag to be FALSE 
            // in order to setup the context for another round pinging
            ctxt->pingServer = FALSE;
            connect_ping_set_pinging_profile_index(ctxt, -1);

            startIdx = connect_ping_get_pinging_profile_index_start(ctxt);
            if (same || (profileIndex != startIdx))
            {
                if (ctxt->mode == backgroundPing_e)
                {
                    connect_ping(ctxt, FALSE);
                }
                else
                {
                    connect_ping(ctxt, TRUE);
                }
            }
            else
            {
                CN_WARNPRINTF ("All profiles done, stop connecting");
                connect_ping_unselect_profile (ctxt);

                if (ctxt->mode != backgroundPing_e 
                    && ctxt->ui_unselect_all_profile)
                {
                    ctxt->ui_unselect_all_profile();
                }
               
                if (ctxt->on_failed_all)
                {
                    ctxt->on_failed_all();
                }
            }
        }
    }
}

// ping status received from pingThread
static void on_ping_status_changed (GObject * object, 
                                    GParamSpec * arg1, 
                                    gpointer data)
{
    CN_LOGPRINTF ("entry");

    pingContext_t* ctxt = (pingContext_t*)data;
    g_return_if_fail(ctxt != NULL);

    const char *name = arg1->name;
    g_return_if_fail (strcmp (name, "label") == 0);

    const char *pingStatus;
    pingStatus = gtk_label_get_text (GTK_LABEL (object));
    CN_LOGPRINTF ("pingStatus [%s]\n", pingStatus);

    if (strcmp(pingStatus, "ping-done-ok") == 0)
    {
        connect_ping_done(ctxt, pingSucceeded_e);
    }
    else if (strcmp(pingStatus, "ping-done-error") == 0)
    {
        connect_ping_done(ctxt, pingFailed_e);
    }
    else if (strcmp(pingStatus, "ping-pcshare-error") == 0)
    {
        connect_ping_done (ctxt, pingFailedPCShare_e);
    }
    else if (strcmp(pingStatus, "ping-network-error") == 0)
    {
        connect_ping_done(ctxt, pingFailedNetwork_e);
    }
    else if(strcmp (pingStatus, "ping-done-abort") == 0)
    {
        CN_LOGPRINTF("ping-done-abort");
    }
    else
    {
        // ping-idle
        /* ignore */ ;
    }
}


