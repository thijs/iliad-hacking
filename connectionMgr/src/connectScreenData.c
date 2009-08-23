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
 * \file connectScreenData.c
 * \brief connectionMgr - connect screen data handling
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "connectionMgrLog.h"
#include "connectionMgr.h"
#include "connectScreenData.h"

// the last connect and auto connect settings
static gint              g_last_connected_profile_ids        = 0;
static gint              g_last_connected_profile_ids_stored = 0;
static gint              g_last_connected_profile_pc         = 0;
static gint              g_last_connected_profile_pc_stored  = 0;
static cmgrConnectType_t g_background_connect_type = cmgrConnectIDS_e;
static cmgrConnectType_t g_manual_connect_type = cmgrConnectUnknown_e;
static cmgrConnectType_t g_manual_connect_type_stored = cmgrConnectUnknown_e;
static connection_t g_network_type = connection_undefined_t;

static gboolean g_ac_wlan = TRUE;

// the network profiles which will be shown on the connect profiles screen
static networkProfile_t*  g_network_profiles = NULL;
static int                g_n_network_profiles = 0;

// internally functions declarations
static gboolean connect_data_read_last_connect(void);
static gboolean connect_data_save_last_connect(void);
static gboolean connect_data_read_auto_connect(void);
static gboolean connect_data_read_ac_wlan(void);

static void connect_data_calc_last_network_type(void);

static void get_wireless_profiles(network_spec_t* pScanNetworks, 
                                  int nScanNetworks);
static int get_regindices_by_ssid(const char* ssid, int** indexList);
static int compare_wireless_profiles(const void * p1, const void * p2);

////////////////////////////////////////////////////////////////////////////
// init/destory the data of lastconnect and autoconnect 
////////////////////////////////////////////////////////////////////////////
void connect_data_init(void)
{
    CN_LOGPRINTF("entry");

    connect_data_read_last_connect();
    connect_data_read_auto_connect();
    connect_data_read_ac_wlan();
    connect_data_calc_last_network_type();
}

void connect_data_destroy(void)
{
    CN_LOGPRINTF("entry");
}

gboolean connect_data_store(void)
{
    gboolean ret = TRUE;  // TRUE = ok, so far

    if (main_get_connect_type() == cmgrConnectUnknown_e)
    {
        // connect type NOT enforced by command line: save connect type
        ret = connect_data_save_last_connect();
    }
    else
    {
        // connect type enforced by command line option: do NOT save
    }

    CN_LOGPRINTF("ret=%d", ret);
    return ret;
}

////////////////////////////////////////////////////////////////////////////
// get auto connect from registry 
////////////////////////////////////////////////////////////////////////////
static gboolean connect_data_read_auto_connect(void)
{
    CN_LOGPRINTF("entry");

    regAutoConnect_t* autoconnect;

    // Read background connect mode
    if (main_get_background())
    {
        autoconnect = erRegGetAutoConnect();
        if (autoconnect)
        {
            if (autoconnect->backgroundConnectTo >= 1)
            {
               g_background_connect_type = cmgrConnectPCShare_e;
            }
            erRegFreeAutoConnect(autoconnect);
        }
    }

    return TRUE;
}

static gboolean connect_data_read_ac_wlan(void)
{
    CN_LOGPRINTF("entry");

    g_ac_wlan = erRegGetAutoConnectWlan();

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////
// get/set last connect from registry 
// and store the results to global variables
////////////////////////////////////////////////////////////////////////////
static gboolean connect_data_read_last_connect(void)
{
    CN_LOGPRINTF("entry");

    regLastConnect_t* theLastConnect = NULL;
    gboolean          ret = TRUE;

    // get last connect from registry and
    // store the settings into global variables 
    theLastConnect = erRegGetLastConnect();
    if (theLastConnect)
    {
        // profile IDs
        g_last_connected_profile_ids = theLastConnect->profileConnectedIDS;
        g_last_connected_profile_pc  = theLastConnect->profileConnectedPC;
        // connect type
        if (ids_t == theLastConnect->manualConnectType)
        {
            g_manual_connect_type = cmgrConnectIDS_e;
        }
        else if (pc_t == theLastConnect->manualConnectType)
        {
            g_manual_connect_type = cmgrConnectPCShare_e;
        }
        else
        {
            g_manual_connect_type = cmgrConnectUnknown_e;
        }

        erRegFreeLastConnect(theLastConnect);
    }
    else
    {
        CN_LOGPRINTF("erRegGetLastConnect returns NULL");
        g_last_connected_profile_ids = -1;
        g_last_connected_profile_pc = -1;
        g_manual_connect_type = cmgrConnectUnknown_e;
        g_network_type = connection_undefined_t;
    }
    
    g_last_connected_profile_ids_stored = g_last_connected_profile_ids;
    g_last_connected_profile_pc_stored = g_last_connected_profile_pc;
    g_manual_connect_type_stored = g_manual_connect_type;

    CN_LOGPRINTF("leave: return [%d]", ret);
    return ret;
}

// calculate the last network type
static void connect_data_calc_last_network_type(void)
{
    int          index = -1;
    connection_t networkType = wireless_t;// default
    const regNetworkProfile_t* pRegNetwork;
    
    // get it from the registry
    switch ( connect_data_get_connect_type() )
    {
        case cmgrConnectIDS_e:
            index = g_last_connected_profile_ids;
            break;
        case cmgrConnectPCShare_e:
            index = g_last_connected_profile_pc;
            break;
        default:
            break;
    }
    
    if (index != -1)
    {
        pRegNetwork = common_data_get_profiledata(index);
        if (pRegNetwork)
        {
            networkType = pRegNetwork->connection;
        }
    }
    
    if (main_get_wifi_disabled() && networkType == wireless_t )
    {
        // overrule default connection type when no wifi present
        networkType = wired_t;
    }

    if (main_get_dialup_disabled() && networkType == dialup_t )
    {
        // overrule default connection type when no dialup present
        networkType = wired_t;
    }
    
    CN_LOGPRINTF("g_network_type[%d]", networkType);
    g_network_type = networkType;
}

static gboolean connect_data_save_last_connect(void)
{
    CN_LOGPRINTF("entry");

    gboolean         ret = FALSE;
    regLastConnect_t theLastConnect;

    if (g_last_connected_profile_ids != g_last_connected_profile_ids_stored
        || g_last_connected_profile_pc != g_last_connected_profile_pc_stored
        || g_manual_connect_type != g_manual_connect_type_stored)
    {
        prepare_registry_write();

        // write new settings to registry 
        theLastConnect.profileConnectedIDS = g_last_connected_profile_ids;
        theLastConnect.profileConnectedPC  = g_last_connected_profile_pc;
        if (g_manual_connect_type == cmgrConnectIDS_e)
        {
            theLastConnect.manualConnectType = ids_t;
        }
        else if (g_manual_connect_type == cmgrConnectPCShare_e)
        {
            theLastConnect.manualConnectType = pc_t;
        }
        else
        {
            theLastConnect.manualConnectType = ids_t;
        }
       
        ret = erRegSetLastConnect(&theLastConnect);
        if (ret)
        {
            // update the global variables
            g_last_connected_profile_ids_stored = g_last_connected_profile_ids;
            g_last_connected_profile_pc_stored  = g_last_connected_profile_pc;
            g_manual_connect_type_stored = g_manual_connect_type;
        }

        do_registry_write();
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////
// get/set background auto connect and last connect 
// from/to global variables 
////////////////////////////////////////////////////////////////////////////
gint connect_data_get_last_connected_profile(void)
{
    CN_LOGPRINTF("entry");

    gint regIndex = -1;
    guint nRegNetworks;
 
    nRegNetworks = common_data_get_n_profiles();

    switch ( connect_data_get_connect_type() )
    {
        case cmgrConnectIDS_e:
            if (   g_last_connected_profile_ids >= 0
                && g_last_connected_profile_ids < nRegNetworks)
            {
                regIndex = g_last_connected_profile_ids;
            }
            break;

        case cmgrConnectPCShare_e:
            if (   g_last_connected_profile_pc >= 0
                && g_last_connected_profile_pc < nRegNetworks)
            {
                regIndex = g_last_connected_profile_pc;
            }
            break;

        default:
            ; // ignore
    }

    CN_LOGPRINTF("Return [%d]", regIndex);
    return regIndex;
}

gboolean connect_data_set_last_connected_profile(gint regIndex)
{
    guint    nRegNetworks;
    gboolean retVal = FALSE;

    CN_LOGPRINTF("entry: regIndex [%d]", regIndex);

    nRegNetworks = common_data_get_n_profiles();

    switch ( connect_data_get_connect_type() )
    {
        case cmgrConnectIDS_e:
            if (regIndex >= 0 && regIndex < nRegNetworks)
            {
                g_last_connected_profile_ids = regIndex;
                retVal = TRUE;
            }
            break;

        case cmgrConnectPCShare_e:
            if (regIndex >= 0 && regIndex < nRegNetworks)
            {
                g_last_connected_profile_pc = regIndex;
                retVal = TRUE;
            }
            break;

        default:
            ; // ignore
    }

    CN_LOGPRINTF("Return [%d]", retVal);
    return retVal;
}

cmgrConnectType_t connect_data_get_connect_type(void)
{
    CN_LOGPRINTF("entry");

    if ( main_get_background() )
    {
        return g_background_connect_type;
    }
    else
    {
        return g_manual_connect_type;
    }
}

gboolean connect_data_set_connect_type(cmgrConnectType_t type)
{
    CN_LOGPRINTF("entry: type [%d]", type);

    g_return_val_if_fail((type >= 0  &&  type < cmgrConnectUndef_e), 
                         FALSE);
    
    if ( main_get_background() )
    {
        g_background_connect_type = type;
    }
    else
    {
        g_manual_connect_type = type;
    }
    return TRUE;
}

connection_t connect_data_get_network_type(void)
{
    CN_LOGPRINTF("ret=%d", g_network_type);
    return g_network_type;
}

gboolean connect_data_set_network_type(connection_t type)
{
    CN_LOGPRINTF("entry type[%d]", type);

    g_return_val_if_fail((type >= 0 && type < connection_undefined_t), 
                         FALSE);

    g_network_type = type;
 
    return TRUE;
}

gboolean connect_data_get_ac_wlan(void)
{
    CN_LOGPRINTF("ret=%d", g_ac_wlan);
    return g_ac_wlan;
}

// update the last connected profile index after deleting a profile
void connect_data_update_last_connected_profile_after_delete(gint delRegIndex)
{
    guint    nRegNetworks;

    CN_LOGPRINTF("entry: delRegIndex [%d]", delRegIndex);

    nRegNetworks = common_data_get_n_profiles();
    if (delRegIndex >= 0 && delRegIndex <= nRegNetworks)
    {
        CN_LOGPRINTF("before update: last_connected_profile_ids[%d]", 
                     g_last_connected_profile_ids);

        CN_LOGPRINTF("before update: last_connected_profile_pc[%d]", 
                     g_last_connected_profile_pc);

        // update the last connected profile to ids
        if (g_last_connected_profile_ids != -1)
        {
            if (delRegIndex < g_last_connected_profile_ids)
            {
                g_last_connected_profile_ids--;
            }
            else if (delRegIndex == g_last_connected_profile_ids)
            {
                g_last_connected_profile_ids = -1;
            }
        }

        // update the last connected profile to pc
        if (g_last_connected_profile_pc != -1)
        {
            if (delRegIndex < g_last_connected_profile_pc)
            {
                g_last_connected_profile_pc--;
            }
            else if (delRegIndex == g_last_connected_profile_pc)
            {
                g_last_connected_profile_pc = -1;
            }
        }

        CN_LOGPRINTF("after update: last_connected_profile_pc[%d]", 
                     g_last_connected_profile_pc);

        CN_LOGPRINTF("after update: last_connected_profile_ids[%d]", 
                     g_last_connected_profile_ids);

    }
}

////////////////////////////////////////////////////////////////////////////
// get pc share settings
////////////////////////////////////////////////////////////////////////////
const regPCProfile_t* connect_data_get_pcsharedata(void)
{
    CN_LOGPRINTF("entry");

    const regPCProfile_t* ret = common_data_get_pcsharedata(0);
    
    CN_LOGPRINTF("return ret [%p]", ret);
    
    return ret;
}
////////////////////////////////////////////////////////////////////////////
// get ecd url
////////////////////////////////////////////////////////////////////////////
gchar* connect_data_get_ecd_url(void)
{
    CN_LOGPRINTF("entry");

    return g_strdup(REDIRECT_URL);
}

////////////////////////////////////////////////////////////////////////////
// deal with g_network_profiles/g_n_network_profiles
//
// input-  the profiles in registry 
//         the profiles scanned automatically(pScanNetworks, nScanNetworks)
// output- the network profiles will be shown on connect profiles screen
//         (g_network_profiles/g_n_network_profiles)
//
// notes, we assumpt pScanNetworks is already sorted 
// by signal/encryption type/ssid
////////////////////////////////////////////////////////////////////////////
void connect_data_init_network_profiles(connection_t networkType, 
                                        network_spec_t* pScanNetworks, 
                                        int nScanNetworks)
{
    CN_LOGPRINTF("entry networkType[%d]pScanNetworks[%p]nScanNetworks[%d]",
                  networkType, pScanNetworks, nScanNetworks);
 
     // empty the old results
     connect_data_destroy_network_profiles();

     switch (networkType)
     {
         case wireless_t:
             get_wireless_profiles(pScanNetworks, nScanNetworks);
             break;
         case wired_t:
         case dialup_t:
             g_n_network_profiles = common_data_init_network_profiles(networkType, 
                                                      &g_network_profiles);
             break;
         default:
             CN_WARNPRINTF("unknown networkType[%d]", networkType);
             break;
     }
}

void connect_data_destroy_network_profiles(void)
{
    CN_LOGPRINTF("entry");

    common_data_destroy_network_profiles(g_network_profiles, 
                                         g_n_network_profiles);
    g_network_profiles = NULL;
    g_n_network_profiles = 0;
}

int connect_data_get_n_network_profiles(void)
{
    CN_LOGPRINTF("entry [%d]", g_n_network_profiles);

    return g_n_network_profiles;
}

const networkProfile_t* connect_data_get_network_profile(int profileIndex)
{
    CN_LOGPRINTF("entry profileIndex[%d]", profileIndex);

    networkProfile_t* ret = NULL;

    g_return_val_if_fail(g_network_profiles != NULL, NULL);
    g_return_val_if_fail(profileIndex < g_n_network_profiles, NULL);
    
    ret = &g_network_profiles[profileIndex];

    CN_LOGPRINTF("ret=%p", ret);

    return ret;
}

static void get_wireless_profiles(network_spec_t* pScanNetworks, 
                                  int nScanNetworks)
{
    int                        i, j, regIndex;
    const regNetworkProfile_t* pRegNetwork;
    networkProfile_t*          pNetwork;

    CN_LOGPRINTF("pScanNetworks[%p], nScanNetworks[%d]", 
                  pScanNetworks, nScanNetworks);

    // tweak them together
    if ((pScanNetworks == NULL) || (nScanNetworks == 0))
    {
        CN_WARNPRINTF("No wireless network profiles scanned "
                      "automatically. ");
     
        // only show SSIDs in the air
        g_n_network_profiles = 0;
        g_network_profiles = NULL;
    }
    else if ((common_data_get_n_profiles() == 0))
    {
        CN_WARNPRINTF("No any network profiles in the registry.");

        g_n_network_profiles = nScanNetworks;
        g_network_profiles = g_new0(networkProfile_t, nScanNetworks);
        g_return_if_fail(g_network_profiles != NULL);

        for (i = 0; i < nScanNetworks; i++)
        {
            pNetwork = &g_network_profiles[i];
            pNetwork->scanSettings = dup_network_spec(&pScanNetworks[i]);
            pNetwork->regSettingsList = NULL;
            pNetwork->regIndexList = NULL;
            pNetwork->nRegProfiles = 0;
            pNetwork->nActiveIndex = -1;
        }
    }
    else
    {
        CN_LOGPRINTF("Found profiles in registry "
                     "and profiles scanned automatically.");
        
        g_n_network_profiles = nScanNetworks;
        g_network_profiles = g_new0(networkProfile_t, nScanNetworks);
        g_return_if_fail(g_network_profiles != NULL);

        for (i = 0; i < nScanNetworks; i++)
        {
            pNetwork = &g_network_profiles[i];
            pNetwork->scanSettings = dup_network_spec(&pScanNetworks[i]);
            pNetwork->nRegProfiles = get_regindices_by_ssid(pScanNetworks[i].ssid, 
                                                &pNetwork->regIndexList);
            if (pNetwork->nRegProfiles > 0)
            {
                pNetwork->regSettingsList = g_new0(regNetworkProfile_t*, 
                                                   pNetwork->nRegProfiles);
                g_return_if_fail(pNetwork->regSettingsList != NULL);

                for (j = 0; j < pNetwork->nRegProfiles; j++)
                {
                    regIndex = pNetwork->regIndexList[j];
                    pRegNetwork = common_data_get_profiledata(regIndex);
                    pNetwork->regSettingsList[j] = erRegDupNetworkProfile(pRegNetwork);
                }
                pNetwork->nActiveIndex = 0;
            }
            else
            {
                pNetwork->regSettingsList = NULL;
                pNetwork->regIndexList = NULL;
                pNetwork->nRegProfiles = 0;
                pNetwork->nActiveIndex = -1;
            }
        }

        // sort them, profiles in registry are on the top 
        qsort(g_network_profiles, g_n_network_profiles, 
             sizeof(networkProfile_t), compare_wireless_profiles);
    }
}

// return how many profiles have ssid
// supports one ssid has multiple profiles in registry
static int get_regindices_by_ssid(const char* ssid, int** indexList)
{
    CN_LOGPRINTF("entry ssid=%s", ssid);

    const regNetworkProfile_t* pRegNetwork;
    int nRegNetworks;
    int i;
    int counter = 0; // number if profiles found
    int *regIndices = NULL; // indexlist for profiles found
    
    nRegNetworks = common_data_get_n_profiles();
    // how many profiles have ssid
    for (i = 0; i < nRegNetworks; i++)
    {
        pRegNetwork = common_data_get_profiledata(i);
        if (pRegNetwork 
            && pRegNetwork->wirelessSettings
            && pRegNetwork->wirelessSettings->SSID 
            && ssid 
            && !strcmp(pRegNetwork->wirelessSettings->SSID, ssid))
        {
            counter++;
        }
    }

    if (counter > 0)
    {
        // malloc the memory
        regIndices = g_new0(int, counter);
        g_return_val_if_fail(regIndices != NULL, 0);
       
        // get the regIndices
        counter = 0;
        for (i = 0; i < nRegNetworks; i++)
        {
            pRegNetwork = common_data_get_profiledata(i);
            if (pRegNetwork 
                && pRegNetwork->wirelessSettings
                && pRegNetwork->wirelessSettings->SSID 
                && ssid 
                && !strcmp(pRegNetwork->wirelessSettings->SSID, ssid))
            {
                regIndices[counter] = i;
                counter++;
            }
        }
    }
    
    CN_LOGPRINTF("leave: counter=%d, regIndices=%p", counter, regIndices);
    *indexList = regIndices;
    return counter;
}

// sort key is whether there's counterpart in registry
static int compare_wireless_profiles(const void * p1, const void * p2)
{
    networkProfile_t *pNetwork1, * pNetwork2;
    int               ret = 0;

    pNetwork1 = (networkProfile_t*)p1;
    pNetwork2 = (networkProfile_t*)p2;
    
    ret = (pNetwork1->nRegProfiles - pNetwork2->nRegProfiles);
    CN_LOGPRINTF("ret=%d", ret);
    // make the result of qsort to be descending
    ret = 0 - ret;
    return ret;
}

// convert regIndex into profileIndex
int connect_data_get_profile_index(int regIndex) 
{
   int profileIndex = -1;

   profileIndex = common_data_get_profile_index(g_network_profiles, 
                                                g_n_network_profiles, 
                                                regIndex);

   return profileIndex;
}

int connect_data_get_reg_index(int profileIndex)
{
    CN_LOGPRINTF("entry profileIndex[%d]", profileIndex);

    int regIndex = -1;
    
    regIndex = common_data_get_reg_index(g_network_profiles, 
                                         g_n_network_profiles, 
                                         profileIndex);
    return regIndex;
}

gboolean connect_data_reach_max_profiles(void)
{
    CN_LOGPRINTF("entry");

    connection_t networkType;
    gboolean     reachMax = FALSE;
   
    networkType = connect_data_get_network_type();
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

// using the last profile in registry to update the 'pNetwork'
void connect_data_update_network_profile_with_last(networkProfile_t* pNetwork) 
{
    CN_LOGPRINTF("entry pNetwork[%p]", pNetwork);

    int                        nRegProfiles;
    const regNetworkProfile_t* pRegNetwork;

    g_return_if_fail(pNetwork != NULL);
    
    nRegProfiles = common_data_get_n_profiles();
    g_return_if_fail(nRegProfiles > 0);
    
    pRegNetwork = common_data_get_profiledata(nRegProfiles-1);
    g_return_if_fail(pRegNetwork != NULL);
    
    pNetwork->nRegProfiles = 1;
    // regIndexList
    pNetwork->regIndexList = g_new0(int, 1);
    pNetwork->regIndexList[0] = nRegProfiles - 1;
    // regSettingsList 
    pNetwork->regSettingsList = g_new0(regNetworkProfile_t*, 1);
    pNetwork->regSettingsList[0] = erRegDupNetworkProfile(pRegNetwork); 
    // nActiveIndex
    pNetwork->nActiveIndex = 0;
}

regNetworkProfile_t* network_spec_to_reg_network(network_spec_t* scanSettings)
{
    regNetworkProfile_t* regSettings = NULL;

    if (scanSettings)
    {
        regSettings = g_new0(regNetworkProfile_t, 1);
        if (regSettings)
        {
            regSettings->name = g_strdup(scanSettings->ssid);
            regSettings->connection = wireless_t;
            regSettings->proxy = FALSE;
            regSettings->addressMode = dhcp_t;
            regSettings->wirelessSettings = g_new0(regWirelessSetting_t, 1);
            regSettings->wirelessSettings->SSID = g_strdup(scanSettings->ssid);
            regSettings->wirelessSettings->encrType = scanSettings->encryption;
            regSettings->wirelessSettings->encrKey = g_strdup("");
            regSettings->dialupSettings = NULL;
            regSettings->proxySettings = NULL;
            regSettings->ipSettings = NULL;
        }
    }

    return regSettings;
}

char* connect_data_get_hidden_ssid_list(void)
{
    char*    ssidList;
    int      len;
    gboolean first;
    int      i, nRegNetworks;
    const regNetworkProfile_t* pRegNetwork; 

    // get how many profiles in the registry
    nRegNetworks = common_data_get_n_profiles();

    // get the len of ssidList
    len = 0;
    first = TRUE;
    for (i = 0; i < nRegNetworks; i++)
    {
        pRegNetwork = common_data_get_profiledata(i);
        if (pRegNetwork 
            && pRegNetwork->connection == wireless_t
            && pRegNetwork->wirelessSettings
            && (pRegNetwork->wirelessSettings->broadcast == FALSE)
            && pRegNetwork->wirelessSettings->SSID)
        {
            if (!first)
            {
                // the number of spaces for the seperator
                len++;
            }
            // add the length of each ssid 
            len += strlen(pRegNetwork->wirelessSettings->SSID);
            first = FALSE;
        }
    }
    // for the '\0'
    len += 1; 
    
    // malloc the memory
    ssidList = g_new0(char, len);
    g_return_val_if_fail(ssidList != NULL, 0);

    first = TRUE;
    // set the value of ssidList
    for (i = 0; i < nRegNetworks; i++)
    {
        pRegNetwork = common_data_get_profiledata(i);
        if (pRegNetwork 
            && pRegNetwork->connection == wireless_t
            && pRegNetwork->wirelessSettings
            && (pRegNetwork->wirelessSettings->broadcast == FALSE)
            && pRegNetwork->wirelessSettings->SSID)
        {
            if (!first)
            {
                // append the seperator
                strcat(ssidList, " ");
            }

            // append each ssid
            strcat(ssidList, pRegNetwork->wirelessSettings->SSID);

            first = FALSE;
        }
    }

    CN_LOGPRINTF("return ssidList=%s", ssidList);
    return ssidList;
}

gboolean connect_data_select_next_regprofile(int profileIndex)
{
    networkProfile_t *pNetwork;
    gboolean selected = FALSE;
    
    pNetwork = (networkProfile_t*)connect_data_get_network_profile(profileIndex);
    if (pNetwork
        && pNetwork->nRegProfiles > 0
        && (pNetwork->nActiveIndex >= 0
            && pNetwork->nActiveIndex < pNetwork->nRegProfiles))
    {
        if ((pNetwork->nActiveIndex+1) < pNetwork->nRegProfiles)
        {
            pNetwork->nActiveIndex += 1;
            selected = TRUE;
        }
    }

    return selected;
}

