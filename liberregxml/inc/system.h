/*
 * This file is part of liberregxml.
 *
 * liberregxml is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberregxml is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file system.h
 * \brief liberreg - eReader system calls
 *
 * This file provides system call functionality to execute commands and such
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Read MAC address from sysset area */
const char *get_sysset_macAddress (void);

/* Read MAC address from wired interface area */
const char *get_wired_macAddress (void);

/* Read MAC address from wireless interface area */
const char *get_wireless_macAddress (void);

/* Read MAC address from wlan_mac.txt */
const char *get_wlanmactxt_macAddress(void);

const char *get_software_version_commercial(void);

const char *get_software_version(void);

// notes: need to free the memory retured by this function
// char* get_public_key(void);
int get_public_key(char **pubkey);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          // _SYSTEM_H_
