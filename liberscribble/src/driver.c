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


#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "driver.h"
#include "scblog.h"

#define FBIO_IOCTL_BASE     'v'
#define FBIO_DRAWPIXELS     _IOW(FBIO_IOCTL_BASE, 3, DrvPointsBuf)

int fbdev = 0;

void scb_driver_init()
{
    if (fbdev)
    {
        SCB_ERROR("/dev/fb0 already open!");
        return;
    }

    fbdev = open("/dev/fb0", O_RDWR);
    if (-1 == fbdev)
    {
        SCB_ERROR("Could not open /dev/fb0!");
        return;
    }
}

void scb_driver_close()
{
    // always close
    close(fbdev);
}

void scb_driver_draw(DrvPointsBufPtr ptr)
{
    if (ptr)
    {   
        if (0 != ioctl (fbdev, FBIO_DRAWPIXELS, ptr))
        {
            SCB_ERROR("ioctl request error!");
        }
    }
}

//------------------------------------------------------------------------
// fast draw
FastDrawContext  s_fast_draw_ctx;
void scb_fast_draw_reset_context()
{
    s_fast_draw_ctx.points.count = 0;
    gettimeofday(&s_fast_draw_ctx.t1, NULL);
    s_fast_draw_ctx.t2.tv_sec   = s_fast_draw_ctx.t1.tv_sec;
    s_fast_draw_ctx.t2.tv_usec  = s_fast_draw_ctx.t1.tv_usec;
}

//------------------------------------------------------------------------
gboolean scb_fast_draw_now()
{
    // check points 
    if (s_fast_draw_ctx.points.count > SCB_DEF_FAST_DRAW_COUNT)
    {
        //SCB_TRACE("Should draw now points!");
        return TRUE;
    }

    // check time
    static const long usec = 1000 * 1000;
    long elapsed = ( s_fast_draw_ctx.t2.tv_sec - s_fast_draw_ctx.t1.tv_sec ) * usec;
    elapsed += s_fast_draw_ctx.t2.tv_usec - s_fast_draw_ctx.t1.tv_usec;

    if (elapsed >= SCB_DEF_FAST_DRAW_TIME)
    {
        ///SCB_TRACE("Should draw now time!");
        return TRUE;
    }
    //SCB_TRACE("No, not yet. Waiting for...");
    return FALSE;
}

//------------------------------------------------------------------------
// record 
void scb_fast_draw_record(const ScbDevPointPtr ptr, 
                          const unsigned char size, 
                          const unsigned char color,
                          const unsigned char pen_down)
{
    // make a check
    if (s_fast_draw_ctx.points.count <  SCB_DEF_FAST_DRAW_BUF_LEN && ptr)
    {
        s_fast_draw_ctx.points.points[s_fast_draw_ctx.points.count].x = ptr->x;
        s_fast_draw_ctx.points.points[s_fast_draw_ctx.points.count].y = ptr->y;
        s_fast_draw_ctx.points.points[s_fast_draw_ctx.points.count].size = size;
        s_fast_draw_ctx.points.points[s_fast_draw_ctx.points.count].color = color;
        s_fast_draw_ctx.points.points[s_fast_draw_ctx.points.count].pen_down = pen_down;
        ++s_fast_draw_ctx.points.count;
        gettimeofday(&s_fast_draw_ctx.t2, NULL);
    }
}

//------------------------------------------------------------------------
void scb_fast_draw()
{
    scb_driver_draw(&s_fast_draw_ctx.points);
}

//------------------------------------------------------------------------
void scb_fast_draw_points(DrvPointsBufPtr ptr)
{
    scb_driver_draw(ptr);
}
