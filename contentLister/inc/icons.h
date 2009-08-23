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
 * \file icons.h
 * \brief contentLister - icon handling
 *
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __EREADER_ICONS_H__
#define __EREADER_ICONS_H__

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define LOADED_ICON_MAX_WIDTH	64
#define LOADED_ICON_MAX_HEIGHT	90

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

/**
 * Load the frequently use items
 *
 * @returns void
 */
void icons_init();

/**
 * Destroy loaded icons
 *
 * @returns void
 */
void icons_destroy();

/**
 * request from contentlister
 *
 * @param Icon_ID - the icon identification
 *
 * @returns READ ONLY reference to icon pixbuf
 */
GdkPixbuf* icons_get(unsigned int Icon_ID);

/**
 * Load the icon image 
 *
 * @param location - icon file location
 *
 * @returns NULL in case of error
 */
GdkPixbuf* icons_load(const char* location);

/**
 * Unload the icon image 
 *
 * @param image - pointer
 *
 * @returns 
 */
void icons_unload(GdkPixbuf* image);

#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__EREADER_ICONS_H__




