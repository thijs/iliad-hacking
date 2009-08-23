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
 * \file diplayUpdate.h
 * \brief 
 *
 * <File description>
 *
 */


#ifndef __SCRIBBLE_DISPLAYUPDATE_H
#define __SCRIBBLE_DISPLAYUPDATE_H

//screen refresh levels 
#define LOWEST_LEVEL				0
#define MAIN_WINDOW_EXPOSE_LEVEL		1
//#define MOZEMBED_UPDATE_LEVEL			2

/*
#define LISTER_EXPOSE_LEVEL         3
#define MAIN_WINDOW_EXPOSE_LEVEL    4
#define NO_DISPLAY_UPDATE_LEVEL     99
*/

//display quality
//#define DM_QUALITY_FULL                 0
//#define DM_QUALITY_TYPING               2

#include <liberdm/erdm.h>

/**
 * set address of gtk idle callback, where the e-inkt display update is requested
 *
 * @param status indicates whether or not not used
 * @param data is quality.see eDmquality in <liberdm/erdm.h>
 * typedef enum
  {
    dmQFull = 0,
    dmQBW,
    dmQTyping,
    dmQUndefined
  } eDmQuality;
 *
 * @returns TRUE on success
 */
gboolean display_update_request_screen_refresh(int level, gpointer data);

/**
 * Increase the level value of the screen refresh. Meaning that the screen refresh will only be requested
 * to the displayManager for levels equal or larger then this level value.
 *
 * @param level screen refresh level
 *
 * @returns TRUE on success
 */
void display_update_increase_level(int level);

#endif //__SCRIBBLE_DISPLAYUPDATE_H




