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
 * \brief liberreg -  import and export device configurations
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <glib.h>

#include "erreglog.h"
#include "erreg.h"
#include "erregbasis.h"
#include "erregcfg.h"
#include "erregnwprofiles.h"
#include "erregtypes_f.h"
#include "erregxsd.h"
#include "erregxml.h"


/** NOT TESTED YET **
 *

///////////////////////////////////////////////////////////////////////////////
//
//  xpath of config 
//
///////////////////////////////////////////////////////////////////////////////
static void cfgRegInitXpathList(regRegistry_t * pRegistry)
{
    ERREG_CFG_LOGPRINTF("entry");
    g_assert(pRegistry);

    xpathList_t *xpaths = g_new0(xpathList_t, 1);
    g_assert(xpaths);

    xpaths->userProfile     = "/" EL_CONFIG_ROOT "/" EL_USER_PROFILE;
    xpaths->userSetting     = "/" EL_CONFIG_ROOT "/" EL_USER_SETTING;
    xpaths->pcProfiles      = "/" EL_CONFIG_ROOT "/" EL_PC_PROFILES;
    xpaths->pcProfile       = "/" EL_CONFIG_ROOT "/" EL_PC_PROFILES       "/" EL_PC_PROFILE;
    xpaths->networkProfiles = "/" EL_CONFIG_ROOT "/" EL_NETWORK_PROFILES;
    xpaths->networkProfile  = "/" EL_CONFIG_ROOT "/" EL_NETWORK_PROFILES "/" EL_NETWORK_PROFILE;
    xpaths->autoConnect     = "/" EL_CONFIG_ROOT "/" EL_AUTO_CONNECT;
    xpaths->startup         = "/" EL_CONFIG_ROOT "/" EL_START_UP;
    xpaths->deviceDetails   = "/" EL_CONFIG_ROOT "/" EL_DEVICE_DETAILS;

    pRegistry->xpaths = xpaths;
}

static void cfgRegDestroyXpathList(regRegistry_t * pRegistry)
{
    ERREG_CFG_LOGPRINTF("entry");
    g_assert(pRegistry);

    g_free(pRegistry->xpaths);
    pRegistry->xpaths = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
//  file utilities
//
///////////////////////////////////////////////////////////////////////////////
static gboolean cfgFileExits(const char *szFileName)
{
    ERREG_CFG_LOGPRINTF("entry %s", szFileName);

    struct stat buf;

    if(NULL==szFileName) return 0;

    return ( 0==stat(szFileName, &buf) );
}

// notes: we create szXmlFile, but load szXsdfile
static regRegistry_t *cfgCreateAndLoadFiles(const char *szXmlFile, const char *szXsdFile)
{
    regRegistry_t* pRegistry = NULL;

    ERREG_CFG_LOGPRINTF("entry %s %s", szXmlFile, szXsdFile);
    
    if (szXmlFile && szXsdFile)
    {
        gboolean bRet = FALSE;
        int nRet;

        pRegistry = g_new0(regRegistry_t, 1);
        if (pRegistry)
        {
            cfgRegInitXpathList(pRegistry);

            nRet = ermXmlCreateFile(szXmlFile, &(pRegistry->xmlCxt));
            if (RET_OK == nRet)
            {
                ERREG_CFG_LOGPRINTF("creating file named in %s successfully", szXmlFile);
                bRet = xsdLoad(&(pRegistry->xsdCxt), szXsdFile);
                if (bRet)
                {
                    ERREG_CFG_LOGPRINTF("loading file named in %s successfully", szXsdFile);
                }
            }

            if (FALSE == bRet)
            {
                ERREG_CFG_ERRORPRINTF("error");

                cfgRegDestroyXpathList(pRegistry);
                regDestroy(pRegistry);
                pRegistry = NULL;
            }
        }
        else
        {
            ERREG_CFG_ERRORPRINTF("memory error");
        }
    }

    return pRegistry;
}

static gboolean cfgStore(regRegistry_t * pRegistry)
{
    gboolean bRet;
    gchar *szPath = pRegistry->xmlCxt.szFileName;
    gchar *szPath_old = g_strdup_printf("%s.old", szPath);

    bRet = regStore(pRegistry, szPath);
    unlink(szPath_old);

    g_free(szPath_old);
    return bRet;
}

// save to file, then close the handler pointed by pRegistry
static void cfgSaveAndClose(regRegistry_t * pRegistry)
{
    ERREG_CFG_LOGPRINTF("entry");
    g_assert(pRegistry);

    cfgStore(pRegistry);

    cfgRegDestroyXpathList(pRegistry);
    regDestroy(pRegistry);
}

///////////////////////////////////////////////////////////////////////////////
//
// import/export functions 
//
///////////////////////////////////////////////////////////////////////////////
gboolean cfgImportUserProfile(regUserProfile_t * theUserProfile, regRegistry_t * pRegistry)
{
    if (theUserProfile && pRegistry)
    {
        // users can't import redirectUrl
        if (theUserProfile->redirectUrl)
        {
            g_free(theUserProfile->redirectUrl);
            theUserProfile->redirectUrl = NULL;
        }

        regSetUserProfile(theUserProfile, pRegistry);
        return TRUE;
    }

    return FALSE;
}

static gboolean cfgExportUserProfile(regUserProfile_t * theUserProfile, regRegistry_t * pRegistry)
{
    if (theUserProfile && pRegistry)
    {
        // excluded password and redirectUrl from being exported
        if (theUserProfile->password)
        {
            g_free(theUserProfile->password);
            theUserProfile->password = g_strdup("");
        }
        
        if (theUserProfile->redirectUrl)
        {
            g_free(theUserProfile->redirectUrl);
            theUserProfile->redirectUrl = NULL;
        }

        regSetUserProfile(theUserProfile, pRegistry);

        return TRUE;
    }

    return FALSE;
}

static gboolean cfgImportUserSetting(regUserSetting_t * theUserSetting, regRegistry_t * pRegistry)
{
    if (theUserSetting && pRegistry)
    {
        regSetUserSetting(theUserSetting, pRegistry);
        
        return TRUE;
    }

    return FALSE;
}

static gboolean cfgExportUserSetting(regUserSetting_t * theUserSetting, regRegistry_t * pRegistry)
{
    if (theUserSetting && pRegistry)
    {
        regSetUserSetting(theUserSetting, pRegistry);
        return TRUE;
    }

    return FALSE;
}

static gboolean cfgImportNetworkProfiles(const regNetworkConfig_t * theNetworkConfig,
                                         const regNetworkProfile_t ** theNetworks,
                                         regRegistry_t * pRegistry)
{
    if (theNetworkConfig && theNetworks && pRegistry)
    {
        regSetNetworkProfiles(theNetworkConfig, theNetworks, pRegistry);
        return TRUE;
    }

    return FALSE;
}

static gboolean cfgExportNetworkProfiles(const regNetworkConfig_t * theNetworkConfig,
                                         const regNetworkProfile_t ** theNetworks,
                                         regRegistry_t * pRegistry)
{
    if (theNetworkConfig && theNetworks && pRegistry)
    {
        int i;
        
        // excluded encrKey from being exported
        for (i = 0; i < theNetworkConfig->size; i++)
        {
            if (wireless_t == theNetworks[i]->connection)
            {
                if (theNetworks[i]->wirelessSettings)
                {
                    if (theNetworks[i]->wirelessSettings->encrKey)
                    {
                        // theNetworks[i]->wirelessSettings->encrType = encr_none_t;
                        g_free(theNetworks[i]->wirelessSettings->encrKey);
                        theNetworks[i]->wirelessSettings->encrKey = g_strdup("");
                    }
                }
            }
            else if (dialup_t == theNetworks[i]->connection)
            {
                if (theNetworks[i]->dialupSettings)
                {
                    g_free(theNetworks[i]->dialupSettings->password);
                    theNetworks[i]->dialupSettings->password = g_strdup("");
                }
            }
        }
        
        regSetNetworkProfiles(theNetworkConfig, theNetworks, pRegistry);

        return TRUE;
    }

    return FALSE;
}

static gboolean cfgImportPCProfiles(const regPCConfig_t * thePCConfig, 
                                    const regPCProfile_t ** thePCs,
                                    regRegistry_t * pRegistry)
{
    if (thePCConfig && thePCs && pRegistry)
    {
        regSetPCProfiles(thePCConfig, thePCs, pRegistry);

        return TRUE;
    }

    return FALSE;
}

static gboolean cfgExportPCProfiles(regPCConfig_t * thePCConfig, 
                                    regPCProfile_t ** thePCs,
                                    regRegistry_t * pRegistry)
{
    if (thePCConfig && thePCs && pRegistry)
    {
        int i;

        regSetPCConfig(thePCConfig, pRegistry);
        
        for (i = 0; i < thePCConfig->size; i++)
        {
            // excluded password from being exported
            if (thePCs[i]->password)
            {
                g_free(thePCs[i]->password);
                thePCs[i]->password = g_strdup("");
            }
            
            regSetPCProfile(thePCConfig->pcList[i], thePCs[i], pRegistry);
        }

        return TRUE;
    }

    return FALSE;
}

static gboolean cfgImportAutoConnect(regAutoConnect_t *theAutoConnect, regRegistry_t * pRegistry)
{
    if (theAutoConnect && pRegistry)
    {
        regSetAutoConnect(theAutoConnect, pRegistry);

        return TRUE;
    }

    return FALSE;
}

static gboolean cfgExportAutoConnect(regAutoConnect_t * theAutoConnect, regRegistry_t * pRegistry)
{
    if (theAutoConnect && pRegistry)
    {
        regSetAutoConnect(theAutoConnect, pRegistry);
        return TRUE;
    }

    return FALSE;
}

static gboolean cfgImportStartUp(regStartUp_t *theStartUp, regRegistry_t * pRegistry)
{
    if (theStartUp && pRegistry)
    {
        regSetStartUp(theStartUp, pRegistry);

        return TRUE;
    }

    return FALSE;
}

gboolean cfgExportStartUp(regStartUp_t * theStartUp, regRegistry_t * pRegistry)
{
    if (theStartUp && pRegistry)
    {
        regSetStartUp(theStartUp, pRegistry);

        return TRUE;
    }

    return FALSE;
}

static gboolean cfgExportDeviceDetails(deviceDetails_t * theDeviceDetails, regRegistry_t * pRegistry)
{
    return regSetDeviceDetails(theDeviceDetails, pRegistry);
}

///////////////////////////////////////////////////////////////////////////////
//
// interface APIs 
//
///////////////////////////////////////////////////////////////////////////////
gboolean erRegCfgFileExist(void)
{
    ERREG_CFG_LOGPRINTF("entry");

    return cfgFileExits(CONFIG_DIR "/" IMPORT_CONFIG_XML_FILE);
}

gboolean erRegImportConfig(void)
{
    ERREG_CFG_LOGPRINTF("entry");

    gboolean bRet = TRUE;
   
//    if (!cfgFileExits(CONFIG_DIR "/" IMPORT_CONFIG_XML_FILE))
//    {
//        return bRet;
//   }

    // get the settings from device configuration file
    regRegistry_t* pRegistry = regParseFiles( CONFIG_DIR "/" IMPORT_CONFIG_XML_FILE, 
                                              CONFIG_XSD_PATH                       );
    if (pRegistry)
    {
        ERREG_CFG_LOGPRINTF("entry");
       
        cfgRegInitXpathList(pRegistry);

        regUserProfile_t* theUserProfile = regGetUserProfile(pRegistry);
        regUserSetting_t* theUserSetting = regGetUserSetting(pRegistry);
        
        regPCConfig_t* pPCConfig = NULL;
        regPCProfile_t** pPCs = NULL;
        regGetPCProfiles(&pPCConfig, &pPCs, pRegistry);

        regNetworkConfig_t* pNetworkConfig = NULL;
        regNetworkProfile_t** pNetworks = NULL;
        regGetNetworkProfiles(&pNetworkConfig, &pNetworks, pRegistry);

        regAutoConnect_t* theAutoConnect = regGetAutoConnect(pRegistry);

        regStartUp_t* theStartUp = regGetStartUp(pRegistry);

        cfgRegDestroyXpathList(pRegistry);
        regDestroy(pRegistry);
        pRegistry = NULL;

        // import to the registry file(basis)
        pRegistry = regParseFiles( REG_DIR "/" REG_BASIS_XML_FILE,
                                   REG_BASIS_XSD_PATH             );
        if (pRegistry)
        {
            ERREG_CFG_LOGPRINTF("entry");
           
            basisRegInitXpathList(pRegistry);

            cfgImportUserProfile(theUserProfile, pRegistry);
            cfgImportUserSetting(theUserSetting, pRegistry);
            cfgImportPCProfiles(pPCConfig, (const regPCProfile_t**)pPCs, pRegistry);
            cfgImportAutoConnect(theAutoConnect, pRegistry);
            cfgImportStartUp(theStartUp, pRegistry);

            // store to file
            cfgStore(pRegistry);
        
            basisRegDestroyXpathList(pRegistry);
            regDestroy(pRegistry);
            pRegistry = NULL;
        }

        // import to the registry file(nwprofiles)
        pRegistry = regParseFiles( REG_DIR "/" REG_NWPROFILES_XML_FILE, 
                                   REG_NWPROFILES_XSD_PATH             );
        if (pRegistry)
        {
            ERREG_CFG_LOGPRINTF("entry");
           
            nwRegInitXpathList(pRegistry);
            
            cfgImportNetworkProfiles(pNetworkConfig, (const regNetworkProfile_t**)pNetworks, pRegistry);
            
            // store to file
            cfgStore(pRegistry);
        
            nwRegDestroyXpathList(pRegistry);
            regDestroy(pRegistry);
            pRegistry = NULL;
        }

        // free the memory
        regFreeUserProfile(theUserProfile);
        regFreeUserSetting(theUserSetting);
        regFreePCProfiles(pPCConfig, pPCs);
        regFreeNetworkProfiles(pNetworkConfig, pNetworks);
        regFreeAutoConnect(theAutoConnect);
        regFreeStartUp(theStartUp);
    }
   
    // after imported, we delete iliadconfig.xml and iliadconfig-ics.xml
    unlink(CONFIG_DIR "/" IMPORT_CONFIG_XML_FILE);
    unlink(CONFIG_DIR "/" EXPORT_CONFIG_XML_FILE);

    return bRet;
}

gboolean erRegExportConfig(void)
{
    gboolean needExport = FALSE;
    gboolean bRet = TRUE;
  
    ERREG_CFG_LOGPRINTF("entry");
 
    if (!cfgFileExits(CONFIG_DIR))
    {
        // firstly, create the dir if needed
        if (mkdir(CONFIG_DIR, 0755) != 0)
        {
            ERREG_CFG_ERRORPRINTF("Unable to create dir: %s", CONFIG_DIR);
            return FALSE;
        }
    }
    else
    {
        // before exported, we delete iliadconfig.xml and iliadconfig-ics.xml, just in case
        unlink(CONFIG_DIR "/" IMPORT_CONFIG_XML_FILE);
        unlink(CONFIG_DIR "/" EXPORT_CONFIG_XML_FILE);
    }

    // get the settings need to export(basis)
    regUserProfile_t *theUserProfile = NULL; 
    regUserSetting_t *theUserSetting = NULL;
    regPCConfig_t *pPCConfig = NULL;
    regPCProfile_t **pPCs = NULL;
    regAutoConnect_t *theAutoConnect = NULL;
    regStartUp_t *theStartUp = NULL;
    deviceDetails_t* theDeviceDetails = NULL;
    regRegistry_t* pRegistry = regParseFiles( REG_DIR "/" REG_BASIS_XML_FILE, 
                                              REG_BASIS_XSD_PATH             );
    if (pRegistry)
    {
        ERREG_CFG_LOGPRINTF("entry");
        needExport = TRUE;

        basisRegInitXpathList(pRegistry);

        theUserProfile = regGetUserProfile(pRegistry);
        theUserSetting = regGetUserSetting(pRegistry);
        
        regGetPCProfiles(&pPCConfig, &pPCs, pRegistry);

        theAutoConnect = regGetAutoConnect(pRegistry);
        theStartUp = regGetStartUp(pRegistry);
        
        theDeviceDetails = regGetDeviceDetails(pRegistry);
        
        basisRegDestroyXpathList(pRegistry);
        regDestroy(pRegistry);
        pRegistry = NULL;
    }

    // get the settings need to export(nwprofiles)
    regNetworkConfig_t *pNetworkConfig = NULL;
    regNetworkProfile_t **pNetworks = NULL;
    pRegistry = regParseFiles( REG_DIR "/" REG_NWPROFILES_XML_FILE, 
                               REG_NWPROFILES_XSD_PATH             );
    if (pRegistry)
    {
        ERREG_CFG_LOGPRINTF("entry");
        needExport = TRUE;

        nwRegInitXpathList(pRegistry);

        regGetNetworkProfiles(&pNetworkConfig, &pNetworks, pRegistry);

        nwRegDestroyXpathList(pRegistry);
        regDestroy(pRegistry);
        pRegistry = NULL;
    }

    if (needExport)
    {
        // exports the device configuration to iliadconfig.xml
        pRegistry = cfgCreateAndLoadFiles( CONFIG_DIR "/" EXPORT_CONFIG_XML_FILE,
                                           CONFIG_XSD_PATH                       );
        if (pRegistry)
        {
            ERREG_CFG_LOGPRINTF("entry");
    
            bRet = cfgExportUserProfile(theUserProfile, pRegistry);
            bRet = cfgExportUserSetting(theUserSetting, pRegistry);
            bRet = cfgExportPCProfiles(pPCConfig, pPCs, pRegistry);
            bRet = cfgExportNetworkProfiles(pNetworkConfig, (const regNetworkProfile_t**)pNetworks, pRegistry);
            bRet = cfgExportAutoConnect(theAutoConnect, pRegistry);
            bRet = cfgExportStartUp(theStartUp, pRegistry);
            bRet = cfgExportDeviceDetails(theDeviceDetails, pRegistry);

            cfgSaveAndClose(pRegistry);
            pRegistry = NULL;
        }
    }
    
    // free the memory
    regFreeUserProfile(theUserProfile);
    regFreeUserSetting(theUserSetting);
    regFreePCProfiles(pPCConfig, pPCs);
    regFreeNetworkProfiles(pNetworkConfig, pNetworks);
    regFreeAutoConnect(theAutoConnect);
    regFreeStartUp(theStartUp);
    regFreeDeviceDetails(theDeviceDetails);

    return bRet;
}

 *
 ** NOT TESTED YET **/

