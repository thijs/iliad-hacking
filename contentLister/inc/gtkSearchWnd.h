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
 * \file gtkSearchWnd.h
 * \brief gtkSearchWnd - eReader content search dialog
 * 
 * <File description>
 *  The eReader content customization window provides gtk widget to enable user customize
 *  content title and description. Which are created and updated with the following API
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __GTKSEARCHWND_H_
#define __GTKSEARCHWND_H_

//////////////////////////////////////////////////////////////////////////
// content search window
//////////////////////////////////////////////////////////////////////////

#include <gtk/gtk.h>

#define SEARCH_BK_IMG_WIDTH         666
#define SEARCH_BK_IMG_HEIGHT        230

#define SEARCH_WND_SPACE_ABOVE      300
#define SEARCH_WND_ITEM_SPACING     10

#define SEARCH_VBORDER              10
#define SEARCH_HBORDER              20
#define SEARCH_ITEM_WIDTH           (SEARCH_BK_IMG_WIDTH - 2*SEARCH_HBORDER)

#define SEARCH_TAG_HEIGHT           25
#define SEARCH_NAME_HEIGHT          30
#define SEARCH_NAME_EXPL_HEIGHT     60

#define SEARCH_BUTTON_AREA_HEIGHT   30
#define SEARCH_BUTTON_WIDTH         200
#define SEARCH_BUTTON_HEIGHT        27


#define SEARCH_PATTERN_MAX          256

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */



typedef struct
{
    GtkWidget*      window;         // parent window, specified by other module
    GtkWidget*      patternTag;     // pattern edit widget
    GtkWidget*      patternEntry;   //
    GtkWidget*      patternExpl;    //
    GtkWidget*      searchTag;      // search button
    GtkWidget*      searchButton;   //
    gboolean        on_change_occurred;
    gboolean        on_movecursor_occurred;
    gboolean        display_update_pending;
} swSearch_t;


/**
 * Create search options window
 *
 * @param parent Gtk object
 *
 * @returns instance pointer of window. Return NULL if failed.
 */
GtkWidget* create_search_wnd(GtkWidget* parent);

/**
 * Set constant text in search window
 */
void search_wnd_set_text(void);



#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  //__GTKSEARCHWND_H_

