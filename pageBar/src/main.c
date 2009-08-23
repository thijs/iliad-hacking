/*
 * This file is part of pageBar.
 *
 * pageBar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pageBar is distributed in the hope that it will be useful,
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

#include <config.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include <liberdm/display.h>
#include <liberipc/eripc.h>
#include <liberipc/eripcpagebar.h>
#include "erbusy.h"
#include "gtkPagebar.h"

#define SERVER_BUFFER_SIZE 1024

static GtkWidget *thePagebar;
erServerChannel_t theServerChannel;

static void on_destroy(GtkWidget * widget, gpointer data)
{
    // Do stuff before quitting... 
    // Noting here yet.

    gtk_main_quit();
}

void pagebarMsgRxd(gpointer data, gint source_fd, GdkInputCondition condition)
{
    char    szBuffer[SERVER_BUFFER_SIZE];
    int     nBuf = SERVER_BUFFER_SIZE;
    erIpcCmd_t command;
    GtkPagebar* pagebar = GTK_PAGEBAR(thePagebar);    

    erIpcGetMessage((erServerChannel_t) data, szBuffer, &nBuf);
    //printf("Received %s\n", szBuffer);

    if (pbParseCommand(szBuffer, &command) >= 0)
    {
        switch (command.cc)
        {
        case ccPbSetPageCount:
            pagebar->pagecount = atoi(command.arg[1]);
            break;
        case ccPbSetCurrentPage:
            // currentPage cannot be less than 1
            pagebar->currentPage = (atoi(command.arg[1]) < 1) ? 1 : atoi(command.arg[1]);
            //following line is added by Jian for bug:347
            pagebar->lastPage=pagebar->currentPage;
            break;
        case ccPbSetCurrentPageOffset:
            pagebar->currentPageOffset = atoi(command.arg[1]);
            break;
        case ccPbSetZoomMode:
            pagebar->zoomMode = atoi(command.arg[1]);
            break;
        case ccPbSetDrawAreaOrientation:
            /* Todo */
            pagebar->orientation = atoi(command.arg[1]);
            break;
        case ccPbSetDrawAreaHOrigin:
            pagebar->drawAreaXPos = atoi(command.arg[1]);
            break;
        case ccPbSetDrawAreaVOrigin:
            pagebar->drawAreaYPos = atoi(command.arg[1]);
            break;
        case ccPbSetDrawAreaHSize:
            pagebar->drawAreaWidth = atoi(command.arg[1]);
            break;
        case ccPbSetDrawAreaVSize:
            pagebar->drawAreaHeight = atoi(command.arg[1]);
            break;
        case ccPbSetBarFontType:
            pagebar->barFontType = atoi(command.arg[1]);
            break;
        case ccPbSetBarFontHeight:
            pagebar->barFontHeight = atoi(command.arg[1]);
            break;
        case ccPbAddBookmark:
            addBookmark(pagebar, atoi(command.arg[1]));
            break;
        case ccPbRemoveBookmark:
            removeBookmark(pagebar, atoi(command.arg[1]));
            break;
        case ccPbSetBookmarkMax:
            pagebar->bookmarkMaxValue = atoi(command.arg[1]);
            break;
        case ccPbAddNote:
            addNote(pagebar, atoi(command.arg[1]));
            break;
        case ccPbRemoveNote:
            removeNote(pagebar, atoi(command.arg[1]));
            break;
        case ccPbSetNotesMax:
            pagebar->notesMaxValue = atoi(command.arg[1]);
            break;
        case ccPbReset:
            pagebar->currentApp = atoi(command.arg[0]);
            //          	  printf("Current app = %d\n", pagebar->currentApp);
            pagebar_reset_values(pagebar);
            break;
        case ccPbShow:
            pagebar->show = atoi(command.arg[1]);
            break;
        case ccPbRedraw:
            gtk_pagebar_draw(pagebar);
            break;
        case ccPbSynchronise:
            // printf("\nOn receive pbSynchronise message from application\n");
            pagebar_reportSync(pagebar);
            break;            
        default:
            printf("UNKNOWN COMMAND %d\n", command.cc);
            break;
        }
    }
}

static int installIpcServer()
{
    int     ret;
    int     fd = -1;

    ret = erIpcOpenServerChannel(ER_PAGEBAR_CHANNEL, &theServerChannel);

    if (ret != (-1))
    {
        fd = erIpcGetServerFd(theServerChannel);

        // gtk specific mechanism to monitor input file descriptor.
        ret = gdk_input_add(fd, GDK_INPUT_READ, pagebarMsgRxd, (gpointer) theServerChannel);
        return TRUE;
    }
    else
    {
        fprintf(stderr, "Could not open server channel\n");
        return FALSE;
    }
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *fixedContainer;

    int num_pages = 30;
    int selected_page = 15;

    if ( argc == 3 )
    {
        num_pages = atoi(argv[1]);
        selected_page = atoi(argv[2]);
    }

    /* init threads */
    g_thread_init(NULL);
    gdk_threads_init();
    gdk_threads_enter();

    // open the RC file associate with this program
    //    gtk_rc_parse(DATA_DIR "/contentLister.rc");
    //    CL_LOGPRINTF("rc file %s", DATA_DIR "/contentLister.rc");

    gtk_init(&argc, &argv);

    // create the main, top level, window 
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), PACKAGE " " VERSION);
    gtk_window_set_accept_focus(GTK_WINDOW(window), FALSE);  // window cannot get focus, as this causes trouble with keyboard
    gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DOCK); // Tell WM to dock this window
    gtk_window_position(GTK_WINDOW(window), GTK_WIN_POS_NONE); 
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    fixedContainer = gtk_fixed_new();

    thePagebar = gtk_pagebar_new();

    if (thePagebar)
    {
        // Connect the destroy event of the window with our on_destroy function
        // When the window is about to be destroyed we get a notificaton and
        // stop the main GTK loop
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_destroy), thePagebar);
    }


    // gtk_container_add(GTK_CONTAINER(fixedContainer), thePagebar);
    gtk_fixed_put(GTK_FIXED(fixedContainer), thePagebar, 0, 0);
    gtk_container_add(GTK_CONTAINER(window), fixedContainer);
    gtk_pagebar_draw(GTK_PAGEBAR(thePagebar));
    {
        GtkPagebar* pagebar = GTK_PAGEBAR(thePagebar);
        GtkWidget* entry = gtk_entry_new_with_max_length(4);
        gtk_widget_set_size_request(GTK_WIDGET(entry), pagebar->cpiWidth-6, 24);
        gtk_fixed_put( GTK_FIXED(fixedContainer),
            entry,
            pagebar->barXPos + (pagebar->barWidth  - pagebar->cpiWidth) / 2 + 3,
            pagebar->barYPos + (pagebar->barHeight - pagebar->cpiWidth) / 2 + 18 );
        pagebar->pageEntry = entry;
    }

    // gtk_pagebar_selectpage(GTK_PAGEBAR(pagebar), selected_page);

    // associate control callback -- IPC calls have to be connected using this...
    // g_signal_connect(G_OBJECT(pagebar), "pagebar_page_selected", G_CALLBACK(ctrl_pagebar_clicked_event), pagebar);

    installIpcServer();
    erbusy_init();

    gtk_widget_show(thePagebar);
    gtk_widget_show(fixedContainer);
    gtk_widget_show(window);

    gtk_main();
    gdk_threads_leave();

    return 0;
}
