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
 * \file displayUpdate.h
 * \brief control - application interface with the display manager                  
 * 
 * <File description>
 *
 */

#ifndef __DL_DISPLAYUPDATE_H__
#define __DL_DISPLAYUPDATE_H__

//screen refresh levels 
#define LOWEST_LEVEL                    0
#define TEXT_UPDATE_LEVEL               1
#define TEXT_EXPOSE_LEVEL               2
#define VIEW_WINDOW_EXPOSE_LEVEL        4

#define WAVEFORM_FULLSCREEN     0
#define WAVEFORM_BW             1
#define WAVEFORM_TYPING         2


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

#endif //__DL_DISPLAYUPDATE_H__
