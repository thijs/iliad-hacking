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

#ifndef _SCRIBBLECORE_H
#define _SCRIBBLECORE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

#include <sys/ioctl.h>
#include <fcntl.h>

#include "ScribbleDefine.h"

void initScribbleCoreCtx(PScribbleCoreCtx pScribbleCoreCtx);

//destroy core context
void destroyScribbleCoreCtx(PScribbleCoreCtx pScribbleCoreCtx);

//generate a new  current stroke 
void genNewCurrentStroke(PScribbleCoreCtx pScribbleCoreCtx);

//flush deleted stroke on screen
void core_flush_delstroke_onscreen(PScribbleUICtx pScribbleUICtx);

void core_process_points(PScribbleUICtx pScribbleUICtx);

int core_addpoint2buf(PointsBuf *ppointsBuf,int x,int y,
		int iPenSize,unsigned char iHwColor,unsigned char iPenDown);

void core_add_point(PScribbleUICtx pScribbleUICtx,int x, int y);

void core_eraseStrokes(PScribbleUICtx pScribbleUICtx,GdkPoint *pCurPoint);

//redraw current strokes and background pixel to pixmap.
int core_redrawBgAndStrks(PScribbleUICtx pScribbleUICtx);

//load specified page and resultimage
int loadPageAndResultImg(PScribbleUICtx pScribbleUICtx);

//flush current pixmap to screen object.
void flushPixMap(PScribbleUICtx pScribbleUICtx);

//save current ink of page,result image will saved when quitting.
//bAppQuit--if scribble is to quit.
int savePageAndResultImg(PScribbleUICtx pScribbleUICtx,gboolean bAppQuit);


#ifdef __cplusplus
}
#endif

#endif
