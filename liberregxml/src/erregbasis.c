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
 * \file   erregbasis.c 
 * \brief  liberreg - ereader basis registry
 *                    loading/recovering and unloading basis registry
 *                    getting and setting functions
 *                    free memory functions
 *                    build-in basis registry
 * 
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <glib.h>

#include "erreglog.h"
#include "erreg.h"
#include "erregbasis.h"
#include "erregrwlock.h"
#include "erregtypes_f.h"



///////////////////////////////////////////////////////////////////////////////
//
//  minset basis profiles(build-in)
//
///////////////////////////////////////////////////////////////////////////////
static const gchar scMinsetString[] = 
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<iLiadRegistry>\n"
"    <version>\n"
"        <registry>21002</registry>\n"
"    </version>\n"
"    <userProfile>\n"
"        <name/>\n"
"        <email/>\n"
"        <password/>\n"
"        <redirectUrl>https://ids.irexnet.com:443/redirector</redirectUrl>\n"
"    </userProfile>\n"
"    <userSetting>\n"
"        <language>en_US</language>\n"
"        <sound>\n"
"            <enable>false</enable>\n"
"            <volume>75</volume>\n"
"        </sound>\n"
"        <longKeyPressTimeout>1000</longKeyPressTimeout>\n"
"        <flipbarDirection>true</flipbarDirection>\n"
"        <keyBuffering>\n"
"            <enable>false</enable>\n"
"            <numberOfKeys>5</numberOfKeys>\n"
"        </keyBuffering>\n"
"    </userSetting>\n"
"    <pcProfileList>\n"
"        <pcProfile profileID=\"PC_0\">\n"
"            <profileName/>\n"
"            <pcName>MyComputer</pcName>\n"
"            <shareName>iLiad</shareName>\n"
"            <workgroup>mshome</workgroup>\n"
"            <userName/>\n"
"            <password/>\n"
"        </pcProfile>\n"
"    </pcProfileList>\n"
"    <autoConnect>\n"
"        <serverType>IDS</serverType>\n"
"        <background>\n"
"            <interval>15</interval>\n"
"            <enable>false</enable>\n"
"        </background>\n"
"        <timedIds>\n"
"            <enable>false</enable>\n"
"            <timeSet>\n"
"                <time>13:00:00</time>\n"
"            </timeSet>\n"
"            <swUpdate>false</swUpdate>\n"
"            <switchOff>false</switchOff>\n"
"        </timedIds>\n"
"    </autoConnect>\n"
"    <categories>\n"
"        <category type=\"MODE\">\n"
"            <location>/var/mode</location>\n"
"            <sort>\n"
"                <field>FSNAME</field>\n"
"                <ascending>true</ascending>\n"
"            </sort>\n"
"        </category>\n"
"    </categories>\n"
"    <usbExport>MAIN</usbExport>\n"
"    <applications>\n"
"        <application type=\"SETUP\">\n"
"            <appCategory>setup</appCategory>\n"
"            <execute>/usr/bin/setup</execute>\n"
"            <argMask/>\n"
"            <xResourceName>setup</xResourceName>\n"
"            <ipcChannel>3</ipcChannel>\n"
"            <appIconPath/>\n"
"        </application>\n"
"        <application type=\"PROFILES\">\n"
"            <appCategory>setup</appCategory>\n"
"            <argMask/>\n"
"            <execute>/usr/bin/connectionMgr --edit-only</execute>\n"
"            <xResourceName>connectionMgr</xResourceName>\n"
"            <ipcChannel>12</ipcChannel>\n"
"            <appIconPath/>\n"
"        </application>\n"
"    </applications>\n"
"    <startup>\n"
"        <behaviour>DEVICE_MANAGER</behaviour>\n"
"        <documentPath/>\n"
"    </startup>\n"
"</iLiadRegistry>\n";

void basisRegInitXpathList(regRegistry_t * pRegistry)
{
    ERREG_BASIS_LOGPRINTF("entry");
    g_assert(pRegistry);

    xpathList_t *xpaths = g_new0(xpathList_t, 1);
    g_assert(xpaths);

    xpaths->version        = "/" EL_REGISTRY_ROOT "/" EL_VERSION;
    xpaths->userProfile    = "/" EL_REGISTRY_ROOT "/" EL_USER_PROFILE;
    xpaths->userSetting    = "/" EL_REGISTRY_ROOT "/" EL_USER_SETTING;
    xpaths->pcProfiles     = "/" EL_REGISTRY_ROOT "/" EL_PC_PROFILES;
    xpaths->pcProfile      = "/" EL_REGISTRY_ROOT "/" EL_PC_PROFILES       "/" EL_PC_PROFILE;
    xpaths->autoConnect    = "/" EL_REGISTRY_ROOT "/" EL_AUTO_CONNECT;
    xpaths->category       = "/" EL_REGISTRY_ROOT "/" EL_CATEGORIES        "/" EL_CATEGORY;
    xpaths->usbExport      = "/" EL_REGISTRY_ROOT "/" EL_EXPORT_MEMORYTYPE;
    xpaths->lastRead       = "/" EL_REGISTRY_ROOT "/" EL_LASTREADS         "/" EL_LASTREAD;
    xpaths->application    = "/" EL_REGISTRY_ROOT "/" EL_APPLICATIONS      "/" EL_APPLICATION;
    xpaths->extensionInfos = "/" EL_REGISTRY_ROOT "/" EL_EXTENSION_INFOS;
    xpaths->extensionInfo  = "/" EL_REGISTRY_ROOT "/" EL_EXTENSION_INFOS   "/" EL_EXTENSION_INFO;        
    xpaths->startup        = "/" EL_REGISTRY_ROOT "/" EL_START_UP;

    pRegistry->xpaths = xpaths;
}

void basisRegDestroyXpathList(regRegistry_t * pRegistry)
{
    ERREG_BASIS_LOGPRINTF("entry");
    g_assert(pRegistry);

    g_free(pRegistry->xpaths);
    pRegistry->xpaths = NULL;
}

static regRegistry_t* basisRegLoadFromMinSet(void)
{
    ERREG_BASIS_LOGPRINTF("entry");

    regRegistry_t *pRegistry = NULL;
    gboolean       bRet = FALSE;

    if (scMinsetString)
    {
        // malloc memory
        pRegistry = g_new0(regRegistry_t, 1);
        g_assert(pRegistry != NULL);
        
        // construct context for xml
        int nRet = ermXmlParseBuffer( scMinsetString,
                                      REG_DIR "/" REG_BASIS_XML_FILE,
                                      &(pRegistry->xmlCxt)           );
        if (RET_OK == nRet)
        {
            // construct context for xsd
            bRet = xsdLoad(&(pRegistry->xsdCxt), REG_BASIS_XSD_PATH);
            if (bRet)
            { 
                // after loading xml and xsd into memory, validate xml against xsd
                bRet = regValidate(pRegistry);
            }
        }

        // if error occurrs when loading registry into memory, free the memory
        if (FALSE == bRet)
        {
            regDestroy(pRegistry);
            pRegistry = NULL;
        }
    }

    return pRegistry;
}


regPCConfig_t *basisRegNewPCConfig(int size)
{
    ERREG_BASIS_LOGPRINTF("entry");

    regPCConfig_t *thePCConfig = NULL;

    thePCConfig = g_new0(regPCConfig_t, 1);
    if (thePCConfig)
    {
        int i = 0;

        thePCConfig->size = size;
        thePCConfig->pcList = g_new0(char *, thePCConfig->size + 1);
        for (i = 0; i < size; i++)
        {
            thePCConfig->pcList[i] = g_strdup_printf("PC_%d", i);
        }
        thePCConfig->pcList[size] = NULL;
    }
    else
    {
        ERREG_BASIS_ERRORPRINTF("memory allocation error");
    }
    
    regDumpPCConfig(thePCConfig);
   
    return thePCConfig;
}


///////////////////////////////////////////////////////////////////////////////
//
// load/unload/recover basis registry
//
///////////////////////////////////////////////////////////////////////////////

regLoad_t basisRegLoad(regRegistry_t** ppRegistry)
{
    ERREG_BASIS_LOGPRINTF("entry");

    const char* path_registry = REG_DIR      "/" REG_BASIS_XML_FILE;
    const char* path_template = TEMPLATE_DIR "/" REG_BASIS_XML_FILE;
    const char* path_xsd      = REG_BASIS_XSD_PATH;

    regRegistry_t* pRegistry = NULL;
    int ret = loadOk_t;

    // load normal registry file
    pRegistry = regParseFiles(path_registry, path_xsd);
    if (pRegistry)
    {
        ERREG_BASIS_LOGPRINTF("Loading basis registry succeeded.");
        ret = loadOk_t;
    }
    else
    {
        // open failed: try template registry
        pRegistry = regParseFiles(path_template, path_xsd); 
        if (pRegistry)
        {
            ERREG_BASIS_WARNPRINTF("Loading basis registry failed, "
                                   "but loading template registry succeeded.");
            ret = loadTemplate_t;
        }
        else
        {
            // open failed: try minimum set
            ERREG_BASIS_ERRORPRINTF("Loading basis registry failed "
                                    "and loading template failed.");
            pRegistry = basisRegLoadFromMinSet();
            if (pRegistry)
            {
                ret = loadMinSet_t;
            }
            else
            {
                ERREG_BASIS_ERRORPRINTF("Loading basis registry "
                                        "and template and minset failed.");
                ret = loadError_t;
            }
        }

        // store the recovered registry values
        if (pRegistry)
        {
            pRegistry->changed = TRUE;
            if (erRegGetLockState() == lock_write)
            {
                regStore(pRegistry, path_registry);
            }
        }
    }

    // add frequently used xpaths
    if (pRegistry)
    {
        basisRegInitXpathList(pRegistry);
    }
    
    *ppRegistry = pRegistry;
    return ret;
}


void basisRegUnload(regRegistry_t* pRegistry) 
{
    ERREG_BASIS_LOGPRINTF("entry");

    if (pRegistry)
    {
        basisRegDestroyXpathList(pRegistry);
        regDestroy(pRegistry);
    }
}


gboolean basisRegStore(regRegistry_t* pRegistry)
{
    g_assert(pRegistry);

    return regStore(pRegistry, REG_DIR "/" REG_BASIS_XML_FILE);
}


///////////////////////////////////////////////////////////////////////////////
//
// get/set functions
//
///////////////////////////////////////////////////////////////////////////////

// check whether the PCProfile indicated by "ID" exists or not
// get the pointer of the certain PCProfile indicated by "ID" 
regPCProfile_t *basisRegGetPCProfileFromList(regPCConfig_t * pPCConfig, 
                        regPCProfile_t ** pPCs, const char *ID, int *index)
{
    ERREG_BASIS_LOGPRINTF("entry");

    int i;
    gboolean bExist = FALSE;
    regPCProfile_t *thePCProfile = NULL;

    // get the index by walking through the pcList
    for (i = 0; i < pPCConfig->size; i++)
    {
        if (!strcmp(pPCConfig->pcList[i], ID))
        {
            bExist = TRUE;
            *index = i;
            break;
        }
    }

    if (bExist)
    {
        thePCProfile = pPCs[*index];
    }

    return thePCProfile;
}

// put the new pc profile at the end of list
// update the size and names in regPCConfig_t
//
// return- the new PC profiles after adding
void basisRegAddPCProfileToList(regPCConfig_t ** pPCConfig, 
        regPCProfile_t *** pPCs, regPCProfile_t * pNewPCProfile)
{
    ERREG_BASIS_LOGPRINTF("entry");

    regPCConfig_t *pNewPCConfig = NULL, *pOldPCConfig = NULL;
    regPCProfile_t **pNewPCs = NULL, **pOldPCs = NULL;
    int i;

    pOldPCConfig = *pPCConfig;
    pOldPCs = *pPCs;

    // update the pNewPCConfig
    pNewPCConfig = basisRegNewPCConfig(pOldPCConfig->size+1);

    // update the pNewPCs
    pNewPCs = g_new0(regPCProfile_t *, pNewPCConfig->size);
    for (i = 0; i < pOldPCConfig->size; i++)
    {
        pNewPCs[i] = pOldPCs[i];
    }
    pNewPCs[pNewPCConfig->size - 1] = pNewPCProfile;

    // free the memory
    regFreePCConfig(pOldPCConfig);
    g_free(pOldPCs);

    // return
    *pPCConfig = pNewPCConfig;
    *pPCs = pNewPCs;
}

// update the size and profile names in regPCConfig_t
// update the profile name in regPCProfile_t
//
// return- the new PC profiles after deleting
void basisRegDeletePCProfileFromList(regPCConfig_t ** pPCConfig, 
                        regPCProfile_t *** pPCs, const char *ID)
{
    ERREG_BASIS_LOGPRINTF("entry");

    regPCConfig_t *pNewPCConfig = NULL, *pOldPCConfig = NULL;
    regPCProfile_t **pNewPCs = NULL, **pOldPCs = NULL;
    int i, index;

    pOldPCConfig = *pPCConfig;
    pOldPCs = *pPCs;

    // update the pNewPCConfig
    pNewPCConfig = basisRegNewPCConfig(pOldPCConfig->size-1);

    // update the pNewPCs
    pNewPCs = g_new0(regPCProfile_t *, pNewPCConfig->size);

    regPCProfile_t *pPCProfile = basisRegGetPCProfileFromList(pOldPCConfig, pOldPCs, ID, &index);

    for (i = 0; i < index; i++)
    {
        pNewPCs[i] = pOldPCs[i];
    }

    for (i = index + 1; i < pOldPCConfig->size; i++)
    {
        pPCProfile = pOldPCs[i];
        pNewPCs[i - 1] = pPCProfile;
    }

    // free the memory
    regFreePCConfig(pOldPCConfig);
    regFreePCProfile(pOldPCs[index]);
    g_free(pOldPCs);

    // return
    *pPCConfig = pNewPCConfig;
    *pPCs = pNewPCs;
}

// if the pc profile is set as default,  we put it on the top in the list
// the first one is "PC_0", the second is "PC_1" etc.
void basisRegSetPCProfileDefault(regPCConfig_t ** pPCConfig, 
                        regPCProfile_t *** pPCs, const char *ID)
{
    ERREG_BASIS_LOGPRINTF("entry");

    regPCConfig_t *pOldPCConfig = NULL;
    regPCProfile_t **pNewPCs = NULL, **pOldPCs = NULL;
    int i, index;

    pOldPCConfig = *pPCConfig;
    pOldPCs = *pPCs;

    // update the pNewPCs
    pNewPCs = g_new0(regPCProfile_t *, pOldPCConfig->size);

    regPCProfile_t *pPCProfile = basisRegGetPCProfileFromList(pOldPCConfig, pOldPCs, ID, &index);
    for (i = 0; i < pOldPCConfig->size; i++)
    {
        pPCProfile = pOldPCs[i];

        if (i < index)
        {
            pNewPCs[i + 1] = pOldPCs[i];
        }
        else if (i == index)
        {
            pNewPCs[0] = pPCProfile;
        }
        else                    // if (i > index)
        {
            pNewPCs[i] = pOldPCs[i];
        }
    }

    // free the memory
    g_free(pOldPCs);

    // return
    *pPCs = pNewPCs;
}

