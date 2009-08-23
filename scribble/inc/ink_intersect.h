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

#ifndef _INK_INTERSECT_H
#define _INK_INTERSECT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ink.h"

  typedef struct _StrokeIntersection_t
  {
    PtrInkPoint beginPt;
    PtrInkPoint endPt;
    struct _StrokeIntersection_t *pNextSection;
  } StrokeIntersection_t;

  typedef struct _StrokeIntersections_t
  {
    int                     nSections;
    //point to the first intersection,
    StrokeIntersection_t   *interSection;
  } StrokeIntersections_t, *PtrStrokeIntersections_t;

  //a point out of rectangle? area-left corner(x1,y1),right corner(x2,y2)
  //return 1 if yes.
  int isPointOutOfRange(int x,int y,int x1,int y1,int x2,int y2);
  int isPointOutOfStroke(PtrStroke pStroke,GdkPoint *p);
  //int isLineCrossStrokeRange(PtrStroke pStroke,GdkPoint *p0,GdkPoint *p1);
  //compute dest_w,dest_h for tolerance for erase few-points stroke.
  //it can improve the performance of eraseing.
  gboolean isLineCrossStrokeRange(PtrStroke pStroke,GdkPoint *p0,GdkPoint *p1,
                                  int* p_rec_width,int* p_rect_height);
  
  //return 1 if lines intersect, 0 if not
  int lineIntersects(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
 
  int isIntersects(PtrStroke pStroke, GdkPoint *srcPt1, GdkPoint *srcPt2);

  int hitTest(PtrStroke pStroke, GdkPoint *point);

  //whether the line interset with stroke.
  PtrStrokeIntersections_t findIntersections(PtrStroke pStroke,
				GdkPoint *srcPt1, GdkPoint *srcPt2);

  void delStroke(PtrInk pink,PtrStroke pPrev,PtrStroke *ppCur,PtrInk pDelInk);
  //erase the strokes intersected with specified line
  void delStrokesByLine(PtrInk pink,GdkPoint *p0,GdkPoint *p1,PtrInk pDelInk);
  //erase the intersected lines with specified line
  void delInterSectLines(PtrInk pink,GdkPoint *p0,GdkPoint *p1);
  
#ifdef __cplusplus
}
#endif

#endif
