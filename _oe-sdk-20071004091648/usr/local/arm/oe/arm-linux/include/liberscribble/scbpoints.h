#ifndef SCBPOINTS_H_
#define SCBPOINTS_H_
#include <glib.h>
#include "scbpoint.h"

#ifdef __cplusplus
extern "C"
{
#endif

// use two arrays so that we can easily expose the points to GTK/GDK and X11
// without allocating any other memory
typedef struct _ScbPoints
{
    GArray* points;
    GArray* pressures;
}ScbPoints;
typedef ScbPoints * ScbPointsPtr;

// allocate an array with initSize
gboolean scb_points_new(ScbPointsPtr points, const int initSize);

// release the array and all items in the array
void scb_points_free(ScbPointsPtr points);

// push point back to array
void scb_points_append(ScbPointsPtr points, ScbDevPointPtr point);

// how many points does the array contain
int scb_points_get_count(ScbPointsPtr ptr);

// retrieve data &ScbPoint[0]. output
ScbPointPtr scb_points_get_data(ScbPointsPtr points);

// dump
void scb_points_dump(ScbPointsPtr points);

#ifdef __cplusplus
}
#endif

#endif
