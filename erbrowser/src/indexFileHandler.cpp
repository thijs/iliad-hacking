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
 * \file indexFilehandler.cpp
 * \brief browser - eReader index file related API            
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#include <stdio.h>
#include "expat.h"
#include "browserTypes.h"
#include "indexFileHandler.h"
#include "browserLog.h"
#include <libermanifest/ermanifest.h>

#define BUFFER_SIZE		256
#define MAX_LEN         1024
#define INDEX_DEBUG     0

#if (INDEX_DEBUG)
#define TRACE(x, args...) fprintf(stderr, "(BR_TRACE)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define TRACE(x...) do {} while(0)
#endif

#if 0
// internally used routines
static int url_compare(char *abs_url1, char *url2);

static XML_Parser parser_create(void);
static void parser_destroy(XML_Parser p);

static void XMLCALL page_list_start_element(void *userData, const char *name, const char **atts);
static void XMLCALL page_list_end_element(void *userData, const char *name);

static void XMLCALL page_nmbr_start_element(void *userData, const char *name, const char **atts);
static void XMLCALL page_nmbr_end_element(void *userData, const char *name);

static void XMLCALL page_url_start_element(void *userData, const char *name, const char **atts);
static void XMLCALL page_url_end_element(void *userData, const char *name);

static XML_Parser parser_create(void)
{
    XML_Parser p = XML_ParserCreate(NULL);

    if (!p)
    {
        BR_ERRORPRINTF("Couldn't allocate memory for parser");
    }

    return p;
}

static void parser_destroy(XML_Parser p)
{
    // Free memory used by the parser
    XML_ParserFree(p);
}

void index_file_close(FILE * indexFile)
{
    if (indexFile != NULL)
    {
        BR_MANIFESTPRINTF("0x%x", (unsigned int) indexFile);
        fclose(indexFile);
        indexFile = NULL;
    }
}

FILE   *index_file_init(char *file)
{
    FILE   *indexFile = NULL;

    BR_MANIFESTPRINTF("location %s", file);

    if ((indexFile = fopen(file, "r")) == NULL)
    {
        BR_WARNPRINTF("Couldn't open %s", file);
    }
    else
    {
        BR_MANIFESTPRINTF("index_file_init 0x%x", (unsigned int) indexFile);
    }

    return indexFile;
}


//****************************************************************************************
//
// Retrieve the page url from the index file, given the page type and number
//
//*************************************************************************************** 
char   *index_file_get_page_location(FILE * indexFile, char *type, int number)
{
    XML_Parser theParser;
    char    buf[BUFFER_SIZE];
    int     done;
    char   *returnValue = NULL;
    PageUrlInfo *pageUrlInfo;

    BR_MANIFESTPRINTF("index_file_get_page_location file 0x%x type %s url %d", indexFile, type, number);

    if (type && indexFile)
    {
        theParser = parser_create();
        pageUrlInfo = g_new0(PageUrlInfo, 1);

        // reset the search variables
        pageUrlInfo->searchedNmbr = number;
        pageUrlInfo->currentNmbr = INVALID_PAGE_NMBR;

        pageUrlInfo->searchedType = type;
        pageUrlInfo->currentType = NULL;

        pageUrlInfo->searchedPage = NULL;
        pageUrlInfo->currentPage = NULL;

        XML_SetUserData(theParser, pageUrlInfo);
        XML_SetElementHandler(theParser, page_url_start_element, page_url_end_element);

        rewind(indexFile);

        while ((!feof(indexFile)) && (pageUrlInfo->searchedPage == NULL))
        {
            size_t  len = fread(buf, 1, sizeof(buf), indexFile);

            done = len < sizeof(buf);
            if (XML_Parse(theParser, buf, len, done) == XML_STATUS_ERROR)
            {
                BR_MANIFESTPRINTF("%s at line %d", XML_ErrorString(XML_GetErrorCode(theParser)), XML_GetCurrentLineNumber(theParser));
                return NULL;
            }
        }

        returnValue = pageUrlInfo->searchedPage;

        // clean up allocated memory
        if (pageUrlInfo->currentType)
        {
            g_free(pageUrlInfo->currentType);
            pageUrlInfo->currentType = NULL;
        }

        if (pageUrlInfo->currentPage)
        {
            g_free(pageUrlInfo->currentPage);
            pageUrlInfo->currentPage = NULL;
        }
        g_free(pageUrlInfo);

        parser_destroy(theParser);
    }
    return returnValue;
}

static void XMLCALL page_url_start_element(void *userData, const char *name, const char **attr)
{
    gint    result = -1;
    PageUrlInfo *pageUrlInfo = (PageUrlInfo *) userData;
    gint    i;

    BR_MANIFESTPRINTF("page_url_start_element --  %s", name);

    result = strcmp("page", name);

    if (result == 0)
    {
        if ((pageUrlInfo->currentType != NULL)
            && (pageUrlInfo->searchedType != NULL) && (strcmp(pageUrlInfo->currentType, pageUrlInfo->searchedType) == 0))
        {

            for (i = 0; attr[i]; i += 2)
            {
                BR_MANIFESTPRINTF("%s='%s'", attr[i], attr[i + 1]);

                result = strcmp(attr[i], "number");

                if (result == 0)
                {
                    pageUrlInfo->currentNmbr = atoi(attr[i + 1]);
                    BR_MANIFESTPRINTF("page_url_start_element-- update of the currentNmbr to %d", pageUrlInfo->currentNmbr);
                }

                result = strcmp(attr[i], "url");

                if (result == 0)
                {
                    pageUrlInfo->currentPage = g_strdup(attr[i + 1]);
                    BR_MANIFESTPRINTF("page_url_start_element -- found %s page url attribute\n", attr[i + 1]);
                }

            }
        }
    }
    else
    {
        result = strcmp("pagelist", name);

        if (result == 0)
        {
            BR_MANIFESTPRINTF("page_url_start_element -- found %s tag", name);

            for (i = 0; attr[i]; i += 2)
            {
                BR_MANIFESTPRINTF(" %s='%s'", attr[i], attr[i + 1]);

                result = strcmp(attr[i], "type");
                if (result == 0)
                {
                    pageUrlInfo->currentType = g_strdup(attr[i + 1]);
                    BR_MANIFESTPRINTF("page_url_start_element -- found %s type attribute", pageUrlInfo->currentType);
                }
            }
        }
    }

}

static void XMLCALL page_url_end_element(void *userData, const char *name)
{
    gint    result = -1;
    PageUrlInfo *pageUrlInfo = (PageUrlInfo *) userData;

    BR_MANIFESTPRINTF("page_url_end_element -- page %s with %d", pageUrlInfo->currentType, pageUrlInfo->currentNmbr);

    if ((pageUrlInfo->currentType != NULL) && (pageUrlInfo->searchedType != NULL))
    {
        result = strcmp(pageUrlInfo->currentType, pageUrlInfo->searchedType);
    }

    if (result == 0)
    {
        result = strcmp("page", name);

        if (result == 0)
        {
            BR_MANIFESTPRINTF("page_url_end_element -- found %s tag -- compare %d with %d", name, pageUrlInfo->currentNmbr,
                              pageUrlInfo->searchedNmbr);

            if (pageUrlInfo->currentNmbr == pageUrlInfo->searchedNmbr)
            {
                pageUrlInfo->searchedPage = g_strdup(pageUrlInfo->currentPage);
                BR_MANIFESTPRINTF("**** page_url_end_element -- found %s type (nmbr %d) => url %s ******", pageUrlInfo->currentType,
                                  pageUrlInfo->currentNmbr, pageUrlInfo->searchedPage);
            }
        }
    }
}

//****************************************************************************************
//
// Retrieve the page number from the index file, given the page location (url)
//
//*************************************************************************************** 
int index_file_get_page_number(FILE * indexFile, char *type, char *url)
{
    XML_Parser theParser;
    char    buf[BUFFER_SIZE];
    int     done;
    int     returnValue = 1;
    PageNmbrInfo *pageNmbrInfo;

    if (url && indexFile)
    {
        theParser = parser_create();
        pageNmbrInfo = g_new0(PageNmbrInfo, 1);

        // reset the search variables
        pageNmbrInfo->searchedNmbr = INVALID_PAGE_NMBR;
        pageNmbrInfo->currentNmbr = INVALID_PAGE_NMBR;

        pageNmbrInfo->searchedPage = url;
        pageNmbrInfo->currentPage = NULL;

        XML_SetUserData(theParser, pageNmbrInfo);
        XML_SetElementHandler(theParser, page_nmbr_start_element, page_nmbr_end_element);

        rewind(indexFile);

        while ((!feof(indexFile)) && (pageNmbrInfo->searchedNmbr == INVALID_PAGE_NMBR))
        {
            size_t  len = fread(buf, 1, sizeof(buf), indexFile);

            done = len < sizeof(buf);
            if (XML_Parse(theParser, buf, len, done) == XML_STATUS_ERROR)
            {
                BR_MANIFESTPRINTF("%s at line %d", XML_ErrorString(XML_GetErrorCode(theParser)), XML_GetCurrentLineNumber(theParser));
                return INVALID_PAGE_NMBR;
            }
        }
        returnValue = pageNmbrInfo->searchedNmbr;

        // clean up
        g_free(pageNmbrInfo);

        parser_destroy(theParser);
    }
    return returnValue;
}

static void XMLCALL page_nmbr_start_element(void *userData, const char *name, const char **attr)
{
    gint    result = -1;
    PageNmbrInfo *pageNmbrInfo = (PageNmbrInfo *) userData;
    gint    i;

    BR_MANIFESTPRINTF("page_nmbr_start_element --  %s", name);
    result = strcmp("page", name);

    if (result == 0)
    {
        BR_MANIFESTPRINTF("page_nmbr_start_element -- found %s tag", name);

        for (i = 0; attr[i]; i += 2)
        {
            BR_MANIFESTPRINTF(" %s='%s'", attr[i], attr[i + 1]);

            result = strcmp(attr[i], "url");
            if (result == 0)
            {
                pageNmbrInfo->currentPage = g_strdup(attr[i + 1]);
                BR_MANIFESTPRINTF("page_nmbr_start_element -- found %s url attribute", attr[i + 1]);
            }

            result = strcmp(attr[i], "number");
            if (result == 0)
            {
                pageNmbrInfo->currentNmbr = atoi(attr[i + 1]);
                BR_MANIFESTPRINTF("page_nmbr_start_element-- update of the currentNmbr to %d", pageNmbrInfo->currentNmbr);
            }
        }
    }
}

static void XMLCALL page_nmbr_end_element(void *userData, const char *name)
{
    gint    result = -1;
    PageNmbrInfo *pageNmbrInfo = (PageNmbrInfo *) userData;

    BR_MANIFESTPRINTF("page_nmbr_end_element -- page %s with %d", pageNmbrInfo->currentPage, pageNmbrInfo->currentNmbr);

    result = strcmp("page", name);

    if (result == 0)
    {
        BR_MANIFESTPRINTF("page_nmbr_end_element -- found %s tag", name);

        // pageNmbrInfo->searchedPage is a absolute URL value
        result = url_compare(pageNmbrInfo->searchedPage, pageNmbrInfo->currentPage);

        if (result == 0)
        {
            pageNmbrInfo->searchedNmbr = pageNmbrInfo->currentNmbr;
            BR_MANIFESTPRINTF("**** page_nmbr_end_element -- found %s type => count %d ******", pageNmbrInfo->searchedPage,
                              pageNmbrInfo->searchedNmbr);
        }

        // clean up allocated memory
        if (pageNmbrInfo->currentPage)
        {
            g_free(pageNmbrInfo->currentPage);
            pageNmbrInfo->currentPage = NULL;
        }
    }
}


//****************************************************************************************
//
// Retrieve the page count from the index file, given the page type
//
//*************************************************************************************** 
int index_file_get_page_count(FILE * indexFile, char *type)
{
    XML_Parser theParser;
    char    buf[BUFFER_SIZE];
    int     done;
    int     returnValue = DEFAULT_PAGE_COUNT;
    PageTypeInfo *pageTypeInfo;

    if (type && indexFile)
    {
        theParser = parser_create();
        pageTypeInfo = g_new0(PageTypeInfo, 1);

        // reset the search variables
        pageTypeInfo->searchedCount = INVALID_PAGE_COUNT;
        pageTypeInfo->currentCount = INVALID_PAGE_COUNT;

        pageTypeInfo->searchedType = type;
        pageTypeInfo->currentType = NULL;

        BR_MANIFESTPRINTF("index_file_get_page_count -- searchedType = %s", pageTypeInfo->searchedType);

        XML_SetUserData(theParser, pageTypeInfo);
        XML_SetElementHandler(theParser, page_list_start_element, page_list_end_element);

        rewind(indexFile);

        while ((!feof(indexFile)) && (pageTypeInfo->searchedCount == INVALID_PAGE_COUNT))
        {
            size_t  len = fread(buf, 1, sizeof(buf), indexFile);

            BR_MANIFESTPRINTF("buffer %s", buf);

            done = len < sizeof(buf);
            if (XML_Parse(theParser, buf, len, done) == XML_STATUS_ERROR)
            {
                BR_MANIFESTPRINTF("%s at line %d", XML_ErrorString(XML_GetErrorCode(theParser)), XML_GetCurrentLineNumber(theParser));
                return INVALID_PAGE_COUNT;
            }
        }

        returnValue = pageTypeInfo->searchedCount;

        // clean up allocated memory
        if (pageTypeInfo->currentType)
        {
            g_free(pageTypeInfo->currentType);
            pageTypeInfo->currentType = NULL;
        }
        g_free(pageTypeInfo);

        parser_destroy(theParser);
    }
    return returnValue;
}

static void XMLCALL page_list_start_element(void *userData, const char *name, const char **attr)
{
    gint    result = -1;
    PageTypeInfo *pageTypeInfo = (PageTypeInfo *) userData;
    gint    i;

    BR_MANIFESTPRINTF("page_list_start_element --  %s", name);
    result = strcmp("pagelist", name);

    if (result == 0)
    {
        BR_MANIFESTPRINTF("page_list_start_element -- found %s tag", name);

        for (i = 0; attr[i]; i += 2)
        {
            BR_MANIFESTPRINTF(" %s='%s'", attr[i], attr[i + 1]);

            result = strcmp(attr[i], "type");
            if (result == 0)
            {
                pageTypeInfo->currentType = g_strdup(attr[i + 1]);
                BR_MANIFESTPRINTF("page_list_start_element -- found %s type attribute", attr[i + 1]);
            }

            result = strcmp(attr[i], "count");
            if (result == 0)
            {
                pageTypeInfo->currentCount = atoi(attr[i + 1]);
                BR_MANIFESTPRINTF("page_list_start_element-- update of the current type pagecount to %d", pageTypeInfo->currentCount);
            }
        }
    }
}

static void XMLCALL page_list_end_element(void *userData, const char *name)
{
    gint    result = -1;
    PageTypeInfo *pageTypeInfo = (PageTypeInfo *) userData;

    BR_MANIFESTPRINTF("page_list_end_element -- pagelist %s with %d", pageTypeInfo->currentType, pageTypeInfo->currentCount);

    result = strcmp("pagelist", name);

    if (result == 0)

    {
        BR_MANIFESTPRINTF("page_list_end_element -- found %s tag", name);

        result = strcmp(pageTypeInfo->searchedType, pageTypeInfo->currentType);

        if (result == 0)
        {
            pageTypeInfo->searchedCount = pageTypeInfo->currentCount;
            BR_MANIFESTPRINTF("**** page_list_end_element -- found %s type => count %d ******", pageTypeInfo->searchedType,
                              pageTypeInfo->searchedCount);
        }
    }
}

//****************************************************************************************
//
// UTILITIES
//
//*************************************************************************************** 

// compare url values, taking relative url values into account
static int url_compare(char *abs_url1, char *url2)
{
    gchar  *ptr = url2;

    if ((abs_url1) && (url2))
    {
        while (*ptr == '.' || *ptr == '/')
        {
            ptr++;
        }

        // quick tryout => full implementation will follow later
        if (strstr(abs_url1, ptr) != 0)
        {
            BR_MANIFESTPRINTF("curently identical URL's %s - %s", abs_url1, ptr);
            return 0;
        }

    }
    return -1;
}

#endif







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

//========================================================================
// internal helper function
// check whether or not the first url contains the second one
// for example:
// first: file:///opt/content/newspapers/De%20Volkskrant/20060410_01/002/index.html
// second: 002/index.html
// return 0 if not, return non-zero if contain 
static int contain_url(const char *first , const char *second)
{
    if (NULL == first || NULL == second) return 1;
    int len1   = strlen(first), len2 = strlen(second);
    int min    = len1; 
    if (min > len2) min = len2;
    
    first += len1; second += len2;
    for(int i = 0; i <min; ++i)
    {
        if (*first-- != *second--) 
        {
            return 0;
        }
    }
    return 1;
}

// somewhat slow, but works
int get_pagelist_length(erManifest * manifest)
{
    char xpath[MAX_LEN]  = {0};
    char type[MAX_LEN]   = {0};
    int i = 0;
    int ret = RET_OK;
    while (1)
    {
        snprintf(xpath, MAX_LEN, "/package/index/pagelist[%d]", i + 1);
        ret = ermXmlGetAttributeString(manifest, xpath, "type", type, MAX_LEN);
        if (RET_ERR == ret) break;
        ++i;
    }
    return i;
}

// construct a pagelistitem from xml manifest file
PageListItem * pageListItem_create(erManifest * manifest, const int index)
{
    char url[MAX_LEN]    = {0};
    char type[MAX_LEN]   = {0};
    char xpath[MAX_LEN]  = {0};
    int ret = RET_OK;
    int count = 0;
    
    snprintf(xpath, MAX_LEN, "/package/index/pagelist[%d]", index);
    ret = ermXmlGetAttributeString(manifest, xpath, "type", type, MAX_LEN);
    ret = ermXmlGetAttributeInt(manifest, xpath, "count", &count);
    if (RET_ERR == ret) 
    {
        TRACE("Could not read attribute!");
        return NULL;
    }
  
    // init page List Item, count and type
    PageListItem *pItem = (PageListItem *)malloc(sizeof(PageListItem));
    pItem->type = (char *)malloc(strnlen(type, MAX_LEN) + 1);
    strcpy(pItem->type, type);
    pItem->type[strlen(pItem->type)] = 0;
    pItem->count = count;
    pItem->page  = (PageInfo **)malloc(sizeof(PageInfo *) * count);

    // construct PageInfo array
    for(int i = 1; i <= pItem->count; ++i)
    {
        // two tags : url and number
        snprintf(xpath, MAX_LEN, "/package/index/pagelist[%d]/page[%d]", index, i);
        ermXmlGetAttributeString(manifest, xpath, "url", url, MAX_LEN);
        ermXmlGetAttributeInt(manifest, xpath, "number", &count);   
        
        pItem->page[i - 1] = (PageInfo *)malloc(sizeof(PageInfo));             
        pItem->page[i - 1]->number = count;
        pItem->page[i - 1]->url = (char *)malloc(strnlen(url, MAX_LEN) + 1);
        strcpy(pItem->page[i - 1]->url, url);
        pItem->page[i - 1]->url[strlen(pItem->page[i - 1]->url)] = 0;
    }
    return pItem;
}

void pageInfo_release(PageInfo *pInfo)
{
    if (pInfo->url)
    {
        free(pInfo->url);
        pInfo->url = NULL;
    }
    pInfo->number = INVALID_PAGE_NMBR;
}

void pageListItem_release(PageListItem *pItem)
{
    for(int i = 0; i < pItem->count; ++i)
    {
        // free memory used by each page fields and page itself
        pageInfo_release(pItem->page[i]);
        free(pItem->page[i]);
    }
    free(pItem->type);
    pItem->type = NULL;
    pItem->count = 0;
}

//========================================================================
// implementation
void index_file_close(IndexInfo *index)
{
    if (NULL == index)
    {
        TRACE("Invalid IndexInfo pointer!");
        return;
    }
    
    if (NULL == index->pagelist || 0 >= index->length)
    {
        // not initialized
        return;
    }
    
    // release all memory
    for(int i = 0; i < index->length; ++i)
    {
        pageListItem_release(index->pagelist[i]);
    }
    free(index->pagelist);
}

int index_file_init(const char* pathName, IndexInfo* pIndex)
{
    pIndex->pagelist  = NULL;
    pIndex->length    = 0;
    erManifest manifest;
    if (RET_OK != ermXmlOpenFile(pathName, &manifest))
    {
        TRACE("Could not open manifest file %s", pathName);
        return -1;
    }
    
    pIndex->length    = get_pagelist_length(&manifest);
    if (0 >= pIndex->length)
    {
        TRACE("No Pagelist item!");
        return 0;
    }
    
    // construct one by one
    pIndex->pagelist = (PageListItem **)malloc(pIndex->length * sizeof(PageListItem *));
    for(int i = 0; i < pIndex->length; ++i)
    {
        pIndex->pagelist[i] = pageListItem_create(&manifest, i + 1);
    }
    ermXmlClose(&manifest);
    
#if (INDEX_DEBUG)
    dump(pIndex);
#endif
    
    return 0;
}

int index_file_get_page_count(IndexInfo * index, const char* type)
{
    if (NULL == index)
    {
        TRACE("Invalid index pointer!");
        return INVALID_PAGE_COUNT;
    }
    
    if (NULL == type)
    {
        TRACE("Invalid type pointer!");
        return INVALID_PAGE_COUNT;
    }
    
    for(int i = 0; i < index->length; ++i)
    {
        if (0 == strcmp(index->pagelist[i]->type, type))
        {
            return index->pagelist[i]->count;
        }
    }
    return INVALID_PAGE_COUNT;
}


// the parameter url always different from url in the list
// so we need to use contain_url instead of strcmp
int index_file_get_page_number(IndexInfo * index, const char* type, const char* url)
{
    if (NULL == index)
    {
        TRACE("Invalid index pointer!");
        return INVALID_PAGE_COUNT;
    }
    
    if (NULL == type)
    {
        TRACE("Invalid type pointer!");
        return INVALID_PAGE_COUNT;
    }
    
    if (NULL == url)
    {
        TRACE("Invalid url pointer!");
        return INVALID_PAGE_COUNT;
    }
   
    for(int i = 0; i < index->length; ++i)
    {
        if (0 == strcmp(index->pagelist[i]->type, type))
        {
            PageListItem *pItem = index->pagelist[i];
            for(int j = 0; j < pItem->count; ++j)
            {
                // should use contain_url. 
                if (contain_url(url, pItem->page[j]->url))
                {
                    return pItem->page[j]->number;
                }
            }
        }
    }
    return INVALID_PAGE_NMBR;
}

char* index_file_get_page_location(IndexInfo * index, const char* type, const int number)
{
    if (NULL == index)
    {
        TRACE("Invalid index pointer!");
        return NULL;
    }
    
    if (NULL == type)
    {
        TRACE("Invalid type pointer!");
        return NULL;
    }

    
    for(int i = 0; i < index->length; ++i)
    {
        if (0 == strcmp(index->pagelist[i]->type, type))
        {
            PageListItem *pItem = index->pagelist[i];
            for(int j = 0; j < pItem->count; ++j)
            {
                if (number == pItem->page[j]->number)
                {
                    return pItem->page[j]->url;
                }
            }
        }
    }
    return NULL;
}

void dump(IndexInfo *index)
{
    if (NULL == index) return;
    
    printf("=======================Index file information=======================\n");
    printf("length %d\n", index->length);
    for(int i = 0; i <index->length; ++i)
    {
        PageListItem *pItem = index->pagelist[i];
        printf("\titem type: %s\tcount: %d\n", pItem->type, pItem->count);
        
        for(int j = 0; j < pItem->count; ++j)
        {
            PageInfo *pInf = pItem->page[j];
            printf("\t\tpage number: %d\turl: %s\n", pInf->number, pInf->url);
        }
    }
    printf("====================================================================\n");
}    

