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
 * \file mozEmbedCallbacks.h
 * \brief browser - eReader "GtkMozEmbed" related callbacks
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __MOZEMBED_CALLBACKS__
#define __MOZEMBED_CALLBACKS__

/**
 *  Callback "location signal" this signal is emitted any time that the location of the document 
 *  has changed.  
 *
 * @param embed reference to MozEmbed widget
 * @param data NULL
 *
 * @returns void
 */
void location_changed_cb(GtkMozEmbed *embed, gpointer data);

/**
 *  Callback "location signal"  This signal is emitted when the toplevel window in question 
 *  needs to be shown or hidden. If the visibility argument is TRUE then the window should 
 *  be shown. If it's FALSE it should be hidden. 
 *
 * @param embed reference to MozEmbed widget
 * @param visibility
 * @param browser reference to rreader struct
 *
 * @returns void
 */
void visibility_cb(GtkMozEmbed *embed, gboolean visibility, Ereader *browser);

/**
 *  Callback "net_stop"  This signal is emitted any time that the loading of 
 *  a document has completed 
 *
 * @param embed reference to MozEmbed widget
* @param browser reference to reader struct
 *
 * @returns void
 */
void load_finished_cb(GtkMozEmbed *embed, Ereader *browser);

/**
 *  Callback "progress"  This signal is emitted any time that there is a cahnge in the progress
 *  of loading a document. 
 *
 * @param embed reference to MozEmbed widget
 * @param browser reference to reader struct
 * @param cur how much of the document has been downloaded
 * @param max the length of the document
 *
 * @returns void
 */
void progress_change_cb(GtkMozEmbed * embed, gint cur, gint max, Ereader *browser);

// ???
void status_change_cb(GtkMozEmbed *embed, gpointer request, gint status, gpointer message);


/**
 *  Callback "open_uri signal" this signal is emitted any time that the browser wants to 
 *  load a new URI.
 *
 * @param embed reference to MozEmbed widget
 * @param uri the URI to be loaded
 * @param data NULL
 *
 * @returns FALSE to load the URI, TRUE to prevent it from loading
 */
gint open_uri_cb (GtkMozEmbed *embed, const char *uri, gpointer data);

//gboolean display_update_during_idle_handling(gpointer data);

#endif // __MOZEMBED_CALLBACKS__
