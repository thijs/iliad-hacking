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
 * \file metadataStoreManager.h
 * \brief metadataStoreManager - init and manage the metadata store                  
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __PROGRAMMANAGER__H__
#define __PROGRAMMANAGER__H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>

#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <X11/Xatom.h>

#include "liberipc/eripc.h"
#include <liberregxml/erregapi.h>

#define WM_NAME_MAX 256

#define PM_DOWNLOADMGR_LOCATION "/usr/bin/downloadMgr"

#define XCLIENT_ARRAY_RESERVED_SIZE      32
#define CONTENTLISTER_UAID     "CONTENTLISTER"
#define CONTENTLISTER_RESNAME  "contentLister" 
#define WINDOWMANAGER_NAME_MAX 256
#define UAID_MAX_SIZE          256
#define COMMAND_MAX_SIZE       1024

#define ERR_OK                 ( 0)
#define ERR_STATE_UPDATE       (-1)
#define ERR_ARGUMENT           (-2)
#define ERR_FORK               (-3)
#define ERR_INVALID_UAID       (-4)
#define ERR_VIEWER_RUNNING     (-5)

typedef struct
{
    char*    uaID;
    char*    iconLocation;	
} erExtInfo_t;

typedef struct
{
    Window  win;
    char    resName[WINDOWMANAGER_NAME_MAX];
    unsigned long winState;	
} erXClient_t;

typedef struct
{
	char          uaID[UAID_MAX_SIZE];
	regUserApp_t   *userApp;
	pid_t          pid;
    erXClient_t    *xclient;
    gboolean       enabled;
} erApplicationInfo_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * create and  init the global data used by the program manager module
 *
 *
 * @return TRUE in case of succes
 */
gboolean pm_InitProgramManager(void);

/**
 * set my own window ID
 *
 * @param window - window of the contentLister application	
 *
 */
void pm_SetMyWindow(Window window);

/**
 * Destroy the global data used by the program manager module
 *
 * @return TRUE in case of succes
 */
void pm_DestroyProgramManager(void);

/**
 * Raise the contentlister application
 *
 * @return TRUE in case of succes
 */
gboolean pm_RaiseContentLister();

/**
 * Raise the specified used application
 *
 * @return TRUE in case of succes
 */
gboolean pm_RaiseUserApp(gchar* uaId);

/**
 * Checks whether a specified viewer is already active.
 *
 * @return TRUE when active
 */
gboolean pm_IsActive(gchar* uaId);

/**
 * Returns the uaID of the user application currently on top of the window stack.
 *
 * @the returned value is READ ONLY
 */
char* pm_getUaOnTop(void);

/**
 * Sets the uaID of the user application that has just been started, and therefore is on top of the window stack now.
 *
 * @param uaID - user application identifier as specified in the registry
 */
void pm_setUaOnTop(char* uaID);

/**
 * send button press signal to app on top.
 */
void pm_SendKey(KeySym keysym);

/**
 * Start/Show the specified viewer. 
 * In case the viewer supports re-displaying new content, this (generic) mechanism will be invoked
 *
 * @param userApplication unique identifier
 * @param filename that needs to be passed to the viewer _FILENAME_ in registry [optional]
 * @param manifest that needs to be passed to the viewer _MANIFEST_ in regsitry [optional]
 * @param options  that needs to be passed to the viewer [optional]
 * @param reuse	if '1' a current instance of the viewer will gain focus and display the 
 *        selected content. If no such instance is available a new instance will be created
 *
 * @returns 0 upon success. A negative value (ERR_INVALID_UAID, ERR_FORK, ERR_ARGUMENT
 *          ERR_STATE_UPDATE) in case of failure
 *
 */
int pm_RunViewer(gchar* uaId, char* filename, char* manifest, char* options, int reuse);


/**
 * Returns the READ ONLY uaID assocaited with this extension.
 *
 * @the returned value is READ ONLY
 */
char* pm_getUaID(char* extension);

/**
 * Returns the READ ONLY icon location for this extension.
 *
 * @the returned value is READ ONLY
 */
char* pm_getIcon(char* extension);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
