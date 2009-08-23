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
 * \file history.h
 * \brief content lister - eReader history handling                  
 * 
 * <File description> - store the last opened item for every category in memory
 * 
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */


#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <glib.h>

#include <liberregxml/erregapi.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "stack.h"
#include "history.h"
#include "programManager.h"
#include "regValue.h"
#include "metadataStoreManager.h"
#include "system.h"

#define NO_VIEWER_ACTIVE    ""

// keep track of the last opened item for every archive key content Type;
static clDisplayItem_t* g_history[ARCHIVE_TYPE_COUNT];  // = { NULL, ... } but this is implicit for static data
static char*            g_location[TOTAL_STACK_COUNT];  // = { NULL, ... } but this is implicit for static data
static char             g_activeViewer[ARCHIVE_TYPE_COUNT][UAID_MAX_SIZE];

//internally used routines
static clDisplayItem_t *hsCreateItem(regLastRead_t * content);
static void hsCreateConvertItem(regLastRead_t * content, clDisplayItem_t * item);
static void hsAddRecent(const clDisplayItem_t * item);

void hsInit(void)
{
    int  i;
    char buf[ERMDS_MAX_FILENAME_SIZE];
    regLastRead_t *theBooks = NULL;
    regLastRead_t *theNews  = NULL;
    regLastRead_t *theDocs  = NULL;
    regLastRead_t *theNotes = NULL;

    CL_HISTORYPRINTF("");

    // retrieve the history information from the registry
    theBooks = erRegGetLastRead(BOOKS_CATEGORY);
    theNews = erRegGetLastRead(NEWSPAPERS_CATEGORY);
    theDocs = erRegGetLastRead(DOCUMENTS_CATEGORY);
    theNotes = erRegGetLastRead(NOTES_CATEGORY);
 
    g_history[st_ContentTypeBooks] = hsCreateItem(theBooks);
    if (theBooks)
    {
        CL_HISTORYPRINTF("Found BOOKS_CATEGORY (loc %s)", theBooks->position);
        g_location[st_ContentTypeBooks] = g_strdup(theBooks->position);
        erRegFreeLastRead(theBooks);
    }
    strncpy(g_activeViewer[st_ContentTypeBooks], NO_VIEWER_ACTIVE, UAID_MAX_SIZE);

    g_history[st_ContentTypeNews] = hsCreateItem(theNews);
    if (theNews)
    {
        CL_HISTORYPRINTF("Found NEWSPAPERS_CATEGORY (loc %s)", theNews->position);
        g_location[st_ContentTypeNews] = g_strdup(theNews->position);
        erRegFreeLastRead(theNews);
    }
    strncpy(g_activeViewer[st_ContentTypeNews], NO_VIEWER_ACTIVE, UAID_MAX_SIZE);

    g_history[st_ContentTypeDocs] = hsCreateItem(theDocs);
    if (theDocs)
    {
        CL_HISTORYPRINTF("Found DOCUMENTS_CATEGORY (loc %s)", theDocs->position);
        g_location[st_ContentTypeDocs] = g_strdup(theDocs->position);
        erRegFreeLastRead(theDocs);
    }
    strncpy(g_activeViewer[st_ContentTypeDocs], NO_VIEWER_ACTIVE, UAID_MAX_SIZE);

    g_history[st_ContentTypeNotes] = hsCreateItem(theNotes);
    if (theNotes)
    {
        CL_HISTORYPRINTF("Found NOTES_CATEGORY (loc %s)", theNotes->position);
        g_location[st_ContentTypeNotes] = g_strdup(theNotes->position);
        erRegFreeLastRead(theNotes);
    }
    strncpy(g_activeViewer[st_ContentTypeNotes], NO_VIEWER_ACTIVE, UAID_MAX_SIZE);

    // retrieve the recent-documents information from the registry
    i = mdsGetRootLocation(st_RecentDocuments, buf, sizeof(buf));
    if (i > 0)
    {
        g_location[st_RecentDocuments] = g_strdup(buf);
    }
    else
    {
        CL_ERRORPRINTF("mdsGetRootLocation(st_RecentDocuments) returns [%d]", i);
    }
}

// Always overwrite the previous location/position entry in the 
// registry - use empty strings when needed
void hsDestroy(void)
{
    int     index;
    regLastRead_t *theLastRead = NULL;

    CL_HISTORYPRINTF("");

    //store the history information in the registry
    prepare_registry_write();
    CL_HISTORYPRINTF("Checked and loaded registry");

    CL_HISTORYPRINTF("Store BOOKS_CATEGORY");
    theLastRead = g_new0(regLastRead_t, 1);
    hsCreateConvertItem(theLastRead, g_history[st_ContentTypeBooks]);
    // avoid writing NULL to registry
    if (g_location[st_ContentTypeBooks])
    {
        theLastRead->position = g_strdup(g_location[st_ContentTypeBooks]);
    }
    else
    {
        theLastRead->position = g_strdup("");
    }
    erRegSetLastRead(BOOKS_CATEGORY, theLastRead);
    erRegFreeLastRead(theLastRead);

    CL_HISTORYPRINTF("Store NEWSPAPERS_CATEGORY");
    theLastRead = g_new0(regLastRead_t, 1);
    hsCreateConvertItem(theLastRead, g_history[st_ContentTypeNews]);
    // avoid writing NULL to registry
    if (g_location[st_ContentTypeNews])
    {
        theLastRead->position = g_strdup(g_location[st_ContentTypeNews]);
    }
    else
    {
        theLastRead->position = g_strdup("");
    }
    CL_HISTORYPRINTF("position %s", theLastRead->position);
    erRegSetLastRead(NEWSPAPERS_CATEGORY, theLastRead);
    erRegFreeLastRead(theLastRead);

    CL_HISTORYPRINTF("Store DOCUMENTS_CATEGORY");
    theLastRead = g_new0(regLastRead_t, 1);
    hsCreateConvertItem(theLastRead, g_history[st_ContentTypeDocs]);
    CL_HISTORYPRINTF("position %s", theLastRead->position);
    // avoid writing NULL to registry
    if (g_location[st_ContentTypeDocs])
    {
        theLastRead->position = g_strdup(g_location[st_ContentTypeDocs]);
    }
    else
    {
        theLastRead->position = g_strdup("");
    }
    CL_HISTORYPRINTF("position %s", theLastRead->position);
    erRegSetLastRead(DOCUMENTS_CATEGORY, theLastRead);
    erRegFreeLastRead(theLastRead);

    CL_HISTORYPRINTF("Store NOTES_CATEGORY");
    theLastRead = g_new0(regLastRead_t, 1);
    hsCreateConvertItem(theLastRead, g_history[st_ContentTypeNotes]);
    // avoid writing NULL to registry
    if (g_location[st_ContentTypeNotes])
    {
        theLastRead->position = g_strdup(g_location[st_ContentTypeNotes]);
    }
    else
    {
        theLastRead->position = g_strdup("");
    }
    CL_HISTORYPRINTF("position %s", theLastRead->position);
    erRegSetLastRead(NOTES_CATEGORY, theLastRead);
    erRegFreeLastRead(theLastRead);

    do_registry_write();
    CL_HISTORYPRINTF("Stored registry");


    CL_HISTORYPRINTF("Free resources");

    for (index = 0; index < ARCHIVE_TYPE_COUNT; index++)
    {
        if (g_history[index])
        {
            g_free(g_history[index]);
            g_history[index] = NULL;
        }

        if (g_location[index])
        {
            g_free(g_location[index]);
            g_location[index] = NULL;
        }
    }
}

void hsStoreItem(st_ContentType_e contentType, clDisplayItem_t * item)
{
    CL_HISTORYPRINTF("contentType %d item 0x%x", contentType, (unsigned int) item);

    if ((item) && (stackIsArchive(contentType)))
    {
        memcpy(g_history[contentType], item, sizeof(clDisplayItem_t));
    }

    if (item)
    {
        if (    item->fit == mdsFitFile
            &&  strcasecmp(item->szFileExt, "png") == 0 )
        {
            // Notes template: do not add to recent documents
            CL_WARNPRINTF("Notes template: file [%s] not added to recent documents", item->szFilename);
        }
        else
        {
            hsAddRecent(item);
        }
    }
}

void hsGetItem(st_ContentType_e contentType, clDisplayItem_t * item)
{
    CL_HISTORYPRINTF("contentType %d item 0x%x", contentType, (unsigned int) item);

    if ((item) && (stackIsArchive(contentType)))
    {
        memcpy(item, g_history[contentType], sizeof(clDisplayItem_t));
    }
}

gboolean hsSetLocation(st_ContentType_e contentType, char *location)
{
    CL_HISTORYPRINTF("type %d - %s", contentType, location);

    if (stackIsArchive(contentType))
    {
        if (g_location[contentType])
        {
            g_free(g_location[contentType]);
            g_location[contentType] = NULL;
        }

        g_location[contentType] = g_strdup(location);
    }

    return TRUE;
}

char *hsGetLocation(st_ContentType_e contentType)
{
    char   *theLocation = NULL;

    CL_HISTORYPRINTF("type %d", contentType);

    if (stackIsArchive(contentType))
    {
        theLocation = g_strdup(g_location[contentType]);
    }
    return theLocation;
}

gboolean hsRemoveLocation(st_ContentType_e contentType)
{
    gboolean returnValue = FALSE;

    if (stackIsArchive(contentType))
    {
        if (g_location[contentType])
        {
            g_free(g_location[contentType]);
            g_location[contentType] = NULL;
            returnValue = TRUE;
        }
    }
    return returnValue;
}

void hsSetActiveViewer(st_ContentType_e contentType, char *viewerId)
{
    int     index;

    CL_HISTORYPRINTF("contentType %d viewerId %s", contentType, viewerId);

    for (index = 0; index < ARCHIVE_TYPE_COUNT; index++)
    {
        if (g_activeViewer[index] == viewerId)
        {
            strncpy(g_activeViewer[index], NO_VIEWER_ACTIVE, UAID_MAX_SIZE);
        }
    }

    if (stackIsArchive(contentType))
    {
        strncpy(g_activeViewer[contentType], viewerId, UAID_MAX_SIZE);
    }
}

char   *hsGetActiveViewer(st_ContentType_e contentType)
{
    char   *viewerId = NULL;

    if (stackIsArchive(contentType))
    {
        viewerId = g_activeViewer[contentType];

        CL_HISTORYPRINTF("contentType %d viewerId %s", contentType, viewerId);
    }
    return viewerId;
}

st_ContentType_e hsGetContentType(char *viewerId)
{
    st_ContentType_e theType = st_ContentTypeUndefined;
    int     index;

    CL_HISTORYPRINTF("viewerId %s", viewerId);

    for (index = 0; index < ARCHIVE_TYPE_COUNT; index++)
    {
        if (strncmp(g_activeViewer[index], viewerId, UAID_MAX_SIZE) == 0)
        {
            CL_HISTORYPRINTF("theType %d", index);
            theType = index;
        }
    }
    return theType;
}

static clDisplayItem_t *hsCreateItem(regLastRead_t * content)
{
    clDisplayItem_t *theItem = NULL;

    theItem = g_new0(clDisplayItem_t, 1);
    if ((theItem) && (content))
    {
        g_snprintf(theItem->szFilename, ERMDS_MAX_FILENAME_SIZE, "%s", content->documentPath);
        g_snprintf(theItem->szFileExt,  ERMDS_MAX_FILENAME_SIZE, "%s", content->extension   );
        if (   content->manifestPath    == NULL
            || content->manifestPath[0] == '\0' )
        {
            theItem->fit = mdsFitFile;
            strcpy(theItem->szManifest, ""); 
        }
        else
        {
            theItem->fit = mdsFitContainer;
            g_snprintf(theItem->szManifest, ERMDS_MAX_FILENAME_SIZE, "%s", content->manifestPath);
        }
    }
    return theItem;
}

static void hsCreateConvertItem(regLastRead_t * content, clDisplayItem_t * theItem)
{
    if ((content) && (theItem))
    {
        content->documentPath = g_strdup(theItem->szFilename);
        content->extension = g_strdup(theItem->szFileExt);
        content->manifestPath = g_strdup(theItem->szManifest);
    }
}



// check whether the specified path is a symbolic link
static gboolean isSymLink(const char *szPath)
{
    struct stat statBuf;

    if (   lstat(szPath, &statBuf) == 0
        && S_ISLNK(statBuf.st_mode)    )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


// add item to recent documents list
static void hsAddRecent(const clDisplayItem_t * item)
{
    g_assert(item != NULL);
    CL_HISTORYPRINTF("Entry: filename [%s] fit [%d]", item->szFilename, item->fit);
    g_return_if_fail(item->fit == mdsFitFile  ||  item->fit == mdsFitContainer);

    int      sequence;
    int      err;
    int      n;
    gboolean done;
    char     *cp;
    char     szTargetFile[ERMDS_MAX_FILENAME_SIZE] = "";
    char     szLinkName[ERMDS_MAX_FILENAME_SIZE]   = "";
    char     szLinkTarget[ERMDS_MAX_FILENAME_SIZE] = "";

    DIR           *dirp;
    struct dirent *direntp;
    const char    *linkdir = g_location[st_RecentDocuments];


    // determine target name
    switch (item->fit)
    {
        case mdsFitFile:
            snprintf(szLinkName, sizeof(szLinkName), item->szFilename);
            break;
            
        case mdsFitContainer:
            snprintf(szLinkName, sizeof(szLinkName), item->szManifest);
            // strip filename, use directory name
            cp = strrchr(szLinkName, '/');
            g_return_if_fail(cp != NULL);
            *cp = '\0';
            break;
            
        default:
            break;  // ignore
    }

    // resolve symlink and symlinks in directory path
    if ( realpath(szLinkName, szTargetFile) == NULL )
    {
        CL_ERRORPRINTF("Could not resolve path [%s] error [%s]", szLinkName, strerror(errno));
        return;
    }
    CL_HISTORYPRINTF("target [%s]", szTargetFile);

    // remove existing link(s) to the target, if any
    if ((dirp = opendir(linkdir)) == NULL)
    {
        CL_ERRORPRINTF("Could not open directory [%s] error [%s].", linkdir, strerror(errno));
    }
    else
    {
        while ((direntp = readdir(dirp)) != NULL)
        {
            // directory entry read
            snprintf(szLinkName, sizeof(szLinkName), "%s/%s", linkdir, direntp->d_name);
            if ( isSymLink(szLinkName) )
            {
                // directory entry is a symlink
                // Note: The code below removes existing symlinks to the new target,
                //       but also symlinks to its parent directory or higher ancestor directories.
                //       This behaviour is unintentional, but serves is good in case a symlink
                //       points to a multi-language document (see tracker 962) while a specific
                //       version of this document is opened.
                //       Example:
                //         symlnk exists to          /mnt/free/documents/Reference/usermanual
                //         new symlink must point to /mnt/free/documents/Reference/usermanual/en_US
                n = readlink(szLinkName, szLinkTarget, sizeof(szLinkTarget) - 1);
                if (n > 0  &&  strncmp(szLinkTarget, szTargetFile, n) == 0)
                {
                    // symlink points to our target: delete symlink
                    szLinkTarget[n] = '\0';
                    CL_HISTORYPRINTF("Remove symlink [%s] -> [%s]", szLinkName, szLinkTarget);
                    unlink(szLinkName);
                }
            }
        }
        closedir(dirp);
    }
    
    // create a new symlink to the target
    done = FALSE;
    for ( sequence = 0 ; !done  &&  sequence <= 99 ; sequence++ )
    {
        // determine name of symlink
        time_t     t_time  = time(NULL);
        struct tm* tm_time = localtime( &t_time );
        snprintf( szLinkName,
                  ERMDS_MAX_FILENAME_SIZE,
                  "%s/Link_%04d-%02d-%02dT%02d:%02d:%02d:%02d",
                   linkdir,
                           tm_time->tm_year + 1900,
                                tm_time->tm_mon + 1,
                                     tm_time->tm_mday,
                                          tm_time->tm_hour,
                                               tm_time->tm_min,
                                                    tm_time->tm_sec,
                                                         sequence   );

        // create symbolic link, replace existing link when present
        if (symlink(szTargetFile, szLinkName) == 0)
        {
            CL_HISTORYPRINTF("Created symlink [%s] -> [%s]", szLinkName, szTargetFile);
            done = TRUE;
        }
        else
        {
            err = errno;
            CL_ERRORPRINTF("Error [%s] on create symlink [%s] -> [%s]", strerror(err), szLinkName, szTargetFile);
            if (err == EEXIST)
            {
                CL_ERRORPRINTF("[%s] already exists", szLinkName);
            }
        }
    }

    // limit the number of symlinks
    hsLimitSymlinks(linkdir, MAX_RECENT_ITEMS);
}


// remove symlinks for which the target is not present
void hsCleanupSymlinks(const char* szLinkDir)
{
    g_assert(szLinkDir != NULL);
    CL_HISTORYPRINTF("Entry: directory [%s]", szLinkDir);
    
    DIR*   dirp;
    struct dirent* direntp;
    struct stat    statBuf;
    
    char   szName[ERMDS_MAX_FILENAME_SIZE];


    if ((dirp = opendir(szLinkDir)) == NULL)
    {
        CL_ERRORPRINTF("Could not open directory %s (error %s).", szLinkDir, strerror(errno));
        return;
    }
    else
    {
        while ((direntp = readdir(dirp)) != NULL)
        {
            if (   strcmp(".",  direntp->d_name) == 0
                || strcmp("..", direntp->d_name) == 0)
            {
                // skip this entry
                continue;
            }
            
            snprintf(szName, ERMDS_MAX_FILENAME_SIZE, "%s/%s", szLinkDir, direntp->d_name);
            if (   isSymLink(szName)
                && stat(szName, &statBuf) == -1 )
            {
                // directory entry is a symlink, but no file/directory behind it: remove symlink
                CL_HISTORYPRINTF("Remove symlink [%s]", szName);
                unlink(szName);
            }
        }

        closedir(dirp);
    }
}


// if more than maxItem symlinks, then remove the oldest ones.
void hsLimitSymlinks(const char* szLinkDir, int maxItems)
{
    int    i;
    DIR*   dirp;
    struct dirent* direntp;

    typedef struct
    {
        char    szName[ERMDS_MAX_FILENAME_SIZE];
    } linkInfo_t;

    linkInfo_t* linkTable = alloca( sizeof(linkInfo_t) * (maxItems + 1) );
    linkInfo_t* linkOldest  = NULL;
    linkInfo_t* linkCurrent = &linkTable[0];
    int         linkNum = 0;

    g_return_if_fail(linkTable != NULL);

    if ((dirp = opendir(szLinkDir)) == NULL)
    {
        CL_ERRORPRINTF("Could not open directory %s (error %s).", szLinkDir, strerror(errno));
        return;
    }
    else
    {
        while ((direntp = readdir(dirp)) != NULL)
        {
            if (   strcmp(".",  direntp->d_name) == 0
                || strcmp("..", direntp->d_name) == 0)
            {
                // skip this entry
                continue;
            }
            
            linkCurrent = &linkTable[linkNum];
            snprintf(linkCurrent->szName, ERMDS_MAX_FILENAME_SIZE, "%s/%s", szLinkDir, direntp->d_name);

            if (linkNum < maxItems)
            {
                // table not full: add this item
                linkNum++;
                if (   linkOldest == NULL
                    || strcmp(linkCurrent->szName, linkOldest->szName) < 0)
                {
                    linkOldest = linkCurrent;
                }
            }
            else if (strcmp(linkCurrent->szName, linkOldest->szName) < 0)
            {
                // table full and item older than oldest: remove current item
                CL_HISTORYPRINTF("Remove symlink [%s]", linkCurrent->szName);
                delete_fsitem(linkCurrent->szName);
            }
            else
            {
                // table full and item newer than oldest: replace oldest item
                CL_HISTORYPRINTF("Remove symlink [%s]", linkOldest->szName);
                delete_fsitem(linkOldest->szName);
                *linkOldest = *linkCurrent;

                // find the oldest item now in the list
                for ( i = 0,      linkCurrent = &linkTable[0] ;
                      i < linkNum ;
                      i++,        linkCurrent++ )
                {
                    if (strcmp(linkCurrent->szName, linkOldest->szName) < 0)
                    {
                        linkOldest = linkCurrent;
                    }
                }
            }
        }// while
        
        closedir(dirp);
    }
}

