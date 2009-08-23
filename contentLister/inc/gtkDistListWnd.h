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
 * \file distListWnd.h
 * \brief distListWnd 
 * 
 *  distListWnd provides GUI to let users choose the persons who users want to send to 
 *  
 * Copyright (C) 2007-2008 iRex Technologies B.V.
 */

#ifndef __DIST_LIST_WND_H__
#define __DIST_LIST_WND_H__


#define DIST_LIST_WND_WIDTH         666

#define DIST_LIST_WND_HEIGHT_SMALL  (CLIENT_AREA - LISTER_BOX_BORDER - TITLE_MIN_HEIGHT - LIST_ITEM_SPACING - LISTER_ITEM_HEIGHT - LIST_ITEM_SPACING - LIST_ITEM_SPACING)
#define DIST_LIST_WND_HEIGHT_BIG    (CLIENT_AREA - LISTER_BOX_BORDER - LIST_ITEM_SPACING)


#define DIST_LIST_WND_V_SPACING     15


#ifdef __cplusplus
extern "C"
{
#endif                          // __cplusplus


typedef struct _GtkDistListWnd
{
    GtkWidget*    parentWnd;
    GtkWidget*    settingItem;
    GtkWidget*    selectLabel;
    GtkWidget*    chkBtnTbl[14];
    GtkWidget*    finishLabel;
    gint          lastSelBtn;  // -1: no last selected button
    mdsSelType_e  selType;
} GtkDistListWnd;

typedef struct
        {
            gchar*   name;
            gboolean state;
        } distListWnd_selectionItem_t;


/**
 * Create an empty distribution list window
 *
 * @param parent - gtk event box on which to append the distribution list window
 *
 * @returns --
 */
void create_dist_list_wnd(GtkWidget *parent);

/**
 * Clear the distribution window, i.e. remove all selection lists from it
 *
 * @param --
 *
 * @returns --
 */
void dist_list_wnd_clear(void);

/**
 * Append a selection list to the distribution list window
 *
 * @param display_as - specifies how to display the item list
 * @param title - text to appear in title bar
 * @param instruction - text to appear above list of items
 * @param item_tbl - array of items to be displayed
 *
 * @returns erGtkSelectionGroup object for this list
 */
erGtkSelectionGroup* dist_list_wnd_add_list(const gchar*  display_as,
                                            const gchar*  title,
                                            const gchar*  instruction,
                                            const gchar** item_tbl);

/**
 * Show specified page of distribution list window
 *
 * @param page - pagenumber (1 ..) to display
 *
 * @returns --
 */
void dist_list_wnd_goto_page(gint page);

/**
 * Goto next page / previous page
 *
 * @param --
 *
 * @returns --
 */
void dist_list_wnd_page_forward(guint offset);
void dist_list_wnd_page_back(guint offset);

/**
 * Tell whether distribution list window is visible
 *
 * @param --
 *
 * @returns TRUE/FALSE - window visible / not visible
 */
gboolean dist_list_wnd_is_visible(void);


#ifdef __cplusplus
}
#endif                          //  __cplusplus

#endif                          // __DIST_LIST_WND_H__
