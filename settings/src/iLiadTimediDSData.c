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
 * \file iLiadTimediDSData.c
 * \brief settings - timed-iDS connection settings data
 *
 * Handling timed-iDS connection settings data. 
 * 
 */

#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>

#include "setupLog.h"
#include "iLiadTimediDSData.h"
#include "settings.h"


// Global variables
static regTimedIds_t* g_timedids_settings = NULL;
static regTimedIds_t* g_stored_settings = NULL;

// Internal functions
static gboolean compare_settings(regTimedIds_t* settings_l, regTimedIds_t* settings_r);

void iLiad_timedids_data_init(void)
{
    ST_LOGPRINTF("entry");
    
    iLiad_timedids_data_destroy();
    
    g_timedids_settings = erRegGetTimedIds();
    if (NULL == g_timedids_settings)
    {
        ST_WARNPRINTF("use default values");
        
        g_timedids_settings = g_new0(regTimedIds_t, 1);
        g_timedids_settings->enable = FALSE;
        g_timedids_settings->timeCnt = 0;
        g_timedids_settings->timeSet[0] = 46800;
        g_timedids_settings->swUpdate = FALSE;
        g_timedids_settings->switchOff = FALSE;
    }
    else
    {
        g_stored_settings = g_new0(regTimedIds_t, 1);
        *g_stored_settings = *g_timedids_settings;
    }
}

void iLiad_timedids_data_destroy(void)
{
    ST_LOGPRINTF("entry");

    if (g_timedids_settings)
    {
        erRegFreeTimedIds(g_timedids_settings);
        g_timedids_settings = NULL;
    }
    if (g_stored_settings)
    {
        erRegFreeTimedIds(g_stored_settings);
        g_stored_settings = NULL;
    }
}

// Return TRUE if two settings are equal
gboolean compare_settings(regTimedIds_t* settings_l, regTimedIds_t* settings_r)
{
    if (settings_l->enable != settings_r->enable)
    {
        return FALSE;
    }

    if (settings_l->timeCnt != settings_r->timeCnt)
    {
        return FALSE;
    }

    int i;
    for (i=0; i<g_stored_settings->timeCnt; i++)
    {
        if (settings_l->timeSet[i] != settings_r->timeSet[i])
        {
            return FALSE;
        }
    }

    if (settings_l->swUpdate != settings_r->swUpdate)
    {
        return FALSE;
    }

    if (settings_l->switchOff != settings_r->switchOff)
    {
        return FALSE;
    }

    return TRUE;    
}

void iLiad_timedids_data_store(void)
{
    ST_LOGPRINTF("entry");
    g_return_if_fail(NULL != g_timedids_settings);
    
    // Check if anything changed
    if (g_stored_settings != NULL && compare_settings(g_stored_settings, g_timedids_settings))
    {
        // Nothing changed, nothing to save
        return;
    }

    prepare_registry_write();

    ST_STOREPRINTF("calling erRegSetTimediDS");
    erRegSetTimedIds(g_timedids_settings);

    *g_stored_settings = *g_timedids_settings;

    do_registry_write();
}

regTimedIds_t* get_current_timedids_settings(void)
{
    return g_timedids_settings;
}

