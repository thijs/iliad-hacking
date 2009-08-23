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
#include "eripcclient.h"
#include "eripcbusyd.h"
#include "eripcparser.h"


int busySetBusy(erClientChannel_t channel, eCcBusyState busy)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "busySetBusy");
    cmd.cc = (int) ccBusySetBusy;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", busy);

    return erIpcSndCommand(channel, &cmd);
}

// Note:
// This function can be made generic provided the command structure is encapsulated
// in a datastructure.
int busyParseCommand(char *szCommand, erIpcCmd_t * pCmd)
{
    int     i;
    char   *pChar;
    char    szToken[TB_MAXCHARONLINE];
    int     nReqArgs = 0;

    pCmd->cc = (int) ccBusyUndefined;
    for (i = 0; i < TB_N_ARG; i++)
    {
        strcpy(pCmd->arg[i], "");
    }

    // Parse Command
    pChar = szCommand;
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

    // TODO: stop doing this hardcoded, rely on a smart datastructure instead.

    if (!strcmp(szToken, "busySetBusy") || !strcmp(szToken, "0"))
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccBusySetBusy;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else
    {
        strcpy(pCmd->name, "Undefined");
        pCmd->cc = (int) ccBusyUndefined;
        fprintf(stderr, "%s: Command %s \n", __FUNCTION__, szToken);
        fprintf(stderr, "Error parsing (%s). Command undefined\n", szCommand);
        pCmd->nArg = -1;
        return -1;
    }

    if (nReqArgs > 0)
    {
        erIpcGetArgs(pChar, pCmd, nReqArgs);
    }
    return 0;
}
