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

//
// C++ Interface: ScribbleDefine
//
// Description: 
//
//
// Author: Jian Shen, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _SCRIBBLEDEFINE_H
#define _SCRIBBLEDEFINE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#include "ink.h"
#include "FileManager.h"

#include <liberdm/erdm.h>
#include <liberipc/eripc.h>
#include <liberipc/eripctoolbar.h>
#include <liberipc/eripcviewer.h>
#include <liberipc/eripcpagebar.h>

#define PIXELBUFSIZE       25

typedef struct _PointInfo
{
	unsigned short x;
	unsigned short y;
	unsigned char  size;
	unsigned char  color;
  	unsigned char  pen_down;
}PointInfo;

typedef struct _PointsBuf
{
  unsigned int count;
  PointInfo points [PIXELBUFSIZE];
} PointsBuf;

#define FBIO_IOCTL_BASE		  'v'
//x represent pensize
#define FBIO_DRAWPIXELS  _IOW(FBIO_IOCTL_BASE, 3, PointsBuf)

struct color
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
};

typedef struct _ScribbleCoreCtx
{
    //points buffer,avoid frequently flushing screen
    PointsBuf pointsBuf;
    //ink data to hold the strokes.
    PtrInk pink;
    //deleted ink file
    PtrInk pDelInk;
    //current stroke
    PtrStroke pCurrStroke;
    //has saved?
    gboolean bNeedSave;
} ScribbleCoreCtx;

typedef ScribbleCoreCtx *PScribbleCoreCtx;

typedef enum {
  SIZE_1PIX=1,
  SIZE_3PIX=3,
  SIZE_5PIX=5,
  SIZE_7PIX=7,
}PEN_SIZE;//pen size ,in pixels

//context for scribble UI
typedef struct _ScribbleUICtx
{
     FileManager fileManager;
     //scribble core ctx
     ScribbleCoreCtx scribbleCoreCtx;

     //is pen_down??
     gboolean pen_down;
     /* Backing pixmap for drawing area */
     GdkPixmap *pixmap;     
     //initial background pixmap.
     GdkPixmap *initBgPixMap;
     //last template name.
     const char *sLastTmplImg;
     
     GtkWidget *drawing_area;

     //handler to framebuffer;
     int  fbdev;
     //last point in the screen
     GdkPoint lastPoint;
} ScribbleUICtx;

typedef ScribbleUICtx *PScribbleUICtx;

#ifdef __cplusplus
}
#endif

#endif
