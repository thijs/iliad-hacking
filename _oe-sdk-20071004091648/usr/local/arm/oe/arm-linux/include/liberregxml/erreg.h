/**
 *\file  erreg.h 
 *\brief liberreg -  ereader registry
 *                   checking registry
 *                   loading and unloading registry
 *                   getting and setting functions
 *                   free memory functions
 *
 *Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _ERREG_H_
#define _ERREG_H_

#include "erregtypes.h"


// Location of registry files
#define REG_DIR         DATADIR "/registry"
#define TEMPLATE_DIR    "/etc/registry"


typedef enum
{
    loadOk_t,       // loading registry succeeded
    loadTemplate_t, // loading registry failed, but loading template registry succeeded
    loadMinSet_t,   // loading registry and template registry failed, but loading minset succeeded
    loadError_t,    // loading registry failed and loading template failed
}regLoad_t;

typedef enum
{
    regBasis_t,
    regToolbar_t,
    regNWProfiles_t,
    regUndefined_t
}regSection_t;

#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

    // Loading the registry into memory.
    regLoad_t erRegLoad(regSection_t section);

    // Destory the registry in memory.
    void erRegUnload(regSection_t section);

    // Store the current registry in memory to file
    gboolean erRegStore(void);

    // Validation
    gboolean erRegValidate(void);

    // Get and Set functions
    regVersion_t *erRegGetVersion(void);
    gboolean erRegSetVersion(const regVersion_t *theVersion);
    void erRegFreeVersion(regVersion_t *theVersion);

    regUserProfile_t *erRegGetUserProfile(void);
    gboolean erRegSetUserProfile(const regUserProfile_t *theUserProfile);
    void erRegFreeUserProfile(regUserProfile_t *theUserProfile);

    regUserSetting_t *erRegGetUserSetting(void);
    gboolean erRegSetUserSetting(const regUserSetting_t *theUserSetting);
    void erRegFreeUserSetting(regUserSetting_t *theUserSetting);

    regPCConfig_t *erRegGetPCConfig(void);
    regPCProfile_t *erRegGetPCProfile(const char *ID);
   
    regPCProfile_t *erRegDupPCProfile(const regPCProfile_t *thePCProfile);
    
    gboolean erRegSetPCConfig(const regPCConfig_t *thePCConfig);
    gboolean erRegSetPCProfile(const char *ID, const regPCProfile_t *thePCProfile);
    
    void erRegFreePCConfig(regPCConfig_t *thePCConfig);
    void erRegFreePCProfile(regPCProfile_t *thePCProfile);
    
    gboolean erRegRemovePCConfig(void);
    gboolean erRegRemovePCProfile(const char *ID);
   
    void erRegGetPCProfiles(regPCConfig_t **pPCConfig, regPCProfile_t ***pPCs);
    gboolean erRegSetPCProfiles(const regPCConfig_t *pPCConfig, const regPCProfile_t **pPCs);
    void erRegFreePCProfiles(regPCConfig_t *thePCConfig, regPCProfile_t **thePCs);
 
    regPCProfile_t *erRegGetPCProfileFromList(regPCConfig_t *pPCConfig, regPCProfile_t **pPCs, const char *ID, int *index);
    void erRegAddPCProfileToList(regPCConfig_t **pPCConfig, regPCProfile_t ***pPCs, regPCProfile_t *pNewPCProfile);
    void erRegDeletePCProfileFromList(regPCConfig_t **pPCConfig, regPCProfile_t ***pPCs, const char *ID);
    void erRegSetPCProfileDefault(regPCConfig_t **pPCConfig, regPCProfile_t ***pPCs, const char *ID);
  
    regAutoConnect_t *erRegGetAutoConnect(void);
    gboolean erRegSetAutoConnect(regAutoConnect_t *theAutoConnect);
    void erRegFreeAutoConnect(regAutoConnect_t *theAutoConnect);

    regTimedIds_t *erRegGetTimedIds(void);
    gboolean erRegSetTimedIds(regTimedIds_t *theTimedIds);
    void erRegFreeTimedIds(regTimedIds_t *theTimedIds);
    
    regContentCategory_t *erRegGetContentCategory(char *ID);
    gboolean erRegSetContentCategory(char *ID, regContentCategory_t *theContentCategory);
    void erRegFreeContentCategory(regContentCategory_t *theContentCategory);

    regExportMemType_t *erRegGetExportMemType();
    gboolean erRegSetExportMemType(regExportMemType_t *theExportMemType);
    void erRegFreeExportMemType();

    regLastRead_t *erRegGetLastRead(char *ID);
    gboolean erRegSetLastRead(char *ID, regLastRead_t *theLastRead);
    void erRegFreeLastRead(regLastRead_t *theLastRead);

    regUserAppList_t *erRegGetUserAppList(void);
    void erRegFreeUserAppList(regUserAppList_t *theUserAppList);

    regUserApp_t *erRegGetUserApp(char *uaID);
    gboolean erRegSetUserApp(char *uaID, regUserApp_t *theUserApp);
    void erRegFreeUserApp(regUserApp_t *theUserApp);

    regExtInfoList_t *erRegGetExtInfoList(const char *extension);
    //gboolean erRegSetExtInfo(const char *extension, regExtInfo_t *theExtInfo);
    void erRegFreeExtInfoList(regExtInfoList_t * theExtInfoList);

    regStartUp_t *erRegGetStartUp(void);
    gboolean erRegSetStartUp(regStartUp_t* theStartUp);
    void erRegFreeStartUp(regStartUp_t *theStartUp);
    regStartUp_t *erRegDupStartUp(const regStartUp_t *theStartup);

    regNetworkConfig_t * erRegGetNetworkConfig(void);
    regNetworkProfile_t *erRegGetNetworkProfile(const char *ID);
     
    regNetworkProfile_t *erRegDupNetworkProfile(const regNetworkProfile_t *theNetworkProfile);
    
    gboolean erRegSetNetworkConfig(regNetworkConfig_t *theNetworkConfig);
    gboolean erRegSetNetworkProfile(char *ID, regNetworkProfile_t *theNetworkProfile);
    
    void erRegFreeNetworkProfile(regNetworkProfile_t *theNetworkProfile);
    void erRegFreeNetworkConfig(regNetworkConfig_t *theNetworkConfig);
    
    gboolean erRegRemoveNetworkProfile(const char *ID);
    gboolean erRegRemoveNetworkConfig(void);

    void erRegGetNetworkProfiles(regNetworkConfig_t **pNetworkConfig, regNetworkProfile_t ***pNetworks);
    gboolean erRegSetNetworkProfiles(const regNetworkConfig_t *pNetworkConfig, const regNetworkProfile_t **pNetworks);    
    void erRegFreeNetworkProfiles(regNetworkConfig_t *theNetworkConfig, regNetworkProfile_t **theNetworks);

    regLastConnect_t *erRegGetLastConnect(void);
    gboolean erRegSetLastConnect(regLastConnect_t* theLastConnect);
    void erRegFreeLastConnect(regLastConnect_t *theLastConnect);

    gboolean erRegGetAutoConnectWlan(void);
    gboolean erRegSetAutoConnectWlan(gboolean enable);

    int erRegGetNetworkProfileFromList(regNetworkConfig_t *pNetworkConfig, 
            const char *ID);
    void erRegAddNetworkProfileToList(regNetworkConfig_t **pNetworkConfig, 
            regNetworkProfile_t ***pNetworks, regNetworkProfile_t *pNewNetworkProfile);
    void erRegDeleteNetworkProfileFromList(regNetworkConfig_t **pNetworkConfig, 
            regNetworkProfile_t ***pNetworks, const char *ID);
    void erRegSetNetworkProfileDefault(regNetworkConfig_t **pNetworkConfig, 
            regNetworkProfile_t ***pNetworks, const char *ID);

    regIconSet_t *erRegGetIconSet(void);
    void erRegFreeIconSet(regIconSet_t *theIconSet);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          // _ERREG_H_
