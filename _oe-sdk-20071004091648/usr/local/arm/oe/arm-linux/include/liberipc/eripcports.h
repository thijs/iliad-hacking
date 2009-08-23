/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 Philips Apptech
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
