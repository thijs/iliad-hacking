/**
 * \file eripcsetup.h
 * \brief liberipc - specifics to IPC with setup application
 *
 * <File description>
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 Philips Apptech
 *
 */

#ifndef _ERIPCSETUP_H_
#define _ERIPCSETUP_H_

#include "eripcclient.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    ccStGotoPage = 0,
    ccStUndefined
} eCcSt;


int stParseCommand(char *szCommand, erIpcCmd_t *pCmd);

/**
 * Go to page 'page' of the current document displayed in the viewer 'uaID'
 *
 * @param channel: Opaque datastructure that contains info on communication channel
 * @param page: page number
 *
 * @returns A value of 0 signals success
 */
int stGotoPage(erClientChannel_t channel, int page);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
