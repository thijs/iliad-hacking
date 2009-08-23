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
 * \file diplayUpdate.cpp
 * \brief browser - browser application interface with the display manager
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#include <gdk/gdkx.h>
#include "liberdm/erdm.h"
#include "browserTypes.h"
#include "timingInfo.h"
#include "displayUpdate.h"
#include "browserLog.h"
#include "erbusy.h"


static int g_currentLevel = LOWEST_LEVEL;

// internally used routines
int     display_update_get_level(void);
int     display_update_reset_level(void);

/**
 * callback : called when GTK in idle mode
 *
 * @param data  - indicates where the update request came from (DEBUG reasons)
 *
 * @returns 0 (to remove the "idle mode" handler)
 */
gboolean display_update_during_idle_handling(gpointer data)
{
    int        q = (int) data;
    eDmQuality quality = dmQFull;
        
    if (q == DM_QUALITY_TYPING)
    {
        quality = dmQTyping;
    }

    BR_DISPLAYPRINTF("request screen update (%d)", quality);

#ifdef TIMING_INFORMATION_ENABLED
    timing_display_time_elapsed();
#endif //TIMING_INFORMATION_ENABLED

    // full screen update,
    gdk_threads_enter();
    gdk_flush();
    gdk_flush();
    gdk_threads_leave();

    //not busy any more
    erbusy_off();

#ifdef TIMING_INFORMATION_ENABLED
    timing_display_time_elapsed();
#endif //TIMING_INFORMATION_ENABLED

    // full screen update,
    dmDisplay(dmCmdPriorUrgent, quality);

    // return FALSE to remove handler
    return FALSE;
}

gboolean display_update_request_screen_refresh(int level, gpointer data)
{
    int     theCurrentLevel = display_update_get_level();
    int     quality = (int) data;
    
    if (quality == 0)
    {
        quality = DM_QUALITY_FULL;
    }

    BR_DISPLAYPRINTF("entry level %d currentlevel %d", level, theCurrentLevel);

    if (level >= theCurrentLevel)
    {
        
        gtk_idle_add(display_update_during_idle_handling, (gpointer) quality);

        //reset the current level
        display_update_reset_level();

    }
    return TRUE;
}

int display_update_get_level(void)
{
    return g_currentLevel;
}

int display_update_reset_level(void)
{
    g_currentLevel = LOWEST_LEVEL;
}

void display_update_increase_level(int level)
{
    if (level > g_currentLevel)
    {
        g_currentLevel = level;
    }
}
