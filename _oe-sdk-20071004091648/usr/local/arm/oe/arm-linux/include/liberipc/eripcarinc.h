/**
 * \file eripcviewer.h
 * \brief liberipc - specifics to IPC with viewer
 *
 * <File description>
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 Philips Apptech
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
