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
 * \file toolbar.h
 * \brief browser - toolbar interaction with browser application
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __EREADER_TOOLBAR_H__
#define __EREADER_TOOLBAR_H__

#define MAX_ZOOM_IN	(2.0)
#define MAX_ZOOM_OUT	(0.6)
#define ZOOM_STEPSIZE   (0.1)


/** Outdated: replaced by e_iconID_t in eripctoolbar.h **
typedef enum {
	sigVwrIgnoreThisCommand  = 0,
	sigVwrBookmarkPage,
	sigVwrInvokeFindDialog,
	sigVwrZoomIn,
	sigVwrZoomOut,
	sigVwrStoreSelection,
	sigVwrUndefined
} eSigVwr;
** Outdated **/

/**
 * A key is selected on the toolbar
 *
 * @param command the command associated with the selected key
 * @param Ereader datastructure containing the "mozembed" reference
 *
 * @returns -
 */
void toolbar_key_selected(int iconID, int iconState, Ereader *browser);

/**
 * Init toolbar (open communication channel and enable/set the correct icons)
 *
 * @returns void
 */
void toolbar_init(void);

#endif //__EREADER_TOOLBAR_H__




