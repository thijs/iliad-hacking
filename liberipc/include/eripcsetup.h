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
 * \file eripcsetup.h
 * \brief liberipc - specifics to IPC with setup application
 *
 * <File description>
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 iRex Technologies B.V.
 * All rights reserved.
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
