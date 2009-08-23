/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file browserTypes.h
 * \brief browser - eReader browser specific structures
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __BROWSER_TYPES_H__
#define __BROWSER_TYPES_H__

// general includes
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "gtkmozembed.h"
#include "indexFileHandler.h"

// Used to keep track of the page status when working with a manifest file 
typedef struct _BrowserPageStatus 
{
//  FILE*	 indexFile;	  				   
  IndexInfo index;  	   
  char*  currentType;
  char*	 manifestFile;
  int    currentPage;
  int    pageCount;
} BrowserPageStatus;

// Used to pass browser application references/data
// through the whole program.  
typedef struct _Ereader
{
	GtkWidget	*topLevelWindow;
	GtkWidget	*topLevelVBox;
	GtkWidget   *eventBox; //addded to have an actual X-window 
	GtkWidget   *mozEmbed; 
// 	GtkWidget	*thepagebar;
	GtkWidget	*dialog; // Find dialog tryout (currently - 20051227 - not used any more)
#ifdef LOCAL_DISPLAY	
	GtkWidget	*toolbar; //Put empty toolbar on the screen - for layout reasons - when working on PC witout Xnest
#endif	//LOCAL_DISPLAY
	BrowserPageStatus *pageStatus;
} Ereader;

#endif // __BROWSER_TYPES_H__
