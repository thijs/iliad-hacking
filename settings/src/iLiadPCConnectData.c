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
 * \file iLiadPCConnectData.c
 * \brief setup application - E-reader PCConnect data handling (registry interaction)               

 * <File description>
 * 
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <unistd.h>

#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>

#include "setupLog.h"
#include "iLiadPCConnectData.h"
#include "settings.h"


// PC-share settings
static regPCConfig_t *g_pc_connect_config = NULL;
static regPCProfile_t **g_pc_connect_settings = NULL;

static gboolean g_pc_connect_changed = FALSE;

// local functions
static guint iLiad_pc_connect_add_profile(void);
//static guint iLiad_pc_connect_remove_profile(guint profile_index);


void iLiad_pc_connect_data_init(void)
{
    regPCProfile_t* setting = NULL;

    ST_LOGPRINTF("entry");

    iLiad_pc_connect_data_destroy();

    // read all pc connect profiles from registry
    erRegGetPCProfiles(&g_pc_connect_config, &g_pc_connect_settings);

    // make sure at least one profile is present
    if (g_pc_connect_config->size == 0)
    {
        ST_WARNPRINTF("no PC profiles, create one empty profile");
        iLiad_pc_connect_add_profile();
        setting = g_new0(regPCProfile_t, 1);
        g_assert(setting != NULL);
        setting->name      = strdup("");
        setting->pcname    = strdup("");
        setting->sharename = strdup("");
        setting->workgroup = strdup("");
        setting->username  = strdup("");
        setting->password  = strdup("");
        g_pc_connect_settings[0] = setting;
    }
}

void iLiad_pc_connect_data_destroy(void)
{
    ST_LOGPRINTF("entry");

    if (g_pc_connect_config)
    {
        erRegFreePCProfiles(g_pc_connect_config, g_pc_connect_settings);
        g_pc_connect_settings = NULL;
        g_pc_connect_config = NULL;
    }
}

const regPCProfile_t *iLiad_pc_connect_get_profiledata(const gint profile_index)
{
    ST_LOGPRINTF("entry: index [%d]", profile_index);

    g_return_val_if_fail((g_pc_connect_config != NULL), NULL);

    if (profile_index < g_pc_connect_config->size)
    {
        return g_pc_connect_settings[profile_index];
    }
    else
    {
        return NULL;
    }
}

gboolean iLiad_pc_connect_set_profile_field(gint profile_index, pc_fieldtype_t field, const char* value)
{
    gchar**  cpp;

    ST_LOGPRINTF("entry: index [%d] field [%d] value [%s] size [%d]", profile_index, field, value, g_pc_connect_config->size);

    g_return_val_if_fail((g_pc_connect_config != NULL), FALSE);
    g_return_val_if_fail((profile_index < g_pc_connect_config->size), FALSE);

    if (value == NULL)
    {
        value = "";
    }

    // remember new value
    switch (field)
    {
        case e_pcfield_profile_name:
            cpp = &(g_pc_connect_settings[profile_index]->name);
            break;
        case e_pcfield_pcname:
            cpp = &(g_pc_connect_settings[profile_index]->pcname);
            break;
        case e_pcfield_sharename:
            cpp = &(g_pc_connect_settings[profile_index]->sharename);
            break;
        case e_pcfield_workgroup:
            cpp = &(g_pc_connect_settings[profile_index]->workgroup);
            break;
        case e_pcfield_username:
            cpp = &(g_pc_connect_settings[profile_index]->username);
            break;
        case e_pcfield_password:
            cpp = &(g_pc_connect_settings[profile_index]->password);
            break;
        default:
            g_assert_not_reached();
            return FALSE;
    }
    if (*cpp)
    {
        g_free(*cpp);
    }
    *cpp = g_strdup(value);

    g_pc_connect_changed = TRUE;
    return TRUE;
}

void iLiad_pc_connect_data_store(void)
{
    ST_LOGPRINTF("entry");

    if (g_pc_connect_changed == FALSE)
    {
        // nothing to save
        ST_LOGPRINTF("nothing to save");
        return;
    }

    // write our pc profiles to registry
    prepare_registry_write();

    const regPCProfile_t** p = (const regPCProfile_t**)g_pc_connect_settings;  // const_cast
    erRegSetPCProfiles(g_pc_connect_config, p);
   
    g_pc_connect_changed = FALSE;

    do_registry_write();
}

static guint iLiad_pc_connect_add_profile()
{
    ST_LOGPRINTF("entry");

    guint profile_index = g_pc_connect_config->size;

    // add one pc ID
    gchar **newList = g_renew(gchar *, g_pc_connect_config->pcList, profile_index + 1);
    g_pc_connect_config->pcList = newList;
    g_pc_connect_config->pcList[profile_index] = g_strdup_printf("PC_%d", profile_index);

    // add pc setting
    regPCProfile_t **newSettings = g_renew(regPCProfile_t *, g_pc_connect_settings, profile_index + 1);
    g_pc_connect_settings = newSettings;
    g_pc_connect_settings[profile_index] = NULL;
    g_pc_connect_config->size = profile_index + 1;

    // report number of profiles
    ST_LOGPRINTF("g_pc_connect_config->size=%d", g_pc_connect_config->size);
    return g_pc_connect_config->size;
}


/**** Not used right now, need this when implementing multiple PCshare profiles ****
 *
static guint iLiad_pc_connect_remove_profile(guint profile_index)
{
    ST_LOGPRINTF("entry: index [%d]", profile_index);

    int i;
    guint num_profiles = g_pc_connect_config->size;
    g_return_val_if_fail((profile_index < num_profiles), num_profiles);

    // remove the last pc ID
    g_free(g_pc_connect_config->pcList[num_profiles - 1]);
    g_pc_connect_config->pcList[num_profiles - 1] = NULL;

    // remove the specified pc setting
    g_free(g_pc_connect_settings[profile_index]);
    for (i = profile_index; i < num_profiles - 1; i++)
    {
        g_pc_connect_settings[i] = g_pc_connect_settings[i + 1];
    }
    g_pc_connect_settings[num_profiles - 1] = NULL;

    // update profile count and report
    g_pc_connect_config->size = num_profiles - 1;
    ST_LOGPRINTF("g_pc_connect_config->size=%d", g_pc_connect_config->size);
    
    return g_pc_connect_config->size;
}
****/

