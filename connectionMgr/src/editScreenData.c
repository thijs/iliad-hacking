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
 * \file editScreenData.c
 * \brief connectionMgr - edit screen data handling
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#include "connectionMgrLog.h"
#include "commonData.h"
#include "connectScreenData.h"
#include "editScreenData.h"

static connection_t g_edit_network_type = connection_undefined_t;

// the network profiles which will be shown on the edit profiles screen
static networkProfile_t*  g_network_profiles = NULL;
static int                g_n_network_profiles = 0;

void edit_data_init(void)
{
    CN_LOGPRINTF("entry");

   // initial
   connection_t networkType = connect_data_get_network_type();
   edit_data_set_network_type(networkType);
}

void edit_data_destroy(void)
{
    CN_LOGPRINTF("entry");
}

connection_t edit_data_get_network_type(void)
{
    CN_LOGPRINTF("ret=%d", g_edit_network_type);
 
    return g_edit_network_type;
}

gboolean edit_data_set_network_type(connection_t type)
{
    CN_LOGPRINTF("entry type[%d]", type);

    g_return_val_if_fail((type >= 0 && type <= connection_undefined_t), 
                         FALSE);

    g_edit_network_type = type;
 
    return TRUE;
}

void edit_data_init_network_profiles(void)
{
    CN_LOGPRINTF("entry");

    connection_t      networkType;
    int               nProfiles;
    networkProfile_t* pNetworks;

    // empty the old results
    edit_data_destory_network_profiles();

    // init the network profiles
    networkType = edit_data_get_network_type();
    nProfiles = common_data_init_network_profiles(networkType, &pNetworks);
    g_network_profiles = pNetworks;
    g_n_network_profiles = nProfiles;
}

void edit_data_destory_network_profiles(void)
{
    CN_LOGPRINTF("entry");

    common_data_destroy_network_profiles(g_network_profiles,
                                         g_n_network_profiles);
    g_network_profiles = NULL;
    g_n_network_profiles = 0;
}

void edit_data_store_network_profiles(void)
{
    CN_LOGPRINTF("entry");

    common_data_store();
}

int edit_data_get_n_network_profiles(void)
{
    CN_LOGPRINTF("entry [%d]", g_n_network_profiles);

    return g_n_network_profiles;
}

const networkProfile_t* edit_data_get_network_profile(guint profileIndex)
{
    CN_LOGPRINTF("entry profileIndex[%d]", profileIndex);

    networkProfile_t* ret = NULL;

    g_return_val_if_fail(g_network_profiles != NULL, NULL);
    g_return_val_if_fail(profileIndex < g_n_network_profiles, NULL);
    
    ret = &g_network_profiles[profileIndex];

    CN_LOGPRINTF("ret=%p", ret);

    return ret;
}

gboolean edit_data_set_network_profile(int profileIndex, 
                                       const networkProfile_t* pNetwork)
{
    int                        regIndex, nActiveIndex;
    regNetworkProfile_t*       settings = NULL;

    CN_LOGPRINTF("entry");

    g_return_val_if_fail(pNetwork != NULL, FALSE);
    if ((pNetwork->nRegProfiles > 0)
         && pNetwork->regSettingsList
         && ((pNetwork->nActiveIndex >= 0) 
             && (pNetwork->nActiveIndex < pNetwork->nRegProfiles)))
    {
        nActiveIndex = pNetwork->nActiveIndex;
        settings = pNetwork->regSettingsList[nActiveIndex];
    }
    g_return_val_if_fail(settings != NULL, FALSE);

    if (profileIndex == -1)
    {
        CN_LOGPRINTF("Adding a new profile.");
        regIndex = common_data_get_n_profiles();
    }
    else
    {
        regIndex = edit_data_get_reg_index(profileIndex);
        CN_LOGPRINTF("Setting %dth profile.", regIndex);
    }

    g_return_val_if_fail(regIndex >= 0, FALSE);
    common_data_set_profiledata(regIndex, settings, FALSE);

    // update the the list network profiles in the meanwhile
    edit_data_init_network_profiles();

    return TRUE;
}

int edit_data_get_profile_index(guint regIndex)
{
    CN_LOGPRINTF("entry regIndex[%d]", regIndex);
    int profileIndex = -1;

    profileIndex = common_data_get_profile_index(g_network_profiles, 
                                                 g_n_network_profiles, 
                                                 regIndex);

    return profileIndex;
}

int edit_data_get_reg_index(guint profileIndex)
{
    CN_LOGPRINTF("entry profileIndex[%d]", profileIndex);

    int regIndex = -1;
    
    regIndex = common_data_get_reg_index(g_network_profiles, 
                                         g_n_network_profiles, 
                                         profileIndex);
    return regIndex;
}

gboolean edit_data_reach_max_profiles(void)
{
    CN_LOGPRINTF("entry");

    connection_t networkType;
    gboolean     reachMax = FALSE;
   
    networkType = edit_data_get_network_type();
    if (   ((networkType == wired_t) 
             && common_data_reach_max_profiles(wired_t))
        || ((networkType == wireless_t) 
            && common_data_reach_max_profiles(wireless_t))
        || ((networkType == dialup_t) 
            && common_data_reach_max_profiles(dialup_t))    )
    {
        reachMax = TRUE;
    } 

    CN_LOGPRINTF("return reachMax=%d", reachMax);
    return reachMax;
}

void edit_data_remove_network_profile(int profileIndex)
{
    CN_LOGPRINTF("entry");

    int regIndex = edit_data_get_reg_index(profileIndex);
    
    if (regIndex != -1)
    {
        common_data_remove_profile(regIndex);
        connect_data_update_last_connected_profile_after_delete(regIndex);
    }

    // update the the list network profiles in the meanwhile
    //  edit_data_init_network_profiles();
}

