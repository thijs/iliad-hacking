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
 * \file mozEmbedCallbacks.cpp
 * \brief browser - eReader "GtkMozEmbed" related callbacks
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "nsMemory.h"
#include "browserTypes.h"
#include "mozEmbedCallbacks.h"
#include "indexFileHandler.h"
#include "mozilla_api.h"
#include "selection.h"
#include "timingInfo.h"
#include "browser.h"
#include "pagebar.h"
#include "displayUpdate.h"
#include "browserLog.h"
#include "erbusy.h"

#ifdef ENABLE_LOGGING
#include <liberlog/erlog.h>
#endif

gint open_uri_cb (GtkMozEmbed *embed, const char *uri, gpointer data)
{
    struct stat statbuf;
    char *path;

    BR_PAGEBARPRINTF("Going to load: %s\n", uri);
    
    path = mozilla_uri_to_path(uri);

    // Check if this location exists, otherwise do not follow URI (return TRUE)
    if (path != NULL && stat(path, &statbuf) != 0)
    {
        BR_ERRORPRINTF("Not jumping to non-existing page: %s [%s]\n", uri, path);
        erbusy_off();
        free(path);
        return TRUE; // Cancel loading of the page
    }
    else if (path != NULL)
    {
        free(path);
    }

#ifdef ENABLE_LOGGING
    // MvdW: log this page!
    char * load = (char*)malloc(strlen(uri)+1);
    strcpy(load, uri);
    erlog_event_open(load);
    free(load);
#endif
    
    // Set busy
    erbusy_blink();
    
    return FALSE; // this will load the URI, if we return TRUE the load will be cancelled
}

void status_change_cb(GtkMozEmbed * embed, gpointer request, gint status, gpointer message)
{
    BR_PAGEBARPRINTF("entry status %d", status);
}

void progress_change_cb(GtkMozEmbed * embed, gint cur, gint max, Ereader * browser)
{
    BR_PAGEBARPRINTF("entry cur %d max %d", cur, max);
}


void location_changed_cb(GtkMozEmbed * embed, gpointer data)
{
    BR_PAGEBARPRINTF("________________BROWSER - location_changed_cb _____________");

    // => only request a refresh load finished or a higher level
    display_update_increase_level(MOZEMBED_UPDATE_LEVEL);

#ifdef TIMING_INFORMATION_ENABLED
    timing_display_time_elapsed();
#endif //TIMING_INFORMATION_ENABLED
}

void visibility_cb(GtkMozEmbed * embed, gboolean visibility, Ereader * browser)
{
    BR_PAGEBARPRINTF("visibility_cb entry");
    set_browser_visibility(browser, visibility);
}

void load_finished_cb(GtkMozEmbed * embed, Ereader * browser)
{
    gchar  *type = NULL;
    gboolean pagebar_update_needed = FALSE;
    gboolean page_type_known = FALSE;
    int     pageCount;
    int     page;
	int     offset;
	gchar  *url = NULL;

    BR_PAGEBARPRINTF("________________BROWSER - load_finished _____________");
#ifdef TIMING_INFORMATION_ENABLED
    timing_display_time_elapsed();
#endif //TIMING_INFORMATION_ENABLED

    BR_PAGEBARPRINTF("0x%x -- mozembed 0x%x -- pagestatus 0x%x", browser, embed, browser->pageStatus);

    // Comes from browser examples => still have to look into this  
    // nsMemory::HeapMinimize(PR_TRUE);

    // retrieve the page type information from the XHTML meta data
    page_type_known = mozilla_get_page_type(GTK_MOZ_EMBED(browser->mozEmbed), &type);

    // given the type, determine the pagecount and the current page 
    if (!page_type_known)
    {
        BR_PAGEBARPRINTF("---- mozilla_get_page_type returned unknown page type ----");
        type = g_strdup(INVALID_PAGE_TYPE);
        pageCount = DEFAULT_PAGE_COUNT;
        page = DEFAULT_PAGE;
	}
    else
    {
		if ( (pageCount = index_file_get_page_count(&browser->pageStatus->index, type)) == INVALID_PAGE_COUNT )
		{
			BR_WARNPRINTF("index_file_get_page_count() returned INVALID_PAGE_COUNT");
			pageCount = DEFAULT_PAGE_COUNT;
			page = DEFAULT_PAGE;
		}
		else
		{
        	BR_PAGEBARPRINTF("---- mozilla_get_page_type returned %s (count %d) ----", type, pageCount);
			
			url = gtk_moz_embed_get_location(GTK_MOZ_EMBED(browser->mozEmbed));
			BR_PAGEBARPRINTF("load_finished_cb -- location %s", url);

			page = index_file_get_page_number(&browser->pageStatus->index, type, url);
			BR_PAGEBARPRINTF("index_file_get_page_number returned %d", page);
		}

    }

	browser->pageStatus->pageCount = pageCount;
	browser->pageStatus->currentPage = page;
	
	offset = mozilla_determine_page_offset(GTK_MOZ_EMBED(browser->mozEmbed));
	BR_PAGEBARPRINTF("mozilla_determine_page_offset returned %d", offset);

	// This pagebar_update_needed stuff is some legacy code. It saves 1 IPC call, but introduces other overhead
	// TODO: evaluate this code... [MvdW]
	BR_PAGEBARPRINTF("---- mozilla_get_page_type -- page_status_handle_type_update %s [0x%x]----", type, browser->pageStatus);
    pagebar_update_needed = page_status_handle_type_update(browser->pageStatus, type);

    if (!pagebar_update_needed)
    {
        BR_PAGEBARPRINTF("---- no pagebar_update_needed => select page %d ----", page);
        pagebar_goto_page(page);
		pagebar_set_offset(offset);
        pagebar_redraw();
    }
    else
    {
        BR_PAGEBARPRINTF("---- pagebar_update_needed => redraw pagebar %d select page %d ----", pageCount, page);
        pagebar_set_pagecount(pageCount);
        pagebar_goto_page(page);
		pagebar_set_offset(offset);
        pagebar_redraw();
    }

    // free used memory
    if (type)
    {
        g_free(type);
        type = NULL;
    }

    if (url)
    {
        g_free(url);
        url = NULL;
    }

#ifdef SELECTION_ENABLED
    // when needed, add the selections to this page
    selection_set_current_page_selections(GTK_MOZ_EMBED(embed));
#endif // SELECTION_ENABLED

    // handle screen refresh
    BR_DISPLAYPRINTF("________________BROWSER - set idle callback _____________");

#ifdef TIMING_INFORMATION_ENABLED
    timing_display_time_elapsed();
#endif //TIMING_INFORMATION_ENABLED

    display_update_request_screen_refresh(MOZEMBED_UPDATE_LEVEL, NULL);

}


