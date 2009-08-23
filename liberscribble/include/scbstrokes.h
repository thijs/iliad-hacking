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

#ifndef SCBSTROKES_H_
#define SCBSTROKES_H_
#include "scbstroke.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _ScbStrokes
{
    GList * strokes;
}ScbStrokes;
typedef ScbStrokes * ScbStrokesPtr;

// construct a new stroke list
gboolean scb_strokes_new(ScbStrokesPtr ptr);

// free operation will free the list item and list
void scb_strokes_free(ScbStrokesPtr strokes);

// empty operation will clear all items but does not remove the list
void scb_strokes_empty(ScbStrokesPtr strokes);

// detach operation destory the list without freeing each element
// detach used when caller wants a temp container for existing items
void scb_strokes_detach(ScbStrokesPtr strokes);

// get count
int scb_strokes_get_count(const ScbStrokesPtr strokes);

// add stroke to the strokes list
void scb_strokes_add_stroke(ScbStrokesPtr strokes, 
                            ScbStrokePtr stroke);

// copy from one list to the other
void scb_strokes_add_strokes(ScbStrokesPtr dst, 
                            const ScbStrokesPtr src);

// detach stroke from list without freeing stroke memory
// notice, caller should release memory of stroke returned
// caller can also add the return value to other stroke container
ScbStrokePtr scb_strokes_detach_stroke(ScbStrokesPtr strokes, 
                            ScbStrokePtr stroke);

// hit test, the result is a stroke list, empty means no intersect
// caller should call scb_stroke_detach
ScbStrokesPtr scb_strokes_point_hit_test(ScbStrokesPtr strokes, 
                                   ScbDevPointPtr point,
                                   const ScbHitTestCtxPtr ctx);

// hit test with line
// caller should call scb_stroke_detach
ScbStrokesPtr scb_strokes_line_hit_test(ScbStrokesPtr strokes, 
                                  ScbDevPointPtr point1,
                                  ScbDevPointPtr point2,
                                  const ScbHitTestCtxPtr ctx);

void scb_strokes_erase_fast_draw(ScbStrokesPtr strokes);

// save / load
gboolean scb_strokes_save(ScbStrokesPtr strokes, ScbXmlPtr ptr);
gboolean scb_strokes_load(ScbStrokesPtr strokes, ScbXmlPtr ptr);

// dump
void scb_strokes_dump(ScbStrokesPtr strokes);

#ifdef __cplusplus
}
#endif

#endif
