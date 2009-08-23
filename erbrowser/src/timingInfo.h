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
//
// Description: timing information
//
// Author: ann <ann@localhost>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TIMINGINFO_h
#define TIMINGINFO_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// used to enable/disable the timing information 
// #define TIMING_INFORMATION_ENABLED

int timing_display_time_elapsed(void);

int timing_init(void);
int timing_destroy(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif //TIMINGINFO_h
