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
 * \file connectPing.h
 * \brief connectionMgr -According to the results of scanning networks,
 *                     use the profile one by one to ping until it succeeds.
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _CONNECT_PING_H_
#define _CONNECT_PING_H_

#include <glib.h>
#include "commonData.h"
#include "pingThread.h"

typedef enum
{
    backgroundPing_e = 0,
    connectPing_e,
    editPing_e,
    undefPingMode_e
}pingMode_t;

typedef enum
{
    pingSucceeded_e=0,
    pingFailed_e,
    pingFailedPCShare_e,
    pingFailedNetwork_e,
    pingSkipped_e,
    pingNeedKey_e,
    pingAborting_e,
    pingAborted_e,
    pingConnecting_e,
    undefPingStatus_e
}pingStatus_t;

// select the pinging start profile
typedef int get_initial_profile_t(void);

// access the data of network profiles
typedef int get_n_network_profiles_t(void);
typedef const networkProfile_t* get_network_profile_t(int profileIndex);
typedef gboolean select_next_regprofile_t(int profileIndex); 

// update the ui 
typedef void ui_update_status_t(int profileIndex, pingStatus_t status);
typedef void ui_display_settings_t(int profileIndex, gboolean resetStatus);
typedef void ui_select_profile_t(int profileIndex);
typedef void ui_unselect_all_profile_t(void);

// ping done callbacks
typedef void on_connected_t(int profileIndex);
typedef void on_failed_all_t(void);

typedef struct
{
    pingMode_t        mode;
    cmgrConnectType_t connectType;   // pc or ids

    ping_thread_parms* pingParms;    // representative of the ping-thread

    // private data
    gboolean pingServer;
    guint    pingTimeoutId;
    gboolean pingAborting;

    int    selectedProfileIndex;     // current selected profile index 
    int    pingingProfileIndexStart; // which profile index is to start pinging 
    int    pingingProfileIndex;      // current pinging profile index

    get_n_network_profiles_t* get_n_network_profiles;
    get_network_profile_t* get_network_profile;
    select_next_regprofile_t* select_next_regprofile; 

    get_initial_profile_t* get_initial_profile;

    ui_update_status_t* ui_update_status;
    ui_display_settings_t* ui_display_settings;
    ui_select_profile_t* ui_select_profile;
    ui_unselect_all_profile_t* ui_unselect_all_profile;

    on_connected_t* on_connected;
    on_failed_all_t* on_failed_all;
}pingContext_t;

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    pingContext_t* connect_ping_ctxt_new(void);

    void connect_ping_ctxt_set_mode(pingContext_t* ctxt, pingMode_t mode);
    void connect_ping_ctxt_set_connect_type(pingContext_t* ctxt, 
                                            cmgrConnectType_t connectType);

    void connect_ping_ctxt_set_get_initial_profile(pingContext_t* ctxt, 
                               get_initial_profile_t* get_initial_profile);

    void connect_ping_ctxt_set_access_network_profiles_callbacks(pingContext_t* ctxt,
                        get_n_network_profiles_t* get_n_network_profiles,
                        get_network_profile_t* get_network_profile,
                        select_next_regprofile_t* select_next_regprofile);

    void connect_ping_ctxt_set_ui_callbacks(pingContext_t* ctxt,
                    ui_update_status_t* ui_update_status,
                    ui_display_settings_t* ui_display_settings,
                    ui_select_profile_t* ui_select_profile,
                    ui_unselect_all_profile_t* ui_unselect_all_profile);

    void connect_ping_ctxt_set_done_callbacks(pingContext_t* ctxt, 
                                          on_connected_t* on_connected,
                                          on_failed_all_t* on_failed_all);

    void connect_ping_ctxt_destory(pingContext_t* ctxt);

    // 
    void connect_ping(pingContext_t* ctxt, gboolean delay);
    void connect_ping_abort(pingContext_t* ctxt);

    int connect_ping_get_pinging_profile_index_start(pingContext_t* ctxt);
    void connect_ping_set_pinging_profile_index_start(pingContext_t* ctxt, 
                                                      int profileIndex);

    int connect_ping_get_pinging_profile_index(pingContext_t* ctxt);
    void connect_ping_set_pinging_profile_index(pingContext_t* ctxt, 
                                                int profileIndex);

    int connect_ping_get_selected_profile_index(pingContext_t* ctxt);
    void connect_ping_set_selected_profile_index(pingContext_t* ctxt, 
                                                 int profileIndex);

    void connect_ping_select_initial_profile (pingContext_t* ctxt);

    // check if in ping to server state
    gboolean connect_ping_in_pinging(pingContext_t *ctxt);
    // while aborting ping thread, need to freeze the ui
    gboolean connect_ping_freeze_ui(pingContext_t* ctxt);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_CONNECT_PING_H_

