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
 * \file stack.h
 * \brief - eReader "content keys (newspaper, book, notes, memory extensions...) history handling
 *
 *          For every content key (which is associated with a ContentType) a stack is
 *          kept to keep track of the browsing through the content for the different
 *          content types 
 *
 * <File description>
 * 
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
 
#ifndef __CL_STACK__H__
#define __CL_STACK__H__

#include <glib.h>
#include "erMdsContent.h" 

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

typedef enum
{
                                // Content types linked to the iLiad archive keys, these need a history stack
    st_ContentTypeBooks = 0,
    st_ContentTypeNews,
    st_ContentTypeDocs,
    st_ContentTypeNotes,
                       st_ARCHIVES_COUNT,
                                // Storage types which do not require a history stack
    st_StorageTypeCF = st_ARCHIVES_COUNT,
                                //   compact-flash card
    st_StorageTypeSD,           //   SD card / MMC card
    st_StorageTypeUSB,          //   USB stick or USB hard-disk
    st_StorageTypeMain,         //   main memory
    st_MenuTypeMode,            //   MODE menu/directory (doesn't need a stack for the moment, but does have a root directory)             
    st_DownloadHistory,         //   The download history directory (doesn't need a stack for the moment, but does have a root directory)
    st_SearchResult,            //   The result of search
    st_RecentDocuments,         //   The recent documents directory (doesn't need a stack for the moment, but does have a root directory)
    st_ContentTypeUndefined             
} st_ContentType_e;

#define ARCHIVE_TYPE_COUNT                      st_ARCHIVES_COUNT  //number of archive types for which a history stack is created
#define TOTAL_STACK_COUNT                       st_ContentTypeUndefined
#define INITIAL_STACK_ARRAY_SIZE                20
#define INITIAL_DIR_COUNT_ARRAY_SIZE            3
#define INITIAL_SEARCH_COUNT_ARRAY_SIZE         3

 
typedef struct {
	GArray          *dirArray;      //array (fixed max string sizes) of directories that was scanned   
	mdsSortStruct_t sort;
	unsigned int    index;          //index of the selected item (within the scan result)
} stItem_t;  

/**
 * Destroy the stack of every content key
 */
void stackDestroyAll(void); 

/**
 * Create an empty stack for every content key (=st_ContentType_e)
 */
void stackInitAll(void); 

/**
 * Creates a copy of the stackItem and puts this on the stack
 * The referenced dirArray and searchArray data is not copied, only
 * the reference is. So DO NOT use "stackItemDestroy" on an 
 * item of which the referenced data is still valid on the stack !!
 */
void stackPush(st_ContentType_e contentType,  stItem_t* stackItem); 

/**
 * Removes the last added item from the stack
 */
void stackPop(st_ContentType_e contentType);

/**
 * Returns reference to top stack item or NULL when not 
 * available
 *
 */
stItem_t* stackPeek(st_ContentType_e contentType);

/**
 * Returns # items on the stack
 */
int stackHeight(st_ContentType_e contentType);

/**
* Clear the stack, by removing all items and freeing the referenced 
* dirArray and searchArray data used by every item.
*/
void stackClear(st_ContentType_e contentType);

/**
* Free the referenced dirArray and searchArray data 
* and the rest of the item. 
*/
void stackItemDestroy(stItem_t* stackItem);

/**
* Display item content and referenced content including address information
*/
void stackItemDisplay(stItem_t* stackItem);

/**
* Display stack
*/
void stackDisplay(st_ContentType_e contentType);

//////////////////////////////////////// Utilities /////////////////////////////////////////////////////

// check if the specified contentType is a storage type
gboolean stackIsStorage(st_ContentType_e contentType);

// check if the specified contentType is a archive type
gboolean stackIsArchive(st_ContentType_e contentType);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__CL_STACK__H__

