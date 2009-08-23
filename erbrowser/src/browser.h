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
 * \file browser.h
 * \brief browser - eReader "GtkMozEmbed" widget creation, signal handlers initialistation
 *                  and browser related API's                 
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __EREADER_BROWSER_H__
#define __EREADER_BROWSER_H__

/**
 * Allocate an Ereader data  structure, creates the ereader browser specific GTK widgets
 * and initializes the embedded browser window 
 *
 * @param Manifest File location 
 * @param URL location of the first file that needs to be displayed
 *
 * @returns reference to Ereader structure indicates success
 */
Ereader* new_gtk_ereader(char* manifest, char* input_URL);
 
 /**
 * clear resources used by the Ereader data  structure
 *
 * @param theEreader reference to the eReader struct
 *
 * @returns void
 */
void destroy_ereader(Ereader* theEreader);

 /**
 * show/hide the Mozembed widget
 *
 * @param theEreader reference to the eReader struct
 * @param visibility
 *
 * @returns void
 */
void set_browser_visibility(Ereader *browser, gboolean visibility);

void browser_quit();
 
#endif //__EREADER_BROWSER_H__



