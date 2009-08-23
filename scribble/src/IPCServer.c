/*
 * This file is part of scribble.
 *
 * scribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * scribble is distributed in the hope that it will be useful,
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

/**
 * \file IPCServer.h
 * \brief scribble - IPC communication for server               
 * \author Jian Shen
 *
 * <File description>
 * 
 */

#include <pthread.h>
#include <liberipc/eripc.h>
#include <liberipc/eripctoolbar.h>
#include <liberipc/eripcicons.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>
#include <liberipc/eripcviewer.h>

#include "ScribbleLog.h"
#include "ScribbleIPC.h"

#include "ScribbleUI.h"
#include "ScribbleIPC.h"
#include "ScribbleDefine.h"

static erServerChannel_t srvChannel;
static gint srvInputTag;

//call back function for "jump to page xx" action.
void ipcMessageReceived(gpointer data, gint source_fd,
                        GdkInputCondition condition)
{
    char    szBuffer[SCRIB_SERVER_BUFFER_SIZE];
    int     nBuf = SCRIB_SERVER_BUFFER_SIZE;
    erIpcCmd_t command;
    int        iconID;
    int        iconState;

    PScribbleUICtx pScribbleUICtx=(PScribbleUICtx)data;

    erIpcGetMessage(srvChannel, szBuffer, &nBuf);
    SB_IPCPRINTF("get msg ok\n");
    if (vwrParseCommand(szBuffer, &command) >= 0) 
    {
        switch ((int) command.cc) 
        {
            case ccVwrJumpToPage:
                SB_IPCPRINTF("\nServiceCallBack: ccVwrJumpToPage %d\n", 
                             atoi(command.arg[1]));
                resetCurrentPage(pScribbleUICtx,atoi(command.arg[1]) );
                SB_IPCPRINTF("reset page ok\n");
                break;
            case ccVwrToolbar:
                iconID    = atoi(command.arg[0]);
                iconState = atoi(command.arg[1]);
                SB_IPCPRINTF("Toolbar iconID [%d] iconState [%d]", \
                        iconID, iconState);
                toolbar_key_selected(iconID, iconState, pScribbleUICtx);
                break;
            default:
                SB_ERRORPRINTF("UNKNOWN COMMAND\n");
        }
    }
}

//install IPC server
gboolean installIpcServer(PScribbleUICtx pScribbleUICtx)
{
    int     iRet = 0;
    int     fd = -1;

    SB_IPCPRINTF("__start__\n");
    iRet = erIpcOpenServerChannel(ER_SCRIBBLE_CHANNEL,
                                  &srvChannel);
    if (iRet != 0)
    {
        SB_ERRORPRINTF("erIpcOpenServerChannel returned %d\n", iRet);
        return iRet;
    }
    fd = erIpcGetServerFd(srvChannel);
    // gtk specific mechanism to monitor input file descriptor.
    srvInputTag = gdk_input_add(fd, GDK_INPUT_READ, 
                                ipcMessageReceived, (gpointer) pScribbleUICtx);
    SB_IPCPRINTF("__finish__\n");
    return iRet;
}

//stop IPC server
void stopIpcServer(void)
{
    int fd = erIpcGetServerFd(srvChannel);

    SB_IPCPRINTF("__start__\n");
    gdk_input_remove(srvInputTag);
    close(fd);
    SB_IPCPRINTF("__finish__\n");
}
