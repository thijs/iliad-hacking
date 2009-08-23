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
 * \file connectScreen.h
 * \brief connectionMgr - the entry of 'connect-to-server' screen
 *                        containing a connectScreenOptions 
 *                               and a connectScreenProfiles
 *                        screen handling and event processing
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _CONNECT_SCREEN_H_
#define _CONNECT_SCREEN_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>

// these values correspond with the notebook page numbers
typedef enum
{
    connectScreenOptions_e=0,
    connectScreenProfiles_e,
    connectScreenUndef_e
} connectScreen_t;

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    GtkWidget* connect_screen_create(void);
    void connect_screen_set_text(void);
    void connect_screen_display(void);

    void connect_set_toolbar(void);
    void connect_set_pagebar(void);
    void connect_goto_page(connectScreen_t newpage);
    gint connect_get_current_page (gint * pagecount);
     
    gboolean on_connect_keypress(GdkEventKey* event);
    void on_connect_goto_page(gint newpage);
    void on_connect_icon_clicked(int iconID, int iconState);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_CONNECT_SCREEN_H_

