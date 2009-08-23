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
 * \file iLiadAutoconnectData.c
 * \brief setup application - E-reader Auto-connect screen data handling (registry interaction)               
 * 
 */

#include <gtk/gtk.h>
#include <string.h>

#include <liberregxml/erregapi.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "iLiadBgConnectScreen.h"
#include "iLiadBgConnectData.h"
#include "languages.h"
#include "settings.h"


static regAutoConnect_t* g_autoconnect = NULL;
static regAutoConnect_t* g_stored_autoconnect = NULL;

static gboolean g_ac_wlan = TRUE;
static gboolean g_stored_ac_wlan = TRUE;

void iLiad_autoconnect_data_init(void)
{
    ST_LOGPRINTF("entry");

    iLiad_autoconnect_data_destroy();

    g_autoconnect = erRegGetAutoConnect();
    if (g_autoconnect == NULL)
    {
        ST_WARNPRINTF("use default values");
        g_autoconnect = g_new0(regAutoConnect_t, 1);

        g_autoconnect->backgroundEnable  = FALSE;
    }
    else
    {
        g_stored_autoconnect  = g_new0(regAutoConnect_t, 1);
        *g_stored_autoconnect = *g_autoconnect;
    }

    g_ac_wlan = erRegGetAutoConnectWlan();
    g_stored_ac_wlan = g_ac_wlan;
}

void iLiad_autoconnect_data_destroy(void)
{
    ST_LOGPRINTF("entry");

    if (g_autoconnect)
    {
        g_free(g_autoconnect);
        g_autoconnect = NULL;
    }

    if (g_stored_autoconnect)
    {
        g_free(g_stored_autoconnect);
        g_stored_autoconnect = NULL;
    }
}

void iLiad_autoconnect_data_display(void)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(g_autoconnect != NULL);

    iLiad_ac_set_wlan_onoff(g_ac_wlan);
    iLiad_ac_set_background_onoff(g_autoconnect->backgroundEnable);
    iLiad_ac_set_background_idspc(g_autoconnect->backgroundConnectTo);
    iLiad_ac_set_background_interval(g_autoconnect->backgroundInterval);

    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_FULLSCREEN);
}

// callback functions
void iLiad_ac_wlan_onoff_changed(gboolean enable)
{
    g_ac_wlan = enable;
}

void iLiad_ac_background_onoff_changed(gboolean enable)
{
    g_return_if_fail(NULL != g_autoconnect);

    g_autoconnect->backgroundEnable = enable;
}

void iLiad_ac_background_idspc_changed(gint connect_to)
{
    g_return_if_fail(NULL != g_autoconnect);

    g_autoconnect->backgroundConnectTo = connect_to;
}

void iLiad_ac_background_interval_changed(gboolean interval)
{
    g_return_if_fail(NULL != g_autoconnect);

    g_autoconnect->backgroundInterval = interval;
}

//store the autoconnect information in the registry
void iLiad_autoconnect_data_store(void)
{
    gboolean changedAutoConnect = TRUE;
    gboolean changedAutoConnectWlan = TRUE;

    ST_LOGPRINTF("entry");

    g_return_if_fail(NULL != g_autoconnect);

    if (   g_stored_autoconnect != NULL
        && g_autoconnect->backgroundEnable    == g_stored_autoconnect->backgroundEnable
        && g_autoconnect->backgroundConnectTo == g_stored_autoconnect->backgroundConnectTo
        && g_autoconnect->backgroundInterval  == g_stored_autoconnect->backgroundInterval)
    {
        changedAutoConnect = FALSE;
    }

    if (g_ac_wlan == g_stored_ac_wlan)
    {
        changedAutoConnectWlan = FALSE;
    }

    if (!changedAutoConnect && !changedAutoConnectWlan)
    {
        // Nothing changed, nothing to save
        return;
    }

    prepare_registry_write();

    if (changedAutoConnect)
    {
        ST_STOREPRINTF("calling erRegSetAutoConnect");
        erRegSetAutoConnect(g_autoconnect);
        g_free(g_stored_autoconnect);
        g_stored_autoconnect  = g_new0(regAutoConnect_t, 1);
        *g_stored_autoconnect = *g_autoconnect;
    }

    if (changedAutoConnectWlan)
    {
        ST_STOREPRINTF("calling erRegSetAutoConnectWlan");
        erRegSetAutoConnectWlan(g_ac_wlan);
    }

    do_registry_write();
}


