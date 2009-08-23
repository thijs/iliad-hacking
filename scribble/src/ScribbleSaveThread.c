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
*  C Implementation: ScribbleSignalDeal
*
* Description: 
*   a indepent thread to execute the image save.
*   beacause the ink need less time,so we keep it saving in main thread.
*
* Author: Jian Shen, (C) 2006
*
* Copyright: See COPYING file that comes with this distribution
*
*/


#include "ScribbleSaveThread.h"
#include "ScribbleLog.h"
#include "ScribbleUtils.h"

//posix mutex for imagedate(pixbuf) when copy/flush.
GMutex* imgdata_mutex = NULL;
//posix condition
GCond* data_cond = NULL;

static GThread* s_threadSave=NULL;

static gboolean s_bQuit=FALSE;
//need save result page?
static gboolean s_bNeedSaveResultPage=FALSE;

//always the main thread copy current pixmap to it,
//and save thread save it to file.
//static GdkPixmap* s_pixmap=NULL;
GdkPixbuf *s_pixbuf=NULL;
//main function to exe save image.
void saveThreadMain(gpointer data)
{
    while( !s_bQuit )
    {        
        g_mutex_lock(imgdata_mutex);
        while(!s_bNeedSaveResultPage && !s_bQuit)
            g_cond_wait (data_cond, imgdata_mutex);      
        
        if(s_bQuit)
        {
            g_mutex_unlock(imgdata_mutex);
            break;
        }
        SB_TIMEDISPLAY("get save request\n");
        char sResultImage[MAX_FILENAME];
        PScribbleUICtx pScribbleUICtx=(PScribbleUICtx)data;
        PFileManager pFM=&pScribbleUICtx->fileManager;
        get_curr_result_filename(sResultImage,MAX_FILENAME,pFM);
        
        s_bNeedSaveResultPage=FALSE; 
        SB_TIMEDISPLAY("start save\n");          
        //save the result page
        util_savePixbuf(s_pixbuf,sResultImage,TYPE_PNG);
        //free memory.
        g_object_unref (s_pixbuf);
        g_mutex_unlock(imgdata_mutex);
        SB_TIMEDISPLAY("end save\n");
    }    
}

//return 0 if success.
int initSaveThread(ScribbleUICtx *pScribbleUICtx)
{
    //init the mutex and cond
    imgdata_mutex=g_mutex_new();
    if(NULL==imgdata_mutex)
    {
        SB_ERRORPRINTF("imgdata_mutex create error\n");
        return -1;
    }
    data_cond=g_cond_new();
    if(NULL==data_cond)
    {
        SB_ERRORPRINTF("data_cond create error\n");
        return -1;
    }
    //init save thread.
    GError *error=NULL;
    s_threadSave=g_thread_create((GThreadFunc)saveThreadMain,
                                  pScribbleUICtx,TRUE,&error);
    if(NULL==s_threadSave)
    {
        SB_ERRORPRINTF("thread create error\n");
        return -1;
    }
    return 0;
}

//for main thread, notify current page need save,
void notifySave(PScribbleUICtx pScribbleUICtx)
{
    SB_TIMEDISPLAY("start\n");
    
    g_mutex_lock(imgdata_mutex);   
    SB_TIMEDISPLAY("fetch pixbuf\n");
    s_pixbuf = gdk_pixbuf_get_from_drawable(NULL,
                                            (GdkDrawable *)pScribbleUICtx->pixmap,
                                            NULL,0,0,0,0,-1,-1);
    SB_TIMEDISPLAY("end fetch\n");
    s_bNeedSaveResultPage=TRUE;
    g_cond_signal(data_cond);
    g_mutex_unlock(imgdata_mutex);
    SB_TIMEDISPLAY("end\n");
}

//exit thread, for main thread.
void exitSaveThread()
{
    SB_TIMEDISPLAY("save-thread ready to exit\n");
    g_mutex_lock(imgdata_mutex);
    s_bQuit=TRUE;
    g_cond_signal(data_cond);
    g_mutex_unlock(imgdata_mutex);

    g_thread_join(s_threadSave);
    SB_TIMEDISPLAY("save-thread exit\n");
    //free resources.
    //g_mutex_free(imgdata_mutex);
    //g_cond_free(data_cond);
}
    
