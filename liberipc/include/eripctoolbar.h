/*
 * This file is part of liberipc.
 *
 * liberipc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberipc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file eripctoolbar.h
 * \brief liberipc - specifics to IPC with toolbar
 *
 * <File description>
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#ifndef _ERIPCTOOLBAR_H_
#define _ERIPCTOOLBAR_H_

#include "eripcclient.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


// signals towards the toolbar application
typedef enum {
    ccTbAppendPlatformIcon = 0,
    ccTbRemovePlatformIcon,
    ccTbSetStatePlatformIcon,
    ccTbDisableUpdate,
    ccTbEnableUpdate,
    ccTbSelectIconSet,
    ccTbClearIconSet,
    ccTbSynchronise,
    ccUndefined
} eCcTb;

// icons that can be displayed
#define ICON_ID_NOTUSED   (-1)
typedef enum
{
    iconID_battery = 0,
    iconID_bookmark,
    iconID_collapse,
    iconID_connectivity,
    iconID_copy,
    iconID_cut,
    iconID_empty,
    iconID_erase,
    iconID_error,
    iconID_expand,
    iconID_filemanager,
    iconID_fontlarger,
    iconID_fontsmaller,
    iconID_keyboard,
    iconID_orientation,
    iconID_paste,
    iconID_pen,
    iconID_pen1pixel,
    iconID_pen3pixel,
    iconID_pen5pixel,
    iconID_pen7pixel,
    iconID_penheavier,
    iconID_penlighter,
    iconID_penpressure,
    iconID_selecttext,
    iconID_share,
    iconID_summarize,
    iconID_trashcan,
    iconID_zoomin,
    iconID_zoomout,
    iconID_eraseline,
    iconID_npage,
    iconID_penblack,
    iconID_pendarkgrey,
    iconID_penlightgrey,
    iconID_penwhite,
    iconID_pan,
    iconID_index,
    iconID_pen1pixelLow,
    iconID_pen3pixelLow,
    iconID_pen5pixelLow,
    iconID_pen7pixelLow,
    iconID_penblackLow,
    iconID_pendarkgreyLow,
    iconID_penlightgreyLow,
    iconID_penwhiteLow,
    iconID_locked,
    iconID_tagging,
    iconID_search,
    iconID_zoomfit,
    iconID_landscape_portrait,
    iconID_portrait_landscape,
    iconID_zoomwidth,
    iconID_zoomback,
    iconID_find,
    iconID_lock_screen,
    iconID_not_used01,      // previously called: iconID_doc_approve,
    iconID_not_user02,      // previously called: iconID_doc_reject,
    iconID_continuous_mode,
    iconID_page_mode,
    iconID_sort,
    iconID_connect,
    iconID_start_reading,
    iconID_back,
    iconID_lookup,
    iconID_dictionary_search,
    iconID_forward,

    iconID_rate_a,
    iconID_rate_aa,
    iconID_rate_aaa,
} e_iconID_t;

// states in which the icon can appear on screen
// Note: icon bitmap files in registry must be listed in this order
typedef enum
{
    iconState_normal = 0,       // normal:   black border, white background, black details
    iconState_grey,             // grey:     grey  border, white background, grey  details
    iconState_selected,         // selected: black border, black background, white details
    iconState_undefined
} e_iconState_t;

// additional states for iconID_connect
typedef enum
{
    iconState_IDconnect_time = iconState_undefined,
    iconState_IDconnect_newcontent,
    iconState_IDconnect_newcontent_selected
} e_iconState_connect_t;

// get the max possible number of icons
int tbGetIconNumber(void);

// get the icon name according to the icon id
const char* tbGetIconName(int iconID);

// Toolbar application: parse message from IPC channel
int tbParseCommand(char *szCommand, erIpcCmd_t *pCmd);

// Toolbar application: write "icon-clicked" message to IPC channel
int tbReportIconClicked(erClientChannel_t channel, int signal, int iconID, int iconState);

// Toolbar application: write "syncrhonised" message to IPC channel
int tbReportSynchronised(erClientChannel_t channel, int signal);


// Other applications: write message to IPC channel
 
/**
 * Append icon with identifier iconID in icon set of application uaID
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param iconID icon identifier
 * @param signal Signal that will be sent to the application when this icon is clicked
 *
 * @returns A value of 0 signals success
 */
int tbAppendPlatformIcon(erClientChannel_t channel, int uaID, int iconID, int signal);

/**
 * Remove icon with identifier iconID in icon set of application uaID
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param iconID icon identifier
 *
 * @returns A value of 0 signals success
 */
int tbRemovePlatformIcon(erClientChannel_t channel, int uaID, int iconID);

/**
 * Set state (i.e. bitmap image) for icon with identifier iconID in icon set of application uaID
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param iconID icon identifier
 * @param iconState icon state, i.e. index in bitmap images
 *
 * @returns A value of 0 signals success
 */
int tbSetStatePlatformIcon(erClientChannel_t channel, int uaID, int iconID, int iconState);

/**
 * All effective changes to the Icon state are postponed until a call to tbEnableUpdate
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int tbDisableUpdate(erClientChannel_t channel, int uaID);

/**
 * Make all postponed changes to the Icon state effective
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int tbEnableUpdate(erClientChannel_t channel, int uaID);

/**
 * Select and display an Icon set
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int tbSelectIconSet(erClientChannel_t channel, int uaID);

/**
 * Remove all Icons from an Icon set
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 *
 * @returns A value of 0 signals success
 */
int tbClearIconSet(erClientChannel_t channel, int uaID);

/**
 * Acknowledge "synchronise" message
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param signal Signal that will be sent to the application
 *
 * @returns A value of 0 signals success
 */
int tbSynchronise(erClientChannel_t channel, int uaID, int signal);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
