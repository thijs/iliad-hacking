/**
 *\file   erregnwprofiles.h 
 *\brief  liberreg - ereader network profiles registry
 *                   loading/recovering and unloading network profiles registry
 *                   getting and setting functions
 *                   free memory functions
 *                   build-in network profiles registry
 *
 *Copyright (C) 2007 iRex Technologies BV.
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
