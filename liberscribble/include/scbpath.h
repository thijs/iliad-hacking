/*
 * This file is part of liberscribble.
 *
 * liberscribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberscribble is distributed in the hope that it will be useful,
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

#ifndef SCBPATH_H_
#define SCBPATH_H_

#include "scbconfig.h"
#include <glib.h>

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct _ScbPath
{
    char    dirname[SCB_MAX_PATH];
    char    manifest[SCB_MAX_PATH];
    char    scbname[SCB_MAX_PATH];
}ScbPath ;

typedef ScbPath* ScbPathPtr;
void scb_path_init(ScbPathPtr ptr);
gboolean scb_path_file_exist(const char *pathName);


#ifdef __cplusplus
}
#endif

#endif

