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

/* liberipc
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#include "eripc.h"
#include "eripcprivate.h"
#include "eripcports.h"
#include "eripcclient.h"
#include "eripclog.h"

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int erIpcGetVersion()
{
    return ER_IPC_VERSION;
}

/*
 * local
 *   When set to '1' the server will only listen to processes running on the same host
 */
int erIpcInitServer(int channelIdx, int *sockfd, int local)
{
    int     error;
    int     port;
    struct sockaddr_in serverAdr;
    int on = 1;

    // test code
    ERIPC_WARNPRINTF("Version: %d\n", erIpcGetVersion());


    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    error = setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on));
    if (error)
    {
        perror("Unable to set socket options to SO_REUSEADDR");
    }

    port = erIpcGetServerPort(channelIdx);
    if (port == ER_IPC_PORT_ILLEGAL)
    {
        ERIPC_ERRORPRINTF("Could not allocate channel %d", channelIdx);
        return -1;
    }

    bzero(&serverAdr, sizeof(serverAdr));
    serverAdr.sin_family = AF_INET;
    // NOTE :
    //   Setting the address to INADDR_LOOPBACK will only allow processes running
    // on the same host to send messages to the displayMgrServer
    //   Setting the address to INADDR_ANY will allow a process on any host to 
    // send messages to the displayMgrServer
    if (local == 1)
    {
        serverAdr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    else
    {
        serverAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    serverAdr.sin_port = htons(port);

    error = bind(*sockfd, (struct sockaddr *) &serverAdr, sizeof(serverAdr));
    if (error)
    {
        perror("Error invoking bind");
        return -1;
    }
    else
    {
        char    szServerIP[INET_ADDRSTRLEN + 1];

        inet_ntop(AF_INET, &serverAdr.sin_addr, szServerIP, sizeof(szServerIP));
        ERIPC_WARNPRINTF("Bound successfully to %s:%d.", szServerIP, port);
    }
    return 0;
}


/* Provide a buffer of size 1024 */
int erIpcStartServer(int channelIdx, erMessageCB callback, char *szBuffer, int *nBuf, void *data)
{
    int     sockfd;
    struct sockaddr_in clientAdr;
    socklen_t len;
    int     n;
    int     nErr;
    int     fLocal = 1;
    erMessageCB messageCallback = NULL;
    int     nBufsize;

    nErr = erIpcInitServer(channelIdx, &sockfd, fLocal);
    if (nErr != 0)
    {
        ERIPC_ERRORPRINTF("Failed to start IPC server");
        return -1;
    }

    nBufsize = *nBuf;
    messageCallback = callback;
    while (1)
    {
        char    szClientIP[INET_ADDRSTRLEN + 1];

        // Blocking call.
        len = sizeof(clientAdr);
        n = recvfrom(sockfd, szBuffer, nBufsize, 0, (struct sockaddr *) &clientAdr, &len);
        szBuffer[n] = '\0';
        // Who's sending this message?
        // clientAdr.sin_addr.s_addr // 32-bit IPv4 address, network byte ordered.
        if (inet_ntop(AF_INET, &clientAdr.sin_addr, szClientIP, sizeof(szClientIP)))
        {
            ERIPC_LOGPRINTF("Message %s received from %s", szBuffer, szClientIP);
        }
        *nBuf = n;
        messageCallback(szBuffer, nBuf, data);
    }
}

int erIpcOpenServerChannel(int channelIdx, erServerChannel_t * channel)
{
    erInternalServerChannel_t *serverChannel;
    int     sockfd;
    int     nErr;
    int     fLocal = 1;

    nErr = erIpcInitServer(channelIdx, &sockfd, fLocal);
    if (nErr != 0)
    {
        ERIPC_ERRORPRINTF("Failed to initialize IPC server");
        return -1;
    }

    serverChannel = (erInternalServerChannel_t *) malloc(sizeof(erInternalServerChannel_t));

    if (serverChannel != NULL)
    {
        *channel = (erServerChannel_t) serverChannel;
    }
    else
    {
        ERIPC_ERRORPRINTF("Out of memory.");
        return -1;
    }
    serverChannel->sockfd = sockfd;
    return 0;
}

int erIpcGetServerFd(erServerChannel_t channel)
{
    erInternalServerChannel_t *serverChannel;

    serverChannel = (erInternalServerChannel_t *) channel;
    return serverChannel->sockfd;
}

int erIpcGetMessage(erServerChannel_t channel, char *szBuffer, int *nBuf)
{
    char    szClientIP[INET_ADDRSTRLEN + 1];
    struct sockaddr_in clientAdr;
    socklen_t len;
    int     sockfd;
    erInternalServerChannel_t *serverChannel;
    int     nBufsize;
    int n;

    nBufsize = *nBuf;
    serverChannel = (erInternalServerChannel_t *) channel;
    sockfd = serverChannel->sockfd;

    // Blocking call.
    len = sizeof(clientAdr);
    n = recvfrom(sockfd, szBuffer, nBufsize, 0, (struct sockaddr *) &clientAdr, &len);
    szBuffer[n] = '\0';
    // Who's sending this message?
    // clientAdr.sin_addr.s_addr // 32-bit IPv4 address, network byte ordered.
    if (inet_ntop(AF_INET, &clientAdr.sin_addr, szClientIP, sizeof(szClientIP)))
    {
        ERIPC_LOGPRINTF("Message %s received from %s", szBuffer, szClientIP);
    }
    *nBuf = n;
    return 0;
}


int erIpcStartClient(int channelIdx, erClientChannel_t * channel)
{
    erInternalClientChannel_t *clientChannel;
    int     port;
    int     ret;
    struct in_addr destinationAdr;

    // Note. The following variable can be turned into an argument later
    //   INADDR_LOOPBACK : "127.0.0.1"
    //   INADDR_ANY      : "0.0.0.0"
    char    szDestination[] = "127.0.0.1";
    int     sockfd;
    struct sockaddr_in serverAdr;

    clientChannel = (erInternalClientChannel_t *) malloc(sizeof(erInternalClientChannel_t));

    if (clientChannel != NULL)
    {
        *channel = (erClientChannel_t) clientChannel;
    }
    else
    {
        ERIPC_ERRORPRINTF("Out of memory.");
        return -1;
    }
    clientChannel->sockfd = -1;

    if ((ret = inet_pton(AF_INET, szDestination, &destinationAdr)) == 1)
    {
        ERIPC_LOGPRINTF("Sending messages to %s", szDestination);
    }
    else if (ret == 0)
    {
        ERIPC_WARNPRINTF("%s not a presentation IP address (not in dotted decimal format)", szDestination);
        return -1;
    }
    else
    {
        ERIPC_WARNPRINTF("Failed to convert to a numeric IP address");
        return -2;
    }

    port = erIpcGetServerPort(channelIdx);
    if (port == ER_IPC_PORT_ILLEGAL)
    {
        ERIPC_ERRORPRINTF("Could not allocate channel %d", channelIdx);
        return -1;
    }


    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&serverAdr, sizeof(serverAdr));
    serverAdr.sin_family = AF_INET;
    serverAdr.sin_addr.s_addr = destinationAdr.s_addr;
    serverAdr.sin_port = htons(port);

    clientChannel->sockfd = sockfd;
    clientChannel->serverAdr = serverAdr;

    return 0;
}
