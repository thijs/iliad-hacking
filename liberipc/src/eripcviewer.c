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
#include "eripcviewer.h"
#include "eripcparser.h"


/** Outdated **
int vwrSignal(erClientChannel_t channel, int uaID, int signal) {
    erIpcCmd_t cmd;

    strcpy(cmd.name, "vwrSignal");
    cmd.cc = (int) ccVwrSignal;
    cmd.nArg = 2;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", signal);

    return erIpcSndCommand(channel, &cmd);
}
** Outdated **/

int vwrJumpToPage(erClientChannel_t channel, int uaID, int page)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "vwrJumpToPage");
    cmd.cc = (int) ccVwrJumpToPage;
    cmd.nArg = 2;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", page);

    return erIpcSndCommand(channel, &cmd);
}

int vwrBookmarkPage(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "vwrBookmarkPage");
    cmd.cc = (int) ccVwrBookmarkPage;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int vwrInvokeFindDialog(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "vwrInvokeFindDialog");
    cmd.cc = (int) ccVwrInvokeFindDialog;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int vwrZoomIn(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "vwrZoomIn");
    cmd.cc = (int) ccVwrZoomIn;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int vwrZoomOut(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "vwrZoomOut");
    cmd.cc = (int) ccVwrZoomOut;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

// Note:
// This function can be made generic provided the command structure is encapsulated
// in a datastructure.
int vwrParseCommand(char *szCommand, erIpcCmd_t * pCmd)
{
    int     i;
    char   *pChar;
    int     cmdcode;
    char    szToken[TB_MAXCHARONLINE];
    int     nReqArgs = 0;

    pCmd->cc = (int) ccVwrUndefined;
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
    if (strcmp(szToken, "vwrJumpToPage") == 0  ||  cmdcode == ccVwrJumpToPage)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccVwrJumpToPage;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else if (strcmp(szToken, "vwrBookmarkPage") == 0  ||  cmdcode == ccVwrBookmarkPage)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccVwrBookmarkPage;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "vwrInvokeFindDialog") == 0  ||  cmdcode == ccVwrInvokeFindDialog)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccVwrInvokeFindDialog;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "vwrZoomIn") == 0  ||  cmdcode == ccVwrZoomIn)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccVwrZoomIn;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "vwrZoomOut") == 0  ||  cmdcode == ccVwrZoomOut)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccVwrZoomOut;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "vwrStore") == 0  ||  cmdcode == ccVwrStore)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccVwrStore;
        nReqArgs = 3;
        pCmd->nArg = 3;
    }
    /** Outdated: replaced by vwrToolbar **
    else if (strcmp(szToken, "vwrSignal") == 0  ||  cmdcode == ccVwrSignal)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccVwrSignal;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    ** Outdated **/
    else if (strcmp(szToken, "vwrToolbar") == 0  ||  cmdcode == ccVwrToolbar)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccVwrToolbar;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else if (strcmp(szToken, "vwrToolbarSync") == 0  ||  cmdcode == ccVwrToolbarSync)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccVwrToolbarSync;
        nReqArgs = 0;
        pCmd->nArg = 0;
    }
    else if (strcmp(szToken, "vwrPagebarSync") == 0 || cmdcode == ccVwrPagebarSync)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccVwrPagebarSync;
        nReqArgs = 0;
        pCmd->nArg = 0;
    }
    else
    {
        strcpy(pCmd->name, "Undefined");
        pCmd->cc = (int) ccVwrUndefined;
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

int vwrStore(erClientChannel_t channel, int uaID, int category, char* itemID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "vwrStore");
    cmd.cc = (int) ccVwrStore;
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
