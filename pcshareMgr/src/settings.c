/*
 * This file is part of pcshareMgr.
 *
 * pcshareMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pcshareMgr is distributed in the hope that it will be useful,
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
 * \file settings.c
 * \brief Get PC share related settings
 * 
 * <File description>
 *
 */

/* downloadMgr - A GTK based application on the eReader platform
 *
 */

#include <stdlib.h>
#include <glib.h>

#include <libermanifest/ermanifest.h>

#include "logging.h"
#include "constants.h"
#include "settings.h"

// Construct default settings for PC share
static pcShareCfg_t* makeDefaultCfg()
{
    pcShareCfg_t* thePCShareSettings = NULL;

    thePCShareSettings = g_new0(pcShareCfg_t, 1);

    if (NULL == thePCShareSettings)
    {
        DL_ERRORPRINTF("Memory allocation failed.");
        g_assert_not_reached();
    }

    thePCShareSettings->overwrite = FALSE;
    return thePCShareSettings;
}

pcShareCfg_t* get_pc_share_settings()
{
    DL_LOGPRINTF("entry");

    // Open /mnt/share/iliad.xml
    char szCfgFilePath[MAX_PATH];
    snprintf(szCfgFilePath, sizeof(szCfgFilePath), "%s/%s", SHARE_MOUNT_POINT, CONFIG_FILE);

    erManifest manifest;
    if (RET_OK != ermXmlOpenFile(szCfgFilePath, &manifest))
    {
        DL_ERRORPRINTF("Could not open manifest file [%s]", szCfgFilePath);
        return makeDefaultCfg();
    }

    pcShareCfg_t* thePCShareSettings = NULL;
    thePCShareSettings = g_new0(pcShareCfg_t, 1);

    if (NULL == thePCShareSettings)
    {
        DL_ERRORPRINTF("Memory allocation failed.");
        g_assert_not_reached();
    }

    int ret = ermXmlGetBoolean(&manifest, OVERWRITE_SETTING_XPATH, &thePCShareSettings->overwrite);
    if (ret == RET_ERR)
    {
        thePCShareSettings->overwrite = FALSE;
    }

    ermXmlClose(&manifest);
    return thePCShareSettings;
}

void free_pc_share_settings(pcShareCfg_t* thePCShareSettings)
{
    DL_LOGPRINTF("entry");
    g_free(thePCShareSettings);
}

