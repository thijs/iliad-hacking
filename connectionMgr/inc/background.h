/*
 * This file is part of connectionMgr.
 *
 * connectionMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * connectionMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file background.h
 * \brief connectionMgr - background creation/update
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <liberdm/display.h>

#define BACKGROUND_TITLE_OFFSET  10

#define TITLE_MIN_HEIGHT         54
#define TITLE_MIN_WIDTH          666

//white border around the full screen
#define SCREEN_BORDER            ((SCREEN_WIDTH - TITLE_MIN_WIDTH) / 2)
#define SCREEN_BORDER_TOP        (SCREEN_BORDER)
#define SCREEN_BORDER_BOTTOM     1
#define SCREEN_BORDER_LEFT       (SCREEN_BORDER)
#define SCREEN_BORDER_RIGHT      (SCREEN_BORDER)

#define SCREEN_ITEM_SPACING      9

// the definition of background's width and height
#define SITEM_BK_WIDTH   666
#define SITEM_BK_HEIGHT  75

#define BK_666_140_WIDTH    666
#define BK_666_140_HEIGHT   140

#define BK_666_168_WIDTH    666
#define BK_666_168_HEIGHT   168

#define BK_666_245_WIDTH    666
#define BK_666_245_HEIGHT   245

#define BK_666_273_WIDTH    666
#define BK_666_273_HEIGHT   273

// the alignment 
#define PADDING_TOP      10
#define PADDING_BOTTOM   10 
#define PADDING_LEFT     13
#define PADDING_RIGHT    13

#define HBOX_SPACING     5
#define VBOX_SPACING     9

#define LBUTTON_WIDTH    310
#define LBUTTON_HEIGHT   27
#define BUTTON_WIDTH     200
#define BUTTON_HEIGHT    27
#define SBUTTON_WIDTH    130
#define SBUTTON_HEIGHT   27

#define LABEL_WIDTH      640
#define LABEL_HEIGHT     -1

#define ENTRY_WIDTH      400
#define ENTRY_HEIGHT     27

#define MAX_PROFILES_PER_PAGE 12 

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    //create and display the background of the setting screens
    //return the container where the page specific content of the 
    //settings screens is displayed 
    GtkWidget* bg_create(GtkWidget* window);

    void bg_set_text(void);

    void bg_display_title(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif //_BACKGROUND_H_

