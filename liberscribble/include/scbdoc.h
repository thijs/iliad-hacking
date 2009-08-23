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

#ifndef SCBDOC_H_
#define SCBDOC_H_

//////////////////////////////////////////////////////////////////////////
// scribble library document definition
//////////////////////////////////////////////////////////////////////////

#include "scbpath.h"
#include "scbpages.h"

#ifdef __cplusplus
extern "C"
{
#endif


// maintain current state, such as current color / pen size/ pen style
// caller can update its UI widget according to doc state
//
// toolbar cmd ---> current state  ---> new style
//                       |
//                       -------------> new state
//                       |
//                       -------------> update UI by macro
// all platform related function is defined by macro

typedef enum _ScbTBState
{
    SCB_TBS_NONE, 
    SCB_TBS_SCRIBBLE,
    SCB_TBS_SELECT_PEN_SIZE,
    SCB_TBS_SELECT_LINE_COLOR,
    SCB_TBS_ERASE,
    SCB_TBS_INVALID
}ScbTBState;

// map a icon to a scribble toolbar state, so that scribble library can handle 
// toolbar request
typedef struct _ScbTBSItem
{
    ScbTBState  state;
    int         iconId;
}ScbTBSItem;
typedef ScbTBSItem* ScbTBSItemPtr;

typedef struct _ScbDocContext
{
    ScbStrokeStyle  curStrokeStyle;     // current style, in runtime
    ScbTBState      curState;           // current state
    GArray *        table;              // icon and action mapping table
    int             appId;              // current appid
}ScbDocContext;

typedef struct _ScbDoc
{
    ScbPath         path;
    ScbPages        pages;
    ScbDocContext   context;
}ScbDoc;
typedef ScbDoc * ScbDocPtr;

// construct doc
ScbDocPtr scb_doc_new();

// free
void scb_doc_free(ScbDocPtr doc);

// open the document
ScbDocPtr scb_doc_open(ScbPathPtr ptr);

// make sure exist, try to open it, if not exist or fail, create a new one
ScbDocPtr scb_doc_make_sure_exist(ScbPathPtr ptr);

// save
gboolean scb_doc_save(ScbDocPtr doc);

// saveAs
gboolean scb_doc_saveAs(ScbDocPtr doc, ScbPathPtr ptr);

// composite 
ScbPagesPtr scb_doc_get_pages(ScbDocPtr doc);
ScbStrokeStylePtr scb_doc_get_current_stroke_style(ScbDocPtr doc);

// make caller easily to find a page
ScbPagePtr scb_doc_get_page(ScbDocPtr doc, ScbPageIdPtr id);

// here, if duplicated id occurs, do nothing. so user can always
// use add page operation 
gboolean scb_doc_add_page(ScbDocPtr doc, ScbPagePtr page);

// partially open/save. not necessary to open the whole file
// From testing, it's not a good idea to open page directly. 
// Remove them
// ScbPagePtr scb_doc_open_page_directly(ScbPathPtr ptr, ScbPageIdPtr id);
// gboolean scb_doc_save_page_directly(ScbPathPtr ptr, ScbPagePtr page);

// dump
void scb_doc_dump(ScbDocPtr ptr);

// context and state
void scb_doc_init_context(ScbDocPtr ptr);
void scb_doc_free_context(ScbDocPtr ptr);
void scb_doc_add_map_item(ScbDocPtr doc, const ScbTBSItemPtr ptr);
ScbTBState scb_doc_get_current_state(ScbDocPtr doc);
gboolean scb_state_is_scribble(ScbDocPtr doc);
gboolean scb_state_is_erase(ScbDocPtr doc);


#ifdef __cplusplus
}
#endif

#endif
