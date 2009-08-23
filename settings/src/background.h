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
 * \file background.h
 * \brief setup application - E-reader settings application background creation/update                

 * <File description>
 * 
 */

#ifndef _SETTINGS_BACKGROUND_H_
#define _SETTINGS_BACKGROUND_H_

#define VERSION_FILE            "/etc/er_version_commercial"

#define SETTINGS_BOX_BORDER     51      //white border around the full settings page 768x1024

#define TITLE_MIN_HEIGHT        54
#define TITLE_MIN_WIDTH         666  
#define TITLE_BORDER_LEFT       10
#define TITLE_BORDER_RIGHT      10

#define SETTINGS_ITEM_PADDING   10


//create and display the background of the setting screens
//return the container where the page specific content of the 
//settings screens is displayed 
GtkWidget* bg_create(GtkWidget* window);

void bg_set_text(void);

#endif //_SETTINGS_APP_LOG_H_



