/*
 * This file is part of pageBar.
 *
 * pageBar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pageBar is distributed in the hope that it will be useful,
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

#include <gtk/gtk.h>
#include <stdlib.h>
#include <liberipc/eripc.h>
#include <liberipc/eripcpagebar.h>
#include <liberipc/eripcviewer.h>

erClientChannel_t channel;

#define UAID ER_APABI_VIEWER_CHANNEL

void serviceCallback(gpointer data, gint source_fd, GdkInputCondition condition)
{
    char    szBuffer[1024];
    int     nBuf = 1024;
    erIpcCmd_t command;

    erIpcGetMessage((erServerChannel_t) data, szBuffer, &nBuf);
    printf("Received %s\n", szBuffer);
    if (vwrParseCommand(szBuffer, &command) >= 0) {
      switch (command.cc) {
        case ccVwrJumpToPage:
          printf("PAGE CHANGE TO PAGE %d\n", atoi(command.arg[1]));
          break;
        default:
          break;
      }
    }
}

void installIpcServer()
{
    int     nRet = 0;
    int     fd = -1;
    erServerChannel_t channel;

    nRet = erIpcOpenServerChannel(ER_APABI_VIEWER_CHANNEL, &channel);
    fd = erIpcGetServerFd(channel);
    // gtk specific mechanism to monitor input file descriptor.
    gdk_input_add(fd, GDK_INPUT_READ, serviceCallback, (gpointer) channel);
}

int main(int argc, char* argv[]) {
    /* init threads */
    g_thread_init(NULL);
    gdk_threads_init();

    erIpcStartClient(ER_PAGEBAR_CHANNEL, &channel);
    installIpcServer();
    
    pbReset(channel, UAID);
    pbSetPageCount(channel, UAID, 50);
    pbSetCurrentPageOffset(channel, UAID, 60);
    pbSetDrawAreaVOrigin(channel, UAID, 1020);
    pbRedraw(channel, UAID);
    
    
    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();
    return 0;
}
