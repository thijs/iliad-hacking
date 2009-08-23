#ifndef _ERIPCPARSER_H_
#define _ERIPCPARSER_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "eripcclient.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void erIpcGetArgs(char *pChar, erIpcCmd_t * pCmd, int nReqArgs);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
