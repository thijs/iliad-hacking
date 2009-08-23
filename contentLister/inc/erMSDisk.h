/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
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

#ifndef _ERMSDISK_H
#define _ERMSDISK_H

#include "stack.h"

#define CMD_LEN 32
#define PROCFS_MOUNT_FILE           "/proc/mounts"

typedef enum
{
    expMain = 0,
    expMMC,
    expUSB,
    expCF,
    expUndefined
} expMemType_e;

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

void erMSDiskInit();
void erStartMSDiskApp();
void erStopMSDiskApp();
int erMSDiskSetConnected(int connected);
expMemType_e get_exported_memory_type(void);
inline expMemType_e get_memory_type_used();

// Handle card plug in/out
void erMSDisk_storage_changed(st_ContentType_e storageType, gboolean storagePresent);

// Handle settings changed
void erMSDisk_handle_reread_settings();

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
