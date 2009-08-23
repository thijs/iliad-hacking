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
 * \file search_files.h
 * \brief content lister - 
 * 
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2006-2008 iRex Technologies
 *
 */
 
#ifndef __SEARCH_FILES_H_
#define __SEARCH_FILES_H_

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    
#define MAX_TOKENS_IN_SEARCHSTRING  (20)

#define SEARCH_MAX_RECURSE          (25)
#define SEARCH_MAX_SYMLINKS         (100)

    
//execute the search and create the symbolic for the results.
int execSearch(const char* sPath, const char* sTarget);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
