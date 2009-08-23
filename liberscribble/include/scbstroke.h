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

#ifndef SCBSTROKE_H_
#define SCBSTROKE_H_
#include "scbcolor.h"
#include "scbpoints.h"
#include "scbtype.h"
#include "scbxml.h"
#include <glib.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _ScbStrokeStyle
{
    int             penSize;    
    int             layer;      // used when display/erase 
    ScbDevColor     color;
    ScbLineStyle    lineStyle;  
    int             attributes; // stroke attributes
    double          zoom;
}ScbStrokeStyle;
typedef ScbStrokeStyle * ScbStrokeStylePtr;

typedef struct _ScbStroke
{
    ScbRect         rect;       // out bound, fast hit test
    ScbStrokeStyle  style;
    ScbPoints       points;     // include pressure information
}ScbStroke;
typedef ScbStroke * ScbStrokePtr;

typedef struct _ScbHitTestCtx
{
    int             size;       // hit test size
    double          zoom;
}ScbHitTestCtx;
typedef ScbHitTestCtx * ScbHitTestCtxPtr;

// construct a stroke
ScbStrokePtr scb_stroke_new();

// construct a stroe with specified style
ScbStrokePtr scb_stroke_new_with_style(ScbStrokeStylePtr ptr);

// release all memory used by the stroke
void scb_stroke_free(ScbStrokePtr stroke);

// get length
int scb_stroke_get_point_count(ScbStrokePtr stroke);

// get point data
ScbPointPtr scb_stroke_get_point_data(ScbStrokePtr stroke);

// add a ScbPoint to the stroke, no UI update.
// the only entry to add point
int scb_stroke_add_point(ScbStrokePtr stroke, 
                          ScbDevPointPtr point);

// add to pixel buffer
// void scb_stroke_fast_draw_add_point(ScbStrokePtr stroke, 
//                                ScbDevPointPtr point);

//void scb_stroke_fast_draw_add_point_done(ScbStrokePtr stroke, 
//                                     ScbDevPointPtr point);

// draw points in pixel buffer
//void scb_stroke_fast_draw_flush();


// fast_draw the point adding
// In testing
// By using ioctl, draw operation will take about 0.2 second to 
// draw a stroke (25 points), while mouse event happens every 0.00048 second
// so some points are ignored. 
// The result is that a circle looks like polygon

void scb_stroke_fast_draw_point(ScbStrokePtr stroke, 
                                ScbDevPointPtr point);

// draw the last point added
void scb_stroke_fast_draw_point_done(ScbStrokePtr stroke, 
                                     ScbDevPointPtr point);


// fast draw the whole stroke according current style
void scb_stroke_fast_draw(ScbStrokePtr stroke);

// have a hit test
gboolean scb_stroke_point_hit_test(ScbStrokePtr stroke, 
                                   ScbDevPointPtr point, 
                                   const ScbHitTestCtxPtr ctx);

// hit test with line
gboolean scb_stroke_line_hit_test(ScbStrokePtr stroke, 
                                  ScbDevPointPtr point1,
                                  ScbDevPointPtr point2,
                                  const ScbHitTestCtxPtr ctx);

// attribute settings, input/output
void scb_stroke_set_color(ScbStrokePtr stroke, const ScbDevColor color);
ScbDevColor scb_stroke_get_color(ScbStrokePtr stroke);

// persistence support: save / load
gboolean scb_stroke_save(ScbStrokePtr stroke, ScbXmlPtr);
gboolean scb_stroke_load(ScbStrokePtr stroke, ScbXmlPtr);

// dump
void scb_stroke_dump(ScbStrokePtr ptr);

#ifdef __cplusplus
}
#endif

#endif
