#ifndef _ERIPCPRIVATE_H_
#define _ERIPCPRIVATE_H_

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

typedef struct erInternalClientChannel_t
{
    int     sockfd;
    struct sockaddr_in serverAdr;
} erInternalClientChannel_t;

typedef struct erInternalServerChannel_t
{
    int     sockfd;
} erInternalServerChannel_t;

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif
