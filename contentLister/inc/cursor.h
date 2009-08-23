/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file cursor.h
 * \brief content lister - eReader cursor handling                  
 * 
 * <File description>
 * 
 * Depending on the configure value "cursor-used" the e-reader specific cursor code will be 
 * enabled/disabled  
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

// TODO Make #include work
//#include <irex/er0100_einkfb.h>
typedef unsigned short u16;
// TODO For now hand-copy all definitions from <irex/er0100_einkfb.h>

/*
 * Framebuffer Driver for Experimental E-Ink Display
 * 
 * Copyright 2006-2008 iRex Technologies B.V.
 * 
 * Author: Jan van de Kamer
 * 
 * Version 1.0
 * 
 * This software and associated documentation files (the "Software") are copyright 
 * 2006-2008 iRex Technologies B.V. All Rights Reserved.
 * A copyright license is hereby granted for redistribution and use of the Software in
 * source and binary forms, with or without modification, provided that the following 
 * conditions are met:
 * => Redistributions of source code must retain the above copyright notice, this
 * copyright license and the following disclaimer.
 * => Redistributions in binary form must reproduce the above copyright notice, this
 * copyright license and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution.
 * => Neither the name of iRex Technologies B.V. nor the names of its
 * subsidiaries may be used to endorse or promote products derived from the Software
 * without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF   
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


#define WAVEFORM_SIZE                   (128*1024)

/* IO Calls */
#define FBIO_IOCTL_BASE                 'v'
#define FBIO_DISPLAY                    _IOW(FBIO_IOCTL_BASE, 1, struct display_update_info)
#define FBIO_ERASE_WHITE                _IOW(FBIO_IOCTL_BASE, 2, struct display_update_info)
#define FBIO_DRAWPIXELS                 _IOW(FBIO_IOCTL_BASE, 3, struct transfer_buffer)
#define FBIO_DRAW_BRUSH                 _IOW(FBIO_IOCTL_BASE, 4, struct brush_draw_info_obsolete)
#define FBIO_REDRAW                     _IOW(FBIO_IOCTL_BASE, 5, struct display_update_info)
#define FBIO_UPLOAD_WAVEFORM            _IOC(_IOC_WRITE, FBIO_IOCTL_BASE, 6, WAVEFORM_SIZE)
#define FBIO_UPLOAD_BRUSH               _IO(FBIO_IOCTL_BASE, 7)
#define FBIO_DRAW_BUBBLE                _IOW(FBIO_IOCTL_BASE, 8, struct brush_draw_info_obsolete)
#define FBIO_DRAW_BRUSH_GENERIC         _IOW(FBIO_IOCTL_BASE, 9, struct brush_draw_info)
#define FBIO_START_PROGRESS_BAR         _IO(FBIO_IOCTL_BASE, 10)


/* Waveforms */
#define WAVEFORM_2BPP_IMAGE             0
#define WAVEFORM_4BPP_IMAGE             1
#define WAVEFORM_DRAW                   4
#define WAVEFORM_FAST_BLACK_WHITE       6
#define WAVEFORM_TYPING                 9

#define HARDWARE_V3                     1   // Thom:required to select between hardware - must be autoselect before production.


/* Boot progress bar indicator defines */
#define ESTIMATED_BOOT_TIME             (39)
#define ESTIMATED_BOOT_JIFFIES          (ESTIMATED_BOOT_TIME * 100)
#define BOOT_PROGRESS_STEPS             (18)
#define BOOT_PROGRESS_START_STEP        (0)
#define PROGRESS_STEP_JIFFIES           (ESTIMATED_BOOT_JIFFIES / (BOOT_PROGRESS_STEPS - BOOT_PROGRESS_START_STEP))

/* Defined brush coordinates */
#define BLACK_CURSOR_X                  0
#define BLACK_CURSOR_Y                  24
#define BLACK_CURSOR_SIZE_X             114
#define BLACK_CURSOR_SIZE_Y             24

#define WHITE_CURSOR_X                  0
#define WHITE_CURSOR_Y                  0
#define WHITE_CURSOR_SIZE_X             114
#define WHITE_CURSOR_SIZE_Y             24

#define BLACK_BUBBLE_X                  102
#define BLACK_BUBBLE_Y                  49
#define BLACK_BUBBLE_SIZE_X             7
#define BLACK_BUBBLE_SIZE_Y             7

#define WHITE_BUBBLE_X                  21
#define WHITE_BUBBLE_Y                  49
#define WHITE_BUBBLE_SIZE_X             7
#define WHITE_BUBBLE_SIZE_Y             7

#define PROGRESS_BLOCK_BWHITE_X         118 
#define PROGRESS_BLOCK_BWHITE_Y         22
#define PROGRESS_BLOCK_BWHITE_SIZE_X    19 
#define PROGRESS_BLOCK_BWHITE_SIZE_Y    36

#define PROGRESS_BLOCK_WHITE_X          138
#define PROGRESS_BLOCK_WHITE_Y          22
#define PROGRESS_BLOCK_WHITE_SIZE_X     19 
#define PROGRESS_BLOCK_WHITE_SIZE_Y     36

#define PROGRESS_BLOCK_BLACK_X          159 
#define PROGRESS_BLOCK_BLACK_Y          22
#define PROGRESS_BLOCK_BLACK_SIZE_X     19 
#define PROGRESS_BLOCK_BLACK_SIZE_Y     36


#define WHITE_BRUSH_X                   1
#define LGRAY_BRUSH_X                   28
#define DGRAY_BRUSH_X                   55
#define BLACK_BRUSH_X                   82
#define BRUSH_Y                         49

#define MAX_BRUSH_SIZE                  6

/* Brush coklors */
#define WHITE                           0
#define LIGHT_GRAY                      1
#define DARK_GRAY                       2
#define BLACK                           3

#define PIXELBUFSIZE                    25


typedef enum
{
    e_cursor_hide = 0,
    e_cursor_show,
    e_cursor_blink,
    e_cursor_undefined
} cursor_state_t;


struct coordinates 
{
  u16 x1;
  u16 y1;
  u16 x2;
  u16 y2;
};

struct point_info
{
    unsigned short x;
    unsigned short y;
    unsigned char  size;
    unsigned char  color;
  unsigned char  pen_down;
};

struct transfer_buffer
{
  unsigned int count;
  struct       point_info point_list [PIXELBUFSIZE];
};

struct display_update_info 
{
    int waveform;
    int sequence;
};

/* Used for old -to be obsolete- API call FBIO_DRAW_BRUSH */
struct brush_draw_info_obsolete 
{
    unsigned short  x;              /* x position on screen     */
    unsigned short  y;              /* Y position on screen     */
    unsigned int    size;           /* size of brush            */
    unsigned short  color;          /* color of brush           */
};

/* Used for new API call FBIO_DRAW_BRUSH_GENERIC          */
struct brush_draw_info
{
    unsigned short sourcex;
    unsigned short sourcey;
    unsigned short sizex;
    unsigned short sizey;
    unsigned short destx;
    unsigned short desty;
    unsigned short brushlsb;
};
// TODO end #include <irex/er0100_einkfb.h>
 
#ifndef __CL_CURSOR__H__
#define __CL_CURSOR__H__

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

/**
 * Load bitmap, open framebuffer_device, init values...
 * 
 * @returns 0/-1 = ok/failure
 */
int cursor_init(ContentLister* theContentLister);

/**
 * Show/hide/blink cursor at lister-item position
 *
 * @param pos - index of lister item (0 ..)
 * @param state - new state of cursor
 *
 * @returns 0/-1 = ok/failure
 */
int cursor_set_state(const int pos, const cursor_state_t state);

/**
 * Hide cursor for all lister items
 *
 * @param --
 *
 * @returns --
 */
void cursor_hide_all(void);

/**
 * Redraw cursur, when needed
 *
 * @param --
 *
 * @returns --
 */
void cursor_redraw(void);

/**
 * Free resources used by cursor
 *
 * @param -
 *
 * @returns -
 */
void cursor_destroy();

/**
 *  Start progress indicator (knight rider)
 *
 *  @param -
 *
 *  @returns -
 */
void start_progress_indicator(void);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  //__CL_CURSOR__H__
