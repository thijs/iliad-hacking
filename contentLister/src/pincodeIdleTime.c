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
 * \file pincodeIdleTime.c
 * \brief content lister - accumulate time of being idle
 *
 * accumulate time of being idle, when being idle for a long time, reach the
 * threshold, call callback function of "accumulateIdleTime_t" to do 
 * something special, then end the accumulate idle time
 *
 * if no threshold(default), just accumulating idle time, don't call callback function
 *
 * in some case, e.g connectMgr and downloadMgr, if user sets threshold to be 5 mins, 
 * but downloading process needs more time which can not be forecasted,
 * in this case, we need no threshold temporily
 *
 * in pincode's case, "accumulatedIdleTime_t" is based on window application. 
 * when create widget, begin to accumulate idle time at the same time.
 * when received key or button event, reset 'accumulate idle time' to zero
 * 
 * Copyright (C) 2006-2008 iRex Technologies BV.
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <glib.h>

#include "contentListerLog.h"
#include "pincodeIdleTime.h"


static accumulateIdleTime_t *g_accumulate_idle_time = NULL;

// internally functions forward declarations
//static gboolean accumulate_idle_time_increase(gpointer internal);
static void accumulate_idle_time_set(gint value);

void accumulate_idle_time_begin()
{
    CL_LOGPRINTF("ACCUMULATE_IDLE_TIME_BEGIN");
    accumulateIdleTime_t *idle_time = NULL;

    if (NULL != g_accumulate_idle_time)
    {
        accumulate_idle_time_end();
    }

    idle_time = g_new0(accumulateIdleTime_t, 1);
    g_return_if_fail(NULL != idle_time);

// set the default value
    idle_time->milliseconds = 0;
    idle_time->threshold = 0;   // default no threshold
    idle_time->callback_on_threshold = NULL;
    idle_time->internal = TIMEOUT_INTERNAL;
//    idle_time->timeout_handler_id = g_timeout_add( idle_time->internal,
//                                                   accumulate_idle_time_increase,
//                                                   (gpointer) idle_time->internal );

    g_accumulate_idle_time = idle_time;
}

void accumulate_idle_time_end()
{
    CL_LOGPRINTF("ACCUMULATE_IDLE_TIME_END");

    if (g_accumulate_idle_time)
    {
        if (g_accumulate_idle_time->timeout_handler_id)
        {
            g_source_remove(g_accumulate_idle_time->timeout_handler_id);
        }
        g_free(g_accumulate_idle_time);
        g_accumulate_idle_time = NULL;
    }
}

// because in future user can decide how many milliseconds being idle for
// let device go to screen lock or power down
void accumulate_idle_time_set_threshold(gint threshold)
{
    accumulateIdleTime_t *idle_time = g_accumulate_idle_time;

    if (idle_time)
    {
        idle_time->threshold = threshold;
    }
}

// in pincode's case, callback functions can be "go to screen lock" or "power down"
void accumulate_idle_time_set_callback(on_threshold_t * func)
{
    accumulateIdleTime_t *idle_time = g_accumulate_idle_time;

    if (idle_time)
    {
        idle_time->callback_on_threshold = func;
    }
}

// normally, call it to zero idle time when there are user activities happened
void accumulate_idle_time_reset()
{
    CL_LOGPRINTF("ACCUMULATE_IDLE_TIME_SET");

    accumulate_idle_time_set(0);
}

// internal functions
/****
static gboolean accumulate_idle_time_increase(gpointer internal)
{
    accumulateIdleTime_t *idle_time = g_accumulate_idle_time;

    if (idle_time)
    {
        idle_time->milliseconds += (gint) internal;

        // if reached the threshold, call callback_on_threshold
        if (0 != idle_time->threshold)
        {
            if (idle_time->milliseconds >= idle_time->threshold)
            {
                if (idle_time->callback_on_threshold)
                {
                    idle_time->callback_on_threshold();
                    accumulate_idle_time_end();
                }
            }
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
****/

static void accumulate_idle_time_set(gint value)
{
    accumulateIdleTime_t *idle_time = g_accumulate_idle_time;

    if (idle_time)
    {
        idle_time->milliseconds = value;
    }
}

gint accumulate_idle_time_get()
{
    gint ms = 0;
    accumulateIdleTime_t *idle_time = g_accumulate_idle_time;

    if (idle_time)
    {
        ms = idle_time->milliseconds;
    }
    return ms;
}
