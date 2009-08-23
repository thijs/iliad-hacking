/*
 * This file is part of scribble.
 *
 * scribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * scribble is distributed in the hope that it will be useful,
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
// C implementation: ScribbleUI
//
// Author: jianshen , (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <gdk/gdkkeysyms.h>
#include <liberdm/display.h>
#include <unistd.h>
#include <signal.h>

#include "ScribbleUI.h"
#include "ScribbleCore.h"
#include "ScribbleIPC.h"
#include "ScribbleLog.h"
#include "ScribbleSignalDeal.h"
#include "ScribbleSaveThread.h"

#include "FileMgr_page.h"
#include "displayUpdate.h"
#include "erbusy.h"
#include "ink_draw.h"


static gboolean g_slideshow_enable = FALSE;
static gint     g_slideshow_interval = 10;


static void flushPageBar(PFileManager pFM);
static gboolean on_slideshow_nextpage(gpointer data);


void jump_to_page (PScribbleUICtx pScribbleUICtx) 
{
    SB_LOGPRINTF("__start__...\n");
    erbusy_blink();
    if ( 0!= loadPageAndResultImg(pScribbleUICtx) )
    {
        erbusy_off();
        SB_ERRORPRINTF("Error loading page and result image\n");
    }
    else
    {
        gtk_widget_queue_draw(pScribbleUICtx->drawing_area);
    }
    SB_LOGPRINTF("__finish__...\n");
}

//reset current page and then jump to it
void resetCurrentPage(PScribbleUICtx pScribbleUICtx,
                      const int iNewPageNum)
{
    SB_LOGPRINTF("__start__\n");
    PFileManager pFM=&pScribbleUICtx->fileManager;
    int iOldCurrPage=getCurrPageNo(pFM);
    SB_LOGPRINTF("\nold current=%d,wanto=%d\n",iOldCurrPage,iNewPageNum);
    if( iNewPageNum==iOldCurrPage )
    { 
        SB_LOGPRINTF("no need to chage page,curr==%d\n",iNewPageNum);
        return;
    }
    //when switch, save current page.
    savePageAndResultImg(pScribbleUICtx,FALSE);

    jumptoScribPage(pFM,iNewPageNum);
    int iNewCurrPage=getCurrPageNo(pFM);
    SB_LOGPRINTF("\nnew current=%d\n",iNewCurrPage);
    if( iOldCurrPage==iNewCurrPage )
    { 
        SB_LOGPRINTF("no need to chage page,last curr==%d\n",iNewCurrPage);
        return;
    }

    SB_LOGPRINTF("will jump to %d\n",getCurrPageNo(pFM));
    flushPageBar(pFM);
    //reset toolbar
    toolbar_reinit( getTotalPage(pFM) );
    jump_to_page(pScribbleUICtx);
    SB_LOGPRINTF("__finish__\n");
}

//for adding one page
gboolean addOnepage(PScribbleUICtx pScribbleUICtx)
{
    PFileManager pFM=&pScribbleUICtx->fileManager;
    insertNewPageAfterCurr(pFM);
    SB_LOGPRINTF("after insert[total=%d,curropen=%d]\n",
                 getTotalPage(pFM),getCurrPageNo(pFM));

    //goto the new page,it will flush the pagebar.
    resetCurrentPage(pScribbleUICtx,getCurrPageNo(pFM)+1);
    return TRUE;
}

//for deleting one page
gboolean delOnepage(PScribbleUICtx pScribbleUICtx)
{
    PFileManager pFM=&pScribbleUICtx->fileManager;
    if( 0!=deleteCurrPage(pFM) )
    {
        SB_ERRORPRINTF("del fail\n");
        return FALSE;
    }
    SB_LOGPRINTF("after del[total=%d,curropen=%d]\n",
                 getTotalPage(pFM),getCurrPageNo(pFM));

    flushPageBar(pFM);
    //reset toolbar
    toolbar_reinit( getTotalPage(pFM) );
    //goto the new page.
    jump_to_page(pScribbleUICtx);
    return TRUE;
}

/***
static void printDrawablePosition(PScribbleUICtx pScribbleUICtx)
{
    GtkWidget* w=pScribbleUICtx->drawing_area;
    int x,y;
  
    SB_LOGPRINTF("\n===========For position=====\n\n");
    gdk_window_get_root_origin(w->window,&x,&y);
    SB_LOGPRINTF("root_origin[%d,%d]\n",x,y);
    gdk_window_get_deskrelative_origin(w->window,&x,&y);
    SB_LOGPRINTF("deskrelative_origin[%d,%d]\n",x,y);
    gdk_window_get_origin(w->window,&x,&y);
    SB_LOGPRINTF("origin[%d,%d]\n",x,y);
    SB_LOGPRINTF("it'sparent[%d,%d]\n",w->allocation.x,w->allocation.y);
    SB_LOGPRINTF("\n================\n");
}
***/

// Create a new backing pixmap of the appropriate size 
// and load the first page of current application
gboolean initPixMap(PScribbleUICtx pScribbleUICtx )
{
    SB_LOGPRINTF("____start____\n");    
    if(pScribbleUICtx->pixmap)
        g_object_unref (pScribbleUICtx->pixmap);
    
    GtkWidget* widget=pScribbleUICtx->drawing_area;
    pScribbleUICtx->pixmap = gdk_pixmap_new (widget->window,
                                             widget->allocation.width,
                                             widget->allocation.height,
                                             -1);
    SB_LOGPRINTF("\nwidth:%d,height=%d\n",
           widget->allocation.width,
           widget->allocation.height);
    //printDrawablePosition(pScribbleUICtx);

    //set the drawable object
    ink_setDrawable(pScribbleUICtx->pixmap);
    SB_LOGPRINTF("__finish___\n");
    return TRUE;
}

/* Redraw the screen from the backing pixmap */
static gboolean expose_event( GtkWidget      *widget1,
                              GdkEventExpose *event,
                              gpointer   callback_data )
{
    SB_LOGPRINTF("____start____\n");
    erbusy_blink(); //will be close after draw
    PScribbleUICtx pScribbleUICtx=(PScribbleUICtx)callback_data;
        
    flushPixMap(pScribbleUICtx);
    display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL,NULL);
    SB_LOGPRINTF("__finish___\n");
    return FALSE;
}

static gboolean button_press_event (GtkWidget *widget, 
                                    GdkEventButton *event,
                                    gpointer data)
{
    //SB_LOGPRINTF("__start__\n");
    erbusy_blink();
    PScribbleUICtx pScribbleUICtx=(PScribbleUICtx)data;
    if ((event->button == 1) && (pScribbleUICtx->pixmap != NULL)) 
    {
        //SB_LOGPRINTF("===press point[%d,%d,%d]===\n",
               //(int)event->x ,(int)event->y,pScribbleUICtx->pen_down);
        pScribbleUICtx->pen_down = TRUE;
        
        if( toolbar_is_drawing()  &&  !g_slideshow_enable )
        {
            core_add_point(pScribbleUICtx,event->x, event->y);
        }
        else if ( toolbar_is_erasing() )
        {
            //save the point as the previous point
            pScribbleUICtx->lastPoint.x=event->x;
            pScribbleUICtx->lastPoint.y=event->y;
            //To add do a hitTest?
        }
        else if( toolbar_is_waitconfirmdel() )
        {
            toolbar_confirm_delpage();
        }
    }
    //SB_LOGPRINTF("__end__\n");
    return TRUE;
}

/*
void testOutPutLine(PScribbleUICtx pScribbleUICtx,int x,int y,int x1,int y1)
{
    pScribbleUICtx->pen_down=TRUE;
    setPenSize(1);
    core_add_point(pScribbleUICtx,x, y);
    pScribbleUICtx->pen_down=FALSE;
    core_add_point(pScribbleUICtx,x1, y1);
}

void testOutPut(PScribbleUICtx pScribbleUICtx)
{
    testOutPutLine(pScribbleUICtx,0,1,0,1);
    testOutPutLine(pScribbleUICtx,0,0,0,200);
    testOutPutLine(pScribbleUICtx,0,0,200,0);
    testOutPutLine(pScribbleUICtx,0,0,200,200);
    
    testOutPutLine(pScribbleUICtx,767,0,167,0);
    testOutPutLine(pScribbleUICtx,767,0,567,0);
    testOutPutLine(pScribbleUICtx,767,0,567,200);
    testOutPutLine(pScribbleUICtx,767,0,767,567);
}
*/

void eraseStrokes(PScribbleUICtx pScribbleUICtx,GdkPoint *pcurPoint)
{
    PtrInk pdelink=pScribbleUICtx->scribbleCoreCtx.pDelInk;
    if( pdelink==NULL) SB_WARNPRINTF("NULL before erase\n");
    core_eraseStrokes(pScribbleUICtx,pcurPoint);
}

static gboolean button_release_event (GtkWidget *widget, 
                                      GdkEventButton *event,
                                      gpointer data)
{
    //SB_LOGPRINTF("__start__\n");
    PScribbleUICtx pScribbleUICtx=(PScribbleUICtx)data;
    //SB_LOGPRINTF("===release[%d,%d,%d]===\n",
           //(int)event->x ,(int)event->y,pScribbleUICtx->pen_down);
    pScribbleUICtx->pen_down = FALSE;
    
    if( toolbar_is_drawing()  &&  !g_slideshow_enable )
    {
        core_add_point(pScribbleUICtx,event->x, event->y);
    }
    else if ( toolbar_is_erasing() )
    {
        GdkPoint curPoint;
        curPoint.x=event->x;
        curPoint.y=event->y;
        eraseStrokes(pScribbleUICtx,&curPoint);
        //also flush screen object, when stroke is finished.
        core_redrawBgAndStrks(pScribbleUICtx);
    }
    erbusy_off();
    //SB_LOGPRINTF("__end__\n");
    return TRUE;
}

static gboolean motion_notify_event (GtkWidget *widget, 
                                     GdkEventMotion *event,
                                     gpointer data)
{
    //SB_LOGPRINTF("__start__\n");
    PScribbleUICtx pScribbleUICtx=(PScribbleUICtx)data;
    if (  !  (event->is_hint) 
              && (pScribbleUICtx->pen_down)
              && (event->x < widget->allocation.width) 
              && (event->y < widget->allocation.height)  )
    {
        if( toolbar_is_drawing()  &&  !g_slideshow_enable )
        {
            core_add_point(pScribbleUICtx,event->x, event->y);
        }
        else if ( toolbar_is_erasing() )
        {
            GdkPoint curPoint;
            curPoint.x=event->x;
            curPoint.y=event->y;
            eraseStrokes(pScribbleUICtx,&curPoint);
            pScribbleUICtx->lastPoint.x=event->x;
            pScribbleUICtx->lastPoint.y=event->y;
        }
    }
  //SB_LOGPRINTF("__end__\n");
    return TRUE;
}

//key press event dealer. 
//currently,only (long)pageup/pagedown and exit take effect
guint on_mainWindow_keypress(GtkWidget * widget, 
                             GdkEventKey * event, 
                             gpointer data)
{
    SB_LOGPRINTF("on_mainWindow_keypress - event 0x%x\n", event->keyval);
    
    PScribbleUICtx pScribbleUICtx=(PScribbleUICtx)data;
    int iCurrPage=getCurrPageNo(&pScribbleUICtx->fileManager);
    switch (event->keyval)
    {
        case GDK_Page_Down:
            SB_LOGPRINTF("jumping to the next page\n");
            iCurrPage++;     
            break;
            case GDK_F1:
            SB_LOGPRINTF("PAGETURN_FORWARD LONG\n");
            iCurrPage+=5;
            break;
        case GDK_Page_Up:
            iCurrPage--;
            break;
        case GDK_F2:
            SB_LOGPRINTF("PAGETURN_BACK LONG\n");
            iCurrPage-=5;
            break;
        case GDK_F5:
        case GDK_Home:
            SB_LOGPRINTF("PAGETURN_HOME,For quitting...\n");
            quit(pScribbleUICtx);//it has exited all threads.     
            return 1;
        
        default:
            // Unhandled key pressed
            erbusy_off();
            return 1;
    } // switch
    //correct function.,
    resetCurrentPage(pScribbleUICtx,iCurrPage);
    erbusy_off();
    return 1;// return TRUE => stop event handling 
}


// slideshow: advance one page
static gboolean on_slideshow_nextpage(gpointer data)
{
    PScribbleUICtx pScribbleUICtx=(PScribbleUICtx)data;
    int iCurrPage = getCurrPageNo(&pScribbleUICtx->fileManager);

    SB_LOGPRINTF("current page [%d]", iCurrPage);

    if (iCurrPage < getTotalPage(&pScribbleUICtx->fileManager))
    {
        iCurrPage++;
    }
    else
    {
        iCurrPage = 1;
    }

    SB_LOGPRINTF("new page [%d]", iCurrPage);
    resetCurrentPage(pScribbleUICtx,iCurrPage);

    return TRUE;  // call me again, please
}


/**
void destroyScribbleUICtx(PScribbleUICtx pScribbleUICtx)
{
    if(pScribbleUICtx)
    {
        destroyScribbleCoreCtx(&pScribbleUICtx->scribbleCoreCtx);
        FreeFileManager(&pScribbleUICtx->fileManager);
    }
}
**/

//when scribble quit
void quit (gpointer data)
{
    SB_LOGPRINTF("__start__\n");

    // show busy, stop GTK main loop
    erbusy_blink();
    gtk_main_quit();
}

//flush points periodly
static gboolean flush_points (gpointer data) 
{
    PScribbleUICtx pScribbleUICtx=(PScribbleUICtx)data;
    //under ease mode, we maybe need flush once.
    if( toolbar_is_erasing() )
    {
        if( pScribbleUICtx->scribbleCoreCtx.pDelInk==NULL)
            SB_WARNPRINTF("NULL before flush screen\n");
        
        core_flush_delstroke_onscreen(pScribbleUICtx);
    }
    else
    {
        core_process_points ( pScribbleUICtx );
    }
    return TRUE;
}

static void flushPageBar(PFileManager pFM)
{
    SB_LOGPRINTF("totalpage=%d,currentpage=%d\n",getTotalPage(pFM),getCurrPageNo(pFM));
    pagebar_set_pagecount(getTotalPage(pFM));
    pagebar_goto_page(getCurrPageNo(pFM));
    pagebar_redraw();
}

static void initPageBarAndToolBar(ScribbleUICtx *pScribbleUICtx)
{
    SB_LOGPRINTF("__start___\n");
    erbusy_init();
    erbusy_blink();
    //install and set toolbar
    toolbar_init();
    PFileManager pFM=&pScribbleUICtx->fileManager;
    toolbar_firstinit( getTotalPage(pFM) );
    
      //install and set page bar
    SB_LOGPRINTF("ER_SCRIBBLE_UA_ID=%d,total page=%d,currpage=%d\n",
                 ER_SCRIBBLE_UA_ID,
                 getTotalPage(pFM),
                 getCurrPageNo(pFM));
    pagebar_init();
    flushPageBar(pFM);
    
    erbusy_off();
    SB_LOGPRINTF("__finish___\n");
}

void printarg(const int argc,char* argv[])
{
    printf("\n======paralist========\n");
    int i;
    for(i=0;i<argc;i++)
    {
        printf("argv[%d]=%s\n", i,argv[i]);
    }
    printf("\n======================\n");
}

int parse_args (int argc, 
                char* argv[],
                ScribbleUICtx *pScribbleUICtx) 
{
    int i, j;

    //parse command-line options
    for (i = 1 ; i < argc ; )
    {
        if (strcmp(argv[i], "--slideshow") == 0)
        {
            g_slideshow_enable = TRUE;
            for (j = i ; j < argc - 1 ; j++)
            {
                argv[j] = argv[j + 1];
            }
            argc = argc - 1;
        }
        else if (   i < argc - 1
                 && strcmp(argv[i], "--interval") == 0 )
        {
            j = atoi(argv[i + 1]);
            if (j >= 2)
            {
                g_slideshow_interval = j;
            }
            for (j = i ; j < argc - 2 ; j++)
            {
                argv[j] = argv[j + 2];
            }
            argc = argc - 2;
        }
        else
        {
            i++;
        }
    }
    
    //printarg(argc,argv);
    PFileManager pFM=&pScribbleUICtx->fileManager;
    
    if(   (3!=argc ) 
       || ( isStrEmpty(argv[1]) && isStrEmpty(argv[2])) )
    {
        SB_ERRORPRINTF("Usage:%s [manifest] |[tmpimagename]\n", argv[0]);
        return -1;
    } 
    
    if( isStrEmpty(argv[1]) )
    {//currently, use template.
        return initFileManagerByTmpl(pFM,argv[2]);    
    }
    //else start with manifest
    return initFileManagerByManifest(pFM,argv[1]);
}

gboolean initScribbleUICtx(ScribbleUICtx *pScribbleUICtx)
{
    //set all pointer to null.
    memset(pScribbleUICtx,0,sizeof(ScribbleUICtx));
    pScribbleUICtx->pen_down= FALSE;
    pScribbleUICtx->lastPoint.x=-1;
    pScribbleUICtx->lastPoint.y=-1;
    return TRUE;
}

int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *vbox;
  
    ScribbleUICtx scribbleUICtx;
    guint g_timeout_handler_id = 0;
  
    if(!initScribbleUICtx(&scribbleUICtx))
        return -1;

    //parse the parameter and init FileManager.
    if ( 0!=parse_args(argc, argv,&scribbleUICtx))  
        return -1;

    dealTermSignal(&scribbleUICtx);

    g_thread_init(NULL);
    gdk_threads_init();

    scribbleUICtx.fbdev = open("/dev/fb0", O_RDWR);
    if ( -1 == scribbleUICtx.fbdev)
    {
        SB_ERRORPRINTF("Error opening fb. # mknod /dev/fb0 c 29 0\n");
        return -1;
    }
  
    SB_LOGPRINTF("version %d\n", 1);
    gtk_init (&argc, &argv);

    window= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_name (window, "Test Input");
    if (g_slideshow_enable)
    {
        gtk_window_fullscreen(GTK_WINDOW(window));  // Fullscreen overlaps toolbar and pagebar
        g_timeout_add(g_slideshow_interval * 1000, on_slideshow_nextpage, &scribbleUICtx);
    }

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);

    /* Create the drawing area */
    scribbleUICtx.drawing_area = gtk_drawing_area_new ();
    if (g_slideshow_enable)
    {
        gtk_widget_set_size_request (GTK_WIDGET (scribbleUICtx.drawing_area),
                SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    else
    {
        gtk_widget_set_size_request (GTK_WIDGET (scribbleUICtx.drawing_area), 
                SCREEN_WIDTH, SCREEN_HEIGHT - TOOLBAR_HEIGHT - PAGEBAR_HEIGHT);
    }
    gtk_box_pack_start (GTK_BOX (vbox), scribbleUICtx.drawing_area, TRUE, 
                        TRUE, 0);
    gtk_widget_show (scribbleUICtx.drawing_area);

    /* Signals used to handle backing pixmap */
    g_signal_connect (G_OBJECT (scribbleUICtx.drawing_area), 
                      "expose_event", 
                      G_CALLBACK (expose_event), 
                      &scribbleUICtx);

    /* Event signals */
    g_signal_connect (G_OBJECT (scribbleUICtx.drawing_area), 
                      "motion_notify_event",
                      G_CALLBACK (motion_notify_event), 
                      &scribbleUICtx);
    g_signal_connect (G_OBJECT (scribbleUICtx.drawing_area), 
                      "button_press_event",
                      G_CALLBACK (button_press_event), 
                      &scribbleUICtx);
    g_signal_connect (G_OBJECT (scribbleUICtx.drawing_area), 
                      "button_release_event",
                      G_CALLBACK (button_release_event), 
                      &scribbleUICtx);
    
    // catch the key-events in the toplevel window
    // update scribbleUICtx.drawing_area to main widow
    g_signal_connect (G_OBJECT (window), 
                       "key_press_event", 
                       G_CALLBACK(on_mainWindow_keypress), 
                       &scribbleUICtx);

    gtk_widget_set_events (scribbleUICtx.drawing_area, 
                           GDK_EXPOSURE_MASK |
                           GDK_LEAVE_NOTIFY_MASK |
                           GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK |
                           GDK_POINTER_MOTION_MASK );
    
    gtk_widget_set_events (window, GDK_KEY_PRESS_MASK);

    g_timeout_handler_id = 
            g_timeout_add (100, flush_points, &scribbleUICtx);
    if (g_timeout_handler_id)
    {
        gtk_widget_show(window);
        installIpcServer(&scribbleUICtx);        
        //pagebar and toolbar should be initialized after 
        //g_init,gtk_init and even the ipc server installed...
        initPageBarAndToolBar(&scribbleUICtx); 
        initPixMap(&scribbleUICtx);
        //init and start thread for save result page.
        if( 0!=initSaveThread(&scribbleUICtx) )
        {
            SB_ERRORPRINTF("Error when create save thread\n");
            return (-1);
        }
        jump_to_page(&scribbleUICtx);//jump to current page.            
        
        gdk_threads_enter();
        gtk_main ();
        gdk_threads_leave();

        //stop IPC server, allowing a new app started while we are saving
        stopIpcServer();

        //save scribble, terminate save-thread and wait till it is done.
        savePageAndResultImg(&scribbleUICtx,TRUE);
        exitSaveThread();   

        g_source_remove (g_timeout_handler_id);
    }
    close (scribbleUICtx.fbdev);
    SB_TIMEDISPLAY("main quit...\n");
    return (0);
}
