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
 * \file viewer.c
 * \brief content lister - contentLister viewer IPC communication                  
 * 
 * <File description>
 * 
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#include <stdio.h>

#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include <liberipc/eripc.h>
#include <liberipc/eripcviewer.h>
#include <liberipc/eripctoolbar.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "erbusy.h"
#include "displayUpdate.h"
#include "stack.h"
#include "viewer.h"
#include "history.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "programManager.h"
#include "erMSDisk.h"
#include "regValue.h"
#include "toolbar.h"
#include "button.h"
#include "click.h"

// temp global variable, probably will be added in the program manager 
// viewer table later on...
static erClientChannel_t theBrowserChannel;
static erServerChannel_t theServerChannel;

//internally used routines
static int viewerGetChannel(char *viewer);
void    viewerMessageReceived(gpointer data, gint source_fd, GdkInputCondition condition);

gboolean viewerInit(char *viewer)
{
    int     channel;

    channel = viewerGetChannel(viewer);

    if (channel > 0)
    {
        // create/init the channel to communicate with the userApplication
        if (erIpcStartClient(channel, &theBrowserChannel) == 0)
        {
            CL_IPCPRINTF("channel %d opened", channel);
            return TRUE;
        }
    }
    return FALSE;
}

gboolean viewerStore(char *viewer, st_ContentType_e contentType)
{
    int     channel;

    CL_IPCPRINTF("entry - %s - %d", viewer, contentType);

    channel = viewerGetChannel(viewer);

    if (channel > 0)
    {
        // request to store the location of the currently diplayed data
        // NULL where itemID used to be since in the past the last location 
        // of every item neede to be stored. Now only hte last accessed item
        // location needs to be stored so no item id is needed any more

        vwrStore(theBrowserChannel, ER_CONTENT_LISTER_UA_ID, contentType, NULL);
    }

    return TRUE;

}

gboolean viewerInstallIpcServer()
{
    int     returnValue;
    int     fd = -1;

    CL_IPCPRINTF("entry");

    returnValue = erIpcOpenServerChannel(ER_CONTENTLISTER_CHANNEL, &theServerChannel);

    if (returnValue != (-1))
    {
        fd = erIpcGetServerFd(theServerChannel);

        CL_IPCPRINTF("erIpcGetServerFd returned %d\n", fd);

        // gtk specific mechanism to monitor input file descriptor.
        returnValue = gdk_input_add(fd, GDK_INPUT_READ, viewerMessageReceived, (gpointer) theServerChannel);

        CL_IPCPRINTF("gdk_input_add returned %d", returnValue);
        return TRUE;
    }
    else
    {
        CL_ERRORPRINTF("Could not open server channel");
        return FALSE;
    }
}

// 
// This callback is invoked when there is data to read on file descriptor source_fd.
// The channel info is passed via parameter 'data'
//
void viewerMessageReceived(gpointer data, gint source_fd, GdkInputCondition condition)
{
    char    szBuffer[SERVER_BUFFER_SIZE];
    int     nBuf = SERVER_BUFFER_SIZE;
    erIpcCmd_t command;
    int     index;
    st_ContentType_e contentType;
    char   *location;
    char   *item;
    eCcBusyState status;

    CL_IPCPRINTF("entry");
    erIpcGetMessage((erServerChannel_t) data, szBuffer, &nBuf);

    CL_IPCPRINTF("Received [%s]", szBuffer);

    if (clParseCommand(szBuffer, &command) >= 0)
    {
        CL_IPCPRINTF("command.cc [%d]", command.cc);
        switch (command.cc)
        {
            case ccClStore:
                CL_IPCPRINTF("STORE - nArg %d", command.nArg);
                //check the argument just to besure
                if (command.nArg == 4)
                {
                    for (index = 0; index < command.nArg; index++)
                    {
                        CL_IPCPRINTF("command.arg[%d]=%s", index, command.arg[index]);
                    }

                    contentType = atoi(command.arg[1]);
                    item = command.arg[2]; // NOT USED ANY MORE !!!!
                    location = command.arg[3];

                    CL_IPCPRINTF("contentType=%d location=%s", contentType, location);
                    hsSetLocation(contentType, location);
                }
                else
                {
                    CL_WARNPRINTF("ccclStore invalid argument count %d", command.nArg);
                }
                break;
                
            case ccClMSDiskSetConnected:
            {
                int connected = atoi(command.arg[0]);
                CL_IPCPRINTF("msdisk: connected = %d\n", connected);
                erMSDiskSetConnected(connected);
                break;
            }
                
            case ccClGotoPage:
            {
                int page = atoi(command.arg[0]);
                CL_IPCPRINTF("Contentlister Goto Page: %d\n", page);
                gdk_threads_enter();
                erbusy_blink();
                ctrl_goto_page(page);
                gdk_threads_leave();
                break;
            }

            case ccClBatteryLow:
            {
                eCcClBatteryAction action     = atoi(command.arg[0]);
                int                percentage = atoi(command.arg[1]);
                int                time_left  = atoi(command.arg[2]);
                CL_IPCPRINTF("BatteryLow: action [%d] perc [%d] time [%d]", action, percentage, time_left);
                gdk_threads_enter();
                if (action == ccClBattery_warning)
                {
                    ctrl_battery_low_screen(percentage, time_left, FALSE);
                }
                else if (action == ccClBattery_shutdown)
                {
                    ctrl_battery_low_screen(percentage, time_left, TRUE);
                }
                gdk_threads_leave();
            }
                
            case ccClToolbarIcon:
            {
                int iconID    = atoi(command.arg[0]);
                int iconState = atoi(command.arg[1]);
                CL_TBPRINTF("ccClToolbarIcon: iconID [%d] iconState [%d]", iconID, iconState);
                gdk_threads_enter();
                ctrl_on_icon_clicked(iconID, iconState);
                gdk_threads_leave();
                break;
            }

            case ccClStoragePresent:
            {
                st_ContentType_e storageType    = ctrl_get_storageType(command.arg[0]);
                gboolean         storagePresent = atoi(command.arg[1]) ? TRUE : FALSE;
                gdk_threads_enter();
                ctrl_storage_changed(storageType, storagePresent);
                gdk_threads_leave(); 
                break;
            }

            case ccClToolbarSynchronised:
                gdk_threads_enter();
                ctrl_on_toolbar_synchronised();
                gdk_threads_leave();
                break;

            case ccClBusyIndicator:
                if (command.nArg == 1)
                {
                    status = atoi(command.arg[0]);
                    if (status == ccBusyState_Off)
                    {
                        CL_LOGPRINTF("enable key buffering for busybar off");
                        gdk_threads_enter();
                        button_enable_key_buffering(TRUE);
                        gdk_threads_leave();
                    }
                    else if (status == ccBusyState_Blink)
                    {
                        CL_LOGPRINTF("disable key buffering for busybar blink");
                        gdk_threads_enter();
                        button_enable_key_buffering(FALSE);                      
                        gdk_threads_leave();
                    }
                }
                break;

            case ccClDisplayUpdated:
                CL_LOGPRINTF("enable key buffering for display updated");              
                gdk_threads_enter();
                button_enable_key_buffering(TRUE);
                gdk_threads_leave();
                break;          

            case ccClNewContent:
                CL_LOGPRINTF("enable key buffering for display updated\n");              
                gdk_threads_enter();
                ctrl_new_content_received();
                gdk_threads_leave();
                break;

            case ccClPenClick:
                click_pen();
                break;

            case ccClEnableAudio:
            {
                int volume = atoi(command.arg[0]);
                CL_LOGPRINTF("enable/disable volume=%d", volume);
                handle_sound_settings_changed(volume);
                break;
            }

            default:
                CL_ERRORPRINTF("UNKNOWN IPC COMMAND %d", command.cc);
                break;
        }
    }
    CL_IPCPRINTF("done");
}


/**
 * retrieve the channel ID of the specified viewer
 *
 * @param viewer unique identifier
 *
 * @returns -1 in case of failure
 */
static int viewerGetChannel(char *viewer)
{
    int     channel = -1;

    CL_IPCPRINTF("entry %s", viewer);

    if (strncmp(BROWSER_APP, viewer, UAID_MAX_SIZE) == 0)
    {
        channel = ER_XHTML_VIEWER_CHANNEL;
    }
    else
    {
        CL_WARNPRINTF("%s unkown viewer", viewer);
    }
    return channel;
}
