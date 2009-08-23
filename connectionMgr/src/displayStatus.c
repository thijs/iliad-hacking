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
 * \file displayStatus.h
 * \brief connectionMgr - application interface with the display manager
 *
 * <File description>
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <stdio.h>
#include <time.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <glib.h>

#include <liberdm/erdm.h>

#include "connectionMgrLog.h"
#include "displayStatus.h"
#include "erbusy.h"


static int g_currentLevel = LOWEST_LEVEL;

static guint g_idle_hander_id = 0; 

#if CN_DISPLAY_ON
static int counter = 0;
#endif


void display_update_remove_idle_handler_id(void)
{
    if (g_idle_hander_id)
    {
        CN_DMPRINTF("\nRemoving the old idle handler.\n");
        g_source_remove(g_idle_hander_id);
        g_idle_hander_id = 0;
    }
}

/**
 * callback : called when GTK in idle mode
 *
 * @param data - indicates where the update request came from 
 *               (DEBUG reasons)
 *
 * @returns 0 (to remove the "idle mode" handler)
 */
gboolean display_update_during_idle_handling(gpointer data)
{
    int     level = (int) data;
    int     nQuality;
    eDmQuality quality;

    CN_DMPRINTF(" **** (%d) ***** [%d]", level, counter++);

    // full screen update,
    gdk_threads_enter();
    gdk_flush();
    gdk_threads_leave();

    erbusy_off();
    
    // Extract 
    //   level from data
    //   waveform from data
    level = (int) data & 0x0000FFFF;
    nQuality = (int) data >> 16;
    switch (nQuality)
    {
        case WAVEFORM_BW:
            CN_DMPRINTF("Request for BW waveform");
            quality = dmQBW;
            break;
        case WAVEFORM_TYPING:
            CN_DMPRINTF("Request for Typing waveform");
            quality = dmQTyping;
            break;
        default:
            quality = dmQFull;
    }

    if ((level == MAIN_WINDOW_EXPOSE_LEVEL)      ||
        (level == SETUP_SCREEN_EXPOSE_LEVEL)     ||
        (level == ONE_BUTTON_ITEM_CLICKED_LEVEL) ||
        (level == SETUP_PAGE_LAYOUT_CHANGE)      ||
        (level == SETTING_ITEM_CHANGE)           ||
        (level == STATUS_ITEM_CHANGE)              )
    {
        // full screen update => should be done immediately
        dmDisplay(dmCmdPriorUrgent, quality);
    }
    else if ((level == BUTTON_FOCUS_IN_LEVEL) ||
             (level == SELECTION_FOCUS_IN_LEVEL) ||
             (level == TEXT_ENTRY_FOCUS_IN_LEVEL)  )
    {
        // with Hard key selecting items on the screen 
        // dmDisplay(dmCmdPriorNormal, quality);
        dmDisplay(dmCmdPriorUrgent, quality);

    }
    else if (level == TEXT_ENTRY_CHANGED_LEVEL)
    {
        //text entry
        dmDisplay(dmCmdPriorNormal, quality);
        //dmDisplay(dmCmdPriorUrgent, quality);
    }
    else if (level == SELECTION_CLICKED_LEVEL)
    {
        // switch of a selection item 
        // dmDisplay(dmCmdPriorHigh, quality);
        dmDisplay(dmCmdPriorUrgent, quality);
    }
    else
    {
        //dmDisplay(dmCmdPriorNormal, quality);
        dmDisplay(dmCmdPriorUrgent, quality);
    }

    //reset the current level
    g_currentLevel = LOWEST_LEVEL;

    // return FALSE to remove handler
    g_idle_hander_id = 0;
    return FALSE;
}

#undef display_update_request_screen_refresh
gboolean display_update_request_screen_refresh(int level, int waveform)
{
    CN_DMPRINTF("[%d] entry level %d currentlevel %d", 
            counter++, level, g_currentLevel);

    if (level >= g_currentLevel)
    {
        g_currentLevel = level + 1;

        if (waveform != 0)
        {
            CN_DMPRINTF("Special waveform request");
            level += waveform << 16;
        }

        display_update_remove_idle_handler_id();

        g_idle_hander_id = gtk_idle_add(display_update_during_idle_handling,
                                        (gpointer) level);
    }
    return TRUE;
}

int display_update_get_level(void)
{
    return g_currentLevel;
}

#undef display_update_increase_level
void display_update_increase_level(int level)
{
    CN_DMPRINTF("increase from %d to %d", g_currentLevel, level);

    if (level > g_currentLevel)
    {
        g_currentLevel = level;
    }
}

#undef display_update_decrease_level
void display_update_decrease_level(int level)
{
    CN_DMPRINTF("decrease from %d to %d", g_currentLevel, level);

    if (level < g_currentLevel)
    {
        g_currentLevel = level;
    }
}

