/*
 * This file is part of scribble.
 *
 * scribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * scribble is distributed in the hope that it will be useful,
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
 * \file ScribbleIPC.h
 * \brief scribble - IPC communication               
 * \author Jian Shen
 *
 * <File description>
 * 
 */

#ifndef __SCRIBBLE_IPC_H__
#define __SCRIBBLE_IPC_H__

#include <gtk/gtk.h>

#include "ScribbleDefine.h"
#include "ScribbleUtils.h"

#define SCRIB_SERVER_BUFFER_SIZE	(1024)

/**
 * Start listening to IPC requests
 *
 * @param PScribbleUICtx scribble ui context
 *
 * @returns -
 */
extern gboolean installIpcServer(PScribbleUICtx pScribbleUICtx);


/**
 * Stop listening to IPC requests
 *
 * @returns -
 */
extern void stopIpcServer(void);


/**
 * Open communication channel with pageBar deamon
 * counts on the fact that pageBar is running
 *
 * @returns void
 */
extern void pagebar_init();

/**
 * Request from scribble to goto page
 *
 * @param page - page to go to
 *
 * @returns -
 */
extern void pagebar_goto_page(int page);

/**
 * Request from browser to set number of pages
 *
 * @param count - number of pages in current view
 *
 * @returns -
 */
extern void pagebar_set_pagecount(int count);


/**
 * Request from contentLister to redraw pagebar
 *
 * @param - 
 *
 * @returns -
 */
extern void pagebar_redraw();



/////////////////ToolBar part//////////////////////////

typedef enum {
    MODE_DRAW=0,  
    MODE_SELSIZE=1,//when selecting the pen size
    MODE_SELCOLOR=2,//when selecting the color
    MODE_ERASE=3,
    MODE_DELPAGE=4,//when wanna deleting pages
    MODE_INVALID=5,//initially,invalid
}OPERATION_MODE;

/**
 * Init toolbar (open communication channel and enable/set the correct icons)
 *
 * @returns void
 */
extern void toolbar_init(void);

/**
 * A key is selected on the toolbar
 *
 * @param command the command associated with the selected key
 * @param PScribbleUICtx scribble ui context
 *
 * @returns -
 */
extern void toolbar_key_selected(int iconID, int iconState,
                          PScribbleUICtx pScribbleUICtx);


extern OPERATION_MODE getOperationMode();

void setOperationMode(OPERATION_MODE iOpMode);

extern HW_COLOR getPenColor();

void setPenColor(HW_COLOR iHwColor);

extern PEN_SIZE getPenSize();

void setPenSize(PEN_SIZE iPenSize);

//wait confirm deleting?
extern gboolean toolbar_is_waitconfirmdel();

//set page-delelted confirmed.
extern void toolbar_confirm_delpage();

//in mode_erase?
extern gboolean toolbar_is_erasing();

//can draw point
extern gboolean toolbar_is_drawing();

//clear the toolbar to make, avoid toolbar exists issue when scribble quit.
extern void toolbar_clearall();

//first init the toolbar,set it to MODE_DRAW
extern void toolbar_firstinit(int iTotalPage);

//reinit the toolbar,reset all the status,include the pages
extern void toolbar_reinit(int iTotalPage);

#endif 

