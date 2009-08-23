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
 * \file commonData.h
 * \brief connectionMgr - common data handling
 *                        getting and setting data from/into registry
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _COMMON_DATA_H_
#define _COMMON_DATA_H_

#include <glib.h>
#include <liberregxml/erregapi.h> 
#include "scanThread.h"

#define REDIRECT_URL            "https://ids.irexnet.com:443/redirector"

#define MAX_WIRED_PROFILES       6 
#define MAX_WIRELESS_PROFILES    12
#define MAX_DIALUP_PROFILES      6  

// Connect action required
typedef enum
{
    cmgrConnectUnknown_e,
    cmgrConnectIDS_e,
    cmgrConnectPCShare_e,
    cmgrConnectUndef_e
} cmgrConnectType_t;

typedef struct
{
    network_spec_t      *scanSettings;    
    // the counterparts array in the registry of scanSettings
    // NULL if there's no counterparts
    regNetworkProfile_t **regSettingsList;
    // the indice array(count from 0) in the profile list from registry, 
    // NULL stands for no counterpart of scanSettings
    int                 *regIndexList;        
    // how many counterparts in the registry of scanSettings
    int                 nRegProfiles;
    // the index in regIndexList of the current used profile, 
    // -1 stands for no profile used
    int                 nActiveIndex;  
}networkProfile_t;

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    void common_data_init(void);
    void common_data_destroy(void);
    void common_data_store(void);

    // get how many profiles in the registry
    int common_data_get_n_profiles(void);

    // get/set registry data for specified network profile, 
    // index 0 is default profile
    const regNetworkProfile_t* common_data_get_profiledata(const gint index);
    gboolean common_data_set_profiledata(gint index, 
                                     const regNetworkProfile_t* setting, 
                                     const gboolean preferred);

    // remove
    guint common_data_remove_profile(guint index);

    // get registry data for specified pcshare profile, 
    // index 0 is default profile
    const regPCProfile_t* common_data_get_pcsharedata(const gint index);

    // before adding a 'networkType' profile, 
    // check whether reach the max profiles
    // return TRUE  : reach the max profiles
    //        FALSE : not reach the max profiles
    gboolean common_data_reach_max_profiles(connection_t networkType);

    // the functions of operating the list of networkProfile_t
    int common_data_init_network_profiles(connection_t networkType, 
                                          networkProfile_t** pNetworks);
    void common_data_destroy_network_profiles(networkProfile_t* pNetworks, 
                                              guint nProfiles);
    int common_data_get_profile_index(networkProfile_t* pNetworks, 
                                      int nProfiles, 
                                      int regIndex);
    int common_data_get_reg_index(networkProfile_t* pNetworks,
                                  int nProfiles,
                                  int profileIndex);
#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif //_COMMON_DATA_H_

