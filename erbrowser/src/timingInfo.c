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

//
// C++ Implementation: timingInfo
//
// Description: 
//
//
// Author: ann <ann@localhost>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <stdio.h>
#include <glib.h>
#include "browserLog.h"
#include "timingInfo.h"

// global data
static GTimer *theTimer = NULL;

int timing_init(void)
{
#ifdef TIMING_INFORMATION_ENABLED
    BR_LOGPRINTF("create timer");
    if (theTimer)
    {
        BR_ERRORPRINTF("create timer already called");
    }
    else
    {
        theTimer = g_timer_new();
    }
#endif //TIMING_INFORMATION_ENABLED
    return 0;
}

int timing_destroy(void)
{
#ifdef TIMING_INFORMATION_ENABLED
    BR_LOGPRINTF("destroy timer");

    if (theTimer)
    {
        g_timer_destroy(theTimer);
    }
#endif //TIMING_INFORMATION_ENABLED
    return 0;
}

int timing_display_time_elapsed(void)
{
#ifdef TIMING_INFORMATION_ENABLED
    gulong  microseconds = 0;
    gdouble seconds = 0;

    if (theTimer)
    {
        seconds = g_timer_elapsed(theTimer, &microseconds);
        BR_LOGPRINTF("######### time elapsed %f sec - %u usec ###########", seconds, microseconds);
        BR_LOGPRINTF("######### time elapsed %f sec ###########", seconds);

        //restart the time 
        g_timer_start(theTimer);
    }

#endif //TIMING_INFORMATION_ENABLED
    return 0;

}
