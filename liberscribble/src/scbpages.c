/*
 * This file is part of liberscribble.
 *
 * liberscribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberscribble is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

#include "scblog.h"
#include "scbtype.h"
#include "scbpages.h"
#include <glib.h>
#include "scbpage.h"

//------------------------------------------------------------------------
// internal function 
// data : the element's data. 
void _page_release(gpointer data, gpointer userdata)
{
    if (data)
    {
        scb_page_free((ScbPagePtr)data);
    }        
}

//------------------------------------------------------------------------
// construct a new page
gboolean scb_pages_new(ScbPagesPtr ptr)
{
    SCB_RET_FALSE_IF(NULL == ptr, "Invalid pointer!");

    ptr->pages = g_list_alloc();
    if (NULL == ptr->pages)
    {
        SCB_ERROR("Could not allocate list!");
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------
// free 
void scb_pages_free(ScbPagesPtr ptr)
{
    if (ptr)
    {
        // release all stroke
        g_list_foreach(ptr->pages, _page_release, NULL);

        // now the pointers are wild pointers
        g_list_free(ptr->pages);
        ptr->pages = NULL;
    }
}

//------------------------------------------------------------------------
// it will check whether or not the page exist. do nothing if exist
// by id and pointer
gboolean scb_pages_add_page(ScbPagesPtr pages, ScbPagePtr page)
{
    SCB_RET_FALSE_IF(NULL == pages || NULL == page, "Invalid pages or page pointer!");
    
    ScbPagePtr pItem  = NULL;
    GList* ptr = g_list_first(pages->pages);
    while(ptr)
    {
        pItem = ptr->data;
        if (pItem && 0 == strncmp(pItem->id.id, page->id.id, SCB_MAX_PAGEID_LEN))
        {
            // already exist, quit, it's ok,
            // SCB_WARN("Duplicated page id %s!", page->id.id);
            return TRUE;
        }
        ptr = g_list_next(ptr);   
    }
    g_list_append(pages->pages, page);
    return TRUE;
}

//------------------------------------------------------------------------
// detach by id
ScbPagePtr scb_pages_detach_page(ScbPagesPtr pages, ScbPageIdPtr id)
{
    ScbPagePtr pItem  = NULL;
    GList* ptr = g_list_first(pages->pages);
    while(ptr)
    {
        pItem = ptr->data;
        if (pItem && 0 == strncmp(pItem->id.id, id->id, SCB_MAX_PAGEID_LEN))
        {
            // ok, hit. notice.......
            g_list_remove(pages->pages, pItem);
            return pItem;
        }
        ptr = g_list_next(ptr);   
    }
    return NULL;
}

//------------------------------------------------------------------------
// remvoe page by id
void scb_pages_remove_page(ScbPagesPtr pages, ScbPageIdPtr id)
{
    SCB_RET_IF(NULL == pages || NULL == id, "Invalid pointer!");
    
    ScbPagePtr pItem  = NULL;
    GList* ptr = g_list_first(pages->pages);
    while(ptr)
    {
        pItem = ptr->data;
        if (pItem && 0 == strncmp(pItem->id.id, id->id, SCB_MAX_PAGEID_LEN))
        {
            // ok, hit. notice.......
            scb_page_free(pItem);
            g_list_remove(pages->pages, ptr->data);
            return;
        }
        ptr = g_list_next(ptr);   
    }
}


//------------------------------------------------------------------------
// get page count
int scb_pages_get_count(ScbPagesPtr pages)
{
    SCB_RET_INT_IF(NULL == pages, SCB_INVALID_COUNT, "Invalid pages pointer!");
    int len = g_list_length(pages->pages);
    if (len > 0)
    {
        return len - 1;
    }
    return SCB_INVALID_COUNT;
}

//------------------------------------------------------------------------
// get certain page
ScbPagePtr scb_pages_get_page(ScbPagesPtr pages, ScbPageIdPtr id)
{
    ScbPagePtr pItem  = NULL;
    GList* ptr = g_list_first(pages->pages);
    while(ptr)
    {
        pItem = ptr->data;
        if (pItem && 0 == strncmp(pItem->id.id, id->id, SCB_MAX_PAGEID_LEN))
        {
            // ok, hit. notice.......
           return pItem;
        }
        ptr = g_list_next(ptr);   
    }
    return NULL;
}

//------------------------------------------------------------------------
// change page id, make sure the id does not conflict. we do not check here
gboolean scb_pages_change_page_id(ScbPagesPtr pages, ScbPageIdPtr oldId, 
                         ScbPageIdPtr newId)
{
    ScbPagePtr pItem  = NULL;
    GList* ptr = g_list_first(pages->pages);
    while(ptr)
    {
        pItem = ptr->data;
        if (pItem && 0 == strncmp(pItem->id.id, oldId->id, SCB_MAX_PAGEID_LEN))
        {
            // ok, hit. 
            strncpy(pItem->id.id, newId->id, SCB_MAX_PAGEID_LEN);
            return TRUE;
        }
        ptr = g_list_next(ptr);   
    }
    return FALSE;
}

//------------------------------------------------------------------------
// internal function
void _page_save(gpointer data, gpointer user_data)
{
    if (data && user_data)
    {
        ScbXmlPtr ptr = (ScbXmlPtr)user_data;
        scb_page_save((ScbPagePtr)data, ptr);
        ++ptr->index;
    }
}

//------------------------------------------------------------------------
// save 
gboolean scb_pages_save(ScbPagesPtr pages, ScbXmlPtr xml)
{
    SCB_RET_FALSE_IF(NULL == pages || NULL == xml, "Invalid pointer!");
    // xPath
    ScbXmlPtr self = scb_xml_clone(xml);
    self->index = 1;
    ermXmlNewString(&xml->handle, self->xPath, "pages", "");
    strncat(self->xPath, "/pages", SCB_MAX_XML_PATH);
    g_list_foreach(pages->pages, _page_save, self);
    scb_xml_free(self);
    return TRUE;
}

//------------------------------------------------------------------------
// load from xml file
gboolean scb_pages_load(ScbPagesPtr pages, ScbXmlPtr xml)
{
    // check
    SCB_RET_FALSE_IF(NULL == pages || NULL == xml, "Invalid pointer!");

    // construct xPath
    ScbXmlPtr self = scb_xml_clone(xml);
    strncat(self->xPath, "/pages", SCB_MAX_XML_PATH);
    self->index = 1;

    // load pages
    gboolean bLoad = TRUE;
    while (bLoad)
    {
        ScbPagePtr page = scb_page_new();
        bLoad = scb_page_load(page, self);
        if (bLoad)
        {
            scb_pages_add_page(pages, page);
            ++self->index;
        }
        else
        {
            scb_page_free(page);
            break;
        }
    }
    scb_xml_free(self);
    return TRUE;
}

//------------------------------------------------------------------------
// dump
void _page_dump(gpointer data, gpointer userdata)
{
    if (data)
    {
        scb_page_dump((ScbPagePtr)data);
    }        
}

void scb_pages_dump(ScbPagesPtr pages)
{
    SCB_RET_IF(NULL == pages, "NULL pointer!");
    g_list_foreach(pages->pages, _page_dump, 0);
}
