/*
 * This file is part of liberdm.
 *
 * liberdm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberdm is distributed in the hope that it will be useful,
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

#include "erdm.h"
#include "erdminternal.h"
#include "erdmlog.h"
#include "erdmServer.h"

static int fInit = 0;
static int sockfd = -1;
static struct sockaddr_in serverAdr;

/*
 * Initialize the port/address to which Messages will be sent.
 */
int dmMessageInit(char *szDestination)
{
    int     port;
    int     ret;
    struct in_addr destinationAdr;

    // Note:
    //   INADDR_LOOPBACK : "127.0.0.1"
    //   INADDR_ANY      : "0.0.0.0"

    if ((ret = inet_pton(AF_INET, szDestination, &destinationAdr)) == 1)
    {
        ERDM_LOGPRINTF("Sending messages to %s.\n", szDestination);
    }
    else if (ret == 0)
    {
        ERDM_ERRORPRINTF("%s not a presentation IP address (not in dotted decimal format).\n", szDestination);
        return -1;
    }
    else
    {
        ERDM_ERRORPRINTF("Failed to convert to a numeric IP address.\n");
        return -2;
    }

    dmGetServerPort(&port);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&serverAdr, sizeof(serverAdr));
    serverAdr.sin_family = AF_INET;
    serverAdr.sin_addr.s_addr = destinationAdr.s_addr;
    serverAdr.sin_port = htons(port);

    fInit = 1;
    return 0;
}

void dmMessageFramer(char *szMessage, uDmCommand * pDmCmd)
{
    switch ((int) pDmCmd->dmCmdGeneric.cmd)
    {
    case (int) dmCcDisplay:
        sprintf(szMessage, "!%d,%d,%d", (int) pDmCmd->dmCmdGeneric.cmd, (int) pDmCmd->dmCmdDisplay.priority,
                (int) pDmCmd->dmCmdDisplay.qual);
        break;

    case (int) dmCcDisplayPartial:
        sprintf(szMessage, "!%d,%d,%d,%d,%d,%d,%d", (int) pDmCmd->dmCmdGeneric.cmd, (int) pDmCmd->dmCmdDisplay.priority,
                (int) pDmCmd->dmCmdDisplay.qual, pDmCmd->dmCmdDisplayPartial.xUp, pDmCmd->dmCmdDisplayPartial.yUp,
                pDmCmd->dmCmdDisplayPartial.xDown, pDmCmd->dmCmdDisplayPartial.yDown);
        break;

    case (int) dmCcEraseToWhite:
        sprintf(szMessage, "!%d,%d", (int) pDmCmd->dmCmdGeneric.cmd, (int) pDmCmd->dmCmdDisplay.priority);
        break;

    default:
        strcpy(szMessage, "error");
        ERDM_ERRORPRINTF("Command Code %d not supported", (int) pDmCmd->dmCmdGeneric.cmd);
        break;
    }
}

int dmDisplay(eDmCmdPriority prior, eDmQuality qual)
{
    char    szMsg[DM_MAXCHARONLINE];
    uDmCommand dmCmd;

    dmCmd.dmCmdDisplay.cmd = dmCcDisplay;
    dmCmd.dmCmdDisplay.priority = prior;
    dmCmd.dmCmdDisplay.qual = qual;

    dmMessageFramer(szMsg, &dmCmd);

    if (!fInit)
    {
        dmMessageInit("127.0.0.1");
    }

    sendto(sockfd, szMsg, strlen(szMsg), 0, (struct sockaddr *) &serverAdr, sizeof(serverAdr));
    return 0;
}

int dmDisplayPartial(eDmCmdPriority prior, eDmQuality qual, int xUp, int yUp, int xDown, int yDown)
{
    char    szMsg[DM_MAXCHARONLINE];
    uDmCommand dmCmd;

    dmCmd.dmCmdDisplayPartial.cmd = dmCcDisplayPartial;
    dmCmd.dmCmdDisplayPartial.priority = prior;
    dmCmd.dmCmdDisplayPartial.qual = qual;
    dmCmd.dmCmdDisplayPartial.xUp = xUp;
    dmCmd.dmCmdDisplayPartial.yUp = yUp;
    dmCmd.dmCmdDisplayPartial.xDown = xDown;
    dmCmd.dmCmdDisplayPartial.yDown = yDown;

    dmMessageFramer(szMsg, &dmCmd);

    if (!fInit)
    {
        dmMessageInit("127.0.0.1");
    }

    sendto(sockfd, szMsg, strlen(szMsg), 0, (struct sockaddr *) &serverAdr, sizeof(serverAdr));
    return 0;
}

int dmDisplayEraseToWhite(eDmCmdPriority prior)
{
    char    szMsg[DM_MAXCHARONLINE];
    uDmCommand dmCmd;

    dmCmd.dmCmdDisplay.cmd = dmCcEraseToWhite;
    dmCmd.dmCmdDisplay.priority = prior;

    dmMessageFramer(szMsg, &dmCmd);

    if (!fInit)
    {
        dmMessageInit("127.0.0.1");
    }

    sendto(sockfd, szMsg, strlen(szMsg), 0, (struct sockaddr *) &serverAdr, sizeof(serverAdr));
    return 0;
}

int dmDump(eDmCmdPriority prior)
{
    ERDM_ERRORPRINTF("dmDump is not implemented.\n");
    return -1;
}
