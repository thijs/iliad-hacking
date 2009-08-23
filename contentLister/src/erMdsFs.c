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
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/vfs.h>

#include <glib/gprintf.h>
#include <gtk/gtk.h>

#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "erMdsContent.h"
#include "languages.h"
#include "metadataStoreManager.h"

#define MOBIPOCKET_CHECK1_OFFSET            0x3C
#define MOBIPOCKET_FIRST_RECORD             0x4E
#define MOBIPOCKET_CHECK2_OFFSET            0x10
#define MOBIPOCKET_ENCODING_OFFSET          0x1C
#define MOBIPOCKET_CHECK3_OFFSET            0x24
#define MOBIPOCKET_TITLE_RECORD_OFFSET      0x54
#define MOBIPOCKET_TITLE_LENGTH_OFFSET      0x58
#define MOBIPOCKET_PDB_TITLE_OFFSET         0x00
#define MOBIPOCKET_PDB_TITLE_LENGTH         0x20

#define MOBIPOCKET_ENCODING_UTF8            0xFDE9
#define MOBIPOCKET_ENCODING_WIN1252         0x04E4

// list of devices trusted to hold user documents
typedef struct
        {
            const char*  path;
            int          len;
        } trusted_device_t;

static trusted_device_t g_trusted_devices[]
                                  = {
                                        { "/mnt/free"   },
                                        { "/mnt/card"   },
                                        { "/mnt/cf"     },
                                        { "/mnt/usb"    },
                                        { "/media/card" },
                                        { "/media/cf"   },
                                        { NULL          }
                                    };

// internal functions forward declarations
static int erClGetFileType(const char *szPath);
static int erClGetSymlinkInfo(const char* szRealpath, mdsFsItemType_e* pFit);

static void mdsContentPreSort(GArray * contentItemArray, mdsSortStruct_t * sort);
static void mdsContentPostSort(GArray * contentItemArray);
static gint mdsFieldFsNameComparison(gconstpointer a, gconstpointer b, gpointer user_data);
static gint mdsFieldGenericComparison(gconstpointer a, gconstpointer b, gpointer user_data);
static int datecmp(const char* dateA, const char* dateB);
static int sizecmp(const char* sizeA, const char* sizeB);

static void mdsFetchField(mdsFsItem_t * pFsItem, mdsSortStruct_t * sort, char *szField);
static void mdsFetchFieldOfFile(mdsFsItem_t * pFsItem, mdsSortStruct_t * sort, char *szField);
static void mdsFetchFieldOfFolder(mdsFsItem_t * pFsItem, mdsSortStruct_t * sort, char *szField);
static void mdsFetchFieldOfContainer(mdsFsItem_t * pFsItem, mdsSortStruct_t * sort, char *szField);
static void mdsFetchFieldOfSymlink(mdsFsItem_t * pFsItem, mdsSortStruct_t * sort, char *szField);

static void getFakeTitle(const char* szPath, char* szTitle);
static int  getSize(const char * pathName);
static void getDate(const char *szPath, char *szDate);
static void getExtName(const char*szPath, char*szExt);
static gboolean getMobipocketTitle(const char* szPath, char* szTitle);
static inline gboolean is_mobipocket_folder(const char * szPath, const int len);

static void displayItemSetDefault(clDisplayItem_t *displayItem);
static void containerGetDisplayItem(mdsFsItem_t *pFsItem, clDisplayItem_t *displayItem);
static void symlinkGetDisplayItem(mdsFsItem_t *pFsItem, clDisplayItem_t *displayItem);
static void fileGetDisplayItem(mdsFsItem_t *pFsItem, clDisplayItem_t *displayItem);
static void folderGetDisplayItem(mdsFsItem_t *pFsItem, clDisplayItem_t *displayItem);

// Check whether the given extension is a Mobipocket extension
static inline gboolean extensionIsMobipocket(char *pExt)
{
   return (gboolean) (strcasecmp(pExt, "mobi") == 0 ||
                        strcasecmp(pExt, "prc") == 0 ||
                        strcasecmp(pExt, "pdb") == 0 );
}

// Check whether the specified path leads to a device trusted for user documents.
// This is a security measure to not allow the contentLister to read unauthorised directories.
const gchar* ctrl_is_trusted_path(const char* path)
{
    const gchar*       ret = NULL;  // return value
    trusted_device_t*  pDevice;

    static gboolean first_time = TRUE;
    static char*    download_history = NULL;  // name of the download history directory
    static int      download_history_len = 0;


    // do first-time initialisations
    if (first_time)
    {
        char   dirname[ERMDS_MAX_FILENAME_SIZE];

        // determine length for pathnames of trusted devices
        for (pDevice = (trusted_device_t*)&g_trusted_devices[0] ; pDevice->path != NULL ; pDevice++)
        {
            pDevice->len = strlen(pDevice->path);
        }

        // determine location of download history
        if (mdsGetRootLocation(st_DownloadHistory, dirname, sizeof(dirname)) > 0)
        {
            download_history     = strdup(dirname);
            download_history_len = strlen(download_history);
        }

        // first time happens only once ;-)
        first_time = FALSE;
    }
    
    // check path in trusted devices
    for (pDevice = &g_trusted_devices[0] ; pDevice->path != NULL && ret == NULL ; pDevice++)
    {
        if (strncmp(path, pDevice->path, pDevice->len) == 0)
        {
            // found
            ret = pDevice->path;
        }
    }

    // if not found, check for download history
    if (ret == NULL  &&  download_history)
    {
        if (strncmp(path, download_history, download_history_len) == 0)
        {
            // found
            ret = download_history;
        }
    }

    // return what we found
    return ret;
}

//
// scan content in dirArray, and filter out the content which are not in searchArray and searchType
// the results will be stored in contentItemArray
//
int erMdsContentScan(GArray * dirArray, GArray ** contentItemArray)
{
    gint    nDir;
    guint   initialArraySize = ERMDS_INITIAL_CONTENT_SCAN_ARRAY_SIZE;
    DIR    *dirp;
    struct dirent *direntp;
    mdsFsItem_t fsItem;
    int     ret = ERMDS_CONTENT_SCAN_OK;

    CL_LOGPRINTF("entry");

    // Only create a new array when necessary:
    if (*contentItemArray == NULL)
    {
        CL_LOGPRINTF("Creating a new contentItemArray");
        // Create a new array for initialArraySize items to store mdsFsItem_t values.
        // Don't zero-terminate or clear to 0's.
        *contentItemArray = g_array_sized_new(FALSE, FALSE, sizeof(mdsFsItem_t), initialArraySize);
    }

    CL_LOGPRINTF("dirArray->len = %d", dirArray->len);

    // Loop over all directories 
    for (nDir = 0; nDir < dirArray->len; nDir++)
    {
        // This works because mdsDirectory_t contains the filename as first member
        char   *szDir = (char *) &g_array_index(dirArray, mdsDirectory_t, nDir);

        if ((dirp = opendir(szDir)) == NULL)
        {
            // This is not an error condition
            CL_WARNPRINTF("Could not open directory %s (error %s).", szDir, strerror(errno));
        }
        else
        {
            while ((direntp = readdir(dirp)) != NULL)
            {
                // Hide '.' and '..' entries and names starting with a dot
                // Hide file/dir names starting with an underscore
                // Hide 'manifest.xml' files
                if ((direntp->d_name[0] != (int) '.') && (direntp->d_name[0] != (int) '_')
                        && (strcasecmp(MANIFEST_FILENAME, direntp->d_name) != 0))
                {
                    gint    nPathLength;

                    CL_LOGPRINTF("%s", direntp->d_name);

                    // Check on buffer overflow
                    nPathLength =
                        g_snprintf(fsItem.szFilename, ERMDS_MAX_FILENAME_SIZE, "%s/%s", szDir, direntp->d_name);
                    fsItem.sortField = NULL; 
                    fsItem.priority = 0;

                    if (nPathLength < ERMDS_MAX_FILENAME_SIZE)
                    {
                        // Determine the file item type: File / Container / Folder
                        // fprintf(stderr, "(%d)", nPathLength);
                        int gft = erClGetFileType(fsItem.szFilename);
                        switch (gft)
                        {
                            case GFT_FILE:
                                fsItem.fit = mdsFitFile;
                                break;

                            case GFT_DIR:
                                {
                                    char        szManifestPath[ERMDS_MAX_FILENAME_SIZE];
                                    struct stat statbuf;

                                    // If the directory contains a manifest file we have a container
                                    g_snprintf( szManifestPath, sizeof(szManifestPath),
                                                              "%s/" MANIFEST_FILENAME,
                                                              fsItem.szFilename       );
                                    // Does the manifest file exist?
                                    if (stat(szManifestPath, &statbuf) == 0)
                                    {
                                        if (S_ISREG(statbuf.st_mode) != 0)
                                        {
                                            fsItem.fit = mdsFitContainer;
                                        }
                                        else
                                        {
                                            CL_WARNPRINTF("%s is not a regular file, default to folder type",
                                                          szManifestPath);
                                            fsItem.fit = mdsFitFolder;
                                        }
                                    }
                                    else
                                    {
                                        fsItem.fit = mdsFitFolder;
                                    }
                                    
                                    // Mobipocket HACK, sort it always on top
                                    if (is_mobipocket_folder(fsItem.szFilename, nPathLength))
                                    {
                                        CL_LOGPRINTF("Found Mobipocket: %s", fsItem.szFilename);
                                        fsItem.priority = MOBIPOCKET_FOLDER_SORT_PRIORITY;
                                    }
                                }
                                break;

                            case GFT_SYMLINK:
                                fsItem.fit = mdsFitSymlink;
                                break;
                                
                            case GFT_NOSUCHFILE:
                            case GFT_IGNORE:
                            default:
                                continue; // while ((direntp = readdir(dirp)) != NULL)
                        }

                        if ((*contentItemArray)->len < ERMDS_MAX_CONTENT_SCAN_ARRAY_SIZE)
                        {
                            g_array_append_val(*contentItemArray, fsItem);
                        }
                        else
                        {
                            ret = ERMDS_CONTENT_SCAN_TOO_MANY_ITEMS;
                            break;
                        }
                    }
                    else
                    {
                        CL_ERRORPRINTF("Filename %s/%s too long. (nPathLength (%d) > %d)", szDir, direntp->d_name,
                                       nPathLength, ERMDS_MAX_FILENAME_SIZE);
                    }
                }
            }                   // while ((direntp = readdir(dirp)) != NULL)
            closedir(dirp);
        }
        if (ret == ERMDS_CONTENT_SCAN_TOO_MANY_ITEMS)
        {
            return ret;
        }
    }                           // for (nDir = 0; nDir < dirArray->len; nDir++)
    return ret;
}

// This function sort 'contentItemArray' by 'sort' criteria
// the results of sorting will be stored in 'contentItemArray'
int erMdsContentSort(GArray * contentItemArray, mdsSortStruct_t * sort)
{
    int     ret;
    
    CL_LOGPRINTF("entry");

    switch (sort->ft)
    {
        case mdsFieldFsName:
            g_array_sort_with_data(contentItemArray, mdsFieldFsNameComparison, (gpointer) sort);
            ret = ERMDS_CONTENT_SORT_OK;
            break;

        case mdsFieldAuthor:
        case mdsFieldDate:
        case mdsFieldDescription:
        case mdsFieldFile:
        case mdsFieldTitle:
        case mdsFieldExtName:
        case mdsFieldSize:
            mdsContentPreSort(contentItemArray, sort);
            g_array_sort_with_data(contentItemArray, mdsFieldGenericComparison, (gpointer) sort);
            mdsContentPostSort(contentItemArray);
            ret = ERMDS_CONTENT_SORT_OK;
            break;

        default:
            CL_ERRORPRINTF("No support for sorting of type %d", (int) sort->ft);
            ret = ERMDS_CONTENT_SORT_NOT_IMPLEMENTED;
    }
    return ret;
}

// calculate sortField of mdsFsItem_t before sorting
static void mdsContentPreSort(GArray * contentItemArray, mdsSortStruct_t * sort)
{
    mdsFsItem_t *pFsItem;
    int          i, total;
    char         szField[ERMDS_MAX_FILENAME_SIZE];
    int          len;
   
    CL_LOGPRINTF("entry");

    if (contentItemArray && sort)
    {
        total = contentItemArray->len;
        for (i = 0; i < total; i++)
        {
            pFsItem = &g_array_index(contentItemArray, mdsFsItem_t, i);
            if (pFsItem)
            {
                // Free the old ones.
                if (pFsItem->sortField)
                {
                    free(pFsItem->sortField);
                    pFsItem->sortField = NULL;
                }
                // Fetch the string on which to compare.
                mdsFetchField(pFsItem, sort, szField);
                len = strlen(szField) + 1;
                pFsItem->sortField = (char *)malloc(len);
                if (pFsItem->sortField)
                {
                    strcpy(pFsItem->sortField, szField);
                }
                else
                {
                    CL_ERRORPRINTF("Can't malloc memory for pFsItem->sortField.");
                }
            }
            else
            {
                CL_ERRORPRINTF("Can't get the (%d)th pFsItem.", i);
            }
        }// for
    }
}

// free the memory of sortField of mdsFsItem_t after sorting 
static void mdsContentPostSort(GArray * contentItemArray)
{
    mdsFsItem_t *pFsItem;
    int          i, total;

    CL_LOGPRINTF("entry");

    if (contentItemArray)
    {
        total = contentItemArray->len;
        for (i = 0; i < total; i++)
        {
            pFsItem = &g_array_index(contentItemArray, mdsFsItem_t, i);
            if (pFsItem)
            {
                if (pFsItem->sortField)
                {
                    free(pFsItem->sortField);
                    pFsItem->sortField = NULL;
                }
            }
            else
            {
                CL_ERRORPRINTF("Can't get the (%d)th pFsItem.", i);
            }
        }
    }
}

// Test to see if the current folder is the Mobipocket special folder.
// This comparison function should be highly optimized, because it is run on all
// files scanned. We therefore avoid calling strlen, because this information 
// is already available when calling the function.
// The current implementation has false positives, but we choose to ignore them in
// favor of the generic and fast approach.
static inline gboolean is_mobipocket_folder(const char * szPath, const int len)
{
    const char *cp = &szPath[0];

    if (len > MOBIPOCKET_FOLDER_LENGTH) 
    { 
        cp += (len - MOBIPOCKET_FOLDER_LENGTH); 
    }
    else
    {
        return FALSE;
    }
     
    if (strcmp(cp, MOBIPOCKET_FOLDER) == 0) 
    { 
        return TRUE;                       
    } 
    else 
    { 
        return FALSE; 
    } 
}

// This comparison function is treated seperately because it never involves XML parsing
static gint mdsFieldFsNameComparison(gconstpointer a, gconstpointer b, gpointer user_data)
{
    mdsFsItem_t *pFsItemA = (mdsFsItem_t *) a;
    mdsFsItem_t *pFsItemB = (mdsFsItem_t *) b;
    mdsSortStruct_t *sort = (mdsSortStruct_t *) user_data;
    int     ret;

    // Check to see if one of the files is the Mobipocket special folder
    // The Mobipocket special folder always sorts on top

    // If equal priorities, check name
    if (pFsItemA->priority == pFsItemB->priority)
    {
        if (sort->ascending == 1)
        {
            ret = strcasecmp(pFsItemA->szFilename, pFsItemB->szFilename);
        }
        else
        {
            ret = strcasecmp(pFsItemB->szFilename, pFsItemA->szFilename);
        }
    }
    else if (pFsItemA->priority < pFsItemB->priority)
    {
        ret = 1;
    }
    else 
    {
        ret = -1;
    }

    return ret;
}

static gint mdsFieldGenericComparison(gconstpointer a, gconstpointer b, gpointer user_data)
{
    mdsFsItem_t     *pFsItemA;
    mdsFsItem_t     *pFsItemB;
    mdsSortStruct_t *sort;
    char            *szFieldA = NULL;
    char            *szFieldB = NULL;
    int             ret = 0;

    pFsItemA = (mdsFsItem_t *) a;
    pFsItemB = (mdsFsItem_t *) b;
    sort = (mdsSortStruct_t *) user_data;
    
    // Check to see if one of the files is the Mobipocket special folder
    // The Mobipocket special folder always sorts on top
    
    // Handle priorities, if priorities are not equal we don't have to do fancy checks
    if (pFsItemA->priority != pFsItemB->priority)
    {
        if (pFsItemA->priority < pFsItemB->priority)
        {
            return 1;
        }
        else 
        {
            return -1;
        }
    }

    szFieldA = pFsItemA->sortField;
    szFieldB = pFsItemB->sortField;

    if (szFieldA && szFieldB)
    {
        // comparing 
        if (mdsFieldDate == sort->ft)
        {
            ret = datecmp(szFieldA, szFieldB);
        }
        else if (mdsFieldSize == sort->ft)
        {
            ret = sizecmp(szFieldA, szFieldB);
        }
        else
        {
            ret = strcasecmp(szFieldA, szFieldB);
        }
    }
    else if (szFieldA && NULL == szFieldB)
    {
        ret = 1;
        CL_LOGPRINTF("szFieldA && szFieldB == NULL");
    }
    else if (NULL == szFieldA && szFieldB)
    {
        ret = -1;
        CL_LOGPRINTF("szFieldA == NULL && szFieldB");
    }
    else 
    {
        ret = 0;
        CL_LOGPRINTF("szFieldA == szFieldB == NULL");
    }
/*
    // if equals, we using title to be sorting criteria
    if (0 == ret && mdsFieldTitle != sort->ft && mdsFieldFsName != sort->ft) 
    {
        CL_LOGPRINTF("using sorting type: title");

        mdsSortStruct_t localSort;
        char fieldA[ERMDS_MAX_FILENAME_SIZE];
        char fieldB[ERMDS_MAX_FILENAME_SIZE];

        localSort.ft = mdsFieldTitle;
        localSort.ascending = sort->ascending;

        mdsFetchField(pFsItemA, &localSort, fieldA);
        mdsFetchField(pFsItemB, &localSort, fieldB);
        ret = strcasecmp(fieldA, fieldB);
    }
*/    
    // if equals, we using filename to be sorting criteria
    if (0 == ret && mdsFieldTitle != sort->ft && mdsFieldFsName != sort->ft)
    {
        CL_LOGPRINTF("using default sorting type: Filename");
        ret = strcasecmp(pFsItemA->szFilename, pFsItemB->szFilename);
    }
    // sort ording
    if (sort->ascending == 0) 
    {
       ret = 0 - ret;
    }
    CL_LOGPRINTF("ret = %d", ret);
    return ret;
}

// dateA and dateB must be something like 2006-12-27T12:35:21
static int datecmp(const char* dateA, const char* dateB)
{   
   struct tm tmA, tmB;
   time_t timeA, timeB;
   double diff = 0;
   char   *string = NULL;

   CL_LOGPRINTF("entry");

   if (NULL == dateA || NULL == dateB)
   {
        CL_ERRORPRINTF("invalid arguments");
        return diff;
   }
   
   string = strptime(dateA, "%Y-%m-%dT%H:%M:%S", &tmA);
   string = strptime(dateB, "%Y-%m-%dT%H:%M:%S", &tmB);
   timeA = mktime(&tmA); 
   timeB = mktime(&tmB);
   diff = difftime(timeA, timeB);

   return (int)diff;
}

// sizeA and sizeB in bytes
static int sizecmp(const char* sizeA, const char* sizeB)
{
    int nSizeA, nSizeB;
    int diff = 0;

    CL_LOGPRINTF("entry");
   
    if (NULL == sizeA || NULL == sizeB)
    {
        CL_ERRORPRINTF("invalid arguments");
        return diff;
    }

    nSizeA = atoi(sizeA);
    nSizeB = atoi(sizeB);
    diff = nSizeA - nSizeB;

    return diff;
}
// 
// this function fetch fields not include mdsFieldFsName.
// for mdsFieldFsName, we can get pFsItem->szFilename easily and directly
//
// notes: must get the same result as what we show(display) to users.
// 
static void mdsFetchField(mdsFsItem_t * pFsItem, mdsSortStruct_t * sort, char *szField)
{
    if (NULL == pFsItem || NULL == sort || NULL == szField)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }

    switch (pFsItem->fit)
    {
        case mdsFitFile:
            mdsFetchFieldOfFile(pFsItem, sort, szField);
            break;
        case mdsFitFolder:
            mdsFetchFieldOfFolder(pFsItem, sort, szField);
            break;
        case mdsFitSymlink:
            mdsFetchFieldOfSymlink(pFsItem, sort, szField);
            break;
        case mdsFitContainer:
            mdsFetchFieldOfContainer(pFsItem, sort, szField);
            break;
        default:
            CL_ERRORPRINTF("fit %d is not defined", pFsItem->fit);
            strcpy(szField, "");
    }
}

static void mdsFetchFieldOfFile(mdsFsItem_t *pFsItem, mdsSortStruct_t *sort, char*szField)
{
    CL_LOGPRINTF("entry");

    int     size = 0;
    
    if (NULL == pFsItem || NULL == sort || NULL == szField)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }
    if (mdsFitFile != pFsItem->fit)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }

    switch (sort->ft)
    {
        case mdsFieldFsName:
            strcpy(szField, pFsItem->szFilename);
            break;
        case mdsFieldTitle:
            getFakeTitle(pFsItem->szFilename, szField);
            break;
        case mdsFieldDate:
            getDate(pFsItem->szFilename, szField);
            break;
        case mdsFieldSize:
            size = getSize(pFsItem->szFilename);
            g_snprintf(szField, ERMDS_MAX_FILENAME_SIZE, "%d", size);
            break;
        case mdsFieldExtName:
            getExtName(pFsItem->szFilename, szField);
            break;
        case mdsFieldAuthor:
        case mdsFieldDescription:
        case mdsFieldFile:
        default:
            strcpy(szField, "");
    }
    CL_LOGPRINTF("%s", szField);
}

static void mdsFetchFieldOfFolder(mdsFsItem_t *pFsItem, mdsSortStruct_t *sort, char*szField)
{
    CL_LOGPRINTF("entry");

    if (NULL == pFsItem || NULL == sort || NULL == szField)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }
    if (pFsItem->fit != mdsFitFolder)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }
   
   switch (sort->ft)
   {
         case mdsFieldFsName:
            strcpy(szField, pFsItem->szFilename);
            break;
        case mdsFieldTitle:
            getFakeTitle(pFsItem->szFilename, szField);
            break;
        case mdsFieldDate:
            getDate(pFsItem->szFilename, szField);
            break;
        case mdsFieldSize:
            {
                // if it's a folder, we treat its size to be zero, otherwise it'll be slow
                // int size = getSize(pFsItem->szFilename);
                 int size = 0;
                 g_snprintf(szField, ERMDS_MAX_FILENAME_SIZE, "%d", size);
            }
            break;
        case mdsFieldExtName:
        case mdsFieldAuthor:
        case mdsFieldDescription:
        case mdsFieldFile:
        default:
            strcpy(szField, "");
   }
   CL_LOGPRINTF("%s", szField);
}

static void mdsFetchFieldOfContainer(mdsFsItem_t *pFsItem, mdsSortStruct_t *sort, char*szField)
{
    CL_LOGPRINTF("entry");

   char    szManifestPath[ERMDS_MAX_FILENAME_SIZE];

    if (NULL == pFsItem || NULL == sort || NULL == szField)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }
    if (mdsFitContainer != pFsItem->fit)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }

    // get the field from manifest
    if (mdsFieldDate == sort->ft || mdsFieldDescription == sort->ft
       || mdsFieldFile == sort->ft || mdsFieldTitle == sort->ft
       || mdsFieldSize == sort->ft)
    {
       // we assume the manifest file exists: no checks
       g_snprintf(szManifestPath, ERMDS_MAX_FILENAME_SIZE, "%s/" MANIFEST_FILENAME, pFsItem->szFilename);
       CL_LOGPRINTF("Calling erClXmlFetchField: %s, %d, ...", szManifestPath, (int) sort->ft);
       erClXmlFetchField(szManifestPath, sort->ft, szField);
    }
    else if (mdsFieldExtName == sort->ft)
    {
        char szStartPage[ERMDS_MAX_FILENAME_SIZE+1];
        char szStartPagePath[ERMDS_MAX_FILENAME_SIZE+1];
        // we assume the manifest file exists: no checks
        g_snprintf(szManifestPath, ERMDS_MAX_FILENAME_SIZE, "%s/" MANIFEST_FILENAME, pFsItem->szFilename);
        CL_LOGPRINTF("Calling erClXmlFetchField: %s, %d, ...", szManifestPath, (int) mdsFieldFile);
        erClXmlFetchField(szManifestPath, mdsFieldFile, szStartPage);
        g_snprintf(szStartPagePath, ERMDS_MAX_FILENAME_SIZE, "%s/%s", pFsItem->szFilename, szStartPage);
        getExtName(szStartPagePath, szField);
    }
    else
    {
        CL_LOGPRINTF("Field(%d) never be found in manifest.", (int)sort->ft);
        strcpy(szField, "");
    }
    // deal with the scenario we can't get the field value from manifest 
    if (0 == strlen(szField))
    {
        switch (sort->ft)
        {
            case mdsFieldFsName:
                strcpy(szField, pFsItem->szFilename);
                break;
            case mdsFieldDate:
                getDate(pFsItem->szFilename, szField);
                break;
            case mdsFieldTitle:
                getFakeTitle(pFsItem->szFilename, szField);
                break;
            case mdsFieldSize:
                {
                    int size = 0;
                    size = getSize(pFsItem->szFilename);
                    g_snprintf(szField, ERMDS_MAX_FILENAME_SIZE, "%d", size);
                    // write the size of container to manifest.xml 
                    erClXmlSetFieldSize(pFsItem->szFilename, size);    
                }
                break;
            default:
                    CL_ERRORPRINTF("can't get the field value");
        }
    }
    CL_LOGPRINTF("Time of %s: %s", pFsItem->szFilename, szField);
}

//  we assume all symbollink is invalid, because we already do "hsCleanupSymbollink" before calling this function
static void mdsFetchFieldOfSymlink(mdsFsItem_t *pFsItem, mdsSortStruct_t *sort, char*szField)
{
    CL_LOGPRINTF("entry");

    char*   cp = NULL;
    char    szFilename[ERMDS_MAX_FILENAME_SIZE];
    char    szRealpath[PATH_MAX];
    mdsFsItemType_e fit;
    mdsFsItem_t     fsItem;
   
    if (NULL == pFsItem || NULL == sort || NULL == szField)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }
    if (mdsFitSymlink != pFsItem->fit)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }

    strcpy(szField, ""); 
    // Convert symlinks into the real path
    // Note: also needed for symlinks in directory path, e.g. /opt/content/books/...
    cp = realpath(pFsItem->szFilename, szRealpath);
    if (cp != NULL)
    {
        snprintf(szFilename, sizeof(szFilename), "%s", szRealpath);
        // Determine the fit and filename for symlinks
        erClGetSymlinkInfo(szFilename, &fit);
        
       if (mdsFitFile == fit || mdsFitFolder == fit || mdsFitContainer == fit)
       {
          // the realpath is a file or a folder or a container
          fsItem.fit = fit;
          strcpy(fsItem.szFilename, szFilename);
          mdsFetchField(&fsItem, sort, szField);
       }
       else
       {
            CL_ERRORPRINTF("entry");
       }
    }
    else
    {
        CL_ERRORPRINTF("entry");
    }

    CL_LOGPRINTF("entry");
}

static void getFakeTitle(const char* szPath, char* szTitle)
{
    char   *pChar = NULL;

    if (NULL == szPath || NULL == szTitle)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }
    
    strcpy(szTitle, "");

    if (strlen(szPath))
    {
        // Mobipocket files are different
        // We need to try to get the title from the file itself
        pChar = strrchr(szPath, (int) '.');
        if (pChar != NULL)
        {
            pChar++;
            if (extensionIsMobipocket(pChar))
            {
                if (getMobipocketTitle(szPath, szTitle))
                {
                    return;
                }
            }
        }
        
        // Use filename as title
        pChar = strrchr(szPath, (int) '/');
        if (pChar != NULL)
        {
            strcpy(szTitle, ++pChar);
        }
        else
        {
            CL_WARNPRINTF("Can not determine title of %s", szPath);
        }
    }
    else
    {
        CL_WARNPRINTF("Can not determine title of %s", szPath);
    }
    CL_LOGPRINTF("the title of %s is %s", szPath, szTitle);
}                   

// This function will try to read the title from the mobipocket file
// in szPath and strcpy the title found into szTitle
// Returns TRUE if title is found, FALSE otherwise
static gboolean getMobipocketTitle(const char* szPath, char* szTitle)
{
    GIOChannel *io;
    guint32 firstRecordIndex = 0;
    guint32 titleEncoding = 0;
    guint32 titleOffset = 0;
    guint32 titleLength = 0;
    guint32 check3Value = 0;
    int i;
    gchar *encoding = NULL;
    gchar *p;
    GString *tmpTitle;
    gunichar uniChar = 0;
    GIOStatus st;

    gchar buf[9];
    memset(buf, '\0', sizeof(buf));
    
    CL_LOGPRINTF("entry: [%s]", szPath);
    
    io = g_io_channel_new_file(szPath, "r", NULL);
    if (io != NULL)
    {
        g_io_channel_set_encoding(io, NULL, NULL); // Binary data

        // First check: make sure this is a Mobipocket document
        st = g_io_channel_seek_position(io, MOBIPOCKET_CHECK1_OFFSET, G_SEEK_SET, NULL);
        if (st != G_IO_STATUS_NORMAL)
        {
            CL_ERRORPRINTF("Unable to seek to 0x%x", MOBIPOCKET_CHECK1_OFFSET);
            goto return_false;
        }
        g_io_channel_read_chars(io, buf, 8, NULL, NULL);
        buf[8] = '\0';
        if (strcmp(buf, "BOOKMOBI") != 0)
        {
            CL_ERRORPRINTF("Identifier 'BOOKMOBI' not found");
            if (strcmp(buf, "TEXtREAd") == 0)
            {
                CL_WARNPRINTF("Identifier 'TEXtREAd' found; Assuming PDB title is valid.");
                // Assume UTF-8 encoding and read title from location 0 (PDB mode)
                // This skips all other tests
                firstRecordIndex = 0;
                titleOffset = MOBIPOCKET_PDB_TITLE_OFFSET;
                titleLength = MOBIPOCKET_PDB_TITLE_LENGTH;
                encoding = g_strdup("UTF-8");
                goto read_title;        
            }
            else
            {
                goto return_false;
            }
        }
        memset(buf, '\0', sizeof(buf));

        // Get firstRecordIndex
        st = g_io_channel_seek_position(io, MOBIPOCKET_FIRST_RECORD, G_SEEK_SET, NULL);
        if (st != G_IO_STATUS_NORMAL)
        {
            CL_ERRORPRINTF("Unable to seek to 0x%x", firstRecordIndex);
            goto return_false;
        }
        g_io_channel_read_chars(io, buf, 4, NULL, NULL);
        for (i = 0; i < 4; i++)
        {
            firstRecordIndex = firstRecordIndex << 8;
            firstRecordIndex = firstRecordIndex + buf[i];
        }
        memset(buf, '\0', sizeof(buf));
        
        CL_LOGPRINTF("firstRecordIndex = 0x%x", firstRecordIndex);

        // Second check: make sure this is a Mobipocket document
        st = g_io_channel_seek_position(io, firstRecordIndex + MOBIPOCKET_CHECK2_OFFSET, G_SEEK_SET, NULL);
        if (st != G_IO_STATUS_NORMAL)
        {
            CL_ERRORPRINTF("Unable to seek to 0x%x", firstRecordIndex + MOBIPOCKET_CHECK2_OFFSET);
            goto return_false;
        }
        g_io_channel_read_chars(io, buf, 4, NULL, NULL);
        buf[4] = '\0';
        if (strcmp(buf, "MOBI") != 0)
        {
            CL_ERRORPRINTF("Identifier 'MOBI' not found");
            goto return_false;
        }
        memset(buf, '\0', sizeof(buf));
        
        // Get encoding of the title
        st = g_io_channel_seek_position(io, firstRecordIndex + MOBIPOCKET_ENCODING_OFFSET, G_SEEK_SET, NULL);
        if (st != G_IO_STATUS_NORMAL)
        {
            CL_ERRORPRINTF("Unable to seek to 0x%x", firstRecordIndex + MOBIPOCKET_ENCODING_OFFSET);
            goto return_false;
        }
        g_io_channel_read_chars(io, buf, 4, NULL, NULL);
        for (i = 0; i < 4; i++)
        {
            titleEncoding = titleEncoding << 8;
            titleEncoding = titleEncoding + buf[i];
        }
        if (titleEncoding == MOBIPOCKET_ENCODING_UTF8)
        {
            encoding = g_strdup("UTF-8");
        }
        else if (titleEncoding == MOBIPOCKET_ENCODING_WIN1252)
        {
            encoding = g_strdup("WINDOWS-1252");
        }
        else
        {
            CL_ERRORPRINTF("Unknown encoding 0x%x", titleEncoding);
            goto return_false;
        }
        memset(buf, '\0', sizeof(buf));

        // Check 3: Make sure this is a Mobipocket document (value must be >= 2)
        st = g_io_channel_seek_position(io, firstRecordIndex + MOBIPOCKET_CHECK3_OFFSET, G_SEEK_SET, NULL);
        if (st != G_IO_STATUS_NORMAL)
        {
            CL_ERRORPRINTF("Unable to seek to 0x%x", firstRecordIndex + MOBIPOCKET_CHECK3_OFFSET);
            goto return_false;
        }
        g_io_channel_read_chars(io, buf, 4, NULL, NULL);
        for (i = 0; i < 4; i++)
        {
            check3Value = check3Value << 8;
            check3Value = check3Value + buf[i];
        }
        memset(buf, '\0', sizeof(buf));

        if (check3Value < 2)
        {
            // PDB title mode: title is in first 32 bytes of the file
            CL_WARNPRINTF("Check 3: value < 2 ; fallback to PDB mode");
            firstRecordIndex = 0;
            titleOffset = MOBIPOCKET_PDB_TITLE_OFFSET;
            titleLength = MOBIPOCKET_PDB_TITLE_LENGTH;
        }
        else
        {
            // Normal Mobipocket document: get the title offset
        
            // Get title offset (big endian int)
            st = g_io_channel_seek_position(io, firstRecordIndex + MOBIPOCKET_TITLE_RECORD_OFFSET, G_SEEK_SET, NULL);
            if (st != G_IO_STATUS_NORMAL)
            {
                CL_ERRORPRINTF("Unable to seek to 0x%x", firstRecordIndex + MOBIPOCKET_TITLE_RECORD_OFFSET);
                goto return_false;
            }
            g_io_channel_read_chars(io, buf, 4, NULL, NULL);
            for (i = 0; i < 4; i++)
            {
                titleOffset = titleOffset << 8;
                titleOffset = titleOffset + buf[i];
            }
            memset(buf, '\0', sizeof(buf));
            
            // Get title length (big endian int)
            st = g_io_channel_seek_position(io, firstRecordIndex + MOBIPOCKET_TITLE_LENGTH_OFFSET, G_SEEK_SET, NULL);
            if (st != G_IO_STATUS_NORMAL)
            {
                CL_ERRORPRINTF("Unable to seek to 0x%x", firstRecordIndex + MOBIPOCKET_TITLE_LENGTH_OFFSET);
                goto return_false;
            }
            g_io_channel_read_chars(io, buf, 4, NULL, NULL);
            for (i = 0; i < 4; i++)
            {
                titleLength = titleLength << 8;
                titleLength = titleLength + buf[i];
            }
            memset(buf, '\0', sizeof(buf));
            
            // Check length and truncate if needed
            if (titleLength >= ERMDS_MAX_FILENAME_SIZE)
            {
                CL_ERRORPRINTF("titleLength %d >= %d; truncating...", titleLength, ERMDS_MAX_FILENAME_SIZE);
                titleLength = ERMDS_MAX_FILENAME_SIZE - 1;
            }
            else if (titleLength == 0)
            {
                // When the titleLength == 0, we fallback to PDB title mode 
                // and get the title from the first 32 bytes of the file
                CL_WARNPRINTF("titleLength == 0");
                firstRecordIndex = 0;
                titleOffset = MOBIPOCKET_PDB_TITLE_OFFSET;
                titleLength = MOBIPOCKET_PDB_TITLE_LENGTH;
            }

        } // if check3Value < 2
        
read_title:

        CL_LOGPRINTF("titleLength = %d, titleOffset = 0x%x", titleLength, titleOffset);

        // Get the title at titleOffset, read titleLength bytes
        st = g_io_channel_seek_position(io, firstRecordIndex + titleOffset, G_SEEK_SET, NULL);
        if (st != G_IO_STATUS_NORMAL)
        {
            CL_ERRORPRINTF("Unable to seek to 0x%x", firstRecordIndex + titleOffset);
            goto return_false;
        }
        // Set the source encoding for the string to be read
        // Strings are automatically converted to unicode when read
        st = g_io_channel_set_encoding(io, encoding, NULL);
        if (st != G_IO_STATUS_NORMAL)
        {
            CL_ERRORPRINTF("Unable to change channel encoding to %s", encoding);
            goto return_false;
        }
        
        CL_LOGPRINTF("Channel encoding changed to %s", encoding);

        // Read titleLength chars, not bytes (because some chars might be multibyte)
        tmpTitle = g_string_sized_new(titleLength);
        for (i = 0; i < titleLength; i++)
        {
            st = g_io_channel_read_unichar(io, &uniChar, NULL);
            if (st != G_IO_STATUS_NORMAL)
            {
                CL_ERRORPRINTF("Unable to read char from pos %d", i);
                goto return_false;
            }
            g_string_append_unichar(tmpTitle, uniChar);
        }
        strcpy(szTitle, tmpTitle->str);
        g_string_free(tmpTitle, TRUE); // free the string including character data

        // Replace all '&amp;' references in szTitle with only '&'
        while ((p = strstr(szTitle, "&amp;")) != NULL)
        {
            strcpy(p + 1, p + 5);
        }

        CL_LOGPRINTF("Found title [%s]", szTitle);
    }
    else
    {
        CL_ERRORPRINTF("Unable to open file [%s]", szPath);
        goto return_false;
    }
    
    // All OK, free stuff and close filepointer
    if (encoding) g_free(encoding);
    g_io_channel_shutdown(io, FALSE, NULL);
    g_io_channel_unref(io);
    CL_LOGPRINTF("leave");
    return TRUE;
    
return_false:
    if (encoding) g_free(encoding);
    if (io)
    {
        g_io_channel_shutdown(io, FALSE, NULL);
        g_io_channel_unref(io);
    }

    CL_WARNPRINTF("Not a valid Mobipocket file [%s]", szPath);
    return FALSE;
}

static int getSize(const char * pathName)
{
    struct stat statbuf;

    // Determin file size
    if (0 != stat(pathName, &statbuf))
    {
        return ERCL_INVALID_SIZE;
    }
    
    if (S_ISREG(statbuf.st_mode))
    {
        int size = 0;
        size = statbuf.st_blocks * 512;
        CL_LOGPRINTF("the title of %s is %d", pathName, size);
        return statbuf.st_blocks * 512; // Use blocks here to show size on disk
    }    
    
    if (S_ISDIR(statbuf.st_mode))
    {
        // visit dir and accumulate
        DIR * dir = opendir(pathName);
        struct dirent *entry;
        int size   = 0, item = 0;
        if (NULL == dir) 
        {
            CL_WARNPRINTF("Open dir returns NULL");
            return size;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(".", entry->d_name) && strcmp("..", entry->d_name))
            {
                char path[ERMDS_MAX_FILENAME_SIZE] ={0};
                snprintf(path, ERMDS_MAX_FILENAME_SIZE, "%s/%s",
                    pathName, entry->d_name);
                item = getSize(path);
                CL_LOGPRINTF("name: %s", path);
                if (ERCL_INVALID_SIZE != item)
                {
                    size += item;
                }
            }
        }       
        closedir(dir);
        CL_LOGPRINTF("the title of %s is %d", pathName, size);
        return size;
    }    
    return ERCL_INVALID_SIZE;
}
//
// date format is the same as it is in manifest.xml
// get the date of regular file and folder, how about link file?
//
static void getDate(const char *szPath, char *szDate)
{
    struct stat statBuf;

    if (NULL == szPath || NULL == szDate)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }
    
    strcpy(szDate, "");             

    if (strlen(szPath))
    {
        if (stat(szPath, &statBuf) == 0)
        {
            if (S_ISREG(statBuf.st_mode) != 0 || S_ISDIR(statBuf.st_mode))
            {
                // What is date?
                struct tm localTime;

                localtime_r(&statBuf.st_mtime, &localTime);
                strftime(szDate, ERCL_MAX_SUBTITLE_SIZE, "%Y-%m-%dT%H:%M:%S", &localTime);
                CL_LOGPRINTF("Time of %s: %s", szPath, szDate);
            }
            else if (S_ISLNK(statBuf.st_mode))
            {
                CL_WARNPRINTF("getDate of S_ISLNK not implemented yet.");
            }
            else
            {
                CL_WARNPRINTF("%s is not a regular file or directory", szPath);
            }
        }
    }
    else
    {
        CL_WARNPRINTF("Can't determinte the date of %s", szPath);
    }
}

static void getExtName(const char*szPath, char*szExt)
{
    struct stat statbuf;
    char *pChar = NULL;
    char *pTmp = NULL;
 
    if (NULL == szPath || NULL == szExt)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }

    strcpy(szExt, "");

    if (strlen(szPath))
    {

        if (0 != stat(szPath, &statbuf))
        {
            CL_WARNPRINTF("entry");
            // dummy.bmp doesn't exsits, do something special for it
            if (strstr(szPath, "dummy.bmp"))
            {
               strcpy(szExt, "bmp");
            }
        }
        else
        {
            if (S_ISREG(statbuf.st_mode))
            {
                pChar = strrchr(szPath, (int) '.');
                pTmp = strrchr(szPath, (int) '/');
                // It is only an extension if it is part of the filename
                // Check this by comparing the position of the last '/' 
                // to the position of the last '.'
                // A path will always contain one '/', but we check for a 
                // NULL pointer anyway.
                if (pChar != NULL && (pTmp == NULL || pTmp < pChar))
                {
                    strcpy(szExt, ++pChar);
                    CL_LOGPRINTF("the extname of %s is %s", szPath, szExt);
                }
                else
                {
                    CL_WARNPRINTF("%s does not have a file extension", szPath);
                }
            }    
            else if(S_ISDIR(statbuf.st_mode))
            {
               CL_WARNPRINTF("%s is a directory", szPath);
            }
            else if(S_ISLNK(statbuf.st_mode))
            {
               CL_WARNPRINTF("%s is a symbollink", szPath);
            }
            else
            {
                CL_WARNPRINTF("entry");
            }
        }
    }
    else
    {
        CL_WARNPRINTF("%s does not have a file extension", szPath);
    }

    // report extension always as lower case
    for (pChar = szExt ; *pChar ; pChar++)
    {
        *pChar = tolower(*pChar);
    }
}

// 
static int erClGetFileType(const char *szPath)
{
    struct stat statBuf;
    char * ext;
    
    // TODO ---- TODO ---- TODO ---- TODO
    // Check file extension to see if we have to ignore it
    // We should create a nice function and get values
    // from the registry. Now this is hardcoded for Mobipocket.
    // PLEASE FIX ME!!!
    ext = strrchr(szPath, '.');
    if (ext && strcasecmp(++ext, "mbp") == 0) 
    {
        CL_LOGPRINTF("Ignoring [%s] because it has extension [%s]", szPath, ext);
        return GFT_IGNORE;
    }

    if (lstat(szPath, &statBuf) == -1)
    {
        CL_ERRORPRINTF("lstat(%s) returned with error [%s]", szPath, strerror(errno));
        return GFT_NOSUCHFILE;
    }
    else
    {
        if (S_ISLNK(statBuf.st_mode))
        {
            return GFT_SYMLINK;
        }
        else if (S_ISDIR(statBuf.st_mode))
        {
            return GFT_DIR;
        }
        else
        {
            return GFT_FILE;
        }
    }
}

static int erClGetSymlinkInfo(const char* szRealpath, mdsFsItemType_e* pFit)
{
    int         n;
    char        szManifestPath[ERMDS_MAX_FILENAME_SIZE];
    struct stat statbuf;
    int         filetype;
    int         ret;

    // Default return: error
    ret = ERCL_NOMATCH;
    *pFit = mdsFitUndefined;
    g_assert(szRealpath != NULL);

    // Determine the fit and filename for symlinks
    filetype = erClGetFileType(szRealpath);
    switch (filetype)
    {
        case GFT_FILE:
            *pFit = mdsFitFile;
            ret = ERCL_OK;
            break;

        case GFT_DIR:
            *pFit = mdsFitFolder;
            ret = ERCL_OK;

            // If the directory contains a manifest file we have a container
            n = g_snprintf(szManifestPath, ERMDS_MAX_FILENAME_SIZE, "%s/" MANIFEST_FILENAME, szRealpath);
            if (n < ERMDS_MAX_FILENAME_SIZE)
            {
                // Does the manifest file exist?
                if (   stat(szManifestPath, &statbuf) == 0
                    && S_ISREG(statbuf.st_mode)           )
                {
                    *pFit = mdsFitContainer;
                }
            }
            break;

        default:
            CL_ERRORPRINTF("unexpected filetype [%d] for [%s]", filetype, szRealpath);
            ;  // ignore
    }

    return ret;
}

int erClGetDisplayItems(GArray * contentItemArray, int nBegin, int *nChunk, clDisplayItem_t * displayItemArray)
{
    int     nEnd = 0;
    int     i;
    int     j;
    mdsFsItem_t*    pFsItem;

    // Validate parameters
    if (nBegin < 0 || nBegin >= contentItemArray->len)
    {
        return ERCL_INVALID_ARGS;
    }
    else if (*nChunk < 0)
    {
        return ERCL_INVALID_ARGS;
    }

    nEnd = (contentItemArray->len - 1 <= nBegin + *nChunk - 1) ? contentItemArray->len - 1 : nBegin + *nChunk - 1;
    *nChunk = nEnd - nBegin + 1;

    // i counts displayItemArray items
    // j counts contentItemArray items
    for (i = 0, j = nBegin; i < *nChunk; i++, j++)
    {
        pFsItem = &g_array_index(contentItemArray, mdsFsItem_t, j);

        // set default value of displayItem
        displayItemSetDefault(&displayItemArray[i]);
        
        // Get information to be displayed
        switch (pFsItem->fit)
        {
            case mdsFitFile:            
                fileGetDisplayItem(pFsItem, &displayItemArray[i]);
                break;

            case mdsFitContainer:
                containerGetDisplayItem(pFsItem, &displayItemArray[i]);
                break;

            case mdsFitFolder:
                folderGetDisplayItem(pFsItem, &displayItemArray[i]);
                break;

            case mdsFitSymlink:
                symlinkGetDisplayItem(pFsItem, &displayItemArray[i]);
                break;

            case mdsFitStorage:
            case mdsFitApplication:
            case mdsFitManifestDirectory:
            default:
                CL_LOGPRINTF("No action required."); // No action required.
        }
    }
    
    return ERCL_OK;
}

static void displayItemSetDefault(clDisplayItem_t *displayItem)
{
    if (NULL == displayItem)
    {
        return;
    }
    displayItem->iconID = clUnknownIcon;
    strcpy(displayItem->clIconURL, "");
    strcpy(displayItem->szFileExt, "");
    strcpy(displayItem->szTitle, "");
    strcpy(displayItem->szSubTitle, "");
    strcpy(displayItem->szDate, "");
    strcpy(displayItem->szDescription, "");
    strcpy(displayItem->szFilename, "");
    strcpy(displayItem->szManifest, "");
    displayItem->size = ERCL_INVALID_SIZE;
    displayItem->fit = mdsFitUndefined;
    displayItem->modifyEnable.bDefault  = TRUE;
    displayItem->modifyEnable.bDelete   = TRUE;
    displayItem->modifyEnable.bScribble = TRUE;
    displayItem->modifyEnable.bTagging  = TRUE;
}

static void fileGetDisplayItem(mdsFsItem_t *pFsItem, clDisplayItem_t *displayItem)
{
    char*   cp = NULL;
    char    szFilename[ERMDS_MAX_FILENAME_SIZE];
    char    szRealpath[ERMDS_MAX_FILENAME_SIZE];

    // check whether arguments valid or not
    if (NULL == pFsItem || pFsItem->fit != mdsFitFile || NULL == displayItem)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }
    // Convert symlinks into the real path
    // Note: also needed for symlinks in directory path, e.g. /opt/content/books/...
    cp = realpath(pFsItem->szFilename, szRealpath);
    if (cp != NULL)
    {
         CL_LOGPRINTF("entry %s", szRealpath);
         snprintf(szFilename, sizeof(szFilename), "%s", szRealpath);
    }
    else
    {
        CL_ERRORPRINTF("entry %s", pFsItem->szFilename);
        strcpy(szFilename, pFsItem->szFilename);
    }

    // szFilename   
    strcpy(displayItem->szFilename, szFilename);
    // fit
    displayItem->fit = pFsItem->fit;
    // sztitle
    getFakeTitle(szFilename, displayItem->szTitle);
    // szFileExt 
    getExtName(szFilename, displayItem->szFileExt);
    if (0 == strlen(displayItem->szFileExt))
    {
        CL_WARNPRINTF("%s does not have a file extension", szFilename);
    }
    // szDate
    getDate(szFilename, displayItem->szDate);
    // size
    displayItem->size = getSize(szFilename);
}

static void folderGetDisplayItem(mdsFsItem_t *pFsItem, clDisplayItem_t *displayItem)
{
    char*   cp = NULL;
    char    szFilename[ERMDS_MAX_FILENAME_SIZE];
    char    szRealpath[PATH_MAX];

    if (pFsItem == NULL || pFsItem->fit != mdsFitFolder || displayItem == NULL)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }
    // Convert symlinks into the real path
    // Note: also needed for symlinks in directory path, e.g. /opt/content/books/...
    cp = realpath(pFsItem->szFilename, szRealpath);
    if (cp != NULL)
    {
         CL_LOGPRINTF("entry %s", szRealpath);
         snprintf(szFilename, sizeof(szFilename), "%s", szRealpath);
    }
    else
    {
        CL_ERRORPRINTF("entry %s", pFsItem->szFilename);
        strcpy(szFilename, pFsItem->szFilename);
    }
     // szFilename
    strcpy(displayItem->szFilename, szFilename);
    // fit
    displayItem->fit = pFsItem->fit;
    // Determine Title
    getFakeTitle(szFilename, displayItem->szTitle);
    // Determine Icon
    displayItem->iconID = clFolderIcon;
    // Determine date
    getDate(szFilename, displayItem->szDate);
    // determinte size
    //  int size = getSize(pFsItem->szFilename);
    //  g_snprintf(szField, ERMDS_MAX_FILENAME_SIZE, "%d", size);
}

static void containerGetDisplayItem(mdsFsItem_t *pFsItem, clDisplayItem_t *displayItem)
{
    char*   cp = NULL;
    char    szFilename[ERMDS_MAX_FILENAME_SIZE];
    char    szRealpath[PATH_MAX];
    char    szManifestPath[ERMDS_MAX_FILENAME_SIZE];
    char    storageDirName[ERMDS_MAX_FILENAME_SIZE];
    int     nPathLength;
    int     ret;
    struct stat   statbuf;
    struct statfs statfsbuf;
    st_ContentType_e storageType;
    mdsFsItem_t fsItemSymlink;

    static int  nSymlinkChain = 0;
    
    if (NULL == pFsItem || pFsItem->fit != mdsFitContainer || NULL == displayItem)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }

    // Convert symlinks into the real path
    // Note: also needed for symlinks in directory path, e.g. /opt/content/books/...
    cp = realpath(pFsItem->szFilename, szRealpath);
    if (cp != NULL)
    {
         CL_LOGPRINTF("entry %s", szRealpath);
         snprintf(szFilename, sizeof(szFilename), "%s", szRealpath);
    }
    else
    {
        CL_LOGPRINTF("entry %s", pFsItem->szFilename);
        strcpy(szFilename, pFsItem->szFilename);
    }
   
    // fit = filesystem item type
    displayItem->fit = pFsItem->fit;

    // A container is a directory containing a manifest file
    nPathLength = g_snprintf(szManifestPath, ERMDS_MAX_FILENAME_SIZE, "%s/" MANIFEST_FILENAME, szFilename);
    if (   stat(szManifestPath, &statbuf) == 0
        && S_ISREG(statbuf.st_mode)           )
    {
        // Store Manifest file
        strcpy(displayItem->szManifest, szManifestPath);

        // Manifest file exists: parse the manifest file
        // Note: function erClXmlParseManifest() may change displayItem->fit and ->szManifest
        ret = erClXmlParseManifest(szFilename, displayItem);

        if (ret == 0)
        {
            switch (displayItem->fit)
            {
                case mdsFitContainer:
                    // normal container: determine the file extension (of the start page)
                    getExtName(displayItem->szFilename, displayItem->szFileExt);
                    if (0 == strlen(displayItem->szFileExt))
                    {
                        CL_WARNPRINTF("%s does not have a file extension", szFilename);
                    }
                    if (displayItem->size <= 0)
                    {
                        // write the size of container to manifest.xml
                        displayItem->size = getSize(szFilename);
                        erClXmlSetFieldSize(szFilename, displayItem->size);    
                    }
                    break;
                    
                case mdsFitStorage:
                    // storage device: get free space when possible
                    storageType = ctrl_get_storageType(displayItem->szExtID);
                    if (   stackIsStorage(storageType)
                        && storageIsMounted(storageType)
                        && mdsGetRootLocation(storageType, storageDirName, sizeof(storageDirName)) > 0 )
                    {
                        if (statfs(storageDirName, &statfsbuf) == 0)
                        {
                            displayItem->size = statfsbuf.f_bavail * statfsbuf.f_bsize / 1024;
                        }
                    }
                    break;

                case mdsFitManifestSymlinkLocale:
                    // Use locale-specific directory, if present.
                    snprintf( szFilename, sizeof(szFilename),
                              "%s/.%s",
                              displayItem->szFilename,
                              languagesGetLocale()    );
                    if ( stat(szFilename, &statbuf) == 0 )
                    {
                        strcpy(displayItem->szFilename, szFilename);
                    }
                    else
                    {
                        snprintf( szFilename, sizeof(szFilename),
                                  "%s/.%s",
                                  displayItem->szFilename,
                                  DEFAULT_LOCALE          );
                        if ( stat(szFilename, &statbuf) == 0 )
                        {
                            strcpy(displayItem->szFilename, szFilename);
                        }
                        else
                        {
                            CL_WARNPRINTF("not found [%s]", szFilename);
                            // downgrade to folder item
                            fsItemSymlink     = *pFsItem;
                            fsItemSymlink.fit = mdsFitFolder;
                            strcpy(fsItemSymlink.szFilename, displayItem->szFilename);
                            folderGetDisplayItem(&fsItemSymlink, displayItem);
                            break;  // exit switch, NO FALLTHROUGH
                        }
                    }
                    // FALLTHROUGH to case mdsFitManifestSymlink

                case mdsFitManifestSymlink:
                    if (nSymlinkChain < ERMDS_MAX_SYMLINK_CHAIN)
                    {
                        nSymlinkChain++;
                        // simulate an FsItem for a symbolic link at filesystem level
                        fsItemSymlink     = *pFsItem;
                        fsItemSymlink.fit = mdsFitSymlink;
                        strcpy(fsItemSymlink.szFilename, displayItem->szFilename);
                        // get details for symlink, or leave displayItem unchanged
                        symlinkGetDisplayItem(&fsItemSymlink, displayItem);
                        nSymlinkChain--;
                    }
                    else
                    {
                        CL_ERRORPRINTF("Symlink chain too long for [%s]", displayItem->szFilename);
                        ret = -1;
                    }
                    break;

                default:
                    if (displayItem->iconID == clUnknownIcon)
                    {
                        displayItem->iconID = clFolderIcon;
                    }
            }
        }
        else
        {
            CL_ERRORPRINTF("Failure to parse %s", szManifestPath);
            CL_ERRORPRINTF("Downgrading %s to folder", szFilename);
            // szFilename
            strcpy(displayItem->szFilename, szFilename);
            // Changing 'fit' to folder
            displayItem->fit = mdsFitFolder;
            // Determine Icon
            displayItem->iconID = clFolderIcon;
            // Determine title
            getFakeTitle(szFilename, displayItem->szTitle);
        }
    }
    else
    {
        // Normally this branch will not be taken.
        CL_ERRORPRINTF("%s is missing. Illegal branch", szManifestPath);
    }
}

static void symlinkGetDisplayItem(mdsFsItem_t *pFsItem, clDisplayItem_t *displayItem)
{
    char*   cp = NULL;
    char    szFilename[ERMDS_MAX_FILENAME_SIZE];
    char    szRealpath[PATH_MAX];
    mdsFsItem_t fsItem;
    mdsFsItemType_e fit;

    if (NULL == pFsItem || pFsItem->fit != mdsFitSymlink || NULL == displayItem)
    {
        CL_ERRORPRINTF("invalid arguments");
        return;
    }

    // Convert symlinks into the real path
    // Note: also needed for symlinks in directory path, e.g. /opt/content/books/...
    cp = realpath(pFsItem->szFilename, szRealpath);
    if (cp != NULL)
    {
        // Determine the fit and filename for symlinks
        snprintf(szFilename, sizeof(szFilename), "%s", szRealpath);
        erClGetSymlinkInfo(szFilename, &fit);
        displayItem->fit = fit;

        fsItem.fit = fit;
        strcpy(fsItem.szFilename, szFilename);
        switch (fit)
        {
            case mdsFitContainer:
                containerGetDisplayItem(&fsItem, displayItem);
                break;
            case mdsFitFile:
                fileGetDisplayItem(&fsItem, displayItem);
                break;
            case mdsFitFolder:
                folderGetDisplayItem(&fsItem, displayItem);
                break;
            default:
                CL_ERRORPRINTF("entry");
                break;
        }
    }
    else
    {
        CL_ERRORPRINTF("realpath error on [%s]", pFsItem->szFilename);
    }
}

