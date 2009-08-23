/**
 * \file eripc.h
 * \brief liberipc - A library to enable IPC on the eReader platform
 *
 * <File description>
 *
 */


/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 Philips Apptech
 *
 */

#ifndef _ERIPC_H_
#define _ERIPC_H_

/* eripcconfig.h is auto generated */
#include "eripcconfig.h"

#ifdef __cplusplus
extern  "C"
{
#endif                          /* __cplusplus */

typedef void *erClientChannel_t;
typedef void *erServerChannel_t;

// Application ID
#define ER_TEST_UA_ID           0  
#define ER_PDF_VIEWER_UA_ID     1  
#define ER_XHTML_VIEWER_UA_ID   2  
#define ER_APABI_VIEWER_UA_ID   3  
#define ER_ARINC_VIEWER_UA_ID   4
#define ER_SETUP_UA_ID          100
#define ER_CONTENT_LISTER_UA_ID 101
#define ER_TEXT_VIEWER_UA_ID    102
#define ER_SCRIBBLE_UA_ID       103
#define ER_CONNECT_UA_ID        104
#define ER_POWERMGR_UA_ID       105
#define ER_BUSYD_UA_ID          106
#define ER_PAGEBAR_UA_ID        107
#define ER_TOOLBAR_UA_ID        108
#define ER_EBA_VIEWER_UA_ID     109
#define ER_MOBIPOCKET_VIEWER_UA_ID    110
#define ER_UAID_ILLEGAL         (-1)

// Channel ID, i.e. destination of the channel
// Note: must correspond with channel numbers in er_registry.txt
#define ER_TOOLBAR_CHANNEL       1
#define ER_TEST_CHANNEL          2
#define ER_PDF_VIEWER_CHANNEL    3
#define ER_XHTML_VIEWER_CHANNEL  4
#define ER_APABI_VIEWER_CHANNEL  5
#define ER_CONTENTLISTER_CHANNEL 6
#define ER_BUSYD_CHANNEL         7
#define ER_POWERMGR_CHANNEL      8 
#define ER_PAGEBAR_CHANNEL       9
#define ER_SCRIBBLE_CHANNEL     10
#define ER_SETUP_CHANNEL        11
#define ER_CONNECT_CHANNEL      12
#define ER_ARINC_VIEWER_CHANNEL 13
#define ER_EBA_VIEWER_CHANNEL   14
#define ER_MOBIPOCKET_VIEWER_CHANNEL   15

#define ER_CHANNEL_NUM          15
#define ER_CHANNEL_ILLEGAL      (-1)
#define erIpcGetChannel(uaID)   (((uaID) == ER_TEST_UA_ID)           ? ER_TEST_CHANNEL          :  \
                                 ((uaID) == ER_PDF_VIEWER_UA_ID)     ? ER_PDF_VIEWER_CHANNEL    :  \
                                 ((uaID) == ER_XHTML_VIEWER_UA_ID)   ? ER_XHTML_VIEWER_CHANNEL  :  \
                                 ((uaID) == ER_APABI_VIEWER_UA_ID)   ? ER_APABI_VIEWER_CHANNEL  :  \
                                 ((uaID) == ER_ARINC_VIEWER_UA_ID)   ? ER_ARINC_VIEWER_CHANNEL  :  \
                                 ((uaID) == ER_SETUP_UA_ID)          ? ER_SETUP_CHANNEL         :  \
                                 ((uaID) == ER_CONTENT_LISTER_UA_ID) ? ER_CONTENTLISTER_CHANNEL :  \
                                 ((uaID) == ER_TEXT_VIEWER_UA_ID)    ? ER_CHANNEL_ILLEGAL       :  \
                                 ((uaID) == ER_SCRIBBLE_UA_ID)       ? ER_SCRIBBLE_CHANNEL      :  \
                                 ((uaID) == ER_CONNECT_UA_ID)        ? ER_CONNECT_CHANNEL       :  \
                                 ((uaID) == ER_POWERMGR_UA_ID)       ? ER_POWERMGR_CHANNEL      :  \
                                 ((uaID) == ER_BUSYD_UA_ID)          ? ER_BUSYD_CHANNEL         :  \
                                 ((uaID) == ER_PAGEBAR_UA_ID)        ? ER_PAGEBAR_CHANNEL       :  \
                                 ((uaID) == ER_TOOLBAR_UA_ID)        ? ER_TOOLBAR_CHANNEL       :  \
                                 ((uaID) == ER_EBA_VIEWER_UA_ID)     ? ER_EBA_VIEWER_CHANNEL    :  \
                                 ((uaID) == ER_MOBIPOCKET_VIEWER_UA_ID)     ? ER_MOBIPOCKET_VIEWER_CHANNEL    :  \
                                 ER_CHANNEL_ILLEGAL)

#define ER_IPC_PORT_FIRST               (50065)
#define ER_IPC_PORT_ILLEGAL             (-1)
#define erIpcGetServerPort(channelIdx)  (((channelIdx) > 0 && (channelIdx) <= ER_CHANNEL_NUM) ? (ER_IPC_PORT_FIRST + (channelIdx) - 1) : -1)

#define TOOLBAR_IPC_PORT        (erIpcGetServerPort(ER_TOOLBAR_CHANNEL      )
#define TEST_IPC_PORT           (erIpcGetServerPort(ER_TEST_CHANNEL         )
#define PDF_VIEWER_IPC_PORT     (erIpcGetServerPort(ER_PDF_VIEWER_CHANNEL   )
#define XHTML_VIEWER_IPC_PORT   (erIpcGetServerPort(ER_XHTML_VIEWER_CHANNEL )
#define APABI_VIEWER_IPC_PORT   (erIpcGetServerPort(ER_APABI_VIEWER_CHANNEL )
#define CONTENTLISTER_IPC_PORT  (erIpcGetServerPort(ER_CONTENTLISTER_CHANNEL)
#define BUSYD_IPC_PORT          (erIpcGetServerPort(ER_BUSYD_CHANNEL        )
#define POWERMGR_IPC_PORT       (erIpcGetServerPort(ER_POWERMGR_CHANNEL     )
#define PAGEBAR_IPC_PORT        (erIpcGetServerPort(ER_PAGEBAR_CHANNEL      )
#define SCRIBBLE_IPC_PORT       (erIpcGetServerPort(ER_SCRIBBLE_CHANNEL     )
#define SETUP_IPC_PORT          (erIpcGetServerPort(ER_SETUP_CHANNEL        )
#define CONNECT_IPC_PORT        (erIpcGetServerPort(ER_CONNECT_CHANNEL      )
#define ARINC_VIEWER_IPC_PORT   (erIpcGetServerPort(ER_ARINC_VIEWER_CHANNEL )
#define EBA_VIEWER_IPC_PORT     (erIpcGetServerPort(ER_EBA_VIEWER_CHANNEL   )
#define MOBIPOCKET_VIEWER_IPC_PORT     (erIpcGetServerPort(ER_MOBIPOCKET_VIEWER_CHANNEL   )
    
// Only change the following three when updating version number.
#define ER_IPC_MAJOR_VERSION 0
#define ER_IPC_MINOR_VERSION 1
#define ER_IPC_ITER_VERSION 0

#define ER_IPC_VERSION (((( ER_IPC_MAJOR_VERSION << 8 ) | ER_IPC_MINOR_VERSION) << 8 ) | ER_IPC_ITER_VERSION)

typedef void (*erMessageCB) (char *szBuffer, int *nBuf, void *data);

int     erIpcGetVersion();

/**
 * Start the server side of the IPC communication channel. 
 * Under normal circumstances this function will not return. It invokes a callback when a message is received.
 * It requires its own thread and is the easiest way to establish the server side. 
 * Note: Use erIpcOpenServerChannel(), erIpcGetServerFd() and  * erIpcGetMessage() if you don't want to create a
 * server specific thread.
 * 
 * @param channelIdx Communication channel identifier
 * @param callback Will be called when a message is received
 * @param szBuffer Buffer that will be used by callback
 * @param nBuf size of szBuffer
 * @param data pointer to opaque datastructure.
 *
 * @returns A value of -1 signals failure
 */
int     erIpcStartServer(int channelIdx, erMessageCB callback, char *szBuffer, int *nBuf, void *data);

/**
 * Create the server side of the IPC communication channel. 
 *
 * After calling this function. Use erIpcGetServerFd() to fetch the filedescriptor associated with
 * this channel. Monitor the filedescriptor for data. When data is available use erIpcGetMessage to
 * fetch the data.
 *
 * @param channelIdx Communication channel identifier
 * @param channel Opaque datastructure that contains info on communication channel
 *
 * @returns A value of -1 signals failure
 */
int     erIpcOpenServerChannel(int channelIdx, erServerChannel_t * channel);

/**
 * Fetch the filedescriptor associated with the channel
 *
 
 * Cf. erIpcOpenServerChannel
 *
 * @param channel Communication channel
 *
 * @returns The file descriptor associated with the channel
 */
int     erIpcGetServerFd(erServerChannel_t channel);

/**
 * Get a message from the channel, will block when no message is available
 *
 * Cf. erIpcOpenServerChannel
 *
 * @param channel Communication channel
 * @param szBuffer Buffer that will be used by callback
 * @param nBuf size of szBuffer
 *
 * @returns The file descriptor associated with the channel
 */
int     erIpcGetMessage(erServerChannel_t channel, char *szBuffer, int *nBuf);


/**
 * Start the client side of the IPC communication channel
 *
 * @param channelIdx Communication channel identifier
 * @param channel Opaque datastructure that contains info on communication channel
 *
 * @returns A value of 0 signals success
 */
int     erIpcStartClient(int channelIdx, erClientChannel_t * channel);

/**
 * Initializes the server. Only use this function when you don't want a dedicated thread for IPC handling.
 *
 * @param channelIdx Communication channel identifier
 * @param sockfd The socket filedescriptor
 * @param local Set to 1 if only local addresses need to be served
 *
 * @returns A value of 0 signals success
 */
int erIpcInitServer(int channelIdx, int *sockfd, int local);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif
