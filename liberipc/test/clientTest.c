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
#include <eripc.h>
#include <eripcclient.h>
#include <eripctoolbar.h>

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

main()
{
    erClientChannel_t channel;
    char    szBuffer[TB_MAXCHARONLINE];
    erIpcCmd_t  cmd;
    int     nRet;

    printf("Starting clientTest.\n");

    erIpcStartClient(ER_TEST_CHANNEL, &channel);

    // This test program reads from the keyboard and sends the interactive messages.
    while (fgets(szBuffer, TB_MAXCHARONLINE, stdin) != NULL)
    {
        int     nLen;

        // lineNr++;

        if (szBuffer[0] == '#')
        {
            // Ignore a comment line;
            continue;
        }
        // Strip the newline
        nLen = strlen(szBuffer);
        if (nLen > 0)
        {
            if (szBuffer[nLen - 1] == '\n')
            {
                szBuffer[nLen - 1] = '\0';
            }
        }

        // Ignore empty lines
        if (nLen != 0)
        {
            nRet = tbParseCommand(szBuffer, &cmd);
	    if ( nRet >= 0 )
	      {
		erIpcSndCommand(channel, &cmd);
	      }
        }
    }


}
