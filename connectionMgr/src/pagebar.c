/*
 * This file is part of connectionMgr.
 *
 * connectionMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * connectionMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file pagebar.c
 * \brief connectionMgr - pageBar deamon interaction with setup application
 *
 * <File description>
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <liberipc/eripc.h>
#include <liberipc/eripcpagebar.h>

#include "connectionMgrLog.h"


static erClientChannel_t erPagebarChannel;

// Init the IPC client
void pagebar_init()
{
    CN_IPCPRINTF("entry");
    erIpcStartClient(ER_PAGEBAR_CHANNEL, &erPagebarChannel);
}

// Reset the pageBar for use with setup
void pagebar_reset()
{
    CN_IPCPRINTF("entry");
    pbReset(erPagebarChannel, ER_CONNECT_UA_ID);
}

// Goto new page 
void pagebar_goto_page(int page)
{
    CN_IPCPRINTF("entry: page = %d", page);
    pbSetCurrentPage(erPagebarChannel, ER_CONNECT_UA_ID, page);
}

// Set page count 
void pagebar_set_pagecount(int count)
{
    CN_IPCPRINTF("entry: count = %d", count);
    pbSetPageCount(erPagebarChannel, ER_CONNECT_UA_ID, count);

}

// Redraw pagebar
void pagebar_redraw()
{
    CN_IPCPRINTF("redraw");
    pbRedraw(erPagebarChannel, ER_CONNECT_UA_ID);
}

