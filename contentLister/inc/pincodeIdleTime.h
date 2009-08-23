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
 * \file pincodeIdleTime.h
 * \brief content lister - accumulate time of being idle
 *
 * accumulate time of being idle, when being idle for a long time, reach the
 * threshold, call callback function of "accumulateIdleTime_t" to do 
 * something special, then end the accumulate idle time
 *
 * if no threshold, just accumulating idle time, don't call callback function
 *
 * in some case, e.g updateMgr and download Mgr, if user sets threshold to be 5 mins, 
 * but downloading process needs more time which can not be forecasted,
 * in this case, we need no threshold temporily
 *
 * in pincode's case, "accumulatedIdleTime_t" is based on window application. 
 * when create widget, begin to accumulate idle time at the same time.
 * when received key or button event, reset 'accumulate idle time' to zero
 * 
 * Copyright (C) 2006-2008 iRex Technologies B.V.
 * 
 */

#ifndef __PINCODE_IDLE_TIME_H__
#define __PINCODE_IDLE_TIME_H__
#ifdef __cplusplus
extern "C"
{
#endif

#define TIMEOUT_INTERNAL 100	// milliseconds

  typedef void on_threshold_t ();

  typedef struct _accumulateIdleTime_t accumulateIdleTime_t;

  struct _accumulateIdleTime_t
  {
    gint milliseconds;		// user idle time in milliseconds
    gint threshold;		// when 'milliseconds' reachs 'threshold', need to do something special
    // if threshold equals zero, that means, no threshold
    on_threshold_t *callback_on_threshold;	// threshold callback function

    gint internal;		// private-increase 'milliseconds' once after 'internal' milliseconds
    gint timeout_handler_id;	// private-timeout handler id
  };

/**
 * begin to accumulate idle time
 *
 * @param - 
 *
 * @returns 
 */
  void accumulate_idle_time_begin ();

/**
 * stop accumulating idle time
 *
 * @param - 
 *
 * @returns 
 */
  void accumulate_idle_time_end ();

/**
 * set threshold of 'accumulateIdleTime_t'
 *
 * @param - threshold-   
 * after being idle for 'threshold' milliseconds, call callback function on threshold
 * if threshold set zero, that means no threshold 
 *
 * @returns 
 */
  void accumulate_idle_time_set_threshold (gint threshold);

/**
 * set callback function on threshold of 'accumulateIdleTime_t"
 * when idle time reachs the threshold, call it to do something sepcial
 *
 * @param - func callback function address
 *
 * @returns 
 */
  void accumulate_idle_time_set_callback (on_threshold_t * func);

/**
 * reset 'accumulate idle time' to zero
 *
 * @param - 
 *
 * @returns 
 */
  void accumulate_idle_time_reset ();

/**
 * get the milliseconds for being idle
 *
 * @param - 
 *
 * @returns the milliseconds for being idle
 */
gint accumulate_idle_time_get ();

#ifdef __cplusplus
}
#endif

#endif				/* __PINCODE_IDLE_TIME_H__ */
