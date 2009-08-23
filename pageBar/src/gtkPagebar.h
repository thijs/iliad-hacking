/*
 * This file is part of pageBar.
 *
 * pageBar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pageBar is distributed in the hope that it will be useful,
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

#ifndef __PAGEBAR_H__
#define __PAGEBAR_H__

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_PAGEBAR_TYPE            (gtk_pagebar_get_type ())
#define GTK_PAGEBAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_PAGEBAR_TYPE, GtkPagebar))
#define GTK_PAGEBAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_PAGEBAR_TYPE, GtkPagebarClass))
#define IS_GTK_PAGEBAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_PAGEBAR_TYPE))
#define IS_GTK_PAGEBAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_PAGEBAR_TYPE))

#define USE_BITMAPS

/* Default settings */
#define PAGEBAR_DEFAULT_PAGECOUNT             1
#define PAGEBAR_DEFAULT_CURRENTPAGE           1
#define PAGEBAR_DEFAULT_CURRENTPAGEOFFSET     100
#define PAGEBAR_DEFAULT_ZOOMMODE              0
#define PAGEBAR_DEFAULT_ORIENTATION           0
#define PAGEBAR_DEFAULT_DRAWAREAHEIGHT        50
#define PAGEBAR_DEFAULT_DRAWAREAXPOS          0
#define PAGEBAR_DEFAULT_DRAWAREAYPOS          SCREEN_HEIGHT-PAGEBAR_DEFAULT_DRAWAREAHEIGHT
#define PAGEBAR_DEFAULT_DRAWAREAWIDTH         SCREEN_WIDTH
#define PAGEBAR_DEFAULT_BARXPOS               9
#define PAGEBAR_DEFAULT_BARYPOS               0
#define PAGEBAR_DEFAULT_BARWIDTH              SCREEN_WIDTH-18
#define PAGEBAR_DEFAULT_BARHEIGHT             42
#define PAGEBAR_DEFAULT_CPIHEIGHT             42
#define PAGEBAR_DEFAULT_CPIWIDTH              59
#define PAGEBAR_DEFAULT_BARFONTTYPE           0
#define PAGEBAR_DEFAULT_BARFONTHEIGHT         8
#define PAGEBAR_DEFAULT_CPIFONTTYPE           0
#define PAGEBAR_DEFAULT_CPIFONTHEIGHT         8
#define PAGEBAR_DEFAULT_PAGEOFFSETXOFFSET     6
#define PAGEBAR_DEFAULT_PAGEOFFSETYOFFSET     34
#define PAGEBAR_DEFAULT_PAGEOFFSETWIDTH       47
#define PAGEBAR_DEFAULT_PAGEOFFSETHEIGHT      3
#define PAGEBAR_DEFAULT_BOOKMARKCOUNT         0
#define PAGEBAR_DEFAULT_BOOKMARKMAXVALUE      PAGEBAR_DEFAULT_PAGECOUNT
#define PAGEBAR_DEFAULT_NOTECOUNT             0
#define PAGEBAR_DEFAULT_NOTESMAXVALUE         PAGEBAR_DEFAULT_PAGECOUNT
#define PAGEBAR_DEFAULT_PAGEFIELDWIDTH        36
#define PAGEBAR_DEFAULT_PAGEFIELDHEIGHT       19
#define PAGEBAR_DEFAULT_SCROLLBUTTONWIDTH     18
#define PAGEBAR_DEFAULT_SCROLLBUTTONHEIGHT    19
#define PAGEBAR_DEFAULT_SHOW                  1

#define PAGEBAR_TIMER_NONE                    0  /* no timer */
#define PAGEBAR_TIMER_PROGRESS                1  /* */
	
	
/* Defines */
#define PAGEBAR_CPI_EXTRA_WIDTH               8


typedef struct _GtkPagebar       GtkPagebar;
typedef struct _GtkPagebarClass  GtkPagebarClass;

enum _icons {
    bitmap_CPI=0,
    bitmap_CPIExpand,
    bitmap_CPICollapse,
    bitmap_PageField,
    bitmap_ScrollLeft,
    bitmap_ScrollRight,
    bitmap_Bookmark,
    bitmap_Note,
    bitmap_count
};


struct _GtkPagebar
{
    GtkEventBox eventbox;				
    GtkWidget* drawingArea;
    GdkPixmap* pixmap;  				
    PangoLayout* pangolayout;
    int pagecount;
    int currentPage;
    int currentPageOffset;
    int zoomMode;
    /* The user must be able to change these params at runtime. That's why it aren't defines */
    GdkPixbuf* bitmaps[bitmap_count];
    int orientation;
    int drawAreaXPos;
    int drawAreaYPos;
    int drawAreaWidth;
    int drawAreaHeight;
    int barXPos;
    int barYPos;
    int barWidth;
    int barHeight;
    int cpiXPos;
    int cpiYPos; 
    int cpiHeight;
    int cpiWidth;
    int cpiFontType;
    int cpiFontHeight;
    int barFontType;
    int barFontHeight;
    int pageFieldWidth;
    int pageFieldHeight;
    int pageOffsetXOffset;
    int pageOffsetYOffset;
    int pageOffsetWidth;
    int pageOffsetHeight;
    int scrollButtonWidth;
    int scrollButtonHeight;
    int* bookmarks;
    int bookmarkCount;
    int bookmarkMaxValue;
    int* notes;
    int noteCount;
    int notesMaxValue;
    int show;
    /* Calculated values */
    int barFontWidth;
    int cpiFontWidth;
    int lastPage;
    int maxItems;       // max items can be displayed in detail

    int currentApp;
    GtkWidget* pageEntry;
    int lastDrawAreaXPos;
    int lastDrawAreaYPos;
    int lastDrawAreaWidth;
    int lastDrawAreaHeight;
    int currentTimer;
};

struct _GtkPagebarClass
{
    GtkEventBoxClass parent_class;

    void (*pagebar_page_selected) (GtkPagebar *pagebar, int page);

};

/* creates a new empty - zero pages - GtkPagebar widget */
GtkWidget* gtk_pagebar_new();

/* returns type of this widget */
GType gtk_pagebar_get_type (void);

/* draws a pagebar */ 
int gtk_pagebar_draw(GtkPagebar* pagebar);

/* display page, as being selected */
void gtk_pagebar_selectpage(GtkPagebar* pageBar, int page);

/* retrieve the pagecount of the current pagebar */
int gtk_pagebar_get_pagecount(GtkPagebar* pageBar);

/* retrieve the currently selected page */
int gtk_pagebar_get_selected_pagenumber(GtkPagebar* pageBar);

/* progress timer control */
void gtk_pagebar_startProgressTimer(GtkPagebar* pageBar);
void gtk_pagebar_stopTimer(GtkPagebar* pageBar);

int addBookmark(GtkPagebar* pagebar, int value);
int removeBookmark(GtkPagebar* pagebar, int value);

int addNote(GtkPagebar* pagebar, int value);
int removeNote(GtkPagebar* pagebar, int value);

int pagebar_reset_values(GtkPagebar* pagebar);

void pagebar_reportSync(GtkPagebar *pagebar);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__PAGEBAR_H__

