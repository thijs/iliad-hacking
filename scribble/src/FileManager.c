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
*  C Implementation: FileManager
*
* Description: 
*
*
* Author: Jian Shen, (C) 2006
*
* Copyright: See COPYING file that comes with this distribution
*
*/

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <strings.h>

#include <libermanifest/ermanifest.h>

#include "FileManager.h"
#include "FileMgr_page.h"
#include "FileMgr_xml.h"
#include "ScribbleDefine.h"
#include "ScribbleLog.h"
#include "ScribbleUtils.h"


static int get_result_dirname(char *sResultDir, int iSize,const char* sDataDir);



//creat directrory hirachy by toplevel data dir
int createDirHirachy(PFileManager pFM)
{    
    //if dir not exist, creat dirstruct
    const char* sDataDir=pFM->sDataDir;
    if( pFM->bNeedCreatDirHirachy && !FileExist(sDataDir) )
    {
        SB_FMPRINTF("create for\n%s\n",sDataDir);
        if( 0!=CreateDir(sDataDir))
        {
            SB_ERRORPRINTF("create directory [%s]fail\n",sDataDir);
            return -1;
        }
        SB_FMPRINTF("create directory [%s] ok\n",sDataDir);
        char sResultDir[MAX_PATHNAME+1];	   
	    //crearete ./result directory
        get_result_dirname(sResultDir,sizeof(sResultDir),sDataDir);
        if( 0!=CreateDir(sResultDir))
        {
            SB_ERRORPRINTF("create directory [%s]fail\n",sResultDir);
            return -1;
        }
        SB_FMPRINTF("create directory [%s] ok\n",sResultDir);
                 
        //now we can init the handle to .irx inkfile.
        if( 0!=fm_initIrxHandle(pFM,FALSE) )
        {
            SB_ERRORPRINTF("create .irx inkfile error\n");
            return -1;
        }
        pFM->bNeedCreatDirHirachy=FALSE; 
        SB_FMPRINTF("done\n");         
    }  

    return 0;
}

//for test correctness
void dumpFileManager(PFileManager pFM)
{

    SB_FMPRINTF("\n==========================================\n"
            "sTmplPathName=[%s]\n"
            "sManifestPathName=[%s]\n"
            "sDataDir=[%s]\n"
            "sScribbleName=[%s]\n"
            "========================================\n",
            pFM->sTmplPathName,
            pFM->sManifestPathName,
            pFM->sDataDir, 
            pFM->manifest.sScribbleName);
}

//init filemanager with template file
int initFileManagerByTmpl(PFileManager pFM,const char* sTmplPathName)
{    
    SB_FMPRINTF("init by %s\n",sTmplPathName);
    int iLenOfSuffix=strlen(".bmp");
    if( NULL==sTmplPathName || strlen(sTmplPathName)<=iLenOfSuffix )
        return -1;
    
    memset(pFM,0,sizeof(FileManager));
    
    strncpy(pFM->sTmplPathName,sTmplPathName,
            sizeof(pFM->sTmplPathName)-1);
    
    GetDirFromFullName(pFM->sDataDir,sizeof(pFM->sDataDir),sTmplPathName);

    if( 0!=getUniqNameForSCRIB(pFM) )
    {
        return -1;
    }
    //DataDir should just above of manifest
    strcat(pFM->sDataDir,pFM->manifest.sScribbleName);
    strcat(pFM->sDataDir,"/");
       
    //constract manifest       
    strcpy(pFM->sManifestPathName,pFM->sDataDir);
    strcat(pFM->sManifestPathName,MANIFEST_FILENAME);
    
    dumpFileManager(pFM);
    //init manifest.
    pFM->manifest.iLastOpenPage=0;//numbered from 1.
    pFM->manifest.iTotalPages=0;       
    
    pFM->bNeedCreatDirHirachy=TRUE;
    pFM->isCreatedFromTmpl=TRUE;
    
    //construct the first new page.
    insertNewPage(pFM,1,NULL);
    jumptoScribPage(pFM,1);    
    
    return 0;
}

int initFileManagerByManifest(PFileManager pFM,const char* sManifestPathName)
{
    SB_FMPRINTF("init by \n%s\n",sManifestPathName);
    memset(pFM,0,sizeof(FileManager));
    strncpy(pFM->sManifestPathName,sManifestPathName,
            sizeof(pFM->sManifestPathName)-1 );
    if( 0!=loadManifest(pFM) )
    {
        SB_ERRORPRINTF("load manifest[%s] error\n",sManifestPathName);
        return -1;
    }    

    GetDirFromFullName(pFM->sDataDir,sizeof(pFM->sDataDir),sManifestPathName);   
    
    pFM->isCreatedFromTmpl=FALSE;
    
    SB_FMPRINTF("notes type is:[%s]",pFM->manifest.sInkType);
    //if type is not irex(current type), we need conversion it to new format
    if( 0!=strcasecmp(pFM->manifest.sInkType,SCRIB_TYPE) )
    {
        SB_FMPRINTF("init by \n%s\n",sManifestPathName);
        if( 0!=fc_convertold2irx(pFM) )
        {
            SB_ERRORPRINTF("converting error\n");
            return -1;
        }
        pFM->bNeedSaveManifest=TRUE;
        SB_FMPRINTF("update manifest...");
        strncpy(pFM->manifest.sInkType,SCRIB_TYPE,sizeof(pFM->manifest.sInkType));
        //change the type to irx and save the manifest
        if( 0!=fm_setIrxTypeAndSave(pFM) )
        {
            SB_ERRORPRINTF("save type error[%s]\n",pFM->sManifestPathName);
            return -1;
        }
    } 
       
    if( 0!=fm_initIrxHandle(pFM,TRUE) )
    {
            SB_ERRORPRINTF("open .irx inkfile error\n");
            return -1;
    }
    
    pFM->bNeedSaveManifest=FALSE;
       
    PScribPage pScribPage=jumptoScribPage(pFM,pFM->manifest.iLastOpenPage);
    if( NULL==pScribPage ) //no page
        return -1;
    //get template from CurrentPage
    strcpy(pFM->sTmplPathName,pScribPage->sImgFileName);    
    
    pFM->bNeedCreatDirHirachy=FALSE;
    
    dumpFileManager(pFM);
    return 0;
}

void FreeFileManager(PFileManager pFM)
{
    if( NULL==pFM ) return;
    
    PScribPage pCurrPage=pFM->manifest.pFirstScribPage;
    PScribPage pNextPage=NULL;
    while(pCurrPage)
    {
        pNextPage=pCurrPage->pNextPage;
        //free scribpage inlcude ink object
        freeScribPage(pCurrPage);
        pCurrPage=pNextPage;
    }
    if( NULL!=pFM->irxHandle)
    {
        free(pFM->irxHandle);
        pFM->irxHandle=NULL;
    }
    memset(pFM,0,sizeof(FileManager));
}


//get absolute path (eg.emplate image) from current scribble page.
char* getAbsPathFromCurrScrib(char* sAbsFileName,
                                    int iSize,
                                    const char* currFileName,
                                    const PFileManager pFM)
{
    memset(sAbsFileName,0,iSize);
    if( currFileName[0] == '/' ) 
    {
        strncpy(sAbsFileName,currFileName,iSize-1);
    }
    else
    {    
        
        snprintf( sAbsFileName,iSize-1,
                    "%s%s",
                    pFM->sDataDir,
                    currFileName);
    }
    return sAbsFileName;
}

//get current ink filename
int get_ink_filename (char *inkFileName, int iSize,
                      const PFileManager pFM)
{
    //SB_FMPRINTF("__start__\n");   
    memset(inkFileName,0,iSize);
    snprintf( inkFileName,iSize-1,
              "%s%s.irx",
              pFM->sDataDir,
              pFM->manifest.sScribbleName);
    //SB_FMPRINTF("__end__\ninkFileName=%s\n", inkFileName);
    return 0;
}


//get result directory name
static int get_result_dirname(char *sResultDir, int iSize,const char* sDataDir)
{
    SB_FMPRINTF("__start__\n");
    memset(sResultDir,0,iSize);
    snprintf(sResultDir,iSize-1, "%sresult",sDataDir);
    SB_FMPRINTF("__end__\nresultdir=%s\n", sResultDir);
    return 0;
}

//get specified page result file name
int get_result_filename (char *resultFileName, int iSize,
                         const PFileManager pFM,const PScribPage pScribPage)
{
    //SB_FMPRINTF("__start__\n");    
    if( NULL==pScribPage) return -1;
    
    memset(resultFileName,0,iSize);
    snprintf( resultFileName,iSize-1,
              "%sresult/%s.png",
              pFM->sDataDir,
              pScribPage->sPageName);
    //SB_FMPRINTF("__end__\nresultfile=%s\n", resultFileName);
    return 0;
}

//get current result file name from current open page
int get_curr_result_filename (char *resultFileName, int iSize,const PFileManager pFM)
{
    return get_result_filename(resultFileName,iSize,
                               pFM,getCurrScribPage(pFM));
}

//for test correctness
void dumpManifest(Manifest *pM)
{
    printf("\n=================================\n"
            "sScribbleName=[%s]\n"
            "iLastOpenPage=[%d]\n"
            "iTotalPages=[%d]\n",
            pM->sScribbleName,pM->iLastOpenPage,pM->iTotalPages);
    PScribPage  pPage=pM->pFirstScribPage;;
    while(pPage)
    {
        printf("----------------------------------\n");
        printf("sImgFileName=[%s]\nsPageName=[%s]\n",  
               pPage->sImgFileName,pPage->sPageName);
        printf("sImgFileName=[%s]\nsPageName=[%s]\nbDirty=%d\n",  
               pPage->sImgFileName,pPage->sPageName,pPage->bDirty);
                
        pPage=pPage->pNextPage;
    }
    printf("=================================\n");
}
