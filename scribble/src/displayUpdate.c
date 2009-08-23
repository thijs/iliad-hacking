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
 * \file diplayUpdate.c
 * \brief for scribble
 *
 * <File description>
 *
 */


#include <gdk/gdk.h>
#include "displayUpdate.h"
#include "erbusy.h"
#include "ScribbleLog.h"

//for usleep(usecond)
//#include <unistd.h>

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

static gboolean display_update_during_idle_handling(gpointer data)
{
    eDmQuality quality = (int) data;

    SB_TIMEDISPLAY("start...,quality=%d",quality);
    erbusy_blink();
    //usleep( 300 * 1000 );
    // full screen update,
    gdk_threads_enter();
    //gdk_flush();
    gdk_flush();
    gdk_threads_leave();

    SB_TIMEDISPLAY("dmDisplay...");
    // full screen update,
    //dmDisplay(dmCmdPriorUrgent, quality);//no wait
    dmDisplay(dmCmdPriorNormal,quality);//work,500ms wait.
    //dmDisplay(dmCmdPriorHigh,quality);doens't work
    erbusy_off();

    SB_TIMEDISPLAY("done.");
    // return FALSE to remove handler
    return FALSE;
}

//data -- quality, four types now
gboolean display_update_request_screen_refresh(int level, gpointer data)
{
    int     theCurrentLevel = display_update_get_level();
    int     quality = (int) data;

    SB_IPCPRINTF("quality:%d\n",quality);
    if (quality == 0)
    {
        quality = dmQFull;
    }


    if (level >= theCurrentLevel)
    {
        g_idle_add_full(G_PRIORITY_HIGH_IDLE,	
			display_update_during_idle_handling,
			(gpointer) quality,
			NULL);
        //gtk_idle_add(display_update_during_idle_handling, (gpointer) quality);
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
    return g_currentLevel;
}

void display_update_increase_level(int level)
{
    if (level > g_currentLevel)
    {
        g_currentLevel = level;
    }
}
