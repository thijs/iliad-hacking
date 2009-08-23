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

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include <glib.h>

#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "erMdsContent.h"

#define DISPLAY_ITEMS 6


void usage()
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  -r : search recursively\n");
    fprintf(stderr, "  -d <dir1> [can be repeated multiple times]\n");
    fprintf(stderr, "  -m <field-type> <pattern> : only include files that match pattern\n");
    fprintf(stderr, "                              [can be repeated multiple times]\n");
    fprintf(stderr,
            "  -c : only include files that match all patterns (specified with -m option) (default = one pattern)\n");
    fprintf(stderr, "  -s <field-type> : sort according to <field-type>\n");
    fprintf(stderr, "                  : i.e. one off author, date, description, file, title, fsname\n");
    fprintf(stderr, "  -a : sort ascending (default = descending )\n");
}

// Testbench
// Arguments are interpreted as directories that need to be scanned

int main(int argc, char *argv[])
{
    GArray *contentItemArray = NULL;

    // Parameters for function_under_test
    GArray *dirArray;
    mdsFsItem_t *pFsItem;
    mdsSortStruct_t sortStruct;
    mdsDirectory_t dirStruct;
    int     i;
    FILE   *pOutput = NULL;
    int     nArg = 1;
    int     nRetContentScan;

    dirArray = g_array_new(FALSE, FALSE, sizeof(mdsDirectory_t));
    sortStruct.ascending = 0;
    sortStruct.ft = mdsFieldNone;

    // Options follow the pattern
    // -option <argument>
    // -option
    if (argc > 1)
    {
        while (nArg < argc)
        {
            if (argv[nArg][0] == '-')
            {
                switch (argv[nArg][1])
                {
                case 'h':
                    usage();
                    return 0;
                    break;

                case 'a':
                    sortStruct.ascending = 1;
                    nArg++;
                    break;

                case 'd':
                    // What directories should be traversed?
                    if (++nArg >= argc)
                    {
                        // Not enough arguments supplied.
                        fprintf(stderr, "Supply a directory name after option -d");
                        usage();
                        return -1;
                    }
                    else
                    {
                        // g_ptr_array_add(dirArray, (gpointer) argv[nArg]);
                        strcpy(dirStruct.szFilename, argv[nArg]);
                        g_array_append_val(dirArray, dirStruct);
                        nArg++;
                    }
                    break;

                case 's':
                    // Sort on what field?
                    if (++nArg >= argc)
                    {
                        // Not enough arguments supplied.
                        fprintf(stderr, "Supply a field type after option -s\n");
                        usage();
                        return -1;
                    }
                    else
                    {
                        // Do we now the field supplied as an argument after -s ?
                        if (strcmp("author", argv[nArg]) == 0)
                            sortStruct.ft = mdsFieldAuthor;
                        else if (strcmp("date", argv[nArg]) == 0)
                            sortStruct.ft = mdsFieldDate;
                        else if (strcmp("description", argv[nArg]) == 0)
                            sortStruct.ft = mdsFieldDescription;
                        else if (strcmp("file", argv[nArg]) == 0)
                            sortStruct.ft = mdsFieldFile;
                        else if (strcmp("title", argv[nArg]) == 0)
                            sortStruct.ft = mdsFieldTitle;
                        else if (strcmp("fsname", argv[nArg]) == 0)
                            sortStruct.ft = mdsFieldFsName;
                        else
                        {
                            // Sort field type unknown
                            fprintf(stderr, "Sort field type %s is unknown (after option -s)\n", argv[nArg]);
                            usage();
                            return -1;
                        }
                        nArg++;
                    }
                    break;

                default:
                    fprintf(stderr, "Option %s not known.\n", argv[nArg]);
                    usage();
                    return -1;
                }
            }
            else
            {
                fprintf(stderr, "Argument supplied not proceded by option.\n");
                usage();
                return -1;
            }
        }
    }
    else
    {
        usage();
        return -1;
    }

    pOutput = fopen("testContentScan.txt", "w");
    if (pOutput == NULL)
    {
        CL_ERRORPRINTF("Could not open file testContentScan.txt");
    }

    // contentItemArray HAS to be set to NULL, otherwise no memory will be allocated by erMdsContentScan
    contentItemArray = NULL;

    CL_ERRORPRINTF("dirArray->len = %d", dirArray->len); // Debug

    nRetContentScan = erMdsContentScan(dirArray, &contentItemArray);

    CL_WARNPRINTF("Basic test of erMdsContentScan");
    for (i = 0; i < contentItemArray->len; i++)
    {
        pFsItem = &g_array_index(contentItemArray, mdsFsItem_t, i);
        printf("Filename: %s", pFsItem->szFilename);
        printf("\tFile Item Type: %d\n", (int) pFsItem->fit);
    }

    // Optionaly sort the contentItemArray
    // 
    if (sortStruct.ft != mdsFieldNone)
    {
        erMdsContentSort(contentItemArray, &sortStruct);
    }

    CL_WARNPRINTF("Basic test of erClGetDisplayItems");
    // Call erClGetDisplayItems with chunks of nChunk items
    for (i = 0; i < contentItemArray->len; i += DISPLAY_ITEMS)
    {
        int     nChunk = DISPLAY_ITEMS;
        clDisplayItem_t displayItemArray[DISPLAY_ITEMS];
        int     nBegin = i;
        int     nRet;

        nRet = erClGetDisplayItems(contentItemArray, nBegin, &nChunk, displayItemArray);
        switch (nRet)
        {
        case ERCL_INVALID_ARGS:
            CL_ERRORPRINTF("ERCL_INVALID_ARGS");
            break;

        case ERCL_OK:
            CL_ERRORPRINTF("ERCL_OK");
            {
                int     j;

                for (j = 0; j < nChunk; j++)
                {
                    fprintf(stderr, "displayItemArray\n");
                    fprintf(stderr, "fit:%d, ", displayItemArray[j].fit);
                    fprintf(stderr, "fileName:%s, ", displayItemArray[j].szFilename);
                    fprintf(stderr, "iconID:%d, ", displayItemArray[j].iconID);
                    fprintf(stderr, "clIconURL:%s, ", displayItemArray[j].clIconURL);
                    fprintf(stderr, "szFileExt:%s, ", displayItemArray[j].szFileExt);
                    fprintf(stderr, "szTitle:%s, ", displayItemArray[j].szTitle);
                    fprintf(stderr, "szSubTitle:%s, ", displayItemArray[j].szSubTitle);
                    fprintf(stderr, "szDate:%s, ", displayItemArray[j].szDate);
                    fprintf(stderr, "szDescription:%s, ", displayItemArray[j].szDescription);
                    // fprintf(stderr,"szFilename: %s, ", displayItemArray[i].szFilename);
                    fprintf(stderr, "szManifest:%s\n", displayItemArray[j].szManifest);
                    if (pOutput != NULL)
                    {
                        fprintf(pOutput, "%d\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
                                displayItemArray[j].fit,
                                displayItemArray[j].szFilename,
                                displayItemArray[j].iconID,
                                displayItemArray[j].clIconURL,
                                displayItemArray[j].szFileExt,
                                displayItemArray[j].szTitle,
                                displayItemArray[j].szSubTitle,
                                displayItemArray[j].szDate, displayItemArray[j].szDescription,
                                displayItemArray[j].szManifest);
                    }
                }
            }
            break;
        }
    }

    if (pOutput != NULL)
        fclose(pOutput);

    // Print out statistics.
    switch (nRetContentScan)
    {
    case ERMDS_CONTENT_SCAN_FAILED:
        fprintf(stderr, "Content Scan failed");
        break;

    case ERMDS_CONTENT_SCAN_TOO_MANY_ITEMS:
        fprintf(stderr, "Content Scan File limit kicked in!\n");
    case ERMDS_CONTENT_SCAN_OK:
        fprintf(stderr, "Content Scan Retrieved %d items\n", contentItemArray->len);
        break;
    }

    g_array_free(contentItemArray, TRUE);
    g_array_free(dirArray, TRUE);
    return 0;
}
