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
 * \file eripcviewer.h
 * \brief liberipc - specifics to IPC with viewer
 *
 * <File description>
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#ifndef _ERIPC_ARINCVIEWER_H_
#define _ERIPC_ARINCVIEWER_H_

#include "eripcclient.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    ccArincJumpToPage = 0,
    ccArincBookmarkPage,
    ccArincInvokeFindDialog,
    ccArincZoomIn,
    ccArincZoomOut,
    ccArincStore,        
    ccArincSignal,
    ccArincUndefined
} eCcArinc;


int arincParseCommand(char *szCommand, erIpcCmd_t *pCmd);

/**
 * Sends a signal to a viewer
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param signal signal
 *
 * @returns A value of 0 signals success
 */
int arincSignal(erClientChannel_t channel, int uaID, int signal);

/**
 * Jump to page 'page' of the current document displayed in the viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param page page number
 *
 * @returns A value of 0 signals success
 */
int arincJumpToPage(erClientChannel_t channel, int uaID, int page);

/**
 * Bookmark the currently displayed page in the viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int arincBookmarkPage(erClientChannel_t channel, int uaID);

/**
 * Invoke the find dialog on viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int arincInvokeFindDialog(erClientChannel_t channel, int uaID);

/**
 * Zoom in on the currently displayed page of viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int arincZoomIn(erClientChannel_t channel, int uaID);

/**
 * Zoom out on the currently displayed page of viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int arincZoomOut(erClientChannel_t channel, int uaID);

/**
 * Zoom out on the currently displayed page of viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int arincZoomOut(erClientChannel_t channel, int uaID);

/**
 * Store the currently displayed page of viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param category identifier
 * @param itemID - null terminated string - unique item ID of the currently displayed content
 *
 * @returns A value of 0 signals success
 */
int arincStore(erClientChannel_t channel, int uaID, int category, char* itemID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
