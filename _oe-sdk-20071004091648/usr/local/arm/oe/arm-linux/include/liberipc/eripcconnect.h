/**
 * \file eripcconnect.h
 * \brief liberipc - specifics to IPC with connectionMgr
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 Philips Apptech
 *
 */

#ifndef _ERIPCCONNECT_H_
#define _ERIPCCONNECT_H_

#include "eripcclient.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum
        {
            ccCnGotoPage = 0,
            ccCnToolbar,
            ccCnToolbarSync,
            ccCnUndefined
        } eCcCn;


int cnParseCommand(char *szCommand, erIpcCmd_t *pCmd);

/**
 * Sends a signal to the connectionMgr application
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param signal signal
 *
 * @returns A value of 0 signals success
 */
int cnSignal(erClientChannel_t channel, int uaID, int signal);

/**
 * Jump to page 'page' of the current document displayed in the viewer 'uaID'
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param page page number
 *
 * @returns A value of 0 signals success
 */
int cnGotoPage(erClientChannel_t channel, int page);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ERIPCCONNECT_H */

