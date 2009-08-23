/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file lister.h
 * \brief lister - eReader content Lister page creation                  
 * 
 * <File description>
 *  The eReader content Lister page contains a predefined number
 *  of gtkContentListItems. Which are created and updated with the following API
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __LISTER__H__
#define __LISTER__H__

#include "erMdsContent.h"
#include "gtkContentListItem.h"

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */


#define LIST_ITEM_SPACING       20

// init value
#define NO_FOCUSSED_ITEM       (-1)

// only used to allocate data structures => layout uses fixed sizes => fixed # items on a page
#define MAX_ITEMS_ON_ONE_PAGE	24

// string size limitations for the list item text
#define LS_MAX_TITLE_SIZE       256
#define LS_MAX_SUBTITLE_SIZE    256
#define LS_MAX_DESCRIPTION_SIZE 512

#define LISTER_BOX_BORDER        51     // white border around the full contentlister page 768x1024
#define TITLE_MIN_HEIGHT         54
#define TITLE_MIN_WIDTH         666  
#define LISTER_ITEM_HEIGHT      115

#define CURSOR_X        (LISTER_BOX_BORDER - HBOX_TEXT_THUMB_SPACING)	
#define CURSOR_START_Y	(LISTER_BOX_BORDER + TITLE_MIN_HEIGHT + LIST_ITEM_SPACING)
#define CURSOR_STEP     (LIST_ITEM_SPACING + LISTER_ITEM_HEIGHT)

// location of metadata edit window
#define KEYBOARD_TOP                  680     // top-of-screen till top-of-keyboard
#define METADATA_CLIENTAREA_HEIGHT    (KEYBOARD_TOP - LISTER_BOX_BORDER - TITLE_MIN_HEIGHT - LIST_ITEM_PADDING - LISTER_ITEM_HEIGHT - LISTER_BOX_BORDER - 30)

typedef struct
{
    GtkWidget *titleItem;
    GtkWidget *titleLocation;      // display location information
    GtkWidget *listItems[MAX_ITEMS_ON_ONE_PAGE];
} lsLister_t;


/**
 * Init Lister page, by creating a predefined number of gtkContentListItems
 * and assigning "control" callback routines
 *
 * @param topLevelWindow reference to the top level window
 * @param data value that needs to be passed in lister item callback routines
 *
 * @returns lister containing the gtkContentListItems and header
 */
lsLister_t *lsInit(GtkWidget * topLevelWindow, gpointer data);

/**
 * Return the specified lister item
 *
 * @param lister reference to the lister page
 * @param index  sequence number of lister item on this page (0 ..)
 *
 * @returns lister item or NULL
 */
GtkWidget * lsGetListerItem(lsLister_t * lister, int index);

/**
 * Update the items of the lister widget
 *
 * <note>
 * the max itemcount/page is the fixed value "MAX_ITEMS_ON_ONE_PAGE"
 * so when more items are available, only the first MAX_ITEMS_ON_ONE_PAGE will be shown
 *
 * @param lister reference to the lister page 
 * @param items  array page items, containing the data that needs to be displayed
 * @param count number of items in the array
 * @param focus item that should get the focus
 * @param category category name - used as the header fro the lster page 
 * @param location current location information - where are we
 *
 * @returns -
 */
void lsUpdatePage(lsLister_t * lister, clDisplayItem_t * items, 
        int count, int focus, gchar* category, const gchar *location);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  //__LISTER__H__
