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
 * \file selectionFilehandler.h
 * \brief browser - selection file handling routines
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __SELECTIONFILEHANDLER_H__
#define __SELECTIONFILEHANDLER_H__

#ifdef SELECTION_ENABLED

#define MAX_FILE_NAME   256
#define MAX_TAG_NAME    128

typedef struct _SelectionID {
  char parentNodeTagName[MAX_TAG_NAME];
  unsigned int parentNodeIndex;
  unsigned int nodeIndex;
  unsigned int selectionStart;
  unsigned int selectionLength; 
} SelectionID;

/**
 * Open or re-open a selection file associated with the current HTML page
 *
 * @param mozEmbed reference to mozembed widget
 *
 * @returns refercence to a selection file on succes

 */
FILE* selection_file_init(GtkMozEmbed *b);

/**
 * Check if there is a selection file associated with this HTML page
 * and destroy it 
 *
 * @param mozEmbed reference to mozembed widget
 *
 * @returns refercence to a selection file on succes

 */
bool selection_file_destroy(GtkMozEmbed *b);

/**
 * Store a new selection in the selection file
 *
 * @param selectionFile reference to the selection file
 * @param theSelection list of selections
 *
 * @returns TRUE on succes

 */
bool selection_file_store(FILE* selectionFile, SelectionID *theSelection);

/**
 * Close the selection file
 *
 * @param selectionFile reference to the selection file
 *
 * @returns TRUE on succes

 */	
bool selection_file_close(FILE* selectionFile);

/**
 * Check if there is a selection file associated with the current HTML page
 *
 * @param mozEmbed reference to mozembed widget
 *
 * @returns TRUE if available

 */	
bool selection_file_available(GtkMozEmbed *b);

/**
 * retrieve the list of currenly available selections in the selection file
 * the memory allocated for this list should be free by the calling routine
 * The sequence of the selections should be respected !!!
 *
 * @param selectionFile reference to the selection file
 *
 * @returns list of selections

 */	
GSList* selection_file_get_list(FILE* selectionFile);

#endif //SELECTION_ENABLED

#endif //__SELECTIONFILEHANDLER_H__



