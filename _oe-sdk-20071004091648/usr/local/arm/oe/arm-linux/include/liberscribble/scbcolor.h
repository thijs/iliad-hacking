#ifndef SCBCOLOR_H_
#define SCBCOLOR_H_

//////////////////////////////////////////////////////////////////////////
// scribble library color definition
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

// color used in iLiad device. serves as input paramter
typedef enum _ScbDevColor
{
    SCB_DEV_COLOR_WHITE         = 0, 
    SCB_DEV_COLOR_LIGHT_GRAY    = 1,
    SCB_DEV_COLOR_DARK_GRAY     = 2,
    SCB_DEV_COLOR_BLACK         = 3,
    SCB_DEV_COLOR_UNKNOWN       = 0xffffffff
}ScbDevColor;

// color struct here is similar to color used in X11 and GDK
// ScbColor is a output parameter
typedef struct _ScbColor
{
    unsigned long   pixel;      
    unsigned short  red;
    unsigned short  green;
    unsigned short  blue;	   
    char            flags;    /* DoRed, DoGreen, DoBlue */
    char            pad;
}ScbColor;
typedef ScbColor * ScbColorPtr;

// generic color
void scb_color_set_white(ScbColorPtr ptr);
void scb_color_set_black(ScbColorPtr ptr);
void scb_color_set_light_gray(ScbColorPtr ptr);
void scb_color_set_dark_gray(ScbColorPtr ptr);

// convert color to html color and dev color
const char * scb_color_to_html_color(const ScbColorPtr ptr);
ScbDevColor scb_color_to_dev_color(ScbColorPtr ptr);

// convert html color to color and dev color
int scb_html_color_to_color(const char * buf, const int length, ScbColorPtr ptr);
ScbDevColor scb_html_color_to_dev_color(const char *buf, const int length);

// convert dev color to color and html color
void scb_dev_color_to_color(ScbColorPtr ptr, const ScbDevColor color);
const char * scb_dev_color_to_html_color(const ScbDevColor color);

#ifdef __cplusplus
}
#endif

#endif
