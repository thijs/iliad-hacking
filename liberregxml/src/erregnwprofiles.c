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
 *\file   erregnwprofiles.c 
 *\brief  liberreg - ereader network profiles registry
 *                   loading and unloading network profiles registry
 *                   getting and setting functions
 *                   free memory functions
 *                   recovering network profiles registry
 *                   build-in network profiles registry
 *
 *Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <glib.h>

#include "erreglog.h"
#include "erreg.h"
#include "erregnwprofiles.h"
#include "erregrwlock.h"
#include "erregtypes_f.h"

///////////////////////////////////////////////////////////////////////////////
//
//  minset network profiles(build-in)
//
///////////////////////////////////////////////////////////////////////////////
static const gchar scNWMinSetString[] = 
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<iLiadRegistry>\n"
"    <networkProfileList>\n"
"        <networkProfile profileID=\"NW_0\">\n"
"            <profileName>Wired network</profileName>\n"
"            <networkType>WIRED</networkType>\n"
"            <useProxy>false</useProxy>\n"
"            <useDhcp>true</useDhcp>\n"
"        </networkProfile>\n"
"    </networkProfileList>\n"
"</iLiadRegistry>\n";

void nwRegInitXpathList(regRegistry_t *pRegistry)
{
    ERREG_NWPROFILES_LOGPRINTF("entry");
    g_assert(pRegistry);

    xpathList_t *xpaths = g_new0(xpathList_t, 1);
    g_assert(xpaths);

    xpaths->networkProfiles = "/" EL_REGISTRY_ROOT "/" EL_NETWORK_PROFILES;
    xpaths->networkProfile  = "/" EL_REGISTRY_ROOT "/" EL_NETWORK_PROFILES "/" EL_NETWORK_PROFILE;
    xpaths->lastConnect     = "/" EL_REGISTRY_ROOT "/" EL_LAST_CONNECT;
    xpaths->autoConnectWlan = "/" EL_REGISTRY_ROOT "/" EL_AUTO_CONNECT_WLAN;

    pRegistry->xpaths = xpaths;
}

void nwRegDestroyXpathList(regRegistry_t *pRegistry)
{
    ERREG_NWPROFILES_LOGPRINTF("entry");
    g_assert(pRegistry);

    g_free(pRegistry->xpaths);
    pRegistry->xpaths = NULL;
}

static regRegistry_t* nwRegLoadFromMinSet(void)
{
    regRegistry_t *pRegistry = NULL;
    gboolean       bRet = FALSE;

    ERREG_NWPROFILES_LOGPRINTF("entry");

    if (scNWMinSetString)
    {
        // malloc memory
        pRegistry = g_new0(regRegistry_t, 1);
        g_assert(pRegistry != NULL);
       
        // construct context for xml
        int nRet = ermXmlParseBuffer( scNWMinSetString, 
                                      REG_DIR "/" REG_NWPROFILES_XML_FILE,
                                      &(pRegistry->xmlCxt)                );
        if (RET_OK == nRet)
        {
            // construct context for xsd
            bRet = xsdLoad(&(pRegistry->xsdCxt), REG_NWPROFILES_XSD_PATH);
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

regNetworkConfig_t *nwRegNewNetworkConfig(int size)
{
    ERREG_NWPROFILES_LOGPRINTF("entry");

    regNetworkConfig_t *theNetworkConfig = NULL;

    theNetworkConfig = g_new0(regNetworkConfig_t, 1);
    if (theNetworkConfig)
    {
        int i = 0;

        theNetworkConfig->size = size;
        theNetworkConfig->networkList = g_new0(char *, theNetworkConfig->size + 1);
        for (i = 0; i < size; i++)
        {
            theNetworkConfig->networkList[i] = g_strdup_printf("NW_%d", i);
        }
        theNetworkConfig->networkList[size] = NULL;
    }
    else
    {
        ERREG_NWPROFILES_ERRORPRINTF("memory allocation error");
    }
    
    regDumpNetworkConfig(theNetworkConfig);

    return theNetworkConfig;
}

regNetworkProfile_t *nwRegNewNetworkProfile(const char *ID, connection_t connection)
{
    ERREG_NWPROFILES_LOGPRINTF("%s, %d", ID, connection);

    regNetworkProfile_t *theNetworkProfile = NULL;

    theNetworkProfile = g_new0(regNetworkProfile_t, 1);
    if (theNetworkProfile)
    {
        if (wired_t == connection)
        {
            theNetworkProfile->name = g_strdup(WIRED_NETWORK_NAME);
        }
        else if (wireless_t == connection)
        {
            theNetworkProfile->name = g_strdup(WIRELESS_NETWORK_NAME);
        }
        else
        {
            theNetworkProfile->name = g_strdup(ID);
        }

        // connection
        theNetworkProfile->connection = connection;

        // address mode 
        theNetworkProfile->addressMode = DEFAULT_ADDRESS_MODE;

        // proxy
        theNetworkProfile->proxy = DEFAULT_USE_PROXY;

        if (theNetworkProfile->connection == wireless_t)
        {
            theNetworkProfile->wirelessSettings = g_new0(regWirelessSetting_t, 1);
            if (theNetworkProfile->wirelessSettings)
            {
                // ssid
                theNetworkProfile->wirelessSettings->SSID = g_strdup(DEFAULT_WLAN_SSID);

                // encrType
                theNetworkProfile->wirelessSettings->encrType = DEFAULT_WLAN_ENCRYPTION_TYPE;
                if (theNetworkProfile->wirelessSettings->encrType != encr_none_t)
                {
                    // encrKey
                    theNetworkProfile->wirelessSettings->encrKey = g_strdup(DEFAULT_WLAN_ENCRYPTION_KEY);
                }

                // wlan broadcast
                theNetworkProfile->wirelessSettings->broadcast = DEFAULT_WLAN_BROADCAST;
            }
            else
            {
                ERREG_NWPROFILES_ERRORPRINTF("memory allocation error");
            }
        }
    }
    else
    {
        ERREG_NWPROFILES_ERRORPRINTF("memory allocation error");
    }

    regDumpNetworkProfile(ID, theNetworkProfile);

    return theNetworkProfile;
}

// new 2 network profiles with dhcp and no-proxy, the first one is wired, the second is wireless
void nwRegNewNetworkProfiles(regNetworkConfig_t **pNetworkConfig, regNetworkProfile_t ***pNetworks)
{
    ERREG_NWPROFILES_LOGPRINTF("entry");

    regNetworkConfig_t *theNetworkConfig = NULL;
    regNetworkProfile_t **theNetworks = NULL;
    
    theNetworkConfig = nwRegNewNetworkConfig(2);
    if (theNetworkConfig)
    {
         theNetworks = g_new0(regNetworkProfile_t *, theNetworkConfig->size);
         if (theNetworks)
         {
               theNetworks[0] = nwRegNewNetworkProfile(theNetworkConfig->networkList[0], wired_t);
               theNetworks[1] = nwRegNewNetworkProfile(theNetworkConfig->networkList[1], wireless_t);
         }
         else
         {
               ERREG_NWPROFILES_ERRORPRINTF("memory error");

               regFreeNetworkConfig(theNetworkConfig);
               theNetworkConfig = NULL;
         }
    }
    else
    {
         ERREG_NWPROFILES_ERRORPRINTF("memory error");
    }
    
    *pNetworkConfig = theNetworkConfig;
    *pNetworks = theNetworks;
}


///////////////////////////////////////////////////////////////////////////////
//
// load/unload/recover network profiles registry
//
///////////////////////////////////////////////////////////////////////////////

regLoad_t nwRegLoad (regRegistry_t** ppRegistry)
{
    ERREG_NWPROFILES_LOGPRINTF("entry");

    const char* path_registry = REG_DIR      "/" REG_NWPROFILES_XML_FILE;
    const char* path_template = TEMPLATE_DIR "/" REG_NWPROFILES_XML_FILE;
    const char* path_xsd      = REG_NWPROFILES_XSD_PATH;

    regRegistry_t* pRegistry = NULL;
    regLoad_t ret = loadOk_t;

    // load normal registry file
    pRegistry = regParseFiles(path_registry, path_xsd);
    if (pRegistry)
    {
        ERREG_NWPROFILES_LOGPRINTF("Loading nwprofiles registry succeeded.");
        ret = loadOk_t;
    }
    else
    {
        // open failed: try template registry
        pRegistry = regParseFiles(path_template, path_xsd);
        if (pRegistry)
        {
            ERREG_NWPROFILES_WARNPRINTF("Loading nwprofiles registry failed, "
                                        "but loading template succeeded.");
            ret = loadTemplate_t;
        }
        else
        {
            ERREG_NWPROFILES_ERRORPRINTF("Loading nwprofiles registry failed "
                                        "and loading template failed.");
            pRegistry = nwRegLoadFromMinSet();
            if (pRegistry)
            {
                ret = loadMinSet_t;
            }
            else
            {
                ERREG_NWPROFILES_ERRORPRINTF("Loading nwprofiles registry "
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
        nwRegInitXpathList(pRegistry);
    }

    *ppRegistry = pRegistry;
    return ret;

}

void nwRegUnload (regRegistry_t* pRegistry)
{
    ERREG_NWPROFILES_LOGPRINTF("entry");

    if (pRegistry)
    {
        nwRegDestroyXpathList(pRegistry);
        regDestroy(pRegistry);
    }
}

gboolean nwRegStore(regRegistry_t* pRegistry)
{
    g_assert(pRegistry);

    return regStore(pRegistry, REG_DIR "/" REG_NWPROFILES_XML_FILE);
}


///////////////////////////////////////////////////////////////////////////////
//
// get/set network profiles
//
///////////////////////////////////////////////////////////////////////////////


// check whether the NetworkProfile indicated by "ID" exists or not
// get the pointer of the certain NetworkProfile indicated by "ID" 
int nwRegGetNetworkProfileFromList(const regNetworkConfig_t *pNetworkConfig, 
                                   const char *ID)
{
    ERREG_NWPROFILES_LOGPRINTF("entry");

    int index = -1;  // return value: -1 = error
    int i;

    // get the index by walking through the networkList
    for (i = 0; i < pNetworkConfig->size; i++)
    {
        if (!strcmp(pNetworkConfig->networkList[i], ID))
        {
            index = i;
            break;
        }
    }

    return index;
}

// put the new network profile at the end of list
// update the size and names in regNetworkConfig_t
//
// return- the new Network profiles after adding
void nwRegAddNetworkProfileToList(regNetworkConfig_t **pNetworkConfig, 
                                  regNetworkProfile_t ***pNetworks,
                                  regNetworkProfile_t *pNewNetworkProfile)
{
    ERREG_NWPROFILES_LOGPRINTF("entry");

    regNetworkConfig_t *pNewNetworkConfig = NULL, *pOldNetworkConfig = NULL;
    regNetworkProfile_t **pNewNetworks = NULL, **pOldNetworks = NULL;
    int i;

    pOldNetworkConfig = *pNetworkConfig;
    pOldNetworks = *pNetworks;

    // update the pNewNetworkConfig
    pNewNetworkConfig = nwRegNewNetworkConfig(pOldNetworkConfig->size+1);

    // update the pNewNetworks
    pNewNetworks = g_new0(regNetworkProfile_t *, pNewNetworkConfig->size);
    for (i = 0; i < pOldNetworkConfig->size; i++)
    {
        pNewNetworks[i] = pOldNetworks[i];
    }
    pNewNetworks[pNewNetworkConfig->size - 1] = pNewNetworkProfile;

    // free the memory
    regFreeNetworkConfig(pOldNetworkConfig);
    g_free(pOldNetworks);

    // return
    *pNetworkConfig = pNewNetworkConfig;
    *pNetworks = pNewNetworks;
}

// update the size and profile names in regNetworkConfig_t
//
// return- the new Network profiles after deleting
void nwRegDeleteNetworkProfileFromList(regNetworkConfig_t **pNetworkConfig, 
                            regNetworkProfile_t ***pNetworks, const char *ID)
{
    ERREG_NWPROFILES_LOGPRINTF("entry");

    regNetworkConfig_t *pNewNetworkConfig = NULL, *pOldNetworkConfig = NULL;
    regNetworkProfile_t **pNewNetworks = NULL, **pOldNetworks = NULL;
    int i, index;

    pOldNetworkConfig = *pNetworkConfig;
    pOldNetworks = *pNetworks;

    // update the pNewNetworkConfig
    pNewNetworkConfig= nwRegNewNetworkConfig(pOldNetworkConfig->size-1);

    // update the pNewNetworks
    pNewNetworks = g_new0(regNetworkProfile_t *, pNewNetworkConfig->size);
    index = nwRegGetNetworkProfileFromList(pOldNetworkConfig, ID);
    for (i = 0; i < index; i++)
    {
        pNewNetworks[i] = pOldNetworks[i];
    }
    for (i = index + 1; i < pOldNetworkConfig->size; i++)
    {
        pNewNetworks[i - 1] = pOldNetworks[i];
    }

    // free the memory
    regFreeNetworkConfig(pOldNetworkConfig);
    regFreeNetworkProfile(pOldNetworks[index]);
    g_free(pOldNetworks);

    // return
    *pNetworkConfig = pNewNetworkConfig;
    *pNetworks = pNewNetworks;
}

// if the network profile is set as default,  we put it on the top in the list
// the first one is "NW_0", the second is "NW_1" etc.
void nwRegSetNetworkProfileDefault(regNetworkConfig_t **pNetworkConfig, 
                            regNetworkProfile_t ***pNetworks, const char *ID)
{
    ERREG_NWPROFILES_LOGPRINTF("entry");

    regNetworkConfig_t *pOldNetworkConfig = NULL;
    regNetworkProfile_t **pNewNetworks = NULL, **pOldNetworks = NULL;
    regNetworkProfile_t *pNetworkProfile = NULL;
    int i, index;

    pOldNetworkConfig = *pNetworkConfig;
    pOldNetworks = *pNetworks;

    // update the pNewNetworks
    pNewNetworks = g_new0(regNetworkProfile_t *, pOldNetworkConfig->size);
    index = nwRegGetNetworkProfileFromList(pOldNetworkConfig, ID);
    for (i = 0; i < pOldNetworkConfig->size; i++)
    {
        pNetworkProfile = pOldNetworks[i];

        if (i < index)
        {
            pNewNetworks[i + 1] = pOldNetworks[i];
        }
        else if (i == index)
        {
            pNewNetworks[0] = pNetworkProfile;
        }
        else                    // if (i > index)
        {
            pNewNetworks[i] = pOldNetworks[i];
        }
    }

    // free the memory
    g_free(pOldNetworks);

    // return
    *pNetworks = pNewNetworks;
}
