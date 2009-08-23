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
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "eripc.h"
#include "eripcclient.h"
#include "eripcarinc.h"
#include "eripcparser.h"


int arincSignal(erClientChannel_t channel, int uaID, int signal) {
    erIpcCmd_t cmd;

    strcpy(cmd.name, "arincSignal");
    cmd.cc = (int) ccArincSignal;
    cmd.nArg = 2;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", signal);

    return erIpcSndCommand(channel, &cmd);
}

int arincJumpToPage(erClientChannel_t channel, int uaID, int page)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "arincJumpToPage");
    cmd.cc = (int) ccArincJumpToPage;
    cmd.nArg = 2;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", page);

    return erIpcSndCommand(channel, &cmd);
}

int arincBookmarkPage(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "arincBookmarkPage");
    cmd.cc = (int) ccArincBookmarkPage;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int arincInvokeFindDialog(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "arincInvokeFindDialog");
    cmd.cc = (int) ccArincInvokeFindDialog;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int arincZoomIn(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "arincZoomIn");
    cmd.cc = (int) ccArincZoomIn;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int arincZoomOut(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "arincZoomOut");
    cmd.cc = (int) ccArincZoomOut;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

// Note:
// This function can be made generic provided the command structure is encapsulated
// in a datastructure.
int arincParseCommand(char *szCommand, erIpcCmd_t * pCmd)
{
    int     i;
    char   *pChar;
    int     cmdcode;
    char    szToken[TB_MAXCHARONLINE];
    int     nReqArgs = 0;

    pCmd->cc = (int) ccArincUndefined;
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

    if (isdigit(szToken[0]))
    {
        cmdcode = atoi(szToken);
    }
    else
    {
        cmdcode = -1;
    }
    if (strcmp(szToken, "arincJumpToPage") == 0  ||  cmdcode == ccArincJumpToPage)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccArincJumpToPage;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else if (strcmp(szToken, "arincBookmarkPage") == 0  ||  cmdcode == ccArincBookmarkPage)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccArincBookmarkPage;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "arincInvokeFindDialog") == 0  ||  cmdcode == ccArincInvokeFindDialog)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccArincInvokeFindDialog;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "arincZoomIn") == 0  ||  cmdcode == ccArincZoomIn)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccArincZoomIn;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "arincZoomOut") == 0  ||  cmdcode == ccArincZoomOut)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccArincZoomOut;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "arincStore") == 0  ||  cmdcode == ccArincStore)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccArincStore;
        nReqArgs = 3;
        pCmd->nArg = 3;
    }
    else if (strcmp(szToken, "arincSignal") == 0  ||  cmdcode == ccArincSignal)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccArincSignal;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else
    {
        strcpy(pCmd->name, "Undefined");
        pCmd->cc = (int) ccArincUndefined;
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


int arincStore(erClientChannel_t channel, int uaID, int category, char* itemID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "arincStore");
    cmd.cc = (int) ccArincStore;
    cmd.nArg = 3;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", category);
	
	if ((itemID) && (strlen(itemID) < TB_ARGLENGTH))
	{
		strncpy(cmd.arg[2], itemID, TB_ARGLENGTH);
	}
	else
	{
	    sprintf(cmd.arg[2],"%s", "");
 	    fprintf(stderr, "Command %s \n", cmd.name);
 		fprintf(stderr, "invalid itemID (%s)\n", itemID);
 	}

    return erIpcSndCommand(channel, &cmd);	
}
