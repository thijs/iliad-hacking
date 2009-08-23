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
 * \file commonData.c
 * \brief connectionMgr - common data handling
 *                        getting and setting data from/into registry
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#include "config.h"

#include <string.h>
#include <errno.h>

#include <liberregxml/erregapi.h>

#include "connectionMgrLog.h"
#include "commonData.h"
#include "connectionMgr.h"

// local data
static regNetworkConfig_t*   g_netConfig = NULL;
static regNetworkProfile_t** g_networks  = NULL;
static regPCConfig_t*        g_pcConfig = NULL;
static regPCProfile_t**      g_pcshares = NULL;
static gboolean              g_configChanged = FALSE;

// local functions
static guint    common_data_add_profile(void);

////////////////////////////////////////////////////////////////////////////
// init/destory/store common data from/to registry
// -network profiles
// -pc share profiles
////////////////////////////////////////////////////////////////////////////
void common_data_init(void)
{
    CN_LOGPRINTF("entry");
 
    // Read all network profiles from registry
    erRegGetNetworkProfiles(&g_netConfig, &g_networks);

    // Read all PC profiles from registry
    erRegGetPCProfiles(&g_pcConfig, &g_pcshares);
}

void common_data_destroy(void)
{
    CN_LOGPRINTF("entry");

    if (g_netConfig)
    {
        erRegFreeNetworkProfiles(g_netConfig, g_networks);
        g_networks = NULL;
        g_netConfig = NULL;
    }

    if (g_pcConfig)
    {
        erRegFreePCProfiles(g_pcConfig, g_pcshares);
        g_pcConfig = NULL;
        g_pcshares = NULL;
    }
}

void common_data_store(void)
{
    CN_LOGPRINTF("entry");

    if ( g_configChanged == FALSE)
    {
        // nothing to save
        return;
    }

    gint index;
    char* theID;
    gboolean NWStored;
    regNetworkConfig_t* theNetworkConfig = NULL;
    regNetworkProfile_t* theNetworkProfile = NULL;
        
    // refresh registry data
    prepare_registry_write();

    // remove all network profiles from registry ...
    theNetworkConfig = erRegGetNetworkConfig();
    if (theNetworkConfig)
    {
        // remove old network profiles
        for (index = 0; index < theNetworkConfig->size; index++)
        {
            theID = theNetworkConfig->networkList[index];
            if (erRegRemoveNetworkProfile(theID) == FALSE)
            {
                CN_WARNPRINTF("could not remove %s", 
                        theNetworkConfig->networkList[index]);
            }
        }
        erRegFreeNetworkConfig(theNetworkConfig);

        // remove old netfork config
        erRegRemoveNetworkConfig();
    }
    else
    {
        CN_WARNPRINTF("could not retrieve theNetworkConfig");
    }

    // ... then write our network profiles to registry ...
    theNetworkConfig = g_new0(regNetworkConfig_t, 1);
    theNetworkConfig->size = g_netConfig->size;
    CN_STOREPRINTF("theNetworkConfig->size %d", theNetworkConfig->size);
    theNetworkConfig->networkList = g_new0(gchar*, 
                        (theNetworkConfig->size + 1));

    for (index = 0 ; index < g_netConfig->size ; index++)
    {
        theNetworkProfile = g_networks[index];

        if (theNetworkProfile == NULL)
        {
            CN_STOREPRINTF("theNetworkProfile = NULL");
        }
        else
        {
            theID = g_strdup_printf("NW_%d", index);
            NWStored = erRegSetNetworkProfile(theID, theNetworkProfile);

            if (NWStored)
            {
                CN_STOREPRINTF("id %s was stored", theID);
                theNetworkConfig->networkList[index] = g_strdup(theID);
            }
            else
            {
                CN_STOREPRINTF("id %s was NOT stored", theID);
                if (theNetworkConfig->size > 0)
                {
                    theNetworkConfig->size = theNetworkConfig->size - 1;
                }
            }
        }
    }
    if (theNetworkConfig->size > 0)
    {
        NWStored = erRegSetNetworkConfig(theNetworkConfig);
        CN_STOREPRINTF("erRegSetNetworkConfig() returned %s", 
                       (NWStored == TRUE) ? "TRUE" : "FALSE");
    }
    erRegFreeNetworkConfig(theNetworkConfig);

    //
    // ... and store registry to disk
    do_registry_write();
    CN_STOREPRINTF("Stored registry");
    g_configChanged = FALSE;
}

////////////////////////////////////////////////////////////////////////////
// get/add/remove the 'index'th regNetworkProfile_t 
// from/to network profile list in memory
// get the 'index'th regPCProfile_t from pc share profile list in memory 
////////////////////////////////////////////////////////////////////////////
int common_data_get_n_profiles(void)
{
    CN_LOGPRINTF("entry");

    g_return_val_if_fail((g_netConfig != NULL), 0);
    return g_netConfig->size;
}

static int common_data_get_n_profiles_by_networktype(connection_t networkType)
{
    CN_LOGPRINTF("entry networkType[%d]", networkType);

    int                        i, nProfiles = 0;
    int                        nRegNetworks;
    const regNetworkProfile_t* pRegNetwork; 

    nRegNetworks = common_data_get_n_profiles();
    for (i = 0; i < nRegNetworks; i++)
    {
        pRegNetwork = common_data_get_profiledata(i);
        if (pRegNetwork && pRegNetwork->connection == networkType)
        {
            nProfiles++;
        }
    }
    CN_LOGPRINTF("nProfiles = %d", nProfiles);
    return nProfiles;
}

const regNetworkProfile_t* common_data_get_profiledata(const gint index)
{
    CN_LOGPRINTF("entry: index [%d]", index);

    g_return_val_if_fail((g_netConfig != NULL), NULL);

    if (index < g_netConfig->size)
    {
        return g_networks[index];
    }
    else
    {
        return NULL;
    }
}

static guint common_data_add_profile()
{
    CN_LOGPRINTF("entry");

    guint index = g_netConfig->size;

    // add one network ID
    gchar** newList = g_renew(gchar*, g_netConfig->networkList, index + 1);
    g_netConfig->networkList = newList;
    g_netConfig->networkList[index] = g_strdup_printf("NW_%d", index);

    // add network setting
    regNetworkProfile_t** newSettings = g_renew(regNetworkProfile_t*, 
                                                g_networks, index + 1);
    g_networks = newSettings;
    g_networks[index] = NULL;
    g_netConfig->size = index + 1;

    // report number of profiles
    return g_netConfig->size;
}

guint common_data_remove_profile(guint index)
{
    CN_LOGPRINTF("entry: index [%d]", index);
    
    int i;
    guint num_profiles = g_netConfig->size;
    g_return_val_if_fail((index < num_profiles), num_profiles);

    // remove the last network ID
    g_free( g_netConfig->networkList[num_profiles-1] );
    g_netConfig->networkList[num_profiles-1] = NULL;

    // remove the specified network setting
    g_free( g_networks[index] );
    for (i = index ; i < num_profiles - 1 ; i++)
    {
        g_networks[i] = g_networks[i+1];
    }
    g_networks[num_profiles-1] = NULL;

    // update profile count and report
    g_netConfig->size = num_profiles - 1;

    g_configChanged = TRUE;

    return g_netConfig->size;
}

const regPCProfile_t* common_data_get_pcsharedata(const gint index)
{
    CN_LOGPRINTF("entry: index [%d]", index);

    g_return_val_if_fail((g_pcConfig != NULL), NULL);

    if (index < g_pcConfig->size)
    {
        return g_pcshares[index];
    }
    else
    {
        return NULL;
    }
}

gboolean common_data_reach_max_profiles(connection_t networkType)
{
    CN_LOGPRINTF("entry networkType[%d]", networkType);
    int      num, max;

    switch (networkType)
    {
        case wired_t:
            max = MAX_WIRED_PROFILES;
            break;
        case wireless_t:
            max = MAX_WIRELESS_PROFILES;
            break;
        case dialup_t:
            max = MAX_DIALUP_PROFILES;
            break;
        default:
            CN_WARNPRINTF("unknow networkType[%d]", networkType);
            max = 0;
            break;
    }

    num = common_data_get_n_profiles_by_networktype(networkType);
    CN_LOGPRINTF("num[%d] >= max[%d] ?", num, max);
    return ((num >= max) ? TRUE : FALSE);
}

////////////////////////////////////////////////////////////////////////////
// set the 'index'th regNetworkProfile_t to network profile list in memory
// and save to registry file
////////////////////////////////////////////////////////////////////////////
gboolean common_data_set_profiledata(gint index, 
                                     const regNetworkProfile_t* setting, 
                                     const gboolean preferred)
{
    gboolean addNew = FALSE;
    gboolean reachMax = FALSE;
    gboolean ok = FALSE;
    regNetworkProfile_t* tmp_setting;

    CN_LOGPRINTF("entry: index [%d]", index);

    g_return_val_if_fail((g_netConfig != NULL), ok);
    g_return_val_if_fail((index <= g_netConfig->size), ok);
    
    // add new profile if needed
    while (g_netConfig->size <= index)
    {
        // before adding the new profile, check whether can add it or not
        g_return_val_if_fail(setting != NULL, ok);
        reachMax = common_data_reach_max_profiles(setting->connection);
        g_return_val_if_fail(reachMax == FALSE, ok);
        
        common_data_add_profile();
        addNew = TRUE;
    }

    // make preferred if needed
    if (preferred  &&  index < g_netConfig->size)
    {
        tmp_setting = g_networks[index];
        while (index > 0)
        {
            g_networks[index] = g_networks[index-1];
            --index;
        }
        g_networks[index] = tmp_setting;
    }
    
    if (   setting == NULL
        || setting->name == NULL
        || setting->name[0] == '\0')
    {
        // empty setting
        common_data_remove_profile(index);
    }
    else
    {
        prepare_registry_write();

        // store profile
        gchar* ID = g_netConfig->networkList[index];
        if (   erRegSetNetworkProfile(ID, (regNetworkProfile_t*)setting) 
            && erRegValidate() )
        {
            // refresh local administration if succeeded
            erRegFreeNetworkProfile( g_networks[index] );
            g_networks[index] = erRegGetNetworkProfile(ID);
            ok = TRUE;
        }
        else
        {
            CN_LOGPRINTF("Set profile [%d] [%s] failed", index, ID);
            if (addNew)
            {
                // undo the changes if failed
                common_data_remove_profile(index);
            }
        }

        do_registry_write();
    }

    g_configChanged = TRUE;
    return ok;
}

////////////////////////////////////////////////////////////////////////////
// the functions of operating the list of networkProfile_t
////////////////////////////////////////////////////////////////////////////
static networkProfile_t* new_network_profile_t(int nNetworks)
{
    networkProfile_t* pNetworksList = NULL; 
    networkProfile_t* pNetwork;
    int               i;

    if (nNetworks > 0)
    {
        pNetworksList = g_new0(networkProfile_t, nNetworks);
        if (pNetworksList)
        {
            for (i = 0; i < nNetworks; i++)
            {
                pNetwork = &pNetworksList[i];
//                pNetwork->scanSettings = NULL;
//                pNetwork->regSettingsList = NULL;
//                pNetwork->regIndexList = NULL;
//                pNetwork->nRegProfiles = 0;
                pNetwork->nActiveIndex = -1;
            }
        }
    }

    return pNetworksList;
}

static void free_network_profile_t(networkProfile_t* pNetworkList, 
                                   int nNetworks)
{
    int                  i, j;
    networkProfile_t*    pNetwork;
   
    if ((nNetworks > 0) && (pNetworkList != NULL))  
    {
        for (i = 0; i < nNetworks; i++)
        {
            pNetwork = &pNetworkList[i];

            // free the members related with profiles in the registry
            if (pNetwork->nRegProfiles)
            {
                // free 'regSettingsList'
                if (pNetwork->regSettingsList)
                {
                    for (j = 0; j < pNetwork->nRegProfiles; j++)
                    {
                        erRegFreeNetworkProfile(pNetwork->regSettingsList[j]);
                    }
                    g_free(pNetwork->regSettingsList);
                }
                
                // free 'regIndexList'
                if (pNetwork->regIndexList)
                {
                    g_free(pNetwork->regIndexList);
                }
            }

            // free scanSettings
            free_network_spec(pNetwork->scanSettings);
        }

        g_free(pNetworkList);
    }
}

int common_data_init_network_profiles(connection_t networkType, 
                                      networkProfile_t** pNetworks)
{
    int                        i, profileIndex;
    int                        nRegNetworks;
    const regNetworkProfile_t* pRegNetwork; 
    networkProfile_t*          pNetwork;
    int                        nNetworks;
    networkProfile_t*          pNetworksList;

    CN_LOGPRINTF("entry networkType[%d]", networkType);
   
    // get how many profiles in registry
    nRegNetworks = common_data_get_n_profiles();
    if (nRegNetworks == 0)
    {
        CN_WARNPRINTF("No network profiles in registry.");
        return 0;
    }
    
    // get how many 'networkType' profiles 
    nNetworks = common_data_get_n_profiles_by_networktype(networkType);
   
    // sepcial case
    if (nNetworks == 0)
    {
        CN_WARNPRINTF("Can't find any [%d] network profiles.", networkType);
        return 0;
    }

    // get the 'networkType' profiles
    pNetworksList = new_network_profile_t(nNetworks);
    g_return_val_if_fail(pNetworksList != NULL, 0);

    profileIndex = 0;
    for (i = 0; i < nRegNetworks; i++)
    {
        pRegNetwork = common_data_get_profiledata(i);
        if (pRegNetwork && pRegNetwork->connection == networkType)
        {
            pNetwork = &pNetworksList[profileIndex];
            
            // scanSettings
            pNetwork->scanSettings = NULL;
            // regSettingsList
            pNetwork->regSettingsList = g_new0(regNetworkProfile_t*, 1);
            pNetwork->regSettingsList[0] = erRegDupNetworkProfile(pRegNetwork);
            // regIndexList
            pNetwork->regIndexList = g_new0(int, 1);
            pNetwork->regIndexList[0] =  i;
            // nRegProfiles
            pNetwork->nRegProfiles = 1;
            // nActiveIndex
            pNetwork->nActiveIndex = 0;

            profileIndex++;
        }
    }
   
    CN_LOGPRINTF("return nNetworks=%d", nNetworks);
    *pNetworks = pNetworksList;
    return nNetworks;
}

void common_data_destroy_network_profiles(networkProfile_t* pNetworks, 
                                          guint nProfiles)
{
    CN_LOGPRINTF("entry");
    free_network_profile_t(pNetworks, nProfiles);
}

int common_data_get_profile_index(networkProfile_t* pNetworks, 
                                  int nProfiles, 
                                  int regIndex)
{
    CN_LOGPRINTF("entry");

    const networkProfile_t    *settings;
    int                       i, j;
    int                       profileIndex = -1;

    if ((nProfiles > 0) && (pNetworks))
    {
        for (i = 0; i < nProfiles; i++)
        {
            settings = &pNetworks[i];
            if (settings 
                && settings->nRegProfiles
                && settings->regSettingsList
                && settings->regIndexList)
            {
                for (j = 0; j < settings->nRegProfiles; j++)
                {
                    if (settings->regIndexList[j] == regIndex)
                    {
                        profileIndex = i;
                        break;
                    }
                }
            }
        }
    }

    return profileIndex;
}

int common_data_get_reg_index(networkProfile_t* pNetworks,
                              int nProfiles,
                              int profileIndex)
{
    CN_LOGPRINTF("entry");

    const networkProfile_t    *settings;
    int                       regIndex = -1;

    if ((nProfiles > 0) && (pNetworks))
    {
        if ((profileIndex >= 0) && (profileIndex < nProfiles))
        {
            settings = &pNetworks[profileIndex];
            if (settings 
                && (settings->nRegProfiles > 0)
                && settings->regSettingsList
                && settings->regIndexList)
            {
                if ((settings->nActiveIndex >= 0) 
                     && (settings->nActiveIndex < settings->nRegProfiles))
                {
                    regIndex = settings->regIndexList[settings->nActiveIndex];
                }
            }
        }
    }

    return regIndex;
}

