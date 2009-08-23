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
*  header : ScribSaveThread.h
*
* Description: 
*   a indepent thread to execute the image save.
*
* Author: Jian Shen, (C) 2006
*
* Copyright: See COPYING file that comes with this distribution
*
*/

#ifndef __SCRIBBLESAVETHREAD_H__
#define __SCRIBBLESAVETHREAD_H__

#include "ScribbleDefine.h"

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

//return 0 if success.
int initSaveThread(PScribbleUICtx pScribbleUICtx);

//for main thread, notify current page need save,
void notifySave(PScribbleUICtx pScribbleUICtx);

//exit thread, for main thread.
void exitSaveThread();



#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
