#ifndef DRIVER_H_
#define DRIVER_H_

// use kernel driver to fast draw pixel
// internal use
#include <sys/time.h>
#include "scbconfig.h"
#include "scbpoint.h"
#include <liberdm/erdm.h>
#include <glib.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _DrvPointInfo
{
    unsigned short x;
    unsigned short y;
    unsigned char  size;
    unsigned char  color;
    unsigned char  pen_down;
}DrvPointInfo;

typedef struct _DrvPointsBuf
{
    unsigned int count;
    DrvPointInfo points [SCB_DEF_FAST_DRAW_BUF_LEN];
} DrvPointsBuf;
typedef DrvPointsBuf * DrvPointsBufPtr;

// for ioctl request
void scb_driver_init();
void scb_driver_close();
void scb_driver_draw(DrvPointsBufPtr ptr);

// fast draw control 
typedef struct _FastDrawContext
{
    struct timeval  t1, t2;         // start end
    DrvPointsBuf    points;
}FastDrawContext;

void scb_fast_draw_reset_context();
gboolean scb_fast_draw_now();
void scb_fast_draw_record(const ScbDevPointPtr ptr, 
                          const unsigned char size, 
                          const unsigned char color,
                          const unsigned char pen_down);
void scb_fast_draw();
void scb_fast_draw_points(DrvPointsBufPtr ptr);

#ifdef __cplusplus
}
#endif


#endif
