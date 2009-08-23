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
 * \file powerMgr.c
 * \brief PowerMgr 
 *
 */


/**
 * eReader Powermanagement daemon
 */

#include <config.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <linux/soundcard.h>

#include <glib.h>

#include <liberdm/erdm.h>
#include <liberipc/eripcpowermgr.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>
#include <liberipc/eripctoolbar.h>
#include "powerMgr.h"
#include "toolbar.h"

#undef DEBUG
#ifdef DEBUG
#define DBG(x)  printf x
#else
#define DBG(x)
#endif
#define ERROR(x)  printf x

static erServerChannel_t theServerChannel;
static erClientChannel_t contentListerChannel;
static board_status_t    g_ereader;

// Enable CPU frequency scaling
#define ENABLE_CPU_THROTTLE

// Constants and data for CPU throttling
#define CPU_SPEED_LOW           "100000"       // cpu frequency in kHz
#define CPU_SPEED_HIGH          "400000"       // cpu frequency in kHz

#define IDLE_PERCENTAGE_SPEED_UP  50    // speed up CPU when idle percentage falls below this threshold
#define IDLE_PERCENTAGE_THROTTLE  95    // throttle down CPU when idle percentage comes above this threshold

#define SAMPLE_INTERVAL_LONG    1000    // in milliseconds
#define SAMPLE_INTERVAL_SHORT    100    // in milliseconds

#define MS_PER_IDLE_TICK          10    // number of milliseconds represented by one tick of the idle counter
#define IDLE_THRESHOLD_SPEED_UP  (SAMPLE_INTERVAL_SHORT * IDLE_PERCENTAGE_SPEED_UP / 100 / MS_PER_IDLE_TICK)
#define IDLE_THRESHOLD_THROTTLE  (SAMPLE_INTERVAL_LONG  * IDLE_PERCENTAGE_THROTTLE / 100 / MS_PER_IDLE_TICK)

static enum { e_cpu_low, e_cpu_high } g_cpu_speed = e_cpu_high;


static int getDeviceState(board_status_t * ereader, int evt_fd, int pwr_fd);


static int setAC97Amp(int enabled)
{
    int     mixer_device = 0;

    DBG(("Setting AC97 to %s\n", (enabled) ? "enabled" : "disabled"));
    mixer_device = open("/dev/dsp", O_RDWR);
    if (mixer_device == -1)
    {
	perror("opening mixer device.\n");
	return -1;
    }

    if(!enabled)
    {
        if (ioctl(mixer_device, SNDCTL_POWER_OFF))
        {
	    perror("Ioctl on mixer device");
        }
    }
    else
    {
        if (ioctl(mixer_device, SNDCTL_POWER_ON))
        {
	    perror("Ioctl on mixer device");
        }
    }

//    if (ioctl(audio_device, SOUND_MIXER_PRIVATE1, &i))
//    {
//	perror("Ioctl on audio device");
//	return -1;
//    }
    close(mixer_device);
    return 0;
}

static int setMMC(int enabled, int pwr_fd, board_status_t * ereader)
{
    DBG(("Setting MMC controller to %s\n", (enabled == POWERED) ? "enabled" : "disabled"));
    if (enabled == POWERED)
    {
	//    if ( 0 == ioctl (pwr_fd, BATTERY_IOCTL_ENABLE_MMC, &status))
        ereader->mmc_ctrl = POWERED;
    }
    else
    {
	//    if ( 0 == ioctl (pwr_fd, BATTERY_IOCTL_DISABLE_MMC, &status))
        ereader->mmc_ctrl = UNPOWERED;
    }
    return 0;
}

static void set_cpu_speed(const char *kHz)
{
#ifdef ENABLE_CPU_THROTTLE
    FILE  *fp;

    if (kHz == NULL)
    {
        return;
    }
    
    fp = fopen("/proc/sys/cpu/0/speed", "w");
    if (fp)
    {
        fputs(kHz, fp);
        fclose(fp);
    }
#else
	// Never set CPU speed
	return;
#endif
}


/**
 * This callback is invoked when there is data to read on file descriptor source_fd.
 * The channel info is passed via parameter 'data'
 */
void powerMgrMsgRxd(erServerChannel_t channel, int source_fd)
{
    char       szBuffer[SERVER_BUFFER_SIZE];
    int        nBuf = SERVER_BUFFER_SIZE;
    erIpcCmd_t command;
    int        enabled;
    int        MHz;

    erIpcGetMessage(channel, szBuffer, &nBuf);

    if (pwrParseCommand(szBuffer, &command) >= 0)
    {
	switch (command.cc)
	{
            case ccPwrAC97:
                enabled = atoi(command.arg[0]);
                setAC97Amp(enabled);
                break;

            case ccPwrIdleMode:
                MHz = atoi(command.arg[0]);
                if (MHz * 1000 == atoi(CPU_SPEED_LOW))
                {
                    set_cpu_speed(CPU_SPEED_LOW);
                    g_cpu_speed = e_cpu_low;
                }
                else if (MHz * 1000 == atoi(CPU_SPEED_HIGH))
                {
                    set_cpu_speed(CPU_SPEED_HIGH);
                    g_cpu_speed = e_cpu_high;
                }
                else
                {
                    printf("Invalid cpu speed requested [%s]\n", command.arg[0]);
                    set_cpu_speed(CPU_SPEED_HIGH);
                    g_cpu_speed = e_cpu_high;
                }
                break;

            default:
                printf("UNKNOWN COMMAND %d\n", command.cc);
                break;
	}
    }
}

int powerMgrInstallIpcServer()
{
    int     returnValue;
    int     fd = -1;
    fd_set  readfds;

    returnValue =
	erIpcOpenServerChannel(ER_POWERMGR_CHANNEL, &theServerChannel);

    if (returnValue != (-1))
    {
	fd = erIpcGetServerFd(theServerChannel);

	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);

	while (1)
	{
	    select(fd + 1, &readfds, NULL, NULL, NULL);
	    powerMgrMsgRxd(theServerChannel, fd);
	}
    }
    else
    {
	fprintf(stderr, "Could not open server channel\n");
	return -1;
    }
    return 0;
}


static guint64 get_cpu_idle_count(void)
{
    static guint64  idle_previous = 0;

    guint64  idle_current;
    guint64  idle_diff = 0;
    FILE     *fp;
    char     buf[200];
    char     *cp;

    fp = fopen("/proc/stat", "r");
    if (fp)
    {
        cp = fgets(buf, sizeof(buf), fp);
        if (cp == buf)
        {
            cp = strtok(buf, " ");
            if (strcmp(cp, "cpu") == 0)
            {
                cp = strtok(NULL, " ");
                cp = strtok(NULL, " ");
                cp = strtok(NULL, " ");
                cp = strtok(NULL, " ");
                idle_current = g_ascii_strtoull(cp, NULL, 10);

                idle_diff = idle_current - idle_previous;
                if (idle_diff < 0)
                {
                    printf("--> Illegal cpu idle count [%s] [%llu], previous [%llu]\n",
                                                        cp,  idle_current,    idle_previous);
                    idle_diff = 0;
                }

                idle_previous = idle_current;
            }
        }
        fclose(fp);
    }

    return idle_diff;
}

static void cpu_throttle(void)
{
    guint64  idle_diff;

    idle_diff = get_cpu_idle_count();
    if (idle_diff >= 0)
    {
        switch (g_cpu_speed)
        {
            case e_cpu_low:
                if (idle_diff <= IDLE_THRESHOLD_SPEED_UP)
                {
                    set_cpu_speed(CPU_SPEED_HIGH);
                    g_cpu_speed = e_cpu_high;
                }
                break;
            case e_cpu_high:
                if (idle_diff >= IDLE_THRESHOLD_THROTTLE)
                {
                    set_cpu_speed(CPU_SPEED_LOW);
                    g_cpu_speed = e_cpu_low;
                }
                break;
            default:
                ; // ignore
        }
    }
}


// charge_status = percentage 0 .. 100
// time_left     = minutes
// temperature   = 0.1 degrees Kelvin
static void check_battery_status(unsigned int charge_status, unsigned int time_left, short current)
{
    static int iconState_previous = 0;  // for battery iconState = 0 is grey
           int iconState;

    DBG(("check_battery_status(): charge [%u] time left [%u] current [%hd]\n", charge_status, time_left, current));

    // update toolbar icon: 0 - 20 - 40 - 60 - 80 - 100%
    // round  0 .. 14 downward
    // round 15 .. 19 upward
    if (charge_status >= 0  &&  charge_status <= 100)
    {
        iconState = 1 + ((charge_status + 4) / 20);
    }
    else
    {
        // invalid battery charge: icon grey
        iconState = 0;
    }

    // show toolbar icon
    if (iconState != iconState_previous)
    {
        iconState_previous = iconState;
        toolbar_setIconState(iconID_battery, iconState);
        usleep(200*1000);  // wait for toolbar to update the icon
        dmDisplay(dmCmdPriorLow, dmQTyping);
    }

    // tell contentlister when battery almost empty
    if ((charge_status <= BATTERY_LOW_WARNING) && (current < BATTERY_CHARGING))
    {
        ERROR(("Battery charge is %u, threshold is %u (time left is %u, current is %hd)\n", charge_status, BATTERY_LOW_WARNING, time_left, current));

        if (charge_status <= BATTERY_LOW_EMERGENCY)
        {
            // this should not happen, seems like contentlister did not respond to our shutdown message
            // do a forced shutdown now to avoid the battery gets completely discharged
            ERROR(("\n\n  --- EMERGENCY SHUTDOWN: battery charge is %d%% ---\n\n", charge_status));
            system("halt");
        }
        else if (charge_status <= BATTERY_LOW_SHUTDOWN)
        {
            // contentlister must shutdown the device immediately
            clBatteryLow(contentListerChannel, ccClBattery_shutdown, charge_status, time_left);
        }
        else
        {
            // contentlister must warn the user about the battery status
            clBatteryLow(contentListerChannel, ccClBattery_warning, charge_status, time_left);
        }
    }
}

static void* eventPoller(void *arg)
{
    static int   battery_poll_ticks = 20;  // delay first update till contentLister started
    static int   next_second_ticks  = 0;
    unsigned int battery_charge_status;
    unsigned int battery_time_left;
    unsigned int battery_current;
    short current;

    board_status_t* ereader = (board_status_t *) arg;
    int  evt_fd;
    int  pwr_fd;
    int  status;
    int  oldstatus;
    int  ret;

    // init event and power mgmt interfaces
    evt_fd = open(DEVEVTIFACE, O_RDWR | O_NONBLOCK);
    if (evt_fd < 0)
    {
	perror("Error opening /dev/buttons");
	pthread_exit(NULL);
    }

    pwr_fd = open(POWERMGMTIFACE, O_RDWR);
    if (pwr_fd < 0)
    {
	perror("Error opening /dev/battery");
	pthread_exit(NULL);
    }

    // Get initial device state
    oldstatus = getDeviceState(ereader, evt_fd, pwr_fd);

    // Initialise cpu speed
    set_cpu_speed(CPU_SPEED_HIGH);
    g_cpu_speed = e_cpu_high;

    while (1)
    {
        if (g_cpu_speed == e_cpu_low)
        {
            // low cpu frequency: short interval so we can speed up when cpu load changes
            usleep(SAMPLE_INTERVAL_SHORT * 1000L);
            cpu_throttle();
            if (--next_second_ticks > 0)
            {
                continue;  //next while iteration
            }
        }
        else
        {
            // high cpu frequency: long interval so we do not disturb other processing
            usleep(SAMPLE_INTERVAL_LONG * 1000L);
            cpu_throttle();
        }
        next_second_ticks = (1000 / SAMPLE_INTERVAL_SHORT);

        /* Check the battery status */
        if (--battery_poll_ticks <= 0)
        {
            battery_poll_ticks = 60;  // seconds

            // battery measurement works reliably only at full cpu speed
            if (g_cpu_speed == e_cpu_low)
            {
                set_cpu_speed(CPU_SPEED_HIGH);
            }

            // read battery status
            if (ioctl(pwr_fd, BATTERY_IOCTL_READ_CHARGE, &battery_charge_status) == -1)  // percentage 0 ... 100
            {
                perror("ioctl read charge failed");
            }
            if (ioctl(pwr_fd, BATTERY_IOCTL_READ_TIME, &battery_time_left) == -1)   // in minutes, 65535 = charging
            {
                perror("ioctl read time failed");
            }
            if (ioctl(pwr_fd, BATTERY_IOCTL_READ_CURRENT, &battery_current) == -1)  // in milliamps, positive or negative
            {
                perror("ioctl read current failed");
            }
            current = battery_current & 0xFFFF; // convert from unsigned int to short
            
            // back to low cpu speed when needed
            if (g_cpu_speed == e_cpu_low)
            {
                set_cpu_speed(CPU_SPEED_LOW);
            }

            // check battery status and set toolbar icon
            check_battery_status(battery_charge_status, battery_time_left, current);
        }
        
        /* Check the buttons */
        ret = ioctl(evt_fd, BUTTON_IOCTL_GET_STATUS, &status);
        if (ret < 0)
        {
            perror("ioctl invalid status");
        }
        else
        {
            //DBG(("decode status: [P = %x] [CF = %x] [MMC = %x]\n", GETPENDETECT(status), GETCFDETECT(status), GETMMCDETECT(status)));
    
            if (GETMMCDETECT(status) ^ GETMMCDETECT(oldstatus))
            {
                if (GETMMCDETECT(status))
                {
                    ERROR(("MMC card inserted\n"));
                    ereader->mmc_card = INSERTED;

                    /* a card was inserted, mount it and tell contentLister */
                    if (ereader->mmc_ctrl == UNPOWERED)
                    {
                        setMMC(POWERED, pwr_fd, ereader);
                    }
                    system("/usr/bin/mountmmc.sh");
                    clStoragePresent(contentListerChannel, SD_ID, 1);
                }
                else
                {
                    ERROR(("MMC card gone\n"));
                    ereader->mmc_card = GONE;

                    /* a card was removed, unmount it and tell contentLister */
                    system("/usr/bin/umountmmc.sh");
                    clStoragePresent(contentListerChannel, SD_ID, 0);
                    if (ereader->mmc_ctrl == POWERED)
                    {
                        setMMC(UNPOWERED, pwr_fd, ereader);
                    }
                }
            }
        }
	oldstatus = status;
    } // while

    /* cleanup */
    close(evt_fd);
    close(pwr_fd);

    pthread_exit(NULL);
}

/**
 * Query device state at boot time
 */
static int getDeviceState(board_status_t * ereader, int evt_fd, int pwr_fd)
{
    int     ret, status;

    ret = ioctl(evt_fd, BUTTON_IOCTL_GET_STATUS, &status);
    if (ret < 0)
        perror("ioctl on event interface");

    DBG(("Got initial state 0x%x\n", status));

    ereader->ac97_amp = POWERED;
    DBG(("- AC97 powered on\n"));

    if (GETMMCDETECT(status))
    {
        ereader->mmc_card = INSERTED;
        ereader->mmc_ctrl = POWERED;
        DBG(("- MMC card present, MMC controller powered\n"));
        /* MMC present at boot time, mount it! */
        system("/usr/bin/mountmmc.sh");
    }
    else
    {
        ereader->mmc_card = GONE;
        ereader->mmc_ctrl = POWERED;
        DBG(("- MMC card gone, MMC controller powered\n"));
    }
    return status;
}


int main(int argc, char *argv[])
{
    pthread_t pollThread;
    //pthread_t timerThread;
    pthread_attr_t my_attr;

    toolbar_init();
    
    erIpcStartClient(erIpcGetChannel(ER_CONTENT_LISTER_UA_ID), &contentListerChannel);

    /* start polling thread */
    if (0 != pthread_attr_init(&my_attr))
    {
	perror("pthread_attr_init");
    }
    if (0 != pthread_attr_setdetachstate(&my_attr, PTHREAD_CREATE_DETACHED))
    {
	perror("thread set detach state");
    }
    if (0 != pthread_create(&pollThread, &my_attr, eventPoller, (void *) &g_ereader))
    {
	perror("pthread_create");
    }

    /* setup ipc */
    powerMgrInstallIpcServer();

    return 0;			/* we never get here */
}

