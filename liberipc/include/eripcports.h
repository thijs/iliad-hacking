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

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#ifndef _ERIPCPORTS_H_
#define _ERIPCPORTS_H_

/** Outdated, moved to eripc.h **

// eripcconfig.h is auto generated
#include "eripcconfig.h"
#include "eripc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TB_IPC_PORT     50065
#define TEST_IPC_PORT    50066
#define PDF_VIEWER_IPC_PORT 50067
#define XHTML_VIEWER_IPC_PORT 50068
#define APABI_VIEWER_IPC_PORT 50069
#define CONTENTLISTER_IPC_PORT 50070
#define BUSYD_IPC_PORT 50071
#define POWERMGR_IPC_PORT 50072
#define PAGEBAR_IPC_PORT 50073
#define SCRIBBLE_IPC_PORT 50074
#define SETUP_IPC_PORT 50075
#define CONNECT_IPC_PORT 50076
#define ARINC_VIEWER_IPC_PORT 50077
    
int erIpcGetServerPort(int channelIdx, int *port);

#ifdef __cplusplus
}
#endif

** Outdated, moved to eripc.h **/

#endif
