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
 * \file connectScreenProfiles.h
 * \brief connectionMgr - the second page of 'connect-to-server' screen
 *                        the connection profiles screen
 *                        containing a profile list and a new profile entry
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _CONNECT_PROFILES_H_
#define _CONNECT_PROFILES_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>

#include "connectPing.h"

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    GtkWidget *connect_profiles_create(void);
    void connect_profiles_set_text(void);
    void connect_profiles_display(void);
    
    gboolean on_connect_profiles_keypress(GdkEventKey *event);
    void on_connect_profiles_icon_clicked(int iconID, int iconState);

    void connect_profiles_restore_ping_ctxt(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_CONNECT_PROFILES_H_

