/**
 * \file eripceba.h
 * \brief liberipc - specifics to IPC with EBA viewer
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 Philips Apptech
 *
 */

#ifndef _ERIPCEBAVIEWER_H_
#define _ERIPCEBAVIEWER_H_

#include "eripcclient.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//TODO: remove signals not needed for EBA viewer
//TODO: add new signals towards EBA viewer
typedef enum {
    ccEbaJumpToPage = 0,
    ccEbaBookmarkPage,
    ccEbaInvokeFindDialog,
    ccEbaZoomIn,
    ccEbaZoomOut,
    ccEbaStore,		
    ccEbaToolbar,
    ccEbaUndefined
} eCcEba;


int ebaParseCommand(char *szCommand, erIpcCmd_t *pCmd);

/**
 * Jump to page 'page' of the current document displayed in the viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param page page number
 *
 * @returns A value of 0 signals success
 */
int ebaJumpToPage(erClientChannel_t channel, int uaID, int page);

/**
 * Bookmark the currently displayed page in the viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int ebaBookmarkPage(erClientChannel_t channel, int uaID);

/**
 * Invoke the find dialog on viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int ebaInvokeFindDialog(erClientChannel_t channel, int uaID);

/**
 * Zoom in on the currently displayed page of viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int ebaZoomIn(erClientChannel_t channel, int uaID);

/**
 * Zoom out on the currently displayed page of viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int ebaZoomOut(erClientChannel_t channel, int uaID);

/**
 * Zoom out on the currently displayed page of viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int ebaZoomOut(erClientChannel_t channel, int uaID);

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
int ebaStore(erClientChannel_t channel, int uaID, int category, char* itemID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

