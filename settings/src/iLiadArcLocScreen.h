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
 * \file iLiadArcLocScreen.h
 * \brief settings - Archive Location Screen
 *
 * Providing GUI interface to let users input their personalize archive location. 
 * 
 */

#ifndef __ILIAD_ARC_LOC_SCR_H__
#define __ILIAD_ARC_LOC_SCR_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define ARC_LOC_SCR_SPACING_V          10
#define ARC_LOC_SCR_SPACING_H          10

#define ARC_LOC_ITEM_BK_W              666
#define ARC_LOC_ITEM_BK_H              115

#define ARC_LOC_SCR_ITEM_PADDING_T     10
#define ARC_LOC_SCR_ITEM_PADDING_B     10
#define ARC_LOC_SCR_ITEM_PADDING_L     13
#define ARC_LOC_SCR_ITEM_PADDING_R     13


#define ARC_LOC_ITEM_LABEL_W (ARC_LOC_ITEM_BK_W - ARC_LOC_SCR_ITEM_PADDING_L - ARC_LOC_SCR_ITEM_PADDING_R)

#define ARC_LOC_BTN_W                  150 
#define ARC_LOC_BTN_H                  27
#define ARC_LOC_BTN_NUM                (arcLocUndefined)
#define ARC_LOC_BTN_SPACING_H          ( (ARC_LOC_ITEM_LABEL_W - (ARC_LOC_BTN_W * ARC_LOC_BTN_NUM)) / (ARC_LOC_BTN_NUM - 1) )

typedef struct
{
    const arcType_e  arcType;
    GtkWidget       *title;
    GtkWidget       *subTitle;
    GtkWidget       *selection;
    GtkWidget       *buttons[ARC_LOC_BTN_NUM];
} gtkArcLoc_t;

GtkWidget *iLiad_archive_location_create(void);

void iLiad_archive_location_set_text(void);

void iLiad_archive_location_display_data(void);

#ifdef __cplusplus
}
#endif

#endif                          /* __ILIAD_ARC_LOC_SCR_H__ */
