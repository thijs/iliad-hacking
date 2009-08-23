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
 * \file iLiadAutoconnectScreen.h
 * \brief setup application - E-reader auto-connect screen                
 * 
 */

#ifndef _ILIAD_AUTOCONNECT_SETTINGS_H_
#define _ILIAD_AUTOCONNECT_SETTINGS_H_


#define BACKGROUND_MIN_INTERVAL         5
#define BACKGROUND_MAX_INTERVAL         99
#define BACKGROUND_DEFAULT_INTERVAL     15

#define BACKGROUND_ITEM_PADDING_TOP     10
#define BACKGROUND_ITEM_PADDING_BOTTOM  10
#define BACKGROUND_ITEM_PADDING_LEFT    13
#define BACKGROUND_ITEM_PADDING_RIGHT   13

#define BACKGROUNDSCREEN_ITEM_SPACING   5//10

#define BACKGROUND_ONOFF_REQ_HEIGHT     115
#define BACKGROUND_ONOFF_LABEL_WIDTH    (TITLE_MIN_WIDTH \
                                          - BACKGROUND_ITEM_PADDING_LEFT \
                                          - BACKGROUND_ITEM_PADDING_RIGHT)

#define BACKGROUND_IDSPC_REQ_HEIGHT     115
#define BACKGROUND_IDSPC_LABEL_WIDTH    (TITLE_MIN_WIDTH \
                                          - BACKGROUND_ITEM_PADDING_LEFT \
                                          - BACKGROUND_ITEM_PADDING_RIGHT)

#define BACKGROUND_INTERVAL_REQ_HEIGHT  115
#define BACKGROUND_INTERVAL_ENTRY_WIDTH 50


//Create the iLiad User-profile screen 
//return the container (eventbox) where the user-profile items are displayed 
GtkWidget* iLiad_autoconnect_create(void);

void iLiad_autoconnect_set_text(void);

void iLiad_ac_set_wlan_onoff( const gboolean enable);
void iLiad_ac_set_background_idspc(const gint connect_to);
void iLiad_ac_set_background_onoff(const gboolean enable);
void iLiad_ac_set_background_interval(const gint interval);


#endif



