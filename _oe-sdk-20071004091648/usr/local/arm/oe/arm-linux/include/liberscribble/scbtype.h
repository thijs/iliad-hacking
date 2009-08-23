#ifndef SCBTYPE_H_
#define SCBTYPE_H_

//////////////////////////////////////////////////////////////////////////
// scribble library type definition
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

// line style
typedef enum _ScbLineStyle
{
    SCB_LINE_SOLID,
    SCB_LINE_INVALID,
}ScbLineStyle;

// rect
typedef struct _ScbRect
{
    int     left;
    int     top;
    int     right;
    int     bottom;
}ScbRect;
typedef ScbRect * ScbRectPtr;

// function
const char * scb_line_style_to_str(const ScbLineStyle style);
ScbLineStyle scb_line_style_from_str(const char * str);

#ifdef __cplusplus
}
#endif

#endif
