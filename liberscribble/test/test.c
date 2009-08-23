/*
 * This file is part of liberscribble.
 *
 * liberscribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberscribble is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

#include <scb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <driver.h>

#define _DEBUG 1

#define SLEEP       0

#ifdef _DEBUG
#define TRACE(x, args...) fprintf(stderr, "(SCB_TEST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args) 
#else
#define TRACE(x, args...) do {} while (0)
#endif

GTimer * timeStamp = NULL;

void timestart()
{
    if (NULL == timeStamp)
    {
        timeStamp = g_timer_new();
    }
    g_timer_start(timeStamp);  
}

void timeend()
{
    if (NULL == timeStamp) return;
    g_timer_stop(timeStamp);
    gulong microSecs = 0;
    gdouble secs = g_timer_elapsed(timeStamp, &microSecs);
    TRACE("time %f", secs);
}


//========================================================================
// test strokes
void testStrokes()
{
    static const int STROKES = 5;    // stroke count per strokes
    static const int POINTS  = 5;
    ScbStrokes strokes;
    ScbStrokes tmp;
    ScbPoint point;
    scb_strokes_new(&strokes);
    scb_strokes_new(&tmp);
    int i, j ; 
    for(i = 0; i < STROKES; ++i)
    {
        ScbStrokePtr stroke = scb_stroke_new();
        // add points
        for(j = 0; j < POINTS; ++j)
        {
            point.x = i ; point.y = j; point.pressure = i * j;
            scb_stroke_add_point(stroke, &point);
        }
        scb_strokes_add_stroke(&strokes, stroke);
        scb_strokes_add_stroke(&tmp, stroke);
            
    }
    TRACE("Add done! Now test detach and remove");

    // ok, now test 
    scb_strokes_detach(&tmp);
    TRACE("Dump strokes:");
    scb_strokes_dump(&strokes);
    TRACE("Dump tmp: should be zero!");
    scb_strokes_dump(&tmp);
    TRACE("Free all!");
    scb_strokes_empty(&strokes);
    scb_strokes_dump(&strokes);


    scb_strokes_free(&tmp);
    
    scb_strokes_free(&strokes);
}


//========================================================================
// common memory data structure test
void testDataStruct()
{
    static const int PAGES      = 5;
    static const int STROKES    = 5;
    static const int POINTS     = 5;

    ScbDocPtr doc = scb_doc_new();
    ScbPagesPtr pages = scb_doc_get_pages(doc);

    // add pages
    ScbPageId id;
    ScbPagePtr page = NULL;
    int i , j , k;
    for(i = 1; i <= PAGES; ++i)     
    {
        scb_page_id_from_int(&id, i);
        page = scb_page_new();
        scb_page_set_id(page, &id);

        // add strokes
        for(k = 1; k <= STROKES; ++k)     
        {
            ScbStrokePtr stroke = scb_stroke_new();
            ScbPoint point;
            for(j = 1;j < POINTS; ++j)     
            {
                point.x = i; point.y = j; point.pressure = i * j;
                scb_stroke_add_point(stroke, &point);
            }
            scb_page_add_stroke(page, stroke);          
        }                
        scb_pages_add_page(pages, page);
    }
    //scb_pages_dump(pages);
    TRACE("add test done! Have a rest!");
    sleep(SLEEP);
    
    // remove pages
    for(i = 1; i <= PAGES / 2; ++i)     
    {
        scb_page_id_from_int(&id, i);
        TRACE("go to remove page %s!", id.id);
        scb_pages_remove_page(pages, &id);
    }
    //scb_pages_dump(pages);
    TRACE("remove test done! new count %d Have a rest!", scb_pages_get_count(pages));
    sleep(SLEEP);
    
    
    // detach
    ScbPages tmp;
    scb_pages_new(&tmp);
    for(i = PAGES/2  + 1; i <=PAGES; ++i)
    {
        scb_page_id_from_int(&id, i);
        scb_pages_add_page(&tmp, scb_pages_detach_page(pages, &id));
    }
    TRACE("dump pages");
    //scb_pages_dump(pages);
    sleep(SLEEP);
    
    TRACE("dump tmp");
    //scb_pages_dump(&tmp);
    sleep(SLEEP);

 
    scb_pages_free(&tmp);
    scb_pages_free(pages);
    scb_doc_free(doc);
}

//========================================================================
// compare paritally open/save with whole document operation
// time and memory usage

void openDirectly(const char *p)
{
    // open directly with p1
    ScbPath path;
    ScbPageId id;
    ScbPagePtr page;
    strncpy(path.scbname, p, MAX_PATH);
    strncpy(id.id, "10", MAX_PAGEID_LEN);
    
    // 
    timestart();
    //page = scb_doc_open_page_directly(&path, &id);
    timeend();

  

    TRACE("done!!!!");
}

void openAll(const char *p)
{
    ScbPath path;
    ScbPageId id;
    ScbPagePtr page;
    strncpy(path.scbname, p, MAX_PATH);
    strncpy(id.id, "10", MAX_PAGEID_LEN);

    // 
    
    timestart();
    ScbDocPtr doc = scb_doc_open(&path);
    ScbPagesPtr pages = scb_doc_get_pages(doc);
    page = scb_pages_get_page(pages, &id);
    timeend();
    

}

//========================================================================
// construct a doc
// add pages
// add strokes
// add point
// dump
// save
// remove 
void testDocSave()
{
    static const int PAGES      = 20;       // total pages
    static const int STROKES    = 100;      //  strokes per page
    static const int POINTS     = 100;      // points per stroke

    // doc
    TRACE("Start to create a new doc!");
    ScbDocPtr doc = scb_doc_new();
    if (NULL == doc)
    {
        TRACE("Could not construct a scribble document!");
        return;
    }
    TRACE("create document done!");

    // page list
    ScbPagesPtr pages = scb_doc_get_pages(doc);

    // add some pages
    ScbPageId id;
    ScbPagePtr page = NULL;
    int i , j , k;
    for(i = 1; i <= PAGES; ++i)     
    {
        scb_page_id_from_int(&id, i);
        page = scb_page_new();
        scb_page_set_id(page, &id);

        // add strokes
        for(k = 1; k <= STROKES; ++k)     
        {
            ScbStrokePtr stroke = scb_stroke_new();
            ScbPoint point;
            for(j = 1;j < POINTS; ++j)     
            {
                point.x = i; point.y = j; point.pressure = i * j;
                scb_stroke_add_point(stroke, &point);
            }
            scb_page_add_stroke(page, stroke);          
        }                
        scb_pages_add_page(pages, page);
    }

    TRACE("Added pages done! Dump Page now!");
    TRACE("pages %d", scb_pages_get_count(pages));
    TRACE("Visit page %s!", id.id);
    page = scb_pages_get_page(pages, &id);

    //   scb_pages_dump(pages);
    strcpy(doc->path.scbname, "/scb.xml");
    scb_doc_save(doc);
    TRACE("Save done");
    scb_doc_free(doc);
}

void testDocLoad(const char *pathName)
{
    ScbPath path; strcpy(path.scbname, pathName);
    ScbDocPtr doc = scb_doc_open(&path);

    if (NULL == doc)
    {
        TRACE("Could not open file %s", pathName);
        return;
    }

    //scb_doc_dump(doc);
    scb_doc_free(doc);
}

// test memory leak
void checkMemoryLeak()
{
    printf("checking memory...");
    int i = 0;
    for(; i < 100000; ++ i )
    {
        testDataStruct();
    }          
    printf("done, check memory!\n");
    sleep(20);
}

void testFastDraw()
{
    ScbPath path; 
    strncpy(path.scbname, "/temp.irx", MAX_PATH);
    ScbDocPtr doc = scb_doc_make_sure_exist(&path);
    
    ScbPageId id; 
    scb_page_id_from_str(&id, "abcdef");
    
    ScbPagePtr page = scb_doc_get_page(doc, &id);
    if (NULL == page)
    {
        page = scb_page_new();
        scb_page_set_id(page, &id);
        scb_doc_add_page(doc, page);
    }        

    
    ScbStrokePtr stroke = scb_stroke_new();
    
    stroke->style.color.pixel = 3;
    stroke->style.penSize = 3;
    static const int POINTS  = 1000;
    
    int i;
    ScbPoint point;
    for(i = 0; i < POINTS - 1; ++i)
    {
        point.x = i; point.y = i; point.pressure = i * i;
        scb_stroke_add_point(stroke, &point);
        scb_stroke_fast_draw_point(stroke, &point); 
    }
    point.x = i; point.y = i; point.pressure = i * i;
    scb_stroke_add_point(stroke, &point);
    scb_stroke_fast_draw_point_done(stroke, &point);
    scb_page_add_stroke(page, stroke);
    scb_doc_save(doc);
    scb_doc_free(doc);
}
/*
#define PIXELBUFSIZE       25

typedef struct _PointInfo
{
	unsigned short x;
	unsigned short y;
	unsigned char  size;
	unsigned char  color;
  	unsigned char  pen_down;
}PointInfo;

typedef struct _PointsBuf
{
  unsigned int count;
  PointInfo points [PIXELBUFSIZE];
} PointsBuf;

#define FBIO_IOCTL_BASE		  'v'
#define FBIO_DRAWPIXELS  _IOW(FBIO_IOCTL_BASE, 3, PointsBuf)

void testFBIO()
{
    int fbdev = 0;
    fbdev = open("/dev/fb0", O_RDWR);
    if (-1 == fbdev)
    {
        TRACE("Could not open /dev/fb0!");
        return;
    }
    
     PointsBuf buf;
     buf.count = 2;
     int i = 0;
     buf.points[i].x = 100;
     buf.points[i].y = 100;
     buf.points[i].size = 5;
     buf.points[i].color = 3;
     buf.points[i].pen_down = 1;
     ++i;
    
     buf.points[i].x = 700;
     buf.points[i].y = 800;
     buf.points[i].size = 5;
     buf.points[i].color = 3;
     buf.points[i].pen_down = 0;
   
     for(i = 0; i < buf.count; ++i)
     {
        printf("%d %d %d %d %d %d\n",i,  buf.points[i].x, buf.points[i].y, 
                buf.points[i].size ,buf.points[i].color,buf.points[i].pen_down);
     }    

    if (-1 == ioctl (fbdev, FBIO_DRAWPIXELS, &buf))
    {
        TRACE("ioctl error!");
    }
    
     buf.count = 2;
     i = 0;
     buf.points[i].x = 700;
     buf.points[i].y = 100;
     buf.points[i].size = 5;
     buf.points[i].color = 3;
     buf.points[i].pen_down = 1;
     ++i;
    
     buf.points[i].x = 100;
     buf.points[i].y = 800;
     buf.points[i].size = 5;
     buf.points[i].color = 3;
     buf.points[i].pen_down = 0;
   
    if (-1 == ioctl (fbdev, FBIO_DRAWPIXELS, &buf))
    {
        TRACE("ioctl error!");
    }
    
    
    close(fbdev);        
}
*/
void testdll()
{
 //   scb_doc_test();
    PointsBufPtr ptr = (PointsBufPtr)1;
    scb_driver_draw(ptr);
}


int main(int argc, char * argv[])
{
    // checkMemoryLeak();
    // testDoc();
    
    //timestart();
    //testDocSave();
    //timeend();
    // testDocLoad("/scb.xml");
    // testDataStruct();
    // testStrokes();
    
    /*
    if (argc >= 2)
    {
        if ('1' == *argv[1])
        {
            openDirectly("/scb.xml");
        }
        else if ('2' == *argv[1])
        {
            openAll("/scb.xml");
        }
    } 
    */       
    /*
    int i = 0;
    //for(; i < 1000; ++i)
    {
        testFastDraw();
    }      
    */
    // testFBIO  ();
    // testFastDraw();
    testdll();
    return 0;
}
