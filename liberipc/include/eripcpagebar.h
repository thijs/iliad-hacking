/*
 * This file is part of liberipc.
 *
 * liberipc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberipc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file eripcpagebar.h
 * \brief liberipc - specifics to IPC with pagebar
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#ifndef _ERIPCPAGEBAR_H_
#define _ERIPCPAGEBAR_H_

#include "eripcclient.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
	ccPbSetPageCount = 0,
	ccPbSetCurrentPage,
	ccPbSetCurrentPageOffset,
	ccPbSetZoomMode,
	ccPbSetDrawAreaOrientation,
	ccPbSetDrawAreaHOrigin,
	ccPbSetDrawAreaVOrigin,
	ccPbSetDrawAreaHSize,
	ccPbSetDrawAreaVSize,
	ccPbSetBarFontType,
	ccPbSetBarFontHeight,
	ccPbAddBookmark,
	ccPbRemoveBookmark,
	ccPbSetBookmarkMax,
	ccPbAddNote,
	ccPbRemoveNote,
	ccPbSetNotesMax,
	ccPbReset,
	ccPbShow,
	ccPbRedraw,
	ccPbSynchronise,
	ccPbUndefined
} eCcPb;

/**
 * Sets the number of pages in the pagebar
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param pagecount Number of pages
 *
 * @returns A value of 0 signals success
 */
int pbSetPageCount(erClientChannel_t channel, int uaID, int pagecount);

/**
 * Sets the current page
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param page The new current page
 *
 * @returns A value of 0 signals success
 */
int pbSetCurrentPage(erClientChannel_t channel, int uaID, int page);

/**
 * Sets the current pageoffset
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param pageoffset The new current pageoffset expressed in percentage
 *
 * @returns A value of 0 signals success
 */
int pbSetCurrentPageOffset(erClientChannel_t channel, int uaID, int pageoffset);

/**
 * Sets the zoom mode
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param zoomMode The zoom Mode: 0 means overview mode, 1 means zoomed mode
 *
 * @returns A value of 0 signals success
 */
int pbSetZoomMode(erClientChannel_t channel, int uaID, int zoomMode);

/**
 * Sets the orientation of the pagebar
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param orientation The new orientation of the pagebar
 *
 * @returns A value of 0 signals success
 */
int pbSetDrawAreaOrientation(erClientChannel_t channel, int uaID, int orientation);

/**
 * Sets the horizontal origin of the bar.
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param origin The horizontal origin expressed in pixels
 *
 * @returns A value of 0 signals success
 */
int pbSetDrawAreaHOrigin(erClientChannel_t channel, int uaID, int origin);

/**
 * Sets the vertical origin of the bar.
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param origin The vertical origin expressed in pixels
 *
 * @returns A value of 0 signals success
 */
int pbSetDrawAreaVOrigin(erClientChannel_t channel, int uaID, int origin);

/**
 * Sets the horizontal size of the bar.
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param size The horizontal size expressed in pixels
 *
 * @returns A value of 0 signals success
 */
int pbSetDrawAreaHSize(erClientChannel_t channel, int uaID, int size);

/**
 * Sets the vertical size of the bar.
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param size The vertical size expressed in pixels
 *
 * @returns A value of 0 signals success
 */
int pbSetDrawAreaVSize(erClientChannel_t channel, int uaID, int size);

/**
 * Sets the fonttype used by the pagebar
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param font The type of the font
 *
 * @returns A value of 0 signals success
 */
int pbSetBarFontType(erClientChannel_t channel, int uaID, int font);

/**
 * Sets the fontheight
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param size The height of the font
 *
 * @returns A value of 0 signals success
 */
int pbSetBarFontHeight(erClientChannel_t channel, int uaID, int size);

/**
 * Adds a bookmark to the pagebar
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param value The bookmark value to add
 *
 * @returns A value of 0 signals success
 */
int pbAddBookmark(erClientChannel_t channel, int uaID, int value);

/**
 * Removes a bookmark from the pagebar
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param value The bookmark value to remove
 *
 * @returns A value of 0 signals success
 */
int pbRemoveBookmark(erClientChannel_t channel, int uaID, int value);

/**
 * Sets the maximum value of the bookmark
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param max The maximum value
 *
 * @returns A value of 0 signals success
 */
int pbSetBookmarkMax(erClientChannel_t channel, int uaID, int max);

/**
 * Adds a note to the pagebar
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param value The note value to add
 *
 * @returns A value of 0 signals success
 */
int pbAddNote(erClientChannel_t channel, int uaID, int value);

/**
 * Removes a note from the pagebar
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param value The note value to remove
 *
 * @returns A value of 0 signals success
 */
int pbRemoveNote(erClientChannel_t channel, int uaID, int value);

/**
 * Sets the maximum value of the notes
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param max The maximum value
 *
 * @returns A value of 0 signals success
 */
int pbSetNotesMax(erClientChannel_t channel, int uaID, int max);

/**
 * Resets the pagebar to the default settings
 * This function must be called eached time a new application gets / regains the focus
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int pbReset(erClientChannel_t channel, int uaID);

/**
 * Shows the pagebar
 * A call to pbRedraw must be made before the pagebar is shown/hidden on the screen
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param show 0: hide, 1: show
 *
 * @returns A value of 0 signals success
 */
int pbShow(erClientChannel_t channel, int uaID, int show);

/**
 * Redraws the pagebar
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int pbRedraw(erClientChannel_t channel, int uaID);


/**
 * Used to synchronise pagebar. (From application to pagebar)
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int pbSynchronise(erClientChannel_t channel, int uaID);


/**
 * Used to report synchronise event. (From pagebar to application)
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int pbReportSynchronise(erClientChannel_t channel, int uaID);



int pbParseCommand(char *szCommand, erIpcCmd_t * pCmd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
