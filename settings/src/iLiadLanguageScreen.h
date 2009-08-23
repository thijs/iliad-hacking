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
 * \file iLiadLanguageScreen.h
 * \brief settings -Language Selection Screen  
 *
 * Providing GUI to let users select language. 
 * Notes: This screen is not one page of gtk_notebook. 
 * 
 */
#ifndef __ILIAD_LANGUAGE_SCREEN_H__
#define __ILIAD_LANGUAGE_SCREEN_H__

#include <liberdm/display.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define LANGUAGE_SCREEN_WIDTH           SCREEN_WIDTH
#define LANGUAGE_SCREEN_HEIGHT          SCREEN_HEIGHT    

#define LANGUAGE_SCREEN_SPACING         24

#define LANGUAGE_BUTTON_WIDTH           640
#define LANGUAGE_BUTTON_HEIGHT          44    

#define LANGUAGE_SCREEN_PADDING_TOP     10   // default values, the acturalvalues depend on the number of buttons
#define LANGUAGE_SCREEN_PADDING_BOTTOM  10
#define LANGUAGE_SCREEN_PADDING_LEFT    (LANGUAGE_SCREEN_WIDTH - LANGUAGE_BUTTON_WIDTH) / 2   
#define LANGUAGE_SCREEN_PADDING_RIGHT   LANGUAGE_SCREEN_PADDING_LEFT

#define LANGUAGE_MAX_BUTTON_NUMBER      ( (LANGUAGE_SCREEN_HEIGHT \
                                           -LANGUAGE_SCREEN_PADDING_TOP \
                                           - LANGUAGE_SCREEN_PADDING_BOTTOM \
                                           + LANGUAGE_SCREEN_SPACING) \
                                           / (LANGUAGE_BUTTON_HEIGHT + LANGUAGE_SCREEN_SPACING) )

typedef struct
{
    GtkWidget*  window;
    GtkWidget*  selection;
}gtkLanguage_t;

GtkWidget* iLiad_language_screen_create(void);
void iLiad_language_screen_display_data(void);

#ifdef __cplusplus
}
#endif

#endif // __ILIAD_LANGUAGE_SCREEN_H__
