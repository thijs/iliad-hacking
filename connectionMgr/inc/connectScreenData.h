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
 * \file connectScreenData.h
 * \brief connectionMgr - connect screen data handling
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _CONNECT_SCREEN_DATA_H_
#define _CONNECT_SCREEN_DATA_H_

#include <glib.h>
#include <liberregxml/erregapi.h>
#include "commonData.h"
#include "scanThread.h"

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    void connect_data_init(void);
    void connect_data_destroy(void);
    gboolean connect_data_store(void);

    gint connect_data_get_last_connected_profile(void);
    gboolean connect_data_set_last_connected_profile(gint regIndex);

    cmgrConnectType_t connect_data_get_connect_type(void);
    gboolean connect_data_set_connect_type(cmgrConnectType_t type);

    connection_t connect_data_get_network_type(void);
    gboolean connect_data_set_network_type(connection_t type);

    gboolean connect_data_get_ac_wlan(void);
 
    void connect_data_update_last_connected_profile_after_delete(gint delRegIndex);
       
    const regPCProfile_t* connect_data_get_pcsharedata(void);
    
    gchar* connect_data_get_ecd_url(void);

    void connect_data_init_network_profiles(connection_t networkType, 
                                            network_spec_t* pScanNetworks, 
                                            int nScanNetworks);
    void connect_data_destroy_network_profiles(void);

    int connect_data_get_n_network_profiles(void);
    const networkProfile_t* connect_data_get_network_profile(int profileIndex);

    int connect_data_get_profile_index(int regIndex);
    int connect_data_get_reg_index(int profileIndex);

    gboolean connect_data_reach_max_profiles(void);

    void connect_data_update_network_profile_with_last(networkProfile_t* pNetwork);

    regNetworkProfile_t* network_spec_to_reg_network(network_spec_t* scanSettings);

    char* connect_data_get_hidden_ssid_list(void);
    
    gboolean connect_data_select_next_regprofile(int profileIndex);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_CONNECT_SCREEN_DATA_H_


