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
 * \file metadataStoreManager.h
 * \brief metadataStoreManager - manage the metadata store                  
 * 
 * <File description>
 *
 * The data retrieved from the meta store is copied into an internal data format 
 * (dsListItem_t) used to render the information  on the screen 
 *  
 * "MAX_ITEMS_ON_ONE_PAGE" is a predefined value indicating the amount of
 * meta store items that are displayed on one content lister page
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __METADATA_STORE_MANAGER__H__
#define __METADATA_STORE_MANAGER__H__

#include "stack.h" 

#ifdef __cplusplus
extern  "C"
{
#endif /* __cplusplus */

/**
 * Init stack (for every content key) 
 * Init storage used for content history
 * 
 * @param - 
 *
 * @returns -
*/
void mdsInit(void);

/**
 * Destroy memory allocated by this module
 * 
 * @param - 
 *
 * @returns -
*/
void mdsDestroy(void);

/**
 * Scan the root directory 
 * 
 * @param contentType indication of the content category
 *
 * @returns error code in case of failure
*/
int mdsScanRoot(st_ContentType_e contentType);

/**
 * Scan using the current "content details" information 
 * if not available use the "root" values 
 * 
 * @param contentType indication of the content category
 *
 * @returns error code in case of failure
*/
int mdsScanCurrent(st_ContentType_e contentType);

/**
 * Scan using the current "content details" information  
 * but with this new directory data. And store these
 * new "content details" for this contentType on the stack.
 * 
 * @param contentType indication of the content category
 * @param folder directory information
 * @param index currently selected index
 *
 * @returns error code in case of failure
*/
int mdsScanNext(st_ContentType_e contentType, char* directory, unsigned int theIndex);

/**
 * Remove the current "content details" from the stack and
 * scan using the new current "content details" information.  
 * 
 * if not available use the "root" values /home/ann/eReader/svn/eReader/content/
 * 
 * @param contentType indication of the content category
 *
 * @returns error code in case of failure
*/
int mdsScanPrevious(st_ContentType_e contentType);


/**
 * Update the index field of the current top stack item
 *
 */
void mdsSetIndex(st_ContentType_e contentType, unsigned int index);

/**
 * Retrieve the index field of the current top stack item
 *
 */
unsigned int mdsGetIndex(st_ContentType_e contentType);


/**
 * Retrieve the root location of specified content type
 *
 */
int mdsGetRootLocation(const st_ContentType_e contentType, char * location, const int length);



int mdsGetItemCount(void);
int mdsGetItemIndex(const gchar* szFilename);
gboolean mdsGetItemFilename(int index, gchar* szFilename, int len);
int mdsSetPageContent(clDisplayItem_t* displayItemArray, int nBegin, int *nChunk);

mdsSortStruct_t* mdsGetSortStructOfRootInfo(st_ContentType_e type);
void mdsSetSortStructOfRootInfo(st_ContentType_e type, const mdsSortStruct_t* sort);

gchar* getContentCategoryID(st_ContentType_e type);

char* getFieldString(mdsFieldType_e type);
mdsFieldType_e getFieldType(char *stringValue);

gboolean storageIsMounted(st_ContentType_e storageType);
gboolean mdsArcLocIsChanged(st_ContentType_e archiveType, st_ContentType_e storageType);
void mdsRecreateArchives(st_ContentType_e storageType);

//////////////////////////////////////// Testing rountines /////////////////////////////////////////////////////
void displayScanResult(void);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          //__METADATA_STORE_MANAGER__H__
