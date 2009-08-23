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
 * \file settings.h
 * \brief Get PC share related settings
 * 
 * <File description>
 *
 */

/* downloadMgr - A GTK based download manager application on the eReader platform
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef __SETTINGS__H__
#define __SETTINGS__H__

#define SHARE_MOUNT_POINT    "/mnt/share"
#define CONFIG_FILE          "er_ctrl.xml"
#define OVERWRITE_STRING_LEN 32

#define OVERWRITE_SETTING_XPATH "/ereaderControl/pcShare/download/overwriteItems"

typedef struct
{
    gboolean overwrite;
}pcShareCfg_t;

#ifdef __cplusplus
extern  "C"
{
#endif /* __cplusplus */

pcShareCfg_t* get_pc_share_settings();
void free_pc_share_settings(pcShareCfg_t* thePCShareSettings);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__SETTINGS__H__
