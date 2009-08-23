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
 * \file iLiadStartUpScreen.h
 * \brief settings - users customized startup behaviour Screen 
 *
 * Providing GUI interface to let users input their customized startup behaviour. 
 *
 */

#ifndef __ILIAD_STARTUP_SCREEN_H__
#define __ILIAD_STARTUP_SCREEN_H__

#include <sys/types.h>
#include "background.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define STARTUP_ITEM_PADDING_TOP     10
#define STARTUP_ITEM_PADDING_BOTTOM  10
#define STARTUP_ITEM_PADDING_LEFT    13
#define STARTUP_ITEM_PADDING_RIGHT   13

#define STARTUP_ITEM_SPACING         10

#define STARTUP_BEHAVIOUR_HEIGHT     300
#define STARTUP_CALIBRATION_HEIGHT   115

#define STARTUP_LABEL_WIDTH          (TITLE_MIN_WIDTH \
                                         - STARTUP_ITEM_PADDING_LEFT \
                                         - STARTUP_ITEM_PADDING_RIGHT)

#define STARTUP_BUTTON_WIDTH         310
#define STARTUP_BUTTON_HEIGHT        27

#define STARTUP_BUTTON_NUMBER        9

typedef struct
{
    GtkWidget* item;
    GtkWidget* label;
    GtkWidget* selection;
    GtkWidget* buttons[STARTUP_BUTTON_NUMBER];
}gtkBehaviour_t;

typedef struct 
{
    GtkWidget* item;
    GtkWidget* label;
    GtkWidget* button;
}gtkCalibration_t;

typedef struct
{
    GtkWidget*     widget;
    gtkBehaviour_t behaviour;
    gtkCalibration_t calibration;
}gtkStartUp_t;

GtkWidget *iLiad_startup_screen_create(void);
void iLiad_startup_screen_set_text(void);
void iLiad_startup_screen_display_data(void);

pid_t get_xtscal_pid();
void handle_xtscal_quit();
void stop_xtscal();

#ifdef __cplusplus
}
#endif

#endif //__ILIAD_STARTUP_SCREEN_H__
