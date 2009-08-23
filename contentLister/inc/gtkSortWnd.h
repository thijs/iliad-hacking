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
 * \file gtkSortWnd.h
 * \brief gtkSortWnd - eReader content lister- Sort Settings 
 * 
 * <File description>
 *  gtkSortWnd provides GUI to let users choose their sorting type, sorting direction
 *  
 * Copyright (C) 2006-2008 iRex Technologies B.V.
 *
 */

#ifndef __GTK_SORT_WND_H__
#define __GTK_SORT_WND_H__

#include <gtk/gtk.h>

#define SORT_WND_SORTTYPE_BK_WIDTH       666
#define SORT_WND_SROTTYPE_BK_HEIGHT      230

#define SORT_WND_SORTORDER_BK_WIDTH       666
#define SORT_WND_SORTORDER_BK_HEIGHT      115

#define SORT_WND_FINISH_BK_WIDTH       666
#define SORT_WND_FINISH_BK_HEIGHT      115

#define BUTTON_NUM                      4
#define SORT_WND_BUTTON_HEIGHT         27
#define SORT_WND_BUTTON_WIDTH          150

#define SORT_WND_SPACING_V             8
#define SORT_WND_SPACING_H             (   (SORT_WND_SORTTYPE_BK_WIDTH - SORT_WND_LEFT_PADDING - SORT_WND_RIGHT_PADDING - (BUTTON_NUM * SORT_WND_BUTTON_WIDTH)) \
                                         / (BUTTON_NUM - 1) )

#define SORT_WND_LEFT_PADDING          17
#define SORT_WND_RIGHT_PADDING         17
#define SORT_WND_TOP_PADDING           10
#define SORT_WND_BOTTOM_PADDING        10


#define ASCENDING_BTN_WIDTH            29
#define ASCENDING_BTN_HEIGHT           29

#ifdef __cplusplus
extern "C"
{
#endif                          // __cplusplus

typedef struct
{
    GtkWidget *window;              // the top widget of GtkSortWnd
    GtkWidget *sortLabel;
    GtkWidget *sortTypeLabel;       // choose sort Type text label GtkLabel
    GtkWidget *sortTypeSelection;   // GtkSelection
    GtkWidget *sortTagLabel;        // choose sort tag text label GtkLabel

    GtkWidget *sortOrderLabel;      // choose sort direction 
    GtkWidget *selLabel;            // your selection label
    GtkWidget *selSortLabel;        // your selected sort type label
    GtkWidget *sortOrderSelection;  // GtkSelection  

    GtkWidget *finishLabel;         // finish text label 
}GtkSortWnd;

GtkWidget *create_sort_wnd(GtkWidget * parent);

void sort_wnd_set_text(void);

void sort_wnd_get_sort_settings(mdsSortStruct_t * sortStruct);

void sort_wnd_set_sort_settings(const mdsSortStruct_t * sortStruct);

#ifdef __cplusplus
}
#endif                          //  __cplusplus

#endif                          // __GTK_SORT_WND_H__
