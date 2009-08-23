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
 *
 */

#ifndef __CL_HISTORY__H__
#define __CL_HISTORY__H__

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    
#define MAX_RECENT_ITEMS     12          // max. number of items in recent-documents list
#define MAX_DOWNLOAD_ITEMS   12         // max. number of items in download-history


/**
 * Allocate memory to store one history item for the available archive 
 * content types and retrieve the history values from the registry
 *
 * @returns - 
 */
void hsInit(void);

/**
 * Store the history information in the registry and 
 * destroy allocated resources used by the history module
 *
 * @returns - 
 */
void hsDestroy(void);

/**
 * Store (copy) the content of the item in the history of the specified 
 * content type
 *
 * @param contentType 
 * @param item - details of the currently "open item"
 *
 * @returns - 
 */
void hsStoreItem(st_ContentType_e contentType, clDisplayItem_t* item);

/**
 * retrieve a copy of the history item of the specified content type
 *
 * @param contentType 
 * @param item - reference to memory where the copy should be returned
 *
 * @returns - 
 */
void hsGetItem(st_ContentType_e contentType, clDisplayItem_t* item);

/**
 * Set the item location information in the history data (RAM)
 * Thread safe
 * 
 * @param contentType 
 * @param location current viewer location of the current history item of this contentType
 *
 * @returns FALSE in case of failure
 */
gboolean hsSetLocation(st_ContentType_e contentType , char* location);

/**
 * Get the item location information from the history data (RAM)
 * 
 * @param contentType 
 *
 * @returns last viewer location of this item, NULL when not available
 *          (this string value should be freed by the calling routine !!)   
 *
 */
char* hsGetLocation(st_ContentType_e contentType);

/**
 * Remove the item location information from the history data (RAM)
 *
 * @param contentType 
*
 * @returns FALSE when not available
 */
gboolean hsRemoveLocation(st_ContentType_e contentType);

/**
 * Store the active viewer for the specified contentType
 * and clear viewer ID if used for another contentType.
 * 
 * @param contentType 
 * @param viewerId unique identification of the viewere
 *
 */
void hsSetActiveViewer(st_ContentType_e contentType , char* viewerId);

/**
 * Get the active viewer for the specified contentType
 * 
 * @param contentType 
 *
 * @returns viewerId  " " or NULL in case no viewer 
 *
 */
char* hsGetActiveViewer(st_ContentType_e contentType);

/**
 * Get the contentType associated with the "active viewer"
 * 
 * @param contentType 
 *
 * @returns viewerId   
 */
st_ContentType_e hsGetContentType(char* viewerid);

/**
 * Remove symlinks for which the target is not present
 * 
 * @param szTargetDir directory to scan for symlinks
 *
 * @returns --
 */
void hsCleanupSymlinks(const char* szLinkDir);

/**
 * Limit symlinks to the specified number, removing symlinks with "lowest" names
 *
 * @param szTargetDir directory to scan for symlinks
 * @param maxItems maximum number of symlinks allowed
 *
 * @returns --
 */
void hsLimitSymlinks(const char* szLinkDir, int maxItems);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  //__CL_HISTORY__H__
