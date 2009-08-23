/*
 * This file is part of libermanifest.
 *
 * libermanifest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libermanifest is distributed in the hope that it will be useful,
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


#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <ermanifest.h>

#define MAX_LEN         1024
#define STROKE_LEN      2000

void dumparg(int argc, char *argv[])
{
    int i = 0; 
    fprintf(stderr, "argc %d\n", argc);
    for(i = 0; i < argc; ++i)
        fprintf(stderr, "argc %d: %s\n", i, argv[i]);
}

void usage()
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  <inputfilename> <outputfilename>\n");
}

void testOpen(const char *pathname)
{
    erManifest manifest;

    ermXmlOpenFile(pathname, &manifest);

    char data[MAX_LEN] = {0};
    
    // test memory leak
    fprintf(stderr, "test memory leak\n");
    int i = 0;
    for( ; i < 10000000; ++i)
    {
        ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Title", data, MAX_LEN); 
    }
    fprintf(stderr, "\n\ntest done!!!!!!!\n\n");
    sleep(100);        
    fprintf(stderr, "result: %s\n", data);
    
    ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Description", data, MAX_LEN); 
    fprintf(stderr, "result: %s\n", data);


    ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Date", data, MAX_LEN); 
    fprintf(stderr, "result: %s\n", data);

    ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Format", data, MAX_LEN); 
    fprintf(stderr, "result: %s\n", data);

    ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Identifier", data, MAX_LEN); 
    fprintf(stderr, "result: %s\n", data);
    
    ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Language", data, MAX_LEN); 
    fprintf(stderr, "result: %s\n", data);

    ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Type", data, MAX_LEN); 
    fprintf(stderr, "result: %s\n", data);

    ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Type", data, MAX_LEN); 
    fprintf(stderr, "result: %s\n", data);


    ermXmlGetString(&manifest, "/package/metadata/y-metadata/FileSize", data, MAX_LEN); 
    fprintf(stderr, "result: %s\n", data);

    int itemSize = -1;
    ermXmlGetInt(&manifest, "/package/metadata/y-metadata/ItemSize", &itemSize); 
    fprintf(stderr, "result: %d\n", itemSize);

    ermXmlClose(&manifest);
}

// read write
void testWrite(const char *pathname)
{
    erManifest manifest;
    
    ermXmlOpenFile(pathname, &manifest);

    char data[MAX_LEN] = {0};

    char *des = "This is a demo test!这是一个中文测试";
    if (RET_OK == ermXmlSetString(&manifest, "/package/metadata/dc-metadata/Description", des))
        printf("set string ok!\n");
    else
        printf("set string failed!\n");

    ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Description", data, MAX_LEN); 
    fprintf(stderr, "result: %s\n", data);
    
    int itemSize = 4567890;
    ermXmlSetInt(&manifest, "/package/metadata/y-metadata/ItemSize", itemSize);
    
    ermXmlNewString(&manifest, "/package/metadata/y-metadata", "a", "content of a");
    ermXmlNewString(&manifest, "/package/metadata/y-metadata/a", "b", "content of b");
    ermXmlNewString(&manifest, "/package/metadata/y-metadata/a/b", "c", "content of c");        
    ermXmlNewString(&manifest, "/package/metadata/y-metadata/a/b/c", "d", "content of d");
    ermXmlNewInt(&manifest, "/package/metadata/y-metadata/a/b/c/d", "e", 987654);    

    
    /* test attributes */            
    /* "/package/index/pagelist[@type='paper']" */
    ermXmlGetAttributeString(&manifest, "/package/index/pagelist[1]/page", "url", data, MAX_LEN);
    fprintf(stderr, "result: %s\n", data);
    ermXmlGetAttributeString(&manifest, "/package/index/pagelist[2]/page", "url", data, MAX_LEN);
    fprintf(stderr, "result: %s\n", data);
    ermXmlGetAttributeString(&manifest, "/package/index/pagelist[3]/page", "url", data, MAX_LEN);        
    fprintf(stderr, "result: %s\n", data);
    
    /* try to change */    
    char *attr = "this is attribute test这是中文属性测试";
    strncpy(&data[0], attr, strlen(attr));
    ermXmlSetAttributeString(&manifest, "/package/index/pagelist[1]/page", "url", data, MAX_LEN);

    /* add a new attribute */    
    ermXmlSetAttributeString(&manifest, "/package/index/pagelist[1]/page", "abc", data, MAX_LEN);            
    
    ermXmlSaveAndClose(&manifest);
}

// create a new xml file
void testCreate(const char *pathname)
{
    erManifest manifest;
    
    char data[MAX_LEN] = {0};
    /* create a new file, it will overwrite existing file */ 
    
    ermXmlCreateFile(pathname, &manifest);
    printf("create done!\n");
        
    memset(&data[0], 0, MAX_LEN);
    strncpy(data, "abcde", 5);
    ermXmlNewString(&manifest, "/", "packet", data);
    printf("newstring done!\n");
    ermXmlNewString(&manifest, "/packet", "metadata", data);
    
    ermXmlNewString(&manifest, "/packet/metadata", "y-metadata", data);
    
    ermXmlNewString(&manifest, "/packet/metadata/y-metadata", "pages", "");
    ermXmlNewString(&manifest, "/packet/metadata/y-metadata/pages", "page", "");
    ermXmlNewString(&manifest, "/packet/metadata/y-metadata/pages/page[1]", "strokes", "");    
    ermXmlNewString(&manifest, "/packet/metadata/y-metadata/pages/page[1]/strokes", "stroke", "");    

    ermXmlNewString(&manifest, "/packet/metadata/y-metadata/pages", "page", "");
    ermXmlNewString(&manifest, "/packet/metadata/y-metadata/pages/page[2]", "strokes", "");    
    ermXmlNewString(&manifest, "/packet/metadata/y-metadata/pages/page[2]/strokes", "stroke", "");    

    
    printf("save done!\n");
    ermXmlSaveAndClose(&manifest);
}

// create a big new xml file
void testHugeCreate(const char *pathname)
{
    erManifest manifest;
    
    char data[MAX_LEN] = {0};
    /* create a new file, it will overwrite existing file */ 
    
    ermXmlCreateFile(pathname, &manifest);
    printf("create done!\n");
        
    memset(&data[0], 0, MAX_LEN);
    strncpy(data, "abcde", 5);
    ermXmlNewString(&manifest, "/", "packet", data);
    printf("newstring done!\n");
    ermXmlNewString(&manifest, "/packet", "metadata", data);
    
    ermXmlNewString(&manifest, "/packet/metadata", "y-metadata", data);
    ermXmlNewString(&manifest, "/packet/metadata/y-metadata", "pages", "");
    
    int pages = 4; // 100 pages
    int i = 1;
    char xPath[MAX_LEN] = {0};
    char stroke[STROKE_LEN] = {0};
    for(;i < pages; ++i)
    {
        
        ermXmlNewString(&manifest, "/packet/metadata/y-metadata/pages", "page", "");
        memset(xPath, 0, MAX_LEN);
        snprintf(xPath, MAX_LEN, "/packet/metadata/y-metadata/pages/page[%d]", i);
        ermXmlNewString(&manifest, xPath, "strokes", "");    
        
        int strokes = 10;
        int j = 1;
        strcat(xPath, "/strokes");
        
        memset(stroke, 'a', STROKE_LEN - 10);
        for(;j < strokes; ++j)
        {
            ermXmlNewString(&manifest, xPath, "stroke", stroke);    
        }
    }
    
    i = 1;
    snprintf(xPath, MAX_LEN, "/packet/metadata/y-metadata/pages/page[%d]", i);
    ermXmlSetString(&manifest, xPath, "");
    
    xmlDocDump(stderr, manifest.pDoc);
    
    printf("save done!\n");
    ermXmlSaveAndClose(&manifest);
}


void testBrowserIndexFile(const char *pathName)
{
    // time stamp
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    
    erManifest manifest;
    
    char url[MAX_LEN]    = {0};
    char number[MAX_LEN] = {0};
    char type[MAX_LEN]   = {0};
    char xpath[MAX_LEN]  = {0};
    ermXmlOpenFile(pathName, &manifest);
    
    printf("open done!\n");
    int i = 1, j = 1;
    int ret = RET_OK;
    while (1)
    {
        sprintf(xpath, "/package/index/pagelist[%d]", i);
        ret = ermXmlGetAttributeString(&manifest, xpath, "type", type, MAX_LEN);
        ret = ermXmlGetAttributeString(&manifest, xpath, "count", number, MAX_LEN);
        if (RET_ERR == ret) break;
        printf("type:\t%s\tcount:\t%s\n", type, number);
        
        j = 1;
        while(1)
        {
            sprintf(xpath, "/package/index/pagelist[%d]/page[%d]", i, j);
         
            memset(url, 0, MAX_LEN); memset(number, 0, MAX_LEN);
            ermXmlGetAttributeString(&manifest, xpath, "url", url, MAX_LEN);
            ret = ermXmlGetAttributeString(&manifest, xpath, "number", number, MAX_LEN);                
            
            if (RET_ERR == ret) break;
            printf("url:\t%s\t", url);
            printf("number:\t%s\n", number);
            ++j;
        }
        
        ++i;
    }
    ermXmlClose(&manifest);
    
    gettimeofday(&t2, NULL);
    printf("time used\tsec %ld usec %ld\n", t2.tv_sec -t1.tv_sec,     t2.tv_usec - t1.tv_usec);
}

void testDel(const char *pathName)
{
    erManifest manifest;
    
  
    ermXmlOpenFile(pathName, &manifest);
    
    printf("Open done!\n");

    int i = 0;
    for(; i < 10; ++i)
    {
        ermXmlRemoveNode(&manifest, "/packet/metadata/y-metadata/pages/page[1]");
    }
    ermXmlSaveAndClose(&manifest);        
        
}

void testSpeXpath(const char *pathname)
{
    erManifest manifest;

    if (RET_OK !=ermXmlOpenFile(pathname, &manifest))
    {
        fprintf(stderr, "could not open file!\n");
        return;
    }

    char xPath[1024] = {0};
    char data[MAX_LEN] = {0};
    snprintf(xPath, 1024, "//page[@id=\"%s\"]", "1");
    fprintf(stderr, "%s\n", xPath);
    if (RET_ERR == ermXmlExist(&manifest, xPath))
    {
        fprintf(stderr, "error!\n");
        ermXmlClose(&manifest);
        return;
    }
    
    strncat(xPath, "/strokes/stroke", 1024);
    ermXmlGetString(&manifest, xPath, data, MAX_LEN); 
    fprintf(stderr, "result: %s\n", data);

    ermXmlClose(&manifest);
}

// generate a sample manifest 
void testSample(const char *pathname)
{
    erManifest manifest;
    
    char data[MAX_LEN] = {0};
    /* create a new file, it will overwrite existing file */ 
    
    ermXmlCreateFile(pathname, &manifest);
    printf("create done!\n");
        
    memset(&data[0], 0, MAX_LEN);
    ermXmlNewString(&manifest, "/", "packet", "");
    printf("newstring done!\n");
    ermXmlNewString(&manifest, "/packet", "metadata", "");
    sleep(1);
    printf("1 done!\n");
    
    ermXmlNewString(&manifest, "/packet/metadata", "y-metadata", "");
  
    ermXmlNewString(&manifest, "/packet/metadata/y-metadata", "pages", "");

    ermXmlNewString(&manifest, "/packet/metadata", "index", "");    
    
    ermXmlNewString(&manifest, "/packet/metadata/index", "pagelist", "");
 
    ermXmlSetAttributeString(&manifest, "/packet/metadata/index/pagelist", "type", "irfanview", 9); 
    ermXmlSetAttributeInt(&manifest, "/packet/metadata/index/pagelist", "count", 689); 
        
    printf("going to create page list!\n");    
    int pages = 689; 
    int i = 1;
    char xPath[MAX_LEN] = {0};
    char tmp[MAX_LEN] = {0};
    for(i = 1;i <= pages; ++i)
    {
        ermXmlNewString(&manifest, "/packet/metadata/index/pagelist", "page", "");
        memset(xPath, 0, MAX_LEN);
        snprintf(xPath, MAX_LEN, "/packet/metadata/index/pagelist/page[%d]", i);
        snprintf(tmp, MAX_LEN, "%03d", i);
        ermXmlSetAttributeString(&manifest, xPath, "number", tmp, strlen(tmp));    
        ermXmlSetAttributeString(&manifest, xPath, "url", "pagl04.html",11);    
    }
    ermXmlSaveAndClose(&manifest);
}


/* test ermanifest */
int main(int argc, char *argv[])
{
    dumparg(argc, argv);
    /*
    if (argc != 3)
    {
        usage();
        return 1;
    }
    */
    
    // testOpen(argv[1]);
    // testCreate(argv[1]);
    // testBrowserIndexFile(argv[1]);
    // testHugeCreate(argv[1]);
    // testDel(argv[1]);
    
    
    // testSpeXpath(argv[1]);
    testSample(argv[1]);
    return 1;    
}
