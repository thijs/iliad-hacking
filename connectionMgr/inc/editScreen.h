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
 * \file editScreen.h
 * \brief connectionMgr - the entry of 'profile-editor' screen
 *                        containing a editScreenProfiles
 *                               and a editScreenSearch
 *                               and a editScreenOverview
 *                               and a editScreenWireless
 *                               and a editScreenWirelessExt
 *                               and a editScreenStatic
 *                               and a editScreenProxy
 *                        screen handling and event processing
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _EDIT_SCREEN_H_
#define _EDIT_SCREEN_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>
#include <liberregxml/erregapi.h> 

#include "commonData.h"
#include "connectPing.h"

// how to launch profile editor
typedef enum
{
    // launch profile editor normally(from device manager)
    editModeNormal_e = 0,  
    // launch profile editor from connect screen
    // (by clicking the 'Profile editor' button
    editModeConnectNormal_e,
    // to create profile from connect screen
    // (by clicking the profile entry for a open-secured access point)
    editModeConnectCreate_e,   
    // launch profile editor from connect screen
    // (in order to fill in some fields 
    // for open access point with encryption)
    editModeConnectFillIn_e, 
    editModeUndef_e
} editMode_t;

// these values correspond with the notebook page numbers
typedef enum
{
    editScreenProfiles_e = 0,
    editScreenSearch_e,
    editScreenOverview_e,
    editScreenWireless_e,
    editScreenWirelessExt_e,
    editScreenDialup_e,
    editScreenStatic_e,
    editScreenProxy_e,
    editScreenUndefined_e
} editScreen_t;

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    GtkWidget *edit_screen_create(void);
    void       edit_screen_set_text(void);
    void       edit_screen_display(void);

    // get/set the flags
    editMode_t edit_get_mode(void);
    void       edit_set_mode(editMode_t mode);

    gboolean edit_get_wizard_mode(void);
    void     edit_set_wizard_mode(gboolean mode);

    // get/set the network settings
    gboolean     edit_get_in_set_network_settings(void);
    void         edit_set_in_set_network_settings(gboolean value);

    gboolean edit_get_back_overview(void);
    void     edit_set_back_overview(gboolean back);

    regNetworkProfile_t* edit_get_network_settings(void);
    void edit_set_network_settings(const regNetworkProfile_t* settings);

    void edit_restore_network_settings(int page);

    // save the profile
    void edit_save_profile(void);

    // switch page management
    gint edit_get_current_page(gint *pagecount);

    void edit_goto_page(gint new_page);
    void edit_set_pagebar(void);

    // toolbar and keyboard management
    void edit_set_toolbar(void);
    void edit_show_trashcan(void);

    // signal handlers 
    void     on_edit_goto_page(gint new_page);
    gboolean on_edit_keypress(GdkEventKey* event);
    void     on_edit_icon_clicked(int iconID, int iconState);
    
    // callbacks for pingContext_t
    int edit_get_initial_profile(void);
    int edit_get_n_network_profiles(void);
    const networkProfile_t* edit_get_network_profile(int profileIndex);
    gboolean edit_select_next_regprofile(int profileIndex);

    void edit_on_connected(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_EDIT_SCREEN_H_

