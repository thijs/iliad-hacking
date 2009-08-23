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

#include <stdio.h>
#include <string.h>

#include "scbconfig.h"
#include "scblog.h"
#include "scbpage.h"
#include "scbtype.h"

//------------------------------------------------------------------------
// page id
void scb_page_id_from_int(ScbPageIdPtr ptr, const int number)
{
    if (ptr)
    {
        snprintf(ptr->id, SCB_MAX_PAGEID_LEN, "%d", number);
    }
}

void scb_page_id_from_str(ScbPageIdPtr ptr, const char *str)
{
    if (ptr && str)
    {
        strncpy(ptr->id, str, SCB_MAX_PAGEID_LEN);
    }
}


//------------------------------------------------------------------------
typedef struct _ScbEraseCtx
{
    ScbDevPoint     p1, p2;
    int             count;
}ScbEraseCtx;
ScbEraseCtx _erase_ctx;
static gboolean bEraseCtxInit = FALSE;

//------------------------------------------------------------------------
// page
ScbPagePtr scb_page_new()
{
    ScbPagePtr ptr = g_new0(ScbPage, 1);    
    if (NULL == ptr)
    {
        // not enough memory, fatal error
        SCB_ERROR("Not enough memory for page!");
        return NULL;
    }    

    // init values
    if (!scb_strokes_new(&ptr->strokes))
    {
        g_free(ptr);
        return NULL;
    }
    
    ptr->rect.left      = SCB_DEF_PAGE_LEFT;
    ptr->rect.right     = SCB_DEF_PAGE_RIGHT;
    ptr->rect.top       = SCB_DEF_PAGE_TOP;
    ptr->rect.bottom    = SCB_DEF_PAGE_BOTTOM;
   
    return ptr;
}

//------------------------------------------------------------------------
void scb_page_free(ScbPagePtr ptr)
{
    SCB_RET_IF(NULL == ptr, "Attempt to release NULL pointer!");
        
    scb_strokes_free(&ptr->strokes);
    scb_strokes_free(&ptr->delStrokes);
    g_free(ptr);
}

//------------------------------------------------------------------------
void scb_page_set_id(ScbPagePtr page, ScbPageIdPtr id)
{
    if (page && id)
    {
        strncpy(page->id.id, id->id, SCB_MAX_PAGEID_LEN);
    }
}

//------------------------------------------------------------------------
// clear all items from list 
void scb_page_clear(ScbPagePtr ptr)
{
    SCB_RET_IF(NULL == ptr, "Attempt to take action on NULL pointer!");
    scb_strokes_empty(&ptr->strokes);
    scb_strokes_empty(&ptr->delStrokes);
}

//------------------------------------------------------------------------
int scb_page_get_stroke_count(ScbPagePtr ptr)
{
    SCB_RET_INT_IF(NULL == ptr, SCB_INVALID_COUNT, "Invalid page pointer!");
    return scb_strokes_get_count(&ptr->strokes);
}

//------------------------------------------------------------------------
gboolean scb_page_add_stroke(ScbPagePtr page, ScbStrokePtr stroke)
{
    SCB_RET_FALSE_IF(NULL == page || NULL == stroke, "Invalid pointer!");        
    g_list_append(page->strokes.strokes, stroke);
    return TRUE;
}

//------------------------------------------------------------------------
ScbStrokesPtr scb_page_get_strokes(ScbPagePtr page)
{
    SCB_RET_NULL_IF(NULL == page, "Attempt to access NULL pointer!");
    return &page->strokes;
}

void scb_page_erase_fast_draw(ScbPagePtr page, ScbDevPointPtr point)
{
    ScbHitTestCtx ctx; ctx.size = SCB_DEF_ERASE_SIZE;
    ScbStrokesPtr strokes = NULL;
    if (!bEraseCtxInit)
    {
        _erase_ctx.count = 0;
        bEraseCtxInit = TRUE;
    }
    
    if (0 == _erase_ctx.count)
    {
        _erase_ctx.p1 = *point;
        _erase_ctx.count = 1;
        strokes = scb_strokes_point_hit_test(&page->strokes, point, &ctx);
    }
    else
    {
        if (_erase_ctx.count > 1)
        {
            _erase_ctx.p1 = _erase_ctx.p2;
        }
        _erase_ctx.p2 = *point;    
        strokes = scb_strokes_line_hit_test(&page->strokes, 
            &_erase_ctx.p1, 
            &_erase_ctx.p2,
            &ctx);
    }

    // result
    if (strokes)
    {
        scb_strokes_erase_fast_draw(strokes);
        scb_strokes_add_strokes(&page->delStrokes, (const ScbStrokesPtr)&strokes);
        scb_strokes_detach(strokes);
    }
}

//------------------------------------------------------------------------
void scb_page_erase_fast_draw_done(ScbPagePtr page, ScbDevPointPtr point)
{
    ScbStrokesPtr strokes = NULL;
    ScbHitTestCtx ctx; ctx.size = SCB_DEF_ERASE_SIZE;
    if (0 == _erase_ctx.count)
    {
        _erase_ctx.p1 = *point;
        _erase_ctx.count = 1;
        strokes = scb_strokes_point_hit_test(&page->strokes, point, &ctx);
    }
    else
    {
        if (_erase_ctx.count > 1)
        {
            _erase_ctx.p1 = _erase_ctx.p2;
        }
        _erase_ctx.p2 = *point;    
        strokes = scb_strokes_line_hit_test(&page->strokes, 
            &_erase_ctx.p1, 
            &_erase_ctx.p2,
            &ctx);
    }

    // strokes to erase
    if (strokes)
    {
        scb_strokes_erase_fast_draw(strokes);
        scb_strokes_add_strokes(&page->delStrokes, (const ScbStrokesPtr)&strokes);
        scb_strokes_detach(strokes);
    }

    // reset
    _erase_ctx.count = 0;
}

void scb_page_erase_init(ScbPageEraseCtxPtr ptr)
{
    ptr->zoom = 1.0;
    ptr->bInit = FALSE;
}

ScbStrokesPtr scb_page_erase_hit_test(ScbPagePtr page, ScbDevPointPtr point, ScbPageEraseCtxPtr eraCtx)
{
    ScbStrokesPtr strokes = NULL;
    ScbHitTestCtx ctx; 
    ctx.size = SCB_DEF_ERASE_SIZE;
    ctx.zoom = eraCtx->zoom;
    if (!eraCtx->bInit)
    {
        eraCtx->p1 = *point;
        eraCtx->bInit = TRUE;
        strokes = scb_strokes_point_hit_test(&page->strokes, point, &ctx);
    }
    else
    {
        eraCtx->p2 = eraCtx->p1;
        eraCtx->p1 = *point;
        strokes = scb_strokes_line_hit_test(&page->strokes, 
            &eraCtx->p2, 
            &eraCtx->p1,
            &ctx);
    }

    return strokes;
}

//------------------------------------------------------------------------
gboolean scb_page_save(ScbPagePtr page, ScbXmlPtr ptr)
{
    // check
    SCB_RET_FALSE_IF(NULL == page || NULL == ptr, "Invalid page or xml pointer!");
            
    // construct xPath
    ScbXmlPtr self = scb_xml_clone(ptr);
    ermXmlNewString(&ptr->handle, self->xPath, "page", "");
    char * tmp = self->xPath + strnlen(self->xPath, SCB_MAX_XML_PATH);
    snprintf(tmp, SCB_MAX_XML_PATH,  "/page[%d]", ptr->index);

    // save attributes
    ermXmlSetAttributeString(&ptr->handle, self->xPath, "backgroundcolor",
        scb_color_to_html_color(&page->style.bgColor), SCB_MAX_COLOR_LEN);
    ermXmlSetAttributeString(&ptr->handle, self->xPath, 
            "id", page->id.id, SCB_MAX_PAGEID_LEN);
    ermXmlNewInt(&ptr->handle, self->xPath, 
            "orientation", page->style.orientation);
    ermXmlNewInt(&ptr->handle, self->xPath, 
            "height", page->rect.bottom - page->rect.top);
    ermXmlNewInt(&ptr->handle, self->xPath, 
            "width", page->rect.right - page->rect.left);

    // save strokes
    gboolean bRet = scb_strokes_save(&page->strokes, self);
    scb_xml_free(self);
    return bRet;
}

//------------------------------------------------------------------------
gboolean scb_page_load(ScbPagePtr page, ScbXmlPtr ptr)
{
    // check
    SCB_RET_FALSE_IF(NULL == page || NULL == ptr, "Invalid page or xml pointer!");

    // load attributes
    ScbXmlPtr self = scb_xml_clone(ptr);
    char * tmp = self->xPath + strnlen(self->xPath, SCB_MAX_XML_PATH);
    snprintf(tmp, SCB_MAX_XML_PATH, "/page[%d]", self->index);
    char xPath[SCB_MAX_XML_PATH] = {0};
    strncpy(xPath, self->xPath, SCB_MAX_XML_PATH);
    int len = strnlen(xPath, SCB_MAX_XML_PATH);
    
    if (RET_ERR == ermXmlExist(&ptr->handle, xPath))
    {
        // nothing more
        return FALSE;
    }
        
    // background color
    char color[SCB_MAX_COLOR_LEN] = {0};
    ermXmlGetAttributeString(&self->handle, self->xPath, 
        "backgroundcolor", color, SCB_MAX_COLOR_LEN);
    scb_html_color_to_color(color, SCB_MAX_COLOR_LEN, &page->style.bgColor);

    // id
    ermXmlGetAttributeString(&ptr->handle, self->xPath,
        "id", page->id.id, SCB_MAX_PAGEID_LEN);

    // orientation
    strncat(xPath, "orientation", SCB_MAX_XML_PATH);
    ermXmlGetInt(&ptr->handle, xPath, &page->style.orientation);
    
    // height
    page->rect.left = page->rect.top = 0;
    int value = 0;
    xPath[len] = 0;
    strncat(xPath, "/height", SCB_MAX_XML_PATH);
    ermXmlGetInt(&ptr->handle, xPath, &value);
    page->rect.bottom = value;

    // width
    xPath[len] = 0;
    strncat(xPath, "/width", SCB_MAX_XML_PATH);
    ermXmlGetInt(&ptr->handle, xPath, &value);
    page->rect.right = value;

    // load strokes
    gboolean bRet = scb_strokes_load(&page->strokes, self);
    scb_xml_free(self);
    return bRet;
}

//------------------------------------------------------------------------
/*
ScbPagePtr scb_page_load_from_xpath(ScbXmlPtr ptr)
{
    SCB_RET_NULL_IF(NULL == ptr, "Invalid pointer!");

    // load attributes
    if (RET_ERR == ermXmlExist(&ptr->handle, ptr->xPath))
    {
        // nothing more
        return NULL;
    }
    ScbPagePtr page = scb_page_new();

    // background color
    char color[MAX_COLOR_LEN] = {0};
    ermXmlGetAttributeString(&ptr->handle, ptr->xPath, 
        "backgroundcolor", color, MAX_COLOR_LEN);
    scb_htmlcolor_to_color(color, MAX_COLOR_LEN, &page->bgColor);

    // id
    ermXmlGetAttributeString(&ptr->handle, ptr->xPath,
        "id", page->id.id, MAX_PAGEID_LEN);

    // orientation
    char tmp[SCB_MAX_XML_PATH] = {0}; 
    strncpy(tmp, ptr->xPath, SCB_MAX_XML_PATH);
    int len = strnlen(tmp, SCB_MAX_XML_PATH);
    strncat(tmp, "/orientation", SCB_MAX_XML_PATH);
    ermXmlGetInt(&ptr->handle, tmp, &page->orientation);

    // height
    page->rect.left = page->rect.top = 0;
    int value = 0;
    tmp[len] = 0;
    strncat(tmp, "/height", SCB_MAX_XML_PATH);
    ermXmlGetInt(&ptr->handle, ptr->xPath, &value);
    page->rect.bottom = value;

    // width
    tmp[len] = 0;
    strncat(tmp, "/width", SCB_MAX_XML_PATH);
    ermXmlGetInt(&ptr->handle, ptr->xPath, &value);
    page->rect.right = value;

    // load strokes
    gboolean bRet = scb_strokes_load(&page->strokes, ptr);
    if (bRet)
    {
        return page;
    }
    else
    {
        scb_page_free(page);
        return NULL;
    }
}
*/

//------------------------------------------------------------------------
void scb_page_dump(ScbPagePtr ptr)
{
    SCB_RET_IF(NULL == ptr, "Attempt to dump on NULL page pointer!");

    SCB_DUMP("================= Page %s Information =================", ptr->id.id);
    SCB_DUMP("orientation %d", ptr->style.orientation);
    SCB_DUMP("rect (%d, %d) - (%d, %d)", ptr->rect.left, ptr->rect.top, 
        ptr->rect.right, ptr->rect.bottom);
    scb_strokes_dump(&ptr->strokes);
    SCB_DUMP("==================== Page %s Done =====================", ptr->id.id);
}
