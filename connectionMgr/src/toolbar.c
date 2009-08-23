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
 * \file toolbar.c
 * \brief connectionMgr - toolbar                
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <glib.h>

#include <liberipc/eripc.h>
#include <liberipc/eripctoolbar.h>
#include <liberipc/eripcconnect.h>

#include "connectionMgrLog.h"
#include "toolbar.h"

static erClientChannel_t toolbarChannel;

static void toolbar_init_icons(void);

void toolbar_init()
{
    int returnValue;

    CN_LOGPRINTF("entry");
    
    // create/init the channel to communicate with the toolbar service
    returnValue = erIpcStartClient(ER_TOOLBAR_CHANNEL, &toolbarChannel);

    if (returnValue != 0)
    {
        CN_WARNPRINTF("erIpcStartClient returned %d", returnValue);
        return;
    }

    toolbar_init_icons();
}

static void toolbar_init_icons(void)
{
    // freeze toolbar layout
    tbDisableUpdate(toolbarChannel, ER_CONNECT_UA_ID);

    // tell toolbar which iconset to display
    tbSelectIconSet(toolbarChannel, ER_CONNECT_UA_ID);

    // clear the toolbar 
    tbClearIconSet(toolbarChannel, ER_CONNECT_UA_ID);

    tbAppendPlatformIcon(toolbarChannel, ER_CONNECT_UA_ID, 
                         iconID_back, ccCnToolbar);
    tbSetStatePlatformIcon(toolbarChannel, ER_CONNECT_UA_ID, 
                           iconID_back, iconState_grey);

    // set the system icons
    tbAppendPlatformIcon(toolbarChannel, ER_CONNECT_UA_ID, 
                         iconID_trashcan, ccCnToolbar);
    toolbar_setIconState(iconID_trashcan, iconState_grey);

    tbAppendPlatformIcon(toolbarChannel, ER_CONNECT_UA_ID, 
                         iconID_keyboard, ccCnToolbar);
    toolbar_setIconState(iconID_keyboard, iconState_grey);

    // redraw toolbar now
    tbEnableUpdate(toolbarChannel, ER_CONNECT_UA_ID);
}

// freeze toolbar layout
void toolbar_disableUpdate()
{
    CN_LOGPRINTF("entry");

    tbDisableUpdate(toolbarChannel, ER_CONTENT_LISTER_UA_ID);
}

// redraw toolbar now
void toolbar_enableUpdate()
{
    CN_LOGPRINTF("entry");

    tbEnableUpdate(toolbarChannel, ER_CONTENT_LISTER_UA_ID);
}


void toolbar_setIconState(int iconID, int iconState)
{
    CN_LOGPRINTF("entry: iconID [%d] iconState [%d]", iconID, iconState);
    
    tbSetStatePlatformIcon(toolbarChannel, ER_CONNECT_UA_ID, 
                           iconID, iconState);
}

void toolbar_synchronise(void)
{
    CN_LOGPRINTF("entry");

    tbSynchronise(toolbarChannel, ER_CONNECT_UA_ID, ccCnToolbarSync);
}

