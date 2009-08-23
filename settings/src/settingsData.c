/*
 * This file is part of settings.
 *
 * settings is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * settings is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

/**
 * \file settingsData.c
 * \brief setup application - E-reader settings data handling                

 * <File description>
 * 
 */

#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>

#include "setupLog.h"
#include "settings.h"
#include "iLiadUserData.h"
#include "iLiadConfigData.h"
#include "iLiadBgConnectData.h"
#include "system.h"
#include "iLiadPincodeData.h"
#include "iLiadPCConnectData.h"
#include "iLiadArcLocData.h"
#include "iLiadStartUpData.h"
#include "iLiadTimediDSData.h"

void settings_data_init(void)
{
    ST_LOGPRINTF("entry");

    // get settings from registry
    iLiad_user_data_init();
    iLiad_config_data_init();
    iLiad_autoconnect_data_init();
    iLiad_pincode_data_init();
    iLiad_pc_connect_data_init();
    iLiad_archive_location_data_init();
    iLiad_startup_data_init();
    iLiad_timedids_data_init();
}

void settings_data_destroy(void)
{
    ST_LOGPRINTF("entry");
    iLiad_config_data_destroy();
    iLiad_pincode_data_destroy();
    iLiad_pc_connect_data_destroy();
    iLiad_archive_location_data_destroy();
    iLiad_startup_data_destroy();
    iLiad_timedids_data_destroy();
}


void settings_data_store_page(gint page)
{
    switch ( page )
    {
        case iLiad_user_screen_t:
            ST_STOREPRINTF("store iLiad_user_profile values");
            iLiad_user_data_store();
            break;
        case iLiad_config_screen_t:
            ST_STOREPRINTF("store iLiad_config values");
            iLiad_config_data_store();
            break;
        case iLiad_pc_connect_screen_t:
            ST_STOREPRINTF("store iLiad_pc_connect values");
            iLiad_pc_connect_data_store();
            break;
        case iLiad_autoconnect_screen_t:
            ST_STOREPRINTF("store iLiad_autoconnect values");
            iLiad_autoconnect_data_store();
            break;
        case iLiad_pincode_screen_t:
            ST_STOREPRINTF("store iLiad_pincode values");
            iLiad_pincode_data_store();
            break;
        case iLiad_archive_location_screen_t:
            ST_STOREPRINTF("store iLiad_archive_location values");
            iLiad_archive_location_data_store();
            break;
        case iLiad_startup_screen_t:
            ST_STOREPRINTF("store iLiad_startup values");
            iLiad_startup_data_store();
            break;
        case iLiad_timedids_screen_t:
            ST_STOREPRINTF("store timed-iDS connection settings values");
            iLiad_timedids_data_store();
            break;
        case iLiad_datetime_screen_t:
            ST_STOREPRINTF("store timezone settings -- do nothing");
            break;
        case iLiad_device_overview_screen_t:
            ST_STOREPRINTF("store iLiad_device_overview values -- do nothing");
            break;
        default:
            ST_ERRORPRINTF("page [%d] is UNKNOWN", page);
            //terminate application if this point is reached
            g_assert_not_reached();
    }
}

void settings_data_store_currentpage()
{
    gint current_page = settings_get_current_page(NULL);
    
    settings_data_store_page(current_page);
}

