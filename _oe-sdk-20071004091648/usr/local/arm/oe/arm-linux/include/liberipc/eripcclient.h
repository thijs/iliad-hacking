#ifndef _ERIPCCLIENT_H_
#define _ERIPCCLIENT_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "eripc.h"

#ifdef __cplusplus
extern  "C"
{
#endif                          /* __cplusplus */

#define TB_MAXCHARONLINE 1024

#define TB_CMD_NAME 128
#define TB_ARGLENGTH 256
#define TB_N_ARG       6

typedef struct
{
    char    name[TB_CMD_NAME]; // Command Name
    int     cc;             // Command Code
    char    arg[TB_N_ARG][TB_ARGLENGTH];
    int     nArg;
} erIpcCmd_t;

int     erIpcSndMessage(erClientChannel_t channel, char *szMsg);

int     erIpcSndCommand(erClientChannel_t channel, erIpcCmd_t * cmd);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif
