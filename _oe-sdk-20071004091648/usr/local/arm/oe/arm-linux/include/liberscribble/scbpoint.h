#ifndef SCBPOINT_H_
#define SCBPOINT_H_

#ifdef __cplusplus
extern "C"
{
#endif

// the point defined here is same with X11 and GDK
// we also have another struct dev point, which contains pressure info
// ScbPoint is used when scribble library needs to expose data to gdk/X11
typedef struct _ScbPoint
{
    int     x;
    int     y;
}ScbPoint;
typedef ScbPoint * ScbPointPtr;

// device point with pressure. Serves as input parameter
typedef struct _ScbDevPoint
{
    int     x;
    int     y;
    int     pressure;
}ScbDevPoint;
typedef ScbDevPoint * ScbDevPointPtr;

// read point data from string. replaced by other function
// int scb_point_read(ScbPointPtr p, const char * buf, const int length);

// write point data into string. replaced by other function
// int scb_point_write(const ScbPointPtr p, char * buf, const int length);

// dump
void scb_point_dump(const ScbPointPtr p);


#ifdef __cplusplus
}
#endif

#endif
