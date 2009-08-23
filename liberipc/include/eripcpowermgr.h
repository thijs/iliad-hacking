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
 * \file eripcpowermgr.h
 * \brief liberipc - specifics to IPC with powermgr
 *
 * <File description>
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#ifndef _ERIPCPOWERMGR_H_
#define _ERIPCPOWERMGR_H_

#include "eripcclient.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
  ccPwrAC97 = 0,
  //Outdated: ccPwrWifi,
  ccPwrIdleMode,
  ccPwrUndefined
} eCcpwr;
   

int pwrSetAC97(erClientChannel_t channel, int enabled);
//Outdated: int pwrSetWifi(erClientChannel_t channel, int enabled);

/**
 * Put cpu in idle mode
 * args: MHz: the desired cpu clock
 *       timeout: the number of seconds to wait before changing the cpu mode
 */
int pwrGotoIdleMode(erClientChannel_t channel, int MHz, int timeout);
 

int pwrParseCommand(char *szCommand, erIpcCmd_t * pCmd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
