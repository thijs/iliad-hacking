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
 * \file editScreenDialup.h
 * \brief connectionMgr - "Dialup" screen
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _EDIT_SCREEN_DIALUP_H_
#define _EDIT_SCREEN_DIALUP_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <glib.h>
#include <liberregxml/erregapi.h>

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    GtkWidget *edit_dialup_create(void);
    void edit_dialup_enable_save_button(gboolean enable);
    void edit_dialup_set_text(void);

    void edit_dialup_set_network_settings(const regNetworkProfile_t *settings);
    void edit_dialup_get_network_settings(regNetworkProfile_t *settings);
    gboolean edit_dialup_check_network_settings(void);
    void edit_dialup_restore_network_settings(void);

    void edit_dialup_set_profilename(const char *profilename);
    const char *edit_dialup_get_profilename(void);

    gboolean on_edit_dialup_keypress(GdkEventKey *event); 

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_EDIT_SCREEN_DIALUP_H_

