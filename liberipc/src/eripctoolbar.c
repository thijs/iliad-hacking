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
#include "eripctoolbar.h"
#include "eripcparser.h"

typedef struct
{
    e_iconID_t iconID;
    char*      iconName;  
}iconMap_t;

static const iconMap_t g_icon_map_table[] = {
    {iconID_battery, "battery"},
    {iconID_bookmark, "bookmark"},
    {iconID_collapse, "collapse"},
    {iconID_connectivity, "connectivity"},
    {iconID_copy, "copy"},
    {iconID_cut, "cut"},
    {iconID_empty, "empty"},
    {iconID_erase, "erase"},
    {iconID_error, "error"},
    {iconID_expand, "expand"},
    {iconID_filemanager, "filemanager"},
    {iconID_fontlarger, "fontlarger"},
    {iconID_fontsmaller, "fontsmaller"},
    {iconID_keyboard, "keyboard"},
    {iconID_orientation, "orientation"},
    {iconID_paste, "paste"},
    {iconID_pen, "pen"},
    {iconID_pen1pixel, "pen1pixel"},
    {iconID_pen3pixel, "pen3pixel"},
    {iconID_pen5pixel, "pen5pixel"},
    {iconID_pen7pixel, "pen7pixel"},
    {iconID_penheavier, "penheavier"},
    {iconID_penlighter, "penlighter"},
    {iconID_penpressure, "penpressure"},
    {iconID_selecttext, "selecttext"},
    {iconID_share, "share"},
    {iconID_summarize, "summarize"},
    {iconID_trashcan, "trashcan"},
    {iconID_zoomin, "zoomin"},
    {iconID_zoomout, "zoomout"},
    {iconID_eraseline, "eraseline"},
    {iconID_npage, "npage"},
    {iconID_penblack, "penblack"},
    {iconID_pendarkgrey, "pendarkgrey"},
    {iconID_penlightgrey, "penlightgrey"},
    {iconID_penwhite, "penwhite"},
    {iconID_pan, "pan"},
    {iconID_index, "index"},
    {iconID_pen1pixelLow, "pen1pixelLow"},
    {iconID_pen3pixelLow, "pen3pixelLow"},
    {iconID_pen5pixelLow, "pen5pixelLow"},
    {iconID_pen7pixelLow, "pen7pixelLow"},
    {iconID_penblackLow, "penblackLow"},
    {iconID_pendarkgreyLow, "pendarkgreyLow"},
    {iconID_penlightgreyLow, "penlightgreyLow"},
    {iconID_penwhiteLow, "penwhiteLow"},
    {iconID_locked, "locked"},
    {iconID_tagging, "tagging"},
    {iconID_search, "search"},
    {iconID_zoomfit, "zoomfit"},
    {iconID_landscape_portrait, "landscape_portrait"},
    {iconID_portrait_landscape, "portrait_landscape"},
    {iconID_zoomwidth, "zoomwidth"},
    {iconID_zoomback, "zoomback"},
    {iconID_find, "find"},
    {iconID_lock_screen, "lock_screen"},
    {iconID_not_used01, ""}, 
    {iconID_not_user02, ""},
    {iconID_continuous_mode, "continuous_mode"},
    {iconID_page_mode, "page_mode"},
    {iconID_sort, "sort"},
    {iconID_connect, "connect"},
    {iconID_start_reading, "start_reading"},
    {iconID_back, "back"},
    {iconID_lookup, "lookup"},
    {iconID_dictionary_search, "dictionary_search"},
    {iconID_forward, "forward"}
};

int tbGetIconNumber(void)
{
    int nIcons;
    nIcons = sizeof(g_icon_map_table) / sizeof(g_icon_map_table[0]);
    return nIcons;
}

const char* tbGetIconName(int iconID)
{
    int         i, nIcons;
    const char* iconName = NULL;
    
    nIcons = tbGetIconNumber();
    if ((iconID >= 0) && (iconID <= nIcons))
    {
        for (i = 0; i < nIcons; i++)
        {
            if (g_icon_map_table[i].iconID == iconID)
            {
                iconName = g_icon_map_table[i].iconName;
                break;
            }
        }
    }

    return iconName;
}

int tbAppendPlatformIcon(erClientChannel_t channel, int uaID, int iconID, int signal)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "tbAppendPlatformIcon");
    cmd.cc = (int) ccTbAppendPlatformIcon;
    cmd.nArg = 3;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", iconID);
    sprintf(cmd.arg[2], "%d", signal);

    return erIpcSndCommand(channel, &cmd);
}

int tbSynchronise(erClientChannel_t channel, int uaID, int signal)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "tbSynchronise");
    cmd.cc = (int) ccTbSynchronise;
    cmd.nArg = 2;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", signal);

    return erIpcSndCommand(channel, &cmd);
}

int tbRemovePlatformIcon(erClientChannel_t channel, int uaID, int iconID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "tbRemovePlatformIcon");
    cmd.cc = (int) ccTbRemovePlatformIcon;
    cmd.nArg = 2;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", iconID);

    return erIpcSndCommand(channel, &cmd);
}


int tbSetStatePlatformIcon(erClientChannel_t channel, int uaID, int iconID, int iconState)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "tbSetStatePlatformIcon");
    cmd.cc = (int) ccTbSetStatePlatformIcon;
    cmd.nArg = 3;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", iconID);
    sprintf(cmd.arg[2], "%d", iconState);

    return erIpcSndCommand(channel, &cmd);
}

int tbDisableUpdate(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "tbDisableUpdate");
    cmd.cc = (int) ccTbDisableUpdate;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int tbEnableUpdate(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "tbEnableUpdate");
    cmd.cc = (int) ccTbEnableUpdate;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int tbSelectIconSet(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "tbSelectIconSet");
    cmd.cc = (int) ccTbSelectIconSet;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}

int tbClearIconSet(erClientChannel_t channel, int uaID)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "tbClearIconSet");
    cmd.cc = (int) ccTbClearIconSet;
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);

    return erIpcSndCommand(channel, &cmd);
}




// Note:
// This function can be made generic provided the command structure is encapsulated
// in a datastructure.
int tbParseCommand(char *szCommand, erIpcCmd_t * pCmd)
{
    int     i;
    char   *pChar;
    int     cmdcode;
    char    szToken[TB_MAXCHARONLINE];
    int     nReqArgs = 0;

    pCmd->cc = (int) ccUndefined;
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
    if (strcmp(szToken, "tbAppendPlatformIcon") == 0 || cmdcode == ccTbAppendPlatformIcon)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccTbAppendPlatformIcon;
        nReqArgs = 3;
        pCmd->nArg = 3;
    }
    else if (strcmp(szToken, "tbRemovePlatformIcon") == 0 || cmdcode == ccTbRemovePlatformIcon)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccTbRemovePlatformIcon;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else if (strcmp(szToken, "tbSetStatePlatformIcon") == 0 || cmdcode == ccTbSetStatePlatformIcon)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccTbSetStatePlatformIcon;
        nReqArgs = 3;
        pCmd->nArg = 3;
    }
    else if (strcmp(szToken, "tbDisableUpdate") == 0 || cmdcode == ccTbDisableUpdate)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccTbDisableUpdate;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "tbEnableUpdate") == 0 || cmdcode == ccTbEnableUpdate)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccTbEnableUpdate;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "tbSelectIconSet") == 0 || cmdcode == ccTbSelectIconSet)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccTbSelectIconSet;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "tbClearIconSet") == 0 || cmdcode == ccTbClearIconSet)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccTbClearIconSet;
        nReqArgs = 1;
        pCmd->nArg = 1;
    }
    else if (strcmp(szToken, "tbSynchronise") == 0 || cmdcode == ccTbSynchronise)
    {
        strcpy(pCmd->name, szToken);
        pCmd->cc = (int) ccTbSynchronise;
        nReqArgs = 2;
        pCmd->nArg = 2;
    }
    else
    {
        strcpy(pCmd->name, "Undefined");
        pCmd->cc = (int) ccUndefined;
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


// Toolbar application: write "icon-clicked" message to IPC channel
int tbReportIconClicked(erClientChannel_t channel, int signal, int iconID, int iconState)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "tbIconClicked");
    cmd.cc = signal;
    cmd.nArg = 2;
    sprintf(cmd.arg[0], "%d", iconID);
    sprintf(cmd.arg[1], "%d", iconState);

    return erIpcSndCommand(channel, &cmd);
}

// Toolbar application: write "synchronised" message to IPC channel
int tbReportSynchronised(erClientChannel_t channel, int signal)
{
    erIpcCmd_t cmd;

    strcpy(cmd.name, "tbSynchronise");
    cmd.cc = signal;
    cmd.nArg = 0;

    return erIpcSndCommand(channel, &cmd);
}

