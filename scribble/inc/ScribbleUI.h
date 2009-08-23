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

#ifndef _SCRIBBLEUI_H
#define _SCRIBBLEUI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ScribbleDefine.h"

void jump_to_page (PScribbleUICtx pScribbleUICtx);

//reset current page and then jump to it
void resetCurrentPage(PScribbleUICtx pScribbleUICtx,
                      const int iNewCurrPageNum);

//for adding one page
gboolean addOnepage(PScribbleUICtx pScribbleUICtx);

//for deleting one page
gboolean delOnepage(PScribbleUICtx pScribbleUICtx);

//exit main thread and save-thread.
void exitAllThreads();

//when scribble quit
void quit (gpointer data);

#ifdef __cplusplus
}
#endif

#endif
