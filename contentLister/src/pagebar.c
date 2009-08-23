/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file pagebar.c
 * \brief contentLister - pageBar deamon interaction with contentLister application
 *
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#include <liberipc/eripc.h>
#include <liberipc/eripcpagebar.h>

#include "contentListerLog.h"


static erClientChannel_t erPagebarChannel;

// Init the IPC client
void pagebar_init()
{
    CL_IPCPRINTF("entry");
    erIpcStartClient(ER_PAGEBAR_CHANNEL, &erPagebarChannel);
}

// Reset the pageBar for use with contentLister
void pagebar_reset()
{
    CL_IPCPRINTF("entry");
    pbReset(erPagebarChannel, ER_CONTENT_LISTER_UA_ID);
}

// Goto new page 
void pagebar_goto_page(int page)
{
    CL_IPCPRINTF("entry %d", page);
    pbSetCurrentPage(erPagebarChannel, ER_CONTENT_LISTER_UA_ID, page);
}

// Set page count 
void pagebar_set_pagecount(int count)
{
    CL_IPCPRINTF("entry %d", count);
    pbSetPageCount(erPagebarChannel, ER_CONTENT_LISTER_UA_ID, count);

}

// Redraw pagebar
void pagebar_redraw()
{
    CL_IPCPRINTF("redraw");
    pbRedraw(erPagebarChannel, ER_CONTENT_LISTER_UA_ID);
}

// That's all for now...
