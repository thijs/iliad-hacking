/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file pagebar.cpp
 * \brief browser - pagebar interface with browser application
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#include <liberipc/eripc.h>
#include <liberipc/eripcpagebar.h>
#include "browserTypes.h"
#include "pagebar.h"
#include "indexFileHandler.h"
#include "browserLog.h"
#include "erbusy.h"

static erClientChannel_t erPagebarChannel;

// Init the IPC client
void pagebar_init()
{
    BR_PAGEBARPRINTF("entry");
    erIpcStartClient(ER_PAGEBAR_CHANNEL, &erPagebarChannel);
	pbReset(erPagebarChannel, ER_XHTML_VIEWER_UA_ID);
}

// Goto new page 
void pagebar_goto_page(int page)
{
    BR_PAGEBARPRINTF("entry %d", page);
	pbSetCurrentPage(erPagebarChannel, ER_XHTML_VIEWER_UA_ID, page);
}

// Set page count 
void pagebar_set_pagecount(int count)
{
    BR_PAGEBARPRINTF("entry %d", count);
	pbSetPageCount(erPagebarChannel, ER_XHTML_VIEWER_UA_ID, count);

}

// Set current page offset
void pagebar_set_offset(int offset)
{
	BR_PAGEBARPRINTF("entry %d", offset);
	pbSetCurrentPageOffset(erPagebarChannel, ER_XHTML_VIEWER_UA_ID, offset);
}

// Redraw pagebar
void pagebar_redraw()
{
    BR_PAGEBARPRINTF("redraw");
	pbRedraw(erPagebarChannel, ER_XHTML_VIEWER_UA_ID);
}

void browser_pagebar_page_selected(int pagenumber, Ereader *browser)
{
    gchar  *location = NULL;
    gchar  *pointer;

    BR_PAGEBARPRINTF("number %d -- Ereader pointer 0x%x ", pagenumber, (unsigned int) browser);

    if (pagenumber > 0)
    {
        location = index_file_get_page_location(&browser->pageStatus->index, browser->pageStatus->currentType, pagenumber);

        if (location)
        {
            gchar  *absolute_new_location;
            gchar  *end;
            int     length;

            BR_PAGEBARPRINTF("gtk_moz_embed_load_url %s", location);

            end = strrchr(browser->pageStatus->manifestFile, '/');

            if (end)
            {
                // size manifestfile path + last /
                length = (end - browser->pageStatus->manifestFile) + 1;

                BR_PAGEBARPRINTF("path lenght %d", length);

                absolute_new_location = (char *) g_malloc0(length + strlen(location));

                memcpy(absolute_new_location, browser->pageStatus->manifestFile, length);

                BR_PAGEBARPRINTF("location %s", location);
                BR_PAGEBARPRINTF("absolute_new_location %s", absolute_new_location);

                strcat(absolute_new_location, location);

                BR_PAGEBARPRINTF("gtk_moz_embed_load_url %s [%d]", absolute_new_location, strlen(absolute_new_location));

                BR_LOGPRINTF("BUSY ON");
                erbusy_blink();
                gtk_moz_embed_load_url(GTK_MOZ_EMBED(browser->mozEmbed), absolute_new_location);

                // free the allocated memory
                g_free(absolute_new_location);
            }
        }
        else
        {
            BR_PAGEBARPRINTF("location = null");
            // reload the current page
            //erbusy_request(1);
            //gtk_moz_embed_reload(GTK_MOZ_EMBED(browser->mozEmbed), GTK_MOZ_EMBED_FLAG_RELOADNORMAL);
        }
    }
    else
    {
        BR_PAGEBARPRINTF("invalid pagenumber %d", pagenumber);
    }

}

BrowserPageStatus *browser_create_page_status(void)
{
    BrowserPageStatus *pageStatus = NULL;

    // create one BrowserPageStatus ereader struct and fill it with zeros
    pageStatus = g_new0(BrowserPageStatus, 1);

    if (pageStatus)
    {
        pageStatus->currentType = g_strdup(INIT_PAGE_TYPE);
        pageStatus->currentPage = -1;
        pageStatus->pageCount = -1;
    }

    BR_PAGEBARPRINTF("0x%x", pageStatus);

    return pageStatus;
}


void browser_destroy_page_status(BrowserPageStatus * pageStatus)
{
    BR_PAGEBARPRINTF("entry  0x%x", pageStatus);

    if (pageStatus)
    {
        if (pageStatus->index.pagelist)
        {
            index_file_close(&pageStatus->index);
        }
        if (pageStatus->currentType)
        {
            // free string allocated with g_strdup
            g_free(pageStatus->currentType);
        }

        g_free(pageStatus);
    }
}

gboolean browser_set_page_status_indexfile(BrowserPageStatus * pageStatus, char *indexfile)
{
    gboolean returnValue = FALSE;

    BR_PAGEBARPRINTF("entry 0x%x file %s", pageStatus, indexfile);

    if (pageStatus && indexfile)
    {
        //init the index file used for page handling
        if (pageStatus->index.pagelist)
        {
            index_file_close(&pageStatus->index);
        }

        if (0 == index_file_init(indexfile, &pageStatus->index))
        {
            pageStatus->manifestFile = indexfile;
            returnValue = TRUE;
        }
    }
    else
    {
        BR_ERRORPRINTF("input parameter == NULL");
    }
    return returnValue;
}


gboolean page_status_handle_type_update(BrowserPageStatus * pageStatus, gchar * pageType)
{
    gint    result = -1;

    if (*pageType)
    {
        BR_PAGEBARPRINTF("currentType %s --> type %s", pageStatus->currentType, pageType);

        if (pageStatus->currentType)
        {
            result = strcmp(pageStatus->currentType, pageType);
        }

        if (result == 0)
        {
            BR_PAGEBARPRINTF("no type update");
            BR_PAGEBARPRINTF("no page bar update needed => new page number => returning FALSE!!!");
            return FALSE;

        }
        else
        {
            BR_PAGEBARPRINTF("type update from %s to %s", pageStatus->currentType, pageType);

            if (pageStatus->currentType)
            {
                g_free(pageStatus->currentType);
                pageStatus->currentType = NULL;
            }

            pageStatus->currentType = g_strdup(pageType);

            BR_PAGEBARPRINTF("page bar update needed (%s) => new page bar, new page number => returning TRUE !!!", pageStatus->currentType);
            return TRUE;
        }
    }

    return TRUE;
}
