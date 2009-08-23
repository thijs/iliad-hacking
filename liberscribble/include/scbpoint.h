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

#ifndef SCBPOINT_H_
#define SCBPOINT_H_

#ifdef __cplusplus
extern "C"
{
#endif

// the point defined here is same with X11 and GDK
// we also have another struct dev point, which contains pressure info
// ScbPoint is used when scribble library needs to expose data to gdk/X11
typedef struct _ScbPoint
{
    int     x;
    int     y;
}ScbPoint;
typedef ScbPoint * ScbPointPtr;

// device point with pressure. Serves as input parameter
typedef struct _ScbDevPoint
{
    int     x;
    int     y;
    int     pressure;
}ScbDevPoint;
typedef ScbDevPoint * ScbDevPointPtr;

// read point data from string. replaced by other function
// int scb_point_read(ScbPointPtr p, const char * buf, const int length);

// write point data into string. replaced by other function
// int scb_point_write(const ScbPointPtr p, char * buf, const int length);

// dump
void scb_point_dump(const ScbPointPtr p);


#ifdef __cplusplus
}
#endif

#endif
