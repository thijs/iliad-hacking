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
 *\file   erregnwprofiles.h 
 *\brief  liberreg - ereader network profiles registry
 *                   loading/recovering and unloading network profiles registry
 *                   getting and setting functions
 *                   free memory functions
 *                   build-in network profiles registry
 *
 *Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#ifndef _ERREG_NWPROFILES_H_
#define _ERREG_NWPROFILES_H_

#include "erregtypes.h"
#include "erregxml.h"
#include "erreg.h"

// network profiles files
#define REG_NWPROFILES_XML_FILE        "nwprofiles.xml"
#define REG_NWPROFILES_XSD_PATH        TEMPLATE_DIR "/nwprofiles.xsd"

// default values for network profiles
#define WIRED_NETWORK_NAME            "Wired network"
#define WIRELESS_NETWORK_NAME         "Wireless network"

#define DEFAULT_WLAN_SSID              "ereader"
#define DEFAULT_WLAN_ENCRYPTION_TYPE   encr_none_t 
#define DEFAULT_WLAN_ENCRYPTION_KEY    ""
#define DEFAULT_WLAN_BROADCAST         TRUE

#define DEFAULT_ADDRESS_MODE           dhcp_t

#define DEFAULT_USE_PROXY              FALSE 

#ifdef __cplusplus
extern "C"
{
#endif

  regLoad_t nwRegLoad (regRegistry_t** ppRegistry);
  void nwRegUnload (regRegistry_t* pRegistry);
  gboolean nwRegStore(regRegistry_t* pRegistry);

  void nwRegInitXpathList(regRegistry_t *pRegistry);
  void nwRegDestroyXpathList(regRegistry_t *pRegistry);

  int nwRegGetNetworkProfileFromList (const regNetworkConfig_t *pNetworkConfig,
                                      const char *ID);
  void nwRegAddNetworkProfileToList (regNetworkConfig_t **pNetworkConfig,
                                     regNetworkProfile_t ***pNetworks, regNetworkProfile_t *pNewNetworkProfile);
  void nwRegDeleteNetworkProfileFromList (regNetworkConfig_t **pNetworkConfig,
                                     regNetworkProfile_t ***pNetworks, const char *ID);
  void nwRegSetNetworkProfileDefault (regNetworkConfig_t **pNetworkConfig,
                                     regNetworkProfile_t ***pNetworks, const char *ID);

  // internal functions
  regNetworkConfig_t *nwRegNewNetworkConfig(int size);
  regNetworkProfile_t *nwRegNewNetworkProfile(const char *ID, connection_t connection);
  void nwRegNewNetworkProfiles(regNetworkConfig_t **pNetworkConfig, regNetworkProfile_t ***pNetworks);

#ifdef __cplusplus
}
#endif

#endif// _ERREG_NWPROFILES_H_
