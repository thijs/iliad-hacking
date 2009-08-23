/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file metadataStoreManager.c
 * \brief metadataStoreManager - transfer for the meta data store (init and manage the metadata store)                 
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
#include <config.h>

#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mntent.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <glib.h>
#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "lister.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "stack.h"
#include "metadataStoreManager.h"
#include "erMdsContent.h"
#include "regValue.h"
#include "languages.h"

static GArray *g_contentArray = NULL;
static stItem_t *g_rootInfo[TOTAL_STACK_COUNT];  // = { NULL, ... } but this is implicit for static data

//internally used values
static void rootInfoInit(void);
static void rootInfoStore(st_ContentType_e type, regContentCategory_t * theContentCategory);
static void rootInfoDestroy(void);

static gboolean isRealPathSubFolder(const char *path, const char *basePath);
static void recreateArchives(st_ContentType_e storageType);
static char* getArchiveName(st_ContentType_e archiveType);

void mdsInit(void)
{
    // destory the old ones
    mdsDestroy();   

    rootInfoInit();
    stackInitAll();

    mdsRecreateArchives(st_StorageTypeMain);
    mdsRecreateArchives(st_StorageTypeCF);
    mdsRecreateArchives(st_StorageTypeSD);
    mdsRecreateArchives(st_StorageTypeUSB);
}

void mdsDestroy(void)
{
    rootInfoDestroy();
    stackDestroyAll();

    if (g_contentArray)
    {
        g_array_free(g_contentArray, TRUE);
        g_contentArray = NULL;
    }
}

int mdsScanRoot(st_ContentType_e contentType)
{
    int     result = 0;
    mdsSortStruct_t *sort = NULL;
    int     sortResult;

    CL_STACKPRINTF("");

    // clear the previous retrieved content information
    if (g_contentArray)
    {
        CL_STACKPRINTF("g_array_free");
        g_array_free(g_contentArray, TRUE);
        g_contentArray = NULL;
    }

    CL_TIMEDISPLAY("before scan");

    if (NULL != g_rootInfo[contentType])
    {
        result = erMdsContentScan(g_rootInfo[contentType]->dirArray, &g_contentArray);
        sort = &g_rootInfo[contentType]->sort;
    }

    CL_TIMEDISPLAY("after scan");

    if ((ERMDS_CONTENT_SCAN_FAILED != result) && (NULL != g_contentArray))
    {
        sortResult = erMdsContentSort(g_contentArray, sort);

        CL_LOGPRINTF("erMdsContentSort returned %d", sortResult);
    }
    return result;
}

int mdsScanCurrent(st_ContentType_e contentType)
{
    stItem_t *contentInfo;
    int     result = 0;
    mdsSortStruct_t *sort = NULL;
    int     sortResult;

    CL_STACKPRINTF("entry: contentType [%d]", contentType);

    if (NULL == g_rootInfo[contentType])
    {
        CL_ERRORPRINTF("No g_rootInfo for contentType [%d], fallback to MODE menu", contentType);
        contentType = st_MenuTypeMode;
    }
    g_return_val_if_fail( (NULL != g_rootInfo[contentType]), result);

    // clear the previous retrieved content information
    if (g_contentArray)
    {
        CL_STACKPRINTF("g_array_free");
        g_array_free(g_contentArray, TRUE);
        g_contentArray = NULL;
    }

    contentInfo = stackPeek(contentType);

    CL_TIMEDISPLAY("before scan");

    if (NULL == contentInfo)
    {
        CL_STACKPRINTF("=> root content");
        result = erMdsContentScan(g_rootInfo[contentType]->dirArray, &g_contentArray);
    }
    else
    {
        //retrieve new content
        stackItemDisplay(contentInfo);
        result = erMdsContentScan(contentInfo->dirArray, &g_contentArray);
    }
    
    CL_TIMEDISPLAY("after scan");

    if ((ERMDS_CONTENT_SCAN_FAILED != result) && (NULL != g_contentArray))
    {
        // we always take the sort settings from the rootInfo
        sort = &g_rootInfo[contentType]->sort;
        sortResult = erMdsContentSort(g_contentArray, sort);

        CL_LOGPRINTF("erMdsContentSort returned %d", sortResult);
    }
    return result;
}

/**
 * Scan using the current "content details" information  
 * but with this new directory data. And store these
 * new "content details" for this contentType on the stack.
 *
 * Also up
*/
int mdsScanNext(st_ContentType_e contentType, char *directory, unsigned int theIndex)
{
    stItem_t *newItem;
    stItem_t *currentItem;
    int     result = 0;
    mdsDirectory_t folder;

    strcpy(folder.szFilename, directory);

    CL_STACKPRINTF("%s", folder.szFilename);

    newItem = g_new0(stItem_t, 1);
    currentItem = stackPeek(contentType);

    if (NULL == currentItem)
    {
        CL_STACKPRINTF("=> root content");
        currentItem = g_rootInfo[contentType];
    }

    // create a new array to store the directory info
    newItem->dirArray = g_array_sized_new(FALSE, TRUE, ERMDS_MAX_FILENAME_SIZE, INITIAL_DIR_COUNT_ARRAY_SIZE);
    CL_STACKPRINTF("newItem->dirArray 0x%x", (unsigned int) newItem->dirArray);
    g_array_append_val(newItem->dirArray, folder);

    // copy the rest of the content from the "current" content details

    // we needn't copy the sort settings because we always use them from the rootInfo.
    // newItem->sort.ascending = currentItem->sort.ascending;
    // newItem->sort.ft = currentItem->sort.ft;
    newItem->index = 0;

    // put the new item on the stack
    stackPush(contentType, newItem);

    //copy of new item (not of the referenced data!) was made on the stack
    g_free(newItem);

    result = mdsScanCurrent(contentType);
    return result;
}

/**
 * Remove the current "content details" from the stack and
 * scan using the new current "content details" information.  
*/
int mdsScanPrevious(st_ContentType_e contentType)
{
    int     result = 0;

    CL_STACKPRINTF("");

    stackPop(contentType);
    result = mdsScanCurrent(contentType);
    return result;
}

// retrieve the item count of the last scanned content
int mdsGetItemCount(void)
{

    if (g_contentArray)
    {
        CL_STACKPRINTF("%d", g_contentArray->len);
        return g_contentArray->len;
    }
    else
    {
        CL_STACKPRINTF("0");
        return 0;
    }
}

// Retrieve the index for the specified file
int mdsGetItemIndex(const gchar* szFilename)
{
    int         i;
    int         index = -1;
    gboolean    found;
    mdsFsItem_t *pFsItem;
    char        *cp;
    char        szRealpath[PATH_MAX];

    g_return_val_if_fail((g_contentArray != NULL), -1);
    
    for ( i = 0, found = FALSE ;
          i < g_contentArray->len  &&  !found ;
          i++ )
    {
        pFsItem = &g_array_index(g_contentArray, mdsFsItem_t, i);
        cp = realpath(pFsItem->szFilename, szRealpath);
        if (cp != NULL)
        {
            if (strcmp(szFilename, szRealpath) == 0 )
            {
                found = TRUE;
                index = i;
            }
        }
    }

    return index;
}

// retrieve the filename of the (index)th item
gboolean mdsGetItemFilename(int index, gchar* szFilename, int len)
{
    mdsFsItem_t *pFsItem = NULL;

    g_return_val_if_fail((g_contentArray != NULL), FALSE);
    g_return_val_if_fail( ((index >= 0) && (index < g_contentArray->len)) , FALSE);
    g_return_val_if_fail((szFilename != NULL), FALSE);
   
    pFsItem = &g_array_index(g_contentArray, mdsFsItem_t, index);
    g_return_val_if_fail((pFsItem != NULL), FALSE);
    snprintf(szFilename, len, "%s", pFsItem->szFilename);

    return TRUE;
}

// Fill in page content data
int mdsSetPageContent(clDisplayItem_t * displayItemArray, int nBegin, int *nChunk)
{
    int     returnValue;

    if (g_contentArray)
    {
        CL_TIMEDISPLAY("before erClGetDisplayItems");

        returnValue = erClGetDisplayItems(g_contentArray, nBegin, nChunk, displayItemArray);

        CL_TIMEDISPLAY("after erClGetDisplayItems");

    }
    else
    {
        CL_ERRORPRINTF("g_contentArray == NULL");
        returnValue = (-1);
    }

    return returnValue;
}


// Update the index field of the current top stack item
void mdsSetIndex(st_ContentType_e contentType, unsigned int index)
{
    stItem_t *theItem;

    theItem = stackPeek(contentType);

    if (NULL != theItem)
    {
        //update the index value
        theItem->index = index;
    }
    else
    {
        //update the root index
        if (NULL != g_rootInfo[contentType])
        {
            g_rootInfo[contentType]->index = index;
        }
    }
    return;
}


// Retrieve the index field of the current top stack item
unsigned int mdsGetIndex(st_ContentType_e contentType)
{
    unsigned int returnValue = 0;
    stItem_t *theItem;

    theItem = stackPeek(contentType);

    if (NULL != theItem)
    {
        returnValue = theItem->index;
    }
    else
    {
        if (NULL != g_rootInfo[contentType])
        {
            returnValue = g_rootInfo[contentType]->index;
        }
    }
    return returnValue;
}

static void rootInfoInit(void)
{
    regContentCategory_t *theBooks = NULL, *theNews = NULL, *theDocs = NULL, *theNotes = NULL;
    regContentCategory_t *theMain = NULL, *theCF = NULL, *theSD = NULL, *theUSB = NULL;
    regContentCategory_t *theRecent = NULL, *theDownload = NULL, *theSearch = NULL, *theMode = NULL;
    
    gchar                *cp;
    struct stat          statbuf;
    
    // reading sections from registry
    theBooks = erRegGetContentCategory(BOOKS_CATEGORY);
    theNews = erRegGetContentCategory(NEWSPAPERS_CATEGORY);
    theDocs = erRegGetContentCategory(DOCUMENTS_CATEGORY);
    theNotes = erRegGetContentCategory(NOTES_CATEGORY);

    theMode = erRegGetContentCategory(MODE_CATEGORY);

    theCF = erRegGetContentCategory(CF_CATEGORY);
    theSD = erRegGetContentCategory(SD_CATEGORY);
    theUSB = erRegGetContentCategory(USB_CATEGORY);
    theMain = erRegGetContentCategory(MAIN_CATEGORY);

    theDownload = erRegGetContentCategory(DOWNLOAD_HISTORY);
    theSearch = erRegGetContentCategory(SEARCH_RESULT);
    theRecent = erRegGetContentCategory(RECENTDOCUMENTS);

    if (theBooks)
    {
        rootInfoStore(st_ContentTypeBooks, theBooks);
        erRegFreeContentCategory(theBooks);
    }
    else
    {
        CL_ERRORPRINTF("BOOKS_CATEGORY registry info missing");
    }

    if (theNews)
    {
        rootInfoStore(st_ContentTypeNews, theNews);
        erRegFreeContentCategory(theNews);
    }
    else
    {
        CL_ERRORPRINTF("NEWSPAPERS_CATEGORY registry info missing");
    }

    if (theDocs)
    {
        rootInfoStore(st_ContentTypeDocs, theDocs);
        erRegFreeContentCategory(theDocs);
    }
    else
    {
        CL_ERRORPRINTF("DOCUMENTS_CATEGORY registry info missing");
    }

    if (theNotes)
    {
        rootInfoStore(st_ContentTypeNotes, theNotes);
        erRegFreeContentCategory(theNotes);
    }
    else
    {
        CL_ERRORPRINTF("NOTES_CATEGORY registry info missing");
    }

    if (theMode)
    {
        if (theMode->location)
        {
            cp = g_strconcat(theMode->location, "/", languagesGetCountry(), NULL);
            if (stat(cp, &statbuf) != 0)
            {
                g_free(cp);
                cp = g_strconcat(theMode->location, "/", languagesGetDefaultCountry(), NULL);
            }
            g_free(theMode->location);
            theMode->location = cp;
            CL_STACKPRINTF("MODE_CATEGORY location changed: %s", theMode->location);
        }

        rootInfoStore(st_MenuTypeMode, theMode);
        erRegFreeContentCategory(theMode);
    }
    else
    {
        CL_ERRORPRINTF("MODE_CATEGORY registry info missing");
    }

    if (theCF)
    {
        rootInfoStore(st_StorageTypeCF, theCF);
        erRegFreeContentCategory(theCF);
    }
    else
    {
        CL_ERRORPRINTF("CF_CATEGORY registry info missing");
    }

    if (theSD)
    {
        rootInfoStore(st_StorageTypeSD, theSD);
        erRegFreeContentCategory(theSD);
    }
    else
    {
        CL_ERRORPRINTF("SD_CATEGORY registry info missing");
    }

    if (theUSB)
    {
        rootInfoStore(st_StorageTypeUSB, theUSB);
        erRegFreeContentCategory(theUSB);
    }
    else
    {
        CL_ERRORPRINTF("USB_CATEGORY registry info missing");
    }

    if (theMain)
    {
        rootInfoStore(st_StorageTypeMain, theMain);
        erRegFreeContentCategory(theMain);
    }
    else
    {
        CL_ERRORPRINTF("MAIN_CATEGORY registry info missing");
    }

    if (theDownload)
    {
        rootInfoStore(st_DownloadHistory, theDownload);
        erRegFreeContentCategory(theDownload);
    }
    else
    {
        CL_ERRORPRINTF("DOWNLOAD_HISTORY registry info missing");
    }

    if (theSearch)
    {
        rootInfoStore(st_SearchResult, theSearch);
        erRegFreeContentCategory(theSearch);
    }
    else
    {
        CL_ERRORPRINTF("SEARCH_RESULT registry info missing");
    }

    if (theRecent)
    {
        rootInfoStore(st_RecentDocuments, theRecent);
        erRegFreeContentCategory(theRecent);
    }
    else
    {
        CL_ERRORPRINTF("RECENTDOCUMENTS registry info missing");
    }
}

static void rootInfoStore(st_ContentType_e type, regContentCategory_t * theContentCategory)
{
    unsigned int index;
    mdsDirectory_t dir;
    stItem_t *rootItem;

    // create a stack item  
    rootItem = g_new0(stItem_t, 1);

    if (theContentCategory->location)
    {
        rootItem->dirArray = g_array_sized_new(FALSE, TRUE, sizeof(mdsDirectory_t), 1);

        for (index = 0; index < 1; index++)
        {
            strncpy(dir.szFilename, theContentCategory->location, ERMDS_MAX_FILENAME_SIZE);
            g_array_append_val(rootItem->dirArray, dir);
            CL_STACKPRINTF("root dir %s", dir.szFilename);
        }
    }

    rootItem->sort.ascending = theContentCategory->sortAscending;
    rootItem->sort.ft = getFieldType(theContentCategory->sortFieldType);

    rootItem->index = 0;

    stackItemDisplay(rootItem);

    g_rootInfo[type] = rootItem;
}

//
mdsSortStruct_t* mdsGetSortStructOfRootInfo(st_ContentType_e type)
{
   stItem_t *rootItem = NULL;

   if (type < st_ContentTypeBooks || type >= st_ContentTypeUndefined || type >= TOTAL_STACK_COUNT)
   {
       CL_ERRORPRINTF("Invalid arguments");
       return NULL;
   }

   rootItem =  g_rootInfo[type];
   if (rootItem)
   {
        return &(rootItem->sort); 
   }
   else
   {
       CL_WARNPRINTF("Can't find the rootItem of type%d", type);
       return NULL;
   }
}

void mdsSetSortStructOfRootInfo(st_ContentType_e type, const mdsSortStruct_t* sort)
{
   stItem_t *rootItem = NULL;
   mdsSortStruct_t *localSort = NULL;

   if (type < st_ContentTypeBooks || type >= st_ContentTypeUndefined || type >= TOTAL_STACK_COUNT || NULL == sort)
   {
       CL_ERRORPRINTF("Invalid arguments");
       return;
   }

   rootItem =  g_rootInfo[type];
   if (rootItem)
   {
        localSort = &(rootItem->sort);
        localSort->ascending = sort->ascending;
        localSort->ft = sort->ft;
   }
   else
   {
       CL_WARNPRINTF("Can't find the rootItem of type%d", type);
       return;
   }
}

char* getFieldString(mdsFieldType_e type)
{
    static char string[ERCL_MAX_FIELDNAME_SIZE+1];

    switch (type)
    {
       case mdsFieldAuthor:
           strcpy(string, AUTHOR);
           break;
       case mdsFieldDate:
            strcpy(string, DATE);
           break;
       case mdsFieldDescription:
            strcpy(string, DESCRIPTION);
            break;
       case mdsFieldFile:
            strcpy(string, FIELDFILE);
           break;
       case mdsFieldTitle:
            strcpy(string, TITLE);
            break;
       case mdsFieldFsName:
            strcpy(string, FSNAME);
            break;
        case mdsFieldExtName:
            strcpy(string, EXTENSION);
            break; 
        case mdsFieldSize:
            strcpy(string, SIZE);
            break;
        case mdsFieldNone:
        default:
            string[0] = '\0';
            break;
    }
    return string;
}

mdsFieldType_e getFieldType(char *stringValue)
{
    if (stringValue)
    {
        //author - date - description - file - fsname - title - none 
        if (strcmp(stringValue, AUTHOR) == 0)
        {
            return mdsFieldAuthor;
        }
        else if (strcmp(stringValue, DATE) == 0)
        {
            return mdsFieldDate;
        }
        else if (strcmp(stringValue, FIELDFILE) == 0)
        {
            return mdsFieldFile;
        }
        else if (strcmp(stringValue, FSNAME) == 0)
        {
            return mdsFieldFsName;
        }
        else if (strcmp(stringValue, TITLE) == 0)
        {
            return mdsFieldTitle;
        }
        else if (strcmp(stringValue, DESCRIPTION) == 0)
        {
            return mdsFieldDescription;
        }
        else if (strcmp(stringValue, EXTENSION) == 0)
        {
            return mdsFieldExtName;
        }
        else if (strcmp(stringValue, SIZE) == 0)
        {
            return mdsFieldSize;
        }
        else
        {
            return mdsFieldNone;
        }
    }
    else
    {
        return mdsFieldNone;
    }

}

gchar* getContentCategoryID(st_ContentType_e type)
{
    static gchar szID[ERMDS_MAX_EXTENSION_ID_SIZE+1] = "";

    switch (type)
    {
        case st_ContentTypeBooks:
            strcpy(szID, BOOKS_CATEGORY);
            break;
        case st_ContentTypeNews:
            strcpy(szID, NEWSPAPERS_CATEGORY);
            break;
        case st_ContentTypeNotes:
            strcpy(szID, NOTES_CATEGORY);
            break;
        case st_ContentTypeDocs:
            strcpy(szID, DOCUMENTS_CATEGORY);
            break;
        case st_StorageTypeCF:
            strcpy(szID, CF_CATEGORY);
            break;
        case st_StorageTypeMain:
            strcpy(szID, MAIN_CATEGORY);
            break;
        case st_StorageTypeSD:
            strcpy(szID, SD_CATEGORY);
            break;
        case st_StorageTypeUSB:
            strcpy(szID, USB_CATEGORY);
            break;
        case st_MenuTypeMode:
            strcpy(szID, MODE_CATEGORY);
            break;
        case st_DownloadHistory:
            strcpy(szID, DOWNLOAD_HISTORY);
            break;
        case st_SearchResult:
            strcpy(szID, SEARCH_RESULT);
            break;
        case st_RecentDocuments:
            strcpy(szID, RECENTDOCUMENTS);
            break;
        case st_ContentTypeUndefined:
        default:
            szID[0] = '\0';
            break;
    }
    return szID;
}

static void rootInfoDestroy(void)
{
    int     index;

    for (index = 0; index < TOTAL_STACK_COUNT; index++)
    {
        if (g_rootInfo[index])
        {
            stackItemDestroy(g_rootInfo[index]);
            g_rootInfo[index] = NULL;
        }
    }
}

int mdsGetRootLocation(const st_ContentType_e contentType, char * location, const int length)
{
    int     nRet = 0;
    char*   cp;
    char    szRealpath[PATH_MAX];
    
    if (NULL == location || 0 >= length) 
    {
        CL_ERRORPRINTF("Invalid buffer pointer or length!");
        return nRet;
    }
    
    if ((NULL == g_rootInfo[contentType]) || (NULL == g_rootInfo[contentType]->dirArray))
    {
        if (contentType == st_MenuTypeMode)
        {
            CL_ERRORPRINTF("No root dir specified for st_MenuTypeMode!");
            return nRet;
        }
        else
        {
            CL_ERRORPRINTF("No root dir for contentType [%d], fallback to MODE menu", contentType);
            return mdsGetRootLocation(st_MenuTypeMode, location, length);
        }
    }
    
    mdsDirectory_t *directory =  &g_array_index(g_rootInfo[contentType]->dirArray,  mdsDirectory_t, 0);
    if (directory)
    {
        cp = realpath(directory->szFilename, szRealpath);
        if (cp == NULL)
        {
            // no realpath found -> report original path
            cp = directory->szFilename;
        }
        nRet = snprintf(location, length, "%s", cp);
    }
    else
    {
        CL_ERRORPRINTF("Invalid directory pointer!");
    }
    return nRet;
}

// check whether archive location is changed or not
gboolean mdsArcLocIsChanged(st_ContentType_e archiveType, st_ContentType_e storageType)
{
    char archiveDirName[ERMDS_MAX_FILENAME_SIZE], storageDirName[ERMDS_MAX_FILENAME_SIZE];
    gboolean bRet = FALSE;
    
    if ((archiveType != st_ContentTypeNews) 
            && (archiveType != st_ContentTypeBooks)
            && (archiveType != st_ContentTypeDocs)
            && (archiveType != st_ContentTypeNotes))
    {
        CL_ERRORPRINTF("invalid archive type[%d]", archiveType);
        return bRet;
    }
    if ((storageType != st_StorageTypeCF)
            && (storageType != st_StorageTypeSD)
            && (storageType != st_StorageTypeUSB)
            && (storageType != st_StorageTypeMain))
    {
        CL_ERRORPRINTF("invalid storage type[%d]", storageType);
        return bRet;
    }
              
    if ( mdsGetRootLocation(archiveType, archiveDirName, sizeof(archiveDirName)) > 0 )
    {
        if ( mdsGetRootLocation(storageType, storageDirName, sizeof(storageDirName)) > 0 )
        {
           CL_LOGPRINTF("%s,%s", archiveDirName, storageDirName);
           if (isRealPathSubFolder(archiveDirName, storageDirName))
           {
              bRet = TRUE;
           }
        }
        else
        {
            CL_ERRORPRINTF("error");
        }
    }
    else
    {
        CL_ERRORPRINTF("error");
    }
    return bRet;
}

// call this function
//  - at system startup
//  - when inserting a removable memory device (CF, MMC, USB)
//  - when disconnecting USB from PC 
void mdsRecreateArchives(st_ContentType_e storageType)
{
    CL_LOGPRINTF("entry");

    // check the argument is valid or not
    if ( (storageType != st_StorageTypeCF) && (storageType != st_StorageTypeSD)
          && (storageType != st_StorageTypeUSB) && (storageType != st_StorageTypeMain) )
    {
        CL_ERRORPRINTF("invalid storage type[%d]", storageType);
        return;
    }

    if (st_StorageTypeMain == storageType)
    {
        // if it's internal memory, 
        // always recreate the archive folders if they don't exist  
        recreateArchives(st_StorageTypeMain);
    }
    else
    {
        // if one of the archives points to the storage, 
        // recreate the archive folders if they don't exsit on storage
        if ( mdsArcLocIsChanged(st_ContentTypeNews, storageType)
             || mdsArcLocIsChanged(st_ContentTypeBooks, storageType)
             || mdsArcLocIsChanged(st_ContentTypeDocs, storageType)
             || mdsArcLocIsChanged(st_ContentTypeNotes, storageType) )
        {
            recreateArchives(storageType);
        }
    }
}

static gboolean isRealPathSubFolder(const char *path, const char *basePath)
{
    char *cp1 = NULL, *cp2 = NULL;
    char realPath[ERMDS_MAX_FILENAME_SIZE], realBasePath[ERMDS_MAX_FILENAME_SIZE];
    gboolean bRet = FALSE;

    cp1 = realpath(path, realPath);
    cp2 = realpath(basePath, realBasePath);
    
    CL_LOGPRINTF("%s,%s", path, realPath);
    CL_LOGPRINTF("cp1=%s, cp2=%s", cp1, cp2);
    CL_LOGPRINTF("%s,%s", realPath, realBasePath);

    if (/*cp1 && cp2 && */strstr(realPath, realBasePath) != NULL)
    {
        CL_LOGPRINTF("realpath of %s is subfolder of realpath of %s", path, basePath);
        bRet = TRUE;
    }
    else
    {
        CL_LOGPRINTF("realpath of %s is not subfolder of realpath of %s", path, basePath);
    }

    return bRet;
}

static void recreateArchives(st_ContentType_e storageType)
{
    char storageDirName[ERMDS_MAX_FILENAME_SIZE];
    char newDir[ERMDS_MAX_FILENAME_SIZE];
    char *cp = NULL;

    if (!storageIsMounted(storageType))
    {
        CL_ERRORPRINTF("storage not mounted, storage type[%d]", storageType);
        return;
    }
 
    if ( mdsGetRootLocation(storageType, storageDirName, sizeof(storageDirName)) > 0 )
    {
        CL_LOGPRINTF("recreate archives on %s", storageDirName);
        
        // newspapers
        cp = getArchiveName(st_ContentTypeNews);
        if (cp)
        {
            snprintf(newDir, ERMDS_MAX_FILENAME_SIZE, "%s%s", storageDirName, cp);
            if (0 != mkdir(newDir, 0755))
            {
                CL_ERRORPRINTF("Can not create directory %s!", newDir);             
            }
        }

        // books
        cp = getArchiveName(st_ContentTypeBooks);
        if (cp)
        {
            snprintf(newDir, ERMDS_MAX_FILENAME_SIZE, "%s%s", storageDirName, cp);
            if (0 != mkdir(newDir, 0755))
            {
                CL_ERRORPRINTF("Can not create directory %s!", newDir);             
            }
        }

        // documents
        cp = getArchiveName(st_ContentTypeDocs);
        if (cp)
        {
            snprintf(newDir, ERMDS_MAX_FILENAME_SIZE, "%s%s", storageDirName, cp);
            if (0 != mkdir(newDir, 0755))
            {
                CL_ERRORPRINTF("Can not create directory %s!", newDir);             
            }
        }

        // notes 
        cp = getArchiveName(st_ContentTypeNotes);
        if (cp)
        {
            snprintf(newDir, ERMDS_MAX_FILENAME_SIZE, "%s%s", storageDirName, cp);
            if (0 != mkdir(newDir, 0755))
            {
                CL_ERRORPRINTF("Can not create directory %s!", newDir);             
            }
        }
    }
}

static char* getArchiveName(st_ContentType_e archiveType)
{
    mdsDirectory_t *directory =  NULL;
    char* cp = NULL;

    if ((archiveType != st_ContentTypeNews) 
            && (archiveType != st_ContentTypeBooks)
            && (archiveType != st_ContentTypeDocs)
            && (archiveType != st_ContentTypeNotes))
    {
        CL_ERRORPRINTF("invalid archive type[%d]", archiveType);
        return NULL;
    }

    if ( g_rootInfo[archiveType] && g_rootInfo[archiveType]->dirArray)
    {
        directory = &g_array_index(g_rootInfo[archiveType]->dirArray,  mdsDirectory_t, 0);
        if (directory && directory->szFilename)
        {
            cp = strrchr(directory->szFilename, '/');
        }
    }
 
    CL_LOGPRINTF("%s", cp);
    return cp;
}

// check whether the externeal card which is 'storageType' is mounted or not
gboolean storageIsMounted(st_ContentType_e storageType)
{
    CL_LOGPRINTF("entry");

    gboolean found = FALSE;

    char storageDirName[ERMDS_MAX_FILENAME_SIZE];

    if ( mdsGetRootLocation(storageType, storageDirName, sizeof(storageDirName)) > 0 )
    {
        char *cp = NULL;
        char realPath[ERMDS_MAX_FILENAME_SIZE];
        
        cp = realpath(storageDirName, realPath);
        CL_LOGPRINTF("%s,%s", storageDirName, realPath);

        FILE *fp = setmntent("/proc/mounts", "r");
        struct mntent* mt = NULL;

         while ((mt = getmntent(fp)))
         {
            CL_LOGPRINTF("Partition: %s: %s %s", mt->mnt_fsname, mt->mnt_dir, mt->mnt_type);

            if (strcasecmp(mt->mnt_dir, realPath) == 0)
            {
                if ( strcasecmp(mt->mnt_type, "vfat") == 0 )
                {
                    found = TRUE;
                    break;
                }
                else
                {
                    found = FALSE;
                }
            }
         }
        
         endmntent(fp);
    }
    else
    {
        CL_ERRORPRINTF("can't get the path of storageType[%d]", storageType);
    }

    CL_LOGPRINTF("found[%d]", found); 
    return found;
}

//////////////////////////////////////// Testing routines /////////////////////////////////////////////////////

void displayScanResult(void)
{
    int     i;
    mdsFsItem_t *pFsItem;

    for (i = 0; i < g_contentArray->len; i++)
    {
        pFsItem = &g_array_index(g_contentArray, mdsFsItem_t, i);
        printf("%d. Name: %s", i, pFsItem->szFilename);
        printf("\ttype: %d\n", (int) pFsItem->fit);
    }
}
