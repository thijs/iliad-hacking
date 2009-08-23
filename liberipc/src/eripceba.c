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
#include "eripceba.h"
#include "eripcparser.h"


int ebaJumpToPage(erClientChannel_t channel, int uaID, int page)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ebaJumpToPage");
    cmd.cc = (int) ccEbaJumpToPage;
    cmd.nArg = 2;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", page);

    return erIpcSndCommand(channel, &cmd);
}

int ebaBookmarkPage(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ebaBookmarkPage");
    cmd.cc = (int) ccEbaBookmarkPage;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int ebaInvokeFindDialog(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ebaInvokeFindDialog");
    cmd.cc = (int) ccEbaInvokeFindDialog;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int ebaZoomIn(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ebaZoomIn");
    cmd.cc = (int) ccEbaZoomIn;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int ebaZoomOut(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ebaZoomOut");
    cmd.cc = (int) ccEbaZoomOut;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

// Note:
// This function can be made generic provided the command structure is encapsulated
// in a datastructure.
int ebaParseCommand(char *szCommand, erIpcCmd_t * pCmd)
{
    int     i;
    char   *pChar;
    int     cmdcode;
    char    szToken[TB_MAXCHARONLINE];
    int     nReqArgs = 0;

    pCmd->cc = (int) ccEbaUndefined;
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
    if (strcmp(szToken, "ebaJumpToPage") == 0  ||  cmdcode == ccEbaJumpToPage)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccEbaJumpToPage;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else if (strcmp(szToken, "ebaBookmarkPage") == 0  ||  cmdcode == ccEbaBookmarkPage)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccEbaBookmarkPage;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "ebaInvokeFindDialog") == 0  ||  cmdcode == ccEbaInvokeFindDialog)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccEbaInvokeFindDialog;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "ebaZoomIn") == 0  ||  cmdcode == ccEbaZoomIn)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccEbaZoomIn;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "ebaZoomOut") == 0  ||  cmdcode == ccEbaZoomOut)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccEbaZoomOut;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "ebaStore") == 0  ||  cmdcode == ccEbaStore)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccEbaStore;
        nReqArgs = 3;
        pCmd->nArg = 3;
    }
    else if (strcmp(szToken, "ebaToolbar") == 0  ||  cmdcode == ccEbaToolbar)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccEbaToolbar;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else
    {
        strcpy(pCmd->name, "Undefined");
        pCmd->cc = (int) ccEbaUndefined;
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

int ebaStore(erClientChannel_t channel, int uaID, int category, char* itemID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ebaStore");
    cmd.cc = (int) ccEbaStore;
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
