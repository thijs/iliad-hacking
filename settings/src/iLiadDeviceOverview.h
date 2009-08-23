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
 * \file iLiadDeviceOverview.h
 * \brief settings - Display device detial information
 *
 * To allow Service and users to get more and neccessary device details
 *
 */

#ifndef __ILIAD_DEVICE_OVERVIEW_H__
#define __ILIAD_DEVICE_OVERVIEW_H__

#include <sys/types.h>
#include "background.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define DEV_INFO_ITEM_NUMBER                12
#define DEV_ACK_ITEM_NUMBER                  2
#define DEV_OVERVIEW_ITEM_SPACING           10
#define DEV_OVERVIEW_SEPARATOR_HEIGHT       10
#define DEVICE_INFO_HEIGHT                 395
#define DEVICE_ACK_HEIGHT                  115

#define DEVICE_OVERVIEW_ITEM_PADDING_TOP    10
#define DEVICE_OVERVIEW_ITEM_PADDING_BOTTOM 10
#define DEVICE_OVERVIEW_ITEM_PADDING_LEFT   13
#define DEVICE_OVERVIEW_ITEM_PADDING_RIGHT  13

#define ACK_DESCRIPTION_LABEL_WIDTH    (TITLE_MIN_WIDTH \
                                      - DEVICE_OVERVIEW_ITEM_PADDING_LEFT \
                                      - DEVICE_OVERVIEW_ITEM_PADDING_RIGHT)
#define DEVICE_INFO_LABEL_WIDTH        (ACK_DESCRIPTION_LABEL_WIDTH / 2)

#define VERSION_FILE            "/etc/er_version_commercial"
#define BUILD_FILE              "/etc/er_version"

typedef struct
{
    GtkWidget* settingitem;
    GtkWidget* label_item_title[DEV_INFO_ITEM_NUMBER];
    GtkWidget* label_item_value[DEV_INFO_ITEM_NUMBER];
}gtkDevInfo_t;

typedef struct
{
    GtkWidget* settingitem;
    GtkWidget* label_description;
    GtkWidget* label_ack_item[DEV_ACK_ITEM_NUMBER];
}gtkAck_t;

typedef struct
{
    GtkWidget*   top_level_vbox;
    gtkDevInfo_t gtk_dev_info;
    gtkAck_t     gtk_acknowledgement;
}gtk_dev_overview;

GtkWidget *iLiad_device_overview_create(void);
void iLiad_device_overview_set_text(void);
void iLiad_device_overview_display_data(void);

#ifdef __cplusplus
}
#endif

#endif //__ILIAD_DEVICE_OVERVIEW_H__
