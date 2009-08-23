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
 * \file contentLister.h
 * \brief browser - contentlister interaction with browser application
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __EREADER_CONTENTLISTER_H__
#define __EREADER_CONTENTLISTER_H__

/**
 * Open communication channel with Contentlister
 * counts on the fact that contentlister is running
 *
 * @returns void
 */
void contentlister_init();

/**
 * request from contentlister
 *
 * @param command the command 
 * @param Ereader datastructure containing the "mozembed" reference
 *
 * @returns -
 */
void contentlister_request(erIpcCmd_t command, Ereader *browser);

#endif //__EREADER_CONTENTLISTER_H__




