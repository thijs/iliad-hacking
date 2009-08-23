/*
 * This file is part of pcshareMgr.
 *
 * pcshareMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pcshareMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

 /**
 * \file constants.h
 * \brief Constant definitions for PC Share Manager
 * 
 */

/* pcshareMgr - A GTK based PC Share Manager on the eReader platform
 * Copyright (C) 2006 iRex Technologies BV
 *
 */

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H___

// For the time being, do it here
#define SHARE_CATEGORY_GROUP         "SHARE_CATEGORY"

// Battery charge required for specific actions
#define BATTERY_MINCHARGE_SOFTWAREDOWNLOAD  80
#define BATTERY_MINCHARGE_SOFTWAREINSTALL   60
#define BATTERY_MINCHARGE_BACKGROUND        15

#define NO_ERROR 0
#define SOME_ERROR 1

// Return values
#define SHARE_CATEGORY_LOCATION_ERROR    3
#define DOWNLOAD_HISTORY_LOCATION_ERROR  4
#define SHARE_NOT_MOUNTED                5
#define SHARE_THREAD_ABORTED             6

#define PROCESS_BUFFER_MAX      (2*1024)
#define MAX_SW_IMAGE_FILENAME   256
#define MAX_SHELL_COMMAND       1024
#define EUI64_BUFFER_MAX        256
#define MAX_PATH                1024
#define MAX_MISC_BUFFER         512
#define MAX_ERROR_MSG           512

#endif // __CONSTANTS_H__
