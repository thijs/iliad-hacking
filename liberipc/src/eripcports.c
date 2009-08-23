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


/** Outdated, replaced by macro in eripc.h **

#include "eripcports.h"

int erIpcGetServerPort(int channelIdx, int *port)
{
    switch (channelIdx)
    {
        case ER_TB_CHANNEL:
            *port = TB_IPC_PORT;
            break;
        case ER_TEST_CHANNEL:
            *port = TEST_IPC_PORT;
            break;
        case ER_PDF_VIEWER_CHANNEL:
            *port = PDF_VIEWER_IPC_PORT;
            break;
        case ER_XHTML_VIEWER_CHANNEL:
            *port = XHTML_VIEWER_IPC_PORT;
            break;
        case ER_APABI_VIEWER_CHANNEL:
            *port = APABI_VIEWER_IPC_PORT;
            break;
        case ER_ARINC_VIEWER_CHANNEL:
            *port = ARINC_VIEWER_IPC_PORT;
            break;
        case ER_CONTENTLISTER_CHANNEL:
            *port = CONTENTLISTER_IPC_PORT;
            break;
        case ER_BUSYD_CHANNEL:
            *port = BUSYD_IPC_PORT;
            break;
        case ER_POWERMGR_CHANNEL:
            *port = POWERMGR_IPC_PORT;
            break;
        case ER_PAGEBAR_CHANNEL:
            *port = PAGEBAR_IPC_PORT;
            break;
        case ER_SCRIBBLE_CHANNEL:
            *port = SCRIBBLE_IPC_PORT;
            break;
        case ER_SETUP_CHANNEL:
            *port = SETUP_IPC_PORT;
            break;
        case ER_CONNECT_CHANNEL:
            *port = CONNECT_IPC_PORT;
            break;
        default:
            return -1;
    }
    return 0;
}

** Outdated, replaced by macro in eripc.h **/
