/**
 * \file eripccontentlister.h
 * \brief liberipc - specifics to IPC with contentlister
 *
 * <File description>
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 Philips Apptech
 *
 */

#ifndef _ERIPCCONTENTLISTER_H_
#define _ERIPCCONTENTLISTER_H_

#include "eripcclient.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    ccClStore = 0,
    ccClMSDiskSetConnected,
    ccClGotoPage,
    ccClBatteryLow,
    ccClToolbarIcon,
    ccClStoragePresent,
    ccClToolbarSynchronised,
    ccClBusyIndicator,
    ccClDisplayUpdated,
    ccClNewContent,
    ccClPenClick,
    ccClEnableAudio,
    ccClUndefined
} eCcCl;

typedef enum {
    ccClBattery_warning = 0,
    ccClBattery_shutdown
} eCcClBatteryAction;


int clParseCommand(char *szCommand, erIpcCmd_t *pCmd);

/**
 * Store the currently displayed page for this category
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param uaID application identifier
 * @param category identifier
 * @param itemID - null terminated string - unique item ID of the currently displayed content
 * @param itemLocation - null terminated string 
 *
 * @returns A value of 0 signals success
 */
int clStore(erClientChannel_t channel, int uaID, int category, char* itemID, char* itemLocation);

/**
 * Notify the contentmanager that we are connected to the PC
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param connected boolean showing connection status
 * @returns A value of 0 signals success
 */
int MSDiskSetConnected(erClientChannel_t channel, int connected);

/**
 * Notify the contentmanager it should goto a new page
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param page pagenumber to jump to
 * @returns A value of 0 signals success
 */
int clGotoPage(erClientChannel_t channel, int page);

/**
 * Notify the contentmanager that the battery level is low
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param action requested from contetnLister
 * @param battery charge as 0 .. 100%
 * @param estimated time left (in minutes)
 * @returns A value of 0 signals success
 */
int clBatteryLow(erClientChannel_t channel, eCcClBatteryAction action, int percentage, int time_left);

/**
 * Notify the contentmanager that a storage module has been inserted or removed
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param storage module identification
 * @param 1/0 = module present yes/no
 * @returns A value of 0 signals success
 */
int clStoragePresent(erClientChannel_t channel, char* storageType, int storagePresent);

/**
 * Notify the contentmanager that busyIndicator changes its status
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param status  busybar change to the 'status' 
 * @returns A value of 0 signals success
 */
int clBusyIndicator(erClientChannel_t channel, eCcBusyState status);

/**
 * Notify the contentmanager that display Updated
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @returns A value of 0 signals success
 */
int clDisplayUpdated(erClientChannel_t channel);

/**
 * Notify the contentmanager that new content has been downloaded
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @returns A value of 0 signals success
 */
int clNewContent(erClientChannel_t channel);

/**
 * Notify the Content Lister that a pen click event is occurred
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @returns A value of 0 signals success
 */
int clPenClick(erClientChannel_t channel);

/**
 * Called by settings application when user changes the sound settings
 *
 * @param channel Opaque datastructure that contains info on communication channel
 * @param volume 0=disabled, >0=enabled
 * @returns A value of 0 signals success
 */
int clEnableAudio(erClientChannel_t channel, int volume);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_ERIPCCONTENTLISTER_H_
