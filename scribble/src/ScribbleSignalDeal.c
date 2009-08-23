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

/*
*  C Implementation: ScribbleSignalDeal
*
* Description: 
*
*
* Author: Jian Shen, (C) 2006
*
* Copyright: See COPYING file that comes with this distribution
*
*/

#include "ScribbleUI.h"
#include "ScribbleSignalDeal.h"
#include "ScribbleLog.h"
#include "ScribbleCore.h"
#include "ScribbleSaveThread.h"
#include "erbusy.h"

#include <signal.h>

static ScribbleUICtx *pCurrScribbleUICtx=NULL;

// signal handlers
static void on_sigterm(int signo)
{
    // stop main process
    SB_TIMEDISPLAY("\n caught signal to term\n"); 
    gdk_threads_enter();//need it in getpixmap...
    quit(pCurrScribbleUICtx);
    gdk_threads_leave();
}

//deal the signal "term"
void dealTermSignal(ScribbleUICtx *pScribbleUICtx)
{
     // catch the SIGTERM signal
    struct sigaction on_term;

    pCurrScribbleUICtx=pScribbleUICtx;
    memset(&on_term, 0x00, sizeof(on_term));
    on_term.sa_handler = on_sigterm;
    sigaction(SIGTERM, &on_term, NULL);
}

