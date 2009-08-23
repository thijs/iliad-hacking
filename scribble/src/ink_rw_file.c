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
*  C Implementation: ink_rw_file.c
*
* Description: 
* brief- ink read/write from/to file.
*
* Author: Jian Shen, (C) 2006
* Original Author: Yehong Wang.
*
* Copyright: See COPYING file that comes with this distribution
*
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "ink_rw.h"
#include "ScribbleLog.h"
#include "ScribbleUtils.h"

//0xFFFFFFFF=4294967295 
#define MAX_INT_CHAR_LEN 16

typedef enum{
	WRITEINT_NULL=0,//only write int
	WRITEINT_2SPACES,//end with 2 spaces char
	WRITEINT_LINEEND,//end with line end
} WRITEINT_MODE;


static unsigned char read_ink (FILE * file, PtrInk ink);
static unsigned char write_ink (PtrInk ink, FILE * file);

static long read_stroke (FILE * file, PtrStroke stroke);
static unsigned char write_stroke (FILE * file, PtrStroke stroke);

static unsigned char read_integer (int *integer, FILE * file);
//write a int with specified mode
static unsigned char write_integer (int integer, FILE * file,WRITEINT_MODE iMode);

//write the rgb value in gdkcolor
static unsigned char write_rgb(GdkColor *pGdkColor, FILE * file);
//read the rgb value in gdkcolor
static unsigned char read_rgb(GdkColor *pGdkColor, FILE * file);


// read ink object from inkfile
// return the root object-ink if success, else return NULL.
PtrInk file_read_ink(const char *inkfile)
{
    FILE *file = NULL;
    PtrInk ink = NULL;
    unsigned char ret;
    if (inkfile == NULL || strlen (inkfile) == 0)
    {
        SB_INKERRPRINTF("failed:incorrect arguments\n");
        return NULL;
    }
    file = fopen (inkfile, "rb");
    if (file == NULL)
    {
        SB_INKERRPRINTF("fopen failed \n");
        return NULL;
    }

    ink = construct_ink ();
    if (ink == NULL)
    {
        fclose (file);
        return NULL;
    }
    ret = read_ink (file, ink);
    if (ret == 0)
    {
        destroy_ink (ink);
        fclose (file);
        return NULL;
    }

    SB_INKPRINTF("succeeded,return ink=%p\n", ink);
    fclose (file);
    return ink;
}

// fillout the ink object from file stream
// if successed return 1, else return 0
// pay attention to free the memory alloced in this function 
unsigned char read_ink (FILE * file, PtrInk ink)
{
    long lPos, fileLen;
    PtrStroke stroke = NULL;
    unsigned char integer_str[MAX_INT_CHAR_LEN];

    if (file == NULL || ink == NULL)
    {
        SB_INKERRPRINTF("failed:incorrect arguments\n");
        return 0;
    }

  //to get the file length
    if ( 0!= fseek (file, 0, SEEK_END))
    {
        SB_INKERRPRINTF("fseek failed \n");
        return 0;
    }
    fileLen = ftell (file);
    if ( 0!=fseek (file, 0, SEEK_SET))
    {
        SB_INKPRINTF("fseek failed \n");
        return 0;
    }

    if ( !read_integer (&(ink->nStrokes), file) )
    {
        SB_INKERRPRINTF("failed getting ink->nStrokes\n");
        return 0;
    }
    if (ink->nStrokes == 0)
    {
        SB_INKERRPRINTF("return:ink->nStrokes=0 \n");
        return 0;
    }
    SB_INKPRINTF("nStrokes=%d\n",ink->nStrokes);
    sprintf (integer_str, "%d", ink->nStrokes);
    lPos = sizeof (char) * (strlen (integer_str) + 2);
    while (lPos != -1 && lPos != fileLen)
    {
      //SB_INKPRINTF("lPos=%d\n", lPos);
        stroke = construct_stroke ();
        if (stroke == NULL)
        {
            SB_INKERRPRINTF("return: construct_stroke failed \n");
            return 0;
        }
        lPos = read_stroke (file, stroke);
        ink_add_stroke (ink, stroke);
    }
    return 1;
}

// read one stroke from file stream
// if successed,return the file position after reading one stroke
// else return -1
// pay attention to call this function,
// we should guarantee file position should stop at the beginning of stroke 
long read_stroke (FILE * file, PtrStroke stroke)
{
    int i;
    PtrInkPoint point;

    if (file == NULL || stroke == NULL)
    {
        SB_INKERRPRINTF("failed:incorrect arguments\n");
        return -1;
    }

    if ( !read_integer(&(stroke->iPenSize), file) )
    {
        SB_INKPRINTF("failed getting stroke->iPenSize\n");
        return -1;
    }
  //read rgb
    if ( !read_rgb(&stroke->gdkColor,file) )
    {
        SB_INKERRPRINTF("failed write rgb\n");
        return 0;
    }

    if ( !read_integer(&(stroke->nPoints), file) )
    {
        SB_INKERRPRINTF("failed getting stroke->nPoints\n");
        return -1;
    }

    SB_INKPRINTF("npoints=%d,ipensize=%d\n",
            stroke->nPoints,stroke->iPenSize);

    if ( stroke->nPoints == 0)
    {
        SB_INKPRINTF("returned because of stroke->nPoints=0\n");
        return ftell (file);//lPos
    }

    //stroke->nPoints will be changed in iteration,so we should keep it.
    int nTotalPoints=stroke->nPoints;
    for (i = 0; i < nTotalPoints; i++)
    {
        point = construct_point ();
        if ( NULL==point )
        { 
            SB_INKERRPRINTF("construct point error\n");
            return -1;
        }
        if ( !read_integer (&(point->x), file) )
        {
            SB_INKERRPRINTF("failed getting (point%d->x)\n",i);
            return -1;
        }
        if ( !read_integer (&(point->y), file) )
        {
            SB_INKERRPRINTF("failed getting (point%d->y)\n",i);
            return -1;
        }
	    //SB_INKPRINTF("point(%d,%d) \n", point->x, point->y);
        ink_add_point(stroke, point);
    }
    return ftell (file);//current file pointer position
}


///////////////////functions about save ink object//////////////////////
// write ink object to inkfile
// if successed return 1, else return 0
unsigned char file_write_ink (PtrInk ink, const char *inkfile)
{
    FILE *file = NULL;
    unsigned char ret;

    if (ink == NULL || ink->nStrokes == 0 || inkfile == NULL
        || strlen (inkfile) == 0)
    {
        return 0;
    }
    file = fopen (inkfile, "wb+");
    if (file == NULL)
    {
        SB_INKERRPRINTF("fopen failed \n");
        return 0;
    }
    ret = write_ink (ink, file);
    fclose (file);
    return ret;
}

// write ink object to file stream
unsigned char write_ink (PtrInk ink, FILE * file)
{
    PtrStroke stroke;

    if (ink == NULL || file == NULL )
    {
        SB_INKPRINTF("failed:incorrect arguments\n");
        return 0;
    }
    if (ink->nStrokes == 0)
    {
        SB_INKPRINTF("return:ink->nStrokes=0 \n");
        return 0;
    }
    if ( 0!= fseek (file, 0, SEEK_SET) )
    {
        SB_INKERRPRINTF("fseek failed \n");
        return 0;
    }
    if ( !write_integer (ink->nStrokes, file,WRITEINT_LINEEND) )
    {
        SB_INKERRPRINTF("failed writing ink->nStrokes=%d \n",
                        ink->nStrokes);
        return 0;
    }

    SB_INKPRINTF("ink=%p, ink->nStrokes=%d \n", ink, ink->nStrokes);
    stroke = ink->firstStroke;
    while (stroke)
    {
        if (0 ==write_stroke (file, stroke) )
        {
            SB_INKERRPRINTF("write_stroke failed \n");
            return 0;
        }
        stroke = stroke->nextStroke;
    }
    SB_INKPRINTF("succeeded \n");
    return 1;
}

// write one stroke to file system
unsigned char write_stroke (FILE * file, PtrStroke stroke)
{
    if (file == NULL || stroke == NULL || stroke->nPoints == 0)
    {
        return 0;
    }
  //write pen size
    if ( !write_integer(stroke->iPenSize,file,WRITEINT_LINEEND) )
    {
        SB_INKERRPRINTF("write failed, iPenSize= %d \n",stroke->iPenSize);
        return 0;
    }
  //write rgb
    if ( !write_rgb(&stroke->gdkColor,file) )
    {
        SB_INKERRPRINTF("failed write rgb\n");
        return 0;
    }

    if ( !write_integer(stroke->nPoints,file,WRITEINT_LINEEND) )
    {
        SB_INKERRPRINTF("write failed, nPoints= %d \n",stroke->nPoints);
        return 0;
    }

    SB_INKPRINTF("stroke->nPoints=%d \n", stroke->nPoints);
    PtrInkPoint point = stroke->firstPoint;
    while (point)
    {
        if ( !write_integer(point->x, file, WRITEINT_2SPACES) )
        {
            SB_INKERRPRINTF("fwrite failed, point->x= %d \n", point->x);
            return 0;
        }
        if ( !write_integer(point->y, file, WRITEINT_LINEEND) )
        {
            SB_INKERRPRINTF("fwrite failed, point->y= %d \n", point->y);
            return 0;
        }
        SB_INKPRINTF("point(%d,%d) \n", point->x, point->y);
        point = point->nextPoint;
    }
    return 1;
}

// wangyehong 20060515,updated by jianshen@20060630 
// change storing integer directly into storing the string corresponding to integer.
unsigned char read_integer (int *integer, FILE * file)
{
    unsigned long lRead = 0;
    unsigned char integer_str[MAX_INT_CHAR_LEN];// 0xFFFFFFFF=4294967295 
    int i = 0;
    int bPrev0Dor20=0;//previous char is 0x0D or 0x20?,special

    if (integer == NULL || file == NULL)
        return 0;
  //two legal mode:[0x20 0x20],[0x0D 0x0A]
    while (1)
    {
        if (i >= MAX_INT_CHAR_LEN) return 0;

        lRead = fread (&(integer_str[i]), sizeof (char), 1, file);
        if(!lRead) return 0;

        if (bPrev0Dor20)
        {
            if(  (integer_str[i-1] == 0x20 && integer_str[i] == 0x20)
                  ||(integer_str[i-1] == 0x0D && integer_str[i] == 0x0A) )
            {
                integer_str[i-1] = '\0';
                *integer = atoi (integer_str);
                return 1;
            }
            return 0;
        }
        if(integer_str[i] == 0x0D || integer_str[i] == 0x20)
        {
            bPrev0Dor20=1;
        }
        i++;
    }
    return 0;
}

unsigned char
write_uchars (FILE * file,const unsigned char* ustr,unsigned int iLen)
{
    unsigned long lWrite ;
    if (file == NULL) return 0;

    lWrite = fwrite (ustr, sizeof (unsigned char), iLen, file);

    return (lWrite == iLen ?1:0);
}

unsigned char write_pure_integer (int integer, FILE * file)
{
    unsigned char integer_str[MAX_INT_CHAR_LEN];// 0xFFFFFFFF=4294967295 
    sprintf (integer_str, "%d", integer);
    return write_uchars(file, integer_str, strlen(integer_str) );
}

//write line end
unsigned char write_line (FILE * file)
{
    unsigned char sep[2];
    sep[0] = 0x0D;
    sep[1] = 0x0A;
    return write_uchars(file, sep, sizeof(sep)/sizeof(sep[0]) );
}

//write 2 spaces char
unsigned char write_2spaces (FILE * file)
{
    unsigned char sep[2];
    sep[0] = 0x20;
    sep[1] = 0x20;
    return write_uchars(file, sep, sizeof(sep)/sizeof(sep[0]) );
}

//write a int with specified mode
unsigned char
write_integer(int integer, FILE * file,const WRITEINT_MODE iMode)
{
    unsigned char bRet= write_pure_integer(integer,file);

    if( WRITEINT_NULL==iMode ) return bRet;

    if( WRITEINT_2SPACES==iMode )
    {
        return write_2spaces(file);
    }
  //WRITEINT_LINEEND
    return write_line(file);
}

//write the rgb value in gdkcolor
unsigned char write_rgb(GdkColor *pGdkColor, FILE * file)
{
    if ( !write_integer(pGdkColor->red, file, WRITEINT_2SPACES) )
    {
        SB_INKERRPRINTF("failed, pGdkColor->red= %d \n", pGdkColor->red);
        return 0;
    }
    if ( !write_integer(pGdkColor->green, file, WRITEINT_2SPACES) )
    {
        SB_INKERRPRINTF("failed, pGdkColor->green= %d \n", 
                        pGdkColor->green);
        return 0;
    }
    if ( !write_integer(pGdkColor->blue, file, WRITEINT_LINEEND) )
    {
        SB_INKERRPRINTF("failed, pGdkColor->blue= %d \n", pGdkColor->blue);
        return 0;
    }
    SB_INKPRINTF("rgb(%d,%d,%d) \n", pGdkColor->red, 
                    pGdkColor->green,pGdkColor->blue);
    return 1;
}

//write the rgb value in gdkcolor
unsigned char read_rgb(GdkColor *pGdkColor, FILE * file)
{
    int iColor;
    if ( !read_integer(&iColor, file) )
    {
        SB_INKERRPRINTF("failed\n");
        return 0;
    }
    pGdkColor->red=(guint16)iColor;
    if ( !read_integer(&iColor, file) )
    {
        SB_INKERRPRINTF("failed\n");
        return 0;
    }
    pGdkColor->green=(guint16)iColor;
    if ( !read_integer(&iColor, file) )
    {
        SB_INKERRPRINTF("failed\n");
        return 0;
    }
    pGdkColor->blue=(guint16)iColor;
    SB_INKPRINTF("rgb(%d,%d,%d) \n", 
                    pGdkColor->red, pGdkColor->green,pGdkColor->blue);
    return 1;
}
