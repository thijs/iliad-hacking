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

#ifndef SCBPOINTS_H_
#define SCBPOINTS_H_
#include <glib.h>
#include "scbpoint.h"

#ifdef __cplusplus
extern "C"
{
#endif

// use two arrays so that we can easily expose the points to GTK/GDK and X11
// without allocating any other memory
typedef struct _ScbPoints
{
    GArray* points;
    GArray* pressures;
}ScbPoints;
typedef ScbPoints * ScbPointsPtr;

// allocate an array with initSize
gboolean scb_points_new(ScbPointsPtr points, const int initSize);

// release the array and all items in the array
void scb_points_free(ScbPointsPtr points);

// push point back to array
void scb_points_append(ScbPointsPtr points, ScbDevPointPtr point);

// how many points does the array contain
int scb_points_get_count(ScbPointsPtr ptr);

// retrieve data &ScbPoint[0]. output
ScbPointPtr scb_points_get_data(ScbPointsPtr points);

// dump
void scb_points_dump(ScbPointsPtr points);

#ifdef __cplusplus
}
#endif

#endif
