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

#ifndef SCBTYPE_H_
#define SCBTYPE_H_

//////////////////////////////////////////////////////////////////////////
// scribble library type definition
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

// line style
typedef enum _ScbLineStyle
{
    SCB_LINE_SOLID,
    SCB_LINE_INVALID,
}ScbLineStyle;

// rect
typedef struct _ScbRect
{
    int     left;
    int     top;
    int     right;
    int     bottom;
}ScbRect;
typedef ScbRect * ScbRectPtr;

// function
const char * scb_line_style_to_str(const ScbLineStyle style);
ScbLineStyle scb_line_style_from_str(const char * str);

#ifdef __cplusplus
}
#endif

#endif
