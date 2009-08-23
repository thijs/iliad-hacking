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
 * \file erConnect.c
 * \brief connect finite state machine and process launcher
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2006-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#include <liberdm/display.h>
#include <liberregxml/erregapi.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "button.h"
#include "erConnect.h"
#include "erMSDisk.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "programManager.h"
#include "regValue.h"
#include "toolbar.h"
#include "timedids.h"

#define KILL_TIMEOUT   5*1000*1000// in suseconds_t
#define timerclear(tvp) ((tvp)->tv_sec = (tvp)->tv_usec = 0)

// local data
static pid_t g_connect_pid = 0;

// local functions
static void *connectThread(void *arg);


void erConnectInit(connectStruct_t * connect)
{
    connect->command              = connectCmdNone;
    connect->state                = connectStateIdle;
    connect->screen               = connectScrDownloadHistory;
    connect->background           = FALSE;
    connect->connect_from_pincode = FALSE;
    connect->connect_after_reboot = FALSE;
    connect->szCommand[0]         = '\0';

    if (pthread_mutex_init(&(connect->mutex), NULL) )
    {
        CL_ERRORPRINTF("Could not initialize connect mutex");
        perror("Could not initialize mutex");
    }
    else
    {
        CL_ERRORPRINTF("Lock the connect mutex");
        pthread_mutex_lock( &(connect->mutex) );
    }

    // Start the download thread
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, connectThread, (void *) connect) != 0)
    {
        CL_CTPRINTF("%s %s: ", __FILE__, __FUNCTION__);
        perror("Could not create thread\n");
    }

    timed_ids_init();
}

pid_t erConnectPid(void)
{
    return g_connect_pid;
}

// This thread is blocked on a mutex, unless there is an ongoing
// download session. 
static void *connectThread(void *arg)
{
    connectStruct_t *connect = (connectStruct_t *) arg;
    struct timeval tv_kill, tv;
    unsigned int stop = 0;// a flag to indicate whether connectThread receives connectCmdStop
    unsigned int first = 1;// aflag to indicate this is the first time we send SIGKILL

    CL_CTPRINTF("enter");

    pthread_mutex_lock( &(connect->mutex) );

    CL_CTPRINTF("enter active part");
    // initialize timevals
    timerclear(&tv_kill);
    timerclear(&tv);

    while (1)
    {
        switch (connect->state)
        {
            case connectStateIdle:
            {
                if (connect->command == connectCmdStart)
                {
                    CL_CTPRINTF("connectCmdStart");
                    connect->command = connectCmdNone;

                    switch (g_connect_pid = fork())
                    {
                        case 0:
                        {
                            // child process
                            int     nRet;

                            CL_WARNPRINTF("Calling: [%s]", connect->szCommand);
                            nRet = shell_exec(connect->szCommand);
                            CL_ERRORPRINTF("shell_exec of [%s] failed (returned [%d]), " 
                                           "cleaning up child\n", connect->szCommand, nRet);
                            perror("Error upon calling shell_exec");
                            fflush(stderr);
                            exit(1);
                            break;
                        }
                            
                        case -1:
                            // error
                            CL_ERRORPRINTF("Can't fork");
                            connect->state = connectStateDisconnected;
                            break;

                        default:
                            // parent process
                            connect->state = connectStateConnected;
                    }
                }
                else if (connect->command == connectCmdNone)
                {
                    // nothing to do -> wait for next command
                    CL_CTPRINTF("connectCmdNone");
                    pthread_mutex_lock( &(connect->mutex) );
                    // initialize these flags
                    stop = 0;
                    first = 1;
                    // initialize timevals
                    timerclear(&tv_kill);
                    timerclear(&tv);
                }
                else
                {
                    CL_ERRORPRINTF("unexpected connectCmd [%d]", connect->command);
                    connect->command = connectCmdNone;
                }
                break;
            }// case connectStateIdle

            case connectStateConnected:
            {
                pid_t   retPid;
                int     status;

                if (connect->command == connectCmdStop)
                {
                    CL_CTPRINTF("connectCmdStop: pid [%d]", g_connect_pid);
                    connect->command = connectCmdNone;

                    if (kill(g_connect_pid, SIGTERM) == -1)
                    {
                        CL_ERRORPRINTF("Failed to stop downloadMgr");
                        perror("Error in kill");
                        connect->state = connectStateDisconnected;
                    }
                    // store the time when killing connectionMgr and downloadMgr
                    gettimeofday(&tv_kill, NULL);
                    // set the 'stop' flag to be 1
                    stop = 1;

                    break;
                }

                // Child process still alive ...
                retPid = waitpid(g_connect_pid, &status, WNOHANG);
                if (retPid == 0)
                {
                    CL_CTPRINTF("connectionMgr alive");
                    
                    unsigned int kill_force = 0;
                    // check whether we need to killall -9 connectionMgr
                    if (stop)
                    {
                        gettimeofday(&tv, NULL);
                        suseconds_t time_diff = ((tv.tv_sec - tv_kill.tv_sec) * 1000 * 1000) + (tv.tv_usec - tv_kill.tv_usec);
                        if (time_diff > KILL_TIMEOUT) { kill_force = 1; }
                        CL_CTPRINTF("elapsed %d suseconds_t", (int)time_diff);
                    }
                   
                    if (first && kill_force)
                    {
                        // need to killall -9 connectionMgr
                        CL_CTPRINTF("Sending 'SIGKILL' to stop connectionMgr.");
                        // set the flag to ensure we only run killall -9 once
                        first = 0;
                        if (kill(g_connect_pid, SIGKILL) == -1)
                        {
                            CL_ERRORPRINTF("Failed to kill -9 connectionMgr");
                            perror("Error in kill");
                            connect->state = connectStateDisconnected;
                        }
                    }
                    else
                    {
                        // ... yes -> check again later
                        CL_CTPRINTF("check again later");
                        sleep(1);
                    }
                }
                else
                {
                    // ... no -> we are disconnected
                    connect->state = connectStateDisconnected;
                }
                break;
            }// connectStateConnected

            case connectStateDisconnected:
            {
                CL_CTPRINTF( "connectStateDisconnected: background [%d] after_reboot [%d] screen [%d]",
                             connect->background, connect->connect_after_reboot, connect->screen       );
                connect->state = connectStateIdle;

                gdk_threads_enter();
                ctrl_cleanup_download_history();
                ctrl_background_connect_timeout_start(FALSE);
                ctrl_set_connect_icon();
                if ( connect->background )
                {
                    if (getListerState() != STATE_PINCODE)
                    {
                        erStartMSDiskApp();
                    }
                    toolbar_synchronise();
                }
                else
                {
                    if (connect->connect_after_reboot)
                    {
                        setListerState(STATE_NORMAL);
                        pm_setUaOnTop(CONTENTLISTER_UAID);
                        toolbar_restore();
                        ctrl_startup();
                    }
                    else if (connect->connect_from_pincode)
                    {
                        // if users press connect button at pincode screen , 
                        // after finishing connected and downloaded, we need return
                        // back to pincode screen, not download history
                        ctrl_connect_back_to_pincode_screen();
                    }
                    else
                    {
                        setListerState(STATE_NORMAL);
                        erStartMSDiskApp();
                        toolbar_restore();

                        switch (connect->screen)
                        {
                            case connectScrUnchanged:
                                CL_CTPRINTF("connectScrUnchanged");
                                pm_setUaOnTop(CONTENTLISTER_UAID);
                                break;
                            case connectScrMode:
                                ctrl_mode_button_clicked();
                                break;
                            case connectScrNews:
                                ctrl_contentType_button_clicked(st_ContentTypeNews);
                                break;
                            case connectScrBooks:
                                ctrl_contentType_button_clicked(st_ContentTypeBooks);
                                break;
                            case connectScrDocs:
                                ctrl_contentType_button_clicked(st_ContentTypeDocs);
                                break;
                            case connectScrNotes:
                                ctrl_contentType_button_clicked(st_ContentTypeNotes);
                                break;   
                           default:
                                ctrl_display_download_history();
                                break;
                        }// switch (connect->screen)
                    }// if (!connect->connect_from_pincode)
                }// if ( !connect->background )
                gdk_threads_leave();
                break;
            }// connectStateDisconnected

            default:
            {
                CL_CTPRINTF("unexpected state [%d]", connect->state);
                connect->state = connectStateIdle;
            }
        }// state
    }// while

    pthread_exit(NULL);
}

