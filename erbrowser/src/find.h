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
 * \file find.h
 * \brief browser - find dialog and Mozilla find functionality
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __EREADER_FIND_H__
#define __EREADER_FIND_H__

// used to enable/disable the find dialog 
//#define FIND_DIALOG_ENABLED

#define FIND_DIALOG_WIDTH	(SCREEN_WIDTH/2)
#define FIND_DIALOG_HEIGHT	(SCREEN_HEIGHT/4)

#ifdef FIND_DIALOG_ENABLED

/**
 *  create the "find" dialog
 *
 * @param Ereader datastructure containing the "mozembed" and the main window reference
 *
 * @returns void
 */
void browser_find_dialog_create(Ereader* theEreader);

#endif //FIND_DIALOG_ENABLED

#endif //__EREADER_FIND_H__




