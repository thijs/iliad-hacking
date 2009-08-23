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
#ifndef _FILEMGR_PAGE_H
#define _FILEMGR_PAGE_H


#ifdef __cplusplus
extern "C"
{
#endif


int getUniqNameForSCRIB(PFileManager pFM);

void setCurrentPageDirty(PFileManager pFM,gboolean bDirty);

int deleteRelatedFiles(PFileManager pFM,ScribPage *pScribPage);

int insertNewPageAfterCurr(PFileManager pFM);


#ifdef __cplusplus
}
#endif
   
#endif
