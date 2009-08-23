 /**
 * \file eripcbusyd.h
 * \brief liberipc - specifics to IPC with busyd
 *
 * <File description>
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 Philips Apptech
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
