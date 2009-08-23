/*
 * This file is part of settings.
 *
 * settings is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * settings is distributed in the hope that it will be useful,
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
 * \brief setup application - E-reader settings application screen creation and handling                
 *
 */

#include <glib.h>

#include <liberipc/eripc.h>
#include <liberipc/eripctoolbar.h>

#include "setupLog.h"
#include "toolbar.h"


static erClientChannel_t toolbarChannel;

void toolbar_init()
{
    int returnValue;

    ST_LOGPRINTF("entry");
    
    // create/init the channel to communicate with the toolbar service
    returnValue = erIpcStartClient(ER_TOOLBAR_CHANNEL, &toolbarChannel);

    if (returnValue != 0)
    {
        ST_WARNPRINTF("erIpcStartClient returned %d", returnValue);
        return;
    }
}


void toolbar_setIconState(int iconID, int iconState)
{
    ST_LOGPRINTF("entry: iconID [%d] iconState [%d]", iconID, iconState);
    
    tbSetStatePlatformIcon(toolbarChannel, ER_SETUP_UA_ID, iconID, iconState);
}

