/*
 * This file is part of pcshareMgr.
 *
 * pcshareMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pcshareMgr is distributed in the hope that it will be useful,
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
 * \file displayUpdate.c
 * \brief control - application interface with the display manager                  
 * 
 * <File description>
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
#include <config.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "liberdm/erdm.h"
#include "logging.h"
#include "displayUpdate.h"

static int g_currentLevel = LOWEST_LEVEL;

// internally used routines
int     display_update_get_level(void);
void    display_update_reset_level(void);

/**
 * callback : called when GTK in idle mode
 *
 * @param data  - indicates where the update request came from (DEBUG reasons)
 *
 * @returns 0 (to remove the "idle mode" handler)
 */
gboolean display_update_during_idle_handling(gpointer data)
{
    int     level = (int) data;
    int     nQuality;
    eDmQuality quality;

    //XImage *xi;

    // Extract 
    //   level from data
    //   waveform from data

    level = (int) data & 0x0000FFFF;
    nQuality = (int) data >> 16;
    switch (nQuality)
    {
        case WAVEFORM_BW:
            DL_DMPRINTF("Request for BW waveform");
            quality = dmQBW;
            break;
        case WAVEFORM_TYPING:
            DL_DMPRINTF("Request for Typing waveform");
            quality = dmQTyping;
            break;
        default:
            quality = dmQFull;
    }

    DL_DMPRINTF("\n________________request screen update (%d - %d)_____________", level, quality);

    // full screen update,

    gdk_threads_enter();
    gdk_flush();
    gdk_threads_leave();

    // full screen update,
    dmDisplay(dmCmdPriorNormal, quality);

    // return FALSE to remove handler
    return FALSE;
}

gboolean display_update_request_screen_refresh(int level, gpointer data)
{
    int     theCurrentLevel = display_update_get_level();
    int     waveform;

    waveform = (int) data;

    DL_DMPRINTF("entry level %d currentlevel %d", level, theCurrentLevel);

    if (level >= theCurrentLevel)
    {
        if (waveform != 0)
        {
            DL_DMPRINTF("Special waveform request");
            level += waveform << 16;
        }


        gtk_idle_add(display_update_during_idle_handling, (gpointer) level);

        //reset the current level
        display_update_reset_level();

    }
    return TRUE;
}

int display_update_get_level(void)
{
    return g_currentLevel;
}

void display_update_reset_level(void)
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
