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
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "erdm.h"
#include "erdminternal.h"
#include "erdmlog.h"
#include "display.h"

int dmGetServerPort(int *port)
{
    *port = DMPORT;
    return 0;
}

/*
 * local
 *   When set to '1' the server will only listen to processes running on the same host
 */
int initServer(int *sockfd, int local)
{
    int     error;
    int     port;
    struct sockaddr_in serverAdr;

    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    dmGetServerPort(&port);

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
        ERDM_WARNPRINTF("Bound successfully to %s:%d.", szServerIP, port);
    }
    return 0;
}

int dmValidateCommand(uDmCommand * dmCommand)
{
    if (dmCommand->dmCmdGeneric.cmd == dmCcDisplayPartial)
    {
        int     xUp = dmCommand->dmCmdDisplayPartial.xUp;
        int     xDown = dmCommand->dmCmdDisplayPartial.xDown;
        int     yUp = dmCommand->dmCmdDisplayPartial.yUp;
        int     yDown = dmCommand->dmCmdDisplayPartial.yDown;

        // Check for offscreen coordinates.
        if (xUp < 0 || xUp > SCREEN_WIDTH)
        {
            ERDM_ERRORPRINTF("Invalid coordinate xUp (%d)", xUp);
            return -1;
        }
        if (xDown < 0 || xDown > SCREEN_WIDTH)
        {
            ERDM_ERRORPRINTF("Invalid coordinate xDown (%d)", xDown);
            return -1;
        }
        if (yUp < 0 || yUp > SCREEN_HEIGHT)
        {
            ERDM_ERRORPRINTF("Invalid coordinate yUp (%d)", yUp);
            return -1;
        }
        if (yDown < 0 || yDown > SCREEN_HEIGHT)
        {
            ERDM_ERRORPRINTF("Invalid coordinate yDown (%d)", yDown);
            return -1;
        }

        // Check for a valid shape. (A line is not allowed)
        if (xUp == xDown || yUp == yDown)
        {
            ERDM_ERRORPRINTF("Trying to do a partial update of a line (%d,%d):(%d,%d)", xUp, yUp, xDown, yDown);
            return -1;
        }
    }

    switch (dmCommand->dmCmdGeneric.cmd)
    {
    case dmCcDisplay:
    case dmCcDisplayPartial:
        // Validate priority and waveforms
        if ((int) dmCommand->dmCmdDisplay.priority < 0 || (int) dmCommand->dmCmdDisplay.priority > (int) dmCmdPriorUrgent)
        {
            ERDM_ERRORPRINTF("priority out of range (%d)", (int) dmCommand->dmCmdDisplay.priority);
            return -1;
        }
        if ((int) dmCommand->dmCmdDisplay.qual < 0 || (int) dmCommand->dmCmdDisplay.qual > dmQUndefined)
        {
            ERDM_ERRORPRINTF("quality out of range (%d)", (int) dmCommand->dmCmdDisplay.qual);
            return -1;
        }
        break;

    case dmCcDump:
        ERDM_WARNPRINTF("Dump command not yet validated");
        break;

    case dmCcEraseToWhite:
        if ((int) dmCommand->dmCmdDisplay.priority < 0 || (int) dmCommand->dmCmdDisplay.priority > (int) dmCmdPriorUrgent)
        {
            ERDM_ERRORPRINTF("priority out of range (%d)", (int) dmCommand->dmCmdDisplay.priority);
            return -1;
        }
        break;

    case dmCcUndefined:
        ERDM_WARNPRINTF("Undefined command");
        return -1;
        break;

    default:
        return -1;
        break;
    }

    return 0;
}

int dmParseCommand(erDmCmd_t * pCmd, uDmCommand * dmCommand)
{
    switch (pCmd->cc)
    {
    case (int) dmCcDisplay:
        ERDM_LOGPRINTF("%s(%s,%s)", pCmd->name, pCmd->arg[0], pCmd->arg[1]);
        dmCommand->dmCmdDisplay.cmd = dmCcDisplay;
        dmCommand->dmCmdDisplay.priority = (eDmCmdPriority) atoi(pCmd->arg[0]);
        dmCommand->dmCmdDisplay.qual = (eDmQuality) atoi(pCmd->arg[1]);
        break;

    case (int) dmCcDisplayPartial:
        ERDM_LOGPRINTF("%s(%s,%s,%s,%s,%s,%s)", pCmd->name, pCmd->arg[0], pCmd->arg[1], pCmd->arg[2], pCmd->arg[3], pCmd->arg[4],
                       pCmd->arg[5]);
        dmCommand->dmCmdDisplayPartial.cmd = dmCcDisplayPartial;
        dmCommand->dmCmdDisplayPartial.priority = (eDmCmdPriority) atoi(pCmd->arg[0]);
        dmCommand->dmCmdDisplayPartial.qual = (eDmQuality) atoi(pCmd->arg[1]);
        dmCommand->dmCmdDisplayPartial.xUp = atoi(pCmd->arg[2]);
        dmCommand->dmCmdDisplayPartial.yUp = atoi(pCmd->arg[3]);
        dmCommand->dmCmdDisplayPartial.xDown = atoi(pCmd->arg[4]);
        dmCommand->dmCmdDisplayPartial.yDown = atoi(pCmd->arg[5]);
        break;

    case (int) dmCcDump:
        ERDM_LOGPRINTF("%s(%s)", pCmd->name, pCmd->arg[0]);
        dmCommand->dmCmdDump.cmd = dmCcDump;
        dmCommand->dmCmdDump.priority = (eDmCmdPriority) atoi(pCmd->arg[0]);
        break;

    case (int) dmCcEraseToWhite:
        ERDM_LOGPRINTF("%s(%s)", pCmd->name, pCmd->arg[0]);
        dmCommand->dmCmdDump.cmd = dmCcEraseToWhite;
        dmCommand->dmCmdDump.priority = (eDmCmdPriority) atoi(pCmd->arg[0]);
        break;

    case (int) dmCcUndefined:
        ERDM_ERRORPRINTF("Undefined command");
        dmCommand->dmCmdGeneric.cmd = dmCcUndefined;
        return -1;
        break;
    }
    return 0;
}


int dmParseMessage(char *szCommand, erDmCmd_t * pCmd)
{
    int     i;
    char   *pChar;
    char    szToken[DM_MAXCHARONLINE];
    int     nReqArgs = 0;

    pCmd->cc = (int) dmCcUndefined;
    for (i = 0; i < DM_N_ARG; i++)
    {
        strcpy(pCmd->arg[i], "");
    }

    // Parse Command
    // The general form of a command is : !n1,n2,n3
    if (szCommand[0] != '!')
    {
        ERDM_ERRORPRINTF("Command should start with \'!\'. (%s)", szCommand);
        return -1;
    }
    // Move beyond initial '!'
    pChar = szCommand + 1;
    i = 0;
    while (*pChar != '\0')
    {
        szToken[i] = '\0';
        if (*pChar == ',')
        {
            pChar++;
            break;
        }
        szToken[i] = *pChar++;
        i++;
        szToken[i] = '\0';
    }

    // We have found the command Token. Figure out what the command is.
    if (!strcmp(szToken, "dmDisplay") || !strcmp(szToken, "0"))
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) dmCcDisplay;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else if (!strcmp(szToken, "dmDisplayPartial") || !strcmp(szToken, "1"))
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) dmCcDisplayPartial;
        nReqArgs = 6;
        pCmd->nArg = 6;
    }
    else if (!strcmp(szToken, "dmDump") || !strcmp(szToken, "2"))
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) dmCcDump;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (!strcmp(szToken, "dmEraseToWhite") || !strcmp(szToken, "3"))
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) dmCcEraseToWhite;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else
    {
        strcpy(pCmd->name, "Undefined");
        pCmd->cc = (int) dmCcUndefined;
        ERDM_ERRORPRINTF("Command %s \n", szToken);
        ERDM_ERRORPRINTF("Error parsing (%s). Command undefined\n", szCommand);
        pCmd->nArg = -1;
        return -1;
    }

    if (nReqArgs > 0)
    {
        erDmGetArgs(pChar, pCmd, nReqArgs);
    }
    return 0;
}

int dmMessageParser(char *szMessage, uDmCommand * dmCommand)
{
    // The real parser....
    erDmCmd_t cmd;
    int     nRet;

    nRet = dmParseMessage(szMessage, &cmd);
    if (nRet != 0)
    {
        ERDM_ERRORPRINTF("dmParseMessage failed with errorcode %d", nRet);
        return -1;
    }

    nRet = dmParseCommand(&cmd, dmCommand);
    if (nRet != 0)
    {
        ERDM_ERRORPRINTF("dmParseCommand failed with errorcode %d", nRet);
        return -1;
    }

    nRet = dmValidateCommand(dmCommand);
    if (nRet != 0)
    {
        ERDM_ERRORPRINTF("dmValidateCommand failed with errorcode %d", nRet);
        return -1;
    }
    return 0;
}
