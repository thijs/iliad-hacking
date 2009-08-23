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
 * \file iLiadUserData.h
 * \brief setup application - E-reader user-profile screen data handling                
 * 
 */

#include <liberregxml/erregapi.h> 
 
#ifndef _ILIAD_USER_DATA_H_
#define _ILIAD_USER_DATA_H_


// init the eReader user-profile data, given the already 
// in memory loaded registry data 
void iLiad_user_data_init(void);

void iLiad_user_data_destroy(void);
void iLiad_user_data_display(void);


//callback routines from the GTK widgets associated with the user-profile data
void on_user_name_changed(GtkWidget* item, const gchar* text);
void on_user_email_changed(GtkWidget* item, const gchar* text);
void on_user_password_changed(GtkWidget* item, const gchar* text);


//store the user-profile information in the registry
// reload registry data from er_registry.txt, handle lock, update data and
// store updated config registry data in er_registry.txt 
void iLiad_user_data_store(void);

regUserProfile_t* get_user_profile_ptr();

#endif
