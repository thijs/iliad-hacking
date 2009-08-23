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

#ifndef SCBPAGE_H_
#define SCBPAGE_H_

#include "scbconfig.h"
#include "scbstrokes.h"

//////////////////////////////////////////////////////////////////////////
// scribble library page definition
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

//========================================================================
// page id
typedef struct _ScbPageId
{
    char id[SCB_MAX_PAGEID_LEN];
}ScbPageId;
typedef ScbPageId * ScbPageIdPtr;

// page style
typedef struct _ScbPageStyle
{
    int             orientation;    // can be 0, 90, 180, 270
    ScbColor        bgColor;
}ScbPageStyle;

// page operation history, undo / redo support
typedef struct _ScbPageOptHist
{
    // todo. maybe a list of operation history is needed
}ScbPageOptHist;

// erase 
typedef struct _ScbPageEraseCtx
{
    ScbDevPoint     p1, p2;
    double          zoom;
    gboolean        bInit;
}ScbPageEraseCtx;
typedef ScbPageEraseCtx * ScbPageEraseCtxPtr;

void scb_page_id_from_int(ScbPageIdPtr ptr, const int number);
void scb_page_id_from_str(ScbPageIdPtr ptr, const char *str);


//========================================================================
// page
typedef struct _ScbPage
{
    ScbPageStyle    style;
    ScbRect         rect;
    ScbPageId       id;
    ScbPageOptHist  actions;
    ScbStrokes      strokes;
    ScbStrokes      delStrokes;
}ScbPage;
typedef ScbPage * ScbPagePtr;

// construct a new empty scribble page
ScbPagePtr scb_page_new();

// release
void scb_page_free(ScbPagePtr ptr);

// clear all items from list without release memory
void scb_page_clear(ScbPagePtr ptr);

// set page id
void scb_page_set_id(ScbPagePtr page, ScbPageIdPtr id);

// retrieve page attributes
int scb_page_get_stroke_count(ScbPagePtr ptr);

// strokes
gboolean scb_page_add_stroke(ScbPagePtr page, ScbStrokePtr stroke);
ScbStrokesPtr scb_page_get_strokes(ScbPagePtr page);

// erase. inside page, it should maintain a point
void scb_page_erase_fast_draw(ScbPagePtr ptr, ScbDevPointPtr point);
void scb_page_erase_fast_draw_done(ScbPagePtr ptr, ScbDevPointPtr point);

// erase hit test
void scb_page_erase_init(ScbPageEraseCtxPtr ptr);
ScbStrokesPtr scb_page_erase_hit_test(ScbPagePtr page, ScbDevPointPtr point, ScbPageEraseCtxPtr ptr);


// save / load
gboolean scb_page_save(ScbPagePtr page, ScbXmlPtr ptr);
gboolean scb_page_load(ScbPagePtr page, ScbXmlPtr ptr);

// remove this function
// ScbPagePtr scb_page_load_from_xpath(ScbXmlPtr ptr);


// dump
void scb_page_dump(ScbPagePtr ptr);

#ifdef __cplusplus
}
#endif

#endif
