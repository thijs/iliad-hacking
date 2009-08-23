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
 * \file editScreenProxy.h
 * \brief connectionMgr - "Proxy Server" screen
 *
 * Copyright (C) 2007 iRex Technologies BV.
 * 
 */

#ifndef _EDIT_SCREEN_PROXY_H_
#define _EDIT_SCREEN_PROXY_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <glib.h>
#include <liberregxml/erregapi.h>

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    GtkWidget *edit_proxy_create(void);
    void edit_proxy_set_text(void);
    void edit_proxy_screen_leave(void);

    void edit_proxy_set_network_settings(const regNetworkProfile_t *settings);
    void edit_proxy_get_network_settings(regNetworkProfile_t *settings);
    gboolean edit_proxy_check_network_settings(void);
    void edit_proxy_restore_network_settings(void);

    void edit_proxy_set_title(const char *profilename);

    gboolean on_edit_proxy_keypress(GdkEventKey *event);

#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif //_EDIT_SCREEN_PROXY_H_

