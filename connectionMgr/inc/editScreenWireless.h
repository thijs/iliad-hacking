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
 * \file editScreenWireless.h
 * \brief connectionMgr - "Wireless Configuration" screen.
 *
 * Copyright (C) 2007 iRex Technologies BV.
 * 
 */

#ifndef _EDIT_SCREEN_WIRELESS_H_
#define _EDIT_SCREEN_WIRELESS_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <glib.h>

#include <liberregxml/erregapi.h>

#ifdef __cplusplus
extern "C"
{
#endif

    GtkWidget *edit_wireless_create(void);
    void edit_wireless_enable_save_button(gboolean enable);
    void edit_wireless_set_text(void);
    void edit_wireless_determine_status(void);
    void delay_edit_wireless_determine_status(void);

    void edit_wireless_set_network_settings(const regNetworkProfile_t *setting);
    void edit_wireless_get_network_settings(regNetworkProfile_t *settings);
    gboolean edit_wireless_check_network_settings(void);
    void edit_wireless_backup_network_settings(void);
    void edit_wireless_restore_network_settings(void);

    void edit_wireless_set_ssid(const char *ssid);
    void edit_wireless_set_profilename(const gchar *profilename);
    void edit_wireless_set_encrtype(wireless_encr_type_t encrtype);
    void edit_wireless_set_encrkey(const char *encrkey);
    
    const char *edit_wireless_get_ssid(void);
    wireless_encr_type_t edit_wireless_get_encrtype(void); 
    const char *edit_wireless_get_profilename(void);
    const char *edit_wireless_get_encrkey(void);

    gboolean on_edit_wireless_keypress(GdkEventKey* event);

#ifdef __cplusplus
}
#endif

#endif //_EDIT_SCREEN_WIRELESS_H_

