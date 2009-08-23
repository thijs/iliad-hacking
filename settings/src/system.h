/*
 * This file is part of settings.
 *
 * settings is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * settings is distributed in the hope that it will be useful,
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
 * \file system.h
 * \brief content lister - eReader system calls
 * 
 * <File description>
 * 
 * This file provides system call functionality to execute commands and such
 */

#define PINCODE_MIN_LENGTH       4
#define PINCODE_MAX_LENGTH       9// 0x0FFFFFFF = 268435455

/* Exec a command like the shell would. - func by p.blundell*/
int shell_exec (const char *cmd);

/* Read MAC address from sysset area */
const char *get_sysset_macAddress (int offset);
const char *get_wired_macAddress (void);
const char *get_wireless_macAddress(void);
const char *get_sysset_product_id(void);

/* Read MAC address from wired interface area */


gint sysset_read_pincode_onoff (gboolean * enbale);

gint sysset_write_pincode_onoff (gboolean enable);

gint sysset_read_pincode_string (gchar * pincode);
//
gint sysset_write_pincode_string (const gchar * pincode);

gboolean sysset_is_pincode_empty ();
const char *get_sysset_fa_model(void);
int get_sysset_fa_region(void);
const char *get_sysset_fa_oem_code(void);
const char *get_sysset_fa_country_code(void);
int get_sysset_pa_board_revision(void);

const char *get_sysset_epd_id(void);
