/*
 * This file is part of powerMgr.
 *
 * powerMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * powerMgr is distributed in the hope that it will be useful,
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
 * \file toolbar.c
 * \brief powerMgr - eReader Power Management daemon
 *
 */

#include <glib.h>
#include <liberipc/eripc.h>
#include <liberipc/eripctoolbar.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>
#include "toolbar.h"

static erClientChannel_t toolbarChannel;


void toolbar_init()
{
    int returnValue;

    // create/init the channel to communicate with the toolbar service
    returnValue = erIpcStartClient(ER_TOOLBAR_CHANNEL, &toolbarChannel);

    if (returnValue != 0)
    {
        fprintf(stderr, "powerMgr toolbar_init(): erIpcStartClient returned %d\n", returnValue);
        return;
    }
}

void toolbar_setIconState(int iconID, int iconState)
{
    fprintf(stderr, "powerMgr toolbar_setIconState() entry: toolbarChannel [%p] iconID [%d] iconState [%d]\n", toolbarChannel, iconID, iconState);
    
    tbSetStatePlatformIcon(toolbarChannel, ER_POWERMGR_UA_ID, iconID, iconState);
}
