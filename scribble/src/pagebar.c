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

/**
 * \file pagebar.c
 * \brief scribble - pagebar interface with scribble
 * \author Jian Shen
 *
 * <File description>
 *
 */

#include <liberipc/eripc.h>
#include <liberipc/eripcpagebar.h>

#include "ScribbleIPC.h"
#include "ScribbleLog.h"

static erClientChannel_t erPagebarChannel;

// Init the IPC client
void pagebar_init()
{
    int iRet=-1;
    SB_PAGEBARPRINTF("entry\n");
    iRet=erIpcStartClient(ER_PAGEBAR_CHANNEL, &erPagebarChannel);
    if (iRet != 0)
    {
        SB_ERRORPRINTF("erIpcStartClient returned %d\n", iRet);
        return;
    }
    pbReset(erPagebarChannel, ER_SCRIBBLE_UA_ID);
}

// Goto new page 
void pagebar_goto_page(int page)
{
    SB_PAGEBARPRINTF("entry %d\n", page);
    pbSetCurrentPage(erPagebarChannel, ER_SCRIBBLE_UA_ID, page);
}

// Set page count 
void pagebar_set_pagecount(int count)
{
    SB_PAGEBARPRINTF("entry %d\n", count);
    pbSetPageCount(erPagebarChannel, ER_SCRIBBLE_UA_ID, count);
}

// Redraw pagebar
void pagebar_redraw()
{
    SB_PAGEBARPRINTF("redraw\n");
    pbRedraw(erPagebarChannel, ER_SCRIBBLE_UA_ID);
}
