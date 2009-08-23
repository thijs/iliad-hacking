/*
 * This file is part of powerMgr.
 *
 * powerMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * powerMgr is distributed in the hope that it will be useful,
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
 * \file notify.c
 * \brief clNotify 
 *
 */


/**
 * eReader CTL USB/CF event notifier
 * This program is called from the hotplug scripts and notifies the CL of a change of state on the USB or CF bus
 */
#include <strings.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>

// identification of the memory extensions (must be same as in contentlister control.h)
#define CF_ID    "CF"
#define SD_ID    "SD"
#define USB_ID   "USB"

static erClientChannel_t contentListerChannel;

static void usage(void)
{
    printf("Usage: clNotify <event> <present>\n"
        "	<event>: is CF, SD or USB\n"
		"	<present>: 1 or 0\n\n");
}

int main(int argc, char *argv[])
{
    char device[5] = "";
    int present = 0;
    
    if ( argc != 3 )
    {
        usage();
        return 1;
    }
    
    // Check 'present' parameter. If not used correctly, make it 0
    present = atoi(argv[2]);
    if (present != 1)
    {
        present = 0;
    }
    
    if (strcasecmp(CF_ID, argv[1]) == 0)
    {
        strcpy(device, CF_ID);
    }
    else if (strcasecmp(SD_ID, argv[1]) == 0)
    {
        strcpy(device, SD_ID);
    }
    else if (strcasecmp(USB_ID, argv[1]) == 0)
    {
        strcpy(device, USB_ID);
    }
    else
    {
        usage();
        return 1;
    }

#ifdef DEBUG
    printf("clNotify: %s is now %s\n", device, (present == 1) ? "present" : "removed");
#endif
    
    erIpcStartClient(erIpcGetChannel(ER_CONTENT_LISTER_UA_ID), &contentListerChannel);
    clStoragePresent(contentListerChannel, device, present);
    return 0;
}
