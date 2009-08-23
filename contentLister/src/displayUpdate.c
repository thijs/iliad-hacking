/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

 /**
 * \file displayUpdate.c
 * \brief control - application interface with the display manager                  
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
#include <config.h>

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#include <liberdm/erdm.h>

#include "contentListerLog.h"
#include "displayUpdate.h"
#include "erbusy.h"


static int  g_currentLevel = LOWEST_LEVEL;

// internally used routines


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

    CL_SCREENPRINTF("__CONTENTLISTER - request screen update (%d)__", level);

    CL_TIMEDISPLAY("Before gdk_flush()");

    // full screen update,
    gdk_threads_enter();
    gdk_flush();
    gdk_threads_leave();

    erbusy_off();
    
    if (level <= LIST_ITEM_FOCUS_LEVEL)
    {
        CL_TIMEDISPLAY("Before dmDisplay (dmQBW)");
        dmDisplay(dmCmdPriorUrgent, dmQBW);
    }
    else if (level <= TEXTENTRY_CHANGED_LEVEL)
    {
        CL_TIMEDISPLAY("Before dmDisplay (dmQTyping)");
        dmDisplay(dmCmdPriorUrgent, dmQTyping);
    }
    else if (level <= MAIN_WINDOW_EXPOSE_LEVEL)
    {
        CL_TIMEDISPLAY("Before dmDisplay (dmQFull)");
        dmDisplay(dmCmdPriorHigh, dmQFull);
    }
    else
    {
        CL_TIMEDISPLAY("Before dmDisplay (dmQFull, low-priority)");
        dmDisplay(dmCmdPriorLow, dmQFull);
    }

    //reset the current level
    g_currentLevel = LOWEST_LEVEL;

    // return FALSE to remove handler
    return FALSE;
}

#undef display_update_request_screen_refresh
gboolean display_update_request_screen_refresh(int level)
{
    CL_SCREENPRINTF("entry level %d currentlevel %d", level, g_currentLevel);

    if (level >= g_currentLevel)
    {
        CL_TIMEDISPLAY("Before adding idle callback");

        gtk_idle_add(display_update_during_idle_handling, (gpointer) level);

        g_currentLevel = level + 1;
    }
    CL_SCREENPRINTF("leave level %d currentlevel %d", level, g_currentLevel);
    return TRUE;
}

int display_update_get_level(void)
{
    return g_currentLevel;
}


#undef display_update_increase_level
void display_update_increase_level(int level)
{
    CL_SCREENPRINTF("new level %d - currentlevel %d", level, g_currentLevel);

    if (level > g_currentLevel)
    {
        g_currentLevel = level;
    }
}


#undef display_update_decrease_level
void display_update_decrease_level(int level)
{
    CL_SCREENPRINTF("new level %d - currentlevel %d", level, g_currentLevel);

    if (level < g_currentLevel)
    {
        g_currentLevel = level;
    }
}

