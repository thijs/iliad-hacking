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

/*
*  C Implementation: ScribbleCore
*
* Description: 
*
*
* Author: Jian Shen, (C) 2006
*
* Copyright: See COPYING file that comes with this distribution
*
*/

#include <strings.h>

#include "FileManager.h"
#include "FileMgr_page.h"
#include "ink.h"
#include "ink_draw.h"
#include "ink_intersect.h"

#include "ScribbleCore.h"
#include "ScribbleLog.h"
#include "ScribbleIPC.h"
#include "erbusy.h"

#include "ScribbleUtils.h"
#include "ScribbleSaveThread.h"


//generate a new  current stroke 
void genNewCurrentStroke(PScribbleCoreCtx pScribbleCoreCtx)
{
    pScribbleCoreCtx->pCurrStroke=construct_stroke();
}

void initScribbleCoreCtx(PScribbleCoreCtx pScribbleCoreCtx)
{
    pScribbleCoreCtx->pointsBuf.count=0;
    pScribbleCoreCtx->pink=NULL;
    pScribbleCoreCtx->pDelInk=construct_ink();
    genNewCurrentStroke(pScribbleCoreCtx);
    pScribbleCoreCtx->bNeedSave=FALSE;
}

//destroy core context
void destroyScribbleCoreCtx(PScribbleCoreCtx pScribbleCoreCtx)
{
    if(pScribbleCoreCtx->pDelInk)
    {
        destroy_ink(pScribbleCoreCtx->pDelInk);
        pScribbleCoreCtx->pDelInk=NULL;
    }
    if(pScribbleCoreCtx->pCurrStroke)
    {
        destroy_stroke(pScribbleCoreCtx->pCurrStroke);
        pScribbleCoreCtx->pCurrStroke=NULL;
    }
}

void core_process_points (PScribbleUICtx pScribbleUICtx)
{
    //SB_LOGPRINTF("__start__\n");
    PointsBuf *pPointsBuf=&pScribbleUICtx->scribbleCoreCtx.pointsBuf;

    if (pPointsBuf->count > 0)
    {
        //SB_LOGPRINTF("SCRIBLE: Sending %d points\n", pPointsBuf->count);
/*
        int i;
        printf("=====================\n");
        for(i=0;i<pPointsBuf->count;i++)
        {	
        printf("[%d,%d,%d,%d,%d]\n",
        pPointsBuf->points [i].x ,
        pPointsBuf->points [i].y,
        pPointsBuf->points [i].size,
        pPointsBuf->points [i].color,
        pPointsBuf->points [i].pen_down);
    }
        printf("=====================\n");
*/
        ioctl (pScribbleUICtx->fbdev,FBIO_DRAWPIXELS,pPointsBuf);
        pPointsBuf->count = 0;
    }
  //SB_LOGPRINTF("__end__\n");
}

//flush deleted stroke on screen
void core_flush_delstroke_onscreen(PScribbleUICtx pScribbleUICtx)
{

    PScribbleCoreCtx pScribbleCoreCtx=&pScribbleUICtx->scribbleCoreCtx;
    PtrInk pDelInk=pScribbleCoreCtx->pDelInk;
    if( NULL==pDelInk || 0==pDelInk->nStrokes) return;

    SB_LOGPRINTF("deleted strokes=%d\n",pDelInk->nStrokes);
    pScribbleCoreCtx->bNeedSave=TRUE;
    PtrStroke pCurStroke=pDelInk->firstStroke; 
    while(pCurStroke)
    {
        int iPenDown=TRUE;
        PtrInkPoint pCurPoint=pCurStroke->firstPoint;
        while(pCurPoint)
        {
            if( NULL==pCurPoint->nextPoint)
            {//last stroke
                iPenDown=False;
            }
            int iCurrPointNum=core_addpoint2buf(
                    &pScribbleUICtx->scribbleCoreCtx.pointsBuf, 
                    pCurPoint->x,
                    pCurPoint->y,
                    pCurStroke->iPenSize,
                    COLOR_WHITE,iPenDown);
            
            if( PIXELBUFSIZE==iCurrPointNum )
            {
                core_process_points(pScribbleUICtx);//flush screen
            }
            pCurPoint=pCurPoint->nextPoint;
        }
        pCurStroke=pCurStroke->nextStroke;
    }
    core_process_points ( pScribbleUICtx );//last time flush
     //empty the deleted ink
    destroy_ink(pScribbleCoreCtx->pDelInk);
    //reconstruct the deleted ink file.
    pScribbleCoreCtx->pDelInk=construct_ink();
}

//do nothing but add a point to pixel buffer,
//return current points number
int core_addpoint2buf(PointsBuf *ppointsBuf,int x,int y,
                      int iPenSize,unsigned char iHwColor,
                      unsigned char iPenDown)
{
    unsigned int count=ppointsBuf->count;
    if ( count < PIXELBUFSIZE)
    {
        ppointsBuf->points [count].x        = x;//temp measure
        ppointsBuf->points [count].y        = y;////temp measure
        ppointsBuf->points [count].size     = iPenSize;
        ppointsBuf->points [count].color    = iHwColor;
        ppointsBuf->points [count].pen_down = iPenDown;
        ppointsBuf->count++;
    }
    return ppointsBuf->count;
}
////according to the pendown status, also draw add_stroke and 
////draw it to pixmap. 
void core_add_point (PScribbleUICtx pScribbleUICtx,int x, int y)
{
    PScribbleCoreCtx pScribbleCoreCtx=&pScribbleUICtx->scribbleCoreCtx;
    PointsBuf *ppointsBuf=&pScribbleCoreCtx->pointsBuf;
    //add point to pixelbuffer
    core_addpoint2buf(ppointsBuf,x,y,
                      getPenSize(),getPenColor(),pScribbleUICtx->pen_down);

    //add points to current stroke
    PtrInkPoint pPoint=construct_point ();
    pPoint->x=x;
    pPoint->y=y;
    PtrStroke pStroke=pScribbleCoreCtx->pCurrStroke;
    ink_add_point(pStroke, pPoint);
    if(!pScribbleUICtx->pen_down) //finish a stroke 
    {
        /*
        SB_LOGPRINTF("__add stroke to ink__\nrange=[(%d,%d),(%d,%d)]\n",
                     pStroke->min_x, pStroke->min_y,
                     pStroke->max_x, pStroke->max_y );
        */

        //set the pensize for stroke.
        pStroke->iPenSize=getPenSize();
        //set pen color
        Util_GdkColorFromHWCOLOR(&pStroke->gdkColor,getPenColor());
        ink_add_stroke(pScribbleCoreCtx->pink,pStroke);
        //also, draw this new stroke in pixmap 
        draw_stroke(pStroke);
        //sometimes, such as power status update, it invoke displayMgrClient
        //which maybe lead to image lost.
        flushPixMap(pScribbleUICtx);
        genNewCurrentStroke(pScribbleCoreCtx);
        //SB_LOGPRINTF("__end add stroke__\n");
    }
    pScribbleCoreCtx->bNeedSave=TRUE;//need save
}

void core_eraseStrokes(PScribbleUICtx pScribbleUICtx,GdkPoint *pCurPoint)
{
    PScribbleCoreCtx pScribbleCoreCtx=&pScribbleUICtx->scribbleCoreCtx;
    delStrokesByLine(pScribbleCoreCtx->pink,
                     &pScribbleUICtx->lastPoint,
                     pCurPoint,
                     pScribbleCoreCtx->pDelInk);
    
}

//not support yet.
void core_eraseInterSectLines(PScribbleUICtx pScribbleUICtx,GdkPoint *pCurPoint)
{
    PScribbleCoreCtx pScribbleCoreCtx=&pScribbleUICtx->scribbleCoreCtx;
    delInterSectLines(pScribbleCoreCtx->pink,
                      &pScribbleUICtx->lastPoint,
                      pCurPoint);
}

//load ink,construct it if ink file does not exist
int core_loadInk(PScribbleUICtx pScribbleUICtx)
{
    PScribbleCoreCtx pScribbleCoreCtx=&pScribbleUICtx->scribbleCoreCtx;
    destroyScribbleCoreCtx(pScribbleCoreCtx);
    initScribbleCoreCtx(pScribbleCoreCtx);
    //load the ink for current page.
    SB_TIMEDISPLAY("load ink file\n");
    pScribbleCoreCtx->pink=fm_loadCurrInk(&pScribbleUICtx->fileManager);
    if( NULL == pScribbleCoreCtx->pink)
    {
        SB_ERRORPRINTF("load ink error\n");
        return -1;
    }
    SB_TIMEDISPLAY("load ink succsessful(strokes=%d)\n",
                   pScribbleCoreCtx->pink->nStrokes); 
    return 0;
}

void drawInitBgPixMap2CurrPixMap(PScribbleUICtx pScribbleUICtx)
{
   //draw the backup pixmap to pix map
    GtkWidget* widget=pScribbleUICtx->drawing_area;
   
    gdk_draw_drawable(pScribbleUICtx->pixmap,
                      widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
                      pScribbleUICtx->initBgPixMap,
                      0,0,0,0,-1,-1);
}


//load specified page and resultimage
int loadPageAndResultImg(PScribbleUICtx pScribbleUICtx)
{
    SB_LOGPRINTF("__start__\n");
    //load/construct ink firstly
    if( 0!=core_loadInk(pScribbleUICtx) ) return -1;

    //need draw strokes?eg:result img lost), always true currently.
    gboolean bNeedDrawInk=TRUE;
    PFileManager pFM=&pScribbleUICtx->fileManager;
  
    const char* sCurrImgFile=getCurrImgFileName(pFM); 
    if(    NULL==pScribbleUICtx->sLastTmplImg
        || 0!=strcasecmp(sCurrImgFile,pScribbleUICtx->sLastTmplImg) )
    {
        GdkPixbuf *pbuf=NULL;
        SB_TIMEDISPLAY("load image..start\n");
        
        char sAbsPath[MAX_FILENAME];
        sCurrImgFile=getAbsPathFromCurrScrib
                (sAbsPath,sizeof(sAbsPath),sCurrImgFile,pFM);
        
        //we creat the background pixmap,and then keep it as initial
        GtkWidget* widget=pScribbleUICtx->drawing_area;
        int width=widget->allocation.width;
        int height=widget->allocation.height;
        //need load the new image as background.
        // notes: pbuf is always smaller than (width,height) 
        if( 0!= util_loadImage_bysize(&pbuf,sCurrImgFile,width,height))
        {
            SB_ERRORPRINTF("load image\n(%s) error\n",sCurrImgFile);
            if(pbuf) g_object_unref(pbuf);
            return -1;
        }
        pScribbleUICtx->sLastTmplImg=sCurrImgFile;       
        
        if(pScribbleUICtx->initBgPixMap)
        {
            g_object_unref(pScribbleUICtx->initBgPixMap);
        }   
        pScribbleUICtx->initBgPixMap=gdk_pixmap_new(widget->window,width,height,-1);
        // clear the background to be white
        gdk_draw_rectangle (pScribbleUICtx->initBgPixMap, widget->style->white_gc, TRUE,
                           0, 0, width, height);
        // center the pixbuf, (width >= w) && (height >= h)
        int xPos, yPos, w, h;
        w = gdk_pixbuf_get_width(pbuf);
        h = gdk_pixbuf_get_height(pbuf);
        xPos = (width - w) / 2;
        yPos = (height - h) / 2;
        //draw the picture to back,use the background widget width,
        //previously, we use -1 as the original size of pixbuf.
        gdk_draw_pixbuf(pScribbleUICtx->initBgPixMap, NULL, pbuf, 
                        0, 0, xPos, yPos,-1,-1, 
                        GDK_RGB_DITHER_NONE, 0, 0);
                
        if(pbuf) g_object_unref(pbuf);//no need any more.
        
        SB_TIMEDISPLAY("load image success\n(%s)\n",sCurrImgFile);
    }  
    drawInitBgPixMap2CurrPixMap(pScribbleUICtx);
    SB_TIMEDISPLAY("after draw bgPixMap\n");
    if(bNeedDrawInk)
    {
        draw_ink(pScribbleUICtx->scribbleCoreCtx.pink);
    }
    SB_TIMEDISPLAY("after draw ink\n");
    pScribbleUICtx->scribbleCoreCtx.bNeedSave=FALSE;
    SB_LOGPRINTF("__end_\n");
    return 0;
}

//save current ink of page,result image will saved when quitting.
//bAppQuit--if scribble is to quit.
int savePageAndResultImg(PScribbleUICtx pScribbleUICtx,gboolean bAppQuit)
{  
    PScribbleCoreCtx pScribbleCoreCtx=&pScribbleUICtx->scribbleCoreCtx;
    if(!pScribbleCoreCtx->bNeedSave && !bAppQuit)
    {
        SB_LOGPRINTF("__saved or no modification!__\n");
        return 0;
    }
    
    SB_TIMEDISPLAY("__saving__\n");
    erbusy_blink();
    
    PFileManager pFM=&pScribbleUICtx->fileManager;
    gboolean bNeedSaveBgPic=FALSE;
    if( pScribbleCoreCtx->bNeedSave )
    { 
         //delete empty ink file,anyway, we need not create dir.
        if(    NULL==pScribbleCoreCtx->pink
               || 0==pScribbleCoreCtx->pink->nStrokes)
        {
            //delete the strokes in .irx
            fm_deleteOnePage(pFM,getCurrScribPage(pFM) );
            //delete related result files
            deleteRelatedFiles(pFM,getCurrScribPage(pFM) );
        }
        else
        {
            //if directory hirachy do not created,creat it.
            if( 0!=createDirHirachy(pFM) )
            {
                SB_ERRORPRINTF("error when create dir hirachy\n");
                erbusy_off();
                return -1;
            }
            SB_TIMEDISPLAY("create dir successful\n");
            bNeedSaveBgPic=TRUE;
        }
    }
    
    //ahead the save action,let contenlister had time to refresh correctly.
    if(bAppQuit || bNeedSaveBgPic)
    {
        //save the filemanager settings and manifest    
        fm_saveFileManager(pFM);
    }
    if (bNeedSaveBgPic)
    { 
        //notify the save thread to save
        notifySave(pScribbleUICtx);
        setCurrentPageDirty(pFM,TRUE);
        fm_saveCurrPage(pFM);//save curr page to irx
    }
    //save ink files
    if (bAppQuit || bNeedSaveBgPic)
    {
        fm_saveIrexInk(pFM, bAppQuit);
    }

    pScribbleCoreCtx->bNeedSave=FALSE;
    SB_TIMEDISPLAY("__saved done__\n");

    return 0;
}

//flush the pixmap to gdk_buffer
void flushPixMap(PScribbleUICtx pScribbleUICtx)
{
    GtkWidget* w=pScribbleUICtx->drawing_area;
    gdk_draw_drawable(w->window,
                      w->style->fg_gc[GTK_WIDGET_STATE(w)],
                      pScribbleUICtx->pixmap,
                      0,0,0,0,-1,-1);
}

//redraw current strokes and background to pixmap.
int core_redrawBgAndStrks(PScribbleUICtx pScribbleUICtx)
{
    //init backup to current pixmap,current will be overwrite.
    SB_TIMEDISPLAY("start");
    drawInitBgPixMap2CurrPixMap(pScribbleUICtx);    
    draw_ink(pScribbleUICtx->scribbleCoreCtx.pink);
    //flush it to screen immediatly.
    flushPixMap(pScribbleUICtx);
    SB_TIMEDISPLAY("end");
    return 0;
}

