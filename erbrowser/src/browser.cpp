/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file browser.cpp
 * \brief browser - eReader "GtkMozEmbed" widget creation, signal hanlders initialistation
 *                  and browser related API's                 
 * 
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#include <liberdm/display.h>
#include "browserTypes.h"
#include "pagebar.h"
#include "indexFileHandler.h"
#include "main.h"
#include "browser.h"
#include "toolbar.h"
#include "displayUpdate.h"
#include "browserLog.h"
#include <libermanifest/ermanifest.h>
#include "mozilla_api.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

// PR_GetEnv call
#include "prenv.h"
#include "mozEmbedCallbacks.h"



//------------------------------------------------------------------------
// To Record last visited page
class CPersist
{
private:
    static Ereader *s_pBrowser;
    static const int MAX_PATH = 1024;
    static char s_url[MAX_PATH];
    static char s_inputUrl[MAX_PATH]; 
    static char s_ManifestPathName[MAX_PATH];
    static void OnTerminated(int sig);
    static void PreparePath();
   
public:
    CPersist() {}
    ~CPersist() {}
public:     
    static void  SetBrowser(Ereader *p) { s_pBrowser = p; }
    static Ereader *GetBrowser() { return s_pBrowser; }
    static char* GetPageLastRead(const char * input);
    static void  StorePageLastRead(const char *url);
    static void  SetManifestPathName(const char *pathName);
    static void  InstallTermHandler();
    static void  UninstallTermHandler();
    static gboolean FileExist(const char *p);
};

Ereader * CPersist::s_pBrowser = NULL;
char CPersist::s_ManifestPathName[];
char CPersist::s_url[];
char CPersist::s_inputUrl[];

void CPersist::SetManifestPathName(const char *pathName)
{
    if (pathName)
    {
        strncpy(s_ManifestPathName, pathName, MAX_PATH);
    }        
}

void CPersist::InstallTermHandler()
{
    // catch the SIGTERM signal
    struct sigaction on_term;
    memset(&on_term, 0x00, sizeof(on_term));
    on_term.sa_handler = OnTerminated;
    sigaction(SIGTERM, &on_term, NULL);
}

void CPersist::UninstallTermHandler()
{
    // uninstall the SIGTERM signal
    struct sigaction on_term;
    memset(&on_term, 0x00, sizeof(on_term));
    on_term.sa_handler = SIG_DFL;
    sigaction(SIGTERM, &on_term, NULL);
}

void CPersist::OnTerminated(int sig)
{
    // save into manifest file
    destroy_ereader(s_pBrowser);
    s_pBrowser = NULL;
    exit(0);
}

gboolean CPersist::FileExist(const char *pathName)
{
    struct stat statbuf;
    if (0 != stat(pathName, &statbuf))
    {
        return FALSE;
    }
    return TRUE;
}

void CPersist::StorePageLastRead(const char *url)
{
    // get rid of the whole prefix, only use relative path
    // find common sub string in this two strings 
    // and reverse find the /
    BR_LOGPRINTF("Input url %s\n", url);
    char * path = mozilla_uri_to_path(url);
    char * mem  = path;
    if (path == NULL)
    {
        BR_LOGPRINTF("No local path %s", url);
        // Use inputUrl as last known location, instead of
        // an invalid one
        path = strdup(s_inputUrl);
    }
    else
    {
        BR_LOGPRINTF("Output path %s\n", path);
        char * tmp = &s_ManifestPathName[0];
        while (path && tmp && *path == *tmp)
        {
            ++path; ++tmp;
        }
        if (path)
        { 
            while (*path != '/')
            {
                --path;
            }
            path++;
        }
    }
    BR_LOGPRINTF("Last location in relative path %s\n", path);
    erManifest manifest;
    if (RET_OK == ermXmlOpenFile(s_ManifestPathName, &manifest))
    {
        // usually the manifest file is existing
        if (RET_OK != ermXmlExist(&manifest, "/package/last-location/document"))
        {
            ermXmlNewString(&manifest, "/package", "last-location", "");
            ermXmlNewString(&manifest, "/package/last-location", "document", "");
        }
        ermXmlSetString(&manifest, "/package/last-location/document", path);
        ermXmlSaveAndClose(&manifest);
    }
    if (mem)
    {
        free(mem);
    }
}

// in order to convert from relative path to absolut path, we need to
// use input url. 
// TODO: fix the pointer-mess that this function makes
// mozilla_uri_to_path() now no longer illegally changes the url parameter
// so the assignments in this function that rely on this must be changed
// Why is s_url a static when it is only used in this function?
char * CPersist::GetPageLastRead(const char * input)
{
    BR_LOGPRINTF("input %s", input);

    // save input url first
    strncpy(s_inputUrl, input, MAX_PATH);

    // get url last visisted from manifest file
    char * url = NULL;
    erManifest manifest;
    memset(s_url, 0, MAX_PATH);
    if (RET_OK == ermXmlOpenFile(s_ManifestPathName, &manifest))
    {
        // usually the manifest file is existing
        if (RET_OK == ermXmlGetString(&manifest, "/package/last-location/document", s_url, MAX_PATH))
        {
            url = &s_url[0];
        }
        ermXmlClose(&manifest);
    }
    
    // check url is valid or not
    BR_LOGPRINTF("url from manifest %s", url);
    if (NULL == url) 
    {
        BR_LOGPRINTF("Could not retrieve url from manifest file %s!", s_ManifestPathName);
        return (char *)input;
    }
    
    // convert to path and check file exist, for those manifest using absolute path
    int prefix = strlen("file://");
    char tmp[MAX_PATH]  = {0};
    strcpy(tmp, "file://");
    char *path = mozilla_uri_to_path(url);
    if (path && FileExist(path))
    {
        // absolute path
        strncpy(&s_url[0], path, MAX_PATH);
        free(path);
        return &s_url[0];
    }
    else
    {
        // release 
        if (path) 
        {
            free(path);
        }
    
        // relative path, change to absolute path
        strcpy(tmp + prefix, s_ManifestPathName);
        char * pos = strrchr(tmp + prefix, '/');
        ++pos;
        strcpy(pos, url);
        pos += strlen(url); *pos = 0;
        strncpy(s_url, tmp , strlen(tmp));
    }
    
    // check again now
    BR_LOGPRINTF("check again the input %s", s_url);
    if (FileExist(s_url + prefix))
    {
        BR_LOGPRINTF("url result %s", s_url);
        return s_url;
    }
    return NULL;
}

static CPersist s_recorder;
//------------------------------------------------------------------------

// callbacks
gboolean delete_cb(GtkWidget * widget, GdkEventAny * event, Ereader * reader);
gboolean browser_main_window_expose_event(GtkWidget * widget, GdkEventExpose * event, Ereader * reader);

//internally used routines
static void browser_create(Ereader * browser, gchar * URL_input);
static GtkWidget *create_window(char *title);
static void maximize_window(GtkWidget * src);

Ereader *new_gtk_ereader(char *manifest, char *input_URL)
{
    Ereader *theEreader;

    BR_LOGPRINTF("entry");

    // create one Ereader ereader struct and fill it with zeros
    theEreader = g_new0(Ereader, 1);

    if (theEreader)
    {
        theEreader->pageStatus = browser_create_page_status();
        browser_set_page_status_indexfile(theEreader->pageStatus, manifest);

        BR_LOGPRINTF("theEreader 0x%x - theEreader->pageStatus 0x%x", theEreader, theEreader->pageStatus);

        theEreader->topLevelWindow = create_window("E-reader");

        // new vbox - homogenous - zero spacing
        theEreader->topLevelVBox = gtk_vbox_new(FALSE, 0);
        gtk_widget_show(theEreader->topLevelVBox);
        gtk_container_add(GTK_CONTAINER(theEreader->topLevelWindow), theEreader->topLevelVBox);

        // event box addded to have an actual X-window     
        theEreader->eventBox = gtk_event_box_new();
        gtk_box_pack_start(GTK_BOX(theEreader->topLevelVBox), theEreader->eventBox, TRUE, TRUE, 0);
        gtk_widget_show(theEreader->eventBox);

        g_signal_connect(G_OBJECT(theEreader->topLevelVBox), "expose-event", G_CALLBACK(browser_main_window_expose_event), theEreader);


#ifdef LOCAL_DISPLAY
        // create toolbar empty area 
        BR_WARNPRINTF("LOCAL_DISPLAY is defined => toolbar");

        theEreader->toolbar = gtk_toolbar_new();
        gtk_widget_set_usize(GTK_WIDGET(theEreader->toolbar), (-1), TOOLBAR_HEIGTH);
        gtk_widget_show(theEreader->toolbar);

        gtk_box_pack_start(GTK_BOX(theEreader->topLevelVBox), GTK_WIDGET(theEreader->toolbar), FALSE, // expand
                           FALSE, // fill
                           0);  // padding
#endif //LOCAL_DISPLAY

        gtk_widget_show(theEreader->topLevelWindow);

        // check manifest file
        s_recorder.SetManifestPathName(manifest);
        
        char *url = s_recorder.GetPageLastRead(input_URL);
        if (url)
        {
            input_URL = url;
            // input_URL = "file:///./pagl01.html";
        }
                
        // create the embedded browser
        browser_create(theEreader, input_URL);

        // catch the destruction of the toplevel window
        g_signal_connect(GTK_OBJECT(theEreader->topLevelWindow), "delete_event", G_CALLBACK(delete_cb), theEreader);

        // catch the key-events in the toplevel window
        gtk_signal_connect(GTK_OBJECT(theEreader->topLevelWindow), "key_press_event", G_CALLBACK(on_mainWindow_keypress), theEreader);
        
        // init 
        s_recorder.InstallTermHandler();
        s_recorder.SetBrowser(theEreader);
    }
    else
    {
        BR_ERRORPRINTF("unable to allocate gEreader");
    }

    return theEreader;
}

void browser_quit()
{
    s_recorder.UninstallTermHandler();
    destroy_ereader(s_recorder.GetBrowser());
}

void destroy_ereader(Ereader * theEreader)
{
    // store url lastest visited
    gchar  *    type = NULL;
    gboolean    page_type_known = FALSE;
    int         page = -1;
		
	// current url 
    gchar  *url = gtk_moz_embed_get_location(GTK_MOZ_EMBED(theEreader->mozEmbed));
       
    // get type so that we can get page number
    if (page_type_known = mozilla_get_page_type(GTK_MOZ_EMBED(theEreader->mozEmbed), &type))
    {
        page = index_file_get_page_number(&theEreader->pageStatus->index, type, url);
    }
    BR_LOGPRINTF("url last visited %s %d page\n", url, page);
    s_recorder.StorePageLastRead(url);
    if (url) 
    {
        free(url);
    }
    
    if (theEreader)
    {
        BR_LOGPRINTF("cleaning up browser allocations");

        if (theEreader->pageStatus)
        {
            browser_destroy_page_status(theEreader->pageStatus);
        }

        g_free(theEreader);
        theEreader = NULL;
    }
}


gboolean delete_cb(GtkWidget * widget, GdkEventAny * event, Ereader * reader)
{
    BR_LOGPRINTF("entry");
    destroy_ereader(reader);
    gtk_widget_destroy(widget);
    return TRUE;
}


/**
 * create a "GtkMozEmbed" widget, specify the callbacks of the GtkMozEmbed specific signals
 * and load the first XHTML file 
 *
 * @param URL location of the first file that needs to be displayed
 *
 * @returns void
 */
static void browser_create(Ereader * browser, gchar * URL_input)
{
    browser->mozEmbed = gtk_moz_embed_new();


    
    // add this to the browser window
    gtk_container_add(GTK_CONTAINER(browser->eventBox), browser->mozEmbed);

    BR_LOGPRINTF("browser 0x%x - created mozEmbed 0x%x", browser, browser->mozEmbed);

    g_signal_connect(GTK_OBJECT(browser->mozEmbed), "location", G_CALLBACK(location_changed_cb), browser);
    g_signal_connect(GTK_OBJECT(browser->mozEmbed), "visibility", G_CALLBACK(visibility_cb), browser);
    g_signal_connect(GTK_OBJECT(browser->mozEmbed), "net_stop", G_CALLBACK(load_finished_cb), browser);
    //g_signal_connect(GTK_OBJECT(browser->mozEmbed), "progress", G_CALLBACK(progress_change_cb), browser);
    //g_signal_connect(GTK_OBJECT(browser->mozEmbed), "progress_all", G_CALLBACK(progress_all), browser);
    //g_signal_connect(GTK_OBJECT(browser->mozEmbed), "status_change", G_CALLBACK(status_change_cb), browser);
    //g_signal_connect(GTK_OBJECT(browser->mozEmbed), "net_state", G_CALLBACK(net_state_change_cb), browser);
    g_signal_connect(GTK_OBJECT(browser->mozEmbed), "open_uri", G_CALLBACK(open_uri_cb), browser);

#ifndef PC_ENVIRONMENT
    // set the chrome type so it's stored in the object
    // commented for scrollbars fixing
    gtk_moz_embed_set_chrome_mask(GTK_MOZ_EMBED(browser->mozEmbed), GTK_MOZ_EMBED_FLAG_DEFAULTCHROME);
#endif
    // set our minimum size 
    gtk_widget_set_size_request(browser->mozEmbed, SCREEN_WIDTH, CLIENT_AREA);

    if (URL_input)
    {
        BR_LOGPRINTF("loading URL %s", URL_input);

        // => only request a refresh when the page is loaded
        display_update_increase_level(MOZEMBED_UPDATE_LEVEL);

        gtk_moz_embed_load_url(GTK_MOZ_EMBED(browser->mozEmbed), URL_input);
    }
    else
    {
        BR_ERRORPRINTF("No default URL available");
    }

    BR_LOGPRINTF("show mozEmbed 0x%x", browser->mozEmbed);
    set_browser_visibility(browser, TRUE);
}

void set_browser_visibility(Ereader * browser, gboolean visibility)
{
    BR_LOGPRINTF("%s", (visibility == TRUE) ? "TRUE" : "FALSE");

    if (!visibility)
    {
        gtk_widget_hide(browser->mozEmbed);
    }
    else
    {
        gtk_widget_show(browser->mozEmbed);
        gtk_widget_grab_focus(browser->mozEmbed);
    }
}

static void maximize_window(GtkWidget * src)
{
#ifndef PC_ENVIRONMENT
#ifndef LOCAL_DISPLAY
//      GdkBitmap       *empty_bitmap;
//      GdkCursor *cursor;
//      GdkColor useless;
//      char invisible_cursor_bits[] = {0x0};

    /* set transparent cursor */
//      useless.red = 0;
//      useless.green = 0;
//      useless.blue = 0;
//      useless.pixel = 0;
//      empty_bitmap = gdk_bitmap_create_from_data(src->window,invisible_cursor_bits,1,1);
//      cursor = gdk_cursor_new_from_pixmap(empty_bitmap,empty_bitmap,&useless,&useless,0,0);
//      gdk_window_set_cursor(src->window,cursor);

    /* no borders, buttons, ... */
//      gdk_window_set_decorations(src->window,GdkWMDecoration(0));
#endif //LOCAL_DISPLAY
#endif //PC_ENVIRONMENT
}


static GtkWidget *create_window(char *title)
{
    GtkWidget *window;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), (title));
    gtk_window_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
	gtk_widget_set_size_request(GTK_WIDGET(window), SCREEN_WIDTH, SCREEN_HEIGHT - TOOLBAR_HEIGTH - PAGEBAR_HEIGHT);
    gtk_window_set_modal(GTK_WINDOW(window), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_signal_connect(GTK_OBJECT(window), "realize", GTK_SIGNAL_FUNC(maximize_window), NULL);
    return window;
}

gboolean browser_main_window_expose_event(GtkWidget * widget, GdkEventExpose * event, Ereader * theEreader)
{
    BR_DISPLAYPRINTF("entry");
    display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL, NULL);
    return FALSE;
}
