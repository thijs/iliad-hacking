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
 * \file system.h
 * \brief content lister - eReader system calls
 * 
 * <File description>
 * 
 * This file provides system call functionality to execute commands and such
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2006-2008 iRex Technologies
 *
 */
#define PINCODE_MIN_LENGTH       4
#define PINCODE_MAX_LENGTH       9// 0x0FFFFFFF = 268435455

#define POWERMGMTIFACE   "/dev/battery"

#define BATTERY_IOCTL_BASE             'b'
#define BATTERY_IOCTL_READ_CURRENT     _IOR( BATTERY_IOCTL_BASE,16,unsigned int)


// Exec a command like the shell would. - func by p.blundell
int shell_exec (const char *cmd);

// Execute specified command with arguments in child process
int fork_exec (const int argc, char *const argv[]);

// Read Connect_After_Rewboot flag from sysset area
int scGet_Reconnect(gboolean* connect_after_reboot);

// Check whether battery is charging or not
gboolean is_battery_charging(void);

// Sysset access for pincode settings
gint sysset_read_pincode_onoff(gboolean* enable);
gint sysset_read_pincode_string(gchar* pincode);
gboolean sysset_is_pincode_empty();

// Read MAC address from sysset area
const char* get_sysset_macAddress(void);

// Delete file-system item: file, symlink or directory
gboolean delete_fsitem(const gchar* path);

