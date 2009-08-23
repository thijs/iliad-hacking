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

#ifndef _INK_DRAW_H
#define _INK_DRAW_H

#include "ink.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void draw_ink (PtrInk ink);

extern void draw_stroke (PtrStroke stroke);

extern void ink_draw_line(int x0, int y0, int x1, int y1);

//set the the target drawable and brush type
extern int ink_setDrawable(GdkDrawable *pDrawable);

#ifdef __cplusplus
}
#endif

#endif
