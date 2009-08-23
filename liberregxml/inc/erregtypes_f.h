/*
 * This file is part of liberregxml.
 *
 * liberregxml is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberregxml is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file 
 * \brief liberreg -  Dump(), Free(), Dup() datatypes in registry
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#ifndef _ER_REG_TYPES_F_H_
#define _ER_REG_TYPES_F_H_

#include "erregtypes.h"

#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */
    
    // debug routines
    void regDumpVersion(const regVersion_t * theVersion);
    void regDumpUserProfile(const regUserProfile_t * theUserProfile);
    void regDumpUserSetting(const regUserSetting_t * theUserSetting);
    void regDumpPCConfig(const regPCConfig_t * thePCConfig);
    void regDumpPCProfile(const char *ID, const regPCProfile_t * thePCProfile);
    void regDumpAutoConnect(const regAutoConnect_t * theAutoConnect);
    void regDumpTimedIds(const regTimedIds_t *theTimedIds);
    void regDumpContentCategory(const char *ID, const regContentCategory_t * theContentCategory);
    void regDumpExportMemType(const regExportMemType_t *theExportMemType);
    void regDumpLastRead(const char *ID, const regLastRead_t * theLastRead);
    void regDumpUserAppList(const regUserAppList_t * theUserAppList);
    void regDumpUserApp(const char *ID, const regUserApp_t * theUserApp);
    void regDumpExtInfo(const regExtInfo_t * theExtInfo);
    void regDumpExtInfoList(const regExtInfoList_t * theExtInfoList);
    void regDumpStartUp(const regStartUp_t * theStartUp);
    void regDumpNetworkConfig(const regNetworkConfig_t * theNetworkConfig);
    void regDumpNetworkProfile(const char *ID, const regNetworkProfile_t * theNetworkProfile);
    void regDumpLastConnect(const regLastConnect_t * theLastConnect);
    void regDumpAutoConnectWlan(const gboolean enable);
    void regDumpIconSet(const regIconSet_t * theIconSet);
    void regDumpDeviceDetails(const deviceDetails_t* theDeviceDetails);

    // copy functions
    regStartUp_t *regDupStartUp(const regStartUp_t * theStartUp);
    regPCProfile_t *regDupPCProfile(const regPCProfile_t * thePCProfile);
    regNetworkProfile_t *regDupNetworkProfile(const regNetworkProfile_t * theNetworkProfile);

    // free the datastructs
    void regFreeVersion(regVersion_t * theVersion);
    void regFreeUserProfile(regUserProfile_t * theUserProfile);
    void regFreeUserSetting(regUserSetting_t * theUserSetting);
    void regFreePCConfig(regPCConfig_t * thePCConfig);
    void regFreePCProfile(regPCProfile_t * thePCProfile);
    void regFreePCProfiles(regPCConfig_t * thePCConfig, regPCProfile_t ** thePCs);

    void regFreeAutoConnect(regAutoConnect_t * theAutoConnect);
    void regFreeTimedIds(regTimedIds_t * theTimedIds);
    void regFreeContentCategory(regContentCategory_t * theContentCategory);
    void regFreeExportMemType(regExportMemType_t *theExportMemType);
    void regFreeLastRead(regLastRead_t * theLastRead);
    void regFreeUserAppList(regUserAppList_t * theUserAppList);
    void regFreeUserApp(regUserApp_t * theUserApp);
    void regFreeExtInfoList(regExtInfoList_t * theExtInfoList);
    void regFreeExtInfo(regExtInfo_t * theExtInfo);
    
    void regFreeStartUp(regStartUp_t * theStartUp);

    void regFreeNetworkProfile(regNetworkProfile_t * theNetworkProfile);
    void regFreeNetworkConfig(regNetworkConfig_t * theNetworkConfig);
    void regFreeNetworkProfiles(regNetworkConfig_t * theNetworkConfig, regNetworkProfile_t ** theNetworks);

    void regFreeWirelessSetting(regWirelessSetting_t * theWirelessSetting);
    void regFreeDialupSetting(regDialupSetting_t* theDialupSetting);
    void regFreeProxySetting(regProxySetting_t * theProxySetting);
    void regFreeIpSetting(regIpSetting_t * theIpSetting);

    void regFreeLastConnect(regLastConnect_t * theLastConnect);

    void regFreeStartUp(regStartUp_t * theStartUp);
    void regFreeIconSet(regIconSet_t * theIconSet);
    
    void regFreeDeviceDetails(deviceDetails_t* theDeviceDetails);

    // covert string to enumeration type
    // convert enumeration type to string
    connectTo_t regServerTypeToEnum(const char*string);
    void regServerTypeToString(connectTo_t serverType, char* string, int len);

    startup_behaviour_t regBehaviourToEnum(const char*string);
    void regBehaviourToString(startup_behaviour_t behaviourType, char* string, int len);

    connection_t regNetworkTypeToEnum(const char*string);
    void regNetworkTypeToString(connection_t networkType, char* string, int len);

    ip_address_mode_t regIpAddressToEnum(const gboolean useDhcp);
    gboolean regIpAddressToBool(const ip_address_mode_t ipAddressType);

    wireless_encr_type_t regWlanEncrToEnum(const char*string);
    void regWlanEncrToString(const wireless_encr_type_t encrType, char* string, const int len);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          // _ER_REG_TYPES_F_H_

