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
 * \file settings.h
 * \brief setup application - E-reader settings application screen creation and handling                

 * <File description>
 * 
 */

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// these values correspond with the notebook page numbers
typedef enum
{
    iLiad_startup_screen_t = 0,
    iLiad_config_screen_t,
    iLiad_archive_location_screen_t,
    iLiad_user_screen_t,
    iLiad_pincode_screen_t,
    iLiad_pc_connect_screen_t,
    iLiad_autoconnect_screen_t,
    iLiad_timedids_screen_t,
    iLiad_datetime_screen_t,
    iLiad_device_overview_screen_t,
    iLiad_undefined_screen_t
} SettingsScreen_t;


void    prepare_registry_write( void);
void    do_registry_write(void);

//Create all settings screens 
//return the container (notebook) where the settings screens are stored 
GtkWidget* settings_create(void);

// get current pagenumber, and optionally total number of pages
gint settings_get_current_page(gint* pagecount);

// Goto the next page of the currently displayed settings screen notebook
void settings_page_down(gint count);

// Goto the previous page of the currently displayed settings screen notebook
void settings_page_up(gint count);

// callback from the main window keypress event
gboolean on_settings_keypress(GtkWidget* widget, GdkEventKey* event, gpointer data);

// Open IPC channel to setup application
gboolean setupInstallIpcServer();

// show/hide the on-screen keyboard
void show_keyboard(gboolean show);
void show_lock_screen_icon(gboolean enable);
#endif //_SETTINGS_H_
