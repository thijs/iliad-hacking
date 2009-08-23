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


#define _GNU_SOURCE

#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#include <libermanifest/ermanifest.h>

#include "scbdoc.h"
#include "scblog.h"

//------------------------------------------------------------------------
// check path, internal use
gboolean _doc_check_path(ScbPathPtr ptr)
{
    if (NULL == ptr || strnlen(ptr->scbname, SCB_MAX_PATH) <= 0)
    {
        SCB_ERROR("Invalid path!");
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------------
// construct a new document object
ScbDocPtr scb_doc_new()
{
    ScbDocPtr ptr = g_new0(ScbDoc, 1);

    if (NULL == ptr)
    {
        SCB_ERROR("Not enough memory!");
        return NULL;
    }

    if (!scb_pages_new(&ptr->pages))
    {
        g_free(ptr);
        return NULL;
    }

    scb_doc_init_context(ptr);

    return ptr;
}

//------------------------------------------------------------------------
void scb_doc_free(ScbDocPtr ptr)
{
    if (NULL == ptr)
    {
        return;
    }

    scb_doc_free_context(ptr);
    scb_pages_free(&ptr->pages);
    g_free(ptr);
}

//------------------------------------------------------------------------
// open
// 1. check and reset all list
// 2. load 
ScbDocPtr scb_doc_open(ScbPathPtr ptr)
{
    // check
    if (!_doc_check_path(ptr)) return NULL;   

    // use lib ermanifest to open the document
    ScbXml xml; 
    if (RET_OK != ermXmlOpenFile(ptr->scbname, &xml.handle))
    {
        SCB_ERROR("Could not open the file %s!", ptr->scbname);
        return NULL;
    }
    SCB_TRACE("Successfully open file %s", ptr->scbname);

    // construct the document object
    ScbDocPtr doc = scb_doc_new();
    if (NULL == doc)
    {
        ermXmlClose(&xml.handle);
        return NULL;
    }
    memcpy(&doc->path, ptr, sizeof(doc->path));
    SCB_TRACE("Successfully construct document object!");
    
    // construct xPath
    strncpy(xml.xPath, "/notes", SCB_MAX_XML_PATH);
    xml.index = 1;

    // load pages
    if (scb_pages_load(&doc->pages, &xml))
    {
        ermXmlClose(&xml.handle);
        SCB_TRACE("Successfully load all pages!");
        return doc;
    }
    else
    {
        ermXmlClose(&xml.handle);
        scb_doc_free(doc);
        SCB_TRACE("Error occurs during loading pages!");
        return NULL;
    }
}

//------------------------------------------------------------------------
// make sure exist, try to open it, if not exist, create a new one
ScbDocPtr scb_doc_make_sure_exist(ScbPathPtr ptr)
{
    if (!_doc_check_path(ptr)) return NULL;
    ScbDocPtr doc = NULL;

    // is exist, just open it
    if (scb_path_file_exist(ptr->scbname)) 
    {
        doc = scb_doc_open(ptr);
        if (doc)
        {
            return doc;
        }
    }

    // something wrong with the document, have to re-create it
    // or construct a new one and save as...
    doc = scb_doc_new();
    if (NULL == doc)
    {
        // it's fatal error, could do nothing
        return NULL;
    }
    if (scb_doc_saveAs(doc, ptr))
    {
        return doc;
    }
    scb_doc_free(doc);
    return NULL;
}

//------------------------------------------------------------------------
// save
gboolean scb_doc_save(ScbDocPtr doc)
{
    SCB_RET_FALSE_IF(NULL == doc, "Invalid doc pointer!");

    // use lib ermanifest to create the document
    ScbXml xml; 
    if (RET_OK != ermXmlCreateFile(doc->path.scbname, &xml.handle))
    {
        SCB_ERROR("Could not create the file %s!", doc->path.scbname);
        return FALSE;
    }
    SCB_TRACE("Successfully create file %s", doc->path.scbname);

    // one by one
    ermXmlNewString(&xml.handle, "/" , "notes", "");
    SCB_TRACE("Create root node scbdoc");

    // notes field
    ermXmlNewString(&xml.handle, "/notes", "version", "");
    ermXmlNewString(&xml.handle, "/notes/version", "number", SCB_LIB_VERSION);
    ermXmlNewString(&xml.handle, "/notes/version", "organization", SCB_LIB_ORG);
    
    // screen field
    ermXmlNewString(&xml.handle, "/notes", "screen", "");
    ermXmlNewString(&xml.handle, "/notes/screen", "units", SCB_DEF_SCREEN_UNITS);
    ermXmlNewInt(&xml.handle, "/notes/screen", "dpi", SCB_DEF_SCREEN_DPI);
    
    // construct xPath
    strncpy(xml.xPath, "/notes", SCB_MAX_XML_PATH);
    xml.index = 0;

    // save pages
    if (scb_pages_save(&doc->pages, &xml))
    {
        ermXmlSaveAndClose(&xml.handle);
        SCB_TRACE("Successfully save all pages!");
        return TRUE;
    }
    else
    {
        ermXmlClose(&xml.handle);
        SCB_TRACE("Error occurs during saving pages!");
        return FALSE;
    }
}

//------------------------------------------------------------------------
// saveAs
gboolean scb_doc_saveAs(ScbDocPtr doc, ScbPathPtr ptr)
{
    SCB_RET_FALSE_IF(NULL == doc || NULL == ptr, "Invalid pointer(s)!");
    memcpy(&doc->path, ptr, sizeof(doc->path));
    return scb_doc_save(doc);
}

//------------------------------------------------------------------------
// composite 
ScbPagesPtr scb_doc_get_pages(ScbDocPtr ptr)
{
    if (ptr)
    {
        return &ptr->pages;
    }
    return NULL;
}

ScbStrokeStylePtr scb_doc_get_current_stroke_style(ScbDocPtr doc)
{
    SCB_RET_NULL_IF(NULL == doc, "Invalid pointer!");
    return &doc->context.curStrokeStyle;
}

//------------------------------------------------------------------------
// a direct method to create page from document. xPath
// for xPath ref: http://www.w3.org/TR/xpath
// Remove them
/*
ScbPagePtr scb_doc_open_page_directly(ScbPathPtr ptr, ScbPageIdPtr id)
{
    SCB_RET_NULL_IF(NULL == ptr || NULL == id, "Invalid pointer(s)!");

    // open xml file
    ScbXml xml;
    if (RET_OK != ermXmlOpenFile(ptr->scbname, &xml.handle))
    {
        SCB_ERROR("Could not open file %s!", ptr->scbname);
        return NULL;
    }

    // construct xPath
    snprintf(xml.xPath, MAX_XML_PATH, "//page[@id = \"%s\"]", id->id);
    SCB_TRACE("xPath %s", xml.xPath);
    ScbPagePtr page = scb_page_load_from_xpath(&xml);
    ermXmlClose(&xml.handle);
    return page;
}
*/

//------------------------------------------------------------------------
// removed
// gboolean scb_doc_save_page_directly(ScbPathPtr ptr, ScbPagePtr page);

//------------------------------------------------------------------------
ScbPagePtr scb_doc_get_page(ScbDocPtr doc, ScbPageIdPtr id)
{
    ScbPagesPtr pages = scb_doc_get_pages(doc);
    if (NULL == pages) return NULL;
    return scb_pages_get_page(pages, id);
}

//------------------------------------------------------------------------
gboolean scb_doc_add_page(ScbDocPtr doc, ScbPagePtr page)
{
    ScbPagesPtr pages = scb_doc_get_pages(doc);
    if (NULL == pages) return FALSE;
    return scb_pages_add_page(pages, page);
}

void scb_doc_init_context(ScbDocPtr ptr)
{
    SCB_RET_IF(NULL == ptr, "Invalid doc pointer!");
    ptr->context.table = g_array_sized_new(FALSE, TRUE, 
            sizeof(ScbTBSItem), SCB_DEF_CMD_ACT_SIZE);
    ptr->context.curState = SCB_TBS_SCRIBBLE;
    ptr->context.curStrokeStyle.color     = SCB_DEV_COLOR_BLACK;
    ptr->context.curStrokeStyle.layer     = SCB_DEF_STROKE_LAYER;
    ptr->context.curStrokeStyle.lineStyle = SCB_DEF_STROKE_LINESTYLE;
    ptr->context.curStrokeStyle.penSize   = SCB_DEF_STROKE_PENSIZE;

    ptr->context.appId = -1;                 
}

void scb_doc_free_context(ScbDocPtr ptr)
{
    SCB_RET_IF(NULL == ptr, "Invalid doc pointer!");
    g_array_free(ptr->context.table, TRUE);
    ptr->context.table = NULL;
}

void scb_doc_add_map_item(ScbDocPtr doc, const ScbTBSItemPtr ptr)
{
    SCB_RET_IF(NULL == doc || NULL == ptr, "Invalid pointer(s)!");
    g_array_append_val(doc->context.table,  *ptr);
}

ScbTBState scb_doc_get_current_state(ScbDocPtr doc)
{
    if (NULL == doc) return SCB_TBS_INVALID;
    return doc->context.curState;
}

gboolean scb_state_is_scribble(ScbDocPtr doc)
{
    return SCB_TBS_SCRIBBLE == scb_doc_get_current_state(doc);
}
gboolean scb_state_is_erase(ScbDocPtr doc)
{
    return SCB_TBS_ERASE == scb_doc_get_current_state(doc);
}


// dump
void scb_doc_dump(ScbDocPtr ptr)
{
    if (ptr)
    {
        scb_pages_dump(&ptr->pages);
    }
}



