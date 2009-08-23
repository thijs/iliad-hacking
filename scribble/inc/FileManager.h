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

//
// C Interface: FileManager
//
// Description: 
//It response for generating the directory structure and filename 
//for saving/loading scribble.  the upper package need no 
//knowledge about the directory information and the filenames 
//rule for scribble, so the manifest file is maintained by //filemanager.  but because some file save/load is invoking the 
//lib function of gtk(such as gdk_pixbuf_new_from_file), so this //package does not  elimilate all the  requirement for file 
//operation.
//
//
// Author: jianshen , (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

#define MAX_PATHNAME 512
#define MAX_FILENAME 512

#ifdef __cplusplus
extern "C"
{
#endif

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#include <assert.h>

#include "ink.h"

#include <libermanifest/ermanifest.h>

#define MAX_INKTYPE_LEN 10

#define SCRIB_TYPE "irx"

  //for scribble image of every page
  typedef struct _ScribPage
  {
      char sImgFileName[MAX_FILENAME];
      //the name of ink and result,but ink and result image can be empty.
      char sPageName[MAX_FILENAME];
      //ink data for this page.
      PtrInk pink;
      //dirty flag for current page.
      gboolean bDirty;
      
      struct _ScribPage* pNextPage;
  }ScribPage;

  typedef ScribPage  *PScribPage;

  typedef struct _Manifest
  {
      //it's formal as "template_yymmddhhmmss",but never duplicated.
      char sScribbleName[MAX_FILENAME];//for sScribbleName 
      int iLastOpenPage;//numbered from 1.
      int iTotalPages;
      //only "notes" or "irx"
      char sInkType[MAX_INKTYPE_LEN];
      
      gboolean bNeedSaveIrx;      
      PScribPage  pFirstScribPage;
  } Manifest;

  typedef struct _FileManager
  {
      Manifest manifest;
      //handle the .irx inkfile.
      erManifest* irxHandle;
      
      PScribPage pCurrPage;//current PScribPage
      int iCurrOpenPage;
      int bNeedSaveManifest;//need save manifest?
      int bNeedCreatDirHirachy;//need create the dir hirachy?
      
      //current template full path and name
      char sTmplPathName[MAX_FILENAME];
      //always is "sCribName/manifest.xml",withpath
      char sManifestPathName[MAX_FILENAME];
      //directory for scrib data,always,in formal:xxx.SCF
      char sDataDir[MAX_PATHNAME];
      //whether or not the current scribble created from template
      gboolean isCreatedFromTmpl;
  } FileManager;

  typedef FileManager *PFileManager;

  //creat directrory hirachy by toplevel data dir if necessary.
  extern int createDirHirachy(PFileManager pFM);
  
  //init filemanager with template file
  extern int 
  initFileManagerByTmpl(PFileManager pFM,const char* sTmplPathName);

  extern int 
  initFileManagerByManifest(PFileManager pFM,const char* sManifest);

  //get current ScribPage
  extern PScribPage getCurrScribPage(PFileManager pFM);
  
  extern void freeScribPage(PScribPage pPage);
  
  //get Image File for current page.
  extern const char* getCurrImgFileName(PFileManager pFM);
  
  //jumpto specified pageno.
  extern PScribPage jumptoScribPage(PFileManager pFM,int iPageNo);

  //load manifest from SCF path.
  extern int loadManifest(PFileManager pFM);

  //insert new page at specified page no.
  //sImgName--if it's null/empty,then set it to the default.
  extern int insertNewPage(PFileManager pFM,int iPageNo,const char* sImgName);

  extern int deleteCurrPage(PFileManager pFM);

  //get current opened page.
  extern int getTotalPage(PFileManager pFM);
  
  extern int getCurrPageNo(PFileManager pFM);

  extern int fm_saveFileManager(PFileManager pFM);
  
  //free the file manager
  extern void FreeFileManager(PFileManager pFM);
  
  //get absolute path (eg.emplate image) from current scribble page.
  char* getAbsPathFromCurrScrib(char* sAbsFileName,
                                int iSize,
                                const char* currFileName,
                                const PFileManager pFM);
  //get .irx ink filename.
  extern int get_ink_filename (char *inkFileName, int iSize,
                               const PFileManager pFM);
  

  //get specified page result file name
  extern int get_result_filename (char *resultFileName, 
                                  int iSize,
                                  const PFileManager pFM,
                                  const PScribPage pScribPage);
  
  //get current result file name from current open page
  extern int get_curr_result_filename(char *resultFileName, 
                                      int iSize,
                                      const PFileManager pFM);

  extern void fm_setIrxInkDirty(PFileManager pFM,gboolean bDirty);
  
  extern PtrInk fm_loadCurrInk(PFileManager pFM);
  
  //init the handle to .irx inkfile.
  extern int fm_initIrxHandle(PFileManager pFM,gboolean isOpen);
  
  //delete one page to .irx
  int fm_deleteOnePage(PFileManager pFM,PScribPage pPage);

  //save one page
  int fm_saveOnePage(PFileManager pFM,PScribPage pPage);

  //save current page
  int fm_saveCurrPage(PFileManager pFM);
  
  //save all current pages to the inkfile.
  //all the pink has been loaded to manifest.
  extern int fm_saveAllPages(PFileManager pFM);
  
  extern int fm_saveIrexInk(PFileManager pFM, gboolean bClose);
   
  //set irex type to .irx and then save it.
  extern int fm_setIrxTypeAndSave(PFileManager pFM);
  
  extern int fc_convertold2irx(PFileManager pFM);
  
  //for debug purpose
  extern void dumpManifest(Manifest *pM);
  extern void dumpFileManager(PFileManager pFM);
  void dumpIrxXml(const char* sPageName,
                  erManifest *pXmlHandle,int i);
#ifdef __cplusplus
}
#endif
   
#endif
