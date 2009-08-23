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
 * \file pagebar.h
 * \brief browser - pagebar interface with browser application and page handling routines
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __EREADER_PAGEBAR_H__
#define __EREADER_PAGEBAR_H__

/**
 * Open communication channel with pageBar deamon
 * counts on the fact that pageBar is running
 *
 * @returns void
 */
void pagebar_init();

/**
 * Request from browser to goto page
 *
 * @param page - page to go to
 *
 * @returns -
 */
void pagebar_goto_page(int page);

/**
 * Request from browser to set number of pages
 *
 * @param count - number of pages in current view
 *
 * @returns -
 */
void pagebar_set_pagecount(int count);

/**
 * Request from browser to set (scroll)offset of current page
 *
 * @param offset - scrolled offset in current page
 *
 * @returns -
 */
void pagebar_set_offset(int offset);

/**
 * Request from contentLister to redraw pagebar
 *
 * @param - 
 *
 * @returns -
 */
void pagebar_redraw();


/**
 * THIS NEEDS A PROPER DESCRIPTION
 *
 * @param pagenumber the newly selected page bar page
 * @param Ereader datastructure containing the "mozembed" reference and the current page status
 *
 * @returns void
 */
void browser_pagebar_page_selected(int pagenumber, Ereader *browser);

/**
 * creation and initalistaion of the BrowserPageStatus structure
 *
 * @returns reference to a newly allocated BrowserPageStatus structure
 */
BrowserPageStatus* browser_create_page_status(void);

/**
 * free memory used by pageStatus and lcose the currenly used indexfile
 *
 * @param pageStatus reference to struct containing current browser page status
 * 
 */
void browser_destroy_page_status(BrowserPageStatus* pageStatus);

/**
 * open the specified index file and update the reference in the pageStatus
 * (closes the currently selected index file)
 *
 * @param pageStatus reference to struct containing current browser page status
 * @param indexfile  index file name
 *
 * @returns TRUE when file opened
 */
gboolean browser_set_page_status_indexfile(BrowserPageStatus* pageStatus, char* indexfile);

/**
 * updates the pageStatus when the pageType differs from current pageStatus pagetype
 * return TRUE when the type was changed, FALSE when no type update was needed
 *
 * @param pageStatus reference to struct containing current browser page status
 * @param pageType  the page type
 *
 * @returns TRUE when file opened
 */
gboolean page_status_handle_type_update(BrowserPageStatus* pageStatus, gchar* pageType);



#endif //__EREADER_PAGEBAR_H__




