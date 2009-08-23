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
 * \file erConnect.h
 * \brief contentLister - connect finite state machine and process launcher
 *
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __ER_CONNECT_H__
#define __ER_CONNECT_H__

#include <pthread.h>

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

#define COMMAND_STRING_MAX 1024

typedef enum
{
    connectCmdNone,
    connectCmdStart,
    connectCmdStop
} connectCmd_e;

typedef enum
{
    connectStateIdle = 0,
    connectStateConnected,
    connectStateDisconnected
} connectState_e;

typedef enum
{
    connectScrUnchanged,
    connectScrDownloadHistory,
    connectScrMode,
    connectScrNews,
    connectScrBooks,
    connectScrDocs,
    connectScrNotes,
} connectScr_e;

typedef struct 
{
    pthread_mutex_t  mutex;
    connectCmd_e     command;
    connectState_e   state;
    connectScr_e     screen;
    gboolean         background;
    gboolean         connect_from_pincode;
    gboolean         connect_after_reboot;
    char             szCommand[COMMAND_STRING_MAX];
} connectStruct_t;

void erConnectInit(connectStruct_t *connect);

pid_t erConnectPid(void);


#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__ER_CONNECT_H__
