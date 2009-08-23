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
 * \file eripcconnect.h
 * \brief liberipc - specifics to IPC with connectionMgr
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 iRex Technologies B.V.
 * All rights reserved.
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

