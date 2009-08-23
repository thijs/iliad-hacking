#ifndef SCBSTROKES_H_
#define SCBSTROKES_H_
#include "scbstroke.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _ScbStrokes
{
    GList * strokes;
}ScbStrokes;
typedef ScbStrokes * ScbStrokesPtr;

// construct a new stroke list
gboolean scb_strokes_new(ScbStrokesPtr ptr);

// free operation will free the list item and list
void scb_strokes_free(ScbStrokesPtr strokes);

// empty operation will clear all items but does not remove the list
void scb_strokes_empty(ScbStrokesPtr strokes);

// detach operation destory the list without freeing each element
// detach used when caller wants a temp container for existing items
void scb_strokes_detach(ScbStrokesPtr strokes);

// get count
int scb_strokes_get_count(const ScbStrokesPtr strokes);

// add stroke to the strokes list
void scb_strokes_add_stroke(ScbStrokesPtr strokes, 
                            ScbStrokePtr stroke);

// copy from one list to the other
void scb_strokes_add_strokes(ScbStrokesPtr dst, 
                            const ScbStrokesPtr src);

// detach stroke from list without freeing stroke memory
// notice, caller should release memory of stroke returned
// caller can also add the return value to other stroke container
ScbStrokePtr scb_strokes_detach_stroke(ScbStrokesPtr strokes, 
                            ScbStrokePtr stroke);

// hit test, the result is a stroke list, empty means no intersect
// caller should call scb_stroke_detach
ScbStrokesPtr scb_strokes_point_hit_test(ScbStrokesPtr strokes, 
                                   ScbDevPointPtr point,
                                   const ScbHitTestCtxPtr ctx);

// hit test with line
// caller should call scb_stroke_detach
ScbStrokesPtr scb_strokes_line_hit_test(ScbStrokesPtr strokes, 
                                  ScbDevPointPtr point1,
                                  ScbDevPointPtr point2,
                                  const ScbHitTestCtxPtr ctx);

void scb_strokes_erase_fast_draw(ScbStrokesPtr strokes);

// save / load
gboolean scb_strokes_save(ScbStrokesPtr strokes, ScbXmlPtr ptr);
gboolean scb_strokes_load(ScbStrokesPtr strokes, ScbXmlPtr ptr);

// dump
void scb_strokes_dump(ScbStrokesPtr strokes);

#ifdef __cplusplus
}
#endif

#endif
