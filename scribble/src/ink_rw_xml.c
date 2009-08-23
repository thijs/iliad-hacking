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
*  C Implementation: ink_rw_xml.c
*
* Description: 
* brief- ink read/write from/to xml.
*
* Author: Jian Shen, (C) 2006
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

#define MAX_XPATH 512

//16k,max points string in one stroke.
//future we need do sth to guarantee all points could be involved.
#define MAX_STOKE_STR 0x10000

gboolean xml_page_exist(erManifest *pXmlHandle,const char* sPageName)
{    
    if( NULL==pXmlHandle || NULL==sPageName)
    {
        SB_INKERRPRINTF("error:invalid parameter\n");
        return FALSE;
    }
    
    char sCurrXPath[MAX_XPATH];
    snprintf(sCurrXPath,MAX_XPATH,"/notes/pages/page[@id=\"%s\"]",sPageName);
    SB_FMPRINTF("sCurrXPath=\n%s\n",sCurrXPath);
    if( RET_OK!=ermXmlExist(pXmlHandle, sCurrXPath) )
    {
        SB_INKERRPRINTF("page[%s] not existed\n",sPageName);
        return FALSE;
    }
    return TRUE;
}

// parse one stroke from string.seperated cordinates by space or line-feed.
// return 0 if successed,else return -1
int xml_parse_stroke(PtrStroke pStroke,const char* strpoints)
{
    if (strpoints == NULL || pStroke == NULL)
    {
        SB_INKERRPRINTF("failed:incorrect arguments\n");
        return -1;
    }
    
    gchar**  points_set=g_strsplit_set(strpoints," \n\t",-1);
    if ( NULL==points_set[0])
    {
        SB_INKERRPRINTF("returned:nPoints=0\n");
        pStroke->nPoints=0;
        return 0;
    }
    
    /*
    int i=0;
    printf("====================\n",i,points_set[i]);
    while( points_set[i] && strlen(points_set[i]) )
    {
        printf("data[%d]=[=%s=]\n",i,points_set[i]);
        i++;
    }
    printf("============total data number=[%d]\n",i);
    */
    
    PtrInkPoint point;
    int i=0;   
    while( points_set[i] && strlen(points_set[i]) )
    {
        point = construct_point ();
        if ( NULL==point )
        { 
            SB_INKERRPRINTF("construct point error\n");
            g_strfreev(points_set);
            return -1;
        }
        point->x=atoi(points_set[i]);
        i++;
        if(!points_set[i])
        {
            SB_INKERRPRINTF("failed getting point->y for the %dth point)\n",i/3+1);
            break;
        }
        point->y=atoi(points_set[i]);
        SB_INKPRINTF("point(%d,%d) \n", point->x, point->y);
        ink_add_point(pStroke, point);
        i++;        
        if(!points_set[i])
        {
            SB_INKERRPRINTF("failed getting point->pressure for the %dth point)\n",i/3+1);
            break;
        }
        //currently ,we ignore the pen presure
        i++;
    }
    pStroke->nPoints=i/3;
    //SB_FMPRINTF("total points=%d",pStroke->nPoints);
    g_strfreev(points_set);
    return 0;
}

// read ink object from xml handle.
// if successed return 1, else return 0
PtrInk xml_read_ink(erManifest *pXmlHandle,const char* sPageName)
{    
    if( NULL==pXmlHandle || NULL==sPageName)
    {
        SB_INKERRPRINTF("error:invalid parameter\n");
        return NULL;
    }
    PtrInk pink = construct_ink ();
    if (NULL == pink )
    {
        SB_INKERRPRINTF("error:construct_ink\n");
        return NULL;
    }
    
    if( !xml_page_exist(pXmlHandle,sPageName) )
    {
        return pink;
    }
    
    char sStrPoints[MAX_STOKE_STR];    
    char sStrokeXPath[MAX_XPATH];
    int i=1;
    char sHtmlColor[HTMLCOLOR_LEN];
    while(TRUE)
    {
        snprintf(sStrokeXPath,MAX_XPATH,
                 "/notes/pages/page[@id=\"%s\"]/strokes/stroke[%d]",
                 sPageName, i );
        //SB_FMPRINTF("path=\n%s\n",sStrokeXPath);
        if( RET_OK!=ermXmlGetString(pXmlHandle,sStrokeXPath, 
                                    sStrPoints, MAX_STOKE_STR) )
        {
            break;
        }
        //SB_INKPRINTF("\n\npoints=%s\n\n",sStrPoints);   
        PtrStroke pStroke = construct_stroke ();
        if (pStroke == NULL)
        {
            SB_INKERRPRINTF("return: construct_stroke failed \n");
            break;
        }
        xml_parse_stroke (pStroke,sStrPoints);
        
        ermXmlGetAttributeInt(pXmlHandle,sStrokeXPath,
                              "pensize",&pStroke->iPenSize);
        ermXmlGetAttributeString(pXmlHandle,sStrokeXPath,
                              "color",sHtmlColor,HTMLCOLOR_LEN);
        Util_GdkColorFromHtmlColor(&pStroke->gdkColor,sHtmlColor);
        
        ink_add_stroke (pink, pStroke);
        i++;
    }
    SB_FMPRINTF("read %d strokes \n",i);
    return pink;
}


#define MAX_CHARS_IN_LINE 64
// write points string to xml file.
/*  fomat of a stroke:
      <stroke layer="0" color="#000000" pensize="1"
               linestyle="solid">
        20 25 0
        30 30 0
      </stroke>
*/
gboolean xml_write_stroke (erManifest *pXmlHandle,
                           const char* xpath, 
                           const PtrStroke pStroke)
{
    if( NULL==xpath || NULL==pStroke || 0==pStroke->nPoints)
    {
        return FALSE;
    }    
    
    ermXmlSetAttributeInt(pXmlHandle,xpath,"layer",0);
    ermXmlSetAttributeString(pXmlHandle,xpath,"linestyle","solid",strlen("solid"));
    ermXmlSetAttributeInt(pXmlHandle,xpath,"pensize",pStroke->iPenSize);
    
    char sHtmlColor[10];
    Util_HtmlColorFromGdkColor(sHtmlColor,10,&pStroke->gdkColor);
    ermXmlSetAttributeString(pXmlHandle,xpath,
                             "color",sHtmlColor,HTMLCOLOR_LEN);
    
    char dataLine[MAX_CHARS_IN_LINE];
    GArray*  garr_stroke=g_array_sized_new(TRUE,FALSE,sizeof(char),1024);
    
    //SB_FMPRINTF("stroke->nPoints=%d \n", pStroke->nPoints);
    PtrInkPoint point = pStroke->firstPoint;
    while (point)
    {
        //write a line for a point
        snprintf(dataLine,MAX_CHARS_IN_LINE,"%d %d 0\n",point->x,point->y);        
        g_array_append_vals(garr_stroke,dataLine,strlen(dataLine));      
        point = point->nextPoint;     
    }  
    //printf("\n\nfinal stroke string:%s\n\n",  garr_stroke->data);
    ermXmlSetString(pXmlHandle, xpath, garr_stroke->data);
    
    g_array_free(garr_stroke,FALSE);
    return TRUE;
}


//write header information to .irx
int xml_write_irxHead(erManifest *pErManiHandle)
{
    ermXmlNewString(pErManiHandle, "/", "notes", "");
    ermXmlNewString(pErManiHandle, "/notes", "version", "");
    ermXmlNewString(pErManiHandle, "/notes/version", "number", "1.0");
    ermXmlNewString(pErManiHandle, "/notes/version","orgnization", "iRex Technologies");
    
    ermXmlNewString(pErManiHandle, "/notes", "screen", "");
    //units can be px,mm or in.
    ermXmlNewString(pErManiHandle, "/notes/screen", "units", "px");
    ermXmlNewInt(pErManiHandle, "/notes/screen","dpi", 160);
        
    ermXmlNewString(pErManiHandle, "/notes", "pages", "");
    
    return 0;
}

//write page header
void xml_write_page_head(erManifest *pXmlHandle,const char* xpath)
{
    ermXmlSetAttributeString(pXmlHandle,xpath, 
                             "backgroundcolor","#000000",HTMLCOLOR_LEN);
    ermXmlNewInt(pXmlHandle, xpath, "orientation", 0);
    ermXmlNewInt(pXmlHandle, xpath, "height", 935);
    ermXmlNewInt(pXmlHandle, xpath, "width", 768);
    ermXmlNewString(pXmlHandle, xpath, "strokes", "");
}

// write one page to xml. under xpath.(end without'/')
gboolean xml_write_ink (erManifest *pXmlHandle,const char* xpath,
                        const PtrInk pink,const char* sPageName)
{
    if( NULL==pXmlHandle || NULL==xpath || NULL==sPageName )//0==pink->nStrokes
    {
        return FALSE;
    }    
    SB_FMPRINTF("--page,path=[%s],sPageName=%s",xpath,sPageName);
    
    char sCurrXPath[MAX_XPATH];
    
    //if the page exist,we first empty it.
    if( !xml_page_exist(pXmlHandle,sPageName) )
    {
        ermXmlNewString(pXmlHandle, xpath, "page", "");
        
        snprintf(sCurrXPath,MAX_XPATH,"%s/page[last()]",xpath);   
        ermXmlSetAttributeString(pXmlHandle,sCurrXPath,
                                "id",sPageName,strlen(sPageName) );
        snprintf(sCurrXPath,MAX_XPATH,"%s/page[@id=\"%s\"]",xpath,sPageName); 
    }
    else
    { //empty it firstly
        snprintf(sCurrXPath,MAX_XPATH,"%s/page[@id=\"%s\"]",xpath,sPageName); 
        ermXmlSetString(pXmlHandle,sCurrXPath, "");
    }
    if( NULL==pink || 0==pink->nStrokes )
    {
        return TRUE;
    }
     
    xml_write_page_head(pXmlHandle,sCurrXPath);
    
    SB_FMPRINTF("ink->nStrokes=%d \n", pink->nStrokes);
    PtrStroke pStroke = pink->firstStroke;
    int i=1;
    char strokesPath[MAX_XPATH];
    snprintf(strokesPath,MAX_XPATH,"%s/strokes",sCurrXPath);
    char sLastStrokeXPath[MAX_XPATH];//xpath for last stroke

    while (pStroke)
    {     
        ermXmlNewString(pXmlHandle, strokesPath, "stroke", "");
        snprintf(sLastStrokeXPath,MAX_XPATH,"%s/stroke[%d]",strokesPath,i);
        //SB_FMPRINTF("\nsLastStrokeXPath=[%s]\n",sLastStrokeXPath);
        xml_write_stroke(pXmlHandle,sLastStrokeXPath,pStroke);
        pStroke = pStroke->nextStroke;
        i++;
    }
    return TRUE;
}
