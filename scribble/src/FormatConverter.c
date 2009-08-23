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
#include <strings.h>

#include "FileManager.h"
#include "ScribbleLog.h"
#include "ScribbleUtils.h"

#include "ink_rw.h"


//for old version.
int fc_get_ink_dirname(char *sInkDir, int iSize,const char* sDataDir)
{
    SB_FMPRINTF("__start__\n");
    memset(sInkDir,0,iSize);
    snprintf (sInkDir,iSize-1, "%sink",sDataDir);
    SB_FMPRINTF("__end__\ninkdir=%s\n", sInkDir);
    return 0;
}

//get specified page ink file name,for old version.
int fc_get_ink_filename (char *inkFileName, int iSize,
                          const PFileManager pFM,const PScribPage pScribPage)
{
    //SB_FMPRINTF("__start__\n");    
    if( NULL==pScribPage) return -1;
    
    memset(inkFileName,0,iSize);
    snprintf( inkFileName,iSize-1,
              "%sink/%s.ink",
              pFM->sDataDir,
              pScribPage->sPageName);
    //SB_FMPRINTF("__end__\nresultfile=%s\n", resultFileName);
    return 0;
}

int fc_convertold2irx(PFileManager pFM)
{
    //currently, we can only(also need only) convert notes
    if( 0!=strcasecmp(pFM->manifest.sInkType,"notes") )
    {
        SB_ERRORPRINTF("error:unknow ink type!\n");
        return -1;
    }
    //load everypage and fill in Manifest
    PScribPage pCurrPage=pFM->manifest.pFirstScribPage;
    char sInkFileName[MAX_FILENAME];
    
    while(pCurrPage)
    {
        fc_get_ink_filename(sInkFileName,MAX_FILENAME,pFM,pCurrPage);
        pCurrPage->pink=file_read_ink(sInkFileName);
        if( pCurrPage->pink== NULL)
        {
            SB_FMPRINTF("-- null in page \n%s\n",pCurrPage->sPageName);
        }else
        {
            SB_FMPRINTF("--load strokes=%d\nin [%s]",pCurrPage->pink->nStrokes,pCurrPage->sPageName);
        }
        pCurrPage=pCurrPage->pNextPage;
    }
    SB_FMPRINTF("-- saving the old ink folder");
    //save the ink
    int iRet=fm_saveAllPages(pFM);
    if( 0!= iRet)
    {
        SB_FMPRINTF("-- save pages error\n");
        return -1;
    }

    //when success,deleted the whole ink directory.
    char sInkDir[MAX_PATHNAME];
    fc_get_ink_dirname(sInkDir,sizeof(sInkDir),pFM->sDataDir);
    SB_FMPRINTF("-- deleting folder [%s]", sInkDir);
    char* cmd = g_strdup_printf("rm -rf \"%s\"", sInkDir);
    system(cmd);
    g_free(cmd);
    return 0;

}


   
