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
 * \file main.h
 * \brief browser - E-reader browser startup
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __MAIN_H__
#define __MAIN_H__

/**
 * callback : key press handler for the main window
 *
 * @param widget   the GTK widget that received the event
 * @param event    the gtk key event info
 * @param reader   reference to the Ereader structure
 *
 * @returns 0 
 */
guint on_mainWindow_keypress(GtkWidget *widget, GdkEventKey *event, Ereader *reader);

/**
 * GTK & APP startup
 *
 * @param URL location of the first file that needs to be displayed
 *
 * @returns A value of 0 indicates success
 */
//int main(int argc, char **argv);
		 
#endif //__MAIN_H__

