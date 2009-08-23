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

/**
 * \file diplayUpdate.h
 * \brief browser - browser application interface with the display manager
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __BROWSER_DISPLAYUPDATE_H__
#define __BROWSER_DISPLAYUPDATE_H__

//screen refresh levels 
#define LOWEST_LEVEL					0
#define MAIN_WINDOW_EXPOSE_LEVEL		1
#define MOZEMBED_UPDATE_LEVEL			2

#define DM_QUALITY_FULL                 0
#define DM_QUALITY_TYPING               2

/**
 * set address of gtk idle callback, where the e-inkt display update is requested
 *
 * @param status indicates whether or not not used
 * @param data not used
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

#endif //__BROWSER_DISPLAYUPDATE_H__




