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
*  C Implementation: FileMgr.c
*
* Description: 
*       manage the scribble page in FileManager data structure.
*
* Author: Jian Shen, (C) 2006
*
* Copyright: See COPYING file that comes with this distribution
*
*/

#include <gtk/gtk.h>
#include <time.h>

#include "FileManager.h"
#include "ScribbleDefine.h"
#include "ScribbleLog.h"
#include "ScribbleUtils.h"


//get template name,no path,and without .bmp
int getTemplName(char* sTmpl,int iSize,const char* sTmplPathName)
{
    int iLenOfSuffix=strlen(".bmp");
    if( NULL==sTmplPathName || strlen(sTmplPathName)<=iLenOfSuffix )
        return -1;        
    
    GetFileNameFromFullName(sTmpl,iSize,sTmplPathName);
    //we should remove the suffix
    sTmpl[strlen(sTmpl)-iLenOfSuffix]=0;
    return 0;
}

int getUniqNameForSCRIB(PFileManager pFM)
{      
    char sTmpl[MAX_FILENAME];
    if( 0!=getTemplName(sTmpl,sizeof(sTmpl),pFM->sTmplPathName) )
    {
        SB_ERRORPRINTF("invalid template name\n%s",pFM->sTmplPathName);
        return -1;
    }
    
    char outstr[200];
    time_t iTime;
    time(&iTime);
    
    char* sScribName=pFM->manifest.sScribbleName;
    int iSize=sizeof(pFM->manifest.sScribbleName);
    
    unsigned int i=0;
    while(i++<60)//limit the collisions
    {
        GetStrFromTime(outstr,sizeof(outstr),iTime);
        memset(sScribName,0,iSize);
        snprintf(sScribName,iSize,"%s_%s",sTmpl,outstr);
        
        if(!FileExist2(sScribName,pFM->sDataDir))
        {
            SB_FMPRINTF("SCRIB=%s\n",sScribName);
            return 0;//get the name
        }
        iTime++;
    }
    return -1;
}

//whether or not a page name existed
gboolean isPageNameExisted(Manifest *pManifest,const char* sPageName)
{
    if( NULL==pManifest) return FALSE;

    PScribPage  pCurrPage=pManifest->pFirstScribPage;
    while(pCurrPage)
    {
        if( 0==strcmp(pCurrPage->sPageName,sPageName) )
        {
            return TRUE;//existed
        }
        pCurrPage=pCurrPage->pNextPage;
    }
    return FALSE;
}

//get new uniq name for one page from the SCF name.
int getNewPageName(ScribPage *pScribPage,Manifest *pManifest)
{
    char outstr[200];
    time_t iTime;
    unsigned int iSize=sizeof(pScribPage->sPageName);

    if( NULL==pScribPage || NULL==pManifest) return -1;

    memset(pScribPage->sPageName,0,iSize);
    time(&iTime);
    //result is string from time but no duplicated.
    int i=0;
    SB_FMPRINTF("start get spagename,time=%d\n",(unsigned int)iTime);
    while(i++<500) //control the collision time
    {
        GetStrFromTime(outstr,sizeof(outstr),iTime);
        if(!isPageNameExisted(pManifest,outstr))
        {
            strncpy(pScribPage->sPageName,outstr,iSize-1);
            SB_FMPRINTF("sPageName=%s\n",pScribPage->sPageName);
            return 0;//get the name
        }
        iTime++;
    }
    return -1;
}


//get current ScribPage
PScribPage getCurrScribPage(PFileManager pFM)
{
    return pFM->pCurrPage;
}

//get Image File for current page.
const char* getCurrImgFileName(PFileManager pFM)
{
    return getCurrScribPage(pFM)->sImgFileName;
}

gboolean isPageNoValid(PFileManager pFM,const int iPageNo)
{
    return ( iPageNo>0 && iPageNo<=getTotalPage(pFM));
}

//get specified pageno.
PScribPage getScribPage(PFileManager pFM,const int iPageNo)
{
    //SB_FMPRINTF("get pageno:%d[total=%d,curropen=%d]\n",
    //iPageNo,getTotalPage(pFM),getCurrPageNo(pFM));
    if( !isPageNoValid(pFM,iPageNo) ) return NULL;
    int iCurr=getCurrPageNo(pFM);
    if( iPageNo==iCurr && isPageNoValid(pFM,iCurr))
    {
        return pFM->pCurrPage;
    }
    PScribPage pCurr=pFM->pCurrPage;
    if( iCurr>iPageNo || iCurr<1) //initial,iCurr==0
    {
        pCurr=pFM->manifest.pFirstScribPage;//first page
        iCurr=1;
    }
    //must exist
    while( iCurr!=iPageNo && iCurr<=getTotalPage(pFM))
    {
        pCurr=pCurr->pNextPage;
        iCurr++;
    }
    //SB_FMPRINTF("result:pCurr==NULL?%d,iCurr=%d\n",pCurr==NULL,iCurr);
    return pCurr;
}


//jumpto specified pageno.
PScribPage jumptoScribPage(PFileManager pFM,int iPageNo)
{
    SB_FMPRINTF("jumpto pageno:%d[total=%d,curropen=%d]\n",
                iPageNo,getTotalPage(pFM),getCurrPageNo(pFM));
    //if iPageNo<1, then set to 1, >iTotalpage, set to last.
    if( iPageNo<1 ) iPageNo=1;

    if( iPageNo>getTotalPage(pFM) ) iPageNo=getTotalPage(pFM);

    PScribPage pPage=getScribPage(pFM,iPageNo);
    //must find, actually.
    if( NULL!=pPage )
    {
        pFM->iCurrOpenPage=iPageNo;
        pFM->pCurrPage=pPage;
    }
    SB_FMPRINTF("jump done.pageno=%d[pPage==NULL?%d]\n",
                iPageNo,NULL==pPage);
    return pPage;
}


//insert new page at specified page no.
//sImgName--if it's null/empty,then set it to the default.
int insertNewPage(PFileManager pFM,int iPageNo,const char* sImgName)
{
    int iTotalPage=getTotalPage(pFM);
    SB_FMPRINTF("insert %d.current[total=%d,curropen=%d]\n",
                iPageNo,getTotalPage(pFM),getCurrPageNo(pFM));
    if( iPageNo<1 || iPageNo>(1+iTotalPage) )
    {
        SB_WARNPRINTF("invalid pageno:%d[total=%d,curropen=%d]\n",
                    iPageNo,iTotalPage,getCurrPageNo(pFM));
        return -1;
    }
    ScribPage *pScribPage=(ScribPage *)malloc(sizeof(ScribPage));
    if(  NULL==pScribPage)
    {
        SB_ERRORPRINTF("error when malloc space\n");
        return -1;
    }
    memset(pScribPage,0,sizeof(ScribPage));
    if( 0!=getNewPageName(pScribPage,&pFM->manifest) )
    {
        SB_ERRORPRINTF("error when get new page name\n");
        free(pScribPage);
        return -1;
    }
    //generate a new ink data structure
    pScribPage->pink=construct_ink();
    assert(pScribPage->pink);
    
    const char *sCurrImgName=sImgName;
    if( isStrEmpty(sImgName) )
    {
        sCurrImgName=pFM->sTmplPathName;
    }
    //currently, the image name is always same as template name
    strcpy( pScribPage->sImgFileName,sCurrImgName);
    
    //get previous scribpage
    ScribPage *pPrevScribPage=getScribPage(pFM,iPageNo-1);
    if( NULL==pPrevScribPage)
    {//it's first page.
        pScribPage->pNextPage=pFM->manifest.pFirstScribPage;
        pFM->manifest.pFirstScribPage=pScribPage;	
    }
    else
    {
        pScribPage->pNextPage=pPrevScribPage->pNextPage;
        pPrevScribPage->pNextPage=pScribPage;
    }
    //reset total page
    pFM->manifest.iTotalPages++;
    //reset current pageno if necessary
    if( iPageNo<=getCurrPageNo(pFM))
    {
        pFM->iCurrOpenPage++;
    }
    
    pFM->bNeedSaveManifest=TRUE;
    SB_FMPRINTF("insert %d done.current[total=%d,curropen=%d]\n",
                iPageNo,getTotalPage(pFM),getCurrPageNo(pFM));
    return 0;
}

//insert new page after current open page.
int insertNewPageAfterCurr(PFileManager pFM)
{
    return insertNewPage(pFM,pFM->iCurrOpenPage+1,getCurrImgFileName(pFM));
}

//delete the ink and result file related with ScribPage
int deleteRelatedFiles(PFileManager pFM,ScribPage *pScribPage)
{
    if( NULL==pScribPage) return -1;

    char sFileName[MAX_PATHNAME+1];

    //del the result page
    get_result_filename (sFileName, sizeof(sFileName),pFM,pScribPage );
    if( 0!=FileDel(sFileName) )
    {
        SB_ERRORPRINTF("del result file[%s] fail\n",sFileName);
        return -1;
    }
    return 0;
}

void freeScribPage(PScribPage pPage)
{
     //free ink object
    if(pPage->pink)
    {
        free(pPage->pink);
        pPage->pink=NULL;
    }
    free(pPage);
}
//when delete current page, we should jump to another page.
//defaultly, jump to the first page.
//when there is only one page ,it is not permitted for del.
int deletePage(PFileManager pFM, int iPageNo)
{
    //dumpManifest(&pFM->manifest);
    int iTotalPage=getTotalPage(pFM);
    if( !isPageNoValid(pFM,iPageNo) )
    {
        SB_ERRORPRINTF("invalid pageno:%d[total=%d,curropen=%d]\n",
                    iPageNo,iTotalPage,getCurrPageNo(pFM));
        return -1;
    }
    if( 1==iTotalPage )
    {
        SB_WARNPRINTF("only 1 page,resuse to del\n");
        return -1;
    }
    //get this scribpage
    ScribPage *pScribPage=getScribPage(pFM,iPageNo);
    if( NULL==pScribPage )
    {//impossible case
        SB_ERRORPRINTF("sth error?[tobe del=%d,total=%d,curr=%d]\n",
                    iPageNo,iTotalPage,getCurrPageNo(pFM));
        return -1;
    }
    ScribPage *pPrevScribPage=getScribPage(pFM,iPageNo-1);
    if( NULL==pPrevScribPage )
    {//first page
        pFM->manifest.pFirstScribPage=pScribPage->pNextPage;	
    }
    else
    {
        pPrevScribPage->pNextPage=pScribPage->pNextPage;
    }
    //delete the real ink and result file.
    deleteRelatedFiles(pFM,pScribPage);
    //delete it from .irx
    fm_deleteOnePage(pFM,pScribPage);
    //free scribpage inlcude ink object
    freeScribPage(pScribPage);
    pScribPage=NULL;
    
    //reset total page
    pFM->manifest.iTotalPages--;

    //reset current pageno if necessary
    int iCurrPage=getCurrPageNo(pFM);
    if( iPageNo<iCurrPage )
    {
        pFM->iCurrOpenPage--;
    }
    if( iPageNo==iCurrPage ) //delete current page,then jump to 1.
    {
        //we should reset currpage pointer!!
        pFM->pCurrPage=pFM->manifest.pFirstScribPage;
        jumptoScribPage(pFM,1);//1 cannot be del,so it bounce to refresh.
    }
    pFM->bNeedSaveManifest=TRUE;
    //dumpManifest(&pFM->manifest); 
    return 0;
}

//when delete current page, we should jump to another page.
//defaultly, jump to the first page.
//when there is only one page ,it is not permitted for del.
int deleteCurrPage(PFileManager pFM)
{
    return deletePage( pFM,getCurrPageNo(pFM) );
}


//get current opened page.
int getTotalPage(PFileManager pFM)
{
    return pFM->manifest.iTotalPages;
}

int getCurrPageNo(PFileManager pFM)
{
    return pFM->iCurrOpenPage;
}

void fm_setIrxInkDirty(PFileManager pFM,gboolean bDirty)
{
    pFM->manifest.bNeedSaveIrx=bDirty;
}

void setCurrentPageDirty(PFileManager pFM,gboolean bDirty)
{
    getCurrScribPage(pFM)->bDirty=bDirty;
    if(bDirty)
    {
        fm_setIrxInkDirty(pFM,TRUE);
    }
}
