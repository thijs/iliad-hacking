/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file selectionFilehandler.cpp
 * \brief browser - selection file handling routines
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#include <stdio.h>
#include <glib.h>
#include <glib/gstdio.h>
#include "browserTypes.h"
#include "selection.h"
#include "selectionFileHandler.h"
#include "browserLog.h"

#ifdef SELECTION_ENABLED

// internally use routines
static int selection_file_get_name(GtkMozEmbed * b, char *name, unsigned int length);

// check if there is a selection file associated with this HTML page
// and destroy it
bool selection_file_destroy(GtkMozEmbed * b)
{
    char    filename[MAX_FILE_NAME];

    if (selection_file_get_name(GTK_MOZ_EMBED(b), filename, MAX_FILE_NAME) == 0)
    {
        BR_LOGPRINTF("name %s", filename);

        if (g_file_test(filename, G_FILE_TEST_EXISTS))
        {
            // using g_remove results in ccmopile error
            if (remove(filename) == 0)
            {
                return TRUE;
            }
            else
            {
                BR_ERRORPRINTF("remove(%s) != 0", filename);
            }
        }
    }
    return FALSE;
}

// check if there is a selection file associated with this HTML page
bool selection_file_available(GtkMozEmbed * b)
{
    char    filename[MAX_FILE_NAME];

    if (selection_file_get_name(GTK_MOZ_EMBED(b), filename, MAX_FILE_NAME) == 0)
    {
        return g_file_test(filename, G_FILE_TEST_EXISTS);
    }
    return FALSE;
}

// Generate filename given the URL of the currently viewed URL
// The memory allocated to store this name should be allocated by the calling routing
// "length" is curently not used, working with fixed max length MAX_FILE_NAME
static int selection_file_get_name(GtkMozEmbed * b, char *name, unsigned int length)
{
    char   *url = NULL;
    char   *ptr_1 = NULL;
    char   *ptr_2 = NULL;
    char    filename[MAX_FILE_NAME];

    //retrieve the URL of the HTML page
    url = gtk_moz_embed_get_location(GTK_MOZ_EMBED(b));

    if (url)
    {
        BR_LOGPRINTF("url %s", url);

        ptr_1 = strchr(url, ':');
        ptr_2 = strrchr(url, '.');

        // both ':' and '.' should appear in the url, 
        // the html filename lenght (ptr_2 - ptr_1) should fit in the 
        // filename (- extension size)

        if (ptr_1 && ptr_2 && ((ptr_2 - ptr_1) < MAX_FILE_NAME - strlen("sel")))
        {
            ptr_1 = ptr_1 + 3;  // don't include ://
            strcpy(filename, ptr_1);
            ptr_2 = strrchr(filename, '.');
            strcpy(++ptr_2, "sel");
            BR_LOGPRINTF("file name %s", filename);

            if (strlen(filename) < length)
            {
                strcpy(name, filename);
                BR_LOGPRINTF("returning %s", name);
                return 0;
            }
        }
        else
        {
            BR_ERRORPRINTF("file name problem");
        }

        g_free(url);
    }
    return 1;
}

// open or re-open a selection file associated with this HTML page
FILE   *selection_file_init(GtkMozEmbed * b)
{
    char    filename[MAX_FILE_NAME];
    FILE   *selectionFile;

    if (selection_file_get_name(GTK_MOZ_EMBED(b), filename, MAX_FILE_NAME) == 0)
    {
        BR_LOGPRINTF("file %s", filename);

        // see if this file already exists
        selectionFile = fopen(filename, "a+");

        if (selectionFile)
        {
            BR_LOGPRINTF("opened file 0x%x", (unsigned int) selectionFile);
            return selectionFile;
        }
        else
        {
            BR_ERRORPRINTF("can not open/create selection file %s", filename);
        }
    }
    return NULL;
}

// store a new selection in the selection file
bool selection_file_store(FILE * selectionFile, SelectionID * theSelection)
{
    if (selectionFile && theSelection)
    {
        fprintf(selectionFile, "selection\n");
        fprintf(selectionFile, "parentNodeTagName= %s\n", theSelection->parentNodeTagName);
        fprintf(selectionFile, "parentNodeIndex= %d\n", theSelection->parentNodeIndex);
        fprintf(selectionFile, "nodeIndex= %d\n", theSelection->nodeIndex);
        fprintf(selectionFile, "selectionStart= %d\n", theSelection->selectionStart);
        fprintf(selectionFile, "selectionLength= %d\n", theSelection->selectionLength);

        BR_LOGPRINTF("selection");
        BR_LOGPRINTF("parentNodeTagName= %s", theSelection->parentNodeTagName);
        BR_LOGPRINTF("parentNodeIndex= %d", theSelection->parentNodeIndex);
        BR_LOGPRINTF("nodeIndex= %d", theSelection->nodeIndex);
        BR_LOGPRINTF("selectionStart= %d", theSelection->selectionStart);
        BR_LOGPRINTF("selectionLength= %d", theSelection->selectionLength);
        return TRUE;
    }
    else
    {
        BR_ERRORPRINTF("input parameter error");
        return FALSE;
    }
}

// close the selection file
bool selection_file_close(FILE * selectionFile)
{
    if (selectionFile != NULL)
    {
        BR_LOGPRINTF("Close the currently used selection file 0x%x", (unsigned int) selectionFile);
        fclose(selectionFile);
        selectionFile = NULL;
        return TRUE;
    }
    return FALSE;
}

// retrieve the list of currenly available selections in the selection file
// the memory allocated for this list should be free by the calling routine
// 
// IMPORTANT REMARK : the sequence of the selections should be respected !!!
//   
GSList *selection_file_get_list(FILE * selectionFile)
{
    GSList *theSelectionList = NULL;
    SelectionID *theSelection = NULL;

    if (selectionFile)
    {
        // goto the beginning of the selection file
        rewind(selectionFile);

        while (!feof(selectionFile))
        {
            theSelection = g_new0(SelectionID, 1);
            fscanf(selectionFile, "selection\n");
            fscanf(selectionFile, "parentNodeTagName= %s\n", &theSelection->parentNodeTagName);
            fscanf(selectionFile, "parentNodeIndex= %d\n", &theSelection->parentNodeIndex);
            fscanf(selectionFile, "nodeIndex= %d\n", &theSelection->nodeIndex);
            fscanf(selectionFile, "selectionStart= %d\n", &theSelection->selectionStart);
            fscanf(selectionFile, "selectionLength= %d\n", &theSelection->selectionLength);

            BR_LOGPRINTF("\n------------------selection---------------------");
            BR_LOGPRINTF("parentNodeTagName= %s", theSelection->parentNodeTagName);
            BR_LOGPRINTF("parentNodeIndex= %d", theSelection->parentNodeIndex);
            BR_LOGPRINTF("nodeIndex= %d", theSelection->nodeIndex);
            BR_LOGPRINTF("selectionStart= %d", theSelection->selectionStart);
            BR_LOGPRINTF("selectionLength= %d", theSelection->selectionLength);
            BR_LOGPRINTF("---------------------------------------------------");

            theSelectionList = g_slist_append(theSelectionList, theSelection);
        }
    }
    return theSelectionList;
}

#endif //SELECTION_ENABLED
