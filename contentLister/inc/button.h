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

/**
 * \file button.h
 * \brief content lister - eReader button handling                  
 * 
 * <File description>
 * 
 * Depending on the configure value "enable-command-line-input" the button input will come
 * from the command line (number 1 till 11) or from the actual button driver  
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#include <linux/ioctl.h>

#ifndef __CL_BUTTON__H__
#define __CL_BUTTON__H__

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

#define FIFO_NAME	"/tmp/my_fifo"
#define DEVBUTTON	"/dev/buttons"

//long keypress key values that are used
// GDK_F1   --> PAGETURN_FORWARD LONG
// GDK_F2   --> PAGETURN_BACK LONG
// GDK_F3   --> NAVIGATE_3 LONG
// GDK_F4   --> NAVIGATE_1 LONG
// GDK_F5   --> OVERVIEW SHORT (This has changed from LONG to short, LONG is now GDK_Home)
// GDK_Home --> OVERVIEW LONG

/* these are the defines from the kernel button driver */
/* TODO: Please move this away to where they belong: in the button driver header file!!! */
#define CONNECT			(0)
#define OVERVIEW		(2)
#define MODE			(1)
#define NEWSPAPER		(3)
#define BOOKS			(4)
#define DOCUMENTS		(5)
#define NOTES			(6)
#define PAGETURN_FORWARD	(7)
#define PAGETURN_BACK		(8)
#define NAVIGATE_1		(9)
#define NAVIGATE_2		(10)
#define NAVIGATE_3		(11)
#define CRADDLE_SYNC		(12)
#define PEN_DETECT		(13)
#define POWER			(14)

#define NUM_BUTTONS	15
#define BUTTON_LONG_PRESS	0x1000
#define BUTTON_LONG_PRESS_TIMEOUT	500 //timeout defined in ms
#define KEYBUFFERING_OFF_TIMEOUT 3000

#define BUTTON_IOCTL_BASE		'b'
#define BUTTON_IOCTL_WRITE_INTERVAL	_IOW( BUTTON_IOCTL_BASE,1,unsigned int)
#define BUTTON_IOCTL_WRITE_ACTIVITY_LED	_IOW( BUTTON_IOCTL_BASE,2,unsigned int)
#define BUTTON_IOCTL_GET_STATUS		_IOR( BUTTON_IOCTL_BASE,7,unsigned int) 

#define GETBUTTON(x) (x&0xFF)
#define GETPENDETECT(x) ((x&0x100)>>8)
#define GETLEDSTATUS(x) ((x&0x200)>>9)
/* end of kernel defines */

#define MAX_COMMAND_SIZE		4

#define MAX_KEYCLICKS_WHEN_LOCKED   5
 

/**
 * Start up button handling thread
 *
 * @param data value that needs to be passed in the button handling 
 *             routines
 *
 * @returns -1 in case of failure
 */
int button_init(gpointer data);

/**
 * Block/unblock all keys
 *
 * @param block - TRUE/FALSE indicates all keys must be blocked/unblocked
 *
 * @returns -
 */
void button_block_all_keys(gboolean do_block);

/**
 * read the value of flipbar direction from er_registry.txt
 *
 * @param 
 *
 * @returns -
 */
void flipbarDirectionSettingInit();

/**
 * enable/disable key buffering
 *
 * @param enable - TRUE/FALSE indicates enable key buffering
 *
 * @returns -
 */
void button_enable_key_buffering(gboolean enable);

/**
 * read the value of key buffering from er_registry.txt
 *
 * @param 
 *
 * @returns -
 */
void keybufferingSettingInit();


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  //__CL_BUTTON__H__
