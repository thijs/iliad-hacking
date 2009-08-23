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
 *\file 
 *\brief liberreg -  ereader registry
 *
 *Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "erreglog.h"
#include "erreg.h"
#include "erregbasis.h" 
#include "erregnwprofiles.h"
#include "erregrwlock.h"
#include "erregtoolbar.h"
#include "erregtypes_f.h"


// Context per registry section
static regRegistry_t *g_registry[regUndefined_t];  // compiler initialises these to NULL pointers


///////////////////////////////////////////////////////////////////////////////
//
//  whole registry
//
///////////////////////////////////////////////////////////////////////////////

regLoad_t erRegLoad(regSection_t section)
{
    ERREG_LOGPRINTF("entry: section [%d]", section);

    g_assert(section >= 0  &&  section <= regUndefined_t);
    g_assert(g_registry[section] == NULL);

    // at least read-lock required to load registry
    lock_state_t lock_state = erRegGetLockState();
    g_assert(lock_state == lock_read || lock_state == lock_write);
    
    regLoad_t ret = loadError_t;
    regRegistry_t** ppRegistry = &g_registry[section];
    
    switch (section)
    {
        case regBasis_t:
            ret = basisRegLoad(ppRegistry);
            break;
            
        case regToolbar_t:
            ret = tbRegLoad(ppRegistry);
            break;
            
        case regNWProfiles_t:
            ret = nwRegLoad(ppRegistry);
            break;

        default:
            ERREG_LOGPRINTF("Invalid section [%d].", section);
    }
    if (*ppRegistry)
    {
        (*ppRegistry)->section = section;
    }

    return ret;
}


void erRegUnload(regSection_t section)
{
    ERREG_LOGPRINTF("entry: section [%d]", section);
 
    g_assert(section >= 0  &&  section < regUndefined_t);
    g_assert(g_registry[section] != NULL);

    switch (section)
    {
        case regBasis_t:
            basisRegUnload(g_registry[regBasis_t]);
            g_registry[regBasis_t] = NULL;
            break;
            
        case regToolbar_t:
            tbRegUnload(g_registry[regToolbar_t]);
            g_registry[regToolbar_t] = NULL;
            break;

        case regNWProfiles_t:
            nwRegUnload(g_registry[regNWProfiles_t]);
            g_registry[regNWProfiles_t] = NULL;
            break;

        default:
            ERREG_LOGPRINTF("Invalid section [%d].", section);
    }
}


gboolean erRegStore(void)
{
    ERREG_LOGPRINTF("entry");

    // write-lock required to store registry
    g_assert(erRegGetLockState() == lock_write);

    gboolean bRet = TRUE;  // TRUE = ok
    gboolean b;
    regRegistry_t *reg;

    // save basis registry
    reg = g_registry[regBasis_t];
    if (reg)
    {
        b = basisRegStore(reg);
        if (b == FALSE)
        {
            bRet = FALSE;
        }
    }

    // never save toolbar registry

    // save network profiles
    reg = g_registry[regNWProfiles_t];
    if (reg)
    {
        b = nwRegStore(reg);
        if (b == FALSE)
        {
            bRet = FALSE;
        }
    }

    ERREG_LOGPRINTF("leave: bRet [%d]", bRet);
    return bRet;
}


gboolean erRegValidate(void)
{
    ERREG_LOGPRINTF("entry");

    gboolean bRet = TRUE;  // TRUE = ok
    gboolean b;
    regRegistry_t *reg;

    // validate basis registry
    reg = g_registry[regBasis_t];
    if (reg)
    {
        b = regValidate(reg);
        if (b == FALSE)
        {
            bRet = FALSE;
        }
    }

    // no need to validate toolbar registry, because it will never be saved

    // validate network profiles
    reg = g_registry[regNWProfiles_t];
    if (reg)
    {
        b = regValidate(reg);
        if (b == FALSE)
        {
            bRet = FALSE;
        }
    }

    ERREG_LOGPRINTF("leave: bRet [%d]", bRet);
    return bRet;
}


///////////////////////////////////////////////////////////////////////////////
//
//  basis registry
//
///////////////////////////////////////////////////////////////////////////////

#define registry  g_registry[regBasis_t]

regVersion_t *erRegGetVersion(void)
{
    g_assert(registry);
    
    return regGetVersion(registry);
}

gboolean erRegSetVersion(const regVersion_t *theVersion)
{
    g_assert(registry);

    return regSetVersion(theVersion, registry);
}

void erRegFreeVersion(regVersion_t *theVersion)
{
    regFreeVersion(theVersion);
}


regUserProfile_t *erRegGetUserProfile(void)
{
    g_assert(registry);
    
    return regGetUserProfile(registry);
}

gboolean erRegSetUserProfile(const regUserProfile_t *theUserProfile)
{
    g_assert(registry);
    
    return regSetUserProfile(theUserProfile, registry);
}

void erRegFreeUserProfile(regUserProfile_t *theUserProfile)
{
    regFreeUserProfile(theUserProfile);
}


regUserSetting_t *erRegGetUserSetting(void)
{
    g_assert(registry);
    
    return regGetUserSetting(registry);
}

gboolean erRegSetUserSetting(const regUserSetting_t *theUserSetting)
{
    g_assert(registry);
    
    return regSetUserSetting(theUserSetting, registry);
}

void erRegFreeUserSetting(regUserSetting_t *theUserSetting)
{
    regFreeUserSetting(theUserSetting);
}

regPCConfig_t *erRegGetPCConfig(void)
{
    g_assert(registry);
    
    return regGetPCConfig(registry);
}

regPCProfile_t *erRegGetPCProfile(const char *ID)
{
    g_assert(registry);
    
    return regGetPCProfile(ID, registry);
}

regPCProfile_t *erRegDupPCProfile(const regPCProfile_t *thePCProfile)
{
    return regDupPCProfile(thePCProfile);
}

gboolean erRegSetPCConfig(const regPCConfig_t *thePCConfig)
{
    g_assert(registry);
    
    return regSetPCConfig(thePCConfig, registry);
}

gboolean erRegSetPCProfile(const char *ID, const regPCProfile_t *thePCProfile)
{
    g_assert(registry);
    
    return regSetPCProfile(ID, thePCProfile, registry);
}

gboolean erRegRemovePCConfig(void)
{
    g_assert(registry);
    
    return regRemovePCConfig(registry);
}

gboolean erRegRemovePCProfile(const char *ID)
{
    g_assert(registry);
    
    return regRemovePCProfile(ID, registry);
}

void erRegFreePCConfig(regPCConfig_t *thePCConfig)
{
    regFreePCConfig(thePCConfig);
}

void erRegFreePCProfile(regPCProfile_t *thePCProfile)
{
    regFreePCProfile(thePCProfile);
}

void erRegGetPCProfiles(regPCConfig_t **pPCConfig, regPCProfile_t ***pPCs)
{
    g_assert(registry);
    
    regGetPCProfiles(pPCConfig, pPCs, registry);
}

gboolean erRegSetPCProfiles(const regPCConfig_t *pPCConfig, const regPCProfile_t **pPCs)
{
    g_assert(registry);
    
    return regSetPCProfiles(pPCConfig, pPCs, registry);
}

void erRegFreePCProfiles(regPCConfig_t *thePCConfig, regPCProfile_t **thePCs)
{
    regFreePCProfiles(thePCConfig, thePCs);
}


regAutoConnect_t *erRegGetAutoConnect(void)
{
    g_assert(registry);
    
    return regGetAutoConnect(registry);
}

gboolean erRegSetAutoConnect(regAutoConnect_t *theAutoConnect)
{
    g_assert(registry);
    
    return regSetAutoConnect(theAutoConnect, registry);
}

void erRegFreeAutoConnect(regAutoConnect_t *theAutoConnect)
{
    regFreeAutoConnect(theAutoConnect);
}


regTimedIds_t *erRegGetTimedIds(void)
{
    g_assert(registry);
    
    return regGetTimedIds(registry);
}

gboolean erRegSetTimedIds(regTimedIds_t *theTimedIds)
{
    g_assert(registry);
    
    return regSetTimedIds(theTimedIds, registry);
}

void erRegFreeTimedIds(regTimedIds_t *theTimedIds)
{
    regFreeTimedIds(theTimedIds);
}


regContentCategory_t *erRegGetContentCategory(char *ID)
{
    g_assert(registry);
    
    return regGetContentCategory(ID, registry);
}

gboolean erRegSetContentCategory(char *ID, regContentCategory_t *theContentCategory)
{
    g_assert(registry);
    
    return regSetContentCategory(ID, theContentCategory, registry);
}

void erRegFreeContentCategory(regContentCategory_t *theContentCategory)
{
    regFreeContentCategory(theContentCategory);
}


regExportMemType_t *erRegGetExportMemType()
{
    g_assert(registry);
    
    return regGetExportMemType(registry);
}

gboolean erRegSetExportMemType(regExportMemType_t *theExportMemType)
{
    g_assert(registry);
    
    return regSetExportMemType(theExportMemType, registry);
}

void erRegFreeExportMemType(regExportMemType_t *theExportMemType)
{
    regFreeExportMemType(theExportMemType);
}


regLastRead_t *erRegGetLastRead(char *ID)
{
    g_assert(registry);
    
    return regGetLastRead(ID, registry);
}

gboolean erRegSetLastRead(char *ID, regLastRead_t *theLastRead)
{
    g_assert(registry);
    
    return regSetLastRead(ID, theLastRead, registry);
}

void erRegFreeLastRead(regLastRead_t *theLastRead)
{
    regFreeLastRead(theLastRead);
}


regUserAppList_t *erRegGetUserAppList(void)
{
    g_assert(registry);
    
    return regGetUserAppList(registry);
}

// Note: function erRegSetUserAppList not needed

void erRegFreeUserAppList(regUserAppList_t *theUserAppList)
{
    regFreeUserAppList(theUserAppList);
}


regUserApp_t *erRegGetUserApp(char *uaID)
{
    g_assert(registry);
    
    return regGetUserApp(uaID, registry);
}

gboolean erRegSetUserApp(char *uaID, regUserApp_t *theUserApp)
{
    g_assert(registry);
    
    return regSetUserApp(uaID, theUserApp, registry);
}

void erRegFreeUserApp(regUserApp_t *theUserApp)
{
    regFreeUserApp(theUserApp);
}

regExtInfoList_t *erRegGetExtInfoList(const char *extension)
{
    g_assert(registry);

    return regGetExtInfoList(extension, registry);
}

/** NOT TESTED: not used, may not work, maybe needs to be reworked to erRegSetExtInfoList. **
gboolean erRegSetExtInfo(const char *extension, regExtInfo_t *theExtInfo)
{
    g_assert(registry);
    
    return regSetExtInfo(extension, theExtInfo, registry);
}
**/

void erRegFreeExtInfoList(regExtInfoList_t *theExtInfoList)
{
    regFreeExtInfoList(theExtInfoList);    
}

regStartUp_t *erRegGetStartUp(void)
{
    g_assert(registry);
    
    return regGetStartUp(registry);
}

gboolean erRegSetStartUp(regStartUp_t* theStartUp)
{
    g_assert(registry);
    
    return regSetStartUp(theStartUp, registry);
}

void erRegFreeStartUp(regStartUp_t *theStartUp)
{
    regFreeStartUp(theStartUp);
}

regStartUp_t *erRegDupStartUp(const regStartUp_t *theStartup)
{
    return regDupStartUp(theStartup);
}


#undef registry


regPCProfile_t *erRegGetPCProfileFromList(regPCConfig_t *pPCConfig, 
                    regPCProfile_t **pPCs, const char *ID, int *index)
{
    return basisRegGetPCProfileFromList(pPCConfig, pPCs, ID, index);
}

void erRegAddPCProfileToList(regPCConfig_t **pPCConfig, 
        regPCProfile_t ***pPCs, regPCProfile_t *pNewPCProfile)
{
    basisRegAddPCProfileToList(pPCConfig, pPCs, pNewPCProfile);
}

void erRegDeletePCProfileFromList(regPCConfig_t **pPCConfig, 
                        regPCProfile_t ***pPCs, const char *ID)
{
    basisRegDeletePCProfileFromList(pPCConfig, pPCs, ID);
}

// TODO remove
void erRegSetPCProfileDefault(regPCConfig_t **pPCConfig, 
                        regPCProfile_t ***pPCs, const char *ID)
{
    basisRegSetPCProfileDefault(pPCConfig, pPCs, ID);
}


///////////////////////////////////////////////////////////////////////////////
//
//  network profiles registry
//
///////////////////////////////////////////////////////////////////////////////

#define registry  g_registry[regNWProfiles_t]

regNetworkConfig_t *erRegGetNetworkConfig(void)
{
    g_assert(registry);

    return regGetNetworkConfig(registry);
}

regNetworkProfile_t *erRegGetNetworkProfile(const char *ID)
{
    g_assert(registry);

     return regGetNetworkProfile(ID, registry);
}

regNetworkProfile_t *erRegDupNetworkProfile(const regNetworkProfile_t *theNetworkProfile)
{
    return regDupNetworkProfile(theNetworkProfile);
}

gboolean erRegSetNetworkConfig(regNetworkConfig_t *theNetworkConfig)
{
    g_assert(registry);

    return regSetNetworkConfig(theNetworkConfig, registry);
}

gboolean erRegSetNetworkProfile(char *ID, regNetworkProfile_t *theNetworkProfile)
{
    g_assert(registry);

    return regSetNetworkProfile(ID, theNetworkProfile, registry);
}

void erRegFreeNetworkConfig(regNetworkConfig_t *theNetworkConfig)
{
    regFreeNetworkConfig(theNetworkConfig);
}

void erRegFreeNetworkProfile(regNetworkProfile_t *theNetworkProfile)
{
    regFreeNetworkProfile(theNetworkProfile);
}

gboolean erRegRemoveNetworkConfig(void)
{
    g_assert(registry);

    return regRemoveNetworkConfig(registry);
}

gboolean erRegRemoveNetworkProfile(const char *ID)
{
    g_assert(registry);

    return regRemoveNetworkProfile(ID, registry);
}

void erRegGetNetworkProfiles(regNetworkConfig_t **pNetworkConfig, regNetworkProfile_t ***pNetworks)
{  
    g_assert(registry);

    regGetNetworkProfiles(pNetworkConfig, pNetworks, registry);
}

gboolean erRegSetNetworkProfiles(const regNetworkConfig_t *pNetworkConfig, const regNetworkProfile_t **pNetworks)
{
    g_assert(registry);

    return regSetNetworkProfiles(pNetworkConfig, pNetworks, registry);
}  
  
void erRegFreeNetworkProfiles(regNetworkConfig_t *theNetworkConfig, regNetworkProfile_t **theNetworks)
{
     regFreeNetworkProfiles(theNetworkConfig, theNetworks);
}

int erRegGetNetworkProfileFromList(regNetworkConfig_t *pNetworkConfig, const char *ID)
{
    return nwRegGetNetworkProfileFromList(pNetworkConfig, ID);
}


regLastConnect_t *erRegGetLastConnect(void)
{
    g_assert(registry);

    return regGetLastConnect(registry);
}

gboolean erRegSetLastConnect(regLastConnect_t* theLastConnect)
{
    g_assert(registry);

    return regSetLastConnect(theLastConnect, registry);
}

void erRegFreeLastConnect(regLastConnect_t *theLastConnect)
{
    regFreeLastConnect(theLastConnect);
}


gboolean erRegGetAutoConnectWlan(void)
{
    g_assert(registry);

    return regGetAutoConnectWlan(registry);
}

gboolean erRegSetAutoConnectWlan(gboolean enable)
{
    g_assert(registry);

    return regSetAutoConnectWlan(enable, registry);
}


void erRegAddNetworkProfileToList(regNetworkConfig_t **pNetworkConfig, 
        regNetworkProfile_t ***pNetworks, regNetworkProfile_t *pNewNetworkProfile)
{
    nwRegAddNetworkProfileToList(pNetworkConfig, pNetworks, pNewNetworkProfile);
}

void erRegDeleteNetworkProfileFromList(regNetworkConfig_t **pNetworkConfig, 
                            regNetworkProfile_t ***pNetworks, const char *ID)
{
    nwRegDeleteNetworkProfileFromList(pNetworkConfig, pNetworks, ID);
}

void erRegSetNetworkProfileDefault(regNetworkConfig_t **pNetworkConfig, 
                            regNetworkProfile_t ***pNetworks, const char *ID)
{
    nwRegSetNetworkProfileDefault(pNetworkConfig, pNetworks, ID);
}

#undef registry


///////////////////////////////////////////////////////////////////////////////
//
//  toolbar registry
//
///////////////////////////////////////////////////////////////////////////////

#define registry  g_registry[regToolbar_t]

regIconSet_t *erRegGetIconSet(void)
{
    return tbRegGetIconSet(registry);
}

void erRegFreeIconSet(regIconSet_t *theIconSet)
{
    tbRegFreeIconSet(theIconSet);
}

#undef registry

