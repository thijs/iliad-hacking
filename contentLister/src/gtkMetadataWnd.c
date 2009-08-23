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
* \file gtkMetadataWnd.c
* \brief gtkMetadataWnd - eReader content metadata editor
* 
* <File description>
*  The eReader content metadata editor provides gtk widgets to enable user change 
*  content title and description. Which are created and updated with the following API
*/

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#define _GNU_SOURCE

// system include files
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

// gtk includes
#include <gtk/gtk.h>

// ereader library includes
#include <gdk/gdkkeysyms.h>
#include <liberdm/display.h>
#include <liberdm/erdm.h>
#include <libergtk/ergtk.h>
#include <liberipc/eripctoolbar.h>
#include <libermanifest/ermanifest.h>

// local includes
#include "contentListerLog.h"
#include "lister.h"
#include "gtkMetadataWnd.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "displayUpdate.h"
#include "languages.h"
#include "toolbar.h"
#include "erbusy.h"


// local data
static cmMetadata_t g_wnd;

// local functions


//////////////////////////////////////////////////////////////////////////
// event handler
//////////////////////////////////////////////////////////////////////////

static void on_changed(GtkEditable * widget, gpointer data)
{
    CL_LOGPRINTF("entry");

    // check for illegal characters in title/filename
    if (GTK_WIDGET(widget) == g_wnd.nameEntry)
    {
        const char* illegal_chars = ILLEGAL_FILENAME_CHARS;
        const char* title = gtk_entry_get_text(GTK_ENTRY(widget));
        //
        char* new_title = alloca( strlen(title) + 1 );
        strcpy(new_title, title);
        //
        gboolean found = FALSE;
        char* cp = new_title;
        while (*cp)
        {
            if ( strchr(illegal_chars, *cp) )
            {
                *cp = '-';
                found = TRUE;
            }
            cp++;
        }
        if (found)
        {
            CL_ERRORPRINTF("illegal title [%s] new_title [%s]", title, new_title);
            gtk_entry_set_text(GTK_ENTRY(widget), new_title);
        }
    }
}

/****
static void on_move_focus(GtkTextView *textView, GtkDirectionType arg1, gpointer data)
{
    CL_LOGPRINTF("entry");

    display_update_request_screen_refresh(TEXTENTRY_CHANGED_LEVEL, NULL);
}

static void on_button_release(GtkWidget * widget, GdkEventButton * event, gpointer data)
{
    CL_LOGPRINTF("entry");

    display_update_request_screen_refresh(TEXTENTRY_CHANGED_LEVEL);
}
****/

static gboolean on_expose(GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
    CL_LOGPRINTF("entry");

    gtk_widget_grab_focus(GTK_WIDGET(g_wnd.descEntry));
    /* problems with multi-line editor */
    const char* cp = gtk_entry_get_text(GTK_ENTRY(g_wnd.descEntry2));
    if (cp != NULL  &&  cp[0] != '\0')
    {
        gtk_widget_grab_focus(GTK_WIDGET(g_wnd.descEntry2));
    }

    // de-select all characters, cursor at end of input field
    gtk_editable_select_region(GTK_EDITABLE(g_wnd.descEntry), -1, -1);
    gtk_editable_select_region(GTK_EDITABLE(g_wnd.descEntry2), -1, -1);

    display_update_request_screen_refresh(LISTER_EXPOSE_LEVEL);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// helper construction function
//////////////////////////////////////////////////////////////////////////

// object hierarchy:
//     parent
//       |-- nameTag (label)
//       |-- nameEntry (gtk_entry)
//
static void createNameItem(GtkWidget* parent)
{
    GtkWidget* widget;

    CL_LOGPRINTF("entry");

    // nameTag
    widget = gtk_label_new("");
    gtk_widget_set_name(GTK_WIDGET(widget), "content_wnd_text_bold");
    gtk_widget_set_size_request(widget, -1, METADATA_TAG_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 1.0);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_wnd.nameTag = widget;

    // nameEntry
    widget = ergtk_entry_new();
    gtk_widget_set_name(GTK_WIDGET(widget), "content_wnd_edit");
    gtk_widget_set_size_request(widget, METADATA_ENTRY_WIDTH, METADATA_NAME_HEIGHT);
    g_signal_connect_after(G_OBJECT(widget), "changed",              G_CALLBACK(on_changed),        NULL);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);    
    gtk_widget_show(widget);
    g_wnd.nameEntry = widget;
}

/*******
// object hierarchy
//  authorItem (vbox)
//    |-- title tag (label)
//    |-- hbox
//          |-- vbox1
//                |-- author1 tag  (label)
//                |-- author1 edit (gtk_entry)
//          |-- vbox2
//                |-- author2 tag  (label)
//                |-- author2 edit (gtk_entry)
static GtkWidget * createAuthorItem(ContentMetaData * data, cmMetadata_t* ptr)
{
    CL_LOGPRINTF("entry");

    GtkWidget *vbox = gtk_vbox_new(FALSE, METADATA_WND_ITEM_SPACING);

    // step2 tag
    GtkWidget * titleTag = gtk_label_new("");
    gtk_misc_set_alignment(GTK_MISC(titleTag), 0.0, 0.5);
    gtk_widget_set_name(GTK_WIDGET(titleTag), "content_wnd_text_bold");
    gtk_box_pack_start(GTK_BOX(vbox), titleTag, FALSE, FALSE, 0);

    // hbox
    GtkWidget * hbox = gtk_hbox_new(TRUE, METADATA_WND_ITEM_SPACING);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    // vbox1
    GtkWidget *vbox1 = gtk_vbox_new(TRUE, METADATA_WND_ITEM_SPACING);
    gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 0);

    // label 
    GtkWidget * author1 = gtk_label_new("");
    gtk_widget_set_name(GTK_WIDGET(author1), "content_wnd_text_normal");
    gtk_misc_set_alignment(GTK_MISC(author1), 0.0, 0.5);
    gtk_label_set_line_wrap(GTK_LABEL(author1), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox1), author1, FALSE, FALSE, 0);

    // edit
    GtkWidget * edit1 = ergtk_entry_new();
    gtk_widget_set_name(GTK_WIDGET(edit1), "content_wnd_edit");
    gtk_box_pack_start(GTK_BOX(vbox1), edit1, FALSE, FALSE, 0);
    g_signal_connect_after(G_OBJECT(edit1), "changed", G_CALLBACK(on_changed), NULL);
    // ptr->authorEntry = edit1;   

    // vbox2
    GtkWidget *vbox2 = gtk_vbox_new(TRUE, METADATA_WND_ITEM_SPACING);
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 0);

    // label 
    GtkWidget * author2 = gtk_label_new("");
    gtk_widget_set_name(GTK_WIDGET(author2), "content_wnd_text_normal");
    gtk_misc_set_alignment(GTK_MISC(author2), 0.0, 0.5);
    gtk_label_set_line_wrap(GTK_LABEL(author2), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox2), author2, FALSE, FALSE, 0);

    // edit
    GtkWidget * edit2 = ergtk_entry_new();
    gtk_widget_set_name(GTK_WIDGET(edit2), "content_wnd_edit");
    gtk_box_pack_start(GTK_BOX(vbox2), edit2, FALSE, FALSE, 0);
    g_signal_connect_after(G_OBJECT(edit2), "changed", G_CALLBACK(on_changed), NULL);
    // ptr->author2Entry = edit2;   
    return vbox;
}
*******/

// object hierarchy:
//     parent
//       |-- descTag (label)
//       |-- descEntry (gtk_entry)
//       |-- descEntry2 (gtk_entry)
static void createDescItem(GtkWidget* parent)
{
    GtkWidget* widget;

    CL_LOGPRINTF("entry");

    // title tag
    widget = gtk_label_new("");
    gtk_widget_set_name(GTK_WIDGET(widget), "content_wnd_text_bold");
    gtk_widget_set_size_request(widget, -1, METADATA_TAG_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 1.0);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_wnd.descTag = widget;
    
    /****
     * Multi-line text editor has strange side-effects.
     * For example, once it has the focus we cannot get the focus back to other objects.
     * Might be that we are doing something wrong, but for the time being we use two single-line entries.
     * 
    // multiline text editor
    widget = gtk_text_view_new();
    gtk_widget_set_name(GTK_WIDGET(widget), "content_wnd_edit");
    gtk_widget_set_size_request(widget, METADATA_ENTRY_WIDTH, METADATA_DESC_HEIGHT);
    gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(widget), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(widget), TRUE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(widget), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(widget), GTK_WRAP_CHAR);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    //
    // signals    
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
    g_signal_connect_after(G_OBJECT(widget), "move-focus",           G_CALLBACK(on_move_focus),     NULL);
    g_signal_connect_after(G_OBJECT(widget), "move-cursor",          G_CALLBACK(on_move_cursor),    NULL);
    g_signal_connect(      G_OBJECT(widget), "button-release-event", G_CALLBACK(on_button_release), NULL);
    g_signal_connect_after(G_OBJECT(buffer), "changed",              G_CALLBACK(on_changed),        NULL);
    ****/

    // descEdit
    widget = ergtk_entry_new();
    gtk_widget_set_name(GTK_WIDGET(widget), "content_wnd_edit");
    gtk_widget_set_size_request(widget, METADATA_ENTRY_WIDTH, METADATA_DESC_HEIGHT);
    g_signal_connect_after(G_OBJECT(widget), "changed",              G_CALLBACK(on_changed),        NULL);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);    
    gtk_widget_show(widget);
    g_wnd.descEntry = widget;

    // descEdit2
    widget = ergtk_entry_new();
    gtk_widget_set_name(GTK_WIDGET(widget), "content_wnd_edit");
    gtk_widget_set_size_request(widget, METADATA_ENTRY_WIDTH, METADATA_DESC_HEIGHT);
    g_signal_connect_after(G_OBJECT(widget), "changed",              G_CALLBACK(on_changed),        NULL);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);    
    gtk_widget_show(widget);
    g_wnd.descEntry2 = widget;
}


// object hierarchy:
//     parent
//       |-- finishTag (label)
static void createFinishItem(GtkWidget* parent)
{
    GtkWidget* widget;

    CL_LOGPRINTF("entry");

    // title tag
    widget = gtk_label_new("");
    gtk_widget_set_name(GTK_WIDGET(widget), "content_wnd_text_bold");
    gtk_widget_set_size_request(widget, -1, METADATA_TAG_HEIGHT);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 1.0);
    gtk_box_pack_start(GTK_BOX(parent), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
    g_wnd.finishTag = widget;
    
    return;
}



//////////////////////////////////////////////////////////////////////////
// exposed function impl.
//////////////////////////////////////////////////////////////////////////

// object hierarchy:
//     parent
//       |-- background (event-box)
//             |-- alignment
//                   |-- vbox
//                         |-- name item
//                         |-- desc item
//                         |-- finish item
// 
GtkWidget* create_content_metadata_wnd(GtkWidget * parent)
{
    CL_LOGPRINTF("entry");

    // main window
    g_wnd.window = parent;
    // g_signal_connect(G_OBJECT(g_wnd.window), "focus-in-event", G_CALLBACK(on_main_wnd_focus_in), wnd);
    // g_signal_connect(G_OBJECT(g_wnd.window), "key_press_event", G_CALLBACK(on_keypress), wnd);
    g_signal_connect(G_OBJECT(g_wnd.window), "expose-event", G_CALLBACK(on_expose), NULL);

    // background
    GtkWidget* background = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(background), "content_wnd_background");   
    gtk_widget_set_size_request(GTK_WIDGET(background), METADATA_BK_IMG_WIDTH, METADATA_BK_IMG_HEIGHT);
    gtk_container_add(GTK_CONTAINER(parent), background);
    gtk_widget_show(background);
        
    // alignment
    GtkWidget* alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), METADATA_VBORDER, METADATA_VBORDER, METADATA_HBORDER, METADATA_HBORDER);
    gtk_container_add(GTK_CONTAINER(background), alignment);
    gtk_widget_show(alignment);

    // vbox
    GtkWidget* vbox = gtk_vbox_new(FALSE, METADATA_WND_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    gtk_widget_show(vbox);
   
    // name item
    createNameItem(vbox);
            
    // desc item
    createDescItem(vbox);

    // finish item
    createFinishItem(vbox);

    // set screen texts
    content_metadata_set_text();

    // instance-private data
    g_wnd.on_change_occurred     = FALSE;
    g_wnd.on_movecursor_occurred = FALSE;
    g_wnd.display_update_pending = FALSE;

    return background;
}


// set screen texts
void content_metadata_set_text()
{
    CL_LOGPRINTF("entry");

    // name item
    gtk_label_set_text( GTK_LABEL(g_wnd.nameTag), _("Step 1: Please enter a name for the content") );
            
    // desc item
    gtk_label_set_text( GTK_LABEL(g_wnd.descTag), _("Step 2: Enter a description by which you can identify the content") );

    // finish item
    gtk_label_set_text( GTK_LABEL(g_wnd.finishTag), _("Step 3: Click on the 'Tagging' icon to save your changes") );
}


// should we popup metadata editor window if we could not open manifest file?
gboolean content_metadata_read_manifest(const gchar* szManifest)
{
    CL_LOGPRINTF("entry");

    g_assert(szManifest != NULL);

    int   ret;
    gchar szTitle[MD_MAX_TITLE_SIZE];
    gchar szDescription[MD_MAX_DESCRIPTION_SIZE];
       
    // open manifest file
    erManifest manifest;
    if (RET_OK != ermXmlOpenFile(szManifest, &manifest))
    {
        CL_ERRORPRINTF("Could not open manifest file [%s]", szManifest);
        return FALSE;
    }
    
    // get title
    ret = ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Title", szTitle, sizeof(szTitle));
    if (ret != RET_OK)
    {
        strcpy(szTitle, "");
    }
    gtk_entry_set_text(GTK_ENTRY(g_wnd.nameEntry), szTitle);
    
    // get description
    ret = ermXmlGetString(&manifest, "/package/metadata/dc-metadata/Description", szDescription, sizeof(szDescription));
    if (ret != RET_OK)
    {
        strcpy(szDescription, "");
    }
    /****
     * Problems with multi-line text editor, see createDescEntry();
     *
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(g_wnd.descEntry));
    gtk_text_buffer_set_text(buffer, szDescription, -1);
    ****/
    char* cp = strchr(szDescription, '\n');
    if (cp)
    {
        *cp = '\0';
        gtk_entry_set_text(GTK_ENTRY(g_wnd.descEntry2), cp + 1);
    }
    else
    {
        gtk_entry_set_text(GTK_ENTRY(g_wnd.descEntry2), "");
    }
    gtk_entry_set_text(GTK_ENTRY(g_wnd.descEntry), szDescription);
        
    // close manifest file
    ermXmlClose(&manifest);
    return TRUE;
}


gboolean content_metadata_rename_container(const gchar* szManifest, gchar* szContainer, gint len)
{
    CL_LOGPRINTF("entry: manifest [%s]", szManifest);

    g_return_val_if_fail((szManifest != NULL), FALSE);
    
    gboolean retval = FALSE;

       
    // retrieve container path
    char* cp = strrchr(szManifest, '/');
    if (cp)
    {
        *cp = '\0';
        snprintf(szContainer, len, "%s", szManifest);
        *cp = '/';
    }
    else
    {
        CL_ERRORPRINTF("Illegal manifest path [%s]", szManifest);
    }

    // retrieve container name
    char* szContainerName = strrchr(szContainer, '/');
    if (szContainerName)
    {
        szContainerName++;
    }
    else
    {
        CL_ERRORPRINTF("Illegal container path [%s]", szContainer);
    }

    // retrieve title from screen object
    const char* szTitle = gtk_entry_get_text(GTK_ENTRY(g_wnd.nameEntry));

    // set container name to (modified) title
    if (   strlen(szTitle) > 0
        && strcmp(szTitle, szContainerName) != 0 )
    {
        // container name is different from title: rename container

        // determine new container name
        int  n = strlen(szContainer) - strlen(szContainerName);
        char* newContainer = alloca(n + strlen(szTitle) + 1);
        g_return_val_if_fail((newContainer != NULL), FALSE);
        strncpy(newContainer, szContainer, n);
        strcpy(&newContainer[n], szTitle);
        CL_LOGPRINTF("New container name [%s]", newContainer);        
        
        // rename container, but only when newName does not exist
        struct stat statbuf;
        if (   stat(newContainer, &statbuf) == -1
            && rename(szContainer, newContainer) ==  0 )
        {
            CL_LOGPRINTF("Renamed container ok");
            snprintf(szContainer, len, "%s", newContainer);
            retval = TRUE;
        }
        else
        {
            CL_ERRORPRINTF("Cannot rename container [%s]", strerror(errno));
            
            // rename failed: force title to container name
            gtk_entry_set_text(GTK_ENTRY(g_wnd.nameEntry), szContainerName);
        }
    }
    
    return retval;
}


gboolean content_metadata_write_manifest(const gchar* szContainer)
{
    CL_LOGPRINTF("entry: manifest [%s]", szContainer);

    g_return_val_if_fail((szContainer != NULL), FALSE);

    gboolean retval = FALSE;
    
       
    // retrieve title from screen object
    const char* szTitle = gtk_entry_get_text(GTK_ENTRY(g_wnd.nameEntry));

    // retrieve description from screen object
    /****
     * Problems with multi-line text editor, see createDescEntry();
     *
    GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(g_wnd.descEntry));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char* szDescription = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);      
    ****/
    const char* szDescription = gtk_entry_get_text(GTK_ENTRY(g_wnd.descEntry));
    const char* szDescription2 = gtk_entry_get_text(GTK_ENTRY(g_wnd.descEntry2));
    if (szDescription2 != NULL  &&  szDescription2[0] != '\0')
    {
        char* cp = alloca( strlen(szDescription) + strlen(szDescription2) + 2 );
        if (cp)
        {
            sprintf(cp, "%s\n%s", szDescription, szDescription2);
            szDescription = cp;
        }
    }
    
    // open manifest file and update it
    erManifest manifest;
    if (RET_OK == ermXmlOpenManifest(szContainer, &manifest))
    {
        // update title
        if (RET_OK == ermXmlExist(&manifest, "/package/metadata/dc-metadata/Title"))
        {
            ermXmlSetString( &manifest,
                             "/package/metadata/dc-metadata/Title",
                             szTitle );
        }
        else
        {
            ermXmlNewString( &manifest,
                             "/package/metadata/dc-metadata",
                             "Title",
                             szTitle );
        }
        
        // update description
        if (RET_OK == ermXmlExist(&manifest, "/package/metadata/dc-metadata/Description"))
        {
            ermXmlSetString( &manifest,
                             "/package/metadata/dc-metadata/Description",
                             szDescription );
        }
        else
        {
            ermXmlNewString( &manifest,
                             "/package/metadata/dc-metadata",
                             "Description",
                             szDescription );
        }
        
        // save manifest
        ermXmlSaveAndClose(&manifest);

        retval = TRUE;
    }
    else
    {
        CL_ERRORPRINTF("Could not open manifest file in [%s]", szContainer);
    }
    
    return retval;
}

