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

#include "eripc.h"
#include "eripcclient.h"
#include "eripcpagebar.h"
#include "eripcparser.h"
#include "eripcviewer.h"

static char* pbFunctionNames[ccPbUndefined] = {
    "pbSetPageCount",
    "pbSetCurrentPage",
    "pbSetCurrentPageOffset",
    "pbSetZoomMode",
    "pbSetDrawAreaOrientation",
    "pbSetDrawAreaHOrigin",
    "pbSetDrawAreaVOrigin",
    "pbSetDrawAreaHSize",
    "pbSetDrawAreaVSize",
    "pbSetBarFontType",
    "pbSetBarFontHeight",
    "pbAddBookmark",
    "pbRemoveBookmark",
    "pbSetBookmarkMax",
    "pbAddNote",
    "pbRemoveNote",
    "pbSetNotesMax",
    "pbReset",
    "pbShow",
    "pbRedraw",
    "ccPbSynchronise"
};

static int pbCallFunction(erClientChannel_t channel, int uaID, int cc, int param) {
    erIpcCmd_t cmd;

    if (cc>=ccPbUndefined) {
        printf("Invalid Pagebar function! (function=%d)\n", cc);
        return -1;
    }

    cmd.cc = cc;
    strcpy(cmd.name, pbFunctionNames[cc]);
    cmd.nArg = 2;
    sprintf(cmd.arg[0], "%d", uaID);
    sprintf(cmd.arg[1], "%d", param);
        
    return erIpcSndCommand(channel, &cmd);
}

int pbSetPageCount(erClientChannel_t channel, int uaID, int pagecount) {
    return pbCallFunction(channel, uaID, ccPbSetPageCount, pagecount);
}

int pbSetCurrentPage(erClientChannel_t channel, int uaID, int page) {
    return pbCallFunction(channel, uaID, ccPbSetCurrentPage, page);
}

int pbSetCurrentPageOffset(erClientChannel_t channel, int uaID, int pageoffset) {
    return pbCallFunction(channel, uaID, ccPbSetCurrentPageOffset, pageoffset);
}

int pbSetZoomMode(erClientChannel_t channel, int uaID, int zoomMode) {
    return pbCallFunction(channel, uaID, ccPbSetZoomMode, zoomMode);
}

int pbSetDrawAreaOrientation(erClientChannel_t channel, int uaID, int orientation) {
    return pbCallFunction(channel, uaID, ccPbSetDrawAreaOrientation, orientation);
}

int pbSetDrawAreaHOrigin(erClientChannel_t channel, int uaID, int origin) {
    return pbCallFunction(channel, uaID, ccPbSetDrawAreaHOrigin, origin);
}

int pbSetDrawAreaVOrigin(erClientChannel_t channel, int uaID, int origin) {
    return pbCallFunction(channel, uaID, ccPbSetDrawAreaVOrigin, origin);
}

int pbSetDrawAreaHSize(erClientChannel_t channel, int uaID, int size) {
    return pbCallFunction(channel, uaID, ccPbSetDrawAreaHSize, size);
}

int pbSetDrawAreaVSize(erClientChannel_t channel, int uaID, int size) {
    return pbCallFunction(channel, uaID, ccPbSetDrawAreaVSize, size);
}

int pbSetBarFontType(erClientChannel_t channel, int uaID, int font) {
    return pbCallFunction(channel, uaID, ccPbSetBarFontType, font);
}

int pbSetBarFontHeight(erClientChannel_t channel, int uaID, int height) {
    return pbCallFunction(channel, uaID, ccPbSetBarFontHeight, height);
}

int pbAddBookmark(erClientChannel_t channel, int uaID, int value) {
    return pbCallFunction(channel, uaID, ccPbAddBookmark, value);
}
                
int pbRemoveBookmark(erClientChannel_t channel, int uaID, int value) {
    return pbCallFunction(channel, uaID, ccPbRemoveBookmark, value);
}
                
int pbSetBookmarkMax(erClientChannel_t channel, int uaID, int max) {
    return pbCallFunction(channel, uaID, ccPbSetBookmarkMax, max);
}
                
int pbAddNote(erClientChannel_t channel, int uaID, int value) {
    return pbCallFunction(channel, uaID, ccPbAddNote, value);
}
                
int pbRemoveNotes(erClientChannel_t channel, int uaID, int value) {
    return pbCallFunction(channel, uaID, ccPbRemoveNote, value);
}
                
int pbSetNotesMax(erClientChannel_t channel, int uaID, int max) {
    return pbCallFunction(channel, uaID, ccPbSetNotesMax, max);
}

int pbReset(erClientChannel_t channel, int uaID) {
    erIpcCmd_t cmd;

    cmd.cc = ccPbReset;
    strcpy(cmd.name, pbFunctionNames[ccPbReset]);
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);
            
    return erIpcSndCommand(channel, &cmd);
}

int pbShow(erClientChannel_t channel, int uaID, int show) {
    return pbCallFunction(channel, uaID, ccPbShow, show);
}

int pbRedraw(erClientChannel_t channel, int uaID) {
    erIpcCmd_t cmd;

    cmd.cc = ccPbRedraw;
    strcpy(cmd.name, pbFunctionNames[ccPbRedraw]);
    cmd.nArg = 1;
    sprintf(cmd.arg[0], "%d", uaID);
        
    return erIpcSndCommand(channel, &cmd);
}

// send synchronise message from application to pageBar
int pbSynchronise(erClientChannel_t channel, int uaID)
{
    // printf("\nSend synchronise message to pagebar\n");
    return pbCallFunction(channel, uaID, ccPbSynchronise, 0);
}

// send synchronise ack message from pageBar to application
int pbReportSynchronise(erClientChannel_t channel, int uaID)
{
    // printf("\nSend synchronise message to application\n");
    return pbCallFunction(channel, uaID, ccVwrPagebarSync, 0);
}

// Note:
// This function can be made generic provided the command structure is encapsulated
// in a datastructure.
int pbParseCommand(char *szCommand, erIpcCmd_t * pCmd) {
    int     i;
    int     paramsOffset;
    char    szToken[TB_MAXCHARONLINE];
    int     nReqArgs = 0;

    pCmd->cc = (int) ccPbUndefined;
    for (i = 0; i < TB_N_ARG; i++)
    {
        strcpy(pCmd->arg[i], "");
    }

    // Parse Command
/* Original: */
/*
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
*/
    for (paramsOffset=0; szCommand[paramsOffset]!='\0' && szCommand[paramsOffset]!=','; paramsOffset++) 
    {
        /* ignore */ ;
    }
    memcpy(szToken, szCommand, paramsOffset);
    szToken[paramsOffset]='\0';
    if (szCommand[paramsOffset]==',')
    {
        paramsOffset++;
    }

    // We have found the command Token. Figure out what the command is.
    for (i=0; i<ccPbUndefined; i++)
    {
        char s[16];
        sprintf(s, "%d", i);
        if (!strcmp(szToken, pbFunctionNames[i]) || !strcmp(szToken, s))
        {
            strcpy(pCmd->name, szToken);
            pCmd->cc = (int) i;
            nReqArgs = 2;
            pCmd->nArg = 2;
            break;
        }
    }

    if (i==ccPbReset || i==ccPbRedraw)
    {
        nReqArgs = 1;
    }
    else if (i==ccPbUndefined)
    {
        strcpy(pCmd->name, "Undefined");
        pCmd->cc = (int) ccPbUndefined;
        fprintf(stderr, "%s: Command %s \n", __FUNCTION__, szToken);
        fprintf(stderr, "Error parsing (%s). Command undefined\n", szCommand);
        pCmd->nArg = -1;
        return -1;
    }

    if (nReqArgs > 0)
    {
        erIpcGetArgs(szCommand+paramsOffset, pCmd, nReqArgs);
    }
    return 0;
}

