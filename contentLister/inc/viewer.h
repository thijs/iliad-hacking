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
 * \file viewer.h
 * \brief content lister - contentLister viewer IPC communication                  
 * 
 * <File description>
 * 
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#define SERVER_BUFFER_SIZE 1024 
 
#ifndef __VIEWER__H__
#define __VIEWER__H__

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

/**
 * Init the communication channel with the viewer
 *
 * @param viewer unique identifier
 *
 * @returns -
 */
gboolean viewerInit(char* viewer);

/**
 * Send IPC request to the viewer to store the filename
 * of the currently displayed data
 *
 * @param viewer unique identifier
 * @param contentType unique identifier
 * @returns -
 */
gboolean viewerStore(char* viewer, st_ContentType_e contentType);

/**
 * Start listening to viewer IPC requests
 *
 * @returns -
 */
gboolean viewerInstallIpcServer();

#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__VIEWER__H__

