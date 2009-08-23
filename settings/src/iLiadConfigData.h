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
 * \file iLiadConfigData.h
 * \brief setup application - E-reader config screen data handling                

 * <File description>
 * 
 */

#include <liberregxml/erregapi.h> 
 
#ifndef _ILIAD_CONFIG_DATA_H_
#define _ILIAD_CONFIG_DATA_H_

// init the eReader configuration data, given the already 
// in memory loaded registry data 
void iLiad_config_data_init(void);

void iLiad_config_data_destroy(void);
void iLiad_config_data_display(void);

// compare the flipbar direction at the startup of settings with 
// the one in the current config data
gboolean iLiad_config_data_change_flipbar_direction();

//returned value should be handled as a read only value
char* iLiad_config_data_get_language(void);

//callback routines from the GTK widgets associated with the config data
void on_keys_selection_update(erGtkSelectionGroup* selection, gpointer button, gpointer data);
void on_language_selection_update(erGtkSelectionGroup* selection, gpointer button, gpointer data);
void on_sound_selection_update(erGtkSelectionGroup* selection, gpointer button, gpointer data);
void on_keybuffering_selection_update(erGtkSelectionGroup* selection, gpointer button, gpointer data);
void on_flipbar_direction_selection_update(erGtkSelectionGroup* selection, gpointer button, gpointer data);

//store the config information in the registry
// reload registry data from er_registry.txt, handle lock, update data and
// store updated config registry data in er_registry.txt 
void iLiad_config_data_store(void);


#endif
