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

#ifndef __DISPLAY_STATUS_H__
#define __DISPLAY_STATUS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// screen refresh levels - a higher number will overrule the screen refresh from a 
// lower number
#define LOWEST_LEVEL            0
#define SELECTION_FOCUS_IN_LEVEL    1
#define SELECTION_CLICKED_LEVEL     2
#define TEXT_ENTRY_FOCUS_IN_LEVEL   3
#define TEXT_ENTRY_CHANGED_LEVEL    4
#define BUTTON_FOCUS_IN_LEVEL       5
#define SETTING_ITEM_CHANGE     6
#define SETUP_PAGE_LAYOUT_CHANGE    7
#define ONE_BUTTON_ITEM_CLICKED_LEVEL   8 //This always results in a full screen refresh
#define SETUP_SCREEN_EXPOSE_LEVEL   9
#define MAIN_WINDOW_EXPOSE_LEVEL    10
#define KEYBOARD_SHOW_HIDE_LEVEL        11
#define NO_DISPLAY_UPDATE_LEVEL         99

#define WAVEFORM_FULLSCREEN             0
#define WAVEFORM_BW                     1
#define WAVEFORM_TYPING                 2


/**
 * set address of gtk idle callback, where the e-inkt display update is requested
 *
 * @param status indicates whether or not not used
 * @param data corresponds to the e-ink waveform (value passed as integer). NULL corresponds to Full Update
 *
 * @returns TRUE on success
 */
gboolean display_update_request_screen_refresh(int level, int waveform);

/**
 * Increase the level value of the screen refresh. Meaning that the screen refresh will only be requested
 * to the displayManager for levels equal or larger then this level value.
 *
 * @param level screen refresh level
 *
 * @returns TRUE on success
 */
void display_update_increase_level(int level);

void display_update_decrease_level(int level);

int  display_update_get_level(void);

void display_update_reset_level(void);

// show where screen updates are requested, only in debug
#if (ST_SCREEN_ON)
#define display_update_request_screen_refresh(level, data)                                                  \
        {                                                                                                   \
            ST_ERRORPRINTF("    -- refresh [%d] current_level [%d]", (level), display_update_get_level());  \
            display_update_request_screen_refresh(level,data);                                              \
        }
#define display_update_increase_level(level)                                                                \
        {                                                                                                   \
            ST_ERRORPRINTF("    -- increase [%d] current_level [%d]", (level), display_update_get_level()); \
            display_update_increase_level(level);                                                           \
        }
#define display_update_decrease_level(level)                                                                \
        {                                                                                                   \
            ST_ERRORPRINTF("    -- decrease [%d] current_level [%d]", (level), display_update_get_level()); \
            display_update_decrease_level(level);                                                           \
        }
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__DISPLAY_STATUS_H__



