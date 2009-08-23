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

#include "scbstrokes.h"
#include "scblog.h"
#include "scbstroke.h"

//------------------------------------------------------------------------
// internal function, used to release each stroke
// data : the element's data. 
void _stroke_release(gpointer data, gpointer user_data)
{
    if (data)
    {
        scb_stroke_free((ScbStrokePtr)data);
    }        
}

void _stroke_dump(gpointer data, gpointer user_data)
{
    if (data)
    {
        scb_stroke_dump((ScbStrokePtr)data);
    }        
}


//------------------------------------------------------------------------
// construct a stroke list
gboolean scb_strokes_new(ScbStrokesPtr ptr)
{
    SCB_RET_FALSE_IF(NULL == ptr, "Invalid pointer!");
    ptr->strokes = g_list_alloc();
    SCB_RET_FALSE_IF(NULL == ptr->strokes, "Could not allocate memory for list!");
    return TRUE;
}

//------------------------------------------------------------------------
// release memory, for both stroke pointer and list item
void scb_strokes_free(ScbStrokesPtr strokes)
{
    SCB_RET_IF(NULL == strokes, "Attempt to release NULL pointer!");
        
    // release all stroke
    g_list_foreach(strokes->strokes, _stroke_release, NULL);

    // now the pointers are wild pointers
    g_list_free(strokes->strokes);
    strokes->strokes = NULL;
}

//------------------------------------------------------------------------
void scb_strokes_empty(ScbStrokesPtr strokes)
{
    SCB_RET_IF(NULL == strokes, "Attempt to release NULL pointer!");
        
    g_list_foreach(strokes->strokes, _stroke_release, NULL);
    // free and allocate again? I can not find list release function
    // maybe we always free the first element unti list is empty
    g_list_free(strokes->strokes);
    strokes->strokes = g_list_alloc();
}

//------------------------------------------------------------------------
int scb_strokes_get_count(const ScbStrokesPtr strokes)
{
    SCB_RET_INT_IF(NULL == strokes, SCB_INVALID_COUNT, "Invalid strokes pointer!");
    
    // here, the list contains a empty item, so we need to decrease one
    int len = g_list_length(strokes->strokes);
    if (len > 0)
    {
        return len - 1;
    }
    return SCB_INVALID_COUNT;
}

//------------------------------------------------------------------------
// it can be used when the stroke list serves as temp container
// two stroke list share the same storkes. 
void scb_strokes_detach(ScbStrokesPtr strokes)
{
    if (strokes)
    {
        g_list_free(strokes->strokes);
        strokes->strokes = NULL;
    }
    // NULL strokes pointer is not an error here. 
}

//------------------------------------------------------------------------
// Add to list
void scb_strokes_add_stroke(ScbStrokesPtr strokes, 
                            ScbStrokePtr stroke)
{
    SCB_RET_IF(NULL == strokes || NULL == stroke, "Invalid storkes or stroke pointer!");
    g_list_append(strokes->strokes, stroke);
}

void scb_strokes_add_strokes(ScbStrokesPtr dst, 
                            const ScbStrokesPtr src)
{
    SCB_RET_IF(NULL == dst || NULL == src, "Invalid pointer(s)!");
    g_list_concat(dst->strokes, src->strokes);
}


//------------------------------------------------------------------------
// delete stroke from stroke list
ScbStrokePtr scb_strokes_detach_stroke(ScbStrokesPtr strokes, 
                            ScbStrokePtr stroke)
{
    SCB_RET_NULL_IF(NULL == strokes || NULL == stroke, "Invalid pointer!");
    g_list_remove(strokes->strokes, stroke);                   
    return stroke;
}

//------------------------------------------------------------------------
// hit test, should consider the size
// for return value, caller should use scb_strokes_detach to release
ScbStrokesPtr scb_strokes_point_hit_test(ScbStrokesPtr strokes, 
                                   ScbDevPointPtr point,
                                   const ScbHitTestCtxPtr ctx)
{
    SCB_RET_NULL_IF(NULL == strokes, "Invalid strokes list pointer!");
        
    ScbStrokesPtr result = NULL;
    ScbStrokePtr stroke  = NULL;
    GList* ptr = g_list_first(strokes->strokes);
    while(ptr)
    {
        stroke = (ScbStrokePtr)ptr->data;
        if (stroke && scb_stroke_point_hit_test(stroke, point, ctx))
        {
            SCB_TRACE("point hit");
            if (NULL == result)
            {
                result = g_new0(ScbStrokes, 1);
                if (! scb_strokes_new(result))
                {
                    return NULL;
                }
            }
            // move from one stroke list to the other
            scb_strokes_add_stroke(result, stroke);
            scb_strokes_detach_stroke(strokes, stroke);
        }
        ptr = g_list_next(ptr);   
    }
    return result;
}

//------------------------------------------------------------------------
// hit test with line, the caller should use scb_strokes_detach to release the list
ScbStrokesPtr scb_strokes_line_hit_test(ScbStrokesPtr strokes, 
                                  ScbDevPointPtr point1,
                                  ScbDevPointPtr point2,
                                  const ScbHitTestCtxPtr ctx)
{
    SCB_RET_NULL_IF(NULL == strokes, "Invalid strokes list pointer!");
        
    ScbStrokesPtr result = NULL;
    ScbStrokePtr stroke  = NULL;
    GList* ptr = g_list_first(strokes->strokes);
    while(ptr)
    {
        stroke = ptr->data;
        if (stroke && scb_stroke_line_hit_test(stroke, point1, point2, ctx))
        {
            SCB_TRACE("line hit");
            if (NULL == result)
            {
                result = g_new0(ScbStrokes, 1);
                if (!scb_strokes_new(result))
                {
                    return NULL;
                }
            }
            // move from one list to the other
            scb_strokes_add_stroke(result, stroke);
            scb_strokes_detach_stroke(strokes, stroke);
        }
        ptr = g_list_next(ptr);   
    }
    return result;
}

//------------------------------------------------------------------------
void scb_strokes_erase_fast_draw(ScbStrokesPtr strokes)
{
    if (NULL == strokes)
    {
        return;
    }

    GList *ptr = g_list_first(strokes->strokes);
    ScbStrokePtr stroke = NULL;
    while (ptr)
    {
        stroke = ptr->data;
        if (stroke)
        {
            ScbDevColor old = stroke->style.color;
            // use reverse color
            stroke->style.color = SCB_DEV_COLOR_WHITE;
            scb_stroke_fast_draw(stroke);
            stroke->style.color = old;
        }
        ptr = g_list_next(ptr);
    }
}


//------------------------------------------------------------------------
// internal function
void _stroke_save(gpointer data, gpointer user_data)
{
    if (data && user_data)
    {
        ScbXmlPtr ptr = (ScbXmlPtr)user_data;
        scb_stroke_save((ScbStrokePtr)data, ptr);
        ++ptr->index;
    }
}

//------------------------------------------------------------------------
gboolean scb_strokes_save(ScbStrokesPtr strokes, ScbXmlPtr ptr)
{
    // check
    SCB_RET_FALSE_IF(NULL == strokes || NULL == ptr, "Invalid strokes or xml pointer!");
        
    // construct xPath
    ScbXmlPtr self = scb_xml_clone(ptr);
    ermXmlNewString(&self->handle, self->xPath, "strokes", "");
    strncat(self->xPath, "/strokes", SCB_MAX_XML_PATH);
    self->index = 1;
    g_list_foreach(strokes->strokes, _stroke_save, self);
    scb_xml_free(self);
    return TRUE;
}

//------------------------------------------------------------------------
gboolean scb_strokes_load(ScbStrokesPtr strokes, ScbXmlPtr ptr)
{
    // check
    SCB_RET_FALSE_IF(NULL == strokes || NULL == ptr, "Invalid strokes or xml pointer!");
        
    // the strokes here, should be clear. maybe memory leak if open and reload
    // the caller should check it
    scb_strokes_empty(strokes);
    ScbXmlPtr self = scb_xml_clone(ptr);
    strncat(self->xPath, "/strokes", SCB_MAX_XML_PATH);
    self->index = 1;
        
    while(1)
    {
        ScbStrokePtr stroke = scb_stroke_new();
        if (scb_stroke_load(stroke, self))
        {
            scb_strokes_add_stroke(strokes, stroke);
            ++self->index;
        }
        else
        {
            scb_stroke_free(stroke);
            break;
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------
void scb_strokes_dump(ScbStrokesPtr strokes)
{
    if (strokes)
    {
        g_list_foreach(strokes->strokes, _stroke_dump, 0);
    }
}
