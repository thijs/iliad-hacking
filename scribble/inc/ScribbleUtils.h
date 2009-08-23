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

#ifndef _SCRIBBLEUTILS_H
#define _SCRIBBLEUTILS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <gtk/gtk.h>

//for INT_MIN,INT_MAX
#include <limits.h>

#include <sys/time.h>

//compute elements of array
#define ARRAYSIZE(ARR) (sizeof(ARR)/sizeof(ARR[0]))

////////// For Color process ///////////////////////////////
typedef enum {
  COLOR_WHITE=0,
  COLOR_LIGHT_GRAY=1,
  COLOR_DARK_GRAY=2,//in select clor or pen size
  COLOR_BLACK=3,
}HW_COLOR;//hardware color,not gdb color set.

#define HTMLCOLOR_LEN sizeof("#000000") 

int Util_GdkColorFromHWCOLOR(GdkColor *pGdkColor,HW_COLOR iHwColor);

HW_COLOR Util_HWCOLORFromGdkColor();

//gdkcolor from htmlcorstring formal as "#000000"
int Util_GdkColorFromHtmlColor(GdkColor *pGdkColor,const char* sHtmlColor);

//gdkcolor to htmlcorstring formal as "#000000"
int Util_HtmlColorFromGdkColor(char* sHtmlColor,int iSize,
                               const GdkColor *pGdkColor);

//////////////For File Operation//////////////
gboolean FileExist(const char* filename);

//dir should contains "/"
gboolean FileExist2(const char* filename,const char* dir);

//delete file or directory
int FileDel(const char* filename);

//move file to new filename,if dest existed,overwrite it.
int FileMove(const char* sDestFile,const char *sSrcFile);
        
int CreateDir(const char* sPath);

void GetFileNameAndDir(char* sFileName,int iNameSize,
                       char* sDir,int iDirSize,
                       const char* sFullName);

//get filename from a fullpath file name
char* GetFileNameFromFullName(char *sFileName,int iSize,
                              const char* sFullName);

//get directory name from a fullpath file name
//sDir-- will end with the "/"
char* GetDirFromFullName(char *sDir, int iSize,const char* sFullName);


////////////////////////For Time related////////////////////////
//get YYMMDDHHMMSS string from current time,for uniquename
int GetStrFromTime(char* sTime,int iSize,time_t iTime);


///////////////////////For Str Related///////////////////
//whether string is NULL or length==0 or all space
gboolean isStrEmpty(const char* s);

////////////////////// for image save/load /////////////////////

typedef enum
{
      TYPE_BMP=1,
      TYPE_PNG,
      TYPE_JPG,
      TYPE_INVALID,
  } IMGTYPE;
  
int util_loadImage(GdkPixbuf** buf, const char* filename);

/* load image in a specified size */
int util_loadImage_bysize(GdkPixbuf** buf,const char* filename,int width,int height);

int util_savePixbuf(GdkPixbuf * pixbuf,
                           char* filename,
                           IMGTYPE filetype);
       
int util_saveImage(GdkPixmap *pixmap,
                          char* filename,
                          IMGTYPE filetype);
#ifdef __cplusplus
}
#endif

#endif
