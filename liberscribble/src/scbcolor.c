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

#include "scbcolor.h"
#include "scblog.h"
#include <stdio.h>
#include "scbtype.h"
#include "scbconfig.h"

void scb_color_set_white(ScbColorPtr ptr)
{
    if (NULL == ptr) return;
    ptr->red    = 0xff;
    ptr->green  = 0xff;
    ptr->blue   = 0xff;
    ptr->pixel  = 0xffff;
    ptr->pad    = 0;
    ptr->flags  = 0;
}

void scb_color_set_black(ScbColorPtr ptr)
{
    if (NULL == ptr) return;
    ptr->red    = 0;
    ptr->green  = 0;
    ptr->blue   = 0;
    ptr->pixel  = 0;
    ptr->pad    = 0;
    ptr->flags  = 0;
}

void scb_color_set_light_gray(ScbColorPtr ptr)
{
    if (NULL == ptr) return;
    ptr->red    = 0xaa;
    ptr->green  = 0xaa;
    ptr->blue   = 0xaa;
    ptr->pixel  = 0xaaaa;
    ptr->pad    = 0;
    ptr->flags  = 0;
}

void scb_color_set_dark_gray(ScbColorPtr ptr)
{
    if (NULL == ptr) return;
    ptr->red    = 0x55;
    ptr->green  = 0x55;
    ptr->blue   = 0x55;
    ptr->pixel  = 0x5555;
    ptr->pad    = 0;
    ptr->flags  = 0;
}

char _g_htmlColor[SCB_MAX_COLOR_LEN] = {0};
//------------------------------------------------------------------------
// convert color to html color and dev color
const char * scb_color_to_html_color(const ScbColorPtr ptr)
{
    SCB_RET_NULL_IF(NULL == ptr, "Invalid color pointer!");
    snprintf(_g_htmlColor, SCB_MAX_COLOR_LEN,
        "#%02X%02X%02X", ptr->red, ptr->green, ptr->blue);
    return _g_htmlColor;
}

//------------------------------------------------------------------------
ScbDevColor scb_color_to_dev_color(ScbColorPtr ptr)
{
    if (NULL == ptr) return SCB_DEV_COLOR_UNKNOWN;
    if (0 == ptr->red && 0 == ptr->green && 0 == ptr->blue)
    {
        return SCB_DEV_COLOR_BLACK;
    }
    else if (0xff == ptr->red && 0xff == ptr->green && 0xff == ptr->blue)
    {
        return SCB_DEV_COLOR_WHITE;
    }
    else if (0xaa == ptr->red && 0xaa == ptr->green && 0xaa == ptr->blue)
    {
        return SCB_DEV_COLOR_LIGHT_GRAY;
    }
    else if (0x55 == ptr->red && 0x55 == ptr->green && 0x55 == ptr->blue)
    {
        return SCB_DEV_COLOR_DARK_GRAY;
    }
    return SCB_DEV_COLOR_UNKNOWN;
}

//------------------------------------------------------------------------
// convert html color to color and dev color
int scb_html_color_to_color(const char * buf, const int length, ScbColorPtr ptr)
{
    if (NULL == ptr)
    {
        SCB_ERROR("Invalid color pointer!");
        return SCB_RET_ERR;
    }

    if (NULL == buf || 0 >= length)
    {
        SCB_ERROR("Buffer or lenght is invalid!");
        return SCB_RET_ERR;
    }
    int r = 0, g = 0, b = 0;
    sscanf(buf,  "#%02x%02x%02x", &r, &g, &b);
    ptr->red = (unsigned short)r;
    ptr->green = (unsigned short)g;
    ptr->blue = (unsigned short)b;
    return SCB_RET_OK;
}

ScbDevColor scb_html_color_to_dev_color(const char *buf, const int length)
{
    if (NULL == buf || 0 >= length)
    {
        SCB_ERROR("Buffer or lenght is invalid!");
        return SCB_DEV_COLOR_UNKNOWN;
    }
    int r = 0, g = 0, b = 0;
    ScbColor color;
    sscanf(buf,  "#%02x%02x%02x", &r, &g, &b);
    color.red = (unsigned short)r;
    color.green = (unsigned short)g;
    color.blue = (unsigned short)b;
    return scb_color_to_dev_color(&color);
}

//------------------------------------------------------------------------
// convert dev color to color and html color
void scb_dev_color_to_color(ScbColorPtr ptr, const ScbDevColor color)
{
    if (NULL == ptr) return;
    switch(color)
    {
    case  SCB_DEV_COLOR_WHITE :
        scb_color_set_white(ptr);
        break;
    case SCB_DEV_COLOR_LIGHT_GRAY :
        scb_color_set_light_gray(ptr);
        break;
    case SCB_DEV_COLOR_DARK_GRAY :
        scb_color_set_dark_gray(ptr);
        break;
    default:
        scb_color_set_black(ptr);
        break;
    }
}

const char * scb_dev_color_to_html_color(const ScbDevColor color)
{
    if (SCB_DEV_COLOR_WHITE == color)
    {
        snprintf(_g_htmlColor, SCB_MAX_COLOR_LEN,
            "#%02X%02X%02X", 0xff, 0xff, 0xff);
    }
    else if (SCB_DEV_COLOR_LIGHT_GRAY == color)
    {
        snprintf(_g_htmlColor, SCB_MAX_COLOR_LEN,
            "#%02X%02X%02X", 0xaa, 0xaa, 0xaa);
    }
    else if (SCB_DEV_COLOR_DARK_GRAY == color)
    {
        snprintf(_g_htmlColor, SCB_MAX_COLOR_LEN,
            "#%02X%02X%02X", 0x55, 0x55, 0x55);
    }
    else 
    {
        snprintf(_g_htmlColor, SCB_MAX_COLOR_LEN,
            "#%02X%02X%02X", 0x00, 0x00, 0x00);
    }
    return _g_htmlColor;
}



