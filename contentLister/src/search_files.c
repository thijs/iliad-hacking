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
 * \file search_files.c
 * \brief search - eReader search implementation.
 * 
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2006-2008 iRex Technologies
 * All rights reserved.
 *
 */

#define _GNU_SOURCE

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "metadataStoreManager.h"
#include "search_files.h"
#include "system.h"



int FileExist(const char* filename)
{
    struct stat buf;

    if(NULL==filename) return 0;

    return ( 0==stat(filename, &buf) );
}

static void DirDel(const char* sDir)
{
    char* cp;
    char* argv[10];
    int   argc;
   
    argc = 0;
    argv[argc++] = "rm";
    argv[argc++] = "-rf";
    //
    cp = alloca( strlen(sDir) + 1 );
    g_assert(cp != NULL);
    strcpy(cp, sDir);
    argv[argc++] = cp;
    //
    argv[argc] = NULL;
    g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
    fork_exec(argc, argv);
}

//get last char of directory name, if it's "/" ,return the location, 
//else return -1;
int getEndSepratorLocation(const char* sDir)
{
    if( NULL==sDir) return -1;
    
    int iLastPos=strlen(sDir)-1;
    if( '/' == sDir[iLastPos] )
    {
        return iLastPos;
    }
    return -1;
}


static int searchDirectory(const char* sPath, char** ssSearchTokens, const char* sResultDir)
{
    CL_SEARCHPRINTF("entry: path [%s] token [%s] [%s] [%s] resultDir [%s]", sPath, ssSearchTokens[0], ssSearchTokens[1], ssSearchTokens[2], sResultDir);

    DIR* dirp = opendir(sPath);
    if (dirp == NULL)
    {
        CL_ERRORPRINTF("Cannot open dir [%s] - %s", sPath, strerror(errno));
        return -1;  // error
    }

    static int recurse_level = 0;    // nesting level of recursive calls
    static int symlinks_num  = 0;    // number of symlinks created

    if (recurse_level > SEARCH_MAX_RECURSE)
    {
        CL_ERRORPRINTF("Search level too high [%d] sPath [%s]", recurse_level, sPath);
        return -1;  // error
    }
    if (recurse_level == 0)
    {
        symlinks_num = 0;
    }
    recurse_level++;

    int             i;
    char*           cp;
    char**          token_ptr;
    gboolean        patternMatch;
    struct dirent*  direntp;
    struct stat     statBuf;
    char            buf[ERMDS_MAX_FILENAME_SIZE];
    char            resolvedPath[PATH_MAX];
    char            fileName[ERMDS_MAX_FILENAME_SIZE];
    char*           extension;

    while ( (direntp = readdir(dirp)) != NULL)
    {
        // skip hidden entries and current/parent directory
        if (   direntp->d_name[0] == '.'
            || direntp->d_name[0] == '_' )
        {
            continue;
        }

        // convert symlink to the file or directory behind it
        snprintf(buf, sizeof(buf), "%s/%s", sPath, direntp->d_name);
        cp = realpath(buf, resolvedPath);
        if (cp == NULL)
        {
            CL_ERRORPRINTF("Cannot resolve path [%s] - %s", buf, strerror(errno));
            continue;  // while
        }
        CL_SEARCHPRINTF("resolved path [%s]", resolvedPath);
        //
        // set a pointer to the filename
        cp = strrchr(resolvedPath, '/');
        g_assert(cp != NULL);
        snprintf(fileName, sizeof(fileName), "%s", cp + 1);
        CL_SEARCHPRINTF("filename [%s]", fileName);

        // determine what kind of directory entry this really is
        if (stat(resolvedPath, &statBuf) == 0)
        {
            // for file or directory check for pattern match
            if ( S_ISREG(statBuf.st_mode)  ||  S_ISDIR(statBuf.st_mode) )
            {
                patternMatch = FALSE;
                for (token_ptr = ssSearchTokens ; *token_ptr != NULL ; token_ptr++)
                {
                    cp = strcasestr(fileName, *token_ptr);
                    if (cp)
                    {
                        patternMatch = TRUE;
                    }
                }

                if (patternMatch)
                {
                    // pattern match -> create symlink
                    if (symlinks_num >= SEARCH_MAX_SYMLINKS)
                    {
                        CL_ERRORPRINTF("too many search results [%d] path [%s]", symlinks_num, resolvedPath);
                    }
                    else
                    {
                        symlinks_num++;

                        // symlink name is filename
                        snprintf(buf, sizeof(buf), "%s/%s", sResultDir, fileName);
                        cp = strrchr(fileName, '.');
                        if (cp)
                        {
                            *cp = '\0';
                            extension = cp + 1;
                        }
                        else
                        {
                            extension = NULL;
                        }
                        // add a sequence number in case of duplicate filenames
                        for (i = 1 ; FileExist(buf)  && i <= 99 ; i++)
                        {
                            if (extension)
                            {
                                snprintf(buf, sizeof(buf), "%s/%s_%02d.%s", sResultDir, fileName, i, extension);
                            }
                            else
                            {
                                snprintf(buf, sizeof(buf), "%s/%s_%02d", sResultDir, fileName, i);
                            }
                        }

                        // now create the symlink
                        if ( 0 == symlink(resolvedPath, buf) )
                        {
                            CL_SEARCHPRINTF("Created symlink [%s] -> [%s]", buf, resolvedPath);
                        }
                        else
                        {
                            CL_ERRORPRINTF("Cannot create symlink [%s] -> [%s] - %s", buf, resolvedPath, strerror(errno));
                        }
                    }
                }
            }

            // recursively search directories that have NO manifest file
            if ( S_ISDIR(statBuf.st_mode) )
            {
                // check whether directory holds a manifest file
                snprintf(buf, sizeof(buf), "%s/%s", resolvedPath, MANIFEST_FILENAME);
                if ( !FileExist(buf) )
                {
                    // no manifest file -> scan recursively
                    cp = strrchr(buf, '/');
                    g_assert(cp != NULL);
                    *cp = '\0';
                    searchDirectory(buf, ssSearchTokens, sResultDir);
                }
            }
        }
    }
    closedir(dirp);

    --recurse_level;
    return 0;  // ok
}

int execSearch(const char* sPath, const char* sPattern)
{
    int   i;
    char* cp;
    
    CL_SEARCHPRINTF("entry: path [%s] pattern [%s]", sPath, sPattern);
    
    gchar dir_searchresult[ERMDS_MAX_FILENAME_SIZE]; 
    mdsGetRootLocation(st_SearchResult, dir_searchresult, sizeof(dir_searchresult));
    CL_SEARCHPRINTF("dir_searchresult [%s]", dir_searchresult);

    gchar dir_searchresult_tmp[ERMDS_MAX_FILENAME_SIZE]; 
    snprintf(dir_searchresult_tmp, sizeof(dir_searchresult_tmp), "%s_tmp", dir_searchresult);
    CL_SEARCHPRINTF("dir_searchresult_tmp [%s]", dir_searchresult_tmp);

    const char* sSearchPath = NULL;    
    int         nRet = -1;  // 0 = ok, -1 = error    
    
    // check whether we are searchig in previous search results
    if( 0 == strncmp(sPath, dir_searchresult, sizeof(dir_searchresult)) ) 
    {
        // searching in search-results: move old results to temporary location
        rename(dir_searchresult, dir_searchresult_tmp);
        sSearchPath = dir_searchresult_tmp;
        CL_SEARCHPRINTF("search path [%s]", sSearchPath);
    }
    else
    {
        // searching elsewhere: discard old results
        DirDel(dir_searchresult);
        sSearchPath = sPath;
        CL_SEARCHPRINTF("search path [%s]", sSearchPath);
    }
    CL_SEARCHPRINTF("search path [%s]", sSearchPath);
    
    // create directory to hold the search result
    if( 0 != mkdir(dir_searchresult, 0755) )
    {
         CL_ERRORPRINTF("Could not create dir: [%s]",dir_searchresult);
         return nRet;
    }

    // split search pattern into multiple tokens
    //     make a working copy of sPattern
    char* pattern = alloca( strlen(sPattern) + 1 );
    g_assert(pattern != NULL);
    strcpy(pattern, sPattern);
    //     find tokens
    char* tokens[MAX_TOKENS_IN_SEARCHSTRING + 1];
    int   n_tokens = 0;
    cp = pattern;
    while (*cp)
    {
        if (*cp == ' ')
        {
            // separator: ignore
            cp++;
        }
        else
        {
            // start of new token
            if (n_tokens < MAX_TOKENS_IN_SEARCHSTRING)
            {
                if (*cp == '"'  &&  *(cp+1) != '"')
                {
                    // token is a quoted string, e.g. "this is a string"
                    cp++;
                    tokens[n_tokens++] = cp;
                    while (*cp != '"'  &&  *cp != '\0')
                    {
                        // character is part of token: next character
                        cp++;
                    }
                    if (*cp)
                    {
                        *cp = '\0';
                        cp++;
                    }
                }
                else
                {
                    // token is a single word, e.g. "word"
                    tokens[n_tokens++] = cp;
                    while (*cp != ' '  &&  *cp != '\0')
                    {
                        // character is part of token: next character
                        cp++;
                    }
                    if (*cp)
                    {
                        *cp = '\0';
                        cp++;
                    }
                }
            }
        }
    }
    if (n_tokens == 0)
    {
        tokens[n_tokens++] = "";
    }
    tokens[n_tokens] = NULL;

    CL_SEARCHPRINTF("Search string [%s]", sPattern);
    for (i = 0 ; i <= n_tokens ; i++)
    {
        CL_SEARCHPRINTF("Search token [%d] [%s]", i, tokens[i]);
    }
    
    // now search for it
    nRet = searchDirectory(sSearchPath, tokens, dir_searchresult);

    // clean-up temporary links
    DirDel(dir_searchresult_tmp);

    return nRet;
}
