/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/********
 * Get HW ID from DOCFLASH
 * Copyright 2006-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 * Author: Daniel Wiermans
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "tffs.h"

#define TFFSDEVICE  "/dev/tffsa"

#if 0
void main(void)
{
    char Buffer[16];
    int i;

    get_hw_id(Buffer);
//  printf("\tUnique hardware ID = ");
    for(i=0; i<16; i++)
    {
        printf("%02X", Buffer[i]);
    }
    printf("\n");
}
#endif

int get_hw_id(char *Buffer)
{
    flIOctlRecord         ioctlRecord;
    flUniqueIdOutput      ID;
    int                   i;
    int                   docHandle;

    docHandle = open(TFFSDEVICE, O_RDWR);
    if(docHandle==-1) 
    {
        return -1;
    }
    
    ioctlRecord.inputRecord = NULL;
    ioctlRecord.outputRecord = &ID;
    memset(ID.id, 0x00, 16);
    
    i = ioctl(docHandle, FL_IOCTL_UNIQUE_ID, &ioctlRecord);
    if(i != 0)
    {
        return -2;
    }
    memcpy(Buffer, ID.id, 16);
    return i;
}
