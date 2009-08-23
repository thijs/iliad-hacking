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

#ifndef SCBCONFIG_H_
#define SCBCONFIG_H_
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////
// scribble library constant and default configuration
//////////////////////////////////////////////////////////////////////////
#define SCB_LIB_VERSION             "1.1"
#define SCB_LIB_ORG                 "iRex Technologies"

#define SCB_DEF_SCREEN_UNITS        "px"
#define SCB_DEF_SCREEN_DPI          160

// return values used by scribble functions
#define SCB_RET_OK                  1
#define SCB_RET_ERR                 0
#define SCB_INVALID_COUNT           -1

// default value for stroke size
#define SCB_DEF_STROKE_POINTSIZE    40
#define SCB_DEF_STROKE_LAYER        0
#define SCB_DEF_STROKE_LINESTYLE    SCB_LINE_SOLID
#define SCB_DEF_STROKE_PENSIZE      3

// default value for configuration
#define SCB_DEF_FAST_DRAW_COUNT     10
#define SCB_DEF_FAST_DRAW_TIME      100 * 100   // micro second
#define SCB_DEF_FAST_DRAW_BUF_LEN   25          // must be 25!!!!!!!!!

// default value for page & page id
#define SCB_MAX_PAGEID_LEN          256
#define SCB_DEF_PAGE_LEFT           0
#define SCB_DEF_PAGE_TOP            0
#define SCB_DEF_PAGE_RIGHT          768
#define SCB_DEF_PAGE_BOTTOM         1024

// default value for path
#define SCB_MAX_PATH                1024

// default value for xml path
#define SCB_MAX_XML_PATH            256

// default vlaue for color
#define SCB_MAX_COLOR_LEN           8

// default value for erase
#define SCB_DEF_ERASE_SIZE          1

#define SCB_DEF_CMD_ACT_SIZE        10

#ifdef __cplusplus
extern "C"
{
#endif

//========================================================================
// configuration for fast draw
typedef struct _ScbFastDrawParam
{
    int     count;  // max points before draw
    int     time;   // max time before draw
}ScbFastDrawParam;
typedef ScbFastDrawParam * ScbFastDrawParamPtr;

void scb_cfg_init_fast_draw_param();
void scb_cfg_set_fast_draw_param(ScbFastDrawParamPtr ptr);
void scb_cfg_get_fast_draw_param(ScbFastDrawParamPtr ptr);

#ifdef __cplusplus
}
#endif


#endif
