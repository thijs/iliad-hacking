/*
 * This file is part of displayMgr.
 *
 * displayMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * displayMgr is distributed in the hope that it will be useful,
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include "liberdm/erdm.h"

// This test program assumes a running displayMgr server.

#define ERDMG_LOGGING_ON 1
#define ERDMG_WARNING_ON 1
#define ERDMG_ERROR_ON 1

#if (ERDMG_LOGGING_ON)
#define ERDMG_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERDMG_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERDMG_WARNING_ON)
#define ERDMG_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERDMG_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERDMG_ERROR_ON)
#define ERDMG_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERDMG_ERRORPRINTF(x, args...) do {} while (0)
#endif


void displayHelp()
{
    printf("Usage: displayMgrClient -<option> <argument>\n");
    printf("  no arguments : send messages to local loop\n");
    printf("  -d <dotted decimal ip address> : send messages to ip address\n");
    printf("  -h : display this message\n");
    printf("  -p : test of partial update\n");
    printf("  -r <seconds> : repeat a full update every <seconds>\n");
    printf("  -t : update toolbar area\n");
}

int main(int argc, char **argv)
{
    char    szDestination[INET_ADDRSTRLEN];
    char    szLocalLoop[] = "127.0.0.1";
    int     fUserAddress = 0;
    int     fRepeat = 0;
    int     fTest = 0;
    int     fToolbarUpdate = 0;
    int     nArg = 1;
    int     ret;
    int     nRepeatAfterSeconds = 0;

    // Send messages to local loop
    fUserAddress = 0;

    // Work our way through all possible options.
    // Currently options are of the form 
    // -option <argument>
    // -option
    while (nArg < argc)
    {
        if (argv[nArg][0] == '-')
        {
            switch (argv[nArg][1])
            {
            case 'h':
                displayHelp();
                return 0;
                break;

            case 'd':
                fUserAddress = 1;
                // What address ?
                if (++nArg >= argc)
                {
                    // Not enough arguments supplied.
                    ERDMG_ERRORPRINTF("Supply a dotted decimal ip address after option -d");
                    displayHelp();
                    return -1;
                }
                else
                {
                    strncpy(szDestination, argv[nArg], INET_ADDRSTRLEN);
                    nArg++;
                }
                break;

            case 'r':
                fRepeat = 1;
                // After how much seconds ?
                if (++nArg >= argc)
                {
                    // Not enough arguments supplied.
                    ERDMG_ERRORPRINTF("Supply seconds after option -r");
                    displayHelp();
                    return -1;
                }
                else
                {
                    char   *pDummy;
                    long    lSeconds;

                    lSeconds = strtol(argv[nArg], &pDummy, 10);
                    if (pDummy == argv[nArg])
                    {
                        // Conversion failed.
                        ERDMG_ERRORPRINTF("Please specify a positive integer larger than 1 after -r option.");
                        displayHelp();
                        return -1;
                    }
                    else if (lSeconds < 1)
                    {
                        // Period too small
                        ERDMG_ERRORPRINTF("Please specify a positive integer larger than 0 after -r option.");
                        displayHelp();
                        return -1;
                    }
                    else
                    {
                        nRepeatAfterSeconds = (int) lSeconds;
                    }
                    nArg++;
                }
                break;

            case 'p':
                fTest = 1;
                nArg++;
                break;

            case 't':
                fToolbarUpdate = 1;
                nArg++;
                break;

            default:
                ERDMG_ERRORPRINTF("Option %s not known.", argv[nArg]);
                displayHelp();
                return -1;
            }
        }
        else
        {
            ERDMG_ERRORPRINTF("Argument supplied not proceded by option.");
            displayHelp();
            return -1;
        }
    }

    if (fUserAddress == 1)
    {
        ERDMG_WARNPRINTF("Server address %s", szDestination);
    }
    else
    {
        strncpy(szDestination, szLocalLoop, INET_ADDRSTRLEN);
        ERDMG_WARNPRINTF("Server address %s", szDestination);
    }

    ret = dmMessageInit(szDestination);
    if (ret != 0)
    {
        ERDMG_ERRORPRINTF("Failed to initialize messaging.");
        return -1;
    }

    if (fTest == 1 || fToolbarUpdate == 1)
    {
        if (fTest == 1)
        {
            // Partial update test
            dmDisplayPartial(dmCmdPriorNormal, dmQFull, 100, 100, 300, 300);
            sleep(4);
            dmDisplayPartial(dmCmdPriorNormal, dmQFull, 500, 700, 700, 900);
            sleep(4);
        }
        if (fToolbarUpdate == 1)
        {
            // Update toolbar area
            dmDisplayPartial(dmCmdPriorNormal, dmQFull, 4, 996, 700, 1020);
            sleep(4);
        }
    }
    else
    {
        dmDisplay(dmCmdPriorNormal, dmQFull);
    }

    if (fRepeat == 1)
    {
        while (1)
        {
            dmDisplay(dmCmdPriorUrgent, dmQTyping);
            sleep(nRepeatAfterSeconds);
        }
    }

    return 0;
}
