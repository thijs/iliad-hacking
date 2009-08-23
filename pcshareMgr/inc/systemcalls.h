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
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

 /**
 * \file systemcalls.h
 * \brief control - application interface with miscelaneous shell scripts and programs
 * 
 * <File description>
 *
 */

#ifndef _SYSTEMCALLS_H_
#define _SYSTEMCALLS_H_

#include "constants.h"
#include <gtk/gtk.h>

#define MAX_ARGS           (10)
#define MAX_FILENAME_SIZE  (1024)

// Sysset area layout
//
//     OS_Ready2Update
#define SA_OS_ADDRESS      (152)
#define SA_OS_LENGTH       (1)
int scSyssetWrite_OSUpdate(gboolean ready_to_update);
//
//     Connect_After_Reboot
#define SA_CONNECT_ADDRESS (153)
#define SA_CONNECT_LENGTH  (1)
int scSyssetWrite_FSUpdate(gboolean ready_to_update);
//
//     FS_Ready2Update
#define SA_FS_ADDRESS      (156)
#define SA_FS_LENGTH       (1)
int scSyssetWrite_Connect(gboolean connect_after_reboot);

// Miliseconds to wait before updating progress
#define PROGRESS_CALLBACK_TIMEOUT   (5000)

// Battery interface
#define POWERMGMTIFACE   "/dev/battery"

#define BATTERY_IOCTL_BASE         'b'
#define BATTERY_IOCTL_READ_CHARGE  _IOR( BATTERY_IOCTL_BASE,1,unsigned int)
#define BATTERY_IOCTL_READ_CURRENT _IOR( BATTERY_IOCTL_BASE,16,unsigned int)

    
int scCheckFileIntegrity(char *szDir, char *szFile);

/**
 * Get the available diskspace on the 'protected' partition
 *
 * @param diskSpaceFree contains the available diskspace in bytes (>= 0)
 *
 */
double scGetDiskSpaceFree(const char* path);

/**
 * Unpack the archive szDir/szFile into directory szDir
 *
 * @param szDir directory from where the archive will be unpacked
 * @param szFile archive path either absolute or relative from szDir
 * @param dlmHistory
 *
 */
typedef void scUnpackProgressCallback_t(int Progress);

//int scUnpackArchive(char *szDir, char *szFile, dlmHistory_t *dlmHistory, scUnpackProgressCallback_t *pProgressFunction);

void setDownloadHistory(char *dir);

int scEmptyDirectory(char *szDir);

void scRemoveFile(char *szDir, char *szFile);

void scFlushFileBuffers();

int shell_exec(const char *cmd);

int fork_exec(const int argc, char *const argv[]);

int recursive_dircopy(const char * source, const char * dest, GSourceFunc progress_callback);

int fileExists(const char* filename);

int delDir(const char* sDir);

char *dirCopy(const char* dirPath, const char* destinationDir, GSourceFunc progress_callback);

char *fileCopy(const char* filePath, const char* destinationDir, GSourceFunc progress_callback);
void fileCopy_abort(void);

const int mkdirhier(const char *path, mode_t mode);

void createDownloadHistoryEntry(const char *filePath);

char * strtolower(register char *s);

pid_t get_forkexec_child_pid();

/**
 * Retrieve the current battery charge
 *
 * @return 0 .. 100 current battery charge percentage
 */
int get_battery_charge(void);

#endif // _SYSTEMCALLS_H_
