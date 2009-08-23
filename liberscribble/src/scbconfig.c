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

#include "scbconfig.h"

//========================================================================
// param of fast draw
static ScbFastDrawParam s_fast_draw_param;
void scb_cfg_init_fast_draw_param()
{
    s_fast_draw_param.time  = SCB_DEF_FAST_DRAW_TIME;
    s_fast_draw_param.count = SCB_DEF_FAST_DRAW_COUNT;
}

void scb_cfg_set_fast_draw_param(ScbFastDrawParamPtr ptr)
{
    if (ptr)
    {
        s_fast_draw_param.time  = ptr->time;
        s_fast_draw_param.count = ptr->count;
    }
}

void scb_cfg_get_fast_draw_param(ScbFastDrawParamPtr ptr)
{
    if (ptr)
    {
        ptr->time  = s_fast_draw_param.time;
        ptr->count = s_fast_draw_param.count;
    }
}
