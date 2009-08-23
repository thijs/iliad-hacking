/*
 * $Id: xcalibrate.h,v 1.1.1.1 2004-06-02 19:26:08 pb Exp $
 *
 * Copyright © 2003 Philip Blundell
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Philip Blundell not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Philip Blundell makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * PHILIP BLUNDELL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL PHILIP BLUNDELL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef XCALIBRATE_H
#define XCALIBRATE_H

typedef struct {
    int type;			/* event base */
    unsigned long serial;
    Bool send_event;
    Display *display;
    int x;
    int y;
    int pressure;
} XCalibrateRawTouchscreenEvent;

Bool XCalibrateQueryExtension (Display *dpy, int *event_basep, int *error_basep);

Status XCalibrateQueryVersion (Display *dpy,
			       int     *major_versionp,
			       int     *minor_versionp);

Status XCalibrateSetRawMode (Display *dpy, Bool enable);

#endif
