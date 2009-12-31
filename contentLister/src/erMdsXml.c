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

/* 
 * Copyright (C) 2006-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "control_share.h"
#include "erMdsContent.h"
#include "languages.h"


// local functions
static void fetchSelectionDetails(const xmlNodePtr node, mdsSelection_t* selection);
static void addSelectionCopyMove(mdsSelectionList_t* selectionList);
static int  selectionCompareFunc(const void* a, const void* b);
static void fetchSelectionItemDetails(const xmlNodePtr node, mdsSelectionItem_t* item);
static int  selectionItemCompareFunc(const void* a, const void* b);


// notes: don't forget free the memory when no use anymore
static char* getVerifiedPath(char* szPath, char* szContainerPath)
{
    int      nPathLength;
    char     szRealPath[PATH_MAX];
    gboolean bUglyHack = FALSE; 

    char* szReturnPath = malloc(sizeof(char) * ERMDS_MAX_FILENAME_SIZE);

    // Test to see if this appears to be an absolute path
    if (szPath[0] == '/')
    {
        nPathLength = g_snprintf(szReturnPath, ERMDS_MAX_FILENAME_SIZE, "%s", szPath);
    }
    else if (strcmp(szPath, "dummy.bmp") == 0)
    {
        CL_WARNPRINTF("Ugly hack for dummy.bmp");
        nPathLength = g_snprintf(szReturnPath, ERMDS_MAX_FILENAME_SIZE, "%s", szContainerPath);
        bUglyHack = TRUE;
    }
    else
    {
        nPathLength = g_snprintf(szReturnPath, ERMDS_MAX_FILENAME_SIZE, "%s/%s", szContainerPath, szPath);
    }

    // Expand the path and see if it is trusted
    if (realpath(szReturnPath, szRealPath) != NULL)
    {
        if (ctrl_is_trusted_path(szRealPath))
        {
            // All is ok
            nPathLength = g_snprintf(szReturnPath, ERMDS_MAX_FILENAME_SIZE, "%s", szRealPath);
        }
        else
        {
            // Error, trigger error condition below
            CL_ERRORPRINTF("ctrl_is_trusted_path error for [%s]", szRealPath);
            nPathLength = ERMDS_MAX_FILENAME_SIZE + 1;
        }
    }
    else
    {
        // Error, trigger error condition below
        CL_ERRORPRINTF("realpath error for [%s]", szReturnPath);
        nPathLength = ERMDS_MAX_FILENAME_SIZE + 1;
    }

    if (bUglyHack == TRUE)
    {
        nPathLength = g_snprintf(szReturnPath, ERMDS_MAX_FILENAME_SIZE, "%s/%s", szReturnPath, szPath);
    }
    
    if (nPathLength > ERMDS_MAX_FILENAME_SIZE)
    {
        // szFilename path is too long.
        // This is an error condition: show the container as a folder?
        CL_ERRORPRINTF("content path too long");
        free(szReturnPath);
        szReturnPath = NULL;
    }
    
    return szReturnPath;
}

static void parseModifyEnable(xmlDocPtr doc, xmlNodePtr cur, clDisplayItem_t * displayItem, char *szContainerPath)
{
    xmlChar*   key;
    xmlNodePtr child;
    gboolean   bDefault;
    gboolean   bSpecific;

    // get default value
    bDefault = FALSE;
    key = xmlGetProp(cur, "default");
    if (key)
    {
        if ( xmlStrcmp(key, "true") == 0 )
        {
            bDefault = TRUE;
        }
        xmlFree(key);
    }
    displayItem->modifyEnable.bDefault = bDefault;

    // specific values as specified in child elements
    bSpecific = bDefault;
    child = ermXmlGetChildNode(cur, "delete-enable");
    if (child)
    {
        key = xmlNodeGetContent(child);
        if (key)
        {
            if ( xmlStrcmp(key, "true") == 0 )
            {
                bSpecific = TRUE;
            }
            else
            {
                // specific is "false" or invalid,
                // in both situations consider it "false"
                bSpecific = FALSE;
            }
            xmlFree(key);
        }
    }
    displayItem->modifyEnable.bDelete = bSpecific;
    //
    bSpecific = bDefault;
    child = ermXmlGetChildNode(cur, "scribble-enable");
    if (child)
    {
        key = xmlNodeGetContent(child);
        if (key)
        {
            if ( xmlStrcmp(key, "true") == 0 )
            {
                bSpecific = TRUE;
            }
            else
            {
                // specific is "false" or invalid,
                // in both situations consider it "false"
                bSpecific = FALSE;
            }
            xmlFree(key);
        }
    }
    displayItem->modifyEnable.bScribble = bSpecific;
    //
    bSpecific = bDefault;
    child = ermXmlGetChildNode(cur, "tagging-enable");
    if (child)
    {
        key = xmlNodeGetContent(child);
        if (key)
        {
            if ( xmlStrcmp(key, "true") == 0 )
            {
                bSpecific = TRUE;
            }
            else
            {
                // specific is "false" or invalid,
                // in both situations consider it "false"
                bSpecific = FALSE;
            }
            xmlFree(key);
        }
    }
    displayItem->modifyEnable.bTagging = bSpecific;
}

static void parseYMetadata(xmlDocPtr doc, xmlNodePtr cur, clDisplayItem_t * displayItem, char *szContainerPath)
{
    xmlChar *key;
    char    *szFilename;

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "image"))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (key != NULL)
            {
                int     nPathLength;

                CL_LOGPRINTF("image: %s", key);

                if (key[0] == '_') {
                  *key++;
                  // This path is not verified and taken relative from
                  // shared folder
                  nPathLength = g_snprintf(displayItem->clIconURL, ERMDS_MAX_FILENAME_SIZE, "/usr/local/share/contentlister/icons/%s", key);
                }
                else {
                  // This path is not verified and no absolute paths are allowed
                  nPathLength = g_snprintf(displayItem->clIconURL, ERMDS_MAX_FILENAME_SIZE, "%s/%s", szContainerPath, key);
                }

                if (nPathLength < ERMDS_MAX_FILENAME_SIZE)
                {
                    displayItem->iconID = clIconUrlDefined;
                }
                else
                {
                    // IconPath is too long.
                    CL_ERRORPRINTF("IconPath is too long: %d", nPathLength);
                    displayItem->iconID = clUnknownIcon;
                }
                xmlFree(key);
            }
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *) "startpage"))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (key != NULL)
            {
                CL_LOGPRINTF("startpage: %s", key);

                // Check to see if the startpage exists and is in a trusted location
                szFilename = getVerifiedPath(key, szContainerPath);
                if (szFilename == NULL)
                {
                    // Something is wrong with the startpage (might not exist, or outside trusted area)
                    CL_ERRORPRINTF("startpage [%s] is incorrect", key);
                    strcpy(displayItem->szFilename, szContainerPath);
                }
                else
                {
                    strcpy(displayItem->szFilename, szFilename);
                    free(szFilename);
                    szFilename = NULL;
                }

                xmlFree(key);
            }
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *) "ItemSize"))
        {
            CL_LOGPRINTF("Found node Size");
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (key != NULL)
            {
                CL_LOGPRINTF("Size: %s", key);
                displayItem->size = atoi(key);
                xmlFree(key);
            }
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *) "modify-enable"))
        {
            CL_LOGPRINTF("Found node modify-enable");
            parseModifyEnable(doc, cur, displayItem, szContainerPath);
        }
        cur = cur->next;
    }
    return;
}

static void parseDcMetadata(xmlDocPtr doc, xmlNodePtr cur, clDisplayItem_t * displayItem, char *szContainerPath)
{
    xmlChar *key;

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "Description"))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (key != NULL)
            {
                int     nPathLength;

                CL_LOGPRINTF("Description: %s", key);
                nPathLength = g_snprintf(displayItem->szDescription, ERCL_MAX_DESCRIPTION_SIZE, "%s", key);
                xmlFree(key);
            }
        }

        else if (!xmlStrcmp(cur->name, (const xmlChar *) "Date"))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (key != NULL)
            {
                int     nPathLength;

                CL_LOGPRINTF("Date: %s", key);
                nPathLength = g_snprintf(displayItem->szDate, ERCL_MAX_SUBTITLE_SIZE, "%s", key);
                xmlFree(key);
            }
        }

        else if (!xmlStrcmp(cur->name, (const xmlChar *) "Title"))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (key != NULL)
            {
                int     nPathLength;

                CL_LOGPRINTF("Title: %s", key);
                nPathLength = g_snprintf(displayItem->szTitle, ERCL_MAX_DESCRIPTION_SIZE, "%s", key);
                xmlFree(key);
            }
        }
        cur = cur->next;
    }
    return;
}

static void parseMetadata(xmlDocPtr doc, xmlNodePtr cur, clDisplayItem_t * displayItem, char *szContainerPath)
{
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "dc-metadata"))
        {
            CL_LOGPRINTF("Found node dc-metadata");
            parseDcMetadata(doc, cur, displayItem, szContainerPath);
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *) "y-metadata"))
        {
            CL_LOGPRINTF("Found node y-metadata");
            parseYMetadata(doc, cur, displayItem, szContainerPath);
        }
        cur = cur->next;
    }
    return;
}

static void parseStoragedata(xmlDocPtr doc, xmlNodePtr cur, clDisplayItem_t * displayItem, char *szContainerPath)
{
    xmlChar *key;

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "Identifier"))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (key != NULL)
            {
                int     nPathLength;

                CL_LOGPRINTF("Identifier: %s", key);
                nPathLength = g_snprintf(displayItem->szExtID, ERMDS_MAX_EXTENSION_ID_SIZE, "%s", key);
                xmlFree(key);
            }
        }
        cur = cur->next;
    }
    return;
}

static void parseApplicationdata(xmlDocPtr doc, xmlNodePtr cur, clDisplayItem_t * displayItem, char *szContainerPath)
{
    xmlChar *key;

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "Identifier"))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (key != NULL)
            {
                int     nPathLength;

                CL_LOGPRINTF("Identifier: %s", key);
                nPathLength = g_snprintf(displayItem->szExtID, ERMDS_MAX_EXTENSION_ID_SIZE, "%s", key);
                xmlFree(key);
            }
        }
        cur = cur->next;
    }
    return;
}

static void parseDirectorydata(xmlDocPtr doc, xmlNodePtr cur, clDisplayItem_t * displayItem, char *szContainerPath)
{
    xmlChar *key;
    char    *szFilename;

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "content"))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (key != NULL)
            {
                CL_LOGPRINTF("content: %s", key);

                szFilename = getVerifiedPath(key, szContainerPath);
                if (szFilename == NULL)
                {
                    CL_ERRORPRINTF("directory [%s] is incorrect", key);
                    strcpy(displayItem->szFilename, szContainerPath);
                }
                else
                {
                    strcpy(displayItem->szFilename, szFilename);
                    free(szFilename);
                    szFilename = NULL;
                }

                xmlFree(key);
            }
        }
        cur = cur->next;
    }
    return;
}

int erClXmlParseManifest(char *szContainerPath, clDisplayItem_t * displayItem)
{
    int      ret = -1;  // return value: 0 = ok, -1 = error

    xmlDoc  *doc            = NULL;
    xmlNode *rootElement    = NULL;
    xmlNode *cur            = NULL;
    xmlNode *symlinkElement = NULL;
    xmlChar *targetPath     = NULL;
    xmlChar *key;


    int n = strlen(szContainerPath) + 1 + strlen(MANIFEST_FILENAME) + 1;
    char *szFilename = alloca(n);
    g_assert(szFilename != NULL);
    snprintf(szFilename, n, "%s/" MANIFEST_FILENAME, szContainerPath);
    
    LIBXML_TEST_VERSION doc = xmlParseFile(szFilename);

    if (doc == NULL)
    {
        CL_ERRORPRINTF("Document not parsed successfully");
        return ret;
    }

    /* Get the root element node */
    rootElement = xmlDocGetRootElement(doc);

    // Check whether we have a document with the correct root (i.e. package)
    if (xmlStrcmp(rootElement->name, (const xmlChar *) "package"))
    {
        CL_ERRORPRINTF("Document of the wrong type, root node != package");
        xmlFreeDoc(doc);
        return ret;
    }

    
    // Check whether this manifest actually is a symlink
    symlinkElement = ermXmlGetChildNode(rootElement, "symlink");
    if (symlinkElement)
    {
        // check whether target must be extended with locale
        displayItem->fit = mdsFitManifestSymlink;
        key = xmlGetProp(symlinkElement, "add-locale");
        if (key)
        {
            if (xmlStrcmp(key, "true") == 0)
            {
                displayItem->fit = mdsFitManifestSymlinkLocale;
            }
            xmlFree(key);
        }

        // get target details
        targetPath = xmlNodeGetContent(symlinkElement);
        szFilename = getVerifiedPath(targetPath, szContainerPath);
        CL_WARNPRINTF("symlink [%s] [%s] --> [%s]", szContainerPath, targetPath, szFilename);
        if (szFilename)
        {
            strcpy(displayItem->szFilename, szFilename);
            free(szFilename);
        }
        else
        {
            CL_ERRORPRINTF("directory [%s] [%s] is incorrect", szContainerPath, targetPath);
            // display as a folder without manifest
            strcpy(displayItem->szFilename, szContainerPath);
            displayItem->fit = mdsFitFolder;
        }
    }
    else
    {
        cur = rootElement->xmlChildrenNode;
        while (cur != NULL)
        {
            if ((!xmlStrcmp(cur->name, (const xmlChar *) "metadata")))
            {
                CL_LOGPRINTF("Found node metadata");
                parseMetadata(doc, cur, displayItem, szContainerPath);
            }

            if ((!xmlStrcmp(cur->name, (const xmlChar *) "storage")))
            {
                CL_LOGPRINTF("Found node storage");
                parseStoragedata(doc, cur, displayItem, szContainerPath);
                //update the type of container, since this is a special type of container
                displayItem->fit = mdsFitStorage;
            }

            if ((!xmlStrcmp(cur->name, (const xmlChar *) "application")))
            {
                CL_LOGPRINTF("Found node application");
                parseApplicationdata(doc, cur, displayItem, szContainerPath);
                //update the type of container, since this is a special type of container
                displayItem->fit = mdsFitApplication;
            }

            if ((!xmlStrcmp(cur->name, (const xmlChar *) "directory")))
            {
                CL_LOGPRINTF("Found node directory");
                parseDirectorydata(doc, cur, displayItem, szContainerPath);
                //update the type of container, since this is a special type of container
                displayItem->fit = mdsFitManifestDirectory;
            }
            cur = cur->next;
        }
    }

    /* free the document */
    xmlFreeDoc(doc);

    /* Free the global variables that may have been allocated by the parser */
    xmlCleanupParser();

    return 0;
}

// Return 1 when field is found
static int parseMetadataForField(xmlDocPtr doc, xmlNodePtr cur, char *szFieldType, char *szField)
{
    xmlChar *key;

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *) szFieldType))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if (key != NULL)
            {
                int     nPathLength;

                CL_LOGPRINTF("szFieldType: %s", key);

                nPathLength = g_snprintf(szField, ERCL_MAX_DESCRIPTION_SIZE, "%s", key);
                xmlFree(key);
                return 1;
            }
        }
        cur = cur->next;
    }
    return -1;
}


// Return 1 when field is found
static int parseMetadataForBranch(xmlDocPtr doc, xmlNodePtr cur, char *szFieldType, int nDcBranch, char *szField)
{
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "dc-metadata"))
        {
            CL_LOGPRINTF("Found node dc-metadata");
            if (nDcBranch == 1)
            {
                return parseMetadataForField(doc, cur, szFieldType, szField);
            }
        }
        else if (!xmlStrcmp(cur->name, (const xmlChar *) "y-metadata"))
        {
            CL_LOGPRINTF("Found node y-metadata");
            if (nDcBranch != 1)
            {
                return parseMetadataForField(doc, cur, szFieldType, szField);
            }
        }
        cur = cur->next;
    }
    return -1;
}


void erClXmlFetchField(char *szFilename, mdsFieldType_e fieldType, char *szField)
{
    char    szFieldType[ERCL_MAX_FIELDNAME_SIZE];
    int     nDcBranch;
    xmlDoc *doc = NULL;
    xmlNode *rootElement = NULL;
    xmlNode *cur = NULL;

    strcpy(szField, "");

    switch (fieldType)
    {
    case mdsFieldDate:
        strcpy(szFieldType, "Date");
        break;

    case mdsFieldDescription:
        strcpy(szFieldType, "Description");
        break;

    case mdsFieldFile:
        strcpy(szFieldType, "startpage");
        break;

    case mdsFieldTitle:
        strcpy(szFieldType, "Title");
        break;
    
    case mdsFieldSize:
        strcpy(szFieldType, "ItemSize");
        break;

    default:
        CL_ERRORPRINTF("FieldType %d can not be retrieved from a manifest file", (int) fieldType);
        strcpy(szField, "");
        return;
    }

    // Another switch to determine the branch in which we need to search for this field
    switch (fieldType)
    {
    case mdsFieldFile:
        nDcBranch = 0;
        break;

    default:
        nDcBranch = 1;
        break;
    }

    LIBXML_TEST_VERSION doc = xmlParseFile(szFilename);

    if (doc == NULL)
    {
        CL_ERRORPRINTF("Document not parsed successfully");
        return;
    }

    /* Get the root element node */
    rootElement = xmlDocGetRootElement(doc);

    // Check whether we have a document with the correct root (i.e. package)
    if (xmlStrcmp(rootElement->name, (const xmlChar *) "package"))
    {
        CL_ERRORPRINTF("Document of the wrong type, root node != package");
        xmlFreeDoc(doc);
        return;
    }

    cur = rootElement->xmlChildrenNode;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "metadata")))
        {
            CL_LOGPRINTF("Found node metadata");
            parseMetadataForBranch(doc, cur, szFieldType, nDcBranch, szField);
        }
        cur = cur->next;
    }

    /* free the document */
    xmlFreeDoc(doc);

    /* Free the global variables that may have been allocated by the parser */
    xmlCleanupParser();

    return;
}

gboolean erClXmlSetFieldSize(const gchar* szContainer, int size)
{
    CL_LOGPRINTF("entry: manifest [%s], size [%d]", szContainer, size);
    
    g_return_val_if_fail((szContainer != NULL), FALSE);

    erManifest manifest;
    int        nLength;
    char       szSize[ERMDS_MAX_FILENAME_SIZE];
    gboolean   retval = FALSE;
    
    // open manifest file and update it
    if (RET_OK == ermXmlOpenManifest(szContainer, &manifest))
    {
        nLength = g_snprintf(szSize, ERMDS_MAX_FILENAME_SIZE, "%d", size);
        CL_LOGPRINTF("size = %s", szSize);

        // update size
        if (RET_OK == ermXmlExist(&manifest, "/package/metadata/y-metadata/ItemSize"))
        {
            ermXmlSetString( &manifest,
                             "/package/metadata/y-metadata/ItemSize",
                             szSize );
        }
        else
        {
            ermXmlNewString( &manifest,
                             "/package/metadata/y-metadata",
                             "ItemSize",
                             szSize );
        }
        
        // save manifest
        ermXmlSaveAndClose(&manifest);

        retval = TRUE;
    }
    else
    {
        CL_ERRORPRINTF("Could not open manifest file in [%s]", szContainer);
    }
    
    return retval;
}


const mdsSelectionList_t* erClXmlFetchSelectionList(const char* szManifestPath)
{
    int  rc;
    int  selNum;  // number of selections
    int  selIdx;  // selection index

    erManifest*         pCtx           = NULL;  // XML context for libermanifest
    xmlXPathObjectPtr   xpathObj       = NULL;
    xmlNodePtr*         p_node         = NULL;
    xmlNodeSetPtr       selectionNodes = NULL;  // list of "selection" nodes
    xmlNodePtr          nodeRoot       = NULL;  // root node of the attribute list
    mdsSelectionList_t* selectionList  = NULL;
    mdsSelection_t*     selection      = NULL;

    CL_LOGPRINTF("entry: manifest [%s]", szManifestPath);

    selectionList = g_new0(mdsSelectionList_t, 1);
    
    // open manifest file
    pCtx = &(selectionList->manifest);
    rc = ermXmlOpenFile(szManifestPath, pCtx);
    if (rc != RET_OK)
    {
        CL_ERRORPRINTF("Cannot read manifest file [%s]", szManifestPath);
        goto ERROR_EXIT;
    }
    else { CL_LOGPRINTF("manifest opened ok"); }

    // validate manifest file
    //rc = ermXmlValidateManifest(pCtx);
    rc = ermXmlValidateManifest(pCtx);
    if (rc != RET_OK)
    {
        CL_ERRORPRINTF("manifest file [%s] NOT validated", szManifestPath);
        goto ERROR_EXIT;
    }
    else { CL_LOGPRINTF("manifest validated ok"); }


    // get selection-list node
    xpathObj = ermXmlSelectNodes(pCtx, XMLNODE_SELECTION_LIST);
    if (xpathObj == NULL)
    {
        CL_WARNPRINTF("No selection-list in [%s]", szManifestPath);
        goto ERROR_EXIT;
    }
    else { CL_LOGPRINTF("selection list found"); }

    //     store root node of the selection-list tree
    nodeRoot = xpathObj->nodesetval->nodeTab[0];
    selectionList->nodeRoot = nodeRoot;
    //     free temporary data
    xmlXPathFreeObject(xpathObj);
    xpathObj = NULL;

    //     get selection nodes
    selectionNodes = ermXmlGetChildNodeSet(nodeRoot, "selection");
    if (selectionNodes->nodeNr == 0)
    {
        CL_ERRORPRINTF("Empty selection-list in [%s]", szManifestPath);
        selectionList->num_selections = 0;
        goto ERROR_EXIT;
    }
    selNum = selectionNodes->nodeNr;
    selectionList->num_selections = selNum;
    selectionList->selections     = g_new0(mdsSelection_t, selNum);

    p_node    = selectionNodes->nodeTab;
    selection = (mdsSelection_t*) selectionList->selections;  // const_cast
    for (selIdx = 0 ; selIdx < selNum ; selIdx++, p_node++, selection++)
    {
        fetchSelectionDetails(*p_node, selection);
    }

    // sort the selections
    qsort( (void*)selectionList->selections,
            selectionList->num_selections,
            sizeof(selectionList->selections[0]),
            selectionCompareFunc );

ERROR_EXIT:
    // add copy/move selection when not found in manifest
    if (selectionList)
    {
        addSelectionCopyMove(selectionList);
    }

    // free temporary memory and return the list of selections
    if (selectionNodes)  { xmlXPathFreeNodeSet(selectionNodes); }
    if (xpathObj)        { xmlXPathFreeObject(xpathObj);        }
    return selectionList;
}

static void addSelectionCopyMove(mdsSelectionList_t* selectionList)
{
    gboolean found;
    int      selIdx;

    int                 selNum    = selectionList->num_selections;
    mdsSelection_t*     selection = NULL;
    mdsSelectionItem_t* item      = NULL;

    // check whether copy/move selection present
    found     = FALSE;
    selection = (mdsSelection_t*) selectionList->selections;  // const_cast
    for (selIdx = 0 ; selIdx < selNum ; selIdx++, selection++)
    {
        if (strcmp(selection->id, SELECTION_TRANSFER_MODE) == 0)
        {
            found = TRUE;
        }
    }

    // add copy/move selection
    if ( !found )
    {
        // allocate one more selection
        selection = (mdsSelection_t*) selectionList->selections;  // const_cast
        selection = g_renew(mdsSelection_t, selection, selNum + 1);
        selectionList->selections = selection;
        selectionList->num_selections++;

        // free up the first selection
        selection  = (mdsSelection_t*) selectionList->selections;  // const_cast
        selection += selNum;
        for (selIdx = 0 ; selIdx < selNum ; selIdx++, selection--)
        {
            *selection = *(selection - 1);
        }
        memset((void*)selection, 0x00, sizeof(*selection));

        // make first selection a copy/move list
        selection = (mdsSelection_t*) selectionList->selections;  // const_cast
        selection->constant    = FALSE;
        selection->hide        = FALSE;
        selection->display_as  = xmlCharStrdup("textbutton");
        selection->title       = xmlCharStrdup( _("Transfer mode") );
        selection->instruction = xmlCharStrdup( _("Please select how to add this document to your outbox") );
        selection->min_selected = 1;
        selection->max_selected = 1;
        selection->num_items    = 2;
        selection->items        = NULL;
        selection->id           = xmlCharStrdup(SELECTION_TRANSFER_MODE);
        selection->sequence     = 0;

        // add items to this selection
        selection->items = g_new0(mdsSelectionItem_t, 2);
        //   item[0] = copy
        item = (mdsSelectionItem_t*) selection->items;  // const_cast
        item->name     = xmlCharStrdup( _("Copy") );
        item->id       = xmlCharStrdup( "copy" );
        item->sequence = 1;
        item->state    = TRUE;
        //   item[1] = move
        item++;
        item->name     = xmlCharStrdup( _("Move") );
        item->id       = xmlCharStrdup( "move" );
        item->sequence = 2;
        item->state    = FALSE;
    }
}

static int selectionCompareFunc(const void* a, const void* b)
{
    int  ret = 0;
    mdsSelection_t* selA = (mdsSelection_t*)a;
    mdsSelection_t* selB = (mdsSelection_t*)b; 

    if (selA  &&  selB)
    {
        ret = selA->sequence - selB->sequence;
    }
    else 
    {
        CL_ERRORPRINTF("selA [%p] selB [%p]", selA, selB);
    }

    return ret;
}

static void fetchSelectionDetails(const xmlNodePtr selectionNode, mdsSelection_t* selection)
{
    mdsSelectionItem_t* item = NULL;

    int           i;
    xmlChar*      xcp;
    xmlNodePtr    child = NULL;
    xmlNodePtr*   p_node = NULL;
    xmlNodeSetPtr itemNodes = NULL;
    int           itemNum;  // number of items
    int           itemIdx;  // item index


    CL_LOGPRINTF("entry");

    // get "id" attribute
    xcp = xmlGetProp(selectionNode, "id");
    if (xcp == NULL)
    {
        xcp = xmlStrdup("");
    }
    selection->id = xcp;

    // get "constant" attribute
    selection->constant = FALSE;
    xcp = xmlGetProp(selectionNode, "constant");
    if (xcp)
    {
        if (xmlStrcmp(xcp, "true") == 0)
        {
            selection->constant = TRUE;
        }
        xmlFree(xcp);
    }

    // get "hide" attribute
    selection->hide = FALSE;
    xcp = xmlGetProp(selectionNode, "hide");
    if (xcp)
    {
        if (xmlStrcmp(xcp, "true") == 0)
        {
            selection->hide = TRUE;
        }
        xmlFree(xcp);
    }

    // get "sequence" attribute
    selection->sequence = 0;
    xcp = xmlGetProp(selectionNode, "sequence");
    if (xcp)
    {
        i = atoi(xcp);
        if (i >= 0)
        {
            selection->sequence = i;
        }
        xmlFree(xcp);
    }

    // get "display-as" value
    xcp   = NULL;
    child = ermXmlGetChildNode(selectionNode, "display-as");
    if (child)
    {
        xcp = xmlNodeGetContent(child);
    }
    if (xcp == NULL)
    {
        xcp = xmlStrdup("");
    }
    selection->display_as = xcp;

    // get "title" value
    xcp   = NULL;
    child = ermXmlGetChildNode(selectionNode, "title");
    if (child)
    {
        xcp = xmlNodeGetContent(child);
    }
    if (xcp == NULL)
    {
        xcp = xmlStrdup("");
    }
    selection->title = xcp;

    // get "instruction" value
    xcp   = NULL;
    child = ermXmlGetChildNode(selectionNode, "instruction");
    if (child)
    {
        xcp = xmlNodeGetContent(child);
    }
    if (xcp == NULL)
    {
        xcp = xmlStrdup("");
    }
    selection->instruction = xcp;

    // get "min-selected" value
    selection->min_selected = 1;  // default
    child = ermXmlGetChildNode(selectionNode, "min-selected");
    if (child)
    {
        xcp = xmlNodeGetContent(child);
        i = atoi(xcp);
        if (i >= 0)
        {
            selection->min_selected = i;
        }
        xmlFree(xcp);
    }

    // get "max-selected" value
    selection->max_selected = 1;  // default
    child = ermXmlGetChildNode(selectionNode, "max-selected");
    if (child)
    {
        xcp = xmlNodeGetContent(child);
        i = atoi(xcp);
        if (i >= 0)
        {
            selection->max_selected = i;
        }
        xmlFree(xcp);
    }

    // get selection items
    selection->num_items = 0;
    child = ermXmlGetChildNode(selectionNode, "selection-item-list");
    if (child)
    {
        itemNodes = ermXmlGetChildNodeSet(child, "selection-item");
        itemNum   = itemNodes->nodeNr;
        selection->num_items = itemNum;
        selection->items     = g_new0(mdsSelectionItem_t, itemNum);

        p_node = itemNodes->nodeTab;
        item = (mdsSelectionItem_t*) selection->items;  // const_cast
        for (itemIdx = 0 ; itemIdx < itemNum ; itemIdx++, p_node++, item++)
        {
            fetchSelectionItemDetails(*p_node, item);
        }
    }
    else { CL_ERRORPRINTF("No selection-item-list"); }

    // sort the selections
    qsort( (void*)selection->items,
            selection->num_items,
            sizeof(selection->items[0]),
            selectionItemCompareFunc );
    
    // free temporary memory
    if (itemNodes) { xmlXPathFreeNodeSet(itemNodes); }
}


static int selectionItemCompareFunc(const void* a, const void* b)
{
    int  ret = 0;
    mdsSelectionItem_t* itemA = (mdsSelectionItem_t*)a;
    mdsSelectionItem_t* itemB = (mdsSelectionItem_t*)b; 

    if (itemA  &&  itemB)
    {
        ret = itemA->sequence - itemB->sequence;
    }
    else 
    {
        CL_ERRORPRINTF("itemA [%p] itemB [%p]", itemA, itemB);
    }

    return ret;
}

static void fetchSelectionItemDetails(const xmlNodePtr itemNode, mdsSelectionItem_t* item)
{
    int           i;
    xmlChar*      xcp;
    xmlNodePtr    child = NULL;


    CL_LOGPRINTF("entry");

    // get "id" attribute
    xcp = xmlGetProp(itemNode, "id");
    if (xcp == NULL)
    {
        xcp = xmlStrdup("");
    }
    item->id = xcp;

    // get "sequence" attribute
    item->sequence = 0;
    xcp = xmlGetProp(itemNode, "sequence");
    if (xcp)
    {
        i = atoi(xcp);
        if (i >= 0)
        {
            item->sequence = i;
        }
        xmlFree(xcp);
    }

    // get "name" value
    xcp   = NULL;
    child = ermXmlGetChildNode(itemNode, "name");
    if (child)
    {
        xcp = xmlNodeGetContent(child);
    }
    if (xcp == NULL)
    {
        xcp = xmlStrdup("");
    }
    item->name = xcp;

    // get "state" value
    item->state = FALSE;
    child = ermXmlGetChildNode(itemNode, "state");
    if (child)
    {
        xcp = xmlNodeGetContent(child);
        if (xcp)
        {
            if (xmlStrcmp(xcp, "true") == 0)
            {
                item->state = TRUE;
            }
            xmlFree(xcp);
        }
    }
}


int erClXmlSaveSelectionList(const mdsSelectionList_t* selectionList)
{
    int rc;
    int ret = RET_ERR;

    CL_LOGPRINTF("entry");

    if (selectionList == NULL)
    {
        CL_ERRORPRINTF("no selectionList");
        return RET_ERR;
    }
    if (selectionList->num_selections > 0  &&  selectionList->selections == NULL)
    {
        CL_ERRORPRINTF("no selectionList->selections");
        return RET_ERR;
    }

    int  selNum;   // number of selections
    int  selIdx;   // selection index
    int  itemNum;  // number of items
    int  itemIdx;  // item index

    erManifest*               pCtx          = (erManifest*) &(selectionList->manifest);  // const_cast
    xmlNodePtr                nodeRoot      = selectionList->nodeRoot;
    xmlNodePtr                nodeSelection = NULL;
    xmlNodePtr                nodeItemList  = NULL;
    xmlNodePtr                node          = NULL;
    xmlNodePtr                child         = NULL;
    const mdsSelection_t*     selection     = NULL;
    const mdsSelectionItem_t* item          = NULL;

    // for each selection in selectionList
    selNum    = selectionList->num_selections;
    selection = selectionList->selections;
    for (selIdx = 0 ; selIdx < selNum ; selIdx++, selection++)
    {
        nodeSelection = ermXmlGetChildNodeWithAttr(nodeRoot, "selection", "id", selection->id);
        if (nodeSelection == NULL)
        {
            CL_ERRORPRINTF("Node selection id=[%s] not found", selection->id);
            continue;  // next selection
        }

        nodeItemList = ermXmlGetChildNode(nodeSelection, "selection-item-list");
        if (nodeItemList == NULL)
        {
            CL_WARNPRINTF("Node selection-item-list id=[%s] not found", selection->id);
            continue;  // next selection
        }

        // for each item in selection
        itemNum = selection->num_items;
        item    = selection->items;
        for (itemIdx = 0 ; itemIdx < itemNum ; itemIdx++, item++)
        {
            node = ermXmlGetChildNodeWithAttr(nodeItemList, "selection-item", "id", item->id);
            if (node)
            {
                // copy state to xml tree
                child = ermXmlGetChildNode(node, "state");
                if (child)
                {
                    xmlNodeSetContent(child, item->state ? "true" : "false");
                    CL_LOGPRINTF("item id=[%s] state set to [%d] [%s]", item->id, item->state, item->state ? "true" : "false");
                }
                else
                {
                    CL_ERRORPRINTF("Attribute node id=[%s] has no child 'state'", item->id);
                }
            }
            else
            {
                CL_ERRORPRINTF("Node selection-item id=[%s] not found", item->id);
            }
        }
    }

    // save xml tree to manifest file
    rc = ermXmlSave(pCtx);
    if (rc == RET_OK)
    {
        ret = RET_OK;
    }
    else
    {
        CL_ERRORPRINTF("error while saving manifest file");
    }

    return ret;
}

void erClXmlFreeSelectionList(const mdsSelectionList_t* selectionList)
{
    CL_LOGPRINTF("entry");

    erManifest*               pCtx      = (erManifest*) &(selectionList->manifest);  // const_cast
    const mdsSelection_t*     selection = NULL;
    const mdsSelectionItem_t* item      = NULL;

    int  selNum;   // number of selections
    int  selIdx;   // selection index
    int  itemNum;  // number of items
    int  itemIdx;  // item index

    if (selectionList)
    {
        if (selectionList->selections)
        {
            // for each selection in selectionList
            selNum    = selectionList->num_selections;
            selection = selectionList->selections;
            for (selIdx = 0 ; selIdx < selNum ; selIdx++, selection++)
            {
                if (selection->items)
                {
                    // for each item in selection
                    itemNum = selection->num_items;
                    item    = selection->items;
                    for (itemIdx = 0 ; itemIdx < itemNum ; itemIdx++, item++)
                    {
                        if (item->name) { xmlFree(item->name); }
                        if (item->id)   { xmlFree(item->id);   }
                    }

                    g_free((void*)(selection->items));  // const_cast
                }

                if (selection->display_as)  { xmlFree(selection->display_as);  }
                if (selection->title)       { xmlFree(selection->title);       }
                if (selection->instruction) { xmlFree(selection->instruction); }
                if (selection->id)          { xmlFree(selection->id);          }
            }

            g_free((void*)(selectionList->selections));  // const_cast
        }

        ermXmlClose(pCtx);
        g_free((void*)selectionList);  // const_cast
    }
}

int erClXmlSetSelectionItem( const mdsSelectionList_t* selectionList,
                             const guint selectionIndex,
                             const guint itemIndex,
                             const gboolean state )
{
    // check selection list
    if (selectionList == NULL  ||  selectionList->selections == NULL)
    {
        CL_ERRORPRINTF( "Invalid selectionList [%p] selections [%p]",
                        selectionList,
                        selectionList ? selectionList->selections : NULL );
        return RET_ERR;
    }
    if (selectionIndex >= selectionList->num_selections)
    {
        CL_ERRORPRINTF( "Invalid selectionIndex [%u] num_selections [%u]",
                        selectionIndex,
                        selectionList->num_selections );
        return RET_ERR;
    }
    
    // find selection
    mdsSelection_t* selection = (mdsSelection_t*) &(selectionList->selections[selectionIndex]);  // const_cast

    // check selection
    if (itemIndex >= selection->num_items)
    {
        CL_ERRORPRINTF( "Invalid itemIndex [%u] num_items [%u]",
                        itemIndex,
                        selection->num_items );
        return RET_ERR;
    }

    // get item
    mdsSelectionItem_t* item = (mdsSelectionItem_t*) &(selection->items[itemIndex]);  // const_cast

    // set item state
    item->state = state;

    return RET_OK;
}

int erClXmlGetSelectionListItemState( const mdsSelectionList_t* selectionList,
                                      const char* selectionId,
                                      const char* itemId,
                                      gboolean*   state  )
{
    gboolean  found = FALSE;

    const mdsSelection_t*     selection;
    int                       sel_idx;
    const mdsSelectionItem_t* item;
    int                       item_idx;

    // check selection list
    if (selectionList == NULL  ||  selectionList->selections == NULL)
    {
        CL_ERRORPRINTF( "Invalid selectionList [%p] selections [%p]",
                        selectionList,
                        selectionList ? selectionList->selections : NULL );
        return RET_ERR;
    }
    
    // find selection "copy or move"
    selection = selectionList->selections;
    for (sel_idx = 0 ; sel_idx < selectionList->num_selections ; sel_idx++, selection++)
    {
        if (strcmp(selection->id, selectionId) == 0)
        {
            // selection found: find item "move"
            item = selection->items;
            for (item_idx = 0 ; item_idx < selection->num_items ; item_idx++, item++)
            {
                if (strcmp(item->id, itemId) == 0)
                {
                    // item found: use as return value
                    found = TRUE;
                    if (item->state)
                    {
                        *state = TRUE;
                    }
                    else
                    {
                        *state = FALSE;
                    }

                    // exit for item_idx
                    item_idx = selection->num_items;
                }
            }

            // exit for sel_idx
            sel_idx = selectionList->num_selections;
        }
    }

    if (found)
    {
        CL_LOGPRINTF("Found: selection [%s] item [%s] state [%d]", selectionId, itemId, *state);
        return RET_OK;
    }
    else
    {
        CL_WARNPRINTF("Not found: selection [%s] item [%s] state [%d]", selectionId, itemId, *state);
        return RET_ERR;
    }
}

