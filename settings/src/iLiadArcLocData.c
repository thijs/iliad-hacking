/*
 * This file is part of settings.
 *
 * settings is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * settings is distributed in the hope that it will be useful,
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
 * \file iLiadArcLocData.c
 * \brief settings - Archive Location Data
 *
 * Providing archive location data. 
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>

#include "setupLog.h"
#include "iLiadArcLocData.h"
#include "settings.h"


static arcLocData_t *g_cur_settings = NULL;
static arcLocData_t *g_stored_settings = NULL;

static regExportMemType_t *g_cur_export_settings = NULL;
static regExportMemType_t *g_stored_export_settings = NULL;

// inited them at the begining, just for reference later
static arcPath_t   *g_arc_paths=NULL;   
static mediaPath_t *g_media_paths=NULL;

static mediaPath_t *getMediaPaths(void);
static arcPath_t *getArcPaths(void);

static void freeMediaPaths(mediaPath_t* mediaPaths);
static void freeArcPaths(arcPath_t* arcPaths);

static regExportMemType_t *export_settings_dup(regExportMemType_t* theExportMemType);
static arcLocData_t *arcLocDataDup(const arcLocData_t * settings);

static void arcPathToArcLocData(arcPath_t* arcPaths, arcLocData_t* arcLocData);
static void arcLocDataToArcPath(arcLocData_t* arcLocData, arcPath_t* arcPaths);

static void path2data(const char* path, arcLocType_e* data, mediaPath_t* mediaPaths);
static char* data2path(const arcType_e arc, const arcLocType_e data, mediaPath_t* mediaPaths, arcPath_t* arcPaths);

void iLiad_archive_location_data_init(void)
{
    ST_LOGPRINTF("entry");

    iLiad_archive_location_data_destroy();

    g_cur_settings = g_new0(arcLocData_t, 1);
    if (g_cur_settings != NULL)
    {
        g_arc_paths = getArcPaths();
        g_media_paths = getMediaPaths();
        
        arcPathToArcLocData(g_arc_paths, g_cur_settings);
        g_stored_settings = arcLocDataDup(g_cur_settings);
    }
    else
    {
        ST_ERRORPRINTF("memory allocation error");
    }

    g_cur_export_settings = erRegGetExportMemType();
    if (g_cur_export_settings == NULL)
    {
        g_cur_export_settings = g_new0(regExportMemType_t, 1);
        g_assert(NULL != g_cur_export_settings);

        // Use internal memory as default
        g_cur_export_settings->location = g_strdup(MAIN_CATEGORY);
    }

    g_stored_export_settings = export_settings_dup(g_cur_export_settings);
}

void iLiad_archive_location_data_destroy(void)
{
    ST_LOGPRINTF("entry");

    if (g_cur_settings)
    {
        g_free(g_cur_settings);
        g_cur_settings = NULL;
    }

    if (g_stored_settings)
    {
        g_free(g_stored_settings);
        g_stored_settings = NULL;
    }

    if (g_arc_paths)
    {
        freeArcPaths(g_arc_paths);
        g_arc_paths = NULL;
    }

    if (g_media_paths)
    {
        freeMediaPaths(g_media_paths);
        g_media_paths = NULL;
    }

    if (g_cur_export_settings)
    {
        erRegFreeExportMemType(g_cur_export_settings);
        g_cur_export_settings = NULL;
    }

    if (g_stored_export_settings)
    {
        erRegFreeExportMemType(g_stored_export_settings);
        g_stored_export_settings = NULL;
    }
}

void iLiad_archive_location_data_store(void)
{
    gboolean bSaveNews=FALSE, bSaveBooks=FALSE, bSaveDocs=FALSE, bSaveNotes=FALSE, bSaveExport=FALSE;

    ST_LOGPRINTF("entry");
    
    g_return_if_fail(NULL != g_cur_settings);
    g_return_if_fail(NULL != g_stored_settings);
   
    // need to save changes or not? 
    if (g_cur_settings->newsLoc != g_stored_settings->newsLoc)
    {
        bSaveNews = TRUE;
    }
   
    if (g_cur_settings->booksLoc != g_stored_settings->booksLoc)
    {
        bSaveBooks = TRUE;
    }

    if (g_cur_settings->docsLoc != g_stored_settings->docsLoc)
    {
        bSaveDocs = TRUE;
    }

    if (g_cur_settings->notesLoc != g_stored_settings->notesLoc)
    {
        bSaveNotes = TRUE;
    }

    if (strcmp(g_cur_export_settings->location, g_stored_export_settings->location) != 0)
    {
        bSaveExport = TRUE;
    }
    
    if (bSaveNews || bSaveBooks || bSaveDocs || bSaveNotes || bSaveExport)
    {
        ST_LOGPRINTF("entry %d %d %d %d %d", bSaveNews, bSaveBooks, bSaveDocs, bSaveNotes, bSaveExport);
        
        // save changes to registry
        prepare_registry_write();

        regContentCategory_t* category = NULL;
        arcPath_t* arcPaths= g_new0(arcPath_t, 1);
        arcLocDataToArcPath(g_cur_settings, arcPaths);
        
        if (bSaveNews)
        {
            // save the location of newspapers
            category = erRegGetContentCategory(NEWSPAPERS_CATEGORY);
            if (category && category->location)
            {
                g_free(category->location);
                category->location = g_strdup(arcPaths->newsPath);
                erRegSetContentCategory(NEWSPAPERS_CATEGORY, category);
            }
            erRegFreeContentCategory(category);
            category = NULL;
        }

        if (bSaveBooks)
        {
            // save the location of books
            category = erRegGetContentCategory(BOOKS_CATEGORY);
            if (category && category->location)
            {
                g_free(category->location);
                category->location = g_strdup(arcPaths->booksPath);
                erRegSetContentCategory(BOOKS_CATEGORY, category);
            }
            erRegFreeContentCategory(category);
            category = NULL;
        }

        if (bSaveDocs)
        {
            // save the location of docs
            category = erRegGetContentCategory(DOCUMENTS_CATEGORY);
            if (category && category->location)
            {
                g_free(category->location);
                category->location = g_strdup(arcPaths->docsPath);
                erRegSetContentCategory(DOCUMENTS_CATEGORY, category);
            }
            erRegFreeContentCategory(category);
            category = NULL;
        }

        if (bSaveNotes)
        {
            // save the location of notes
            category = erRegGetContentCategory(NOTES_CATEGORY);
            if (category && category->location)
            {
                g_free(category->location);
                category->location = g_strdup(arcPaths->notesPath);
                erRegSetContentCategory(NOTES_CATEGORY, category);
            }
            erRegFreeContentCategory(category);
            category = NULL;
        }

        if (bSaveExport)
        {
            if (!erRegSetExportMemType(g_cur_export_settings))
            {
                ST_ERRORPRINTF("Error storing export settings");
            }
            else
            {
                // success
                erRegFreeExportMemType(g_stored_export_settings);
                g_stored_export_settings = export_settings_dup(g_cur_export_settings);
            }
        }
       
        // free the memory 
        freeArcPaths(arcPaths);
        arcPaths = NULL; 
      
        // also update the g_stored_settings
        g_free(g_stored_settings);
        g_stored_settings = arcLocDataDup(g_cur_settings);

        do_registry_write();
    }
}

void iLiad_export_location_data_set(arcLocType_e location)
{
    ST_LOGPRINTF("Set export memory types: location=%d", location);

    g_return_if_fail(g_cur_export_settings != NULL);

    if ((location < arcLocMain) || (location >= arcLocUndefined))
    {
        ST_WARNPRINTF("invalid location value and use the default value(%d)", arcLocMain);
        location = arcLocMain;
    }

    g_free(g_cur_export_settings->location);
    switch (location)
    {
        case arcLocMain:
            g_cur_export_settings->location = g_strdup(MAIN_CATEGORY);
            break;
        case arcLocCard:
            g_cur_export_settings->location = g_strdup(SD_CATEGORY);
            break;
        case arcLocUSB:
            g_cur_export_settings->location = g_strdup(USB_CATEGORY);
            break;
        case arcLocCF:
            g_cur_export_settings->location = g_strdup(CF_CATEGORY);
            break;
        default:
            g_assert_not_reached();
            break;
    }
}

void iLiad_archive_location_data_set(arcType_e arc, arcLocType_e location)
{
    ST_LOGPRINTF("arc=%d, location=%d", arc, location);

    g_return_if_fail(g_cur_settings != NULL);

    if ((location < arcLocMain) || (location >= arcLocUndefined))
    {
        ST_WARNPRINTF("invalid location value and use the default value(%d)", arcLocMain);
        location = arcLocMain;
    }

    switch (arc)
    {
        case arcNews:
            g_cur_settings->newsLoc = location;
            break;
        case arcBooks:
            g_cur_settings->booksLoc = location;
            break;
        case arcDocs:
            g_cur_settings->docsLoc = location;
            break;
        case arcNotes:
            g_cur_settings->notesLoc = location;
            break;
        default:
            ST_ERRORPRINTF("error:arc=%d", arc);
    }
}

arcLocType_e iLiad_archive_location_data_get(arcType_e arc)
{
    ST_LOGPRINTF("arc=%d", arc);

    arcLocType_e location = arcLocMain;

    if (g_cur_settings)
    {
        switch (arc)
        {
            case arcNews:
                location = g_cur_settings->newsLoc;
                break;
            case arcBooks:
                location = g_cur_settings->booksLoc;
                break;
            case arcDocs:
                location = g_cur_settings->docsLoc;
                break;
            case arcNotes:
                location = g_cur_settings->notesLoc;
                break;
            default:
                ST_ERRORPRINTF("error:arc=%d", arc);
        }
    }
    else
    {
        ST_WARNPRINTF("g_cur_settings == NULL");
    }

    return location;
}

arcLocType_e iLiad_export_location_data_get()
{
    ST_LOGPRINTF("entry");

    if (strcmp(g_cur_export_settings->location, MAIN_CATEGORY) == 0)
    {
        return arcLocMain;
    }
    else if (strcmp(g_cur_export_settings->location, SD_CATEGORY) == 0)
    {
        return arcLocCard;
    }
    else if (strcmp(g_cur_export_settings->location, USB_CATEGORY) == 0)
    {
        return arcLocUSB;
    }
    else if (strcmp(g_cur_export_settings->location, CF_CATEGORY) == 0)
    {
        return arcLocCF;
    }
    else
    {
        ST_ERRORPRINTF("Invalid location: %s", g_cur_export_settings->location);

        // Use internal memory as default
        return arcLocMain;
    }
}

static regExportMemType_t *export_settings_dup(regExportMemType_t *theExportMemType)
{
    ST_LOGPRINTF("entry");

    regExportMemType_t *dup = NULL;

    if (theExportMemType)
    {
        dup = g_new0(regExportMemType_t, 1);
        g_assert(NULL != dup);

        dup->location = g_strdup(theExportMemType->location);
    }

    return dup;
}

static arcLocData_t *arcLocDataDup(const arcLocData_t * settings)
{
    ST_LOGPRINTF("entry");

    arcLocData_t *dup = NULL;

    if (settings)
    {
        dup = g_new0(arcLocData_t, 1);
        if (dup)
        {
            dup->newsLoc = settings->newsLoc;
            dup->booksLoc = settings->booksLoc;
            dup->docsLoc = settings->docsLoc;
            dup->notesLoc = settings->notesLoc;
        }
    }
    return dup;
}

static mediaPath_t *getMediaPaths(void)
{
    ST_LOGPRINTF("entry");

    mediaPath_t *media = NULL;

    media = g_new0(mediaPath_t, 1);
    if (media)
    {
        regContentCategory_t *category = NULL;
        // get main memory path
        category = erRegGetContentCategory(MAIN_CATEGORY);
        if (category && category->location)
        {
            media->mainPath = g_strdup(category->location);
            erRegFreeContentCategory(category);
        }
        else
        {
            ST_ERRORPRINTF("error occurred when reading MAIN_CATEGORY from registry");
        }
        // get sd/mmc path 
        category = erRegGetContentCategory(SD_CATEGORY);
        if (category && category->location)
        {
            media->cardPath = g_strdup(category->location);
            erRegFreeContentCategory(category);
        }
        else
        {
            ST_ERRORPRINTF("error occurred when reading SD_CATEGORY from registry");
        }
        // get usb path
        category = erRegGetContentCategory(USB_CATEGORY);
        if (category && category->location)
        {
            media->usbPath = g_strdup(category->location);
            erRegFreeContentCategory(category);
        }
        else
        {
            ST_ERRORPRINTF("error occurred when reading USB_CATEGORY from registry");
        }
        // get cf path
        category = erRegGetContentCategory(CF_CATEGORY);
        if (category && category->location)
        {
            media->cfPath = g_strdup(category->location);
            erRegFreeContentCategory(category);
        }
        else
        {
            ST_ERRORPRINTF("error occurred when reading CF_CATEGORY from registry");
        }
    }
    else
    {
        ST_ERRORPRINTF("memory allocation error");
    }

    return media;
}

static void freeMediaPaths(mediaPath_t* mediaPaths)
{
    ST_LOGPRINTF("entry");

    if (mediaPaths)
    {    
        if (mediaPaths->mainPath)
        {
            g_free(mediaPaths->mainPath);
        }
        if (mediaPaths->cfPath)
        {
            g_free(mediaPaths->cfPath);
        }
        if (mediaPaths->cardPath)
        {
            g_free(mediaPaths->cardPath);
        }
        if (mediaPaths->usbPath)
        {
            g_free(mediaPaths->usbPath);
        }
        g_free(mediaPaths);
    }
}

static arcPath_t *getArcPaths(void)
{
    ST_LOGPRINTF("entry");

    arcPath_t *arc = NULL;

    arc = g_new0(arcPath_t, 1);
    if (arc)
    {
        regContentCategory_t *category = NULL;
        
        // get the path of the archive arc of newspaper
        category = erRegGetContentCategory(NEWSPAPERS_CATEGORY);
        if (category && category->location)
        {
            arc->newsPath = g_strdup(category->location);
            erRegFreeContentCategory(category);
        }
        else
        {
            ST_ERRORPRINTF("error occurred when reading NEWSPAPERS_CATEGORY from registry");
        }
        // get the path of the archive arc of books
        category = erRegGetContentCategory(BOOKS_CATEGORY);
        if (category && category->location)
        {
            arc->booksPath = g_strdup(category->location);
            erRegFreeContentCategory(category);
        }
        else
        {
            ST_ERRORPRINTF("error occurred when reading BOOKS_CATEGORY from registry");
        }
        // get the path of the archive arc of documents
        category = erRegGetContentCategory(DOCUMENTS_CATEGORY);
        if (category && category->location)
        {
            arc->docsPath = g_strdup(category->location);
            erRegFreeContentCategory(category);
        }
        else
        {
            ST_ERRORPRINTF("error occurred when reading DOCUMENTS_CATEGORY from registry");
        }
        // get the path of the archive arc of notes
        category = erRegGetContentCategory(NOTES_CATEGORY);
        if (category && category->location)
        {
            arc->notesPath = g_strdup(category->location);
            erRegFreeContentCategory(category);
        }
        else
        {
            ST_ERRORPRINTF("error occurred when reading NOTES_CATEGORY from registry");
        }
    }
    else
    {
        ST_ERRORPRINTF("memory allocation error");
    }
    return arc;
}

static void freeArcPaths(arcPath_t* arcPaths)
{
    ST_LOGPRINTF("entry");

    if (arcPaths)
    {
        if (arcPaths->newsPath)
        {
            g_free(arcPaths->newsPath);
        }
        if (arcPaths->booksPath)
        {
            g_free(arcPaths->booksPath);
        }
        if (arcPaths->docsPath)
        {
            g_free(arcPaths->docsPath);
        }
        if (arcPaths->notesPath)
        {
            g_free(arcPaths->notesPath);
        }
        g_free(arcPaths);
        arcPaths = NULL;
    }
}

// arcPath_t-->arcLocData_t
static void arcPathToArcLocData(arcPath_t* arcPaths, arcLocData_t* arcLocData)
{
    ST_LOGPRINTF("entry");

    if (arcPaths && arcLocData)
    {
        // set the default values
        arcLocData->newsLoc = arcLocMain;
        arcLocData->booksLoc = arcLocMain;
        arcLocData->docsLoc = arcLocMain;
        arcLocData->notesLoc = arcLocMain;

        mediaPath_t *mediaPaths = g_media_paths;
        if (mediaPaths)
        {
            // arcLocData->newsLoc
            path2data(arcPaths->newsPath, &(arcLocData->newsLoc), mediaPaths);

            // arcLocData->booksLoc
            path2data(arcPaths->booksPath, &(arcLocData->booksLoc), mediaPaths);
            
            // arcLocData->docsLoc
            path2data(arcPaths->docsPath, &(arcLocData->docsLoc), mediaPaths);

            // arcLocData->notesLoc
            path2data(arcPaths->notesPath, &(arcLocData->notesLoc), mediaPaths);
        }
    }
}

// arcLocData_t-->arcPath_t
static void arcLocDataToArcPath(arcLocData_t* arcLocData, arcPath_t* arcPaths)
{
    ST_LOGPRINTF("entry %p, %p", arcLocData, arcPaths);

    if (arcLocData && arcPaths)
    {
        mediaPath_t *mediaPaths = g_media_paths;
        arcPath_t* archivePaths = g_arc_paths;

        if (mediaPaths && archivePaths)
        {
            // arcPaths->newsPath
            g_free(arcPaths->newsPath);
            arcPaths->newsPath= NULL;
            arcPaths->newsPath = data2path(arcNews, arcLocData->newsLoc, mediaPaths, archivePaths);

            // arcPaths->booksPath
            g_free(arcPaths->booksPath);
            arcPaths->booksPath= NULL;
            arcPaths->booksPath = data2path(arcBooks, arcLocData->booksLoc, mediaPaths, archivePaths);

            // arcPaths->docsPath
            g_free(arcPaths->docsPath);
            arcPaths->docsPath= NULL;
            arcPaths->docsPath = data2path(arcDocs, arcLocData->docsLoc, mediaPaths, archivePaths);
           
            // arcPaths->notesPath
            g_free(arcPaths->notesPath);
            arcPaths->notesPath= NULL;
            arcPaths->notesPath = data2path(arcNotes, arcLocData->notesLoc, mediaPaths, archivePaths);
        }
    }
}

static void path2data(const char* path, arcLocType_e* data, mediaPath_t* mediaPaths)
{
    if (path && data && mediaPaths)
    {
        if (mediaPaths->mainPath && strstr(path, mediaPaths->mainPath))
        {
            *data = arcLocMain;
        }
        else if (mediaPaths->cardPath && strstr(path, mediaPaths->cardPath))
        {
            *data = arcLocCard;
        }
        else if (mediaPaths->cfPath && strstr(path, mediaPaths->cfPath))
        {
            *data = arcLocCF;
        }
        else if (mediaPaths->usbPath && strstr(path, mediaPaths->usbPath))
        {
            *data = arcLocUSB;
        }
        else
        {
            *data = arcLocMain;
        }
    }
}

static char* data2path(const arcType_e arc, const arcLocType_e data, 
                       mediaPath_t* mediaPaths, arcPath_t* arcPaths)
{
    char* path = NULL;

    if (mediaPaths && arcPaths)
    {
        char* subdir = NULL;
        switch (arc)   
        {
            case arcNews:
                subdir = strrchr(arcPaths->newsPath, '/');
                break;
            case arcBooks:
                subdir = strrchr(arcPaths->booksPath, '/');
                break;
            case arcDocs:
                subdir = strrchr(arcPaths->docsPath, '/');
                break;
            case arcNotes:
                subdir = strrchr(arcPaths->notesPath, '/');
                break;
            default:
                break;
        }
        
        if (mediaPaths->mainPath && (data == arcLocMain))
        {
            path = g_strdup_printf("%s%s", mediaPaths->mainPath, subdir);
        }
        else if (mediaPaths->cardPath && (data == arcLocCard))
        {
            path = g_strdup_printf("%s%s", mediaPaths->cardPath, subdir);
        }
        else if (mediaPaths->cfPath && (data == arcLocCF))
        {
            path = g_strdup_printf("%s%s", mediaPaths->cfPath, subdir);
        }
        else if (mediaPaths->usbPath && (data == arcLocUSB))
        {
            path = g_strdup_printf("%s%s", mediaPaths->usbPath, subdir);
        }
        else
        {
            path = g_strdup_printf("%s%s", mediaPaths->mainPath, subdir);
        }
    }

    return path;
}
