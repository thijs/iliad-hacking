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
*  C Implementation: ScribbleUtils
*
* Description: 
*
*
* Author: Jian Shen, (C) 2006
*
* Copyright: See COPYING file that comes with this distribution
*
*/

#include "ScribbleUtils.h"
#include "ScribbleLog.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


//////////////////////For Color Related/////////////////////
//internal used
//response to:COLOR_WHITE=0,COLOR_LIGHT_GRAY=1,
//COLOR_DARK_GRAY=2,COLOR_BLACK=3
static int COLORVALUE[4]={0xFFFF,0xAAAA,0x5555,0x0000};

int Util_GdkColorFromHWCOLOR(GdkColor *pGdkColor,HW_COLOR iHwColor)
{
    if(iHwColor<0 || iHwColor>3) iHwColor=COLOR_BLACK;
    
    if( NULL==pGdkColor) return -1;
    
    pGdkColor->red=pGdkColor->green=pGdkColor->blue
                                   =COLORVALUE[iHwColor];
    return 0;
}

//when the input color does not match current 
//4 color system, we need use approximate method to get a nearest color
HW_COLOR Util_HWCOLORFromGdkColor(GdkColor *pGdkColor)
{
    HW_COLOR iHwColor=COLOR_BLACK;
    //compute the average value
    int iCurrColor=
            (pGdkColor->red+pGdkColor->green+pGdkColor->blue)/3;
    int i=0;
    int iDiff=INT_MAX;
    int iCurrDiff;
    for(i=0; i<ARRAYSIZE(COLORVALUE); i++)
    {
        iCurrDiff=abs(COLORVALUE[i]-iCurrColor);
        if(iCurrDiff<iDiff)
        {
            iDiff= iCurrDiff;
            iHwColor=i;
        }
    }

    return iHwColor;	
}

//gdk color is max to 65535. change it to max 256 system.
#define GET_256COLOR(gdkcolor) ((unsigned int)gdkcolor/256)

//a lesson, else the lead to >>n*256 -->0!
#define GET_GDKCOLOR(color256) ((color256)*256)

//gdkcolor from htmlcorstring formal as "#000000"
int Util_GdkColorFromHtmlColor(GdkColor *pGdkColor,const char* sHtmlColor)
{       
    if( NULL==pGdkColor || NULL==sHtmlColor)
    {
        return -1;
    }
    if( '#'!=sHtmlColor[0] && strlen(sHtmlColor)!=HTMLCOLOR_LEN )
    {
        return -1;
    }
    
    unsigned int rgbValue;
    sscanf(sHtmlColor+1,"%x",&rgbValue);
    //printf("color=%s,rgbvalue=%08x\n",sHtmlColor,rgbValue);
    
    //first compute the max-255 color,then transform to max-0xffff.      
    pGdkColor->red=   GET_GDKCOLOR( (rgbValue>>16)& 0xff );
    pGdkColor->green= GET_GDKCOLOR( (rgbValue&0xff00)>>8 );
    pGdkColor->blue=  GET_GDKCOLOR( rgbValue & 0x00ff );

    return 0;
}

//gdkcolor to htmlcorstring formal as "#000000"
int Util_HtmlColorFromGdkColor(char* sHtmlColor,int iSize,const GdkColor *pGdkColor)
{
    if( NULL==pGdkColor || NULL==sHtmlColor || iSize<(HTMLCOLOR_LEN+1) )
    {
        return -1;
    }
    
    unsigned int rgbValue= (GET_256COLOR(pGdkColor->red) & 0xff)<<16;
    rgbValue |= (GET_256COLOR(pGdkColor->green) &0xff)<<8;
    rgbValue |= GET_256COLOR(pGdkColor->blue) & 0xff;
    
    snprintf(sHtmlColor,iSize,"#%06x",rgbValue);
    
    return 0;   
}
//////////////////////For File Operation Related/////////////////////
gboolean FileExist(const char* filename)
{
    struct stat buf;

    if(NULL==filename) return FALSE;

    return ( 0==stat(filename, &buf) );
}

//dir should contains "/"
gboolean FileExist2(const char* filename,const char* dir)
{
    if(NULL==filename && NULL==dir) return FALSE;
    
    char sFullPathName[512];
    memset(sFullPathName,0,sizeof(sFullPathName));    
    snprintf(sFullPathName,sizeof(sFullPathName),
             "%s%s",dir,filename);
    
    return FileExist(sFullPathName);
}

//delete file or directory
int FileDel(const char* filename)
{
    return unlink(filename);
}

//move file to new filename,if dest existed,overwrite it.
int FileMove(const char* sDestFile,const char *sSrcFile)
{
    if( NULL==sDestFile || NULL==sSrcFile ) return -1;
    if( FileExist(sDestFile) )
    {
        if( 0!=FileDel(sDestFile) )
        {
            SB_ERRORPRINTF("del [%s] fail\n",sDestFile);
            return -1;//delete bakfile
        }
    }
    if( 0!=rename(sSrcFile,sDestFile) )
    {
        SB_ERRORPRINTF("\nrename [%s]\nto[%s] fail\n",sSrcFile,sDestFile);
        return -1;//delete bakfile
    }
    
    return 0;
}

int CreateDir(const char* sPath)
{
    return  mkdir(sPath, 700);//mode_t,default to root only
}


void GetFileNameAndDir(char* sFileName,int iNameSize,
                        char* sDir,int iDirSize,
                        const char* sFullName)
{
    int iLen=strlen(sFullName);
    int iDestLen=0;
    SB_FMPRINTF("__fullname=%s\n",sFullName);
    for(iDestLen=0;iDestLen<iLen;iDestLen++)
    {
        if(sFullName[iLen-1-iDestLen] == '/')
            break;
    }
    //fetch dir
    memset(sDir,0,iDirSize);
    if(iDestLen==iLen) //no path char 
    {
        strcpy(sDir,"./");//current directory
    }
    else
    {
        strncpy(sDir,sFullName,iLen-iDestLen);//keep last"/"
    }
    
    if(iDestLen>0)
    {
        if(iDestLen>iDirSize-1) iDestLen=iDirSize-1;
        strncpy(sFileName,sFullName+iLen-iDestLen,iDestLen);
    }
    
    sFileName[iDestLen]=0;
    SB_FMPRINTF("\n__dir=%s,filename=%s\n",sDir,sFileName);
}

//get filename from a fullpath file name
char* GetFileNameFromFullName(char *sFileName,int iSize,
                              const char* sFullName)
{
    char sDir[512+1];
    GetFileNameAndDir(sFileName,iSize,sDir,sizeof(sDir),sFullName);
    return sFileName;
}

//get directory name from a fullpath file name
//sDir-- will end with the "/"
char* GetDirFromFullName(char *sDir, int iSize,const char* sFullName)
{
    char sFileName[512+1];
    GetFileNameAndDir(sFileName,sizeof(sFileName),sDir,iSize,sFullName);
    return sDir;
}

////////////////////////For Time related////////////////////////

//get YYMMDDHHMMSS string from current time,for uniquename
int GetStrFromTime(char* sTime,int iSize,time_t iTime)
{
    struct tm *tmp;
    tmp = (struct tm *)localtime(&iTime);
    if ( NULL==tmp )
    {
        SB_ERRORPRINTF("error get localtime");
        return -1;
    }
    memset(sTime,0,sizeof(iSize));
/*
    printf("time is:%02d,%02d,%02d,%02d,%2d,%02d\n",
    tmp->tm_sec,   // Seconds.     [0-60] (1 leap second)
    tmp->tm_min,   // Minutes.     [0-59]
    tmp->tm_hour,   // Hours.       [0-23] 
    tmp->tm_mday,   // Day.         [1-31] 
    tmp->tm_mon,    // Month.       [0-11] 
    tmp->tm_year-100); //year-1900
*/
    if (strftime(sTime, iSize, "%Y-%m-%d__%H-%M-%S", tmp) == 0) 
    {
        SB_ERRORPRINTF("strftime returned 0");
        return -1;
    }
    //SB_LOGPRINTF("time str:%s\n",sTime);
    return 0;
}


///////////////////////For Str Related///////////////////
//whether string is NULL or length==0 or all space
gboolean isStrEmpty(const char* s)
{
    if ( NULL==s ) return TRUE;
    
    int i;
    
    for( i=0; i<strlen(s); i++ )
    {
        if( ' '!=s[i] ) return FALSE;//find a meaningful char.
    }
    return TRUE;
}


////////////////////// for image save/load /////////////////////
void dumpPixbufInfo(GdkPixbuf* pixbuf)
{
    printf("==========pixbuf info===================\n");
    printf("colorsapce=%d,n_channels=%d,has_alpha=%d,per_sample=%d,\n"
            "width=%d,height=%d,rowstride=%d,colormap_size=%d\n",
    gdk_pixbuf_get_colorspace (pixbuf),
    gdk_pixbuf_get_n_channels (pixbuf),
    gdk_pixbuf_get_has_alpha(pixbuf),
    gdk_pixbuf_get_bits_per_sample(pixbuf),
    gdk_pixbuf_get_width(pixbuf),
    gdk_pixbuf_get_height(pixbuf),
    gdk_pixbuf_get_rowstride(pixbuf),
    gdk_colormap_get_system_size () );
    printf("========================================\n");
            
}
   
int util_loadImage(GdkPixbuf** buf, const char* filename) 
{
    GError *error = NULL;

    SB_FMPRINTF("__Loading(%s)...\n",filename);
    (*buf) = gdk_pixbuf_new_from_file(filename, &error);
    if ((*buf)==NULL)
    {        
        SB_ERRORPRINTF("Error loading bitmap(%s)\n", filename);
        if( NULL!= error )
        {
            SB_ERRORPRINTF("Error msg\n(%s)\n",error->message);
        }
        return -1;
    }
    //dumpPixbufInfo(*buf);
    SB_FMPRINTF("__Loaded...\n");
    return 0;
}

/* load image in a specified size */
int util_loadImage_bysize(GdkPixbuf** buf,const char* filename,int width,int height) 
{
    int w, h;
    GdkPixbufFormat* format;
    GError *error = NULL;

    format = gdk_pixbuf_get_file_info(filename, &w, &h);
    if (format)
    {
        SB_FMPRINTF("__Loading(%s)...\n",filename);
        if ((w <= width) && (h <= height))
        {
            (*buf) = gdk_pixbuf_new_from_file_at_size(filename, w, h, &error);
        }
        else
        {
            (*buf) = gdk_pixbuf_new_from_file_at_size(filename, width, height, &error);
        }
        if ((*buf)==NULL)
        {        
            SB_ERRORPRINTF("Error loading bitmap\n(%s)\n", filename);
            if( NULL!= error )
            {
                SB_ERRORPRINTF("Error msg\n(%s)\n",error->message);
            }
            return -1;
        }
        //dumpPixbufInfo(*buf);
        SB_FMPRINTF("__Loaded...\n");
    }
    else
    {
        SB_ERRORPRINTF("unknow image format of %s", filename);
    }
    return 0;
}

char* getFileExtension(IMGTYPE filetype) 
{
    switch(filetype) 
    {
        case TYPE_BMP:
            return "bmp";
        case TYPE_PNG:
            return "png";
        case TYPE_JPG:
            return "jpg";
        default:
            return "";
    }
}

int util_savePixbuf(GdkPixbuf * pixbuf,
                  char* filename,
                  IMGTYPE filetype) 
{
    GError *error = NULL;
    //dumpPixbufInfo(pixbuf);
    SB_TIMEDISPLAY("--start save \n%s\n",filename);
    if (!gdk_pixbuf_save(pixbuf, filename,getFileExtension(filetype), &error, NULL))
    {
        SB_ERRORPRINTF("Error saving image.msg(%s)\n",error->message);
        return -1;
    }
    SB_TIMEDISPLAY("__Saved Done\n");
    return 0;
}

//make sure the invoker is not in the gdk_threads_enter().
int util_saveImage(GdkPixmap * pixmap,
                 char* filename,
                 IMGTYPE filetype) 
{
    GdkPixbuf *pixbuf;

    SB_TIMEDISPLAY("__Saving-->\n%s\n",filename);
    gdk_threads_enter();    
    pixbuf = gdk_pixbuf_get_from_drawable(NULL,(GdkDrawable *)pixmap,
                                          NULL,0,0,0,0,-1,-1);
    gdk_flush();//make sure all x command has sent to X server.
    gdk_threads_leave();
    int iRet=util_savePixbuf(pixbuf,filename,filetype);
    
    if(pixbuf)
    {
        g_object_unref(pixbuf);
    }
    return iRet;
}
