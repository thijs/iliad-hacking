/*
 * This file is part of settings.
 *
 * settings is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * settings is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

/**
 * \file iLiadPCConnectScreen.h
 * \brief settings application - screen handling and event processing for screen "pc connect"
 * 
 */

#ifndef _ILIAD_PC_CONNECT_SCREEN_H_
#define _ILIAD_PC_CONNECT_SCREEN_H_

#define PC_CONNECT_SCREEN_WIDTH   666

#define PC_CONNECT_SCREEN_PADDING_TOP    10
#define PC_CONNECT_SCREEN_PADDING_BOTTOM 10
#define PC_CONNECT_SCREEN_PADDING_LEFT   13
#define PC_CONNECT_SCREEN_PADDING_RIGHT  13

#define PC_CONNECT_SCREEN_SPACING_H    5
#define PC_CONNECT_SCREEN_SPACING_V    5

#define PC_CONNECT_PCNAME_HEIGHT       115
#define PC_CONNECT_SHARENAME_HEIGHT    115
#define PC_CONNECT_WORKGROUP_HEIGHT    115

#define PC_CONNECT_USER_PASSWORD_BK_WIDTH  666 
#define PC_CONNECT_USER_PASSWORD_BK_HEIGHT 115

#define PC_CONNECT_SCREEN_USER_PASSWORD_SPACING_V 5
#define PC_CONNECT_SCREEN_USER_PASSWORD_SPACING_H 50

#define PC_CONNECT_INPUT_ENTRY_WIDTH   200
#define PC_CONNECT_INPUT_ENTRY_HEIGHT  30


GtkWidget *iLiad_pc_connect_screen_create(void);
void iLiad_pc_connect_screen_set_text(void);
void iLiad_pc_connect_screen_display_settings();


#endif // _ILIAD_PC_CONNECT_SCREEN_H_
