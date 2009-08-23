/**
 * \file eripcpowermgr.h
 * \brief liberipc - specifics to IPC with powermgr
 *
 * <File description>
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 Philips Apptech
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
