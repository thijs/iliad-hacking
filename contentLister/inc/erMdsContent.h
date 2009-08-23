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
 * \file erMdsContent.h
 * \brief eReader Content Lister content and filesystem datatypes
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2006-2008 iRex Technologies B.V.
 *
 */
#ifndef __ERMDSCONTENT__H__
#define __ERMDSCONTENT__H__

#include <glib.h>

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */


// name of the outbox folder on each device
#define OUTBOX_NAME                     "outbox"

// XML root node for selection lists
#define XMLNODE_SELECTION_LIST  "/package/metadata/selection-list"

// name of special selection lists and items
#define SELECTION_TRANSFER_MODE         "transfer-mode"
#define ITEM_TRANSFER_MOVE              "move"

#define SELECTION_DOCUMENT_SIGNING      "document-signing"
#define ITEM_SIGN_YES                   "sign"


#define ERMDS_MAX_FILENAME_SIZE 1024
#define ERMDS_MAX_EXTENSION_SIZE 5
#define ERMDS_MAX_SEARCH_STRING_SIZE 512
#define ERMDS_MAX_EXTENSION_ID_SIZE 128

#define ERMDS_MAX_SYMLINK_CHAIN 10

#define ERCL_MAX_TITLE_SIZE 512
#define ERCL_MAX_SUBTITLE_SIZE 512
#define ERCL_MAX_DESCRIPTION_SIZE 512

#define ERCL_MAX_FIELDNAME_SIZE 128

#define ERMDS_INITIAL_CONTENT_SCAN_ARRAY_SIZE 100
#define ERMDS_MAX_CONTENT_SCAN_ARRAY_SIZE (1024-1) // Always take a power of two minus 1

// Return values erMdsContentScan
#define ERMDS_CONTENT_SCAN_OK 0
#define ERMDS_CONTENT_SCAN_FAILED -1
#define ERMDS_CONTENT_SCAN_TOO_MANY_ITEMS -2

// Return values erMdsContentSort
#define ERMDS_CONTENT_SORT_OK 0 
#define ERMDS_CONTENT_SORT_FAILED -1
#define ERMDS_CONTENT_SORT_NOT_IMPLEMENTED -2

// Additional Return values
#define ERCL_OK 0
#define ERCL_INVALID_ARGS -10
#define ERCL_NOMATCH -11

#define ERCL_INVALID_SIZE -1

// name of the Mobipocket folder (on each device)
// make sure the length is accurate when changing this
#define MOBIPOCKET_FOLDER "/books/Mobipocket"
#define MOBIPOCKET_FOLDER_LENGTH 17

// Sort priority of Mobipocket folder
#define MOBIPOCKET_FOLDER_SORT_PRIORITY 10

// Define Enumerated types
typedef enum
{
    mdsFitFile = 0,
    mdsFitContainer,
    mdsFitFolder,
    mdsFitStorage,               //extension to the manifest file
    mdsFitApplication,           //extension to the manifest file
    mdsFitManifestDirectory,     //extension to the manifest file
    mdsFitSymlink,
    mdsFitManifestSymlink,       //extension to the manifest file
    mdsFitManifestSymlinkLocale, //extension to the manifest file
    mdsFitUndefined
} mdsFsItemType_e;

typedef enum
{
    mdsFieldAuthor = 0,
    mdsFieldDate,
    mdsFieldDescription,
    mdsFieldFile,
    mdsFieldTitle,
    mdsFieldFsName, // Corresponds to the name of the file/folder/container. Not necessarily to
                    // the filename that will be passed to the viewer.
    mdsFieldExtName,
    mdsFieldSize,
    mdsFieldNone
} mdsFieldType_e;

typedef enum 
{
    clFolderIcon = 0,
    clUnknownIcon,
    clBookIcon,
    clDocumentIcon,
    clMagazineIcon,
    clMiscIcon,
    clMp3Icon,
    clNewspaperIcon,
    clNoteIcon,
    clPhotoIcon,
    clReviewIcon,
    clOutboxIcon,
    clIconUrlDefined,   // icon specified in manifest file
} clIconID_e;

#define ICON_COUNT      clIconUrlDefined        // number of icons

typedef enum
{
   mdsSingleSel_e = 0,
   mdsMultiSel_e,
   mdsSelUndefined
}mdsSelType_e;

// Define structs
typedef struct
{
    mdsFieldType_e ft;
    int     ascending;
} mdsSortStruct_t;

typedef struct
{
    char    szFilename[ERMDS_MAX_FILENAME_SIZE];
    mdsFsItemType_e fit;
    char    *sortField;
    int     priority; // Now only used to sort Mobipocket on top
} mdsFsItem_t;

typedef struct 
{
    clIconID_e iconID;
    char clIconURL[ERMDS_MAX_FILENAME_SIZE];
    char szFileExt[ERMDS_MAX_FILENAME_SIZE];
    char szTitle[ERCL_MAX_TITLE_SIZE];
    char szSubTitle[ERCL_MAX_SUBTITLE_SIZE];
    char szDate[ERCL_MAX_SUBTITLE_SIZE];
    char szDescription[ERCL_MAX_DESCRIPTION_SIZE];
    mdsFsItemType_e fit;
    char szFilename[ERMDS_MAX_FILENAME_SIZE];
    char szManifest[ERMDS_MAX_FILENAME_SIZE];
    char szExtID[ERMDS_MAX_EXTENSION_ID_SIZE];
    int size;  // file or container size in byte, free space in kilobyte
    struct 
    {
        gboolean bDefault;
        gboolean bDelete;
        gboolean bScribble;
        gboolean bTagging;
    } modifyEnable;
} clDisplayItem_t;

typedef struct
{
    char szFilename[ERMDS_MAX_FILENAME_SIZE];
} mdsDirectory_t;


// selection list data from manifest file
typedef struct
{
    // public read-only
    xmlChar*            name;

    // private
    xmlChar*            id;
    guint               sequence;
    gboolean            state;
} mdsSelectionItem_t;

typedef struct
{
    // public read-only
    gboolean            constant;
    gboolean            hide;
    xmlChar*            display_as;
    xmlChar*            title;
    xmlChar*            instruction;
    guint               min_selected;
    guint               max_selected;
    guint               num_items;
    const mdsSelectionItem_t* items;

    // private
    xmlChar*            id;
    guint               sequence;
} mdsSelection_t;

typedef struct
{
    // public read-only
    guint               num_selections;
    const mdsSelection_t* selections;

    // private
    erManifest          manifest;
    xmlNodePtr          nodeRoot;      // root node of the selection-list tree
} mdsSelectionList_t;


/**
 * Scan an array of directories for content items that match search criteria. 
 *
 * @param dirArray An array of directories that will be scanned
 * @param contentItemArray Will contain retrieved content items. Pass NULL when you want the function to allocate memory
 *
 * @returns one of the following errorcodes: ERMDS_CONTENT_SCAN_OK, ERMDS_CONTENT_SCAN_FAILED, ERMDS_CONTENT_SCAN_TOO_MANY_ITEMS
 */

int erMdsContentScan(GArray * dirArray, GArray ** contentItemArray);

/**
 * Sort the array passed as an argument according to the sort criterium.
 *
 * @param contentItemArray - Contain items to be sorted
 * @param sort -
 *
 * @returns one of the following errorcodes: ERMDS_CONTENT_SORT_OK, ERMDS_CONTENT_SORT_FAILED
 */

int erMdsContentSort(GArray * contentItemArray, mdsSortStruct_t * sort);

// Return values of erClGetFileType()
#define GFT_IGNORE    -1
#define GFT_NOSUCHFILE 0
#define GFT_DIR        1
#define GFT_FILE       2
#define GFT_SYMLINK    3
// int erClGetFileType(const char *szPath);
// int erClGetSymlinkInfo(const char* szRealpath, mdsFsItemType_e* pFit);
int erClGetDisplayItems(GArray * contentItemArray, int nBegin, int *nChunk, clDisplayItem_t * displayItemArray);

/* 
 * @param szFilename
 *
 * @returns 0 upon success 
 */
int erClXmlParseManifest(char *szContainerPath, clDisplayItem_t * displayItem);

void erClXmlFetchField(char *szFilename, mdsFieldType_e fieldType, char *szField);

gboolean erClXmlSetFieldSize(const gchar* szContainer, int size);


/**
 * Get list of selections from manifest file
 *
 * @param szManifestPath - pathname of manifest file
 *
 * @returns ptr to s selection-list structure, must be freed with erClXmlFreeSelectionList()
 */
const mdsSelectionList_t* erClXmlFetchSelectionList(const char* szManifestPath);

/**
 * Save list of selections to manifest file
 *
 * @param selectionList - as returned by erClXmlFetchSelectionList
 *
 * @returns RET_OK or RET_ERR 
 */
int erClXmlSaveSelectionList(const mdsSelectionList_t* selectionList);

/**
 * Free data structure for selections
 *
 * @param selectionList - as returned by erClXmlFetchSelectionList
 *
 * @returns --
 */
void erClXmlFreeSelectionList(const mdsSelectionList_t* selectionList);

/**
 * Set specified in item in selection list to true/false
 *
 * @param selectionList - as returned by erClXmlFetchSelectionList
 * @param selection_index - requested selection as index in mdsSelection_t
 * @param item_index - requested item as index in mdsSelection_t[].items
 * @param state - new state of this item
 *
 * @returns RET_OK or RET_ERR
 */
int erClXmlSetSelectionItem( const mdsSelectionList_t* selectionList,
                             const guint selectionIndex,
                             const guint itemIndex,
                             const gboolean state );

/**
 * Get current value (state) of specified selection.item
 *
 * @param selectionList - as returned by erClXmlFetchSelectionList
 * @param selectionId - id= string of the requested selection
 * @param itemId - id= string of the requested item
 * @param state - ptr to location where to store current state value
 *
 * @returns RET_OK or RET_ERR
 */
int erClXmlGetSelectionListItemState( const mdsSelectionList_t* selectionList,
                                      const char* selectionId,
                                      const char* itemId,
                                      gboolean*   state  );

/*
 * Check whether the specified path leads to a device trusted for user documents.
 * This is a security measure to not allow the contentLister to read unauthorised directories.
 *
 * @param path
 *
 * @returns device mountpoint or NULL if fail
 */
const gchar* ctrl_is_trusted_path(const char* path);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  // __ERMDSCONTENT__H__
