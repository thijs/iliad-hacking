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
 * \file displayUpdate.h
 * \brief control - application interface with the display manager                  
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __LISTER_DISPLAYUPDATE_H__
#define __LISTER_DISPLAYUPDATE_H__

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */


//screen refresh levels
enum
{
    LOWEST_LEVEL = 0,
    LIST_ITEM_FOCUS_LEVEL,
    TOOLBAR_UPDATE_LEVEL,
    TEXTENTRY_CHANGED_LEVEL,
    LISTER_EXPOSE_LEVEL,
    MAIN_WINDOW_EXPOSE_LEVEL,
    KEYBOARD_SHOW_HIDE_LEVEL,
    NO_DISPLAY_UPDATE_LEVEL = 99
};

/**
 * set address of gtk idle callback, where the e-inkt display update is requested
 *
 * @param status indicates whether or not not used
 *
 * @returns TRUE on success
 */
gboolean display_update_request_screen_refresh(int level);

/**
 * Increase the level value of the screen refresh. Meaning that the screen refresh will only be requested
 * to the displayManager for levels equal or larger then this level value.
 *
 * @param level screen refresh level
 *
 * @returns TRUE on success
 */
void display_update_increase_level(int level);

/**
 * Decrease the level value of the screen refresh. Meaning that the screen refresh will only be requested
 * to the displayManager for levels equal or larger then this level value.
 *
 * @param level screen refresh level
 *
 * @returns TRUE on success
 */
void display_update_decrease_level(int level);

int display_update_get_level(void);


// Show where display_update is called from, only when debug enabled.
// CL_ERRORPRINTF prints the location where display_update_request_screen_refresh() is called from,
// usefull in case too many screen updates occur.
#if (CL_SCREEN_DEBUG_ON)
#define display_update_request_screen_refresh(level)                                                        \
        {                                                                                                   \
            CL_ERRORPRINTF("    -- refresh [%d] current_level [%d]", (level), display_update_get_level());  \
            display_update_request_screen_refresh(level);                                                   \
        }
#define display_update_increase_level(level)                                                                \
        {                                                                                                   \
            CL_ERRORPRINTF("    -- increase [%d] current_level [%d]", (level), display_update_get_level()); \
            display_update_increase_level(level);                                                           \
        }
#define display_update_decrease_level(level)                                                                \
        {                                                                                                   \
            CL_ERRORPRINTF("    -- decrease [%d] current_level [%d]", (level), display_update_get_level()); \
            display_update_decrease_level(level);                                                           \
        }
#endif


#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__LISTER_DISPLAYUPDATE_H__
