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
 * \file iLiadDateTimeScreen.h
 * \brief settings - Date time screen
 *
 * User interface for user to set iLiad time
 *
 */

#ifndef __ILIAD_DATETIME_SCREEN_H__
#define __ILIAD_DATETIME_SCREEN_H__

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define DATETIME_SCREEN_ITEM_SPACING            10
#define DATETIME_DISPLAY_SPACING                25
#define TIMEZONE_BUTTON_ITEM_SPACING            35
#define DATETIME_DISPLAY_HEIGHT                115
#define TIMEZONE_SELECTION_HEIGHT              425

#define DATETIME_SCREEN_ITEM_PADDING_TOP        10
#define DATETIME_SCREEN_ITEM_PADDING_BOTTOM     10
#define DATETIME_SCREEN_ITEM_PADDING_LEFT       13
#define DATETIME_SCREEN_ITEM_PADDING_RIGHT      13

#define TZ_BUTTON_WIDTH                        100
#define TZ_BUTTON_HEIGHT                        30

#define TOTAL_TIMEZONES                         34
#define DATE_STRING_LENGTH                      64

#define TZ_SETTING_FILE   "/mnt/settings/timezone"
#define TZ_FILES_DIR      "/usr/share/zoneinfo/Etc/"
#define TZ_FILE           "/etc/localtime"

#define DEFAULT_TIMEZONE_OFFSET   0
#define DEFAULT_TIMEZONE_NAME     "GMT+00:00"

typedef struct
{
    GtkWidget* top_vbox;
    GtkWidget* settingitem;
    GtkWidget* timezones;
    GtkWidget* label_show_time_prompt;
    GtkWidget* label_current_time;
    GtkWidget* label_timezone_prompt;
    GtkWidget* label_possible_date[2];
    GtkToggleButton* button_timezones[TOTAL_TIMEZONES+1];
    GtkWidget* selection;
    GtkWidget* infoitem;
}gtk_datetime_screen;

typedef struct
{
    int  tzoffset;
    char tzname[10];
}tz_info;

// Header of timezone files, you can find them in /usr/share/zoneinfo
struct tzhead
{
    char tzh_magic[4];       /* TZ_MAGIC */
    char tzh_reserved[16];   /* reserved for future use */
    char tzh_ttisgmtcnt[4];  /* coded number of trans. time flags */
    char tzh_ttisstdcnt[4];  /* coded number of trans. time flags */
    char tzh_leapcnt[4];     /* coded number of leap seconds */
    char tzh_timecnt[4];     /* coded number of transition times */
    char tzh_typecnt[4];     /* coded number of local time types */
    char tzh_charcnt[4];     /* coded number of abbr. chars */
};

// Create the iLiad date time screen
// return the container (eventbox) where the datetime settings items are displayed
GtkWidget *iLiad_date_time_create();

void iLiad_date_time_set_text(void);
void iLiad_date_time_display_data(void);
void iLiad_date_time_handle_refresh(void);

#ifdef __cplusplus
}
#endif
#endif //__ILIAD_DATETIME_SCREEN_H__
