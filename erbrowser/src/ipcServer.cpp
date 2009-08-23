/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file ipcServer.cpp
 * \brief browser - browser IPC communication               
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#include <pthread.h>
#include <liberipc/eripc.h>
#include <liberipc/eripctoolbar.h>
#include <liberipc/eripcicons.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>
#include <liberipc/eripcviewer.h>
#include "browserTypes.h"
#include "contentLister.h"
#include "browserLog.h"
#include "toolbar.h"
#include "ipcServer.h"
#include "pagebar.h"

static erServerChannel_t theServerChannel;

// internally used routines
void    ipcMessageReceived(gpointer data, gint source_fd, GdkInputCondition condition);


gboolean ipc_InstallIpcServer(Ereader * browser)
{
    int     returnValue;
    int     channel;
    int     fd = -1;

    BR_IPCPRINTF("entry");

    returnValue = erIpcOpenServerChannel(ER_XHTML_VIEWER_CHANNEL, &theServerChannel);

    if (returnValue != (-1))
    {
        fd = erIpcGetServerFd(theServerChannel);

        BR_IPCPRINTF("erIpcGetServerFd returned %d\n", fd);

        // gtk specific mechanism to monitor input file descriptor.
        returnValue = gdk_input_add(fd, GDK_INPUT_READ, ipcMessageReceived, (gpointer) browser);

        BR_IPCPRINTF("gdk_input_add returned %d", returnValue);
        return TRUE;
    }
    else
    {
        BR_ERRORPRINTF("Could not open server channel");
        return FALSE;
    }
}

// 
// This callback is invoked when there is data to read on file descriptor source_fd.
// The Ereader datastructure containing the "mozembed" reference info is passed via parameter 'data'
//
void ipcMessageReceived(gpointer data, gint source_fd, GdkInputCondition condition)
{
    Ereader *browser;
    char       szBuffer[SERVER_BUFFER_SIZE];
    int        nBuf = SERVER_BUFFER_SIZE;
    erIpcCmd_t command;
    int        page;
    int        iconID;
    int        iconState;

    browser = (Ereader *) data;
    erIpcGetMessage(theServerChannel, szBuffer, &nBuf);

    BR_IPCPRINTF("Received %s\n", szBuffer);

    if (vwrParseCommand(szBuffer, &command) >= 0)
    {
        switch (command.cc)
        {
            case ccVwrToolbar:
                iconID    = atoi(command.arg[0]);
                iconState = atoi(command.arg[1]);
                BR_IPCPRINTF("Toolbar iconID [%d] iconState [%d]", iconID, iconState);
                gdk_threads_enter();
                toolbar_key_selected(iconID, iconState, browser);
                gdk_threads_leave();
                break;
            case ccVwrStore:
                // no widget related requests => gdk_threads_enter() not needed
                contentlister_request(command, browser);
                break;
            case ccVwrJumpToPage:
                page = atoi(command.arg[1]);
                BR_IPCPRINTF("PageBar JumpToPage: %d", page);
                gdk_threads_enter();
                browser_pagebar_page_selected(page, browser);
                gdk_threads_leave();
            default:
                BR_IPCPRINTF("UNHANDLED COMMAND %d", command.cc);
                break;
        }
    }
    else
    {
        BR_WARNPRINTF("vwrParseCommand returned < 0\n");
    }
}
