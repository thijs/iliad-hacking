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
 * \file iLiadConfigScreen.h
 * \brief setup application - E-reader config screen                

 * <File description>
 * 
 */

#ifndef _ILIAD_CONFIG_SETTINGS_H_
#define _ILIAD_CONFIG_SETTINGS_H_

// When user changes 'language setting', the length of strings of each language 
// becomes longer or shorter, in this case, we shouldn't use widget height to 
// control 'config screen item' spacing, should use spacing and padding to control
// So the #define ..._REQ_HEIGHT should be removed, but we leave them as comment for now.
#define CONFIGSCREEN_ITEM_SPACING        11
#define CONFIGSCREEN_INTER_SPACING       0
#define CONFIGSCREEN_INTRA_PADDING       5
#define CONFIGSCREEN_INTER_PADDING       15
//#define CALIBRATION_ITEM_REQ_HEIGHT      90
//#define SOUND_ITEM_REQ_HEIGHT            90
//#define KEYBUFFERING_ITEM_REQ_HEIGHT    130
//#define KEYS_ITEM_REQ_HEIGHT            110
//#define FLIPBAR_DIRECTION_ITEM_REQ_HEIGHT    200
//#define LANGUAGE_ITEM_REQ_HEIGHT        250
#define LANGUAGE_BUTTONS_PER_LINE        4

//Create the iLiad Config screen 
//return the container (eventbox) where the config settings items are displayed 
GtkWidget *iLiad_config_create(void);

void iLiad_config_set_text(void);

void iLiad_config_set_language(const gchar *locale);
const gchar *iLiad_config_get_language(void);

void iLiad_config_set_keys(guint timeout);
guint iLiad_config_get_keys(void);

void iLiad_config_set_volume(guint volume);
guint iLiad_config_get_volume(void);

void iLiad_config_set_keybuffering(guint volume);
guint iLiad_config_get_keybuffering(void);

void iLiad_config_set_flipbar_direction(gboolean left);
gboolean iLiad_config_get_flipbar_direction(void);

#endif //_ILIAD_CONFIG_SETTINGS_H_
