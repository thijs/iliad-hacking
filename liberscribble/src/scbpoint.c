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

#include "scbpoint.h"
#include "scbtype.h"
#include "scblog.h"
#include <stdio.h>

/*
The following two function are used by xml storage, they are replaced by
function in ScbStroke
int scb_point_read(ScbPointPtr p, const char * buf, const int length)
{
    if (NULL == p)
    {
        SCB_ERROR("Invalid pointer!");
        return SCB_RET_ERR;
    }

    if (NULL == buf || 0 >= length)
    {
        SCB_ERROR("Invalid buffer pointer or length");
        return SCB_RET_ERR;
    }

    // in fact, we should check the fields returned
    scanf(buf, length, "%d %d %d", &p->x, &p->y, &p->pressure);
    return SCB_RET_OK;
}

int scb_point_write(const ScbPointPtr p, char * buf, const int length)
{
    if (NULL == p)
    {
        SCB_ERROR("Invalid point pointer!");
        return SCB_RET_ERR;
    }

    if (NULL == buf || 0 >= length)
    {
        SCB_ERROR("Invalid buffer pointer or length");
        return SCB_RET_ERR;
    }

    snprintf(buf, length, "%d %d %d\n", p->x, p->y, p->pressure);
    return SCB_RET_OK;
}
*/


void scb_point_dump(const ScbPointPtr p)
{
    SCB_RET_IF(p == NULL, "NULL pointer!");
    SCB_DUMP("(%d, %d)", p->x, p->y);
}
