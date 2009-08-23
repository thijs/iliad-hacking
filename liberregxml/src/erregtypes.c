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
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "erreglog.h"
#include "erreg.h"
#include "erregtypes_f.h"
#include "erregxml.h"
#include "erregbasis.h"

void regDumpVersion(const regVersion_t * theVersion)
{
    if (theVersion)
    {
        ERREG_DUMPPRINTF("[%s]", EL_VERSION);
        ERREG_DUMPPRINTF("%s=%s", EL_REGISTRY, theVersion->registry);
    }
}

void regDumpUserProfile(const regUserProfile_t * theUserProfile)
{
    if (theUserProfile)
    {
        ERREG_DUMPPRINTF("[%s]", EL_USER_PROFILE);
        ERREG_DUMPPRINTF("%s=%s", EL_NAME, theUserProfile->name);
        ERREG_DUMPPRINTF("%s=%s", EL_EMAIL, theUserProfile->email);
        ERREG_DUMPPRINTF("%s=%s", EL_PASSWORD, theUserProfile->password);
        ERREG_DUMPPRINTF("%s=%s", EL_REDIRECT_URL, theUserProfile->redirectUrl);
    }
}

void regDumpUserSetting(const regUserSetting_t * theUserSetting)
{
    if (theUserSetting)
    {
        ERREG_DUMPPRINTF("[%s]", EL_USER_SETTING);
        ERREG_DUMPPRINTF("%s=%s", EL_LANGUAGE, theUserSetting->language);
        ERREG_DUMPPRINTF("%s=%d", EL_VOLUME, theUserSetting->volume);
        ERREG_DUMPPRINTF("%s=%d", EL_LONG_KP_TIMEOUT, theUserSetting->longkeypressTimeout);
        ERREG_DUMPPRINTF("%s=%d", EL_KEY_BUFFERING, theUserSetting->keyBuffering);
        ERREG_DUMPPRINTF("%s=%d", EL_FLIPBAR_DIRECTION, theUserSetting->flipbarLeftIsDown);
    }
}

void regDumpPCConfig(const regPCConfig_t * thePCConfig)
{
    if (thePCConfig)
    {
        int i;
        char szBuf[BUF_LEN + 1];

        g_snprintf(szBuf, BUF_LEN, "%s=", EL_PC_PROFILES);
        for (i = 0; i < thePCConfig->size; i++)
        {
            strcat(szBuf, thePCConfig->pcList[i]);
            strcat(szBuf, ";");
        }

        ERREG_DUMPPRINTF("[%s]", EL_PC_PROFILES);
        ERREG_DUMPPRINTF("%s", szBuf);
    }
}

void regDumpPCProfile(const char *ID, const regPCProfile_t * thePCProfile)
{
    if (thePCProfile)
    {
        ERREG_DUMPPRINTF("[%s]", ID);
        ERREG_DUMPPRINTF("%s=%s", EL_PROFILE_NAME, thePCProfile->name);
        ERREG_DUMPPRINTF("%s=%s", EL_PC_NAME, thePCProfile->pcname);
        ERREG_DUMPPRINTF("%s=%s", EL_SHARE_NAME, thePCProfile->sharename);
        ERREG_DUMPPRINTF("%s=%s", EL_WORKGROUP, thePCProfile->workgroup);
        ERREG_DUMPPRINTF("%s=%s", EL_USER_NAME, thePCProfile->username);
        ERREG_DUMPPRINTF("%s=%s", EL_PASSWORD, thePCProfile->password);
    }
}

void regDumpAutoConnect(const regAutoConnect_t * theAutoConnect)
{
    if (theAutoConnect)
    {
        ERREG_DUMPPRINTF("[%s]", EL_AUTO_CONNECT);
        ERREG_DUMPPRINTF("%s=%d", EL_ENABLE, theAutoConnect->backgroundEnable);
        ERREG_DUMPPRINTF("%s=%d", EL_SERVER_TYPE, theAutoConnect->backgroundConnectTo);
        ERREG_DUMPPRINTF("%s=%d", EL_INTERVAL, theAutoConnect->backgroundInterval);
    }
}

void regDumpTimedIds(const regTimedIds_t *theTimedIds)
{
    if (theTimedIds)
    {
        ERREG_DUMPPRINTF("[%s]",  EL_TIMED_IDS);
        ERREG_DUMPPRINTF("%s=%d", EL_ENABLE_TIMEDIDS,  theTimedIds->enable);

        int i;
        for (i=0; i<theTimedIds->timeCnt; i++)
        {
            ERREG_DUMPPRINTF("%s[%d]=%d", EL_TIME, i, theTimedIds->timeSet[i]);
        }
        
        ERREG_DUMPPRINTF("%s=%d", EL_ENABLE_SWUPDATE,  theTimedIds->swUpdate);
        ERREG_DUMPPRINTF("%s=%d", EL_ENABLE_SWITCHOFF, theTimedIds->switchOff);
    }
}

void regDumpContentCategory(const char *ID, const regContentCategory_t * theContentCategory)
{
    if (ID && theContentCategory)
    {
        ERREG_DUMPPRINTF("[%s]", ID);

        ERREG_DUMPPRINTF("%s=%s", EL_LOCATION, theContentCategory->location);

        ERREG_DUMPPRINTF("%s=%d", EL_SORT_ASCENDING, theContentCategory->sortAscending);
        ERREG_DUMPPRINTF("%s=%s", EL_SORT_FIELD_TYPE, theContentCategory->sortFieldType);
    }
}

void regDumpExportMemType(const regExportMemType_t *theExportMemType)
{
    if (theExportMemType)
    {
        ERREG_DUMPPRINTF("%s=%s", EL_EXPORT_MEMORYTYPE, theExportMemType->location);
    }
}

void regDumpLastRead(const char *ID, const regLastRead_t * theLastRead)
{
    if (ID && theLastRead)
    {
        ERREG_DUMPPRINTF("[%s]", ID);

        ERREG_DUMPPRINTF("%s=%s", EL_EXT_NAME, theLastRead->extension);
        ERREG_DUMPPRINTF("%s=%s", EL_DOCUMENT_PATH, theLastRead->documentPath);
        ERREG_DUMPPRINTF("%s=%s", EL_MANIFEST_PATH, theLastRead->manifestPath);
        ERREG_DUMPPRINTF("%s=%s", EL_POSITION, theLastRead->position);
    }
}

void regDumpUserAppList(const regUserAppList_t * theUserAppList)
{
    if (theUserAppList)
    {
        int i = 0;
        char szBuf[BUF_LEN + 1];

        g_snprintf(szBuf, BUF_LEN, "%s=", EL_APPLICATIONS);

        for (i = 0; i < theUserAppList->uaIDArraySize; i++)
        {
            strcat(szBuf, theUserAppList->uaIDArray[i]);
            strcat(szBuf, ";");
        }

        ERREG_DUMPPRINTF("[%s]", EL_APPLICATIONS);
        ERREG_DUMPPRINTF("%s", szBuf);
    }
}

void regDumpUserApp(const char *ID, const regUserApp_t * theUserApp)
{
    if (ID && theUserApp)
    {
        int i = 0;
        char szBuf[BUF_LEN + 1];

        ERREG_DUMPPRINTF("[%s]", ID);
        ERREG_DUMPPRINTF("%s=%s", EL_CATEGORY, theUserApp->szCategory);
        ERREG_DUMPPRINTF("%s=%s", EL_ARG_MASK, theUserApp->szArgMask);
        ERREG_DUMPPRINTF("%s=%s", EL_EXECUTE, theUserApp->szExec);
        ERREG_DUMPPRINTF("%s=%s", EL_X_RES_NAME, theUserApp->xResName);
        ERREG_DUMPPRINTF("%s=%d", EL_IPC_CHANNEL, theUserApp->ipcChannel);

        g_snprintf(szBuf, BUF_LEN, "%s=", EL_SUPPORT_FORMAT);
        for (i = 0; i < theUserApp->extensionArraySize; i++)
        {
            strcat(szBuf, theUserApp->extensionArray[i]);
            strcat(szBuf, ";");
        }

        ERREG_DUMPPRINTF("%s", szBuf);
    }
}

void regDumpExtInfo(const regExtInfo_t * theExtInfo)
{
    if (theExtInfo)
    {
        ERREG_DUMPPRINTF("[%s]", EL_EXTENSION_INFO);
        ERREG_DUMPPRINTF("%s=%s", EL_EXT_NAME, theExtInfo->extName);
        ERREG_DUMPPRINTF("%s=%s", EL_EXT_ICON, theExtInfo->iconLocation);
        ERREG_DUMPPRINTF("%s=%s", EL_ASSOCIATE_APP, theExtInfo->associateApp);
        ERREG_DUMPPRINTF("%s=%d", EL_ASSOCIATE_ORDER, theExtInfo->associateOrder);
    }
}

void regDumpExtInfoList(const regExtInfoList_t * theExtInfoList)
{
    if (theExtInfoList && theExtInfoList->extArraySize && theExtInfoList->extArray)
    {
        int i = 0;
        for(; i < theExtInfoList->extArraySize; ++i)
        {
            ERREG_DUMPPRINTF("[%s]", EL_EXTENSION_INFO);
            ERREG_DUMPPRINTF("%s=%s", EL_EXT_NAME, theExtInfoList->extArray[i].extName);
            ERREG_DUMPPRINTF("%s=%s", EL_EXT_ICON, theExtInfoList->extArray[i].iconLocation);
            ERREG_DUMPPRINTF("%s=%s", EL_ASSOCIATE_APP, theExtInfoList->extArray[i].associateApp);
            ERREG_DUMPPRINTF("%s=%d", EL_ASSOCIATE_ORDER, theExtInfoList->extArray[i].associateOrder);
        }
    }
}


void regDumpStartUp(const regStartUp_t * theStartUp)
{
    if (theStartUp)
    {
        ERREG_DUMPPRINTF("[%s]", EL_START_UP);
        ERREG_DUMPPRINTF("%s=%d", EL_BEHAVIOUR, theStartUp->behaviour);
        ERREG_DUMPPRINTF("%s=%s", EL_DOCUMENT_PATH, theStartUp->documentPath);
    }
}

void regDumpNetworkConfig(const regNetworkConfig_t * theNetworkConfig)
{
    if (theNetworkConfig)
    {
        int i;
        char szBuf[BUF_LEN + 1];

        g_snprintf(szBuf, BUF_LEN, "%s=", EL_NETWORK_PROFILES);
        for (i = 0; i < theNetworkConfig->size; i++)
        {
            strcat(szBuf, theNetworkConfig->networkList[i]);
            strcat(szBuf, ";");
        }

        ERREG_DUMPPRINTF("[%s]", EL_NETWORK_PROFILES);
        ERREG_DUMPPRINTF("%s", szBuf);
    }
}

void regDumpNetworkProfile(const char *ID, const regNetworkProfile_t * theNetworkProfile)
{
    if (ID && theNetworkProfile)
    {
        ERREG_DUMPPRINTF("[%s]", ID);
        ERREG_DUMPPRINTF("%s=%s", EL_PROFILE_NAME, theNetworkProfile->name);
        ERREG_DUMPPRINTF("%s=%d", EL_NETWORK_TYPE, theNetworkProfile->connection);
        ERREG_DUMPPRINTF("%s=%s", EL_USE_PROXY, (theNetworkProfile->proxy == TRUE) ? "TRUE" : "FALSE");
        ERREG_DUMPPRINTF("%s=%d", EL_USE_DHCP, theNetworkProfile->addressMode);
        ERREG_DUMPPRINTF("%s=%d", EL_TIMESTAMP, theNetworkProfile->timestamp);

        if (theNetworkProfile->wirelessSettings)
        {
            ERREG_DUMPPRINTF("%s=%s", EL_WLAN_SSID, theNetworkProfile->wirelessSettings->SSID);
            ERREG_DUMPPRINTF("%s=%d", EL_WLAN_ENCRYPTION_TYPE, theNetworkProfile->wirelessSettings->encrType);
            ERREG_DUMPPRINTF("%s=%s", EL_WLAN_ENCRYPTION_KEY, theNetworkProfile->wirelessSettings->encrKey);
            ERREG_DUMPPRINTF("%s=%d", EL_WLAN_BROADCAST, theNetworkProfile->wirelessSettings->broadcast);
        }

        if (theNetworkProfile->dialupSettings)
        {
            ERREG_DUMPPRINTF("%s=%s", EL_DIALUP_PHONE, theNetworkProfile->dialupSettings->phone);
            ERREG_DUMPPRINTF("%s=%s", EL_DIALUP_USER, theNetworkProfile->dialupSettings->user);
            ERREG_DUMPPRINTF("%s=%s", EL_DIALUP_PASSWORD, theNetworkProfile->dialupSettings->password);
        }

        if (theNetworkProfile->proxySettings)
        {
            ERREG_DUMPPRINTF("%s=%s", EL_PROXY_SERVER, theNetworkProfile->proxySettings->address);
            ERREG_DUMPPRINTF("%s=%s", EL_PROXY_PORT, theNetworkProfile->proxySettings->port);
        }

        if (theNetworkProfile->ipSettings)
        {
            ERREG_DUMPPRINTF("%s=%s", EL_IP_ADDRESS, theNetworkProfile->ipSettings->address);
            ERREG_DUMPPRINTF("%s=%s", EL_IP_NETMASK, theNetworkProfile->ipSettings->netmask);
            ERREG_DUMPPRINTF("%s=%s", EL_IP_DNS, theNetworkProfile->ipSettings->dns);
            ERREG_DUMPPRINTF("%s=%s", EL_IP_GATEWAY, theNetworkProfile->ipSettings->gateway);
        }
    }
}

void regDumpLastConnect(const regLastConnect_t * theLastConnect)
{
    if (theLastConnect)
    {
        ERREG_DUMPPRINTF("[%s]", EL_LAST_CONNECT);
        ERREG_DUMPPRINTF("%s=%d", EL_PROFILE_CONNECTED_IDS, theLastConnect->profileConnectedIDS);
        ERREG_DUMPPRINTF("%s=%d", EL_PROFILE_CONNECTED_PC, theLastConnect->profileConnectedPC);
        ERREG_DUMPPRINTF("%s=%d", EL_MANUAL_CONNECT_TYPE, theLastConnect->manualConnectType);
    }
}

void regDumpAutoConnectWlan(const gboolean enable)
{
    ERREG_DUMPPRINTF("[%s]", EL_AUTO_CONNECT_WLAN);
    ERREG_DUMPPRINTF("%s=%d", EL_ENABLE, enable);
}

void regDumpIconSet(const regIconSet_t * theIconSet)
{
    if (theIconSet)
    {
        regIcon_t *theIcon = NULL;
        int i, j;

        ERREG_DUMPPRINTF("[%s]", EL_TOOLBAR_ICONS);

        for (i = 0; i < theIconSet->nIcons; i++)
        {
            theIcon = (theIconSet->iconArray) + i;
            if (theIcon)
            {
                char szBuf[BUF_LEN + 1];

                if (theIcon->iconName)
                {
                    g_snprintf(szBuf, BUF_LEN, "%s=", theIcon->iconName);
                    for (j = 0; j < theIcon->iconPathsSize; j++)
                    {
                        strcat(szBuf, theIcon->iconPaths[j]);
                        strcat(szBuf, ";");
                    }               // for (j...)

                    ERREG_DUMPPRINTF("%s", szBuf);
                }
                else
                {
                    ERREG_LOGPRINTF("The %dth icon's name is NULL.", i);
                }
            }                   // theIcon
        }                       // for (i...)
    }                           // theIconSet
}

void regDumpDeviceDetails(const deviceDetails_t* theDeviceDetails)
{
    if (theDeviceDetails)
    {
        ERREG_DUMPPRINTF("[%s]", EL_DEVICE_DETAILS);
        
        if (theDeviceDetails->software)
        {
            ERREG_DUMPPRINTF("%s=%s", EL_BUILD_NAME, theDeviceDetails->software->szBuildName);
            ERREG_DUMPPRINTF("%s=%s", EL_BUILD_NUM, theDeviceDetails->software->szBuildNum);
        }
        
        ERREG_DUMPPRINTF("%s=%s", EL_MAC_OF_WIRED, theDeviceDetails->szMacOfWired);
        ERREG_DUMPPRINTF("%s=%s", EL_MAC_OF_WIRELESS, theDeviceDetails->szMacOfWireless);        
        
        ERREG_DUMPPRINTF("%s=%s", EL_PUBLIC_KEY, theDeviceDetails->szPublicKey);        
    }
}

regPCProfile_t *regDupPCProfile(const regPCProfile_t * thePCProfile)
{
    regPCProfile_t *ret = NULL;

    ERREG_LOGPRINTF("entry");

    if (thePCProfile)
    {
        ret = g_new0(regPCProfile_t, 1);

        if (thePCProfile->name)
        {
            ret->name = strdup(thePCProfile->name);
        }
        if (thePCProfile->pcname)
        {
            ret->pcname = strdup(thePCProfile->pcname);
        }
        if (thePCProfile->sharename)
        {
            ret->sharename = strdup(thePCProfile->sharename);
        }
        if (thePCProfile->username)
        {
            ret->username = strdup(thePCProfile->username);
        }
        if (thePCProfile->workgroup)
        {
            ret->workgroup = strdup(thePCProfile->workgroup);
        }
        if (thePCProfile->password)
        {
            ret->password = strdup(thePCProfile->password);
        }
    }
    return ret;
}

regStartUp_t *regDupStartUp(const regStartUp_t * theStartup)
{
    ERREG_LOGPRINTF("entry");

    regStartUp_t *ret = NULL;

    if (theStartup)
    {
        ret = g_new0(regStartUp_t, 1);
        if (ret)
        {
            ret->behaviour = theStartup->behaviour;
            if (theStartup->documentPath)
            {
                ret->documentPath = g_strdup(theStartup->documentPath);
            }
        }
    }

    return ret;
}


regNetworkProfile_t *regDupNetworkProfile(const regNetworkProfile_t * theNetworkProfile)
{
    ERREG_LOGPRINTF("entry");

    regNetworkProfile_t *ret = NULL;

    if (theNetworkProfile)
    {
        ret = g_new0(regNetworkProfile_t, 1);

        if (theNetworkProfile->name)
        {
            ret->name = g_strdup(theNetworkProfile->name);
        }
        ret->connection = theNetworkProfile->connection;
        ret->proxy = theNetworkProfile->proxy;
        ret->addressMode = theNetworkProfile->addressMode;

        if (theNetworkProfile->wirelessSettings)
        {
            ret->wirelessSettings = g_new0(regWirelessSetting_t, 1);

            if (theNetworkProfile->wirelessSettings->SSID)
            {
                ret->wirelessSettings->SSID = g_strdup(theNetworkProfile->wirelessSettings->SSID);
            }
            ret->wirelessSettings->encrType = theNetworkProfile->wirelessSettings->encrType;

            if (theNetworkProfile->wirelessSettings->encrKey)
            {
                ret->wirelessSettings->encrKey = g_strdup(theNetworkProfile->wirelessSettings->encrKey);
            }
        }

        if (theNetworkProfile->dialupSettings)
        {
            ret->dialupSettings = g_new0(regDialupSetting_t, 1);

            if (theNetworkProfile->dialupSettings->phone)
            {
                ret->dialupSettings->phone = g_strdup(theNetworkProfile->dialupSettings->phone);
            }
            if (theNetworkProfile->dialupSettings->user)
            {
                ret->dialupSettings->user = g_strdup(theNetworkProfile->dialupSettings->user);
            }
            if (theNetworkProfile->dialupSettings->password)
            {
                ret->dialupSettings->password = g_strdup(theNetworkProfile->dialupSettings->password);
            }  
        }

        if (theNetworkProfile->proxySettings)
        {
            ret->proxySettings = g_new0(regProxySetting_t, 1);

            if (theNetworkProfile->proxySettings->address)
            {
                ret->proxySettings->address = g_strdup(theNetworkProfile->proxySettings->address);
            }

            if (theNetworkProfile->proxySettings->port)
            {
                ret->proxySettings->port = g_strdup(theNetworkProfile->proxySettings->port);
            }
        }

        if (theNetworkProfile->ipSettings)
        {
            ret->ipSettings = g_new0(regIpSetting_t, 1);

            if (theNetworkProfile->ipSettings->address)
            {
                ret->ipSettings->address = g_strdup(theNetworkProfile->ipSettings->address);
            }

            if (theNetworkProfile->ipSettings->netmask)
            {
                ret->ipSettings->netmask = g_strdup(theNetworkProfile->ipSettings->netmask);
            }

            if (theNetworkProfile->ipSettings->dns)
            {
                ret->ipSettings->dns = g_strdup(theNetworkProfile->ipSettings->dns);
            }

            if (theNetworkProfile->ipSettings->gateway)
            {
                ret->ipSettings->gateway = g_strdup(theNetworkProfile->ipSettings->gateway);
            }
        }
    }

    return ret;
}

void regFreeVersion(regVersion_t * theVersion)
{
    ERREG_LOGPRINTF("entry");

    if (theVersion)
    {
        if (theVersion->registry)
        {
            g_free(theVersion->registry);
            theVersion->registry = NULL;
        }
        g_free(theVersion);
    }
}

void regFreeUserProfile(regUserProfile_t * theUserProfile)
{
    ERREG_LOGPRINTF("entry");

    if (theUserProfile)
    {
        if (theUserProfile->name)
        {
            g_free(theUserProfile->name);
            theUserProfile->name = NULL;
        }

        if (theUserProfile->email)
        {
            g_free(theUserProfile->email);
            theUserProfile->email = NULL;
        }

        if (theUserProfile->email)
        {
            g_free(theUserProfile->password);
            theUserProfile->password = NULL;
        }

        if (theUserProfile->redirectUrl)
        {
            g_free(theUserProfile->redirectUrl);
            theUserProfile->redirectUrl = NULL;
        }

        g_free(theUserProfile);
    }
}

void regFreeUserSetting(regUserSetting_t * theUserSetting)
{
    ERREG_LOGPRINTF("entry");

    if (theUserSetting)
    {
        if (theUserSetting->language)
        {
            g_free(theUserSetting->language);
            theUserSetting->language = NULL;
        }

        g_free(theUserSetting);
    }
}

void regFreePCConfig(regPCConfig_t * thePCConfig)
{
    ERREG_LOGPRINTF("entry");

    if (thePCConfig)
    {
        if (thePCConfig->pcList)
        {
            g_strfreev(thePCConfig->pcList);
        }
        g_free(thePCConfig);
    }
}

void regFreePCProfile(regPCProfile_t * thePCProfile)
{
    ERREG_LOGPRINTF("entry");

    if (thePCProfile)
    {
        if (thePCProfile->name)
        {
            g_free(thePCProfile->name);
            thePCProfile->name = NULL;
        }
        
        if (thePCProfile->pcname)
        {
            g_free(thePCProfile->pcname);
            thePCProfile->pcname = NULL;
        }
        if (thePCProfile->sharename)
        {
            g_free(thePCProfile->sharename);
            thePCProfile->sharename = NULL;
        }
        if (thePCProfile->username)
        {
            g_free(thePCProfile->username);
            thePCProfile->username = NULL;
        }
        if (thePCProfile->password)
        {
            g_free(thePCProfile->password);
            thePCProfile->password = NULL;
        }
        if (thePCProfile->workgroup)
        {
            g_free(thePCProfile->workgroup);
            thePCProfile->workgroup = NULL;
        }
        g_free(thePCProfile);
    }
}

void regFreePCProfiles(regPCConfig_t * thePCConfig, regPCProfile_t ** thePCs)
{
    ERREG_LOGPRINTF("entry");

    if (thePCConfig != NULL && thePCs != NULL)
    {
        int i;
        // free each profile
        for (i = 0; i < thePCConfig->size; i++)
        {
            regFreePCProfile(thePCs[i]);
        }
        g_free(thePCs);

        // free the config
        regFreePCConfig(thePCConfig);
    }
}

void regFreeAutoConnect(regAutoConnect_t * theAutoConnect)
{
    ERREG_LOGPRINTF("entry");

    if (theAutoConnect)
    {
        g_free(theAutoConnect);
    }
}

void regFreeTimedIds(regTimedIds_t *theTimedIds)
{
    ERREG_LOGPRINTF("entry");

    if (theTimedIds)
    {
        g_free(theTimedIds);
    }
}

void regFreeContentCategory(regContentCategory_t * theContentCategory)
{
    ERREG_LOGPRINTF("entry");

    if (theContentCategory)
    {
        if (theContentCategory->location)
        {
            g_free(theContentCategory->location);
            theContentCategory->location = NULL;
        }

        if (theContentCategory->sortFieldType)
        {
            g_free(theContentCategory->sortFieldType);
            theContentCategory->sortFieldType = NULL;
        }

        g_free(theContentCategory);
    }
}

void regFreeExportMemType(regExportMemType_t *theExportMemType)
{
    ERREG_LOGPRINTF("entry");

    if (theExportMemType)
    {
        if (theExportMemType->location)
        {
            g_free(theExportMemType->location);
            theExportMemType->location = NULL;
        }

        g_free(theExportMemType);
    }
}

void regFreeLastRead(regLastRead_t * theLastRead)
{
    ERREG_LOGPRINTF("entry");

    if (theLastRead)
    {
        if (theLastRead->extension)
        {
            g_free(theLastRead->extension);
            theLastRead->extension = NULL;
        }
        if (theLastRead->documentPath)
        {
            g_free(theLastRead->documentPath);
            theLastRead->documentPath = NULL;
        }
        if (theLastRead->manifestPath)
        {
            g_free(theLastRead->manifestPath);
            theLastRead->manifestPath = NULL;
        }
        if (theLastRead->position)
        {
            g_free(theLastRead->position);
            theLastRead->position = NULL;
        }
        g_free(theLastRead);
    }
}

void regFreeUserAppList(regUserAppList_t * theUserAppList)
{
    ERREG_LOGPRINTF("entry");

    if (theUserAppList)
    {
        g_strfreev(theUserAppList->uaIDArray);
        g_free(theUserAppList);
    }
}

void regFreeUserApp(regUserApp_t * theUserApp)
{
    ERREG_LOGPRINTF("entry");

    if (theUserApp)
    {
        g_strfreev(theUserApp->extensionArray);

        if (theUserApp->szCategory)
        {
            g_free(theUserApp->szCategory);
            theUserApp->szCategory = NULL;
        }

        if (theUserApp->szArgMask)
        {
            g_free(theUserApp->szArgMask);
            theUserApp->szArgMask = NULL;
        }
        if (theUserApp->szExec)
        {
            g_free(theUserApp->szExec);
            theUserApp->szExec = NULL;
        }
        if (theUserApp->xResName)
        {
            g_free(theUserApp->xResName);
            theUserApp->xResName = NULL;
        }
        g_free(theUserApp);
    }
}

void regFreeExtInfoList(regExtInfoList_t * theExtInfoList)
{
    ERREG_LOGPRINTF("entry");

    if (theExtInfoList)
    {
        int i = 0; 
        for(; i < theExtInfoList->extArraySize; ++i)
        {
            g_free(theExtInfoList->extArray[i].extName);
            g_free(theExtInfoList->extArray[i].associateApp);
            g_free(theExtInfoList->extArray[i].iconLocation);
        }
        g_free(theExtInfoList->extArray);
        g_free(theExtInfoList);
    }
}

void regFreeExtInfo(regExtInfo_t * theExtInfo)
{
    ERREG_LOGPRINTF("entry");

    if (theExtInfo)
    {
        if (theExtInfo->iconLocation)
        {
            g_free(theExtInfo->iconLocation);
            theExtInfo->iconLocation = NULL;
        }

        g_free(theExtInfo);
    }
}

void regFreeStartUp(regStartUp_t * theStartUp)
{
    ERREG_LOGPRINTF("entry");

    if (theStartUp)
    {
        if (theStartUp->documentPath)
        {
            g_free(theStartUp->documentPath);
            theStartUp->documentPath = NULL;
        }

        g_free(theStartUp);
    }
}

void regFreeWirelessSetting(regWirelessSetting_t * theWirelessSetting)
{
    ERREG_LOGPRINTF("entry");

    if (theWirelessSetting)
    {
        if (theWirelessSetting->SSID)
        {
            g_free(theWirelessSetting->SSID);
            theWirelessSetting->SSID = NULL;
        }

        if (theWirelessSetting->encrKey)
        {
            g_free(theWirelessSetting->encrKey);
            theWirelessSetting->encrKey = NULL;
        }

        g_free(theWirelessSetting);
    }
}

void regFreeDialupSetting(regDialupSetting_t * theDialupSetting)
{
    ERREG_LOGPRINTF("entry");

    if (theDialupSetting)
    {
        if (theDialupSetting->phone)
        {
            g_free(theDialupSetting->phone);
            theDialupSetting->phone = NULL;
        }

        if (theDialupSetting->user)
        {
            g_free(theDialupSetting->user);
            theDialupSetting->user = NULL;
        }

        if (theDialupSetting->password)
        {
            g_free(theDialupSetting->password);
            theDialupSetting->password = NULL;
        }
        
        g_free(theDialupSetting);
    }
}

void regFreeProxySetting(regProxySetting_t * theProxySetting)
{
    ERREG_LOGPRINTF("entry");

    if (theProxySetting)
    {
        if (theProxySetting->address)
        {
            g_free(theProxySetting->address);
            theProxySetting->address = NULL;
        }

        if (theProxySetting->port)
        {
            g_free(theProxySetting->port);
            theProxySetting->port = NULL;
        }

        g_free(theProxySetting);
    }
}

void regFreeIpSetting(regIpSetting_t * theIpSetting)
{
    ERREG_LOGPRINTF("entry");

    if (theIpSetting)
    {
        if (theIpSetting->address)
        {
            g_free(theIpSetting->address);
            theIpSetting->address = NULL;
        }

        if (theIpSetting->netmask)
        {
            g_free(theIpSetting->netmask);
            theIpSetting->netmask = NULL;
        }

        if (theIpSetting->dns)
        {
            g_free(theIpSetting->dns);
            theIpSetting->dns = NULL;
        }

        if (theIpSetting->gateway)
        {
            g_free(theIpSetting->gateway);
            theIpSetting->gateway = NULL;
        }

        g_free(theIpSetting);
    }
}

void regFreeNetworkProfile(regNetworkProfile_t * theNetworkProfile)
{
    ERREG_LOGPRINTF("entry");

    if (theNetworkProfile)
    {
        if (theNetworkProfile->name)
        {
            g_free(theNetworkProfile->name);
            theNetworkProfile->name = NULL;
        }

        if (theNetworkProfile->wirelessSettings)
        {
            regFreeWirelessSetting(theNetworkProfile->wirelessSettings);
            theNetworkProfile->wirelessSettings = NULL;
        }

        if (theNetworkProfile->dialupSettings)
        {
            regFreeDialupSetting(theNetworkProfile->dialupSettings);
            theNetworkProfile->dialupSettings = NULL;
        }

        if (theNetworkProfile->proxySettings)
        {
            regFreeProxySetting(theNetworkProfile->proxySettings);
            theNetworkProfile->proxySettings = NULL;
        }

        if (theNetworkProfile->ipSettings)
        {
            regFreeIpSetting(theNetworkProfile->ipSettings);
            theNetworkProfile->ipSettings = NULL;
        }
        g_free(theNetworkProfile);
    }
}

void regFreeNetworkConfig(regNetworkConfig_t * theNetworkConfig)
{
    ERREG_LOGPRINTF("entry");

    if (theNetworkConfig)
    {
        if (theNetworkConfig->networkList)
        {
            g_strfreev(theNetworkConfig->networkList);
        }
        g_free(theNetworkConfig);
    }
}

void regFreeNetworkProfiles(regNetworkConfig_t * theNetworkConfig, regNetworkProfile_t ** theNetworks)
{
    ERREG_LOGPRINTF("entry");

    if (theNetworkConfig != NULL && theNetworks != NULL)
    {
        int i;
        // free each profile
        for (i = 0; i < theNetworkConfig->size; i++)
        {
            regFreeNetworkProfile(theNetworks[i]);
        }
        g_free(theNetworks);

        // free the config
        regFreeNetworkConfig(theNetworkConfig);
    }
}

void regFreeLastConnect(regLastConnect_t * theLastConnect)
{
    ERREG_LOGPRINTF("entry");

    if (theLastConnect)
    {
        g_free(theLastConnect);
    }
}

void regFreeIconSet(regIconSet_t * theIconSet)
{
    ERREG_LOGPRINTF("entry");

    if (theIconSet)
    {
        int i;

        if (theIconSet->iconArray)
        {
            for (i = 0; i < theIconSet->nIcons; i++)
            {
                g_strfreev(theIconSet->iconArray[i].iconPaths);
                g_free(theIconSet->iconArray[i].iconName);
            }
            g_free(theIconSet->iconArray);
        }
        g_free(theIconSet);
    }
}

void regFreeDeviceDetails(deviceDetails_t* theDeviceDetails)
{
    ERREG_LOGPRINTF("entry");

    if (theDeviceDetails)
    {
        if (theDeviceDetails->software)
        {
            g_free(theDeviceDetails->software->szBuildName);
            g_free(theDeviceDetails->software->szBuildNum);
            g_free(theDeviceDetails->software);
        }
        
        g_free(theDeviceDetails->szMacOfWired);
        g_free(theDeviceDetails->szMacOfWireless);
        g_free(theDeviceDetails->szPublicKey);
                        
        g_free(theDeviceDetails);
    }
}

connectTo_t regServerTypeToEnum(const char*string)
{
    connectTo_t serverType = ids_t;

    if (string)
    {
        if (strcmp(string, IDS) == 0)
        {
            serverType = ids_t;
        }
        else if (strcmp(string, PC) == 0)
        {
            serverType = pc_t;
        }
        else
        {
            serverType = ids_t;
        }
    }
    ERREG_LOGPRINTF("%s-->%d", string, serverType);
    return serverType;
}

void regServerTypeToString(connectTo_t serverType, char* string, int len)
{
    if (string && len > 0)
    {
        if (serverType == ids_t)
        {
            g_snprintf(string, len, "%s", IDS);
        }
        else if (serverType == pc_t)
        {
            g_snprintf(string, len, "%s", PC);
        }
        else
        {
            g_snprintf(string, len, "%s", IDS);
        }
    }
    ERREG_LOGPRINTF("%d-->%s", serverType, string);
}

startup_behaviour_t regBehaviourToEnum(const char*string)
{
    startup_behaviour_t behaviourType = DEFAULT_STARTUP_BEHAVIOUR;

    if (string)
    {
        if (strcmp(string, SPECIFIC_DOCUMENT) == 0)
        {
            behaviourType = specificDocument_t;
        }
        else if (strcmp(string, REFERENCE_MATERIALS) == 0)
        {
            behaviourType = referenceMaterials_t;
        }
        else if (strcmp(string, LAST_READ_DOCUMENT) == 0)
        {
            behaviourType = lastReadDocument_t;
        }
        else if (strcmp(string, RECENT_DOCUMENTS) == 0)
        {
            behaviourType = recentDocuments_t;
        }
        else if (strcmp(string, DEVICE_MANAGER) == 0)
        {
            behaviourType = deviceManager_t;
        }
        else if (strcmp(string, NEWS_FOLDER) == 0)
        {
            behaviourType = newsFolder_t;
        }
        else if (strcmp(string, BOOKS_FOLDER) == 0)
        {
            behaviourType = booksFolder_t;
        }
        else if (strcmp(string, DOCS_FOLDER) == 0)
        {
            behaviourType = docsFolder_t;
        }
        else if (strcmp(string, NOTES_FOLDER) == 0)
        {
            behaviourType = notesFolder_t;
        }
    }

    ERREG_LOGPRINTF("%s-->%d", string, behaviourType);
    return behaviourType;
}

void regBehaviourToString(startup_behaviour_t behaviourType, char* string, int len)
{
    if (string && len > 0)
    {
        if (behaviourType == specificDocument_t)
        {
            g_snprintf(string, len, "%s", SPECIFIC_DOCUMENT);
        }
        else if (behaviourType == referenceMaterials_t) 
        {
            g_snprintf(string, len, "%s", REFERENCE_MATERIALS);
        }
        else if (behaviourType == lastReadDocument_t)
        {
            g_snprintf(string, len, "%s", LAST_READ_DOCUMENT);
        }
        else if (behaviourType == recentDocuments_t)
        {
            g_snprintf(string, len, "%s", RECENT_DOCUMENTS);
        }
        else if (behaviourType == deviceManager_t)
        {
            g_snprintf(string, len, "%s", DEVICE_MANAGER);
        }
        else if (behaviourType == newsFolder_t)
        {
            g_snprintf(string, len, "%s", NEWS_FOLDER);
        }
        else if (behaviourType == booksFolder_t)
        {
            g_snprintf(string, len, "%s", BOOKS_FOLDER);
        }
        else if (behaviourType == docsFolder_t)
        {
            g_snprintf(string, len, "%s", DOCS_FOLDER);
        }
        else if (behaviourType == notesFolder_t)
        {
            g_snprintf(string, len, "%s", NOTES_FOLDER);
        }
        else
        {
            regBehaviourToString(DEFAULT_STARTUP_BEHAVIOUR, string, len);
        }
    }

    ERREG_LOGPRINTF("%s-->%d", string, behaviourType);    
}

connection_t regNetworkTypeToEnum(const char*string)
{
    connection_t networkType = wired_t;

    if (string)
    {
        if (strcmp(string, WIRELESS) == 0)
        {
            networkType = wireless_t;
        }
        else if (strcmp(string, WIRED) == 0)
        {
            networkType = wired_t;
        }
        else if (strcmp(string, DIALUP) == 0)
        {
            networkType = dialup_t;
        }
        else
        {
            networkType = wired_t;
        }
    }
    
    ERREG_LOGPRINTF("%s-->%d", string, networkType);
    return networkType;
}

void regNetworkTypeToString(connection_t networkType, char* string, int len)
{
    if (string && len > 0)
    {
        if (networkType == wireless_t)
        {
            g_snprintf(string, len, "%s", WIRELESS);
        }
        else if (networkType == wired_t)
        {
            g_snprintf(string, len, "%s", WIRED);
        }
        else if (networkType == dialup_t)
        {
            g_snprintf(string, len, "%s", DIALUP);
        }
        else
        {
            g_snprintf(string, len, "%s", WIRED);
        }
    }
    ERREG_LOGPRINTF("%d-->%s", networkType, string);
}

ip_address_mode_t regIpAddressToEnum(gboolean useDhcp)
{
    ip_address_mode_t ipAddressType;

    if (useDhcp)
    {
        ipAddressType = dhcp_t;
    }
    else
    {
        ipAddressType = static_t;
    }

    ERREG_LOGPRINTF("%d-->%d", useDhcp, ipAddressType);
    return ipAddressType;
}

gboolean regIpAddressToBool(ip_address_mode_t ipAddressType)
{
    gboolean useDhcp;

    if (ipAddressType == dhcp_t)
    {
        useDhcp = TRUE;
    }
    else if (ipAddressType == static_t)
    {
        useDhcp = FALSE;
    }
    else
    {
        useDhcp = TRUE;
    }

    ERREG_LOGPRINTF("%d-->%d", ipAddressType, useDhcp);
    return useDhcp;
}

wireless_encr_type_t regWlanEncrToEnum(const char*string)
{
    wireless_encr_type_t encrType = encr_none_t;

    if (string)
    {
        if (strcmp(string, ENCR_WEP) == 0)
        {
            encrType = encr_wep_t;
        }
        else if (strcmp(string, ENCR_WPA) == 0)
        {
            encrType = encr_wpa_t;
        }
        else if (strcmp(string, ENCR_NONE) == 0)
        {
            encrType = encr_none_t;
        }
        else
        {
            encrType = encr_none_t;
        }
    }
    
    ERREG_LOGPRINTF("%s-->%d", string, encrType);
    return encrType;
}

void regWlanEncrToString(wireless_encr_type_t encrType, char* string, int len)
{
    if (string && len > 0)
    {
        if (encrType == encr_wep_t)
        {
            g_snprintf(string, len, "%s", ENCR_WEP); 
        }
        else if (encrType == encr_wpa_t)
        {
            g_snprintf(string, len, "%s", ENCR_WPA); 
        }
        else if (encrType == encr_none_t)
        {
            g_snprintf(string, len, "%s", ENCR_NONE); 
        }
        else 
        {
            g_snprintf(string, len, "%s", ENCR_NONE);
        }
    }

    ERREG_LOGPRINTF("%d-->%s", encrType, string);
}

