/*
 * This file is part of erbusy.
 *
 * erbusy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * erbusy is distributed in the hope that it will be useful,
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

#include <liberipc/eripc.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define BUFSIZE 1024

#define BUTTON_IOCTL_BASE		'b'
#define BUTTON_IOCTL_WRITE_ACTIVITY_LED	_IOW( BUTTON_IOCTL_BASE,2,unsigned int)

static eCcBusyState busy = ccBusyState_Blink;
static erClientChannel_t contentListerChannel;

static int execCommand(erIpcCmd_t * cmd)
{
    switch ((int) cmd->cc)
    {
        case ccBusySetBusy:
            if (busy != ccBusyState_Shutdown)
            {
                busy = atoi(cmd->arg[0]);
                if (busy < 0  ||  busy >= ccBusyState_Undefined)
                {
                    busy = ccBusyState_Off;
                }
                //printf("-- ccBusySetBusy [%d]\n", busy);
				clBusyIndicator(contentListerChannel, busy);			
            }
            break;
            
        case ccBusyUndefined:
        default:
            // ignore
            break;
    }
}

void serviceCallback(char *szBuffer, int *nBuf, void *data)
{
    erIpcCmd_t command;

    if (busyParseCommand(szBuffer, &command) >= 0)
    {
        execCommand(&command);
    }
}

void* testIPC(void *arg)
{
    char    szBuffer[BUFSIZE];
    int     nBuf = BUFSIZE;
    int     nRet = 0;

    nRet = erIpcStartServer(ER_BUSYD_CHANNEL, serviceCallback, szBuffer, &nBuf, NULL);

    return (void*)nRet;
}

void switch_led(int device, int led_status)
{
    int ret = ioctl(device, BUTTON_IOCTL_WRITE_ACTIVITY_LED, &led_status);
    if (ret == -1)
    {
        printf("Error writing led %d\n", led_status);
    }
}

int main(int argc, char *argv[])
{
    pthread_t     erTid;
    int           button_device;
    int           blink = 0;
    eCcBusyState  wasbusy = ccBusyState_Undefined;
    int           sleep = 100*1000;
    int           ret = 0;
	
    if (pthread_create(&erTid, NULL, testIPC, (void *) "1") != 0)
    {
        printf("%s %s: ", __FILE__, __FUNCTION__);
        perror("Could not create thread\n");
    }
    // setup the client channel of contentlister
    ret = erIpcStartClient(ER_CONTENTLISTER_CHANNEL, &contentListerChannel);
	if (ret != 0)
        printf("erIpcStartClient returned %d", ret);
	
    button_device = open("/dev/buttons", O_RDWR);
    if (button_device == -1)
    {
        printf("Error opening button device\n");
        exit(-1);
    }
		
    while (1)
    {
        sleep = 100*1000;

        switch (busy)
        {
            case ccBusyState_Blink:
                switch_led(button_device, blink);
                blink = !blink;
                sleep = 40*1000;
                break;

            case ccBusyState_On:
                if (wasbusy != ccBusyState_On)
                {
                    switch_led(button_device, 1);
                }
                break;

            case ccBusyState_Shutdown:
                if (wasbusy != ccBusyState_Shutdown)
                {
                    switch_led(button_device, 1);
                }
                break;

            case ccBusyState_Off:
            default:
                if (wasbusy != ccBusyState_Off)
                {
                    switch_led(button_device, 0);
                }
        }

        wasbusy = busy;
        usleep(sleep);
    }

    return 0;
}
