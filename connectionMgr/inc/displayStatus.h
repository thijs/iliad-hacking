/*
 * This file is part of connectionMgr.
 *
 * connectionMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * connectionMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file displayStatus.h
 * \brief control - application interface with the display manager  
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef __DISPLAY_STATUS_H__
#define __DISPLAY_STATUS_H__

#include <glib.h>

// screen refresh levels - 
// a higher number will overrule the screen refresh from a 
// lower number
#define LOWEST_LEVEL                0

#define SELECTION_FOCUS_IN_LEVEL    1
#define SELECTION_CLICKED_LEVEL     1

#define TEXT_ENTRY_FOCUS_IN_LEVEL   1
#define TEXT_ENTRY_CHANGED_LEVEL    1

#define BUTTON_FOCUS_IN_LEVEL       1

#define SELECTION_CHANGED_LEVEL     2

#define SETTING_ITEM_CHANGE         3
#define STATUS_ITEM_CHANGE          3
#define SETUP_PAGE_LAYOUT_CHANGE    3

//This always results in a full screen refresh
#define ONE_BUTTON_ITEM_CLICKED_LEVEL   4 
#define SETUP_SCREEN_EXPOSE_LEVEL       4
#define MAIN_WINDOW_EXPOSE_LEVEL        5

#define KEYBOARD_SHOW_HIDE_LEVEL        6

#define NO_DISPLAY_UPDATE_LEVEL         99

#define WAVEFORM_FULLSCREEN             0
#define WAVEFORM_BW                     1
#define WAVEFORM_TYPING                 2

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    void display_update_remove_idle_handler_id(void);

    /**
     * set address of gtk idle callback, 
     * where the e-inkt display update is requested
     *
     * @param level - priority of this screen udpate request
     * @param waveform - requested e-ink waveform
     *
     * @returns TRUE on success
     */
    gboolean display_update_request_screen_refresh(int level, int waveform);

    /**
     * Increase/decrease the level value of the screen refresh. 
     * Meaning that the screen refresh will only
     * be requested to the displayManager for levels equal 
     * or larger then this level value.
     *
     * @param level screen refresh level
     *
     * @returns TRUE on success
     */
    void display_update_increase_level(int level);
    void display_update_decrease_level(int level);
    int  display_update_get_level(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

// show where screen updates are requested, only in debug
#if (CN_DISPLAY_ON)
#define display_update_request_screen_refresh(level, data)                                                  \
        {                                                                                                   \
            CN_ERRORPRINTF("    -- refresh [%d] current_level [%d]", (level), display_update_get_level());  \
            display_update_request_screen_refresh(level,data);                                              \
        }
#define display_update_increase_level(level)                                                                \
        {                                                                                                   \
            CN_ERRORPRINTF("    -- increase [%d] current_level [%d]", (level), display_update_get_level()); \
            display_update_increase_level(level);                                                           \
        }
#define display_update_decrease_level(level)                                                                \
        {                                                                                                   \
            CN_ERRORPRINTF("    -- decrease [%d] current_level [%d]", (level), display_update_get_level()); \
            display_update_decrease_level(level);                                                           \
        }
#endif

#endif //__DISPLAY_STATUS_H__

