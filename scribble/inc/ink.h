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

#ifndef _INK_H
#define _INK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <gtk/gtk.h>

  typedef struct _Ink *PtrInk;
  typedef struct _Stroke *PtrStroke;
  typedef struct _InkPoint *PtrInkPoint;

  typedef struct _Ink
  {
    int nStrokes;
    PtrStroke firstStroke;
    PtrStroke lastStroke;
  } Ink;


  typedef struct _Stroke
  {
    //for defining the stroke area
    int min_x;
    int min_y;
    int max_x;
    int max_y;

    ////pen size and color,should save to inkfile
    int iPenSize;
    GdkColor gdkColor;//only save it's rgb value

    int nPoints;
    PtrInkPoint firstPoint;
    PtrInkPoint lastPoint;
    PtrStroke nextStroke;
  } Stroke;

  typedef struct _InkPoint
  {
    int x;
    int y;
    PtrInkPoint nextPoint;
  } InkPoint;

  extern PtrInk construct_ink ();
  extern void destroy_ink (PtrInk ink);

  extern PtrStroke construct_stroke ();
  extern PtrStroke destroy_stroke (PtrStroke stroke);

  extern PtrInkPoint construct_point ();
  extern PtrInkPoint destroy_point (PtrInkPoint point);

  extern unsigned char ink_add_stroke (PtrInk ink, PtrStroke stroke);
  extern int get_num_of_strokes (PtrInk ink);

  extern unsigned char ink_add_point (PtrStroke stroke, PtrInkPoint point);
  extern int get_num_of_points (PtrStroke stroke);
  
#ifdef __cplusplus
}
#endif

#endif				/* _INK_H */
