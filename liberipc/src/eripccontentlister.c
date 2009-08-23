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
#include "eripcbusyd.h"
#include "eripccontentlister.h"
#include "eripcparser.h"

// Note:
// This function can be made generic provided the command structure is encapsulated
// in a datastructure.
int clParseCommand(char *szCommand, erIpcCmd_t * pCmd)
{
    int     i;
    char   *pChar;
    int     cmdcode;
    char    szToken[TB_MAXCHARONLINE];
    int     nReqArgs = 0;

    pCmd->cc = (int) ccClUndefined;
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
    if (strcmp(szToken, "ccClStore") == 0  ||  cmdcode == ccClStore)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClStore;
        nReqArgs = 4;
        pCmd->nArg = 4;
    }
    else if (strcmp(szToken, "ccClMSDiskSetConnected") == 0  ||  cmdcode == ccClMSDiskSetConnected)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClMSDiskSetConnected;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "ccClGotoPage") == 0  ||  cmdcode == ccClGotoPage)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClGotoPage;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "ccClBatteryLow") == 0  ||  cmdcode == ccClBatteryLow)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClBatteryLow;
        nReqArgs = 3;
        pCmd->nArg = 3;
    }
    else if (strcmp(szToken, "ccClToolbarIcon") == 0  ||  cmdcode == ccClToolbarIcon)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClToolbarIcon;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else if (strcmp(szToken, "ccClStoragePresent") == 0  ||  cmdcode == ccClStoragePresent)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClStoragePresent;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else if (strcmp(szToken, "ccClToolbarSynchronised") == 0  ||  cmdcode == ccClToolbarSynchronised)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClToolbarSynchronised;
        nReqArgs = 0;
        pCmd->nArg = 0;
    }
    else if (strcmp(szToken, "ccClBusyIndicator") == 0  ||  cmdcode == ccClBusyIndicator)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClBusyIndicator;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }	
    else if (strcmp(szToken, "ccClDisplayUpdated") == 0  ||  cmdcode == ccClDisplayUpdated)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClDisplayUpdated;
        nReqArgs = 0;
        pCmd->nArg = 0;
    }	
    else if (strcmp(szToken, "ccClNewContent") == 0  ||  cmdcode == ccClNewContent)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClNewContent;
        nReqArgs = 0;
        pCmd->nArg = 0;
    }
    else if (strcmp(szToken, "ccClPenClick") == 0  ||  cmdcode == ccClPenClick)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClPenClick;
        nReqArgs = 0;
        pCmd->nArg = 0;
    }
    else if (strcmp(szToken, "ccClEnableAudio") == 0  ||  cmdcode == ccClEnableAudio)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccClEnableAudio;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else
    {
        strcpy(pCmd->name, "ccClUndefined");
        pCmd->cc = (int) ccClUndefined;
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

int clStore(erClientChannel_t channel, int uaID, int category, char* itemID, char* itemLocation)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ccClStore");
    cmd.cc = (int) ccClStore;
    cmd.nArg = 4;
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

    if ((itemLocation) && (strlen(itemLocation) < TB_ARGLENGTH))
    {
        strncpy(cmd.arg[3], itemLocation, TB_ARGLENGTH);
    }
    else
    {
        sprintf(cmd.arg[3],"%s", "");
         fprintf(stderr, "Command %s \n", cmd.name);
         fprintf(stderr, "invalid itemLocation (%s)\n", itemLocation);
     }

    return erIpcSndCommand(channel, &cmd);    
}

int MSDiskSetConnected(erClientChannel_t channel, int connected)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ccClMSDiskSetConnected");
    cmd.cc = (int) ccClMSDiskSetConnected;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", connected);

    return erIpcSndCommand(channel, &cmd);
}

int clGotoPage(erClientChannel_t channel, int page)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ccClGotoPage");
    cmd.cc = (int) ccClGotoPage;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", page);

    return erIpcSndCommand(channel, &cmd);
}

int clBatteryLow(erClientChannel_t channel, eCcClBatteryAction action, int percentage, int time_left)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ccClBatteryLow");
    cmd.cc = (int) ccClBatteryLow;
    cmd.nArg = 3;
    sprintf(cmd.arg[0], "%d", action);
    sprintf(cmd.arg[1], "%d", percentage);
    sprintf(cmd.arg[2], "%d", time_left);

    return erIpcSndCommand(channel, &cmd);
}

int clStoragePresent(erClientChannel_t channel, char* storageType, int storagePresent)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ccClStoragePresent");
    cmd.cc = (int) ccClStoragePresent;
    cmd.nArg = 2;
    sprintf(cmd.arg[0], "%s", storageType);
    sprintf(cmd.arg[1], "%d", storagePresent);

    return erIpcSndCommand(channel, &cmd);
}


int clBusyIndicator(erClientChannel_t channel,  eCcBusyState status)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ccClBusyIndicator");
    cmd.cc = (int) ccClBusyIndicator;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", status);

    return erIpcSndCommand(channel, &cmd);
}

int clDisplayUpdated(erClientChannel_t channel)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ccClDisplayUpdated");
    cmd.cc = (int) ccClDisplayUpdated;
    cmd.nArg = 0;

    return erIpcSndCommand(channel, &cmd);
}

int clNewContent(erClientChannel_t channel)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ccClNewContent");
    cmd.cc = (int) ccClNewContent;
    cmd.nArg = 0;

    return erIpcSndCommand(channel, &cmd);
}

int clPenClick(erClientChannel_t channel)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ccClPenClick");
    cmd.cc = (int) ccClPenClick;
    cmd.nArg = 0;

    return erIpcSndCommand(channel, &cmd);
}

int clEnableAudio(erClientChannel_t channel, int volume)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "ccClEnableAudio");
    cmd.cc = (int) ccClEnableAudio;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", volume);

    return erIpcSndCommand(channel, &cmd);
}

