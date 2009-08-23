/*
 * This file is part of connectionMgr.
 *
 * connectionMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * connectionMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file connectScreenOptions.h
 * \brief connectionMgr - the first page of 'connect-to-server' screen
 *                        the connection options configuration screen
 *                        containing 
 *                        a selection widget 
 *                            lets users choose 
 *                            which server(iDS/PC) connect with
 *                        a selection widget 
 *                            lets users choose the network type
 *                            (wired/wireless/dialup)
 *                        a information widget gives some feedback to users 
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _CONNECT_SCREEN_OPTIONS_H_
#define _CONNECT_SCREEN_OPTIONS_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    GtkWidget* connect_options_create(void);
    void connect_options_set_text(void);
    void connect_options_display(void);
    gboolean on_connect_options_keypress(GdkEventKey *event);
    void connect_options_restore_scan_ctxt(void);

    gboolean on_connect_options_keypress(GdkEventKey *event);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_CONNECT_SCREEN_OPTIONS_H_

