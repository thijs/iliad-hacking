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
 * \file displayStatus.h
 * \brief control - application interface with the display manager                  
 * 
 * <File description>
 *
 */

#include <stdio.h>
#include <time.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <glib.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "liberdm/erdm.h"
#include "erbusy.h"

static int g_currentLevel = LOWEST_LEVEL;

#if ST_LOGGING_ON
  static int counter = 0;
#endif


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
    XImage *xi;

    // full screen update,
    gdk_threads_enter();
    xi = XGetImage(GDK_DISPLAY(), GDK_ROOT_WINDOW(), 0, 0, 1, 1, AllPlanes, XYPixmap);
    gdk_flush();
    XDestroyImage(xi);
    gdk_threads_leave();

    erbusy_off();

    // Extract 
    //   level from data
    //   waveform from data

    level = (int) data & 0x0000FFFF;
    nQuality = (int) data >> 16;
    
#if (ST_SCREEN_ON)
    static int counter = 0;
    ST_WARNPRINTF(" **** (%d.%d) ***** [%d]", level, nQuality, counter++);
#endif

    switch (nQuality)
    {
        case WAVEFORM_BW:
            ST_LOGPRINTF("Request for BW waveform");
            quality = dmQBW;
            break;
        case WAVEFORM_TYPING:
            ST_LOGPRINTF("Request for Typing waveform");
            quality = dmQTyping;
            break;
        default:
            quality = dmQFull;
    }

    if ((level == MAIN_WINDOW_EXPOSE_LEVEL) ||
        (level == SETUP_SCREEN_EXPOSE_LEVEL) ||
        (level == ONE_BUTTON_ITEM_CLICKED_LEVEL) || (level == SETUP_PAGE_LAYOUT_CHANGE) || (level == SETTING_ITEM_CHANGE))
    {
        // full screen update => should be done immediately
        dmDisplay(dmCmdPriorUrgent, quality);
    }
    else if ((level == BUTTON_FOCUS_IN_LEVEL) || (level == SELECTION_FOCUS_IN_LEVEL) || (level == TEXT_ENTRY_FOCUS_IN_LEVEL))
    {
        // with Hard key selecting items on the screen 
        dmDisplay(dmCmdPriorNormal, quality);

    }
    else if (level == TEXT_ENTRY_CHANGED_LEVEL)
    {
        //text entry
        dmDisplay(dmCmdPriorUrgent, quality);
    }
    else if (level == SELECTION_CLICKED_LEVEL)
    {
        // switch of a selection item 
        dmDisplay(dmCmdPriorUrgent, quality);
    }
    else
    {
        dmDisplay(dmCmdPriorUrgent, quality);
    }

    //reset the current level
    g_currentLevel = LOWEST_LEVEL;

    // return FALSE to remove handler
    return FALSE;
}

#undef display_update_request_screen_refresh
gboolean display_update_request_screen_refresh(int level, int waveform)
{
    ST_LOGPRINTF("[%d] entry level %d currentlevel %d", counter++, level, g_currentLevel);

    if (level >= g_currentLevel)
    {
        if (waveform != 0)
        {
            ST_LOGPRINTF("Special waveform request");
            level += waveform << 16;
        }

        gtk_idle_add(display_update_during_idle_handling, (gpointer) level);

        g_currentLevel = level + 1;
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
    ST_LOGPRINTF("increase from %d to %d", g_currentLevel, level);

    if (level > g_currentLevel)
    {
        g_currentLevel = level;
    }
}

#undef display_update_decrease_level
void display_update_decrease_level(int level)
{
    ST_LOGPRINTF("new level %d - currentlevel %d", level, g_currentLevel);

    if (level < g_currentLevel)
    {
        g_currentLevel = level;
    }
}
