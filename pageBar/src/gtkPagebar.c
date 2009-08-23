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

//
// C++ Implementation: gtkPageBar
//
// Description: 
//
//
// Author: ann <ann@localhost>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include <liberdm/display.h> 
#include <liberdm/erdm.h>
#include <liberipc/eripcpagebar.h>
#include <liberipc/eripcviewer.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>
#include <liberipc/eripcsetup.h>
#include <liberipc/eripcconnect.h>
#include <liberipc/eripcarinc.h>
#include "gtkPagebar.h"
#include "displayUpdate.h"

// DEBUG DEFINES
#ifdef DEBUG_PRINTS
#define PRINT_TRACE(X)  printf X
#define PRINT_DEBUG(X)  printf X
#define PRINT_INFO(X)   printf X
#else
#define PRINT_TRACE(X)  ;
#define PRINT_DEBUG(X)  ;
#define PRINT_INFO(X)   ;
#endif

#define PRINT_ERROR(X)  printf X

//#define DEBUG_FLOW_LEVEL 1
#ifdef DEBUG_FLOW_LEVEL
#define ENTER(x) {if (x<DEBUG_FLOW_LEVEL) printf("%s: Entered\n", __FUNCTION__);}
#define LEAVE(x) {if (x<DEBUG_FLOW_LEVEL) printf("%s: Left \n", __FUNCTION__);}
#else
#define ENTER(x) ;
#define LEAVE(x) ;
#endif

#define RETURN(x) {ret=x; goto end;}

#define PAGE_BAR_PADDING	2
#define PAGEBAR_BUTTON_PADDING 2

#define PAGEBAR_LISTBLOCKSIZE 128

#define PAGEBAR_ORIENTATION_X   0   // zeng: hor, provide vertical support
#define PAGEBAR_ORIENTATION_Y   1   // zeng: ver, provide vertical support 
#define SCROLLBOX_LEFT          0   // scroll box
#define SCROLLBOX_RIGHT         1
#define SCROLLBOX_NONE          -1
#define TIMER_SPAN              1500 // one second does not work. use too many cpu resource

#define TRACE()
/*\
{\
    fprintf(stderr, "%s %d %s\n", __FILE__, __LINE__, __func__);\
}
*/

enum
{
    PAGE_SELECTED,  // signals emitted when the user selects a page, using the page bar
    LAST_SIGNAL,
};

static gint  pagebar_signals[LAST_SIGNAL] = { 0 };

static gboolean on_pagebar_exposed(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);
static gboolean on_pagebar_configured(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data);
static gboolean on_pagebar_button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data);
static gboolean on_pagebar_button_release_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data);

// zeng. used to draw page bar
static void pagebar_calculate(GtkPagebar *pagebar);
static void pagebar_draw_cpi(GtkPagebar *pagebar);
static void pagebar_draw_left(GtkPagebar *pagebar);
static void pagebar_draw_right(GtkPagebar *pagebar);
static int  pagebar_scrollbox_hitTest(GtkPagebar *pagebar, int, int);
static gboolean pagebar_scrollbox_needLeft(GtkPagebar *pagebar);
static gboolean pagebar_scrollbox_needRight(GtkPagebar *pagebar);
static int framebuffer_device;

static void gtk_pagebar_class_init (GtkPagebarClass *klass);
static void gtk_pagebar_init (GtkPagebar *pagebar);

static erClientChannel_t apabiChannel;
static erClientChannel_t pdfChannel;
static erClientChannel_t xhtmlChannel;
static erClientChannel_t contentListerChannel;
static erClientChannel_t scribbleChannel;
static erClientChannel_t setupChannel;
static erClientChannel_t connectChannel;
static erClientChannel_t ebaChannel;
static erClientChannel_t arincViewerChannel;
static erClientChannel_t arincMenuChannel;
static erClientChannel_t mobipocketChannel;

static int loadBitmap(GdkPixbuf** buf, char* filename) {
    GError *error = NULL;
    int ret=0;

    ENTER(1);

    (*buf) = gdk_pixbuf_new_from_file(filename, &error);
    if ((*buf)==NULL) {
        PRINT_ERROR(("Error loading bitmap %s (%s)\n", filename, error->message));
        RETURN(-1);
    }

end:
    LEAVE(1);
    return ret;
}

static int addToList(int** list, int* count, int value) {
    int i;
    int ret=0;

    ENTER(1);

    if (value<0)
        RETURN(-1);

    if (!(*list)) {
        (*list)=(int*)malloc(PAGEBAR_LISTBLOCKSIZE*sizeof(int));
        for (i=0; i<PAGEBAR_LISTBLOCKSIZE; i++)
            (*list)[i]=-1;
        *count = PAGEBAR_LISTBLOCKSIZE;
    }

    i=0;
    while (i<*count && (*list)[i]!=-1 && (*list)[i]!=value)
        i++;

    /* If the value is already present, we exit */
    if (i<*count && (*list)[i]==value)
        RETURN(0);


    /* If we ran out of space, we realloc the list */
    if (i==*count) {
        (*list)=(int*)realloc(*list, (*count+PAGEBAR_LISTBLOCKSIZE)*sizeof(int));
        for (i=0; i<PAGEBAR_LISTBLOCKSIZE; i++)
            (*list)[i+*count]=-1;
        *count += PAGEBAR_LISTBLOCKSIZE;
    }

    (*list)[i]=value;

end:
    LEAVE(1);
    return ret;
}

static int removeFromList(int** list, int* count, int value) {
    int i;

    ENTER(1);

    i=0;
    while (i<*count && (*list)[i]!=value && (*list)[i]!=-1)
        i++;
    if (i<*count && (*list)[i]==value) {
        memmove(*list, (*list)+1, *count-i-1);
        (*list)[*count-1]=-1;
        *count--;
    }

    LEAVE(1);
    return 0;
}

int addBookmark(GtkPagebar* pagebar, int value) {
    return addToList(&pagebar->bookmarks, &pagebar->bookmarkCount, value);
}

int addNote(GtkPagebar* pagebar, int value) {
    return addToList(&pagebar->notes, &pagebar->noteCount, value);
}

int removeBookmark(GtkPagebar* pagebar, int value) {
    return removeFromList(&pagebar->bookmarks, &pagebar->bookmarkCount, value);
}

int removeNote(GtkPagebar* pagebar, int value) {
    return removeFromList(&pagebar->notes, &pagebar->noteCount, value);
}

int pagebar_reset_values(GtkPagebar* pagebar) {
    ENTER(0);

    pagebar->pagecount = PAGEBAR_DEFAULT_PAGECOUNT;
    pagebar->currentPage = PAGEBAR_DEFAULT_CURRENTPAGE;
    pagebar->currentPageOffset = PAGEBAR_DEFAULT_CURRENTPAGEOFFSET;
    pagebar->zoomMode = PAGEBAR_DEFAULT_ZOOMMODE;
    pagebar->orientation = PAGEBAR_DEFAULT_ORIENTATION;
    pagebar->drawAreaXPos = PAGEBAR_DEFAULT_DRAWAREAXPOS;
    pagebar->drawAreaYPos = PAGEBAR_DEFAULT_DRAWAREAYPOS;
    pagebar->drawAreaWidth = PAGEBAR_DEFAULT_DRAWAREAWIDTH;
    pagebar->drawAreaHeight = PAGEBAR_DEFAULT_DRAWAREAHEIGHT;
    pagebar->barXPos = PAGEBAR_DEFAULT_BARXPOS;
    pagebar->barYPos = PAGEBAR_DEFAULT_BARYPOS;
    pagebar->barWidth = PAGEBAR_DEFAULT_BARWIDTH;
    pagebar->barHeight = PAGEBAR_DEFAULT_BARHEIGHT;
    pagebar->cpiHeight = PAGEBAR_DEFAULT_CPIHEIGHT;
    pagebar->cpiWidth = PAGEBAR_DEFAULT_CPIWIDTH;
    pagebar->barFontType = PAGEBAR_DEFAULT_BARFONTTYPE;
    pagebar->barFontHeight = PAGEBAR_DEFAULT_BARFONTHEIGHT;
    pagebar->cpiFontType = PAGEBAR_DEFAULT_CPIFONTTYPE;
    pagebar->cpiFontHeight = PAGEBAR_DEFAULT_CPIFONTHEIGHT;
    pagebar->pageFieldWidth = PAGEBAR_DEFAULT_PAGEFIELDWIDTH;
    pagebar->pageFieldHeight = PAGEBAR_DEFAULT_PAGEFIELDHEIGHT;
    pagebar->pageOffsetWidth = PAGEBAR_DEFAULT_PAGEOFFSETWIDTH;
    pagebar->pageOffsetHeight = PAGEBAR_DEFAULT_PAGEOFFSETHEIGHT;
    pagebar->pageOffsetXOffset = PAGEBAR_DEFAULT_PAGEOFFSETXOFFSET;
    pagebar->pageOffsetYOffset = PAGEBAR_DEFAULT_PAGEOFFSETYOFFSET;
    pagebar->bookmarkCount = PAGEBAR_DEFAULT_BOOKMARKCOUNT;
    pagebar->bookmarkMaxValue = PAGEBAR_DEFAULT_BOOKMARKMAXVALUE;
    pagebar->noteCount = PAGEBAR_DEFAULT_NOTECOUNT;
    pagebar->notesMaxValue = PAGEBAR_DEFAULT_NOTESMAXVALUE;
    pagebar->scrollButtonWidth = PAGEBAR_DEFAULT_SCROLLBUTTONWIDTH;
    pagebar->show = PAGEBAR_DEFAULT_SHOW;
    if (PAGEBAR_TIMER_PROGRESS  != pagebar->currentTimer)
        pagebar->currentTimer = PAGEBAR_TIMER_NONE;

    pagebar->lastPage = -1;
    pagebar->lastDrawAreaXPos = pagebar->lastDrawAreaYPos = -1;

    if (pagebar->bookmarks)
        free(pagebar->bookmarks);
    if (pagebar->notes)
        free(pagebar->notes);

    pagebar->bookmarks = NULL;
    pagebar->notes = NULL;

    LEAVE(0);
    return 0;
}

int pagebar_load_bitmaps(GtkPagebar* pagebar)
{
    int ret = 0;
#ifdef USE_BITMAPS
    /* Load bitmaps */
    if (loadBitmap(&(pagebar->bitmaps[bitmap_CPI]), DATA_DIR "/pagebar_cpi.png"))
        RETURN(-1);
    pagebar->cpiWidth = gdk_pixbuf_get_width(pagebar->bitmaps[bitmap_CPI]);
    pagebar->cpiHeight = gdk_pixbuf_get_height(pagebar->bitmaps[bitmap_CPI]);
    /* MvdW: removed collapse/expand bitmaps
    if (loadBitmap(&(pagebar->bitmaps[bitmap_CPIExpand]), DATA_DIR "/pagebar_cpiexpand.png"))
    RETURN(-1);
    if (loadBitmap(&(pagebar->bitmaps[bitmap_CPICollapse]), DATA_DIR "/pagebar_cpicollapse.png"))
    RETURN(-1); */
    if (loadBitmap(&(pagebar->bitmaps[bitmap_PageField]), DATA_DIR "/pagebar_pagefield.png"))
        RETURN(-1);
    pagebar->pageFieldWidth = gdk_pixbuf_get_width(pagebar->bitmaps[bitmap_PageField]);
    pagebar->pageFieldHeight = gdk_pixbuf_get_height(pagebar->bitmaps[bitmap_PageField]);
    if (loadBitmap(&(pagebar->bitmaps[bitmap_ScrollLeft]), DATA_DIR "/pagebar_scrollleft.png"))
        RETURN(-1);
    pagebar->scrollButtonWidth = gdk_pixbuf_get_width(pagebar->bitmaps[bitmap_ScrollLeft]);
    if (loadBitmap(&(pagebar->bitmaps[bitmap_ScrollRight]), DATA_DIR "/pagebar_scrollright.png"))
        RETURN(-1);
    if (loadBitmap(&(pagebar->bitmaps[bitmap_Bookmark]), DATA_DIR "/pagebar_bookmark.png"))
        RETURN(-1);
    if (loadBitmap(&(pagebar->bitmaps[bitmap_Note]), DATA_DIR "/pagebar_note.png"))
        RETURN(-1);
#endif

end:
    return ret;
}

/* Create a new backing pixmap of the appropriate size */
static gboolean on_pagebar_configured(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data)
{
    GtkPagebar* pagebar = GTK_PAGEBAR(user_data);

    ENTER(0);

    if (pagebar->pixmap)
        g_object_unref (pagebar->pixmap);

    pagebar->pixmap = gdk_pixmap_new (widget->window,
        widget->allocation.width,
        widget->allocation.height,
        -1);
    gdk_draw_rectangle (pagebar->pixmap,
        widget->style->white_gc,
        TRUE,
        0, 0,
        widget->allocation.width,
        widget->allocation.height);

    gtk_pagebar_draw(pagebar);
    LEAVE(0);
    return TRUE;
}

/* Redraw the screen from the backing pixmap */
static gboolean on_pagebar_exposed(GtkWidget *widget, GdkEventExpose *event, gpointer user_data)
{

    GtkPagebar* pagebar = GTK_PAGEBAR(user_data);

    ENTER(0);

    /* Copy image from pixmap to output */
    gdk_draw_drawable (widget->window,	
        widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
        pagebar->pixmap,
        event->area.x, event->area.y,
        event->area.x, event->area.y,
        event->area.width, event->area.height);
    
    LEAVE(0);
    return FALSE;
}

static gboolean	on_pagebar_button_release_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    ENTER(1);

    PRINT_TRACE(("button_release_event -- location %dx%d - %dx%d \n",(int)event->x,(int)event->y,(int)event->x_root,(int)event->y_root));

    LEAVE(1);

    return TRUE;	
}

static int currentPageChanged(GtkPagebar* pagebar) {
    erClientChannel_t channel;
    int ret = 0;

    ENTER(1);

    if (pagebar->lastPage==pagebar->currentPage)
        RETURN (0);

    pagebar->lastPage=pagebar->currentPage;
    gtk_pagebar_draw(pagebar);

    switch (pagebar->currentApp) {
        case ER_APABI_VIEWER_UA_ID:
            channel = apabiChannel;
            break;
        case ER_PDF_VIEWER_UA_ID:
            channel = pdfChannel;
            break;
        case ER_XHTML_VIEWER_UA_ID:
            channel = xhtmlChannel;
            break;
        case ER_SCRIBBLE_UA_ID:
            channel = scribbleChannel;
            break;
        case ER_CONTENT_LISTER_UA_ID:
            channel = contentListerChannel;
            clGotoPage(channel, pagebar->currentPage);
            RETURN (0);
        case ER_SETUP_UA_ID:
            channel = setupChannel;
            stGotoPage(channel, pagebar->currentPage);
            RETURN (0);
        case ER_CONNECT_UA_ID:
            channel = connectChannel;
            cnGotoPage(channel, pagebar->currentPage);
            RETURN (0);
        case ER_EBA_VIEWER_UA_ID:
            channel = ebaChannel;
            break;
        case ER_ARINC_VIEWER_UA_ID:
            channel = arincViewerChannel;
            arincJumpToPage(channel, pagebar->currentApp, pagebar->currentPage);
            RETURN (0);
        case ER_ARINC_MENU_UA_ID:
            channel = arincMenuChannel;
            arincJumpToPage(channel, pagebar->currentApp, pagebar->currentPage);
            RETURN (0);
        case ER_MOBIPOCKET_VIEWER_UA_ID:
            channel = mobipocketChannel;
            break;
        default:
            return -1;
    }    
    vwrJumpToPage(channel, pagebar->currentApp, pagebar->currentPage);

end:
    LEAVE(1);
    return ret;
}

/***
static void on_pageentry_activate(GtkWidget *widget, gpointer user_data) {
    GtkPagebar* pagebar = GTK_PAGEBAR(user_data);

    ENTER(0);
    int page = atoi(gtk_entry_get_text(GTK_ENTRY(widget)));
    if (page<1)
        page=1;
    else if (page>pagebar->pagecount)
        page=pagebar->pagecount;

    pagebar->currentPage=page;
    currentPageChanged(pagebar);

    LEAVE(0);
}
***/

/***
static gboolean on_gtk_idle_callback(gpointer data) {
    //    XImage *xi;

    ENTER(1);

    gdk_threads_enter();
    //    xi = XGetImage(GDK_DISPLAY(), GDK_ROOT_WINDOW(), 0, 0, 1, 1, AllPlanes, XYPixmap);
    gdk_flush();
    //    XDestroyImage(xi);
    gdk_threads_leave();
    dmDisplay(dmCmdPriorUrgent, dmQBW);

    LEAVE(1);

    return FALSE;
}
***/

/***
static gboolean on_text_changed(GtkWidget * widget, int level, gpointer data) {

    ENTER(1);

    gtk_idle_add(on_gtk_idle_callback, (gpointer) 0);

    LEAVE(1);

    return TRUE;
}
***/


static gboolean	on_pagebar_button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    GtkPagebar* pagebar = GTK_PAGEBAR(user_data);
    int x=event->x;
    int y=event->y;
    int i;
    int j;


    ENTER(0);

    PRINT_TRACE(("button_press_event -- location %dx%d - %dx%d \n",(int)event->x,(int)event->y,(int)event->x_root,(int)event->y_root));

    /* Check if we clicked on the CPI */
    if (x>=pagebar->barXPos+(pagebar->barWidth-pagebar->cpiWidth)/2 && x<=pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2 &&
        y>=pagebar->barYPos+(pagebar->barHeight-pagebar->cpiHeight)/2 && y<=pagebar->barYPos+(pagebar->barHeight+pagebar->cpiHeight)/2)
    {
        /* Did we press the expand/collapse button?  -- disabled */
        /***
        if (y<pagebar->barYPos+(pagebar->barHeight-pagebar->cpiHeight)/2+10) {
        pagebar->zoomMode=!pagebar->zoomMode;
        gtk_pagebar_draw(pagebar);
        dmDisplay(dmCmdPriorUrgent, dmQFull);
        }
        else {
        ***/

        /***
        gtk_widget_show(pagebar->pageEntry);
        gtk_widget_grab_focus(pagebar->pageEntry);
        gtk_entry_set_text(GTK_ENTRY(pagebar->pageEntry), "");
        gtk_entry_set_alignment(GTK_ENTRY(pagebar->pageEntry), 0.5);
        gtk_entry_set_activates_default(GTK_ENTRY(pagebar->pageEntry), TRUE);
        g_signal_connect(GTK_OBJECT(pagebar->pageEntry), "activate", G_CALLBACK(on_pageentry_activate),pagebar);
        gtk_signal_connect(GTK_OBJECT(pagebar->pageEntry), "changed", GTK_SIGNAL_FUNC(on_text_changed),pagebar);
        gtk_signal_connect(GTK_OBJECT(pagebar->drawingArea), "expose-event", GTK_SIGNAL_FUNC(on_pagebar_exposed),pagebar);
        ***/

        // zeng. update . according to new design
        pagebar->zoomMode=!pagebar->zoomMode;
        gtk_pagebar_draw(pagebar);
       

        // change to DM_QUALITY_FULL if not acceptable
        display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL, (void*) DM_QUALITY_TYPING);
    }
    else 
    {
        if (pagebar->zoomMode==0) {
            int smallBarWidth;
            smallBarWidth = (pagebar->barWidth-pagebar->cpiWidth)/2-pagebar->pageFieldWidth;
            /* Check if we clicked on the left part of the bar */
            if (x<pagebar->barXPos+(pagebar->barWidth-pagebar->cpiWidth)/2 && pagebar->currentPage!=1) {
                i=pagebar->barXPos+(pagebar->barWidth-pagebar->cpiWidth)/2-x;
                i=i*(pagebar->pagecount-2)/(smallBarWidth);
                if (i<pagebar->currentPage-1) {
                    pagebar->currentPage-=(i+1);
                }
                else {
                    pagebar->currentPage=1;
                }          
                currentPageChanged(pagebar);
            }
            /* We clicked on the right part of the bar */ 
            else if (x>pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2 && pagebar->currentPage!=pagebar->pagecount) {
                i=x-(pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2);
                i=i*(pagebar->pagecount-2)/(smallBarWidth);
                if (i<pagebar->pagecount-pagebar->currentPage) {
                    pagebar->currentPage+=(i+1);
                }
                else {
                    pagebar->currentPage=pagebar->pagecount;
                }          
                currentPageChanged(pagebar);
            } 
        }
        else if (pagebar->zoomMode==1) {

            /* Calculate the number of pages we can show on each side of the CPI */
            j = ((pagebar->barWidth-pagebar->cpiWidth)/2-pagebar->scrollButtonWidth)/pagebar->pageFieldWidth;
            /* Check if we clicked on the left part of the bar */
            if (x<pagebar->barXPos+(pagebar->barWidth-pagebar->cpiWidth)/2 &&
                x>pagebar->barXPos+(pagebar->barWidth-pagebar->cpiWidth)/2-j*pagebar->pageFieldWidth &&
                pagebar->currentPage!=1)
            {
                i=pagebar->barXPos+(pagebar->barWidth-pagebar->cpiWidth)/2-x;
                i/=pagebar->pageFieldWidth;
                if (i<pagebar->currentPage-1 && i<j) 
                {
                    pagebar->currentPage-=(i+1);
                    currentPageChanged(pagebar);
                }
            }
            /* Check if we clicked on the right part of the bar */ 
            else if (x>pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2 && 
                x<pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2+j*pagebar->pageFieldWidth && 
                pagebar->currentPage!=pagebar->pagecount) 
            {
                i=x-(pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2);
                i/=pagebar->pageFieldWidth;
                if (i<pagebar->pagecount-pagebar->currentPage && i<j) 
                {
                    pagebar->currentPage+=(i+1);
                    currentPageChanged(pagebar);
                } 
            } 
            /* Check if we clicked on the scroll left button */    
            /* replace j with pagebar->maxItems */       
            else if (SCROLLBOX_LEFT == pagebar_scrollbox_hitTest(pagebar, x, y))
            {
                // MvdW: changed this to be non-confusing ;)
                // the left & right were swapped. 
                pagebar->currentPage-=2*pagebar->maxItems+1;
                if (pagebar->currentPage<1)
                    pagebar->currentPage=1;

                currentPageChanged(pagebar);

            }
            else if (SCROLLBOX_RIGHT == pagebar_scrollbox_hitTest(pagebar, x, y))
            {
                pagebar->currentPage+=2*pagebar->maxItems+1;
                if (pagebar->currentPage>pagebar->pagecount)
                    pagebar->currentPage=pagebar->pagecount;

                currentPageChanged(pagebar);
            }
        }
    }

    LEAVE(0);

    return TRUE;
}


GType gtk_pagebar_get_type (void)
{
    static GType pagebar_type = 0;

    if (!pagebar_type)
    {
        static const GTypeInfo pagebar_info =
        {
            sizeof (GtkPagebarClass),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc) gtk_pagebar_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof (GtkPagebar),
            0,    /* n_preallocs */
            (GInstanceInitFunc) gtk_pagebar_init,
        };

        pagebar_type = g_type_register_static (GTK_TYPE_EVENT_BOX, "Pagebar", &pagebar_info, 0);  
    }

    return pagebar_type;
}

static void gtk_pagebar_class_init (GtkPagebarClass *klass)
{
    GtkObjectClass *object_class;

    ENTER(0);

    object_class = (GtkObjectClass *) klass;

    // event to notify page update 

    pagebar_signals[PAGE_SELECTED] =
        g_signal_new ("pagebar_page_selected",
        G_OBJECT_CLASS_TYPE (object_class),
        G_SIGNAL_RUN_LAST,
        G_STRUCT_OFFSET (GtkPagebarClass, pagebar_page_selected),
        NULL, NULL,
        gtk_marshal_VOID__INT,
        G_TYPE_NONE,
        1,
        G_TYPE_INT);
    LEAVE(0);
}

static void gtk_pagebar_init(GtkPagebar *pagebar)
{
    ENTER(0);

    /* create drawingarea */
    pagebar->drawingArea = gtk_drawing_area_new();

    gtk_widget_set_events (GTK_WIDGET(pagebar->drawingArea), GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK |  GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK );

    gtk_container_add(GTK_CONTAINER(pagebar),pagebar->drawingArea);

    gtk_signal_connect(GTK_OBJECT(pagebar->drawingArea), "expose-event", GTK_SIGNAL_FUNC(on_pagebar_exposed),pagebar);
    gtk_signal_connect(GTK_OBJECT(pagebar->drawingArea), "configure-event", GTK_SIGNAL_FUNC(on_pagebar_configured),pagebar);

    g_signal_connect (G_OBJECT (pagebar->drawingArea), "button_press_event",  G_CALLBACK (on_pagebar_button_press_event), pagebar);

    g_signal_connect (G_OBJECT (pagebar->drawingArea), "button_release_event",  G_CALLBACK (on_pagebar_button_release_event), pagebar);

    erIpcStartClient(ER_APABI_VIEWER_CHANNEL,      &apabiChannel        );
    erIpcStartClient(ER_PDF_VIEWER_CHANNEL,        &pdfChannel          );
    erIpcStartClient(ER_XHTML_VIEWER_CHANNEL,      &xhtmlChannel        );
    erIpcStartClient(ER_CONTENTLISTER_CHANNEL,     &contentListerChannel);
    erIpcStartClient(ER_SCRIBBLE_CHANNEL,          &scribbleChannel     );
    erIpcStartClient(ER_SETUP_CHANNEL,             &setupChannel        );
    erIpcStartClient(ER_CONNECT_CHANNEL,           &connectChannel      );
    erIpcStartClient(ER_EBA_VIEWER_CHANNEL,        &ebaChannel          );
    erIpcStartClient(ER_ARINC_VIEWER_CHANNEL,      &arincViewerChannel  );
    erIpcStartClient(ER_ARINC_MENU_CHANNEL,        &arincMenuChannel    );
    erIpcStartClient(ER_MOBIPOCKET_VIEWER_CHANNEL, &mobipocketChannel   );

    gtk_widget_show(pagebar->drawingArea);

    LEAVE(0);
}

GtkWidget* gtk_pagebar_new()
{
    GtkPagebar* pagebar;
    PangoFontDescription* fontdesc;

    ENTER(0);

    pagebar = (GtkPagebar*) g_object_new(GTK_PAGEBAR_TYPE,NULL);

    memset(pagebar->bitmaps, 0, sizeof(pagebar->bitmaps));
    pagebar->bookmarks = NULL;
    pagebar->notes = NULL;

    // init data fields
    pagebar_reset_values(pagebar);
    pagebar_load_bitmaps(pagebar);

    framebuffer_device = open("/dev/fb0", O_RDWR);    

    PRINT_DEBUG(("bar format %dx%d\n",pagebar->drawAreaWidth,pagebar->drawAreaHeight));

    gtk_drawing_area_size(GTK_DRAWING_AREA(pagebar->drawingArea),pagebar->drawAreaWidth,pagebar->drawAreaHeight);

    pagebar->pangolayout=gtk_widget_create_pango_layout(GTK_WIDGET(pagebar->drawingArea),"X");
    fontdesc = pango_font_description_from_string("Bitstream Vera 6.5");
//    pango_font_description_set_size(fontdesc, PAGEBAR_DEFAULT_CPIFONTHEIGHT);
    pango_layout_set_font_description(pagebar->pangolayout, fontdesc);
    pango_font_description_free(fontdesc);
    pango_layout_get_pixel_size(pagebar->pangolayout,&pagebar->barFontWidth,&pagebar->barFontHeight);

    pagebar->cpiFontWidth = pagebar->barFontWidth;

    PRINT_DEBUG(("text format %dx%d\n",pagebar->barFontWidth,pagebar->barFontHeight));

    //gtk_widget_show(pagebar->drawingArea);

    LEAVE(0);

    return GTK_WIDGET(pagebar);
}


static int drawMarkers(GtkPagebar* pagebar, int width, int y, int bitmap, int* markers, int count, int maxValue) {
    int i;
    int j;
    int x;

    ENTER(1);

    for (i=0; i<count && markers[i]!=-1; i++) {
        /* Convert bookmark to pagenumber */
        j=markers[i]*pagebar->pagecount/maxValue;
        if (j>pagebar->currentPage) {
            x = pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2;
            /* & 0xFFFFFFFC: Align to markers */
            if (j>=pagebar->pagecount) 
                x += (((pagebar->pagecount-pagebar->currentPage-1)*width/(pagebar->pagecount-2))&0xFFFFFFFC)+pagebar->pageFieldWidth/2;
            else
                x += (((pagebar->pagecount-pagebar->currentPage-1)*width/(pagebar->pagecount-2))*(j-pagebar->currentPage-1)/(pagebar->pagecount-1-pagebar->currentPage-1))&0xFFFFFFFC;
            /* Bitmaps aren't transparant so we can't draw over the CPI. In that case we move the marker a little bit to the right */
            if (x-(pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2)<2)
                x+=3;

#ifdef USE_BITMAPS
            gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap], 0,0,x-2,y,-1,-1,GDK_RGB_DITHER_NONE,0,0);
#else
            gdk_draw_line(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, x, y, x, y+6);
#endif
        }
        else if (j<pagebar->currentPage) {
            x = pagebar->barXPos+(pagebar->barWidth-pagebar->cpiWidth)/2-2;
            if (j==1)
                x -= ((width*(pagebar->currentPage-2)/(pagebar->pagecount-2))&0xFFFFFFFC) + pagebar->pageFieldWidth/2;
            else
                x -= (width*(pagebar->currentPage-2)/(pagebar->pagecount-2)*(pagebar->currentPage-j-1)/(pagebar->currentPage-3))&0xFFFFFFFC;

#ifdef USE_BITMAPS
            gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap], 0,0,x-2,y,-1,-1,GDK_RGB_DITHER_NONE,0,0);
#else
            gdk_draw_line(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, x, y, x, y+6);
#endif
        }
    }

    LEAVE(1);

    return 0;
}

//
// draws a page bar with the specified pagecount  
// steps:
// 1. adjust position and size
// 2. reset background
// 3. draw cpi (current page indicator)
// 4. draw left and right 
// 5. draw bookmarks and notes
//
int gtk_pagebar_draw(GtkPagebar* pagebar)
{
    int ret = 0;

    ENTER(1);
    

    if (!IS_GTK_PAGEBAR(pagebar)) {
        RETURN(-1);
    }
    
  
    if (!pagebar->pixmap) 
        RETURN(-1);

    /* If the user moved the window, we first have to do that */
    if (pagebar->lastDrawAreaXPos!=pagebar->drawAreaXPos || pagebar->lastDrawAreaYPos!=pagebar->drawAreaYPos) {
        pagebar->lastDrawAreaXPos=pagebar->drawAreaXPos;
        pagebar->lastDrawAreaYPos=pagebar->drawAreaYPos;
        gtk_window_move(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(pagebar))), pagebar->drawAreaXPos, pagebar->drawAreaYPos);
    }

    /* If the user resized the window, we first have to do that */
    if (pagebar->lastDrawAreaWidth!=pagebar->drawAreaWidth || pagebar->lastDrawAreaHeight!=pagebar->drawAreaHeight) {
        pagebar->lastDrawAreaWidth=pagebar->drawAreaWidth;
        pagebar->lastDrawAreaHeight=pagebar->drawAreaHeight;
        gtk_widget_set_size_request(GTK_WIDGET(gtk_widget_get_toplevel(GTK_WIDGET(pagebar))), pagebar->drawAreaWidth, pagebar->drawAreaHeight);
    }

    // clear the area
    gdk_draw_rectangle (pagebar->pixmap,
        GTK_WIDGET(pagebar->drawingArea)->style->white_gc, 
        TRUE, 
        pagebar->barXPos, 
        pagebar->barYPos, 
        pagebar->barWidth, 
        pagebar->barHeight);

    if (!pagebar->show)
        RETURN(-1);

    // calculate
    pagebar_calculate(pagebar);

    // draw cpi 
    pagebar_draw_cpi(pagebar);

    if (pagebar->pagecount==1)
        RETURN(0);

    // draw the others
    pagebar_draw_left(pagebar);
    pagebar_draw_right(pagebar);
end:
    // make things appear on the screen 
    gtk_widget_queue_draw(GTK_WIDGET(pagebar->drawingArea));
    PRINT_TRACE(("gtk_widget_queue_draw_area called \n"));

    LEAVE(1);

    return ret;	
}

//
// #define SCROLLBOX_LEFT  0
// #define SCROLLBOX_RIGHT 1
// #define SCROLLBOX_NONE  -1
//
gboolean pagebar_scrollbox_needLeft(GtkPagebar *pagebar)
{
    if (pagebar->currentPage > pagebar->maxItems + 1) 
        return TRUE;
    return FALSE;
}

gboolean pagebar_scrollbox_needRight(GtkPagebar *pagebar)
{
    if (pagebar->pagecount - pagebar->currentPage > pagebar->maxItems) 
        return TRUE;
    return FALSE;
}

int  pagebar_scrollbox_hitTest(GtkPagebar *pagebar, int x, int y)
{
    int width = pagebar->maxItems *pagebar->pageFieldWidth;

    // position and scroll box enabled
    if (x < pagebar->cpiXPos - width &&
        x > pagebar->cpiXPos - width - pagebar->scrollButtonWidth &&
        pagebar_scrollbox_needLeft(pagebar))
    {
        return SCROLLBOX_LEFT;
    }

    if (x > pagebar->cpiXPos + pagebar->cpiWidth + width &&
        x < pagebar->cpiXPos + pagebar->cpiWidth + width + pagebar->scrollButtonWidth && 
        pagebar_scrollbox_needRight(pagebar))
    {
        return SCROLLBOX_RIGHT;
    }

    return SCROLLBOX_NONE;
}


// 
// calculate the pagebar parameters including cpi, maxItems, etc.
// according to the orientation
//
void pagebar_calculate(GtkPagebar *pagebar)
{
    if (PAGEBAR_ORIENTATION_X == pagebar->orientation)
    {
        pagebar->cpiXPos  = pagebar->barXPos + (pagebar->barWidth-pagebar->cpiWidth)/2;
        pagebar->cpiYPos  = pagebar->barYPos - (pagebar->cpiHeight -pagebar->barHeight)/2;
        pagebar->maxItems = ((pagebar->barWidth-pagebar->cpiWidth)/2-pagebar->scrollButtonWidth)/pagebar->pageFieldWidth; 
    }
    else if (PAGEBAR_ORIENTATION_Y == pagebar->orientation)
    {
        // make sure bar height is larger than cpi height
        pagebar->cpiXPos  = (pagebar->barWidth-pagebar->cpiWidth)/2;
        pagebar->cpiYPos  = pagebar->barYPos + (pagebar->barHeight - pagebar->cpiHeight)/2;        
        pagebar->maxItems = ((pagebar->barHeight - pagebar->cpiHeight)/2-pagebar->scrollButtonHeight)/pagebar->pageFieldHeight; 
    }
}

//
// draw cpi
//
void pagebar_draw_cpi(GtkPagebar *pagebar)
{
    // If there are less pages than we can show, overview mode is not used 
    // mode can only be changed when user click cpi
    int canZoom  = 1;
    int x = 0, y = 0, w = 0, h = 0;
    char pageStr[16] = {0};
    if (pagebar->pagecount<= pagebar->maxItems *2+1) {
        pagebar->zoomMode = 1;
        canZoom = 0;
    }

    /* Once we have to redraw the pagebar, we don't need the pageEntry anymore */
    gtk_widget_hide(pagebar->pageEntry);

    /* Draw Current Page Indicator, load differnet indicator according orientation*/
#ifdef USE_BITMAPS
    gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap_CPI], 0, 0,
        pagebar->cpiXPos, pagebar->cpiYPos, -1, -1, GDK_RGB_DITHER_NONE, 0, 0);
#else
    gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, FALSE, 
        pagebar->cpiXPos, pagebar->cpiYPos, pagebar->cpiWidth, h);
#endif

    /* Draw pageoffset */
    if (pagebar->currentPageOffset <= PAGEBAR_DEFAULT_CURRENTPAGEOFFSET)
    {
        int i = pagebar->currentPageOffset*pagebar->pageOffsetWidth/PAGEBAR_DEFAULT_CURRENTPAGEOFFSET;
        x = pagebar->cpiXPos;
        y = pagebar->cpiYPos;
        gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, TRUE, 
            x+pagebar->pageOffsetXOffset, 
            y+pagebar->pageOffsetYOffset, 
            i, 
            pagebar->pageOffsetHeight);
        gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->white_gc, TRUE, 
            x+pagebar->pageOffsetXOffset+i, 
            y+pagebar->pageOffsetYOffset, 
            pagebar->pageOffsetWidth-i, 
            pagebar->pageOffsetHeight);
    }

    /* Draw pagenumber */
    sprintf(pageStr, "%d", pagebar->currentPage);
    pango_layout_set_text(pagebar->pangolayout, pageStr, -1);
    pango_layout_get_pixel_size(pagebar->pangolayout,&w,&h);
    x = pagebar->barXPos + (pagebar->barWidth-w)/2;
    y = pagebar->barYPos + (pagebar->barHeight-h)/2;
    gdk_draw_layout(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, x, y, pagebar->pangolayout);
}

//
// draw left part of pagebar
// display all if possible (independant of zoomMode)
// display in detail + scroll box according to zoomMode
//
void pagebar_draw_left(GtkPagebar *pagebar)
{
    int w = 0, x = 0, y = 0;
    int i = 0, j = 0;
    char pageStr[16] = {0};
    int left = 1;

    // init 
    x = pagebar->cpiXPos;
    y = pagebar->cpiYPos + (pagebar->cpiHeight - pagebar->pageFieldHeight)/2;

    // check can we display all. 
    if (pagebar->pagecount > 2 * pagebar->maxItems + 1 && 0 == pagebar->zoomMode)
    {
        // can not display all
        w = (pagebar->barWidth-pagebar->cpiWidth)/2-pagebar->pageFieldWidth;
        y = pagebar->barYPos+(pagebar->barHeight-pagebar->pageFieldHeight)/2;
        x = pagebar->barXPos+(pagebar->barWidth-pagebar->cpiWidth)/2-2;
        if (pagebar->currentPage!=1) 
        {
            for (i=0; i<w*(pagebar->currentPage-2)/(pagebar->pagecount-2)/4; i++) 
            {
                gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, FALSE, x, y, 1, pagebar->pageFieldHeight);
                x-=4;
            }
            x=x-pagebar->pageFieldWidth+1;
#ifdef USE_BITMAPS
            gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap_PageField], 0,0,x,y,-1,-1,GDK_RGB_DITHER_NONE,0,0);
#else
            gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, FALSE, x, y, pagebar->pageFieldWidth, pagebar->pageFieldHeight);
#endif
            sprintf(pageStr, "1");
            pango_layout_set_text(pagebar->pangolayout, pageStr, -1);
            pango_layout_get_pixel_size(pagebar->pangolayout,&i,&j);
            gdk_draw_layout(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, x+(pagebar->pageFieldWidth-i)/2, y+(pagebar->pageFieldHeight-j)/2, pagebar->pangolayout);
        }
        /* Draw bookmarks */
        if (pagebar->bookmarkMaxValue)
            drawMarkers(pagebar, w, y-6, bitmap_Bookmark, pagebar->bookmarks, pagebar->bookmarkCount, pagebar->bookmarkMaxValue);
        if (pagebar->notesMaxValue)
            drawMarkers(pagebar, w, y+pagebar->pageFieldHeight, bitmap_Note, pagebar->notes, pagebar->noteCount, pagebar->notesMaxValue);
    }
    else
    {
        // display in detail
        for(left; left < pagebar->currentPage && left <= pagebar->maxItems; ++left)
        {
            x -= pagebar->pageFieldWidth;  
            gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->white_gc, FALSE, x, y, 1, pagebar->pageFieldHeight);

#ifdef USE_BITMAPS
            gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap_PageField], 0,0,x,y,-1,-1,GDK_RGB_DITHER_NONE,0,0);
#else
            gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, FALSE, x, y, pagebar->pageFieldWidth, pagebar->pageFieldHeight);
#endif

            sprintf(pageStr, "%d", pagebar->currentPage - left );
            pango_layout_set_text(pagebar->pangolayout, pageStr, -1);
            pango_layout_get_pixel_size(pagebar->pangolayout,&i,&j);
            gdk_draw_layout(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, x+(pagebar->pageFieldWidth-i)/2, y+(pagebar->pageFieldHeight-j)/2, pagebar->pangolayout);
        }
        /* need scroll bar or not */
        /* something confused. left one indicates right, right one indicates left. understand? */
        if (pagebar_scrollbox_needLeft(pagebar))
        {
            x = pagebar->cpiXPos - pagebar->maxItems * pagebar->pageFieldWidth;
            x-=pagebar->scrollButtonWidth;
#ifdef USE_BITMAPS
            gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap_ScrollLeft], 0,0,x,y,-1,-1,GDK_RGB_DITHER_NONE,0,0);
#else
            gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, TRUE, x, y, pagebar->scrollButtonWidth, pagebar->pageFieldHeight);
#endif
        }
        else
        {
            // load a gray one. 
        }
        /* draw bookmarkers */
        i = pagebar->maxItems;
        for (j=0; pagebar->bookmarkMaxValue > 0 && j<pagebar->bookmarkCount && pagebar->bookmarks[j]!=-1; j++)
       	{
            int page;
            page=pagebar->bookmarks[j]*pagebar->pagecount/pagebar->bookmarkMaxValue;
            if (page<pagebar->currentPage && page>=pagebar->currentPage-i)
                x=pagebar->barXPos+(pagebar->barWidth-pagebar->cpiWidth)/2-(pagebar->currentPage-page)*pagebar->pageFieldWidth+pagebar->pageFieldWidth/2;
            else if (page>pagebar->currentPage && page<=pagebar->currentPage+i)
                x=pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2+(page-pagebar->currentPage-1)*pagebar->pageFieldWidth+pagebar->pageFieldWidth/2;
            else
                continue;

#ifdef USE_BITMAPS
            gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap_Bookmark], 0,0,x,y-6,-1,-1,GDK_RGB_DITHER_NONE,0,0);
#else
            gdk_draw_line(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, x, y-6, x, y);
#endif        
        }    
        for (j=0; pagebar->notesMaxValue > 0 && j<pagebar->noteCount && pagebar->notes[j]!=-1; j++)
       	{
            int page;
            page=pagebar->notes[j]*pagebar->pagecount/pagebar->notesMaxValue;
            if (page<pagebar->currentPage && page>=pagebar->currentPage-i)
                x=pagebar->barXPos+(pagebar->barWidth-pagebar->cpiWidth)/2-(pagebar->currentPage-page)*pagebar->pageFieldWidth+pagebar->pageFieldWidth/2;
            else if (page>pagebar->currentPage && page<=pagebar->currentPage+i)
                x=pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2+(page-pagebar->currentPage-1)*pagebar->pageFieldWidth+pagebar->pageFieldWidth/2;
            else
                continue;

#ifdef USE_BITMAPS
            gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap_Note], 0,0,x,y+pagebar->pageFieldHeight,-1,-1,GDK_RGB_DITHER_NONE,0,0);
#else
            gdk_draw_line(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, x, y+pagebar->pageFieldHeight, x, y+pagebar->pageFieldHeight+6);
#endif        
        }  
    }
}

void pagebar_draw_right(GtkPagebar *pagebar)
{
    int w = 0, x = 0, y = 0;
    int i = 0, j = 0;
    char pageStr[16] = {0};
    int right = 1;

    // init 
    x = pagebar->cpiXPos + pagebar->cpiWidth;
    y = pagebar->cpiYPos + (pagebar->cpiHeight - pagebar->pageFieldHeight)/2;

    // check can we display all 
    if (pagebar->pagecount > 2 * pagebar->maxItems + 1  && 0 == pagebar->zoomMode)
    {
        // can not display all
        w = (pagebar->barWidth-pagebar->cpiWidth)/2-pagebar->pageFieldWidth;
        x = pagebar->barXPos+(pagebar->barWidth+pagebar->cpiWidth)/2;
        y = pagebar->barYPos+(pagebar->barHeight-pagebar->pageFieldHeight)/2;
        if (pagebar->currentPage!=pagebar->pagecount) 
        {
            for (i=0; i<w*(pagebar->pagecount-pagebar->currentPage-1)/(pagebar->pagecount-2)/4; i++) 
            {
                gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, FALSE, x, y, 1, pagebar->pageFieldHeight);
                x+=4;
            }
#ifdef USE_BITMAPS
            gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap_PageField], 0,0,x,y,-1,-1,GDK_RGB_DITHER_NONE,0,0);
#else
            gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, FALSE, x, y, pagebar->pageFieldWidth, pagebar->pageFieldHeight);
#endif      
            sprintf(pageStr, "%d", pagebar->pagecount);
            pango_layout_set_text(pagebar->pangolayout, pageStr, -1);
            pango_layout_get_pixel_size(pagebar->pangolayout,&i,&j);
            gdk_draw_layout(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, x+(pagebar->pageFieldWidth-i)/2, y+(pagebar->pageFieldHeight-j)/2, pagebar->pangolayout);
        }
    }
    else
    {
        // display in detail
        for(right; right <= pagebar->pagecount - pagebar->currentPage && right <= pagebar->maxItems; ++right)
        {
            gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->white_gc, FALSE, x, y, 1, pagebar->pageFieldHeight);

#ifdef USE_BITMAPS
            gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap_PageField], 0,0,x,y,-1,-1,GDK_RGB_DITHER_NONE,0,0);
#else
            gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, FALSE, x, y, pagebar->pageFieldWidth, pagebar->pageFieldHeight);
#endif

            sprintf(pageStr, "%d", pagebar->currentPage + right );
            pango_layout_set_text(pagebar->pangolayout, pageStr, -1);
            pango_layout_get_pixel_size(pagebar->pangolayout,&i,&j);
            gdk_draw_layout(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, x+(pagebar->pageFieldWidth-i)/2, y+(pagebar->pageFieldHeight-j)/2, pagebar->pangolayout);
            x += pagebar->pageFieldWidth;
        }

        /* draw the right scroll box if needed*/
        if (pagebar_scrollbox_needRight(pagebar))
        {
            x = pagebar->cpiXPos + pagebar->maxItems * pagebar->pageFieldWidth; 
            x += pagebar->cpiWidth;
#ifdef USE_BITMAPS
            gdk_draw_pixbuf(pagebar->pixmap, NULL, pagebar->bitmaps[bitmap_ScrollRight], 0,0,x,y,-1,-1,GDK_RGB_DITHER_NONE,0,0);
#else
            gdk_draw_rectangle(pagebar->pixmap, GTK_WIDGET(pagebar->drawingArea)->style->black_gc, TRUE, x, y, pagebar->scrollButtonWidth, pagebar->pageFieldHeight);
#endif

        }
        else
        {
            // Load a gray one. 
        }
    }
}

// only when idle, we can sure all invalid region has been update
gboolean on_idle(gpointer data)
{
    GtkPagebar *pagebar = (GtkPagebar *)data;
    gdk_threads_enter();
    gdk_flush();
    gdk_threads_leave();
    
    erClientChannel_t channel;
    switch (pagebar->currentApp) 
    {
    case ER_APABI_VIEWER_UA_ID:
        channel = apabiChannel;
        break;
    case ER_PDF_VIEWER_UA_ID:
        channel = pdfChannel;
        break;
    case ER_XHTML_VIEWER_UA_ID:
        channel = xhtmlChannel;
        break;
    case ER_SCRIBBLE_UA_ID:
        channel = scribbleChannel;
        break;
    case ER_CONTENT_LISTER_UA_ID:
        channel = contentListerChannel;
        break;
    case ER_SETUP_UA_ID:
        channel = setupChannel;
        break;
    case ER_CONNECT_UA_ID:
        channel = connectChannel;
        break;
    case ER_EBA_VIEWER_UA_ID:
        channel = ebaChannel;
        break;
    case ER_ARINC_VIEWER_UA_ID:
        channel = arincViewerChannel;
        break;
    case ER_ARINC_MENU_UA_ID:
        channel = arincMenuChannel;
        break;
    default:
        return FALSE;
    }    
    // printf("\nPagebar reports synchronise message to %d\n", pagebar->currentApp);
    pbReportSynchronise(channel, pagebar->currentApp);
    return FALSE;
}

void pagebar_reportSync(GtkPagebar *pagebar)
{
    gtk_idle_add(on_idle, (gpointer) pagebar);
}

