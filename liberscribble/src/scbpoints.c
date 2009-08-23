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

#include "scbpoints.h"
#include "scbtype.h"
#include "scblog.h"
#include "scbconfig.h"

//------------------------------------------------------------------------
// allocate a serial points. we can add ref to compare new/free
// so that we can track memory usage.
// maybe we need to check re-allocate
gboolean scb_points_new(ScbPointsPtr ptr, const int initSize)
{
    SCB_RET_FALSE_IF(NULL == ptr, "Invalid pointer!");
    SCB_RET_FALSE_IF(initSize <= 0, "Invalid initial size!");
        
    ptr->points = g_array_sized_new(FALSE, TRUE, sizeof(ScbPoint), initSize);

    SCB_RET_FALSE_IF(NULL == ptr->points, "Could not allocate enough memory!");
    ptr->pressures = g_array_sized_new(FALSE, TRUE, sizeof(int), initSize);
    
    if (NULL == ptr->pressures)
    {
        g_free(ptr->points); ptr->points = NULL;
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------
// free array 
void scb_points_free(ScbPointsPtr ptr)
{
    SCB_RET_IF(NULL == ptr, "Try to release NULL pointer!");
        
    g_array_free(ptr->points, TRUE);
    g_array_free(ptr->pressures, TRUE);
    ptr->points = NULL;
    ptr->pressures = NULL;
}

//------------------------------------------------------------------------
// push point back to array
void scb_points_append(ScbPointsPtr points, ScbDevPointPtr point)
{
    // check
    SCB_RET_IF(NULL == points || NULL == point, "Invalid pointer(s)!");
        
    // by value. they are binary compatible
    g_array_append_val(points->points, *((ScbPointPtr)point));
    g_array_append_val(points->pressures, point->pressure);
}

//------------------------------------------------------------------------
// how many points does the array contain
int scb_points_get_count(ScbPointsPtr ptr)
{
    SCB_RET_INT_IF(NULL == ptr, SCB_INVALID_COUNT, "Attempt to access NULL pointer!");
    SCB_RET_INT_IF(NULL == ptr->points, SCB_INVALID_COUNT, "Point array is not allocated!");
    // lengthes are equal
    return ptr->points->len;
}

//------------------------------------------------------------------------
// retrieve data &ScbPoint[0]
ScbPointPtr scb_points_get_data(ScbPointsPtr ptr)
{
    SCB_RET_NULL_IF(NULL == ptr || NULL == ptr->points, "NULL pointer(s)");
    return (ScbPointPtr)ptr->points->data;
}
//------------------------------------------------------------------------
// dump
void scb_points_dump(ScbPointsPtr ptr)
{
    ScbPointPtr point = scb_points_get_data(ptr);
    SCB_RET_IF(NULL == point, "");
    
    int len = scb_points_get_count(ptr);
    SCB_DUMP("points count %d. data:", len);
    while (len > 0)
    {
        SCB_DUMP("(%d, %d)", point->x, point->y);
        ++point; --len;
    }
}
