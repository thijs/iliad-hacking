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
 * \file iLiadPincodeScreen.h
 * \brief settings - Pincode Screen
 *
 * in Pincode Screen of settings, user can switch on or off PINCODE
 * also can set new PINCODE
 *
 */
#ifndef __ILIAD_PINCODE_SCREEN_H__
#define __ILIAD_PINCODE_SCREEN_H__
#ifdef __cplusplus
extern "C"
{
#endif

// macro about pincode screen 
#define PINCODE_SCREEN_SPACING_V  10
#define PINCODE_SCREEN_SPACING_H  10

#define PINCODE_SCREEN_ITEM_PADDING_TOP  10
#define PINCODE_SCREEN_ITEM_PADDING_BOTTOM  10
#define PINCODE_SCREEN_ITEM_PADDING_LEFT  13
#define PINCODE_SCREEN_ITEM_PADDING_RIGHT  13

#define PINCODE_SELECT_ITEM_INTER_SPACING1     17
#define PINCODE_SELECT_ITEM_INTER_SPACING2     13

#define PINCODE_ONOFF_BK_WIDTH          666
#define PINCODE_ONOFF_BK_HEIGHT         115

#define PINCODE_SELECT_BK_WIDTH          666
#define PINCODE_SELECT_BK_HEIGHT         230    

#define PINCODE_LABELS_WIDTH    320
#define PINCODE_LABELS_HEIGHT   30

#define PINCODE_BUTTON_WIDTH 170
#define PINCODE_BUTTON_HEIGHT 27

#define PINCODE_ICON_WIDTH  75
#define PINCODE_ICON_HEIGHT 102

#define PINCODE_ONOFF_ITEM_LABEL_WIDTH (PINCODE_ONOFF_BK_WIDTH \
                                       - PINCODE_SCREEN_ITEM_PADDING_LEFT \
                                       - PINCODE_SCREEN_ITEM_PADDING_RIGHT)
#define PINCODE_SELECT_ITEM_LABEL_WIDTH (PINCODE_SELECT_BK_WIDTH \
                                       - PINCODE_SCREEN_ITEM_PADDING_LEFT \
                                       - PINCODE_SCREEN_ITEM_PADDING_RIGHT)
#define PINCODE_SELECT_MESSAGE_WIDTH (PINCODE_SELECT_BK_WIDTH \
                                       - PINCODE_SCREEN_ITEM_PADDING_LEFT \
                                       - PINCODE_SCREEN_ITEM_PADDING_RIGHT \
                                       - PINCODE_BUTTON_WIDTH \
                                       - PINCODE_SCREEN_SPACING_H)
#define PINCODE_SELECT_INFO_WIDTH (PINCODE_SELECT_BK_WIDTH \
                                       - PINCODE_SCREEN_SPACING_H \
                                       - PINCODE_SCREEN_ITEM_PADDING_LEFT \
                                       - PINCODE_SCREEN_ITEM_PADDING_RIGHT \
                                       - PINCODE_ICON_WIDTH)

//Create the iLiad Pincode screen 
//return the container (eventbox) where the pincode settings items are displayed 
GtkWidget *iLiad_pincode_create(void);

void iLiad_pincode_set_text(void);

void iLiad_pincode_display_data(void);

//  void iLiad_pincode_set_on (gboolean on);
//  gboolean iLiad_pincode_get_on (void);

#ifdef __cplusplus
}
#endif
#endif                          /* __ILIAD_PINCODE_SCREEN_H__ */
