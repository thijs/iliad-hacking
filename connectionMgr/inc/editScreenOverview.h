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
 * \file editScreenOverview.c
 * \brief connectionMgr - edit overview screen
 *
 * Copyright (C) 2007 iRex Technologies BV.
 * 
 */

#ifndef _EDIT_SCREEN_OVERVIEW_H_
#define _EDIT_SCREEN_OVERVIEW_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <glib.h>

#include <liberregxml/erregapi.h>

typedef enum
{
    inputDetails_e = 0,
    inputAddress_e,
    inputProxy_e,
    inputProfileName_e,
    testing_e,
    testAborting_e,
    testAborted_e,
    testSucceeded_e,
    testFailed_e,
    connecting_e,
    connectAborting_e,
    connectAborted_e,
    connectSucceeded_e,
    connectFailed_e,
    toSave_e,
    toSaveConnect_e,
    undefinedStatus_e
}overviewStatus_t;

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    GtkWidget *edit_overview_create(void);
    void      edit_overview_set_text(void);
    
    void edit_overview_show_profilename(void);
    void edit_overview_update_info_text(overviewStatus_t status);
    void edit_overview_enable_save_button(gboolean enable);
    void edit_overview_update_profile(void);

    void edit_overview_set_network_settings(const regNetworkProfile_t *settings);
    void edit_overview_get_network_settings(regNetworkProfile_t *settings);

    void edit_overview_set_profile(const regNetworkProfile_t *settings);
    void edit_overview_set_address(const ip_address_mode_t addressMode);
    void edit_overview_set_proxy(const gboolean use_proxy);
    void edit_overview_set_profilename(const gchar *profilename);

    void edit_overview_click_test(void);
    void edit_overview_click_save(void);
    void edit_overview_click_saveconnect(void);

    gboolean on_edit_overview_keypress(GdkEventKey *event);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_EDIT_SCREEN_OVERVIEW_H_

