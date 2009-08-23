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

#ifndef SCBPAGES_H_
#define SCBPAGES_H_

#include "scbpage.h"
#include <glib.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _ScbPages
{
    GList * pages;
}ScbPages;
typedef ScbPages * ScbPagesPtr;

// construct a new page
gboolean scb_pages_new(ScbPagesPtr ptr);

// free 
void scb_pages_free(ScbPagesPtr ptr);

// it will check whether or not the page exist
gboolean scb_pages_add_page(ScbPagesPtr pages, ScbPagePtr page);

// detach ScbPage from list without freeing the page
// detach means the page detaches from the list
ScbPagePtr scb_pages_detach_page(ScbPagesPtr pages, ScbPageIdPtr id);

// remove page and remove it from page list
void scb_pages_remove_page(ScbPagesPtr pages, ScbPageIdPtr id);

// get page count
int scb_pages_get_count(ScbPagesPtr ptr);

// get certain page
ScbPagePtr scb_pages_get_page(ScbPagesPtr pages, ScbPageIdPtr id);

// change page id
gboolean scb_pages_change_page_id(ScbPagesPtr pages, ScbPageIdPtr oldId, 
                         ScbPageIdPtr newId);

// save to xml file
gboolean scb_pages_save(ScbPagesPtr pages, ScbXmlPtr xml);

// load from xml file
gboolean scb_pages_load(ScbPagesPtr pages, ScbXmlPtr xml);

// dump
void scb_pages_dump(ScbPagesPtr pages);

#ifdef __cplusplus
}
#endif

#endif
