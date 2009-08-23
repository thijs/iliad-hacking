/*
 * This file is part of liberdm.
 *
 * liberdm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberdm is distributed in the hope that it will be useful,
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

//
//
// Description: display settings + general includes
//
//
#ifndef display_h
#define display_h

#define SCREEN_WIDTH	768
#define SCREEN_HEIGHT	1024

#define TOOLBAR_HEIGHT	39
#define TOOLBAR_WIDTH	(SCREEN_WIDTH)
#define KEYBOARD_HEIGHT	(SCREEN_HEIGHT/5)

#define TOOLBAR_HEIGTH	(TOOLBAR_HEIGHT)
#define KEYBOARD_HEIGTH	(KEYBOARD_HEIGHT)

#define PAGEBAR_HEIGHT 50
#define CLIENT_AREA	(SCREEN_HEIGHT - TOOLBAR_HEIGHT - PAGEBAR_HEIGHT)

#endif //display_h
