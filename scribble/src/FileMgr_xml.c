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
*  C Implementation: FileMgr_xml.c
*
* Description: 
*       fetch/save the xml manifest file for filemanager 
*
* Author: Jian Shen, (C) 2006
*
* Copyright: See COPYING file that comes with this distribution
*
*/

#include <gtk/gtk.h>

#include <libermanifest/ermanifest.h>

#include "FileManager.h"
#include "FileMgr_page.h"
#include "ScribbleLog.h"
#include "ScribbleUtils.h"
#include "ink_rw.h"


/* save the header of manifest as:
<package>
  <metadata>
    <dc-metadata>
        <Title>De Volkskrant</Title>
        <Description>van Maandag 10 april, 2006</Description>
        <Date>2006-04-10T16:38:56-01:00</Date>
        <Format>text/x-ereader-document</Format>
        <Identifier>urn:uuid:dde46e5c-c8a0-11da-9cba-001422726cc4</Identifier>
        <Language>dut/nla</Language>
        <Type>irx</Type>
    </dc-metadata>
    <y-metadata>
        <FileSize>000</FileSize>
        <ItemSize>000</ItemSize>
        <startpage>index.html</startpage>
        <image>001/grayimages/page.png</image>
        <version>000</version>
    </y-metadata>
  </metadata>
</package>
*/
int saveManifestHead(erManifest *pErManiHandle,Manifest *pManifest)
{
    ermXmlNewString(pErManiHandle, "/", "package", "");
 
    ermXmlNewString(pErManiHandle, "/package", "metadata", "");
    ermXmlNewString(pErManiHandle, "/package/metadata","dc-metadata", "");
    ermXmlNewString(pErManiHandle, "/package/metadata/dc-metadata", "Title", 
                    pManifest->sScribbleName);
    ermXmlNewString(pErManiHandle, "/package/metadata/dc-metadata", "Description", 
                    pManifest->sScribbleName);
    ermXmlNewString(pErManiHandle, "/package/metadata/dc-metadata", "Type", SCRIB_TYPE);
        
    ermXmlNewString(pErManiHandle, "/package/metadata", "y-metadata", "");
    ermXmlNewString(pErManiHandle, "/package/metadata/y-metadata", 
                    "startpage", "dummy.bmp");    
    return 0;
}
    
/* save as following format:
<package>
    <Scribble>
        <ScribbleName>sScribbleName</ScribbleName>
        <LastOpenPage>1</LastOpenPage>
        <TotalPages>2</TotalPages>
        <ScribPages>
            <Page>
                <ImgFileName>...</ImgFileName>
                <PageName>...</PageName>
            </Page>
            <Page>
                <ImgFileName>...</ImgFileName>
                <PageName>..</PageName>
            </Page>
        </ScribPages>
    </Scribble>
<package>
*/
int fm_saveManifest(Manifest *pManifest,const char* sManifestFile,
                    gboolean isCreatedFromTmpl)
{
    erManifest erManifest;
    if(isCreatedFromTmpl)
    {
        //create new manifest file
        if( RET_OK!=ermXmlCreateFile(sManifestFile, &erManifest))
        {
            SB_ERRORPRINTF("error when create xml\n");
            return -1;
        }
        SB_FMPRINTF("xml created\n"); 
        saveManifestHead(&erManifest,pManifest);   
        ermXmlNewString(&erManifest, "/package", "Scribble", "");
        SB_FMPRINTF("manifest head created\n");     
    }
    else
    {
        //open the manifest and then reset the value
        if( RET_OK!=ermXmlOpenFile(sManifestFile, &erManifest))
        {
            SB_ERRORPRINTF("error when open manifest file\n");
            return -1;
        }
        //empty the existed content
        ermXmlSetString(&erManifest,"/package/Scribble","");
    }
        
    ermXmlNewString(&erManifest, "/package/Scribble", "ScribbleName", 
                     pManifest->sScribbleName);
    ermXmlNewInt(&erManifest, "/package/Scribble", "LastOpenPage", 
                  pManifest->iLastOpenPage);
    ermXmlNewInt(&erManifest, "/package/Scribble", "TotalPages", 
                  pManifest->iTotalPages);        
    ermXmlNewString(&erManifest, "/package/Scribble", "ScribPages", "");
    
    PScribPage  pCurrPage=pManifest->pFirstScribPage;
    
    int i = 0;
    char xpathname[256] ={0};
    while(pCurrPage)
    {
        ermXmlNewString(&erManifest, "/package/Scribble/ScribPages",
                         "Page","");
        snprintf(xpathname, MAX_PATHNAME, 
                 "/package/Scribble/ScribPages/Page[%d]", ++i);
        ermXmlNewString(&erManifest, xpathname,
                         "ImgFileName",pCurrPage->sImgFileName);
        ermXmlNewString(&erManifest, xpathname,
                         "PageName",pCurrPage->sPageName);
        pCurrPage=pCurrPage->pNextPage;
    }
    ermXmlSaveAndClose(&erManifest);
    SB_FMPRINTF("manifest saved\n");
    return 0;
}

//set irex type to .irx and then save it.
int fm_setIrxTypeAndSave(PFileManager pFM)
{
    erManifest erManifest;
    //open the manifest and then reset the value
    if( RET_OK!=ermXmlOpenFile(pFM->sManifestPathName, &erManifest))
    {
            SB_ERRORPRINTF("error when open manifest file\n");
            return -1;
    }
    //change the type to irx     
    ermXmlSetString(&erManifest,"/package/metadata/dc-metadata/Type",
                    SCRIB_TYPE);
    //save the manifest
    if( RET_OK!=ermXmlSaveAndClose(&erManifest) )
    {
        SB_ERRORPRINTF("update manifest error[%s]\n",pFM->sManifestPathName);
        return -1;
    }
    return 0;
}

int fm_saveFileManager(PFileManager pFM)
{
    int ret;

    SB_FMPRINTF("\n\nstart save...[curropen=%d,lastopen=%d]\n",
                pFM->iCurrOpenPage,pFM->manifest.iLastOpenPage);
    //if the last openpage is changed ,then save.
    if(pFM->iCurrOpenPage!=pFM->manifest.iLastOpenPage)
    {
        pFM->manifest.iLastOpenPage=pFM->iCurrOpenPage;
        pFM->bNeedSaveManifest=TRUE;
    }
    //bNeedCreatDirHirachy is true means nothing in current foder.
    if(!pFM->bNeedSaveManifest || pFM->bNeedCreatDirHirachy)
    {
        SB_FMPRINTF("manifest need no save\n");
        return 0;
    }
    ret = fm_saveManifest( &pFM->manifest,pFM->sManifestPathName,
                             pFM->isCreatedFromTmpl);
    if ( (0 == ret) && (TRUE == pFM->isCreatedFromTmpl) )
    {
        pFM->isCreatedFromTmpl = FALSE;
    }
    return ret;
}

//load manifest from SCF path.
int loadManifest(PFileManager pFM)
{
    erManifest manifest;
    if( RET_OK!=ermXmlOpenFile(pFM->sManifestPathName, &manifest) )
    {
        SB_ERRORPRINTF("open xml [%s] fail\n",pFM->sManifestPathName);
        return -1;
    }

    Manifest *pManifest=&pFM->manifest;
    
    ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Type",  
                     pFM->manifest.sInkType, MAX_INKTYPE_LEN);    
    ermXmlGetString(&manifest, "/package/Scribble/ScribbleName", 
                     pManifest->sScribbleName, MAX_FILENAME+1); 
    ermXmlGetInt(&manifest, "/package/Scribble/LastOpenPage", 
                  &pManifest->iLastOpenPage); 
    ermXmlGetInt(&manifest, "/package/Scribble/TotalPages", 
                  &pManifest->iTotalPages);  
    
    ScribPage *pLastPage=NULL;
    ScribPage *pScribPage=NULL;
    int i = 0;
    char xpathname[256] ={0};        
    //fetch every page
    for( i=0;i<pManifest->iTotalPages;i++ )
    {
        pScribPage=(ScribPage *)malloc(sizeof(ScribPage));
        if(  NULL==pScribPage)
        {
            SB_ERRORPRINTF("error when malloc space\n");
            return -1;
        }
        memset(pScribPage,0,sizeof(ScribPage));
        snprintf(xpathname, 256, 
                 "/package/Scribble/ScribPages/Page[%d]/ImgFileName",i+1);
        if(    RET_OK != ermXmlGetString(&manifest,xpathname, 
               pScribPage->sImgFileName,MAX_FILENAME+1)
               || isStrEmpty(pScribPage->sImgFileName) )
        {
            free(pScribPage);
            break;
        }
        
        snprintf(xpathname, 256, 
                 "/package/Scribble/ScribPages/Page[%d]/PageName",i+1);
        if(    RET_OK != ermXmlGetString(&manifest,xpathname, 
               pScribPage->sPageName,MAX_FILENAME+1)
               || isStrEmpty(pScribPage->sPageName) )
        {
            free(pScribPage);
            break;
        }
        if( 0==i )
        {   //first page
            pManifest->pFirstScribPage=pScribPage;
        }
        else
        {
            pLastPage->pNextPage=pScribPage;
        }
        pScribPage->bDirty=FALSE;
        pLastPage=pScribPage;
    }//end for loop
    //correct it if error totalpages    
       
    if( pManifest->iTotalPages != i  )
    {
        SB_WARNPRINTF("real pages=%d[in xml:%d]\n",i,pManifest->iTotalPages);
        pManifest->iTotalPages=i;
    }
    ermXmlClose(&manifest);
    
    if( 0==pManifest->iTotalPages  )
    {
        SB_ERRORPRINTF("no valid page!\n");
        return -1;
    }
    SB_FMPRINTF("manifest loaded success\n");
    //dumpManifest(pManifest);
    return 0;
}

PtrInk fm_loadIrexInk(erManifest *pIrxHandle,const PScribPage pScribPage)
{
    //assert(pIrxHandle!=NULL);
    if( NULL==pScribPage->pink )
    {   
        if( NULL==pIrxHandle)
        {
            pScribPage->pink=construct_ink ();
        }
        else
        {
            pScribPage->pink=xml_read_ink(pIrxHandle,pScribPage->sPageName);
        }
    }
    return pScribPage->pink;
}

PtrInk fm_loadCurrInk(PFileManager pFM)
{
    SB_FMPRINTF("entry");
    return fm_loadIrexInk(pFM->irxHandle,getCurrScribPage(pFM));
}

//init the handle to .irx inkfile. 
//isOpen--if it's not open, we need write the header.
int fm_initIrxHandle(PFileManager pFM,gboolean isOpen)
{
    char inkFileName[MAX_FILENAME];
    get_ink_filename (inkFileName, MAX_FILENAME,pFM);
    if( NULL==pFM->irxHandle)
    {
        pFM->irxHandle=(erManifest*)malloc(sizeof(erManifest));
    }
    gboolean bNeedCreate=TRUE ;
    if( isOpen )
    {
        if( FileExist(inkFileName) )
        {
            if( RET_OK!=ermXmlOpenFile(inkFileName, pFM->irxHandle) )
            {
                SB_ERRORPRINTF("open xml [%s] fail\n",inkFileName);
                return -1;
            }
            bNeedCreate=FALSE;
        }
    }
    if(bNeedCreate)
    {
        if( RET_OK!=ermXmlCreateFile(inkFileName,pFM->irxHandle) )
        {
            SB_ERRORPRINTF("create xml [%s] fail\n",inkFileName);
            return -1;
        }
        xml_write_irxHead(pFM->irxHandle);
    }
    SB_FMPRINTF("init [%s] successfully[open?=%d]\n",inkFileName,isOpen);
    return 0;
}

//save one page
int fm_deleteOnePage(PFileManager pFM,PScribPage pCurrPage)
{
    //loop save every pages.
    SB_FMPRINTF("-- delete page=%s",pCurrPage->sPageName);
    //ignore currently.if(pCurrScribPage->bDirty)
    xml_write_ink(pFM->irxHandle,"/notes/pages",
                   NULL,pCurrPage->sPageName);
    fm_setIrxInkDirty(pFM,TRUE);
    
    setCurrentPageDirty(pFM,FALSE);
    SB_TIMEDISPLAY("--done--");
    return 0; 
}

void dumpIrxXml(const char* sPageName,erManifest *pXmlHandle,int i)
{
    char sFileName[512];
    snprintf(sFileName,512,"/dump_%d.xml",i);
    FILE* file=fopen(sFileName,"w+");
    if(file==NULL)
    {
        printf(" error when open %s\n",sFileName);
        return;
    }
    xmlDocDump(file,pXmlHandle->pDoc);
    fclose(file);
}

//save one page
int fm_saveOnePage(PFileManager pFM,PScribPage pCurrPage)
{
    if(!pCurrPage->bDirty)
        return 0;
    
    //loop save every pages.
    SB_TIMEDISPLAY("-- write page=%s",pCurrPage->sPageName);
    //ignore currently.if(pCurrScribPage->bDirty)
    xml_write_ink(pFM->irxHandle,"/notes/pages",
                       pCurrPage->pink,pCurrPage->sPageName);
    fm_setIrxInkDirty(pFM,TRUE);
    
    setCurrentPageDirty(pFM,FALSE);
    SB_TIMEDISPLAY("write irx inkfile finished");
    return 0; 
}

//save current page
int fm_saveCurrPage(PFileManager pFM)
{
    return fm_saveOnePage(pFM,getCurrScribPage(pFM));
}


//save all current pages to the inkfile.
//all the pink has been loaded to manifest.
int fm_saveAllPages(PFileManager pFM)
{
    erManifest irxHandle;
    
    char inkFileName[MAX_FILENAME];
    get_ink_filename (inkFileName, MAX_FILENAME,pFM);
    if( RET_OK!=ermXmlCreateFile(inkFileName, &irxHandle) )
    {
        SB_ERRORPRINTF("open irx xml [%s] fail (for save)\n",inkFileName);
        return -1;
    }
    SB_TIMEDISPLAY("-- write head to \n[%s]",inkFileName);
    xml_write_irxHead(&irxHandle);
    //loop save every pages.
    PScribPage pCurrPage=pFM->manifest.pFirstScribPage;
    while(pCurrPage)
    {
        SB_FMPRINTF("-- write page=%s",pCurrPage->sPageName);
        //ignore currently.if(pCurrScribPage->bDirty)
        xml_write_ink(&irxHandle,"/notes/pages",
                      pCurrPage->pink,pCurrPage->sPageName);
        
        pCurrPage=pCurrPage->pNextPage;
    }
    ermXmlSaveAndClose(&irxHandle);
    SB_TIMEDISPLAY("write irx inkfile finished");
    return 0; 
}

//load all inks to memory in current .irx inkfile.    
int fm_loadAllInks(PFileManager pFM)
{
    if( NULL==pFM->irxHandle ) return -1;
    
    PScribPage pCurrPage=pFM->manifest.pFirstScribPage;
    while(pCurrPage)
    {
        fm_loadIrexInk(pFM->irxHandle,pCurrPage);
        pCurrPage=pCurrPage->pNextPage;
    }
    return 0;
}

// save Irex inks,maybe some ink did not loaded yet
int fm_saveIrexInk(PFileManager pFM, gboolean bClose)
{
    //bNeedCreatDirHirachy is true means nothing in current foder.
    if(!pFM->manifest.bNeedSaveIrx || pFM->bNeedCreatDirHirachy)
    {
        SB_FMPRINTF("need not save .irx inkfile\n");
        if (bClose)
        {
            if( NULL!=pFM->irxHandle )
            {
                ermXmlClose(pFM->irxHandle);
            }
            pFM->irxHandle=NULL;
        }
        return 0;
    }
    
    if( NULL!=pFM->irxHandle )
    {
        SB_FMPRINTF("saving irx inkfile...\n");
        if (bClose)
        {
            ermXmlSaveAndClose(pFM->irxHandle);
        }
        else
        {
            ermXmlSave(pFM->irxHandle);
        }
    }

    SB_FMPRINTF("done...\n");
    return 0; 
}

