/*
 * This file is part of scribble.
 *
 * scribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * scribble is distributed in the hope that it will be useful,
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

/*
*  C Implementation: ink.c
*
* Description: 
* brief- ink file construct,destroy basic functions
*
* Author: Jian Shen, (C) 2006
* Original Author: Yehong Wang.
*
* Copyright: See COPYING file that comes with this distribution
*
*/
#include <memory.h>
#include <stdlib.h>

#include "ink.h"
#include "ScribbleLog.h"

/* functions about construct,destroy objects */
// malloc the memory for ink object
PtrInk construct_ink ()
{
    PtrInk ink = NULL;

    ink = malloc (sizeof (Ink));
    if (ink)
        memset (ink, 0, sizeof (Ink));
  
    SB_INKPRINTF("return %p \n", ink);
    return ink;
}

// free the memory for ink object including its children objects such as strokes, points
void destroy_ink (PtrInk ink)
{
    PtrStroke stroke = NULL;
    if (ink == NULL)
        return;
    stroke = ink->firstStroke;
    while (stroke)
        stroke = destroy_stroke (stroke);

    SB_INKPRINTF("return (%p)\n", ink);
    free (ink);
}

// malloc the memory for stroke object
PtrStroke construct_stroke ()
{
    PtrStroke stroke = NULL;

    stroke = malloc (sizeof (Stroke));
    if (stroke)
        memset (stroke, 0, sizeof (Stroke));

  //init range
    stroke->min_x = stroke->min_y = INT_MAX;
    stroke->max_x = stroke->max_y = INT_MIN;
    SB_INKPRINTF("initial construct %p \n", stroke);
    return stroke;
}

// free the memory for stroke object including its children objects such as points
PtrStroke destroy_stroke (PtrStroke stroke)
{
    PtrInkPoint point = NULL;
    PtrStroke nextStroke = NULL;
    if (stroke == NULL)
        return NULL;
    nextStroke = stroke->nextStroke;
    point = stroke->firstPoint;
    while (point)
        point = destroy_point (point);

    SB_INKPRINTF("return(%p)\n", stroke);
    free (stroke);
    return nextStroke;
}

// malloc the memory for point object
PtrInkPoint construct_point ()
{
    PtrInkPoint point = NULL;

    point = malloc (sizeof (InkPoint));
    if (point)
        memset (point, 0, sizeof (InkPoint));

    SB_INKPRINTF("return %p \n", point);
    return point;
}

// free the memory for point object
PtrInkPoint destroy_point (PtrInkPoint point)
{
    PtrInkPoint nextPoint = NULL;
    if (point == NULL)
        return NULL;
    nextPoint = point->nextPoint;

    SB_INKPRINTF("return %p \n", point);
    free (point);
    return nextPoint;
}

// add one new stroke to ink object
// if succeeded return 1, else return 0
unsigned char ink_add_stroke (PtrInk ink, PtrStroke stroke)
{
    if (ink == NULL || stroke == NULL)
    {
        SB_INKERRPRINTF("failed:incorrect arguments\n");
        return 0;
    }

    if (ink->lastStroke == NULL)
    {
        ink->firstStroke = ink->lastStroke= stroke;
        ink->nStrokes = 1;
    }
    else
    {
        ink->lastStroke=ink->lastStroke->nextStroke = stroke;
        ink->nStrokes += 1;
    }
    return 1;
}

// get the number of strokes in ink object
int get_num_of_strokes (PtrInk ink)
{
    if (ink == NULL)
        return 0;
    return ink->nStrokes;
}

// add one new point to stroke object
// if succeeded return 1, else return 0
unsigned char ink_add_point(PtrStroke stroke, PtrInkPoint point)
{
    if (stroke == NULL || point == NULL)
    {
        SB_INKERRPRINTF("failed:incorrect arguments\n");
        return 0;
    }

    if (stroke->lastPoint == NULL)
    {
        stroke->lastPoint=stroke->firstPoint = point;
        stroke->nPoints = 1;
    }
    else
    {
        stroke->lastPoint=stroke->lastPoint->nextPoint = point;
        stroke->nPoints += 1;
    }

    //recompute the range, if saved, maybe duplicated when load from file!!
    if( point->x < stroke->min_x) stroke->min_x=point->x;
    if( point->x > stroke->max_x) stroke->max_x=point->x;
    if( point->y < stroke->min_y) stroke->min_y=point->y;
    if( point->y > stroke->max_y) stroke->max_y=point->y;

    return 1;
}

int get_num_of_points (PtrStroke stroke)
{
    if (stroke == NULL)
        return 0;
    return stroke->nPoints;
}
