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
 * \file find.cpp
 * \brief browser - find dialog and Mozilla find functionality
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
#include "find.h"
#include "browserLog.h"

/* mozilla specific headers */
#include "nsCOMPtr.h"
#include "nsMemory.h"
#include "gtkmozembed_internal.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsString.h"
#include "nsIWebBrowser.h"
#include "nsIWebBrowserFind.h"
#include "languages.h"

#ifdef FIND_DIALOG_ENABLED

// callbacks
static void find_dialog_ok_button_cb(GtkWidget * button, gpointer data);

//internally used routines
static gboolean mozilla_find_text_in_page(GtkMozEmbed * b,
                                          char *text,
                                          gboolean matchCase, gboolean findBackwards, gboolean entireWord, gboolean wrapFind,
                                          gboolean searchFrames);

static PRUnichar *mozilla_locale_to_unicode(const gchar * locStr);

void browser_find_dialog_create(Ereader * theEreader)
{
    GtkWidget *dialog;
    GtkWidget *hbox;

    //GtkWidget *hbox_toggle_buttons;
    GtkWidget *vbox_toggle_buttons;
    GtkWidget *entry;
    GtkWidget *label;
    GtkWidget *match_case;
    GtkWidget *find_backwards;
    GtkWidget *entire_word;
    GtkWidget *wrap_find;
    GtkWidget *button;

    if (theEreader)
    {
        BR_LOGPRINTF("entry");

        // tryout => toe overrule the mozilla language settings
        //languagesInit();  

        dialog = gtk_dialog_new();

        theEreader->dialog = dialog;

        gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
        gtk_window_set_keep_above(GTK_WINDOW(dialog), TRUE);
        gtk_widget_set_size_request(dialog, FIND_DIALOG_WIDTH, FIND_DIALOG_HEIGHT);
        gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
        gtk_window_set_title(GTK_WINDOW(dialog), _("Find in this Page"));

        hbox = gtk_hbox_new(FALSE, 0);
        gtk_container_set_border_width(GTK_CONTAINER(hbox), 4);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox, FALSE, FALSE, 0);

        label = gtk_label_new(gettext("Find: "));
        gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

        entry = gtk_entry_new();
        g_object_set_data(G_OBJECT(dialog), "entry", entry);
        gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);

        vbox_toggle_buttons = gtk_vbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox_toggle_buttons, FALSE, FALSE, 0);

        match_case = gtk_check_button_new_with_label(gettext("Match upper/lower case"));
        g_object_set_data(G_OBJECT(dialog), "match_case", match_case);
        gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(match_case), TRUE);
        gtk_box_pack_start(GTK_BOX(vbox_toggle_buttons), match_case, FALSE, FALSE, 0);

        find_backwards = gtk_check_button_new_with_label(gettext("Search backwards"));
        g_object_set_data(G_OBJECT(dialog), "find_backwards", find_backwards);
        gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(find_backwards), FALSE);
        gtk_box_pack_start(GTK_BOX(vbox_toggle_buttons), find_backwards, FALSE, FALSE, 0);

        entire_word = gtk_check_button_new_with_label(gettext("Whole words only"));
        g_object_set_data(G_OBJECT(dialog), "entire_word", entire_word);
        gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(entire_word), FALSE);
        gtk_box_pack_start(GTK_BOX(vbox_toggle_buttons), entire_word, FALSE, FALSE, 0);

        wrap_find = gtk_check_button_new_with_label(gettext("Wrap around"));
        g_object_set_data(G_OBJECT(dialog), "wrap_find", wrap_find);
        gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(wrap_find), FALSE);
        gtk_box_pack_start(GTK_BOX(vbox_toggle_buttons), wrap_find, FALSE, FALSE, 0);

        button = gtk_button_new_from_stock(GTK_STOCK_FIND);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area), button, FALSE, FALSE, 0);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(find_dialog_ok_button_cb), theEreader);

        button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area), button, FALSE, FALSE, 0);
        g_signal_connect_swapped(G_OBJECT(button), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer) dialog);

        gtk_widget_show_all(dialog);
        gtk_widget_grab_focus(entry);
    }

    return;
}

static void find_dialog_ok_button_cb(GtkWidget * button, gpointer data)
{
    GtkWidget *dialog;
    GtkEntry *entry;
    gboolean matchCase;
    gboolean findBackwards;
    gboolean entireWord;
    gboolean wrapFind;
    Ereader *theEreader;
    gboolean returnValue;

    BR_LOGPRINTF("entry");

    theEreader = (Ereader *) data;

    if (theEreader)
    {
        dialog = theEreader->dialog;

        if (dialog)
        {
            matchCase = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_object_get_data(G_OBJECT(dialog), "match_case")));
            findBackwards = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_object_get_data(G_OBJECT(dialog), "find_backwards")));
            entireWord = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_object_get_data(G_OBJECT(dialog), "entire_word")));
            wrapFind = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_object_get_data(G_OBJECT(dialog), "wrap_find")));

            BR_LOGPRINTF("match_case=%s find_backwards=%s entire_word=%s wrap_find=%s -- entry",
                         matchCase ? "TRUE" : "FALSE", findBackwards ? "TRUE" : "FALSE", entireWord ? "TRUE" : "FALSE",
                         wrapFind ? "TRUE" : "FALSE");

            entry = GTK_ENTRY(g_object_get_data(G_OBJECT(dialog), "entry"));

            returnValue = mozilla_find_text_in_page(GTK_MOZ_EMBED(theEreader->mozEmbed), (char *) gtk_entry_get_text(entry), matchCase, findBackwards, entireWord, wrapFind, TRUE); //always search through all frames in the content area


            if (returnValue == TRUE)
            {
                BR_LOGPRINTF("mozilla_find_text_in_page returned TRUE");
            }
            else
            {
                BR_LOGPRINTF("mozilla_find_text_in_page returned FALSE");
            }
        }
    }

    BR_LOGPRINTF("destroy the dialog box");

    gtk_widget_destroy(dialog);
    theEreader->dialog = NULL;
}

/*
 * mozilla_find_text_in_page
 * 
 *     Finds, highlights and scrools into view the next occurence of hte search string
 *
 */
static  gboolean
mozilla_find_text_in_page(GtkMozEmbed * b,
                          char *text, gboolean matchCase, gboolean findBackwards, gboolean entireWord, gboolean wrapFind,
                          gboolean searchFrames)
{
    gboolean DidFind = FALSE;
    PRUnichar *search_string;
    nsIWebBrowser *wb = nsnull;

    nsCOMPtr < nsIWebBrowserFind > finder;

    BR_LOGPRINTF("entry");

    g_return_val_if_fail(b != NULL, FALSE);
    gtk_moz_embed_get_nsIWebBrowser(b, &wb);

    finder = do_GetInterface(wb);

    if (wb)
    {
        if (finder)
        {
            search_string = mozilla_locale_to_unicode(text);

            finder->SetSearchString(search_string);
            finder->SetMatchCase(matchCase);
            finder->SetFindBackwards(findBackwards);
            finder->SetWrapFind(wrapFind);
            finder->SetEntireWord(entireWord);
            finder->SetSearchFrames(searchFrames);
            finder->FindNext(&DidFind);

            // free memory allocated for the search string
            g_free(search_string);

        }
        else
        {
            BR_ERRORPRINTF("wb = NULL");
        }
    }
    else
    {
        BR_ERRORPRINTF("finder = NULL");
    }

    return (DidFind);
}

/*
 * mozilla_locale_to_unicode: Decodes a string encoded for the current
 * locale into unicode. Used for getting text entered in a GTK+ entry
 * into a form which mozilla can use.
 *
 */
PRUnichar *mozilla_locale_to_unicode(const gchar * locStr)
{
    if (locStr == NULL)
    {
        return (NULL);
    }
    nsAutoString autoStr;

    autoStr.AssignWithConversion(locStr);
    PRUnichar *uniStr = ToNewUnicode(autoStr);

    return (uniStr);
}

#endif //FIND_DIALOG_ENABLED
