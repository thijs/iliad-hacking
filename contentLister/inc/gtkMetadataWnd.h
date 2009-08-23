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
 * \file gtkMetadataWnd.h
 * \brief gtkMetadataWnd - eReader content metadata editor
 * 
 * <File description>
 *  The eReader content customization window provides gtk widget to enable user customize
 *  content title and description. Which are created and updated with the following API
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __GTKMETADATAWND_H_
#define __GTKMETADATAWND_H_

//////////////////////////////////////////////////////////////////////////
// content metadata editor window
//////////////////////////////////////////////////////////////////////////

#include <gtk/gtk.h>

#define METADATA_BK_IMG_WIDTH       666
#define METADATA_BK_IMG_HEIGHT      345

#define METADATA_WND_SPACE_ABOVE    50
#define METADATA_WND_ITEM_SPACING   8

#define METADATA_VBORDER            10
#define METADATA_HBORDER            20
#define METADATA_ENTRY_WIDTH        (METADATA_BK_IMG_WIDTH - 2*METADATA_HBORDER)

#define METADATA_TAG_HEIGHT         40
#define METADATA_NAME_HEIGHT        30
#define METADATA_DESC_HEIGHT        30

// copy from other header files, in order to use this independently
#define MD_MAX_TITLE_SIZE		    256
#define MD_MAX_SUBTITLE_SIZE	    256
#define MD_MAX_DESCRIPTION_SIZE	    512


#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */


/* define gtk widget */
typedef struct
{
    GtkWidget*      window;      // parent window, specified by other module
    GtkWidget*      nameTag;     // title and name on disk
    GtkWidget*      nameEntry;   //
    GtkWidget*      descTag;     // description
    GtkWidget*      descEntry;   //
    GtkWidget*      descEntry2;  //
    GtkWidget*      finishTag;   // finish text
    gboolean        on_change_occurred;
    gboolean        on_movecursor_occurred;
    gboolean        display_update_pending;
} cmMetadata_t;


/**
 * Create content metadata editor window
 *
 * @param data -
 *
 * @returns instance pointer of window. Return NULL if failed.
 */
GtkWidget* create_content_metadata_wnd(GtkWidget * parent);

/**
 * Set constant texts for metedata editor window
 */
void content_metadata_set_text(void);

/**
 * read manifest file into content data
 *
 * @param pathname of the manifest file
 *
 */
gboolean content_metadata_read_manifest(const gchar* szManifest);

/**
 * rename container when needed, depending on current title
 *
 * @param in  szManifest pathname of manifest file
 * @param out szContainer pathname of container directory
 * @param in  len size in bytes of szContainer buffer
 */
gboolean content_metadata_rename_container(const gchar* szManifest, gchar* szContainer, gint len);

/**
 * update the content metadata according to input parameter
 *
 * @param pathname of container directory
 *
 */
gboolean content_metadata_write_manifest(const gchar* szContainer);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  //__GTKMETADATAWND_H_

