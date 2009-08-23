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
 * \file editScreenData.h
 * \brief connectionMgr - edit screen data handling
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _EDIT_SCREEN_DATA_H_
#define _EDIT_SCREEN_DATA_H_

#include <glib.h>
#include "commonData.h"

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    void edit_data_init(void);
    void edit_data_destroy(void);

    connection_t edit_data_get_network_type(void);
    gboolean edit_data_set_network_type(connection_t type);

    void edit_data_init_network_profiles(void);
    void edit_data_destory_network_profiles(void);
    void edit_data_store_network_profiles(void);

    int edit_data_get_n_network_profiles(void);
    const networkProfile_t* edit_data_get_network_profile(guint profileIndex);
    gboolean edit_data_set_network_profile(int profileIndex, 
                                       const networkProfile_t* pNetwork);

    int edit_data_get_profile_index(guint regIndex);
    int edit_data_get_reg_index(guint profileIndex);
   
    gboolean edit_data_reach_max_profiles(void);

    void edit_data_remove_network_profile(int profileIndex);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_EDIT_SCREEN_DATA_H_


