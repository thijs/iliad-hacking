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
 * \file selection.h
 * \brief browser - API to identify a selection in an e-reader XHTML file and recreate this selection
 *                  when needed.
 *
 *
 *  Restrictions/Remark : - the sequence of the selections in the selection file should be respected
 *                        - selections are stored in a selection file which is located in the same directory as the 
 *                          original XHTML file
 *                        - selections are put in the actual XHTML file by surrounding the TEXT tags with a <Higlight> tag.
 *                          The actual result is determined by the style associated with this <Highlight> tag in the CSS file
 *                        - higlighting should not affect font or other size related things since higlights may not affect 
 *                          the actual page layout. 
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __SELECTION_H__
#define __SELECTION_H__

// used to enable/disable the selection/highligth feature
//#define SELECTION_ENABLED

#ifdef SELECTION_ENABLED
/**
 * Identify in the selected XHTML text a list of selections and 
 * store these is the appropriate selection file
 *
 * @param mozEmbed reference to mozembed widget
 *
 * @returns TRUE on succes
 */
bool selection_identify_and_store_selection(GtkMozEmbed* mozEmbed);

/**
 * Retrieve the associated selection file content and update the 
 * currently displayed XHTML page  
 *
 * @param mozEmbed reference to mozembed widget
 *
 * @returns TRUE on succes
 */
bool selection_set_current_page_selections(GtkMozEmbed* mozEmbed);

/**
 *  Remove the associated selection file, if available 
 *  and reload the XHTML page    
 *
 * @param mozEmbed reference to mozembed widget
 *
 * @returns TRUE on succes
 */
bool selection_clear_current_page_selections(GtkMozEmbed* mozEmbed);

#endif // SELECTION_ENABLED

#endif //__SELECTION_H__



