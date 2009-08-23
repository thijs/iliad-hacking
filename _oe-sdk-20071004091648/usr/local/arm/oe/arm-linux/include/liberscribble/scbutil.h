#ifndef SCBUTIL_H_
#define SCBUTIL_H_

#include <scbtype.h>
#include <glib.h>
#include <scbpoint.h>

//////////////////////////////////////////////////////////////////////////
// scribble library util. Internal use
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

// check two rectangles is intersect or not
gboolean scb_is_rect_intersect(const ScbRectPtr rect1, 
                               const ScbRectPtr rect2);

gboolean scb_is_lines_intersect(const ScbPointPtr p1, 
                                const ScbPointPtr p2,
                                const ScbPointPtr p3, 
                                const ScbPointPtr p4);

#ifdef __cplusplus
}
#endif

#endif
