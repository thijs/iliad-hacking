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
*  C Header: ink_rw.h
*
* Description: 
* brief- the reader/writer API for ink data structure.
*
* Author: Jian Shen, (C) 2006
*
* Copyright: See COPYING file that comes with this distribution
*
*/

#ifndef _INK_RW_H
#define _INK_RW_H

#include <libermanifest/ermanifest.h>

#include "ink.h"

#ifdef __cplusplus
extern "C"
{
#endif

//read ink object from ink file.
extern PtrInk file_read_ink(const char *inkfile);

//write ink object to ink file
extern unsigned char file_write_ink (PtrInk ink, const char *inkfile);


/* Reference .irx fomat

<?xml version="1.0" ?>
<version>
  <number>1.0</number>
  <organization>iRex Technologies</organization>
</version>
<screen>
  <units>px</units>
  <dpi>160</dpi>
</screen>
<pages>
  <page backgroundcolor="#FFFFFF" id="0">
    <orientation>0</orientation>
    <width>768</width>
    <height>935</height>
    <strokes>
      <stroke layer="0" color="#000000" pensize="3"
               linestyle="solid">
        000
        10 3 0
      </stroke>
      <stroke layer="0" color="#000000" pensize="1"
               linestyle="solid">
        20 25 0
        30 30 0
      </stroke>
    </strokes>
  </page>
</pages>
*/

// read ink object from xml handle.the new ink should be free after use.
// if successed return 1, else return 0
extern PtrInk xml_read_ink(erManifest *pXmlHandle,const char* sPageName);

//write header information to .irx
extern int xml_write_irxHead(erManifest *pErManiHandle);

// write one page to xml. under xpath.(end without'/')
extern gboolean xml_write_ink (erManifest *pXmlHandle,const char* xpath,
                               const PtrInk pink,const char* sPageName);
  
#ifdef __cplusplus
}
#endif

#endif
