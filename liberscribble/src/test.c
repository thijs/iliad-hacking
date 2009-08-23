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

#include "scb.h"

#define TRACE(x, args...) fprintf(stderr, "(SCB_TEST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args)


// construct a doc
// add pages
// add strokes
// add point
// dump
// remove 
void testDoc()
{
    // doc
    ScbDocPtr doc = scb_doc_new();
    if (NULL == doc)
    {
        TRACE("Could not construct a scribble document!");
        return;
    }

    // page list
    ScbPagesPtr pages = scb_doc_get_pages(doc);

    // add some pages
    {
        ScbPageId id;
        ScbPagePtr page = NULL;
        int i = 0; 
        for(; i < 5; ++i)
        {
            scb_page_id_from_int(&id, i);
            page = scb_page_new(&id);
            scb_pages_add_page(pages, page);
        }
        scb_pages_dump(pages);
    }

    scb_doc_free(doc);
}





int main(int argc, char * argv[])
{
    testDoc();
    return 0;
}
