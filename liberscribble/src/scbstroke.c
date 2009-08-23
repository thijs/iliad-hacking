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
#include <strings.h>
#include <limits.h>
#include <math.h>

#include "scbutil.h"
#include "scbtype.h"
#include "scbstroke.h"
#include "scblog.h"
#include "scbconfig.h"
#include "driver.h"

static const double e = 0.001;

// Default boundary for points passed to driver. 
// Driver does not check the point boundary. Do it in the library.
// The boundary should be changed later to use auto detect function.
static const int SCB_DEF_BOUNDARY_LEFT   = 0;
static const int SCB_DEF_BOUNDARY_TOP    = 0;
static const int SCB_DEF_BOUNDARY_RIGHT  = 768;
static const int SCB_DEF_BOUNDARY_BOTTOM = 935;     // use client area.

//------------------------------------------------------------------------
// impl. of scb line style
typedef struct _LineStrTab
{
    ScbLineStyle    style;
    char *          name;
}LineStrTab;

// global constants
const LineStrTab _Tab[] =
{
    {SCB_LINE_SOLID, "solid"},
    {SCB_LINE_INVALID, ""},
};
const int LineTabSize = sizeof(_Tab)/sizeof(_Tab[0]);

//------------------------------------------------------------------------
// functions
const char * scb_line_style_to_str(const ScbLineStyle style)
{
    int i;
    for(i = 0; i < LineTabSize; ++i)
    {
        if (style == _Tab[i].style)
        {
            return _Tab[i].name;
        }
    }
    return NULL;
}

ScbLineStyle scb_line_style_from_str(const char * str)
{
    int i;
    for(i = 0; i < LineTabSize; ++i)
    {
        if (0 == strcasecmp(str, _Tab[i].name))
        {
            return _Tab[i].style;
        }
    }
    return SCB_LINE_INVALID;
}

// impl. Done
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// construct a new stroke
ScbStrokePtr scb_stroke_new()
{
    ScbStrokePtr ptr = g_new0(ScbStroke, 1);
    SCB_RET_NULL_IF(NULL == ptr, "Not enough memory for stroke!");
    
    // allocate points array
    if (!scb_points_new(&ptr->points, SCB_DEF_STROKE_POINTSIZE))
    {
        // could not alloc memory for list
        g_free(ptr);
        return NULL;
    }

    // init the stroke paramters
    ptr->style.color     = SCB_DEV_COLOR_BLACK;
    ptr->style.layer     = SCB_DEF_STROKE_LAYER;
    ptr->style.lineStyle = SCB_DEF_STROKE_LINESTYLE;
    ptr->style.penSize   = SCB_DEF_STROKE_PENSIZE;

    // init out bound rect. used to fast hit test
    ptr->rect.left   = INT_MAX;
    ptr->rect.top    = INT_MAX;
    ptr->rect.right  = INT_MIN;
    ptr->rect.bottom = INT_MIN;

    // init fast draw context 
    scb_fast_draw_reset_context();
    return ptr;
}

ScbStrokePtr scb_stroke_new_with_style(ScbStrokeStylePtr pStyle)
{
    SCB_RET_NULL_IF(NULL == pStyle, "Invalid pointer!");

    ScbStrokePtr ptr = g_new0(ScbStroke, 1);
    SCB_RET_NULL_IF(NULL == ptr, "Not enough memory for stroke!");
    
    // allocate points array
    if (!scb_points_new(&ptr->points, SCB_DEF_STROKE_POINTSIZE))
    {
        // could not alloc memory for list
        g_free(ptr);
        return NULL;
    }

    // init the stroke paramters
    ptr->style  = *pStyle;

    // init out bound rect. used to fast hit test
    ptr->rect.left   = INT_MAX;
    ptr->rect.top    = INT_MAX;
    ptr->rect.right  = INT_MIN;
    ptr->rect.bottom = INT_MIN;

    // init fast draw context 
    scb_fast_draw_reset_context();
    return ptr;
}

//------------------------------------------------------------------------
// release all memory used by the stroke
void scb_stroke_free(ScbStrokePtr stroke)
{
    SCB_RET_IF(NULL == stroke, "Attempt to release NULL pointer!");
    if (stroke->points.points)
    {
        scb_points_free(&stroke->points);
    }
    g_free(stroke);
}

//------------------------------------------------------------------------
// Add a point to storke
int scb_stroke_add_point(ScbStrokePtr stroke, ScbDevPointPtr point)
{
    SCB_RET_INT_IF(NULL == stroke, SCB_RET_ERR, "stroke pointer is NULL!");
    SCB_RET_INT_IF(NULL == point, SCB_RET_ERR, "Invalid point pointer!");

    // for debug. report points whose x or y is incorrect. 
    // It maybe not correct, the coordinates can be very large. 
#if (SCB_DEBUG_ON)
    // if (point->x >=  1024 || point->y >= 1024)
    // {
        // SCB_WARN("Odd points found! (%d %d)", point->x, point->y);
    // }
#endif 

    scb_points_append(&stroke->points, point);
    
    // adjust rectangle
    if (stroke->rect.left > point->x)
    {
        stroke->rect.left = point->x;
    }
    if (stroke->rect.right < point->x)
    {
        stroke->rect.right = point->x;
    }

    if (stroke->rect.top > point->y)
    {
        stroke->rect.top = point->y;
    }
    if (stroke->rect.bottom < point->y)
    {
        stroke->rect.bottom = point->y;
    }
    return SCB_RET_OK;
}

//------------------------------------------------------------------------
int scb_stroke_get_point_count(ScbStrokePtr stroke)
{
    SCB_RET_INT_IF(NULL == stroke || NULL == stroke->points.points, SCB_INVALID_COUNT, "Invalid pointer(s)!");
    return stroke->points.points->len;
}


//-----------------------------------------------------------------------
ScbPointPtr scb_stroke_get_point_data(ScbStrokePtr stroke)
{
    SCB_RET_NULL_IF(NULL == stroke, "Invalid pointer!");
    return scb_points_get_data(&stroke->points);
}

/*
void scb_stroke_fast_draw_add_point(ScbStrokePtr stroke, 
                                ScbDevPointPtr point)
{
    scb_fast_draw_record(point, 
        stroke->style.penSize, 
        stroke->style.color, 
        TRUE);
}

void scb_stroke_fast_draw_add_point_done(ScbStrokePtr stroke, 
                                     ScbDevPointPtr point)
{
    scb_fast_draw_record(point, 
        stroke->style.penSize,
        stroke->style.color,
        FALSE);
}

// draw points in pixel buffer
void scb_stroke_fast_draw_flush()
{
    scb_fast_draw();
    scb_fast_draw_reset_context();
}
*/

//------------------------------------------------------------------------
// Fast draw points by using the additional global points buffer.
// Here, we have two choices:
// 1. use a timer, but the application should have a message loop. Gtk/X11
// 2. check timestamp and points, draw them if necessary. 

void scb_stroke_fast_draw_point(ScbStrokePtr stroke, ScbDevPointPtr point)
{
    SCB_RET_IF(NULL == stroke || NULL == point, "NULL storke or point!");
        
    if (scb_fast_draw_now())
    {
        // draw it right now
        scb_fast_draw();

        // reset
        scb_fast_draw_reset_context();
    }

    // record this point
    scb_fast_draw_record(point, 
        stroke->style.penSize, 
        stroke->style.color, 
        TRUE);
}


//------------------------------------------------------------------------
// draw last point of stroke, reset fast draw context

void scb_stroke_fast_draw_point_done(ScbStrokePtr stroke, ScbDevPointPtr point)
{
    SCB_RET_IF(NULL == stroke || NULL == point, "NULL storke or point!");
        
    // add the last point 
    scb_fast_draw_record(point, 
        stroke->style.penSize,
        stroke->style.color,
        FALSE);

    // fast draw 
    scb_fast_draw();

    // reset
    scb_fast_draw_reset_context();
}


//------------------------------------------------------------------------
// fast draw the whole stroke
void scb_stroke_fast_draw(ScbStrokePtr stroke)
{
    SCB_RET_IF(NULL == stroke || NULL == stroke->points.points, "NULL pointer!");

    // construct a temp PointsBufPtr, copy stroke data into points buf
    DrvPointsBuf tmp;
    gboolean end_of_stroke;
    int i = 0;
    int len = stroke->points.points->len;
    ScbPointPtr pt = (ScbPointPtr)stroke->points.points->data;

    // do nothing
    if (len < 0) return;

    // collect valid points
    tmp.count = 0;

    // construct the buffer
    end_of_stroke = FALSE;
    for(i = 0; i < len; ++i, ++pt)
    {
        // split stroke in smaller ones containing max. SCB_DEF_FAST_DRAW_BUF_LEN valid points
        if (   pt->x >= SCB_DEF_BOUNDARY_LEFT  &&  pt->x <= SCB_DEF_BOUNDARY_RIGHT 
            && pt->y >= SCB_DEF_BOUNDARY_TOP   &&  pt->y <= SCB_DEF_BOUNDARY_BOTTOM )
        {                    
            // valid point: add to stroke to be drawn
            tmp.points[tmp.count].x = pt->x;
            tmp.points[tmp.count].y = pt->y;
            tmp.points[tmp.count].size = stroke->style.penSize;
            tmp.points[tmp.count].pen_down = TRUE;
            tmp.points[tmp.count].color = stroke->style.color;
            ++tmp.count;
            if (   i         >= len - 1
                || tmp.count >= SCB_DEF_FAST_DRAW_BUF_LEN)
            {
                end_of_stroke = TRUE;
            }
        }                
        else
        { 
            // invalid point: end stroke to be drawn, if any
            if (tmp.count > 0)
            {
                end_of_stroke = TRUE;
            }
        }

        // draw the valid (sub)stroke as needed
        if (end_of_stroke)
        {
            // force pen_up for last point of stroke
            tmp.points[tmp.count - 1].pen_down = FALSE;                
            scb_driver_draw(&tmp);
            tmp.count = 0;
            end_of_stroke = FALSE;
        }
    }
}

//------------------------------------------------------------------------
// hit test
// 1. check out bound and the point (fast check)
// 2. check points in the stroke
gboolean scb_stroke_point_hit_test(ScbStrokePtr stroke, 
                                   ScbDevPointPtr point,
                                   const ScbHitTestCtxPtr ctx)
{
    SCB_RET_FALSE_IF(NULL == stroke || NULL == point || NULL == ctx, 
        "Invalid pointer(s)!");

    // fast check
    ScbRect tmp; 
    double ratio = 1.0;
    if (fabs(stroke->style.zoom - ctx->zoom) >= e)
    {
        ratio = stroke->style.zoom / ctx->zoom;
    }
    tmp.left    = point->x * ratio - ctx->size;
    tmp.right   = point->x * ratio + ctx->size;
    tmp.top     = point->y * ratio - ctx->size;
    tmp.bottom  = point->y * ratio + ctx->size;
    if (!scb_is_rect_intersect(&stroke->rect, &tmp))
    {
        return FALSE;
    }

    // check each point in stroke
    ScbRect src;
    int size = stroke->style.penSize;
    int len = stroke->points.points->len;
    ScbPointPtr pts = (ScbPointPtr)stroke->points.points->data;
    while (len)
    {
        src.left    = pts->x - size;
        src.right   = pts->x + size;
        src.top     = pts->y - size;
        src.bottom  = pts->y + size;
        if (scb_is_rect_intersect(&src, &tmp))
        {
#if (SCB_DEBUG_ON)
            SCB_TRACE("\nrect1: (%d %d %d %d)\nrect2: (%d %d %d %d)",
                src.left, src.top, src.right, src.bottom,
                tmp.left, tmp.top, tmp.right, tmp.bottom);
#endif
            return TRUE;
        }
        ++pts; --len;
    }
    return FALSE;
}

//------------------------------------------------------------------------
// hit test with line. check line and line segment in stroke
// not consider the line width here...
gboolean scb_stroke_line_hit_test(ScbStrokePtr stroke, 
                                  ScbDevPointPtr point1,
                                  ScbDevPointPtr point2,
                                  const ScbHitTestCtxPtr ctx)
{
    SCB_RET_FALSE_IF(NULL == stroke || NULL == point1 || NULL == point2 || NULL == ctx, 
        "Invalid pointer(s)!");

    // fast check
    ScbRect tmp; 
    double ratio = 1.0;
    ScbDevPoint p1, p2;
    if (fabs(stroke->style.zoom - ctx->zoom) >= e)
    {
        ratio = stroke->style.zoom / ctx->zoom;
        p1.x = (int)point1->x * ratio; p1.y = (int)point1->y * ratio;
        p2.x = (int)point2->x * ratio; p2.y = (int)point2->y * ratio;
    }
    else
    {
        p1 = *point1; p2 = *point2;
    }


    tmp.left    = MIN(p1.x, p2.x) - ctx->size;
    tmp.right   = MAX(p1.x, p2.x) + ctx->size;
    tmp.top     = MIN(p1.y, p2.y) - ctx->size;
    tmp.bottom  = MAX(p1.y, p2.y) + ctx->size;
    if (!scb_is_rect_intersect(&stroke->rect, &tmp))
    {
        return FALSE;
    }

    // check each line segment in stroke
    int len = stroke->points.points->len;
    ScbPointPtr begin = (ScbPointPtr)stroke->points.points->data;
    ScbPointPtr end   = begin;

    // only one point
    if (len == 1)
    {
        if (scb_is_lines_intersect((ScbPointPtr)&p1, (ScbPointPtr)&p2, begin, end))
        {
#if (SCB_DEBUG_ON)
            SCB_TRACE("one point hit!");
            SCB_TRACE("line in stroke: (%d %d) - (%d %d)", 
                begin->x, begin->y, end->x, end->y);
            SCB_TRACE("user line: (%d %d) - (%d %d)",
                p1.x, p1.y, p2.x, p2.y);
#endif
            return TRUE;
        }
        return FALSE;
    }

    // others
    while (len > 1)
    {
        ++end;
        if (scb_is_lines_intersect((ScbPointPtr)&p1, (ScbPointPtr)&p2, begin, end))
        {
            // for debug
#if (SCB_DEBUG_ON)
            SCB_TRACE("line in stroke: (%d %d) - (%d %d)", 
                begin->x, begin->y, end->x, end->y);
            SCB_TRACE("user line: (%d %d) - (%d %d)",
                p1.x, p1.y, p2.x, p2.y);
#endif
            return TRUE;
        }
        begin = end; --len;
    }
    return FALSE;
}

//------------------------------------------------------------------------
// persistence support
// save path
// <strokes> ------------------------------------------ caller
//      <stroke attribute1, attribute2, ...> ---------- this function
//          100 100 1 ....
//      </stroke>
// </strokes>
gboolean scb_stroke_save(ScbStrokePtr stroke, ScbXmlPtr ptr)
{
    // construct xpath
    char xPath[SCB_MAX_XML_PATH] = {0};
    strncpy(xPath, ptr->xPath, SCB_MAX_XML_PATH);
    strncat(xPath, "/stroke", SCB_MAX_XML_PATH);
    snprintf(xPath, SCB_MAX_XML_PATH, "%s/stroke[%d]", ptr->xPath, ptr->index);
    // SCB_TRACE("xPath: %s", xPath);
    
    // create node, construct data, from stroke data to string
    const int LINE_BYTES = 33; // 2147483647 2147483647 2147483647\n
    int count = scb_stroke_get_point_count(stroke);
    if (count)
    {
        int len = LINE_BYTES * count;
        char *buf = g_new0(char, len);
        if (NULL == buf) return FALSE;
        char *tmp = buf;
        ScbPointPtr point = (ScbPointPtr)stroke->points.points->data;
        int* ppressure = (int *)stroke->points.pressures->data;
        // write information into buf
        while (count > 0)
        {
            tmp += snprintf(tmp, LINE_BYTES, "%d %d %d\n", 
                    point->x, point->y, *ppressure);
            --count; ++point; ++ppressure;
        }            
        ermXmlNewString(&ptr->handle, ptr->xPath, "stroke", buf);
        g_free(buf);
    }

    // set attributes
    ermXmlSetAttributeString(&ptr->handle, xPath, "color", 
        scb_dev_color_to_html_color(stroke->style.color), SCB_MAX_COLOR_LEN);
    ermXmlSetAttributeInt(&ptr->handle, xPath, "layer", stroke->style.layer);
    ermXmlSetAttributeInt(&ptr->handle, xPath, "penSize", stroke->style.penSize);
    ermXmlSetAttributeString(&ptr->handle, xPath, "linestyle", 
        scb_line_style_to_str(stroke->style.lineStyle), SCB_MAX_XML_PATH);
    // zoom factor
    char tmp[20] = {0}; snprintf(tmp, 20, "%lf", stroke->style.zoom);
    ermXmlSetAttributeString(&ptr->handle, xPath, "zoom", 
        tmp, SCB_MAX_XML_PATH);

    return TRUE;
}

//------------------------------------------------------------------------
// load from xml file
// caller should specify index
gboolean scb_stroke_load(ScbStrokePtr stroke, ScbXmlPtr ptr)
{
    // check
    SCB_RET_FALSE_IF(NULL == stroke || NULL == ptr, "Invalid pointer!");
        
    // construct xpath
    char xPath[SCB_MAX_XML_PATH] = {0};
    strncpy(xPath, ptr->xPath, SCB_MAX_XML_PATH);
    char * addr = xPath + strnlen(xPath, SCB_MAX_XML_PATH);
    snprintf(addr, SCB_MAX_XML_PATH, "/stroke[%d]", ptr->index);
    // SCB_TRACE("xPath %s", xPath);
    
    // read attributes
    const int MAX_BUF_LEN = SCB_MAX_XML_PATH;
    char buf[SCB_MAX_XML_PATH] = {0};
    ermXmlGetAttributeString(&ptr->handle, xPath, "color", buf, MAX_BUF_LEN);
    stroke->style.color = scb_html_color_to_dev_color(buf, MAX_BUF_LEN);
    ermXmlGetAttributeInt(&ptr->handle, xPath, "layer", &stroke->style.layer);
    ermXmlGetAttributeInt(&ptr->handle, xPath, "penSize", (int *)&stroke->style.penSize);
    ermXmlGetAttributeString(&ptr->handle, xPath, "linestyle", buf, MAX_BUF_LEN);
    stroke->style.lineStyle = scb_line_style_from_str(buf);
    ermXmlGetAttributeString(&ptr->handle, xPath, "zoom", buf, MAX_BUF_LEN);
    sscanf(buf, "%lf", &stroke->style.zoom);

    // read stroke data from stroke node
    int len = 0;
    const char *data  = ermXmlGetStringBuffer(&ptr->handle, xPath, &len);
    addr = (char *)data;           // remember the address
    if (len <= 0 || NULL == data)
    {
#if (SCB_DEBUG_ON)
        // in fact, it's not an error. when no data could read, just return false
        // SCB_TRACE("No data in this stroke!");
#endif
        return FALSE;
    }

    // here, we assume that the xmlchar is zero terminated
    ScbDevPoint point;
    while (data && 0 != *data)
    {
        if (3 != sscanf(data, "%d %d %d", &point.x, &point.y, &point.pressure))
        {
            break;
        }            

        scb_stroke_add_point(stroke, &point);
        data = strchr(data, '\n');              // to next begin
        if (NULL == data)
        {
            SCB_ERROR("Could not find new line!");
            ermXmlFreeStringBuffer(addr);
            return FALSE;
        }
        ++data;
    }
    ermXmlFreeStringBuffer(addr);
    return TRUE;
}

void scb_stroke_set_color(ScbStrokePtr stroke, const ScbDevColor color)
{
    SCB_RET_IF(NULL == stroke, "Invalid pointer!");
    stroke->style.color = color;
}


ScbDevColor scb_stroke_get_color(ScbStrokePtr stroke)
{
    if (NULL == stroke)
    {
        return SCB_DEV_COLOR_UNKNOWN;
    }
    return stroke->style.color;
}


//------------------------------------------------------------------------
// dump
void scb_stroke_dump(ScbStrokePtr ptr)
{
    SCB_RET_IF(NULL == ptr || NULL == ptr->points.points, "");
    
    int count = ptr->points.points->len;
    ScbPointPtr p = (ScbPointPtr)ptr->points.points->data;
    int *pp = (int *)ptr->points.pressures->data;
    while(count > 0)
    {
        SCB_DUMP("\t(%d,\t%d,\t%d)", p->x, p->y, *pp);
        --count; ++ p; ++pp;
    }
}
