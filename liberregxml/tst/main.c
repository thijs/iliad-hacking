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
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

/**
 * \file 
 * \brief  - test liberregxml
 *               the functions of getting
 *               the functions of setting
 *               the functions of importing and exporting
 *               the functions of rwlocking
 *               the functions of validation
 *               the functions of minset
 *               the functions of the time consumption
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "erregapi.h"
#include "erregbasis.h"
#include "erregnwprofiles.h"
#include "erreglog.h"

static void getAllSettings(void);
static gboolean setAllSettings(void);

static void testPCProfiles(void);
static void testNetworkProfiles(void);

static gboolean exportConfig(void);
static gboolean importConfig(void);

static void testValidation(void);

static void testMinSet(void);

static void testTimeConsumption(void);

int main()
{
    gboolean bRet;

    // initialize rwlock
    bRet = erRegRWLockInit();
    if (FALSE == bRet)
    {
        ERREG_ERRORPRINTF("error");
        return 0;
    }
    // get all settings
    getAllSettings();

    // set all settings
    bRet = setAllSettings();

    // test pc profiles
    testPCProfiles();
    
    // test network profiles
    testNetworkProfiles();

    // export device configuration
    exportConfig();

    // import device configuration
    importConfig();

    // test validation
    testValidation(); 
    
    // test minset
    testMinSet();
    
    // test time cosumption
    testTimeConsumption();

    // destroy rwlock
    erRegRWLockDestroy();

    return 0;
}

static void getAllSettings(void)
{

    gboolean bRet;
    regLoad_t ret;

    // locking read lock
    bRet = erRegReadLock();
    if (FALSE == bRet)
    {
        ERREG_ERRORPRINTF("error");
        return;
    }

    // loading registry
    ret = erRegLoad(regBasis_t);
/* 
 *  for getting, even thougth loading registry fails, 
 *  we still can get the settings
    if (loadError_t == ret)
    {
        ERREG_ERRORPRINTF("error");
        erRegUnlock();
        return;
    }
*/
    regVersion_t *theVersion = erRegGetVersion();
    erRegFreeVersion(theVersion);
    theVersion = NULL;

    regUserProfile_t *theUserProfile = erRegGetUserProfile();
    erRegFreeUserProfile(theUserProfile);
    theUserProfile = NULL;

    regUserSetting_t *theUserSetting = erRegGetUserSetting();
    erRegFreeUserSetting(theUserSetting);
    theUserSetting = NULL;

    regPCConfig_t *thePCConfig = NULL;
    regPCProfile_t **thePCs = NULL;
    erRegGetPCProfiles(&thePCConfig, &thePCs);
    erRegFreePCProfiles(thePCConfig, thePCs);
    thePCConfig = NULL;
    thePCs = NULL;
        
    regAutoConnect_t *theAutoConnect = erRegGetAutoConnect();
    erRegFreeAutoConnect(theAutoConnect);
    theAutoConnect = NULL;

    regContentCategory_t *theContentCategory = erRegGetContentCategory(BOOKS_CATEGORY);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    theContentCategory = erRegGetContentCategory(NEWSPAPERS_CATEGORY);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    theContentCategory = erRegGetContentCategory(NOTES_CATEGORY);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    theContentCategory = erRegGetContentCategory(DOCUMENTS_CATEGORY);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    theContentCategory = erRegGetContentCategory(MAIN_CATEGORY);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    theContentCategory = erRegGetContentCategory(SD_CATEGORY);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    theContentCategory = erRegGetContentCategory(USB_CATEGORY);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    theContentCategory = erRegGetContentCategory(MODE_CATEGORY);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    theContentCategory = erRegGetContentCategory(DOWNLOAD_HISTORY);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    theContentCategory = erRegGetContentCategory(SEARCH_RESULT);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    theContentCategory = erRegGetContentCategory(RECENTDOCUMENTS);
    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    regLastRead_t *theLastRead = erRegGetLastRead(BOOKS_CATEGORY);
    erRegFreeLastRead(theLastRead);
    theLastRead = NULL;

    theLastRead = erRegGetLastRead(NEWSPAPERS_CATEGORY);
    erRegFreeLastRead(theLastRead);
    theLastRead = NULL;

    theLastRead = erRegGetLastRead(DOCUMENTS_CATEGORY);
    erRegFreeLastRead(theLastRead);
    theLastRead = NULL;

    theLastRead = erRegGetLastRead(NOTES_CATEGORY);
    erRegFreeLastRead(theLastRead);
    theLastRead = NULL;

    regUserAppList_t *theUserAppList = NULL;
    regUserApp_t *theUserApp = NULL;
    int i;
    theUserAppList = erRegGetUserAppList();
    if (theUserAppList)
    {
        for (i = 0; i < theUserAppList->uaIDArraySize; i++)
        {
            theUserApp = erRegGetUserApp(theUserAppList->uaIDArray[i]);
            erRegFreeUserApp(theUserApp);
        theUserApp = NULL;
        }
    }
    erRegFreeUserAppList(theUserAppList);
    theUserAppList = NULL;
    
    
    /// test extension name list.
    {
        static const char * extTable [] = 
        {
            "pdf",
            "eba",
            "xeb",
            "xhtml",
            "html",
            "txt",
            "jpg",
            "jpeg",                                                                                                                                    
            "png",            
            "bmp",                                                            
            0,
        };
        static const int extTableSize = sizeof(extTable)/sizeof(extTable[0]);
        int index = 0;
        for(; index < extTableSize; ++index)
        {             
            regExtInfoList_t *theExtInfoList = erRegGetExtInfoList((char *)extTable[index]);
            erRegFreeExtInfoList(theExtInfoList);
            theExtInfoList = NULL;
        }
    }

    regStartUp_t *theStartUp = erRegGetStartUp();
    erRegFreeStartUp(theStartUp);
    theStartUp = NULL;

    // unloading registry
    erRegUnload(regBasis_t);
    
    // test on nwprofiles registry
    ret = erRegLoad(regNWProfiles_t);
    
    regNetworkConfig_t *theNetworkConfig = NULL;
    regNetworkProfile_t **theNetworks = NULL;
    erRegGetNetworkProfiles(&theNetworkConfig, &theNetworks);
    erRegFreeNetworkProfiles(theNetworkConfig, theNetworks);
    theNetworkConfig = NULL;
    theNetworks = NULL;

    regLastConnect_t *theLastConnect = erRegGetLastConnect();
    erRegFreeLastConnect(theLastConnect);
    theLastConnect = NULL;

    erRegGetAutoConnectWlan();

    erRegUnload(regNWProfiles_t);
    
    // test on toolbar registry
    ret = erRegLoad(regToolbar_t);
    if (loadOk_t == ret)
    {
        regIconSet_t *theIconSet = erRegGetIconSet();
        erRegFreeIconSet(theIconSet);
        theIconSet = NULL;
    }
    erRegUnload(regToolbar_t);
    
    // unlocking read lock
    erRegUnlock();
}

static gboolean setAllSettings(void)
{
    return FALSE;

    regLoad_t ret;
    gboolean bRet;
   
    // locking write lock
    bRet = erRegWriteLock();
    if (FALSE == bRet)
    {
        ERREG_ERRORPRINTF("error");
        return FALSE;
    }
    
    // loading registry
    ret = erRegLoad(regBasis_t);
    if (loadError_t == ret)
    {
        ERREG_ERRORPRINTF("error");
        erRegUnlock();
        return FALSE;
    }

    /** Functions xxRegNewYyy() removed from library because only used in test application **

    regVersion_t *theVersion = basisRegNewVersion();
    regUserProfile_t *theUserProfile = basisRegNewUserProfile();
    regUserSetting_t *theUserSetting = basisRegNewUserSetting();

    regPCConfig_t *thePCConfig = basisRegNewPCConfig(2);
    regPCProfile_t *thePCProfile0 = basisRegNewPCProfile(thePCConfig->pcList[0]);
    regPCProfile_t *thePCProfile1 = basisRegNewPCProfile(thePCConfig->pcList[1]);
    g_free(thePCProfile1->username);
    g_free(thePCProfile1->password);
    thePCProfile1->username = g_strdup("yehong");
    thePCProfile1->password = g_strdup("!wyh19801206");

    regAutoConnect_t *theAutoConnect = basisRegNewAutoConnect();

    regContentCategory_t *theContentCategory = basisRegNewContentCategory(MODE_CATEGORY);

    regContentCategory_t *theBooks = basisRegNewContentCategory(BOOKS_CATEGORY);
    regContentCategory_t *theNewspapers = basisRegNewContentCategory(NEWSPAPERS_CATEGORY);
    regContentCategory_t *theNotes = basisRegNewContentCategory(NOTES_CATEGORY);
    regContentCategory_t *theDocuments = basisRegNewContentCategory(DOCUMENTS_CATEGORY);
    regContentCategory_t *theCF = basisRegNewContentCategory(CF_CATEGORY);
    regContentCategory_t *theMain = basisRegNewContentCategory(MAIN_CATEGORY);
    regContentCategory_t *theSD = basisRegNewContentCategory(SD_CATEGORY);
    regContentCategory_t *theUSB = basisRegNewContentCategory(USB_CATEGORY);
    regContentCategory_t *theDownload = basisRegNewContentCategory(DOWNLOAD_HISTORY);
    regContentCategory_t *theSearch = basisRegNewContentCategory(SEARCH_RESULT);
    regContentCategory_t *theRecent = basisRegNewContentCategory(RECENT_DOCUMENTS);

    regLastRead_t *theBook = basisRegNewLastRead(BOOKS_CATEGORY);
    regLastRead_t *theNewspaper = basisRegNewLastRead(NEWSPAPERS_CATEGORY);
    regLastRead_t *theNote = basisRegNewLastRead(NOTES_CATEGORY);
    regLastRead_t *theDocument = basisRegNewLastRead(DOCUMENTS_CATEGORY);
    
    theDocument->extension = g_strdup("pdf");
    theDocument->documentPath = g_strdup("/mnt/free/documents/bible.pdf/bible.pdf");
    theDocument->manifestPath = g_strdup("/mnt/free/documents/bible.pdf/manifest.xml");
    theDocument->position = g_strdup("5");

    regUserAppList_t *theUserAppList = basisRegNewUserAppList();
    regUserApp_t *theUserApp0 = basisRegNewUserApp(theUserAppList->uaIDArray[0]);
    regUserApp_t *theUserApp1 = basisRegNewUserApp(theUserAppList->uaIDArray[1]);

    regUserApp_t *theUserApp2 = g_new0(regUserApp_t, 1);;
    theUserApp2->szCategory = g_strdup("viewer");
    theUserApp2->szArgMask = g_strdup("<MANIFEST> <FILENAME>");
    theUserApp2->szExec = g_strdup("/usr/bin/browser");
    theUserApp2->xResName = g_strdup("browser");
    theUserApp2->ipcChannel = 1;
    theUserApp2->appIconPath = g_strdup("browser.png");
    theUserApp2->extensionArray = g_new0(char *, 4 + 1);
    theUserApp2->extensionArraySize = 4;
    theUserApp2->extensionArray[0] = g_strdup("html");
    theUserApp2->extensionArray[1] = g_strdup("htm");
    theUserApp2->extensionArray[2] = g_strdup("xhtml");
    theUserApp2->extensionArray[3] = g_strdup("txt");
    theUserApp2->extensionArray[4] = NULL;

    // extensionInfos
    regExtInfo_t *theExtInfo1 = basisRegNewExtInfo();
    regExtInfo_t *theExtInfo2 = basisRegNewExtInfo();
    theExtInfo1->associateApp = g_strdup("IPDF");
    theExtInfo1->extName = g_strdup("pdf");
    theExtInfo1->iconLocation = g_strdup("/usr/share/contentlister/Icon_books.png");

    theExtInfo2->associateApp = g_strdup("EBA");
    theExtInfo2->extName = g_strdup("eba");
    theExtInfo2->iconLocation = g_strdup("/usr/share/contentlister/Icon_books.png");

    regStartUp_t *theStartUp = basisRegNewStartUp();

    bRet = erRegSetVersion(theVersion);
    bRet = erRegSetUserProfile(theUserProfile);
    bRet = erRegSetUserSetting(theUserSetting);

    bRet = erRegRemovePCConfig();
    bRet = erRegSetPCConfig(thePCConfig);
    bRet = erRegSetPCProfile(thePCConfig->pcList[0], thePCProfile0);
    bRet = erRegSetPCProfile(thePCConfig->pcList[1], thePCProfile1);

    bRet = erRegSetAutoConnect(theAutoConnect);

    bRet = erRegSetContentCategory(MODE_CATEGORY, theContentCategory);

    bRet = erRegSetContentCategory(BOOKS_CATEGORY, theBooks);
    bRet = erRegSetContentCategory(NEWSPAPERS_CATEGORY, theNewspapers);
    bRet = erRegSetContentCategory(NOTES_CATEGORY, theNotes);
    bRet = erRegSetContentCategory(DOCUMENTS_CATEGORY, theDocuments);
    bRet = erRegSetContentCategory(CF_CATEGORY, theCF);
    bRet = erRegSetContentCategory(MAIN_CATEGORY, theMain);
    bRet = erRegSetContentCategory(SD_CATEGORY, theSD);
    bRet = erRegSetContentCategory(USB_CATEGORY, theUSB);
    bRet = erRegSetContentCategory(DOWNLOAD_HISTORY, theDownload);
    bRet = erRegSetContentCategory(SEARCH_RESULT, theSearch);
    bRet = erRegSetContentCategory(RECENTDOCUMENTS, theRecent);

    bRet = erRegSetLastRead(BOOKS_CATEGORY, theBook);
    bRet = erRegSetLastRead(NEWSPAPERS_CATEGORY, theNewspaper);
    bRet = erRegSetLastRead(NOTES_CATEGORY, theNote);
    bRet = erRegSetLastRead(DOCUMENTS_CATEGORY, theDocument);

    bRet = erRegSetUserApp(theUserAppList->uaIDArray[0], theUserApp0);
    bRet = erRegSetUserApp(theUserAppList->uaIDArray[1], theUserApp1);
    bRet = erRegSetUserApp("BROWSER", theUserApp2);

    bRet = erRegSetExtInfo("pdf", theExtInfo1);
    bRet = erRegSetExtInfo("eba", theExtInfo2);

    bRet = erRegSetStartUp(theStartUp);

    erRegFreeVersion(theVersion);
    erRegFreeUserProfile(theUserProfile);
    erRegFreeUserSetting(theUserSetting);

    erRegFreePCConfig(thePCConfig);
    erRegFreePCProfile(thePCProfile0);
    erRegFreePCProfile(thePCProfile1);

    erRegFreeAutoConnect(theAutoConnect);

    erRegFreeContentCategory(theContentCategory);

    erRegFreeContentCategory(theBooks);
    erRegFreeContentCategory(theNewspapers);
    erRegFreeContentCategory(theNotes);
    erRegFreeContentCategory(theDocuments);
    erRegFreeContentCategory(theCF);
    erRegFreeContentCategory(theMain);
    erRegFreeContentCategory(theSD);
    erRegFreeContentCategory(theUSB);
    erRegFreeContentCategory(theDownload);
    erRegFreeContentCategory(theSearch);
    erRegFreeContentCategory(theRecent);

    erRegFreeLastRead(theBook);
    erRegFreeLastRead(theNewspaper);
    erRegFreeLastRead(theNote);
    erRegFreeLastRead(theDocument);

    erRegFreeUserAppList(theUserAppList);
    erRegFreeUserApp(theUserApp0);
    erRegFreeUserApp(theUserApp1);

    erRegFreeUserApp(theUserApp2);

    // erRegFreeExtInfoList(theExtInfo1);
    // erRegFreeExtInfoList(theExtInfo2);

    erRegFreeStartUp(theStartUp);

    // write the settings into file
    erRegStore();

    // unloading registry
    erRegUnload(regBasis_t);
    
    // test on nwprofiles registry
    ret = erRegLoad(regNWProfiles_t);
    if (loadOk_t == ret)
    {
        regNetworkConfig_t *theNetworkConfig = nwRegNewNetworkConfig(4);
        regNetworkProfile_t *theNetworkProfile0 = nwRegNewNetworkProfile(theNetworkConfig->networkList[0],
                                                                       wired_t);
        regNetworkProfile_t *theNetworkProfile1 = nwRegNewNetworkProfile(theNetworkConfig->networkList[1],
                                                                       wireless_t);
        regNetworkProfile_t *theNetworkProfile2 = nwRegNewNetworkProfile(theNetworkConfig->networkList[2],
                                                                       wireless_t);
        regNetworkProfile_t *theNetworkProfile3 = nwRegNewNetworkProfile(theNetworkConfig->networkList[3],
                                                                       wired_t);
        regLastConnect_t* theLastConnect = g_new0(regLastConnect_t, 1);
        theLastConnect->profileConnectedIDS = 1;
        theLastConnect->profileConnectedPC = 4;
        theLastConnect->manualConnectType = ids_t;

        if (theNetworkProfile2->addressMode == dhcp_t)
        {
            theNetworkProfile2->addressMode = static_t;
            theNetworkProfile2->ipSettings = g_new0(regIpSetting_t, 1);
            theNetworkProfile2->ipSettings->address = g_strdup("192.176.1.118");
            theNetworkProfile2->ipSettings->netmask = g_strdup("255.255.255.0");
            theNetworkProfile2->ipSettings->dns = g_strdup("202.114.1.45");
            theNetworkProfile2->ipSettings->gateway = g_strdup("192.176.1.1");
        }
        if (theNetworkProfile2->proxy == FALSE)
        {
            theNetworkProfile2->proxy = TRUE;
            theNetworkProfile2->proxySettings = g_new0(regProxySetting_t, 1);
            theNetworkProfile2->proxySettings->address = g_strdup("isaserver.irexnet.com");
            theNetworkProfile2->proxySettings->port = g_strdup("80");
        }

        theNetworkProfile3->connection = dialup_t;
        theNetworkProfile3->dialupSettings = g_new0(regDialupSetting_t, 1);
        theNetworkProfile3->dialupSettings->phone = g_strdup("12345678");
        theNetworkProfile3->dialupSettings->user = g_strdup("CDMA");
        theNetworkProfile3->dialupSettings->password = g_strdup("sldlala");

        bRet = erRegRemoveNetworkConfig();
        bRet = erRegSetNetworkConfig(theNetworkConfig);
        bRet = erRegSetNetworkProfile(theNetworkConfig->networkList[0], theNetworkProfile0);
        bRet = erRegSetNetworkProfile(theNetworkConfig->networkList[1], theNetworkProfile1);
        bRet = erRegSetNetworkProfile(theNetworkConfig->networkList[2], theNetworkProfile2);
        bRet = erRegSetNetworkProfile(theNetworkConfig->networkList[3], theNetworkProfile3);

        bRet = erRegSetLastConnect(theLastConnect);
        bRet = erRegSetAutoConnectWlan(FALSE);

        erRegFreeNetworkConfig(theNetworkConfig);
        erRegFreeNetworkProfile(theNetworkProfile0);
        erRegFreeNetworkProfile(theNetworkProfile1);
        erRegFreeNetworkProfile(theNetworkProfile2);
        erRegFreeNetworkProfile(theNetworkProfile3);

        erRegFreeLastConnect(theLastConnect);

        erRegStore();
        erRegUnload(regNWProfiles_t);
    }

    // unlocking write lock
    erRegUnlock();

    return TRUE;

    ** Functions xxRegNewYyy() removed from library because only used in test application **/
}

// for the API for PCProfiles and NetworkProfiles are a litttle bit more complex,
// so here, we write independent testing routines to test these API
//
// mainly test the functions of adding/deleting/modifing
static void testPCProfiles(void)
{
    return;

    regPCConfig_t *pPCConfig = NULL;
    regPCProfile_t **pPCs = NULL;

    gboolean bRet;
    regLoad_t ret;

    // locking write lock
    bRet = erRegWriteLock();
    if (FALSE == bRet)
    {
        ERREG_ERRORPRINTF("error");
        return;
    }

    // loading registry
    ret = erRegLoad(regBasis_t);
    if (loadError_t == ret)
    {
        ERREG_ERRORPRINTF("error");
        erRegUnlock();
        return;
    }

    // get all of current pc profiles 
    erRegGetPCProfiles(&pPCConfig, &pPCs);
    if (pPCConfig && pPCs)
    {
        int index;
        regPCProfile_t *thePCProfile = NULL;

        /* testing add functions */

        //  duplicate one pc profile
        thePCProfile = erRegDupPCProfile(pPCs[0]);
        // change something in the new pc profile
        if (thePCProfile->username)
        {
            g_free(thePCProfile->username);
            thePCProfile->username = g_strdup("alice123");
        }
        // add one more profile into the profile list 
        erRegAddPCProfileToList(&pPCConfig, &pPCs, thePCProfile);

        /* testing delete functions */

        // delete one in the profile list
        erRegDeletePCProfileFromList(&pPCConfig, &pPCs, pPCConfig->pcList[(pPCConfig->size) / 2]);

        /* modify one profile in the profile list and set it as default */
        thePCProfile = erRegGetPCProfileFromList(pPCConfig, pPCs, pPCConfig->pcList[(pPCConfig->size) / 2], &index);
        
        // modify something
        if (thePCProfile->username)
        {
            g_free(thePCProfile->username);
            thePCProfile->username = g_strdup("tom");
        }
        if (thePCProfile->password)
        {
            g_free(thePCProfile->password);
            thePCProfile->password = g_strdup("qwerty");
        }
        // set it as default
        erRegSetPCProfileDefault(&pPCConfig, &pPCs, pPCConfig->pcList[(pPCConfig->size) / 2]);
//         erRegSetPCProfileDefault(&pPCConfig, &pPCs, pPCConfig->pcList[pPCConfig->size-1]);

        // save the changes into xml tree 
        const regPCProfile_t **p = (const regPCProfile_t **)pPCs;  // const_cast
        erRegSetPCProfiles(pPCConfig, p);

        // free the memory
        erRegFreePCProfiles(pPCConfig, pPCs);
    }

    // write the settings into file
    erRegStore();

    // unloading registry
    erRegUnload(regBasis_t);

    // unlocking write lock
    erRegUnlock();
}

// for the API for NetworkProfiles and NetworkProfiles are a litttle bit more complex,
// so here, we write independent testing routines to test these API
//
// mainly test the functions of adding/deleting/modifing
static void testNetworkProfiles(void)
{
    return;

    regNetworkConfig_t *pNetworkConfig = NULL;
    regNetworkProfile_t **pNetworks = NULL;

    gboolean bRet;
    regLoad_t ret;

    // locking write lock
    bRet = erRegWriteLock();
    if (FALSE == bRet)
    {
        ERREG_ERRORPRINTF("error");
        return;
    }

    // loading registry
    ret = erRegLoad(regNWProfiles_t);
    if (loadError_t == ret)
    {
        ERREG_ERRORPRINTF("error");
        erRegUnlock();
        return;
    }

    // get all of current network profiles 
    erRegGetNetworkProfiles(&pNetworkConfig, &pNetworks);
    if (pNetworkConfig && pNetworks)
    {
        int index;
        regNetworkProfile_t *theNetworkProfile = NULL;

        /* testing add functions */

        //  duplicate one network profile
        theNetworkProfile = erRegDupNetworkProfile(pNetworks[0]);
        // change something in the new network profile
        if (theNetworkProfile->name)
        {
            g_free(theNetworkProfile->name);
            theNetworkProfile->name = g_strdup("Network Environment At Home");
        }

        // add one more profile into the profile list 
        erRegAddNetworkProfileToList(&pNetworkConfig, &pNetworks, theNetworkProfile);

        /* testing delete functions */

        // delete one in the profile list
        erRegDeleteNetworkProfileFromList(&pNetworkConfig, &pNetworks, 
                pNetworkConfig->networkList[(pNetworkConfig->size) / 2]);

        /* modify one profile in the profile list and set it as default */
        index = erRegGetNetworkProfileFromList(pNetworkConfig,
                                pNetworkConfig->networkList[(pNetworkConfig->size) / 2]);
        theNetworkProfile = pNetworks[index];
        if (theNetworkProfile->name)
        {
            g_free(theNetworkProfile->name);
            theNetworkProfile->name = g_strdup("Network Environment At Office");
        }

        // set it as default
        erRegSetNetworkProfileDefault(&pNetworkConfig, &pNetworks, 
                    pNetworkConfig->networkList[(pNetworkConfig->size) / 2]);
//        erRegSetNetworkProfileDefault(&pNetworkConfig, &pNetworks, 
//                    pNetworkConfig->networkList[pNetworkConfig->size-1]);


        // save the changes into xml tree 
        const regNetworkProfile_t **p = (const regNetworkProfile_t **)pNetworks;  // const_cast
        erRegSetNetworkProfiles(pNetworkConfig, p);

        // free the memory
        erRegFreeNetworkProfiles(pNetworkConfig, pNetworks);
    }
    
    // write the settings into file
    erRegStore();

    // unloading registry
    erRegUnload(regNWProfiles_t);

    // unlocking write lock
    erRegUnlock();
}

static gboolean exportConfig(void)
{
    return FALSE;

    gboolean bRet;
    
    bRet = erRegWriteLock();
    if (FALSE == bRet)
    {
        return FALSE;
    }

    // NOT TESTED: bRet = erRegExportConfig();
   
    erRegUnlock();

    return bRet;
}

static gboolean importConfig(void)
{
    return FALSE;

    gboolean bRet;

    bRet = erRegWriteLock();
    if (FALSE == bRet)
    {
        return FALSE;
    }

    // NOT TESTED: bRet = erRegImportConfig();

    erRegUnlock();

    return bRet;
}

static void testValidation(void)
{
    return;
    gboolean bRet;
    regLoad_t ret;
    
    bRet = erRegWriteLock();
    if (FALSE == bRet)
    {
        return;
    }
    
    ret = erRegLoad(regBasis_t);
    if (loadError_t == ret)
    {
        ERREG_ERRORPRINTF("error");
        erRegUnlock();
        return;
    }

    // write valid data
    regUserProfile_t *theUserProfile = erRegGetUserProfile();
    if (theUserProfile->password)
    {
        g_free(theUserProfile->password);
        theUserProfile->password = g_strdup("1234567890");
    }
    erRegSetUserProfile(theUserProfile);
    erRegFreeUserProfile(theUserProfile);
    
    erRegStore();
    
    // write invalid data
    regLastRead_t *theLastRead = erRegGetLastRead(BOOKS_CATEGORY);
    bRet = erRegSetLastRead("yehong", theLastRead);
    erRegFreeLastRead(theLastRead);

    erRegStore();
    
    erRegUnload(regBasis_t);
    erRegUnlock();
}

static void testMinSet(void)
{
    return;
    gboolean bRet;
    
    bRet = erRegWriteLock();
    if (FALSE == bRet)
    {
        return;
    }

    /** Functions xxRecoverFromMinSet() have been removed **
     * 
    basisRegRecoverFromMinSet();
    nwRegRecoverFromMinSet();
    erRegUnlock();
    return;
     *
     ** Functions xxRecoverFromMinSet() have been removed **/

    /** Functions xxRegNewYyy() removed from library because only used in test application **
     *
    regVersion_t *theVersion = basisRegNewVersion();
    regUserProfile_t *theUserProfile = basisRegNewUserProfile();
    regUserSetting_t *theUserSetting = basisRegNewUserSetting();
    
    regPCConfig_t *thePCConfig = NULL;
    regPCProfile_t **thePCs = NULL;
    basisRegNewPCProfiles(&thePCConfig, &thePCs);
    
    regNetworkConfig_t *theNetworkConfig = NULL;
    regNetworkProfile_t **theNetworks = NULL;
    nwRegNewNetworkProfiles(&theNetworkConfig, &theNetworks);
  
    regAutoConnect_t *theAutoConnect = basisRegNewAutoConnect();

    regContentCategory_t *theContentCategory = basisRegNewContentCategory(MODE_CATEGORY);

    regLastRead_t *theLastRead = basisRegNewLastRead(BOOKS_CATEGORY);

    regUserAppList_t *theUserAppList = basisRegNewUserAppList();
    regUserApp_t *theUserApp0 = basisRegNewUserApp(theUserAppList->uaIDArray[0]);
    regUserApp_t *theUserApp1 = basisRegNewUserApp(theUserAppList->uaIDArray[1]);
    
    regStartUp_t *theStartUp = basisRegNewStartUp();
    
    erRegFreeVersion(theVersion);
    theVersion = NULL;

    erRegFreeUserProfile(theUserProfile);
    theUserProfile = NULL;

    erRegFreeUserSetting(theUserSetting);
    theUserSetting = NULL;

    erRegFreePCProfiles(thePCConfig, thePCs);
    thePCConfig = NULL;
    thePCs = NULL;

    erRegFreeNetworkProfiles(theNetworkConfig, theNetworks);
    theNetworkConfig = NULL;
    theNetworks = NULL;

    erRegFreeAutoConnect(theAutoConnect);
    theAutoConnect = NULL;

    erRegFreeContentCategory(theContentCategory);
    theContentCategory = NULL;

    erRegFreeLastRead(theLastRead);
    theLastRead = NULL;

    erRegFreeUserAppList(theUserAppList);
    erRegFreeUserApp(theUserApp0);
    erRegFreeUserApp(theUserApp1);
    theUserAppList = NULL;
    theUserApp0 = NULL;
    theUserApp1 = NULL;

    erRegFreeStartUp(theStartUp);
    theStartUp = NULL;
     *
     ** Functions xxRegNewYyy() removed from library because only used in test application **/
}

static void testTimeConsumption(void)
{
    return;

    // locking read lock
    gboolean bRet = erRegReadLock();
    if (FALSE == bRet)
    {
        ERREG_ERRORPRINTF("error");
        return;
    }
    
    ERREG_TIMEDISPLAY("Begin loading registry");

    // loading registry
    regLoad_t ret = erRegLoad(regBasis_t);
    if (loadError_t == ret)
    {
        ERREG_ERRORPRINTF("error");
        erRegUnlock();
        return;
    }
    // unloading registry
    erRegUnload(regBasis_t);

    ret = erRegLoad(regNWProfiles_t);
    regNetworkConfig_t *theNetworkConfig = NULL;
    regNetworkProfile_t **theNetworks = NULL;
    erRegGetNetworkProfiles(&theNetworkConfig, &theNetworks);
    erRegFreeNetworkProfiles(theNetworkConfig, theNetworks);
    theNetworkConfig = NULL;
    theNetworks = NULL;
    erRegUnload(regNWProfiles_t);
 
    ret = erRegLoad(regToolbar_t);
    regIconSet_t *theIconSet = erRegGetIconSet();
    erRegFreeIconSet(theIconSet);
    theIconSet = NULL;
    erRegUnload(regToolbar_t);

    ERREG_TIMEDISPLAY("End loading registry");

    // unlocking read lock
    erRegUnlock();
}

