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

#include "scbutil.h"
#include <math.h>
#include <glib.h>
#include <scblog.h>
//#define MIN(a,b) (((a)<(b))?(a):(b))
//#define MAX(a,b) (((a)>(b))?(a):(b))

/*************************************************************************

* FUNCTION:   CCW (CounterClockWise)
*
* PURPOSE
* Determines, given three points, if when travelling from the first to
* the second to the third, we travel in a counterclockwise direction.
*
* RETURN VALUE
* (int) 1 if the movement is in a counterclockwise direction, -1 if
* not.
*************************************************************************/ 
int CCW(const ScbPointPtr p0, const ScbPointPtr p1, const ScbPointPtr p2)
{
    long dx1, dx2 ;
    long dy1, dy2 ;

    dx1 = p1->x - p0->x ; dx2 = p2->x - p0->x ;
    dy1 = p1->y - p0->y ; dy2 = p2->y - p0->y ;

    /* This is basically a slope comparison: we don't do divisions because

    * of divide by zero possibilities with pure horizontal and pure
    * vertical lines.
    */ 
    return ((dx1 * dy2 > dy1 * dx2) ? 1 : -1) ;
}


/*************************************************************************
* FUNCTION:   Intersect
*
* PURPOSE
* Given two line segments, determine if they intersect.
*
* RETURN VALUE
* TRUE if they intersect, FALSE if not.
*************************************************************************/ 
gboolean Intersect(const ScbPointPtr p1, const ScbPointPtr p2, const ScbPointPtr p3, const ScbPointPtr p4)
{
    return ((( CCW(p1, p2, p3) * CCW(p1, p2, p4)) <= 0)
        && (( CCW(p3, p4, p1) * CCW(p3, p4, p2)  <= 0) )) ;
}

//------------------------------------------------------------------------
// check two rectangles is intersect or not
// these two rectangles should be normalized. 
// We do not check them here.
gboolean scb_is_rect_intersect(const ScbRectPtr r1, 
                               const ScbRectPtr r2)
{
    return ! ( r2->left > r1->right 
        || r2->right < r1->left 
        || r2->top > r1->bottom 
        || r2->bottom < r1->top 
        );
}

//------------------------------------------------------------------------
gboolean scb_is_lines_intersect(const ScbPointPtr p1, 
                                const ScbPointPtr p2,
                                const ScbPointPtr p3, 
                                const ScbPointPtr p4)
{
    gboolean bRet = ((( CCW(p1, p2, p3) * CCW(p1, p2, p4)) <= 0)
        && (( CCW(p3, p4, p1) * CCW(p3, p4, p2)  <= 0) )) ;

   
    return bRet;
}
