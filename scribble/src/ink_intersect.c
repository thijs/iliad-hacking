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
*  C Implementation: ink_intersect.c
*
* Description: 
* brief- functions for intersection judge and erase of ink.
*
* Author: Jian Shen,Lin Ren. (C) 2006
*
* Copyright: See COPYING file that comes with this distribution
*
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

//for INT_MIN,INT_MAX
#include <limits.h>

#include "ink_intersect.h"
#include "ScribbleLog.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))


//if intersect in tolerance pixels and the points in stroke is enough little,
//we do hittest.
#define TOLERANCE_PIXELS -3
#define LONELY_STROKE_POINTS 4

//a point out of rectangle? area-left corner(x1,y1),right corner(x2,y2)
//return 1 if yes.
int isPointOutOfRange(int x,int y,int x1,int y1,int x2,int y2)
{
    return ( x < x1 || x>x2 || y<y1 || y>y2);
} 

int isPointOutOfStroke(PtrStroke pStroke,GdkPoint *p)
{
    return isPointOutOfRange(p->x,p->y,
                             pStroke->min_x,pStroke->min_y,
                             pStroke->max_x,pStroke->max_y );
}

//compute dest_w,dest_h for tolerance for erase few-points stroke.
//it can improve the performance of eraseing
gboolean isLineCrossStrokeRange(PtrStroke pStroke,GdkPoint *p0,GdkPoint *p1,
                           int* p_rec_width,int* p_rect_height)
{    
    if( NULL==p_rec_width || NULL==p_rect_height) return FALSE;
    
    int dest_x,dest_y;
    dest_x = max (min(p0->x,p1->x), pStroke->min_x);
    dest_y = max (min(p0->y,p1->y), pStroke->min_y);
    *p_rec_width = min (max(p0->x,p1->x), pStroke->max_x)- dest_x;
    *p_rect_height = min (max(p0->y,p1->y), pStroke->max_y) - dest_y;
    
    return ( *p_rec_width>=0 && *p_rect_height>=0);
}

/* intern functions */
/* return 1 if lines intersect, 0 if not */ 
int lineIntersects(int x1, int y1, int x2, int y2, 
                   int x3, int y3, int x4, int y4) 
{ 
    long ry1, ry2, ry3, ry4; 
    long rx1, rx2, rx4;

    if ( (min(x1,x2) > max(x3,x4)) || (min(x3,x4) > max(x1,x2)) )
    {
        return 0;
    }

    if ( (min(y1,y2) > max(y3,y4)) || (min(y3,y4) > max(y1,y2)) )
    {
        return 0;
    }

    x2 -= x1;  y2 -= y1; 
    x3 -= x1;  y3 -= y1; 
    x4 -= x1;  y4 -= y1; 

    ry3 = (long) y3 * x2 - (long) x3 * y2; 
    ry4 = (long) y4 * x2 - (long) x4 * y2; 

    if((ry3 > 0 && ry4 > 0) || (ry3 < 0 && ry4 < 0)) 
        return (0); 

    x1 = -x3;  y1 = -y3; 
    x2 -= x3;  y2 -= y3; 
    x4 -= x3;  y4 -= y3; 

    ry1 = (long) y1 * x4 - (long) x1 * y4; 
    ry2 = (long) y2 * x4 - (long) x2 * y4; 

    if((ry1 > 0 && ry2 > 0) || (ry1 < 0 && ry2 < 0)) 
        return 0; 

    if(ry1 || ry2 || ry3 || ry4) 
        return 1; 

    /* special cases: */ 
    if(x4 == 0 && y4 == 0)
    { 
        if((x1 > 0 && x2 > 0) || (x1 < 0 && x2 < 0)) return 0; 
        if(x1 == 0 && x2 == 0)
        { 
            if ((y1 > 0 && y2) > 0 || (y1 < 0 && y2 < 0)) return 0; 
        } 
        return 1; 
    } 

    rx1 = (long) x1 * x4 + (long) y1 * y4; 
    rx2 = (long) x2 * x4 + (long) y2 * y4; 
    rx4 = (long) x4 * x4 + (long) y4 * y4; 

    if(rx1 >= 0 && rx1 <= rx4) return 1; 

    if(rx2 >= 0 && rx2 <= rx4) return 1; 

    if(rx1 <= rx2)
    { 
        if(0 >= rx1 && 0 <= rx2) return 1; 
        if(rx4 >= rx1 && rx4 <= rx2) return 1; 
    } 
    else
    { 
        if(0 >= rx2 && 0 <= rx1) return 1; 
        if(rx4 >= rx2 && rx4 <= rx1) return 1; 
    } 
    return 0; 
} 


int isIntersects(PtrStroke pStroke, GdkPoint *srcPt1, GdkPoint *srcPt2)
{
    InkPoint *targetPt1, *targetPt2;
    int i;

    if ((srcPt1->x == srcPt2->x) && (srcPt1->y == srcPt2->y))
    {
        return 0;//??
    }

    targetPt1 = pStroke->firstPoint;

    for (i=1; i < pStroke->nPoints; i++)
    {
        targetPt2 = targetPt1->nextPoint;
        if (  (targetPt1->x != targetPt2->x)
            ||(targetPt1->y != targetPt2->y) )
        {
            if (lineIntersects(srcPt1->x - pStroke->iPenSize/2, 
                srcPt1->y - pStroke->iPenSize/2, 
                srcPt2->x + pStroke->iPenSize/2, 
                srcPt2->y + pStroke->iPenSize/2, 
                targetPt1->x, 
                targetPt1->y, 
                targetPt2->x, 
                targetPt2->y) > 0)
            {
                return 1;
                break;
            }
        }
        //update point ptr
        targetPt1 = targetPt2;
    }
    return 0;
}

//currently, we fixed the eraser size.
#define ERASER_SIZE 8
int hitTest(PtrStroke pStroke, GdkPoint *point)
{
    PtrInkPoint pt;
    int i;

    pt = pStroke->firstPoint;
    //int iEraserSize=pStroke->iPenSize;
    int iEraserSize=ERASER_SIZE;
    for (i=0; i < pStroke->nPoints; i++)
    { 
        if ( ( pt->x >= (point->x - iEraserSize/2))
               && ( pt->x <= (point->x + iEraserSize/2))
               && ( pt->y >= (point->y - iEraserSize/2))
               && ( pt->y <= (point->y + iEraserSize/2)) )
        {
            return 1;
        }
        pt = pt->nextPoint;
    }
    return 0;
}

//whether the line interset with stroke.
PtrStrokeIntersections_t findIntersections(PtrStroke pStroke, GdkPoint *srcPt1, GdkPoint *srcPt2)
{
    InkPoint *targetPt1, *targetPt2;
    int    i;
    PtrStrokeIntersections_t intersections = NULL;
    StrokeIntersection_t  *pSection;
    StrokeIntersection_t  *pCurSection = NULL;;

    if ( (srcPt1->x == srcPt2->x) && (srcPt1->y == srcPt2->y) )
    {
        return NULL;
    }

    //allocate memory 
    intersections = (PtrStrokeIntersections_t)
            malloc(sizeof(StrokeIntersections_t));
    //assert(intersections != NULL);
    memset(intersections, 0, sizeof(StrokeIntersections_t)); 

    targetPt1 = pStroke->firstPoint;
    for (i=1; i < pStroke->nPoints; i++)
    {
        targetPt2 = targetPt1->nextPoint;
        if (  (targetPt1->x != targetPt2->x)
               ||(targetPt1->y != targetPt2->y) )
        {
            if (lineIntersects(srcPt1->x - pStroke->iPenSize/2,
                srcPt1->y - pStroke->iPenSize/2,
                srcPt2->x + pStroke->iPenSize/2,
                srcPt2->y + pStroke->iPenSize/2,
                targetPt1->x,
                targetPt1->y,
                targetPt2->x,
                targetPt2->y) > 0)
            {
                pSection = (StrokeIntersection_t  *)
                        malloc(sizeof(StrokeIntersection_t));
                //assert(pSection != NULL);
                pSection->beginPt = targetPt1;
                pSection->endPt = targetPt2;
                pSection->pNextSection = NULL;
                //add the section into the section list
                if (intersections->interSection == NULL)
                {
                    pCurSection = intersections->interSection = pSection;
                }
                else if (pCurSection != NULL)
                {
                    pCurSection->pNextSection = pSection;
                }
                else
                {
                    SB_INKERRPRINTF("ERROR in Scribble\n");
                }
                intersections->nSections++;
            }
        }
        //update point ptr
        targetPt1 = targetPt2;
    }
    return intersections;
}

//delete current stroke,and move pCurr to next.
void delStroke(PtrInk pink,PtrStroke pPrev,PtrStroke *ppCur,PtrInk pDelInk)
{
    if( NULL==pink || NULL==*ppCur) return ;
    //validate accurracy
    if( pPrev!=NULL && pPrev->nextStroke!=*ppCur) return ;

    PtrStroke pNext=(*ppCur)->nextStroke;
    //remove the Strok to deleted ink
    if( NULL==pDelInk->firstStroke)
    {
        pDelInk->firstStroke=pDelInk->lastStroke=*ppCur;
    }
    else
    {
        pDelInk->lastStroke->nextStroke=*ppCur;
        pDelInk->lastStroke=*ppCur;
    }
    pDelInk->nStrokes++;
    (*ppCur)->nextStroke=NULL;

    pink->nStrokes--;
    *ppCur=pNext;
    if( NULL==pPrev )
    {//head pointer
        pink->firstStroke=pNext;
    }
    else
    {
        pPrev->nextStroke=pNext;
    }
    if( NULL==pNext)
    {//tail pointer
        pink->lastStroke=pPrev;
    }
    SB_INKPRINTF("removed!\n");
}

//erase the strokes intersected with specified line
//pDelink--a linked list of the deleted stroke.
void delStrokesByLine(PtrInk pink,GdkPoint *p0,GdkPoint *p1,PtrInk pDelInk)
{
    int i;

    if( NULL==pink || NULL==pDelInk) return;

    PtrStroke pCurStroke = pink->firstStroke;
    PtrStroke pPrevStroke = NULL;
    
    gboolean bRealIntersect,bCrossStrokeRange;
    int rect_w,rect_h;
            
    int nStrokes=pink->nStrokes;//it may change when running
    for (i=0; i< nStrokes ;i++)
    {
        if ( NULL==pCurStroke ) break;
        //a little inaccuracy,but accelerate more,
        //if( isPointOutOfStroke(pCurStroke,p0) &&  /
        //    isPointOutOfStroke(pCurStroke,p1) )
        bCrossStrokeRange=isLineCrossStrokeRange(pCurStroke,p0,p1,&rect_w,&rect_h);
        bRealIntersect=FALSE;
        if( pCurStroke->nPoints <= LONELY_STROKE_POINTS )
        {
            if( rect_w>=TOLERANCE_PIXELS && rect_h>=TOLERANCE_PIXELS )
            {   
                SB_INKPRINTF("\n__special:p0=[%d,%d],p1=[%d,%d],[rect_w=%d,rect_h=%d]\n",
                              p0->x,p0->y,p1->x,p1->y,rect_w,rect_h);
                
                //for LONELEY point ,do hittest
                if( hitTest(pCurStroke, p0) || hitTest(pCurStroke, p1))
                { 
                    bRealIntersect=TRUE;
                }
                SB_INKPRINTF("hitTest: bRealIntersect=%d\n",bRealIntersect);
            }
        }
        if( !bRealIntersect && bCrossStrokeRange)
        {
            if( isIntersects(pCurStroke,p0,p1) )
            {
                bRealIntersect=TRUE;
            }
            SB_INKPRINTF("isIntersects: bRealIntersect=%d\n",bRealIntersect);
        }
        if(bRealIntersect)
        {
            SB_INKPRINTF("\n(%d,%d),(%d,%d)intersect,range[(%d,%d),(%d,%d)]\n",
                         p0->x,p0->y,p1->x,p1->y,
                         pCurStroke->min_x,pCurStroke->min_y,
                         pCurStroke->max_x,pCurStroke->max_y );
            //remove current stroke,current move to next
            delStroke(pink,pPrevStroke,&pCurStroke,pDelInk);
            continue;//need not change prev,and current is changed.
        }
        //not intersect and not in range.
        pPrevStroke=pCurStroke;
        pCurStroke = pCurStroke->nextStroke;
    }//end for stroke iteration
}


//erase the intersected lines with specified line
void delInterSectLines(PtrInk pink,GdkPoint *p0,GdkPoint *p1)
{
    PtrStrokeIntersections_t intersections;
    int i,j;
    StrokeIntersection_t  *pCurIntersection=NULL;

    PtrStroke pCurStroke = pink->firstStroke;
    int nStrokes=pink->nStrokes;//it may change when running
    for (i=0; i<nStrokes ;i++,pCurStroke = pCurStroke->nextStroke)
    {
        if ( NULL==pCurStroke ) break;

        intersections = findIntersections(pCurStroke,p0,p1);

        if ( NULL==intersections ) continue;

        pCurIntersection = intersections->interSection;

        for (j=0; j<intersections->nSections;i++)
        {
            if ( NULL==pCurIntersection ) break;
	//Erase the intersection,and if possible,split it
/*
            ink_draw_line(pCurIntersection->beginPt->x,
            pCurIntersection->beginPt->y,
            pCurIntersection->endPt->x,
            pCurIntersection->endPt->y);
*/
	
            pCurIntersection = pCurIntersection->pNextSection;
        }
    }//end for stroke iteration
}

