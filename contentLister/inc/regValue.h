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
 * \file regValue.h
 * \brief content lister - registry values                  
 * 
 * <File description>
 * 
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */


#ifndef __CL_REGVALUE__H__
#define __CL_REGVALUE__H__

#ifdef __cplusplus
extern  "C" {
#endif  /* __cplusplus */

#include <liberregxml/erregapi.h>

// program arguments
#define FILENAME_ARG           "_FILENAME_"
#define MANIFEST_ARG           "_MANIFEST_"

// registry functions (in main.c)
void reload_registry(void);
void prepare_registry_write( void);
void do_registry_write(void);

// startup behaviour from command-line (in main.c)
startup_behaviour_t main_get_startup_behaviour(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  //__CL_REGVALUE__H__
