/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file ipcServer.h
 * \brief browser - browser IPC communication               
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __BROWSER_IPC_H__
#define __BROWSER_IPC_H__

#define SERVER_BUFFER_SIZE	(1024)

/**
 * Start listening to IPC requests
 *
 * @param Ereader datastructure containing the "mozembed" reference
 *
 * @returns -
 */
gboolean ipc_InstallIpcServer(Ereader *browser);

#endif //__BROWSER_IPC_H__



