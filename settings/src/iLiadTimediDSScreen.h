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
 * \file iLiadTimediDSScreen.h
 * \brief settings - Timed iDS connection settings
 *
 * User interface for user to set the timed iDS connection
 *
 */

#ifndef __ILIAD_TIMEDIDS_SCREEN_H__
#define __ILIAD_TIMEDIDS_SCREEN_H__

#include "background.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TIMEDIDS_ITEM_SPACING                10
#define ENABLE_TIMEDIDS_HEIGHT              115

#define TIMEDIDS_SCREEN_ITEM_PADDING_TOP     10
#define TIMEDIDS_SCREEN_ITEM_PADDING_BOTTOM  10
#define TIMEDIDS_SCREEN_ITEM_PADDING_LEFT    13
#define TIMEDIDS_SCREEN_ITEM_PADDING_RIGHT   13

#define ENABLE_TIMEDIDS_LABEL_WIDTH    (TITLE_MIN_WIDTH \
                                      - TIMEDIDS_SCREEN_ITEM_PADDING_LEFT \
                                      - TIMEDIDS_SCREEN_ITEM_PADDING_RIGHT)                      
#define ENABLE_TIMEDIDS_ITEM_SPACING         15
#define TIME_SETTINGS_HEIGHT                230
#define TIME_SETTINGS_ITEM_SPACING           10
#define TIME_SETTINGS_CONNECTAT_WIDTH       120
#define TIME_SETTINGS_COLON_WIDTH            10
#define TIME_SETTINGS_ENTRY_WIDTH            40
#define TIME_SETTINGS_ENTRY_HEIGHT           30

typedef struct
{
    GtkWidget* settingitem;
    GtkWidget* label_desc;
    GtkToggleButton* button_timedids_on;
    GtkToggleButton* button_timedids_off;
}gtk_enable_timedids_t;

typedef struct
{
    GtkWidget* label_connectat;
    GtkWidget* entry_hour;
    GtkWidget* entry_minute;
}gtk_time_entry_t;

typedef struct
{
    GtkWidget* settingitem;
    GtkWidget* label_desc;
    gtk_time_entry_t time_entries[TIME_OPTIONS];
    GtkWidget* check_software_update;
    GtkWidget* check_auto_shutdown;
}gtk_time_settings_t;

typedef struct
{
    gtk_enable_timedids_t enable_timedids;
    gtk_time_settings_t settime;
    GtkWidget* infoitem;
}gtk_timedids_screen_t;
    
GtkWidget* iLiad_timedids_screen_create(void);
void iLiad_timedids_screen_set_text(void);
void iLiad_timedids_screen_data_display(void);
void iLiad_timedids_handle_page_switch(void);

#ifdef __cplusplus
}
#endif
#endif //__ILIAD_TIMEDIDS_SCREEN_H__
