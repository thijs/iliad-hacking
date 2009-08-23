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
 * \file main.cpp
 * \brief browser - E-reader browser startup
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "browserTypes.h"
#include <pthread.h>
#include "pagebar.h"
#include <liberdm/display.h>
#include "browser.h"
#include "mozilla_api.h"
#include "selection.h"
#include "find.h"
#include "timingInfo.h"
#include "browserSettings.h"
#include "find.h"
#include "languages.h"
#include "ipcServer.h"
#include "toolbar.h"
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>
#include "contentLister.h"
#include "displayUpdate.h"
#include "browserLog.h"
#include "erbusy.h"

// global variables
static Ereader *gEreader;

// callbacks
guint   on_mainWindow_keypress(GtkWidget * widget, GdkEventKey * event, Ereader * reader);

/**
 * GTK & APP startup
 *
 * @param Manifest File 
 * @param URL location of the first file that needs to be displayed
 *
 * @returns A value of 0 indicates success
 */
int main(int argc, char **argv)
{
    bool    initialized;
    char   *input_URL = NULL;
    char   *manifest = NULL;
    int     returnValue = 0;

    // init threads - needed for toolbar service handling
    g_thread_init(NULL);
    gdk_threads_init();

    BR_LOGPRINTF("gtk threads init");

#ifdef TIMING_INFORMATION_ENABLED
    timing_init();
#endif //TIMING_INFORMATION_ENABLED

    gtk_init(&argc, &argv);

    languagesInit();

    //setup the erbusy deamon interaction
    erbusy_init();

    if (argc > 1)
    {
        BR_LOGPRINTF("argc = %d  argv[1] = %s", argc, argv[1]);
        if (argv[1] && strlen(argv[1]))
        {
            manifest = g_strdup(argv[1]);
        }            
    }

    if (argc > 2)
    {
        BR_LOGPRINTF("argc = %d  argv[2] = %s\n", argc, argv[2]);
        input_URL = g_strdup(argv[2]);
    }

    gEreader = new_gtk_ereader(manifest, input_URL);

    if (gEreader)
    {
        BR_LOGPRINTF("ereader_init gEreader 0x%x gEreader->pageStatus 0x%x", gEreader, gEreader->pageStatus);

        // init mozilla settings specific for this ereader device
        initialized = browser_settings_init(GTK_MOZ_EMBED(gEreader->mozEmbed));

        BR_LOGPRINTF("main -- browser_settings_init returned %s", (initialized == TRUE) ? "TRUE" : "FALSE");

        if (initialized == TRUE)
        {
            if (ipc_InstallIpcServer(gEreader) == TRUE)
            {
                //setup the contentlister server interaction
                contentlister_init();
                //setup the toolbar server interaction 
                toolbar_init();

                //startup => busy
                erbusy_blink();

                //setup the pagebar interaction
                pagebar_init();


                BR_LOGPRINTF("ereader_init gEreader 0x%x gEreader->pageStatus 0x%x", gEreader, gEreader->pageStatus);
                gdk_threads_enter();
                gtk_main();
                gdk_threads_leave();
            }
            else
            {
                BR_ERRORPRINTF("browser_toolbar_start_listening returned FALSE");
                returnValue = 1;
            }
        }
        else
        {
            BR_ERRORPRINTF("browser_settings_init returned FALSE");
            returnValue = 1;
        }
    }
    else
    {
        BR_ERRORPRINTF("new_gtk_ereader returned NULL");
        returnValue = 1;
    }

    if (input_URL)
    {
        g_free(input_URL);
    }

#ifdef TIMING_INFORMATION_ENABLED
    timing_destroy();
#endif //TIMING_INFORMATION_ENABLED

    return returnValue;
}


// currently used for debug reasons on the PC platform to try out 
// mozilla features
guint on_mainWindow_keypress(GtkWidget * widget, GdkEventKey * event, Ereader * reader)
{
    GtkWidget *mozEmbed;
    int     returnValue = 0;

    int     pagecount;
    int     currentpage;
    int     newpage;
	int     offset;

    BR_LOGPRINTF("on_mainWindow_keypress - event 0x%xn", event->keyval);

    mozEmbed = gEreader->mozEmbed;

    switch (event->keyval)
    {
    case GDK_Page_Up:
        BR_LOGPRINTF("jumping to the next page");
        if (mozilla_scrollable(pageUp_t, GTK_MOZ_EMBED(mozEmbed)) == FALSE)
        {
            if (mozilla_previous_page(GTK_MOZ_EMBED(mozEmbed)) == TRUE)
            {
                BR_LOGPRINTF("BUSY ON");
                erbusy_blink();
            }
            else
            {
                BR_LOGPRINTF("mozilla_previous_page returend FALSE");
                erbusy_off();
            }
        }
        else
        {
            if (mozilla_scroll(-1, GTK_MOZ_EMBED(mozEmbed)) == TRUE)
            {
              offset = mozilla_determine_page_offset(GTK_MOZ_EMBED(mozEmbed));
              pagebar_set_offset(offset);
              pagebar_redraw();
              display_update_request_screen_refresh(MOZEMBED_UPDATE_LEVEL, NULL);
            }
        }
        // return TRUE => stop event handling 
        returnValue = 1;
        break;

    case GDK_F1:
        BR_LOGPRINTF("PAGETURN_FORWARD LONG");

        pagecount = reader->pageStatus->pageCount;
        currentpage = reader->pageStatus->currentPage;
        newpage = ((currentpage + 5) <= pagecount) ? (currentpage + 5) : pagecount;

		printf("NEWPAGE: %d (%d, %d)\n", newpage, currentpage, pagecount);
		
        browser_pagebar_page_selected(newpage, gEreader);

        // return TRUE => stop event handling 
        returnValue = 1;
        break;

    case GDK_Page_Down:
        BR_LOGPRINTF("jumping to the previous page");
        if (mozilla_scrollable(pageDown_t, GTK_MOZ_EMBED(mozEmbed)) == FALSE)
        {
            if (mozilla_next_page(GTK_MOZ_EMBED(mozEmbed)) == TRUE)
            {
                BR_LOGPRINTF("BUSY ON");
                erbusy_blink();
            }
            else
            {
                BR_LOGPRINTF("mozilla_next_page returend FALSE");
                erbusy_off();
            }
        }
        else
        {
            if (mozilla_scroll(1, GTK_MOZ_EMBED(mozEmbed)) == TRUE)
            {
              offset = mozilla_determine_page_offset(GTK_MOZ_EMBED(mozEmbed));
              pagebar_set_offset(offset);
              pagebar_redraw();
              display_update_request_screen_refresh(MOZEMBED_UPDATE_LEVEL, NULL);
            }
        }

        // return TRUE => stop event handling 
        returnValue = 1;
        break;

    case GDK_F2:
        BR_LOGPRINTF("PAGETURN_BACK LONG");

        pagecount = reader->pageStatus->pageCount;
        currentpage = reader->pageStatus->currentPage;
        newpage = ((currentpage - 5) < 1) ? (1) : (currentpage - 5);

        printf("NEWPAGE: %d (%d, %d)\n", newpage, currentpage, pagecount);

        // erbusy_blink() is handled in this routine
        browser_pagebar_page_selected(newpage, gEreader);

        // return TRUE => stop event handling 
        returnValue = 1;
        break;

    case GDK_Up:
        BR_LOGPRINTF("jumping to the previous link");

        if (mozilla_previous_link(GTK_MOZ_EMBED(mozEmbed)) == TRUE)
        {
            // use non-flashy display update
            display_update_request_screen_refresh(MOZEMBED_UPDATE_LEVEL, (gpointer) DM_QUALITY_TYPING);
        }
        else
        {
            BR_LOGPRINTF("mozilla_previous_link returend FALSE");
            erbusy_off();
        }
        // return TRUE => stop event handling 
        returnValue = 1;
        break;

    case GDK_Down:
        BR_LOGPRINTF("jumping to the next link");

        if (mozilla_next_link(GTK_MOZ_EMBED(mozEmbed)) == TRUE)
        {
            // use non-flashy display update
            display_update_request_screen_refresh(MOZEMBED_UPDATE_LEVEL, (gpointer) DM_QUALITY_TYPING);
        }
        else
        {
            BR_LOGPRINTF("mozilla_next_link returend FALSE");
            erbusy_off();
        }

        // return TRUE => stop event handling 
        returnValue = 1;
        break;

    case GDK_F5:
      BR_LOGPRINTF("up key (F5) pressed... history.go(-1)");
        if (mozilla_back(GTK_MOZ_EMBED(mozEmbed)) == TRUE)
        {
            BR_LOGPRINTF("BUSY ON");
            erbusy_blink();
        }
        else
        {
            BR_ERRORPRINTF("mozilla_back returend FALSE (we are home), exit the app");
            erbusy_blink();
            browser_quit();
            gtk_main_quit();
        }
        // return TRUE => stop event handling 
        returnValue = 1;
        break;

    case GDK_F6:
        BR_LOGPRINTF("long confirm key (F6) pressed... goto ALT page");
        if (mozilla_alt_page(GTK_MOZ_EMBED(mozEmbed)) == TRUE)
        {
          BR_LOGPRINTF("BUSY ON");
          erbusy_blink();
        }
        else
        {
          BR_LOGPRINTF("mozilla_back returend FALSE");
          erbusy_off();
        }
        // return TRUE => stop event handling 
        returnValue = 1;
        break;

      case GDK_Home:
        BR_LOGPRINTF("long up key (Home) pressed... goto INDEX page");
        if (mozilla_overview_page(GTK_MOZ_EMBED(mozEmbed)) == TRUE)
        {
//          BR_LOGPRINTF("BUSY ON");
//          erbusy_blink();
        }
        else
        {
          BR_LOGPRINTF("mozilla_overview_page returend FALSE");
          erbusy_off();
        }
        // return TRUE => stop event handling 
        returnValue = 1;
        break;

    case GDK_Return:
        BR_LOGPRINTF("handle the current link");
        if (mozilla_handle_link(GTK_MOZ_EMBED(mozEmbed)) == TRUE)
        {
            BR_LOGPRINTF("BUSY ON");
            erbusy_blink();
        }
        break;
//
// Highlight/selection is enabled/disabled in selection.h in the browser code 
//    - should be added to the toolbar.cpp file late one  
//
#ifdef SELECTION_ENABLED
    case GDK_p:
        char    filename[256];

        //Used for debugging the highligth feature  
        sprintf(filename, "/home/ann/temp_files/selection.html");
        BR_LOGPRINTF("p key pressed => mozilla_save page in %s!!!", filename);
        if (mozilla_save(GTK_MOZ_EMBED(mozEmbed), filename, 1) == FALSE)
        {
            BR_LOGPRINTF("mozilla_save returned FALSE");
        }
        break;

    case GDK_i:
        BR_LOGPRINTF("identify_selection key pressed...");
        selection_identify_and_store_selection(GTK_MOZ_EMBED(mozEmbed));
        break;

    case GDK_r:
        BR_LOGPRINTF("r pressed ==> clear selections");
        selection_clear_current_page_selections(GTK_MOZ_EMBED(mozEmbed));
        break;
#endif //SELECTION_ENABLED

//
// Find was added to try out the language support 
//    - only tried on PC environment (no display update for dialog yet) 
//    - the setup application should be used as an example for the language support
//    - only searches within the currenly loaded XHTML page
//    - should be added to the toolbar.cpp file late one  
//
#ifdef FIND_DIALOG_ENABLED
    case GDK_f:
        BR_LOGPRINTF("f pressed => INVOKE FIND DIALOG");
        browser_find_dialog_create(reader);
        break;
#endif //FIND_DIALOG_ENABLED

    default:
        // Unhandled key pressed
        erbusy_off();
        break;
    
    } // switch

    return returnValue;
}
