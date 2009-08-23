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
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */


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
#include "eripclog.h"
#include "eripcprivate.h"
#include "eripcclient.h"

int erIpcSndCommand(erClientChannel_t channel, erIpcCmd_t * cmd)
{
    char    szBuffer[TB_MAXCHARONLINE];
    int     i;
    int     len;

    if (channel == NULL  ||  cmd == NULL)
    {
        ERIPC_ERRORPRINTF("Illegal parameters: channel [%p] cmd [%p]", channel, cmd);
        return -1;
    }

    snprintf(szBuffer, sizeof(szBuffer), "%d", cmd->cc);

    for (i = 0; i < cmd->nArg; i++)
    {
        len = strlen(szBuffer);
        snprintf(szBuffer + len, sizeof(szBuffer) - len, ",%s", cmd->arg[i]);
    }
    return erIpcSndMessage(channel, szBuffer);
}

int erIpcSndMessage(erClientChannel_t channel, char *szMsg)
{
    int     sockfd;
    struct sockaddr_in serverAdr;
    erInternalClientChannel_t *clientChannel;

    if (channel == NULL  ||  szMsg == NULL)
    {
        ERIPC_ERRORPRINTF("Illegal parameters: channel [%p] szMsg [%p]", channel, szMsg);
        return -1;
    }

    clientChannel = (erInternalClientChannel_t *) channel;
    sockfd = clientChannel->sockfd;
    serverAdr = clientChannel->serverAdr;

    sendto(sockfd, szMsg, strlen(szMsg), 0, (struct sockaddr *) &serverAdr, sizeof(serverAdr));
    return 0;
}

