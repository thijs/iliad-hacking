/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file main.c
 * \brief lister - Content Lister application startup                  
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
extern "C" {

#include <config.h>

#include <gtk/gtk.h>

#include <libermanifest/ermanifest.h>
#include <liberregxml/erregapi.h>
#include <liberdm/display.h>

#include "contentListerLog.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "erbusy.h"
#include "languages.h"
#include "click.h"

#ifdef ENABLE_LOGGING
#include <liberlog/erlog.h>
#endif /* ENABLE_LOGGING */

}; // end of extern "C"

#include "Geometry.h"


extern "C" {
static startup_behaviour_t  g_startup_behaviour = behaviourUndefined_t;

static  struct
        {
            const char*                argv;
            const startup_behaviour_t  behaviour;
        } g_startup_locations[] =
            {
                { "LASTREADDOCUMENT", lastReadDocument_t   },
                { "RECENTDOCUMENTS",  recentDocuments_t    },
                { "MODE",             deviceManager_t      },
                { "NEWSPAPERS",       newsFolder_t         },
                { "BOOKS",            booksFolder_t        },
                { "DOCUMENTS",        docsFolder_t         },
                { "NOTES",            notesFolder_t        },
                { "DOWNLOADHISTORY",  downloadHistory_t    },
                { NULL,               behaviourUndefined_t }
            };


// read registry data into memory
static void load_registry(void)
{
    static gboolean firsttime = TRUE;

    gboolean       b;
    regLoad_t      regLoad;
    
    // init registry locking
    b = erRegRWLockInit();
    if (b == FALSE)
    {
        CL_ERRORPRINTF("erRegRWLockInit fails with return code [%d]", b);
        g_assert_not_reached();
    }

    if (firsttime)
    {
        // acquire write lock to force recovery, if needed
        b = erRegWriteLock();
        if (b == FALSE)
        {
            CL_ERRORPRINTF("erRegWriteLock fails with return code [%d]", b);
            g_assert_not_reached();
        }

        // load network profiles, so these will be recovered if needed
        regLoad = erRegLoad(regNWProfiles_t);
        if (regLoad != loadOk_t)
        {
            CL_WARNPRINTF("erRegLoad(regNWProfiles_t) returns [%d]", regLoad);
            if (regLoad == loadError_t)
            {
                CL_ERRORPRINTF("erRegLoad(regNWProfiles_t) fails with return code [%d]", regLoad);
                g_assert_not_reached();
            }
        }
        // release network profiles, contentLister does not use them
        erRegUnload(regNWProfiles_t);
    }
    else
    {
        // acquire read-only lock
        b = erRegReadLock();
        if (b == FALSE)
        {
            CL_ERRORPRINTF("erRegReadLock fails with return code [%d]", b);
            g_assert_not_reached();
        }
    }
    
    // load registry sections used
    regLoad = erRegLoad(regBasis_t);
    if (regLoad == loadError_t)
    {
        CL_ERRORPRINTF("erRegLoad(regBasis_t) fails with return code [%d]", regLoad);
        g_assert_not_reached();
    }
    
    // release lock, all data now in memory
    erRegUnlock();

    firsttime = FALSE;
}

// release registry data from memory
static void release_registry(void)
{
    // unload registry sections
    erRegUnload(regBasis_t);

    // discard registry locking
    erRegRWLockDestroy();
}

// release in-memory data + read fresh data from registry
void reload_registry(void)
{
    gboolean       b;
    regLoad_t      regLoad;
    
    // acquire read lock
    b = erRegReadLock();
    if (b == FALSE)
    {
        CL_ERRORPRINTF("erRegReadLock fails with return code [%d]", b);
        g_assert_not_reached();
    }
    
    // reload registry sections used
    erRegUnload(regBasis_t);
    regLoad = erRegLoad(regBasis_t);
    if (regLoad == loadError_t)
    {
        CL_ERRORPRINTF("erRegLoad(regBasis_t) fails with return code [%d]", regLoad);
        g_assert_not_reached();
    }

    // release lock, all data now in memory
    erRegUnlock();
}

// prepare registry for writing: lock + reload
void prepare_registry_write( void)
{
    gboolean       b;
    regLoad_t      regLoad;
    
    // acquire write lock
    b = erRegWriteLock();
    if (b == FALSE)
    {
        CL_ERRORPRINTF("erRegWriteLock fails with return code [%d]", b);
        g_assert_not_reached();
    }
    
    // reload section(s) that may be written
    erRegUnload(regBasis_t);
    regLoad = erRegLoad(regBasis_t);
    if (regLoad == loadError_t)
    {
        CL_ERRORPRINTF("erRegLoad(regBasis_t) fails with return code [%d]", regLoad);
        g_assert_not_reached();
    }
}

// report startup location
startup_behaviour_t main_get_startup_behaviour(void) {
    return g_startup_behaviour;
}

// write modified registry: write + unlock
void do_registry_write(void) {
    gboolean       b;

    // verify write lock
    g_assert(lock_write == erRegGetLockState());
    // save registry
    b = erRegStore();
    if (b == FALSE) {
        CL_ERRORPRINTF("erRegStore fails with return code [%d]", b);
    }
    // release lock
    erRegUnlock();
}


/*
* Terminate the main loop.
*/
static void on_destroy(GtkWidget * widget, gpointer data) {
    ContentLister *theContentLister = (ContentLister *) data;
    CL_WARNPRINTF("entry");
    ctrlDestroy(theContentLister);
    gtk_main_quit();
}

}; // end of extern "C"

// print usage text and quit
static void usage(const char *argv_0)
{
    int i;

    static const char *usage_text = 
                        "\n"
                        "usage: %s [options]\n"
                        "\n"
                        "options:\n"
                        "    --help\n"
                        "        Print help text and quit\n"
                        "    --items <4-20>\n"
                        "        Set number of items on one page\n"
                        "    --location <location>\n"
                        "        Start contentLister in <location>\n"
                        "\n"
                        "<location>:\n";

    printf(usage_text, argv_0);

    for (i = 0 ; g_startup_locations[i].argv ; i++)
    {
        printf("    %s\n", g_startup_locations[i].argv);
    }
    printf("\n");

    exit(1);
}

// get command-line options
static void parse_arguments(int argc, char **argv)
{
    int  i;
    int  j;
    gboolean found = FALSE;

    { 
      // try to read nr of items on a page from /mnt/settings/newLister
      // For now: just read one line, and try to make a number from it. 
      FILE *f;
      if ((f=fopen("/mnt/settings/newLister", "r"))) {
         int items;
         if (fscanf(f, "%d", &items) == 1) {
           if (items>=4 && items <=20) {
             _G.pageItems = items;
           }
         }
         fclose(f);
      }
      else {
         // try to create the file if it does not exist
         if ((f=fopen("/mnt/settings/newLister", "w"))) {
           fprintf(f,"%d\n", _G.pageItems);
           fclose(f);
         }
      }
    }

    // parse contentLister options
    for (i = 1 ; i < argc ; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
        }
        else if(strcmp(argv[i], "--items") == 0) {
            i++;
            if (i >= argc) {
                usage(argv[0]);
            }
            else {
                int items = 6;
                items = atoi(argv[i]);
                if (items <4)  items =  4;
                if (items >20) items = 20;
                _G.pageItems = items;
                i++;
            }
        }
        else if(strcmp(argv[i], "--location") == 0) {
            i++;
            if (i >= argc) {
                usage(argv[0]);
            }
            else {
                // get startup location (behaviour)
                for (j = 0 ; g_startup_locations[j].argv ; j++) {
                    if (strcmp(argv[i],  g_startup_locations[j].argv) == 0) {
                        g_startup_behaviour = g_startup_locations[j].behaviour;
                        found = TRUE;
                    }
                }
                if (!found) {
                    // invalid location specified
                    usage(argv[0]);
                }
                i++;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    ContentLister *theContentLister;

    parse_arguments(argc, argv);

    /* init threads */
    g_thread_init(NULL);
    gdk_threads_init();
    gdk_threads_enter();

    // open the RC file associate with this program
    gtk_rc_parse(DATA_DIR "/newLister.rc");
    CL_LOGPRINTF("rc file %s", DATA_DIR "/newLister.rc");

    gtk_init(&argc, &argv);

    gchar** files = gtk_rc_get_default_files();
    while( *files ) {
        CL_WARNPRINTF("gtk_rc_get_default_files [%s]", *files);
        files++;
    }

    
#ifdef ENABLE_LOGGING
    erlog_write_start();
    erlog_event_system(power_on);
#endif /* ENABLE_LOGGING */

//#ifdef COMMAND_LINE_INPUT
//    erRegInitSystemAccess();
//#endif /* COMMAND_LINE_INPUT */

    load_registry();

    languagesInit();
    click_init();

    // create the main, top level, window 
    
    _G.recalc();	// calc gui geometry settings

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), PACKAGE " " VERSION);
    gtk_window_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    gtk_widget_set_size_request(GTK_WIDGET(window), _G.top.w, _G.top.h);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    theContentLister = ctrlInit(window);

    if (theContentLister)
    {
        CL_LOGPRINTF("Inside if (theContentLister)");

        // Connect the destroy event of the window with our on_destroy function
        // When the window is about to be destroyed we get a notificaton and
        // stop the main GTK loop
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_destroy), theContentLister);

        // make sure that everything, window and label, are visible 
        gtk_widget_show(window);
    }

    // start the main loop //
    CL_LOGPRINTF("Before gtk_main");

    gtk_main();
    gdk_threads_leave();

    release_registry();

    return 0;
}


