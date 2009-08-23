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
* brief- functions to draw ink data structure
*
* Author: Jian Shen, (C) 2006
* Original Author: Yehong Wang.
*
* Copyright: See COPYING file that comes with this distribution
*
*/
#include <stdlib.h>
#include <gtk/gtk.h>

#include "ink_draw.h"
#include "ScribbleLog.h"
#include "ScribbleUtils.h"


static GdkDrawable *pCurrDrawable=NULL;
static GdkGC       *pCurrGDKGC=NULL;

int ink_setDrawable(GdkDrawable *pDrawable)
{
    //avoiding duplicated efforts
    if(pCurrDrawable==pDrawable) return 0;

    pCurrDrawable=pDrawable;

    if(pCurrGDKGC)
        g_object_unref(pCurrGDKGC);

    pCurrGDKGC=gdk_gc_new(pDrawable);
    if(NULL==pCurrGDKGC)
    {
        SB_INKERRPRINTF("error when create GDKGC\n");
        return -1;
    }
    SB_INKPRINTF("create gdk gc successfully\n");
    return 0;
}

static int iLastPenSize=-1;
//set pen size attributes
gboolean setPenSizeAtrr(int iPenSize)
{
    if(iLastPenSize != iPenSize)
    {
        gdk_gc_set_line_attributes(pCurrGDKGC,
                                   iPenSize,
                                   GDK_LINE_SOLID,
                                   GDK_CAP_PROJECTING,
                                   GDK_JOIN_MITER);
        iLastPenSize=iPenSize;
    }
    return TRUE;
}

static int iLastHwColor=-1;
//set pen color attributes
gboolean setPenColorAttr(GdkColor* pGdkColor)
{
    HW_COLOR iColor=Util_HWCOLORFromGdkColor(pGdkColor);
    if(iLastHwColor != iColor)
    {
        gdk_gc_set_rgb_fg_color(pCurrGDKGC,pGdkColor);
        iLastHwColor=iColor;
    }
    return TRUE;
}

void ink_draw_line (int x0, int y0, int x1, int y1)
{
    GdkPoint points[2];
    points[0].x=(gint)x0;
    points[0].y=(gint)y0;
    points[1].x=(gint)x1;
    points[1].y=(gint)y1;
    gdk_draw_lines(pCurrDrawable,pCurrGDKGC,points,2);
}

// display ink object
void draw_ink (PtrInk ink)
{
    int nStrokes;
    PtrStroke stroke = NULL;
    if (ink == NULL)
        return;

    stroke = ink->firstStroke;
    nStrokes = get_num_of_strokes (ink);
    SB_INKPRINTF("nStrokes=%d\n", nStrokes);
    if (stroke == NULL || nStrokes <= 0)
    {
        return;
    }

    while (stroke)
    {
        draw_stroke (stroke);
        stroke = stroke->nextStroke;
    }
}

// display stroke object by call scribble_draw_line
void draw_stroke (PtrStroke stroke)
{
    PtrInkPoint point = NULL;
    int nPoints;
    if (stroke == NULL)
        return;

    point = stroke->firstPoint;
    nPoints = get_num_of_points (stroke);
    if (point == NULL || nPoints <= 0)
        return;

    //select pensize
    setPenSizeAtrr(stroke->iPenSize);
    //set pen color
    setPenColorAttr(&stroke->gdkColor);

    SB_INKPRINTF("(nPoints=%d)\n", nPoints);
    // only one point in the stroke
    if ( 1==nPoints )
    {
        ink_draw_line(point->x, point->y,point->x, point->y);
        return;
    }

    while (point->nextPoint)
    {
        ink_draw_line(point->x, point->y, 
                      point->nextPoint->x,point->nextPoint->y);
        point = point->nextPoint;
    }
}
