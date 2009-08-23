/*
 * This file is part of liberscribble.
 *
 * liberscribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberscribble is distributed in the hope that it will be useful,
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

#ifndef SCBUTIL_H_
#define SCBUTIL_H_

#include <scbtype.h>
#include <glib.h>
#include <scbpoint.h>

//////////////////////////////////////////////////////////////////////////
// scribble library util. Internal use
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

// check two rectangles is intersect or not
gboolean scb_is_rect_intersect(const ScbRectPtr rect1, 
                               const ScbRectPtr rect2);

gboolean scb_is_lines_intersect(const ScbPointPtr p1, 
                                const ScbPointPtr p2,
                                const ScbPointPtr p3, 
                                const ScbPointPtr p4);

#ifdef __cplusplus
}
#endif

#endif
