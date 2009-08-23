/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file control_share.h
 * \brief control - handle input and keep state                  
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __CONTROL_SHARE__H__
#define __CONTROL_SHARE__H__


#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */


#define XMLNODE_STARTPAGE                   "/package/metadata/y-metadata/startpage"
#define XMLNODE_DOCUMENT_SIGNING_PARENT     "/package/metadata/y-metadata"
#define XMLNODE_DOCUMENT_SIGNING_NODENAME   "document-signing"
#define XMLNODE_DOCUMENT_SIGNING            XMLNODE_DOCUMENT_SIGNING_PARENT "/" XMLNODE_DOCUMENT_SIGNING_NODENAME

#define TARFILE_EXTENSION                   ".tar.gz"
#define SIGNATURE_EXTENSION                 ".tar.gz.sha1"


// Share the specified content item
void ctrl_listItem_share(const int* index_tbl, ContentLister* theContentLister);
void ctrl_doc_share_now(ContentLister* theContentLister);

// Close selection-list window
void ctrl_hide_sel_list_wnd(ContentLister* theContentLister, gboolean bSave);


#ifdef __cplusplus
}
#endif  //* __cplusplus */


#endif  // __CONTROL_SHARE__H__

