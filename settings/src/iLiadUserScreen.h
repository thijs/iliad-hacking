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
 * \file iLiadUserScreen.h
 * \brief setup application - E-reader user-profile screen                
 * 
 */

#ifndef _ILIAD_USER_SETTINGS_H_
#define _ILIAD_USER_SETTINGS_H_


#define USERSCREEN_ITEM_SPACING         10
#define ACCOUNT_SETTINGS_SPACING        17

#define USERNAME_ITEM_REQ_HEIGHT       146

#define USERSCREEN_ACC_HEIGHT          230
#define USERSCREEN_ITEM_PADDING_TOP     10
#define USERSCREEN_ITEM_PADDING_BOTTOM  10
#define USERSCREEN_ITEM_PADDING_LEFT    13
#define USERSCREEN_ITEM_PADDING_RIGHT   13

typedef struct
{
    GtkWidget* settingitem;
    GtkWidget* label_email;
    GtkWidget* entry_email;
    GtkWidget* label_password;
    GtkWidget* entry_password;
}gtkAccountSettings_t;

//Create the iLiad User-profile screen 
//return the container (eventbox) where the user-profile items are displayed 
GtkWidget* iLiad_user_create(void);

void iLiad_user_set_text(void);

void iLiad_user_set_name(const gchar* name);
void iLiad_user_set_email(const gchar* email);
void iLiad_user_set_password(const gchar* password);


#endif //_ILIAD_USER_SCREEN_H_

