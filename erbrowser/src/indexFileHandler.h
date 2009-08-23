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
 * \file indexFilehandler.h
 * \brief browser - eReader index file (= manifest file) related API            
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __INDEXFILEHANDLER_H__
#define __INDEXFILEHANDLER_H__

#define INVALID_PAGE_COUNT		(-1)
#define INVALID_PAGE_NMBR		(-1)
#define DEFAULT_PAGE_COUNT		1
#define DEFAULT_PAGE			1
#define DEFAULT_PAGE_OFFSET		100
#define INVALID_PAGE_TYPE		"unknown"
#define INIT_PAGE_TYPE			"init"
#define ONE_PAGE_TYPE			"one_page"

typedef struct _PageTypeInfo {
  int    searchedCount;
  int	 currentCount;	  				   
  char*  searchedType;
  char*  currentType;
} PageTypeInfo;

typedef struct _PageNmbrInfo {
  int    searchedNmbr;
  int	 currentNmbr;	  				   
  char*  searchedPage;
  char*  currentPage;	
} PageNmbrInfo;

typedef struct _PageUrlInfo {
  char*  searchedPage;
  char*  currentPage;	
  char*  searchedType;
  char*  currentType;
  int    searchedNmbr;
  int	 currentNmbr;	  				   
} PageUrlInfo;

/*
Current manifest index file
<pagelist type="paper" count="3">
    <page number="1" url="001/index.html"/>
    <page number="2" url="002/index.html"/>
    <page number="3" url="003/index.html"/>
</pagelist>

<pagelist type="layout" count="3">
    <page number="1" url="001/overview.html"/>
    <page number="2" url="002/overview.html"/>
    <page number="3" url="003/overview.html"/>
</pagelist>

<pagelist type="articles" count="3">
    <page number="1" url="001/summary.html"/>
    <page number="2" url="002/summary.html"/>
    <page number="3" url="003/summary.html"/>
</pagelist>
*/

typedef struct _PageInfo {
    int     number;
    char *  url;
}PageInfo;

typedef struct _pagelistItem {
    char *      type;
    int         count;
    PageInfo ** page;
} PageListItem;

typedef struct _IndexInfo {
    PageListItem ** pagelist;
    int             length;
}IndexInfo;


/**
 * Close the index/manifest file
 *
 * @param IndexInfo pointer
 *
 * @returns -
 */
void index_file_close(IndexInfo *index);

/**
 * Open the index/manifest file
 *
 * @param file name
 *
 * @returns non zero in case of error
 */
int index_file_init(const char* file, IndexInfo* index);

/**
 * Retrieve the page count from the index file, given the page type
 *
 * @param FILE reference to the current inde file
 * @param type page type
 *
 * @returns pagecount (DEFAULT_PAGE_COUNT in case of error)
 */
int index_file_get_page_count(IndexInfo * index, const char* type);

/**
 * Retrieve the page number from the index file, given the page type and the url
 *
 * @param IndexInfo reference to the current index struct
 * @param type page type
 * @param url  page url
 *
 * @returns pagenumber 
 */
int index_file_get_page_number(IndexInfo * index, const char* type, const char* url); 
 
 /**
 * Retrieve the page number from the index file, given the page type and the url
 *
 * @param IndexInfo reference to the current inde file
 * @param type page type
 * @param number page number
 *
 * @returns url (allocated memory should be freed by the calling routine)
 */
char* index_file_get_page_location(IndexInfo * index, const char* type, const int number); 

/**
 * Dump IndexInfo information, for debug
 *  
 * @param IndexInfo reference to the current inde file
 *
 * @returns void
 */
void dump(IndexInfo *index);
 
#endif //__INDEXFILEHANDLER_H__



