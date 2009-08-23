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
 * \file cursor.c
 * \brief content lister - eReader cursor handling    
 *                         (x,y) --> (y,x) on e-inkt display              
 * 
 * <File description>
 * 
 * Depending on the configure value "cursor-used" the e-reader specific cursor code will be 
 * enabled/disabled  
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#include <config.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <liberdm/display.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "displayUpdate.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "cursor.h"
#include "lister.h"

#include "Geometry.h"

// Dirty trick... 
// The cursor is drawn by using a predifined brush that
// is designed for 6 items on a page. We use a fixed 
// small rectangle instead for all layouts...
const int H=WHITE_CURSOR_SIZE_Y;	// fixed size
const int W=WHITE_CURSOR_SIZE_Y;
const int CURSOR_START=WHITE_CURSOR_X+12;   // skip rounded part

// local data
static struct display_update_info updateinfo;

static ContentLister* g_contentLister = NULL;

static int      g_framebuffer_device   = -1;
static gboolean g_cursor_needs_update  = FALSE;

static guint    g_cursor_num_blinking  = 0;
static guint    g_cursor_blink_handler = 0;

static int      g_cursor_x_pos;        // normal cursor position
static int      g_cursor_x_pos_alt;    // alternate cursor position, used in blinking mode
static int      g_cursor_x_pos_blink;  // current cursor position for blinking cursors

static struct
{
    int             y_pos;
    cursor_state_t  state;
} cursor_info[MAX_ITEMS_ON_ONE_PAGE];


// local functions
static gboolean cursor_blink(gpointer user_data);
static void     cursor_blink_handler_stop(void);


// Note: The framebuffer is intended as a landscape screen with (0,0) at the connect button.
//       The X coordinate in the framebuffer is the Y coordinate as we see it.
//       The Y coordinate in the framebuffer is 768 - the X coordinate as we see it. 
int cursor_init(ContentLister* theContentLister)
{
    int i;

    g_contentLister = theContentLister;

    // open framebuffer device
    g_framebuffer_device = open("/dev/fb0", O_RDWR);
    if (g_framebuffer_device == -1)
    {
        CL_ERRORPRINTF("Error opening framebufferdevice. # mknod /dev/fb0 c 29 0");
        return -1;  // -1 = error
    }

    // determine X coordinate
    g_cursor_x_pos     = _G.cursor.x;
    g_cursor_x_pos_alt = g_cursor_x_pos - W;
    if (g_cursor_x_pos_alt < W + 3)
    {
        g_cursor_x_pos_alt = W + 3;
    }
    g_cursor_x_pos_blink = g_cursor_x_pos;

    // determine Y coordinate
    for (i = 0 ; i < _G.pageItems ; i++)
    {
        cursor_info[i].y_pos = _G.cursor.y + i * _G.cursorStep;
        cursor_info[i].state = e_cursor_hide;
    }

    // set waveform parameters
    updateinfo.waveform = WAVEFORM_FAST_BLACK_WHITE;
    updateinfo.sequence = 0;
    
    return 0;  // 0 = ok
}


// Note: The framebuffer is intended as a landscape screen with (0,0) at the connect button.
//       The X coordinate in the framebuffer is the Y coordinate as we see it.
//       The Y coordinate in the framebuffer is 768 - the X coordinate as we see it. 
int cursor_set_state(const int pos, const cursor_state_t new_state)
{
    int retVal = 0;  // 0 = ok

    if (pos == -1)
    {
        return 0;  // 0 = ok
    }
    CL_CURSORPRINTF("entry: pos [%d] new_state [%d]", pos, new_state);
    g_return_val_if_fail((pos >= 0  &&  pos < _G.pageItems), -1);

    int                    rc;
    struct brush_draw_info drawinfo;
    cursor_state_t         old_state = cursor_info[pos].state;
    GtkWidget*             listItem  = lsGetListerItem(g_contentLister->lister, pos);

    if (new_state == e_cursor_hide)
    {
        gtk_content_list_item_show_cursor(GTK_CONTENT_LIST_ITEM(listItem), FALSE);

        // remove cursor (i.e. paint white)
        drawinfo.sourcex  = CURSOR_START;
        drawinfo.sourcey  = WHITE_CURSOR_Y;
        drawinfo.sizex    = H;
        drawinfo.sizey    = W;
        drawinfo.destx    = cursor_info[pos].y_pos;
        drawinfo.desty    = _G.screen.w - ((old_state == e_cursor_blink) ? g_cursor_x_pos_blink : g_cursor_x_pos);
        drawinfo.brushlsb = WHITE;
    }
    else
    {
        gtk_content_list_item_show_cursor(GTK_CONTENT_LIST_ITEM(listItem), TRUE);

        // show cursor (black)
        drawinfo.sourcex  = CURSOR_START;
        drawinfo.sourcey  = BLACK_CURSOR_Y;
        drawinfo.sizex    = H;
        drawinfo.sizey    = W;
        drawinfo.destx    = cursor_info[pos].y_pos;
        drawinfo.desty    = _G.screen.w - ((new_state == e_cursor_blink) ? g_cursor_x_pos_blink : g_cursor_x_pos);
        drawinfo.brushlsb = BLACK;
    }
    cursor_info[pos].state = new_state;

    // write the brush/drawing to framebuffer (in-memory)
    if (display_update_get_level() < NO_DISPLAY_UPDATE_LEVEL)
    {
        CL_CURSORPRINTF("FBIO_DRAW_BRUSH location %dx%d color %d", drawinfo.desty, drawinfo.destx, drawinfo.brushlsb);
        rc = ioctl(g_framebuffer_device, FBIO_DRAW_BRUSH_GENERIC, &drawinfo);
        if (rc != 0)
        {
            CL_ERRORPRINTF("Error drawing brush (err=0x%x)", rc);
            retVal = -1;  // -1 = error
        }
    }

    // set blinking parameters
    //   adjust number of blinking cursors
    if (old_state == e_cursor_blink  &&  g_cursor_num_blinking > 0)
    {
        g_cursor_num_blinking--;
    }
    if (new_state == e_cursor_blink)
    {
        g_cursor_num_blinking++;
    }
    g_cursor_num_blinking = g_cursor_num_blinking;
    //   start/stop blink timeout handler
    if (g_cursor_num_blinking == 0)
    {
        if (g_cursor_blink_handler != 0)
        {
            cursor_blink_handler_stop();
            g_cursor_x_pos_blink = g_cursor_x_pos;
        }
    }
    else
    {
        if (g_cursor_blink_handler == 0)
        {
            g_cursor_blink_handler = g_timeout_add(800, cursor_blink, NULL);
        }
    }

    // screen update needed except when blink
    if (   (new_state == e_cursor_blink  &&  g_cursor_num_blinking > 1)
        || (old_state == e_cursor_blink  &&  g_cursor_num_blinking > 0) )
    {
        // wait for next blink timer to do screen update
    }
    else
    {
        // do screen update now
        g_cursor_needs_update = TRUE;
    }

    return retVal;
}

void cursor_hide_all(void)
{
    int                    rc;
    int                    pos;
    struct brush_draw_info drawinfo;
    GtkWidget*             listItem = NULL;

    CL_CURSORPRINTF("entry");

    // stop blink handler
    cursor_blink_handler_stop();
    g_cursor_num_blinking = 0;
    g_cursor_x_pos_blink  = g_cursor_x_pos;

    // hide all cursors (i.e. paint white)
    drawinfo.sourcex  = CURSOR_START;
    drawinfo.sourcey  = WHITE_CURSOR_Y;
    drawinfo.sizex    = H;
    drawinfo.sizey    = W;
    drawinfo.brushlsb = WHITE;
    for (pos = 0 ; pos < _G.pageItems ; pos++)
    {
        drawinfo.destx = cursor_info[pos].y_pos;

        cursor_info[pos].state = e_cursor_hide;
        
        // hide gtk cursor
        listItem  = lsGetListerItem(g_contentLister->lister, pos);
        gtk_content_list_item_show_cursor(GTK_CONTENT_LIST_ITEM(listItem), FALSE);

        // write the brush/drawing for standard location to framebuffer (in-memory)
        drawinfo.desty = _G.screen.w - g_cursor_x_pos;
        if (display_update_get_level() < NO_DISPLAY_UPDATE_LEVEL)
        {
            CL_CURSORPRINTF("FBIO_DRAW_BRUSH location %dx%d color %d", drawinfo.desty, drawinfo.destx, drawinfo.brushlsb);
            rc = ioctl(g_framebuffer_device, FBIO_DRAW_BRUSH_GENERIC, &drawinfo);
            if (rc != 0)
            {
                CL_ERRORPRINTF("Error drawing brush (err=0x%x)", rc);
            }
        }
        
        // write the brush/drawing for alternative location to framebuffer (in-memory)
        drawinfo.desty = _G.screen.w - g_cursor_x_pos_alt;
        if (display_update_get_level() < NO_DISPLAY_UPDATE_LEVEL)
        {
            CL_CURSORPRINTF("FBIO_DRAW_BRUSH location %dx%d color %d", drawinfo.desty, drawinfo.destx, drawinfo.brushlsb);
            rc = ioctl(g_framebuffer_device, FBIO_DRAW_BRUSH_GENERIC, &drawinfo);
            if (rc != 0)
            {
                CL_ERRORPRINTF("Error drawing brush (err=0x%x)", rc);
            }
        }
        g_cursor_needs_update = TRUE;
    }
}

void cursor_redraw()
{
    CL_CURSORPRINTF("entry");

    int  rc;

    if (   g_cursor_needs_update
        && display_update_get_level() < NO_DISPLAY_UPDATE_LEVEL )
    {
        // display the modification on screen
        CL_CURSORPRINTF("FBIO_REDRAW waveform %d - sequence %d", updateinfo.waveform, updateinfo.sequence);
        rc = ioctl(g_framebuffer_device, FBIO_REDRAW, &updateinfo);
        if (rc != 0)
        {
            CL_ERRORPRINTF("Error sending FBIO_REDRAW: %x", rc);
        }
        g_cursor_needs_update = FALSE;
    }
}

static void cursor_blink_handler_stop(void)
{
    CL_CURSORPRINTF("entry: blink_handler [%d]", g_cursor_blink_handler);

    if (g_cursor_blink_handler != 0)
    {
        g_source_remove(g_cursor_blink_handler);
        g_cursor_blink_handler = 0;
    }
}

static gboolean cursor_blink(gpointer user_data)
{
    int                    rc;
    int                    pos;
    gboolean               call_me_again = FALSE;
    struct brush_draw_info drawinfo;

    CL_CURSORPRINTF("entry");

    if (g_cursor_num_blinking == 0)
    {
        CL_WARNPRINTF("Stop blink handler, because num_blinking is zero");
        g_cursor_blink_handler = 0;
        return call_me_again;
    }

    // hide old cursors (i.e. paint white)
    drawinfo.sourcex  = CURSOR_START;
    drawinfo.sourcey  = WHITE_CURSOR_Y;
    drawinfo.sizex    = H;
    drawinfo.sizey    = W;
    drawinfo.desty    = _G.screen.w - g_cursor_x_pos_blink;
    drawinfo.brushlsb = WHITE;
    for (pos = 0 ; pos < _G.pageItems ; pos++)
    {
        if (cursor_info[pos].state == e_cursor_blink)
        {
            drawinfo.destx = cursor_info[pos].y_pos;

            // write the brush/drawing to framebuffer (in-memory)
            if (display_update_get_level() < NO_DISPLAY_UPDATE_LEVEL)
            {
                CL_CURSORPRINTF("FBIO_DRAW_BRUSH location %dx%d color %d", drawinfo.desty, drawinfo.destx, drawinfo.brushlsb);
                rc = ioctl(g_framebuffer_device, FBIO_DRAW_BRUSH_GENERIC, &drawinfo);
                if (rc != 0)
                {
                    CL_ERRORPRINTF("Error drawing brush (err=0x%x)", rc);
                }
            }
        }
    }

    // determine new cursor position
    if (g_cursor_x_pos_blink == g_cursor_x_pos)
    {
        g_cursor_x_pos_blink = g_cursor_x_pos_alt;
    }
    else
    {
        g_cursor_x_pos_blink = g_cursor_x_pos;
    }

    // show new cursors (black)
    drawinfo.sourcex  = CURSOR_START;
    drawinfo.sourcey  = BLACK_CURSOR_Y;
    drawinfo.sizex    = H;
    drawinfo.sizey    = W;
    drawinfo.desty    = _G.screen.w - g_cursor_x_pos_blink;
    drawinfo.brushlsb = BLACK;
    for (pos = 0 ; pos < _G.pageItems ; pos++)
    {
        if (cursor_info[pos].state == e_cursor_blink)
        {
            drawinfo.destx = cursor_info[pos].y_pos;

            // write the brush/drawing to framebuffer (in-memory)
            if (display_update_get_level() < NO_DISPLAY_UPDATE_LEVEL)
            {
                CL_CURSORPRINTF("FBIO_DRAW_BRUSH location %dx%d color %d", drawinfo.desty, drawinfo.destx, drawinfo.brushlsb);
                rc = ioctl(g_framebuffer_device, FBIO_DRAW_BRUSH_GENERIC, &drawinfo);
                if (rc != 0)
                {
                    CL_ERRORPRINTF("Error drawing brush (err=0x%x)", rc);
                }
            }
        }
    }

    // update screen
    g_cursor_needs_update = TRUE;
    cursor_redraw();
    
    // trigger next blink
    call_me_again = TRUE;
    return call_me_again;
}

void cursor_destroy()
{
    close(g_framebuffer_device);
}

void start_progress_indicator()
{
    int framebuffer_device;

    // start the timer now, the viewer has been launched
    if (display_update_get_level() < NO_DISPLAY_UPDATE_LEVEL)
    {
        framebuffer_device = open("/dev/fb0", O_RDWR);    
        if ((ioctl(framebuffer_device, FBIO_START_PROGRESS_BAR)) != 0)
        {
            CL_ERRORPRINTF("einkfb IOCTL failed");
        }
        close(framebuffer_device);
    }
}

