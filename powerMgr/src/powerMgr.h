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
 * eReader Power Management daemon
 */

#ifndef __POWERMGR_H
#define __POWERMGR_H

typedef enum {
    GONE=0,
    INSERTED,
    DEV_STATE_LAST
} dev_state;

typedef enum {
    UNPOWERED=0,
    POWERED,
    POWER_STATE_LAST
} power_state;

typedef struct {
    power_state ac97_amp;
    power_state mmc_ctrl;
    dev_state mmc_card;
} board_status_t;

typedef struct {
    time_t time;
    int MHz;
} pwrQ_item_t;

/* powerq is a linked list */
typedef struct lnode_rec {
    pwrQ_item_t * data;
    struct lnode_rec * next;
} lnode;

/* insert a new entry in a sorted linked list */
void llist_insert(lnode ** head, pwrQ_item_t * data);

/* pop the first entry of the linked list */
void llist_pop(lnode ** head);

/* delete a complete linked list */
void llist_delete(lnode ** head);

/* print a linked list */
void llist_print(lnode ** head);


// identification of the memory extensions (must be same as in contentlister control.h)
#define CF_ID    "CF"
#define SD_ID    "SD"
#define USB_ID   "USB"


#define POWERMGMTIFACE   "/dev/battery"
#define DEVEVTIFACE      "/dev/buttons"

// Percentage of battery left before warning
#define BATTERY_LOW_WARNING        12
#define BATTERY_LOW_SHUTDOWN       10
#define BATTERY_LOW_EMERGENCY       8

// Magic number representing battery is full
#define BATTERY_TIMELEFT_MAX    0xFFFF

// Threshold above which battery is charging
#define BATTERY_CHARGING        0


#define BUTTON_IOCTL_BASE          'b'
//#define BUTTON_IOCTL_WAIT_FOR_STATUSCHANGE _IOR( BUTTON_IOCTL_BASE,3, unsigned int)
#define BUTTON_IOCTL_READ_STATE    _IOR( BUTTON_IOCTL_BASE,4,unsigned int)
#define BUTTON_IOCTL_ENABLE_WACOM  _IOW( BUTTON_IOCTL_BASE,5,unsigned int)
#define BUTTON_IOCTL_DISABLE_WACOM _IOW( BUTTON_IOCTL_BASE,6,unsigned int)
#define BUTTON_IOCTL_GET_STATUS    _IOR( BUTTON_IOCTL_BASE,7,unsigned int)

#define BATTERY_IOCTL_BASE             'b'
#define BATTERY_IOCTL_READ_CHARGE      _IOR( BATTERY_IOCTL_BASE,1,unsigned int)
#define BATTERY_IOCTL_READ_TIME        _IOR( BATTERY_IOCTL_BASE,2,unsigned int)
#define BATTERY_IOCTL_READ_TEMPERATURE _IOR( BATTERY_IOCTL_BASE,3,unsigned int)
#define BATTERY_IOCTL_ENABLE_ETHERNET  _IOR( BATTERY_IOCTL_BASE,5,unsigned int)
#define BATTERY_IOCTL_DISABLE_ETHERNET _IOR( BATTERY_IOCTL_BASE,6,unsigned int)
#define BATTERY_GET_TURBO_MODE         _IOR( BATTERY_IOCTL_BASE,7,unsigned int)
#define BATTERY_SET_TURBO_MODE         _IOW( BATTERY_IOCTL_BASE,8,unsigned int)
#define BATTERY_IOCTL_ENABLE_WACOM     _IOR( BATTERY_IOCTL_BASE,9,unsigned int)
#define BATTERY_IOCTL_DISABLE_WACOM    _IOR( BATTERY_IOCTL_BASE,10,unsigned int)
#define BATTERY_IOCTL_ENABLE_CF        _IOR( BATTERY_IOCTL_BASE,11,unsigned int)
#define BATTERY_IOCTL_DISABLE_CF       _IOR( BATTERY_IOCTL_BASE,12,unsigned int)
#define BATTERY_IOCTL_ENABLE_MMC       _IOR( BATTERY_IOCTL_BASE,13,unsigned int)
#define BATTERY_IOCTL_DISABLE_MMC      _IOR( BATTERY_IOCTL_BASE,14,unsigned int)
#define BATTERY_IOCTL_READ_STATUS      _IOR( BATTERY_IOCTL_BASE,15,unsigned int)
#define BATTERY_IOCTL_READ_CURRENT     _IOR( BATTERY_IOCTL_BASE,16,unsigned int)

/* Power management IOCTLs Copy of linux/soundcard.h */
#define SNDCTL_POWER_OFF       _SIO('P', 99)
#define SNDCTL_POWER_ON        _SIO('P', 100)


#define GETEVENT(x) (x&0xFF)
#define GETPENDETECT(x) ((x&0x100)>>8)
#define GETLEDSTATUS(x) ((x&0x200)>>9)
#define GETCFDETECT(x)  ((x&0x400)>>10)
#define GETMMCDETECT(x) ((x&0x800)>>11)

#define SIOCIWFIRSTPRIV   0x8BE0
#define WLANIOCTL         SIOCIWFIRSTPRIV
#define WLANDEEPSLEEP     (WLANIOCTL + 27)

#define SERVER_BUFFER_SIZE 1024

#endif //__POWERMGR_H
