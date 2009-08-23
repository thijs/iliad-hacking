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


#include <string.h>
#include <sys/stat.h>

#include "scbpath.h"


void scb_path_init(ScbPathPtr ptr)
{
    if (ptr)
    {
        memset(ptr, 0, sizeof(ScbPath));
    }
}


gboolean scb_path_file_exist(const char *pathName)
{
    struct stat statbuf;
    if (0 != stat(pathName, &statbuf))
    {
        return FALSE;
    }
    if (S_ISREG(statbuf.st_mode))
    {
        return TRUE;
    }
    return FALSE;
}
