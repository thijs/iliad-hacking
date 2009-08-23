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
 * \file connectBackground.c
 * \brief connectionMgr - background connection 
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <liberdm/connectionMgrConstants.h>

#include "connectionMgrLog.h"
#include "connectScan.h"
#include "connectPing.h"
#include "connectScreenData.h"
#include "connectBackground.h"
#include  "connectionMgr.h"

static void connect_background_scan_networks_done(void);

void connect_background_restore_scan_ctxt(void)
{
    scanContext_t *ctxt;
    connection_t networkType;
    char *ssidList;

    CN_LOGPRINTF("entry");

    ctxt = cmgr_get_scan_ctxt();
    if (ctxt)
    {
        connect_scan_ctxt_set_mode(ctxt, backgroundScan_e);
        networkType = connect_data_get_network_type();
        connect_scan_ctxt_set_network_type(ctxt, networkType);
        connect_scan_ctxt_set_done_callbacks(ctxt, 
                        connect_data_init_network_profiles,
                        connect_background_scan_networks_done);
        ssidList = connect_data_get_hidden_ssid_list();
        connect_scan_ctxt_set_ssidlist(ctxt, ssidList ? ssidList : "");
        g_free(ssidList);
    }
}

void connect_background_restore_ping_ctxt(void)
{
    pingContext_t *ctxt;
    cmgrConnectType_t connectType;

    CN_LOGPRINTF("entry");

    ctxt = cmgr_get_ping_ctxt();
    if (ctxt)
    {
        connect_ping_ctxt_set_mode(ctxt, backgroundPing_e);
        connectType = connect_data_get_connect_type();
        connect_ping_ctxt_set_connect_type(ctxt, connectType);
   
        connect_ping_ctxt_set_done_callbacks(ctxt,
                    connect_background_on_connected,
                    connect_background_on_failed_all);

        connect_ping_ctxt_set_get_initial_profile(ctxt, 
                    connect_background_get_initial_profile);

        connect_ping_ctxt_set_access_network_profiles_callbacks(ctxt,
                                    connect_data_get_n_network_profiles,
                                    connect_data_get_network_profile,
                                    connect_data_select_next_regprofile);
    }
}

static void connect_background_scan_networks_done(void)
{
    int nProfiles;
    pingContext_t *ctxt;

    CN_LOGPRINTF("entry");
    
    nProfiles = connect_data_get_n_network_profiles();
    if (nProfiles > 0)
    {
        connect_background_restore_ping_ctxt();
        ctxt = cmgr_get_ping_ctxt();
        connect_ping_select_initial_profile(ctxt);
        connect_ping(ctxt, FALSE);
    }
}

int connect_background_get_initial_profile(void)
{
    gint regIndex, profileIndex = 0;

    // select last used profile or first profile
    regIndex = connect_data_get_last_connected_profile();
    CN_LOGPRINTF ("last connected profile [%d]", regIndex);

    if (main_get_use_last_connected () && regIndex >= 0)
    {
        // convert regIndex in registry into profileIndex in all profiles
        profileIndex = connect_data_get_profile_index(regIndex);
        CN_LOGPRINTF("regIndex[%d]-->profileIndex[%d]", 
                regIndex, profileIndex);
        if (profileIndex == -1)
        {
            profileIndex = 0;
        }
    }
    return profileIndex;
}

void connect_background_on_connected(int profileIndex)
{
    int regIndex = -1;

    // remember last successfull profile
    if (profileIndex != -1)
    {
        regIndex = connect_data_get_reg_index(profileIndex);
        if (regIndex != -1)
        {
            connect_data_set_last_connected_profile(regIndex);
        }
    }

    // start child process, if specified
    main_start_child_process();
}

void connect_background_on_failed_all(void)
{
    CN_LOGPRINTF("It's background connection, exit.");
    main_quit();
}

