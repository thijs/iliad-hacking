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
 * \file settingsData.h
 * \brief setup application - E-reader settings data handling                

 * <File description>
 * 
 */

#ifndef _SETTINGS_DATA_H_
#define _SETTINGS_DATA_H_

// open the registry file en read the relevant settings data
void settings_data_init(void);

void settings_data_store_page(gint page);
void settings_data_store_currentpage();

// free allocated settings data
void settings_data_destroy(void);
 
#endif //_SETTINGS_DATA_H_
