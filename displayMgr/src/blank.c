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

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>

#define FBIO_IOCTL_BASE           'v'
#define FBIO_ERASE_WHITE          _IOW(FBIO_IOCTL_BASE, 2, struct display_update_info)
#define WAVEFORM_4BPP_IMAGE       1

struct display_update_info
{
	int waveform;
	int sequence;
};

	
int main(int argc, char *argv[])
{
    int fbdev;
    int ret;
    struct display_update_info updateinfo;

    fbdev = open ("/dev/fb0", O_RDWR);
	
    if (fbdev == -1)	
    {
        printf("Error opening framebufferdevice. # mknod /dev/fb0 c 29 0\n");
        exit(-1);
    }

    updateinfo.waveform = WAVEFORM_4BPP_IMAGE;
    updateinfo.sequence = 0;
        
    if ((ret=ioctl(fbdev, FBIO_ERASE_WHITE, &updateinfo))) 
    { 
        printf("Error sending FBIO_ERASE_WHITE: %x\n",ret); 
        exit(-1); 
    }

    close(fbdev);

    return 0;
}
