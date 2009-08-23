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
 * \file pagebar.h
 * \brief contentLister - pageBar deamon interaction with contentLister application
 *
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __EREADER_PAGEBAR_H__
#define __EREADER_PAGEBAR_H__

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

/**
 * Open communication channel with pageBar deamon
 * counts on the fact that pageBar is running
 *
 * @returns void
 */
void pagebar_init();

/**
 * Resets the internal pageBar variables to their defaults
 * also tells the pageBar that the contentLister is now currentApp
 *
 * @returns void
 */
void pagebar_reset();

/**
 * Request from contentLister to goto page
 *
 * @param page - page to go to
 *
 * @returns -
 */
void pagebar_goto_page(int page);

/**
 * Request from contentLister to set number of pages
 *
 * @param count - number of pages in current view
 *
 * @returns -
 */
void pagebar_set_pagecount(int count);

/**
 * Request from contentLister to redraw pagebar
 *
 * @param - 
 *
 * @returns -
 */
void pagebar_redraw();

/**
 * Request from contentLister to start progress timer
 *
 * @param - 
 *
 * @returns -
 */
void pagebar_startTimer();

/**
 * Request from contentLister to start progress timer
 *
 * @param - 
 *
 * @returns -
 */
void pagebar_stopTimer();







#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__EREADER_PAGEBAR_H__




