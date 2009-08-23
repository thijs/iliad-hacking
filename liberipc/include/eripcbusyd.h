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
 * \file eripcbusyd.h
 * \brief liberipc - specifics to IPC with busyd
 *
 * <File description>
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#ifndef _ERIPCBUSYD_H_
#define _ERIPCBUSYD_H_

#include "eripcclient.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
	ccBusySetBusy = 0,
	ccBusyUndefined
} eCcBusy;

typedef enum {
    ccBusyState_Off = 0,
    ccBusyState_Blink,
    ccBusyState_On,
    ccBusyState_Shutdown,
    ccBusyState_Undefined
} eCcBusyState;

int busyParseCommand(char *szCommand, erIpcCmd_t *pCmd);

int busySetBusy(erClientChannel_t channel, eCcBusyState busy);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
