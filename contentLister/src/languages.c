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
 * \file languages.c
 * \brief contentLister - internationalization 
 
 * <File description>
 * 
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#include "config.h"
#include <locale.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include <liberregxml/erregapi.h>

#include "contentListerLog.h"
#include "gettext.h"
#include "languages.h"

static gchar *g_szUserLanguage = NULL;


// Check whether user has selected language or not.
// If the current language hasn't been installed, we consider them as no language selection.
// return TRUE  language has been already selected
//        FALSE language hasn't been selected yet
gboolean languageSelected(void)
{
    gboolean    ret = FALSE;  // return value
    struct stat statbuf;
    char*       cp;

    CL_LOGPRINTF("entry");

    cp = g_strdup_printf(LOCALE_LIB_DIR "/%s", g_szUserLanguage);
    if (   stat(cp, &statbuf) == 0
        && S_ISDIR(statbuf.st_mode) )
    {
        ret = TRUE;
    }
    g_free(cp);

    return ret;
}

void languagesInit()
{
    char   *result;
    regUserSetting_t *theUserSetting = NULL;

#if ENABLE_NLS
    CL_LOGPRINTF("ENABLE_NLS = TRUE");
#else
    CL_LOGPRINTF("ENABLE_NLS = FALSE");
#endif

    // retrieve user setting from registry
    g_free(g_szUserLanguage);
    theUserSetting = erRegGetUserSetting();
    if (theUserSetting)
    {
        CL_LOGPRINTF("Using LOCALE: %s\n", theUserSetting->language);
        setlocale(LC_ALL, theUserSetting->language);
        g_setenv("LANG", theUserSetting->language, 1);
        g_szUserLanguage = g_strdup(theUserSetting->language);
        erRegFreeUserSetting(theUserSetting);
    }
    else
    {
        // default is english
        CL_WARNPRINTF("Using default LOCALE: %s\n", DEFAULT_LOCALE);
        setlocale(LC_ALL, DEFAULT_LOCALE);
        g_setenv("LANG", DEFAULT_LOCALE, 1);
        g_szUserLanguage = g_strdup(DEFAULT_LOCALE);
    }

    textdomain(PACKAGE);

    CL_LOGPRINTF("LOCALE_DIR: %s", LOCALE_DIR);
    CL_LOGPRINTF("PACKAGE: %s", PACKAGE);

    result = bindtextdomain(PACKAGE, LOCALE_DIR);;

    CL_LOGPRINTF("bindtextdomain: %s", result);

    textdomain(PACKAGE);
}

const gchar *languagesGetLocale()
{
    return g_szUserLanguage ? g_szUserLanguage : DEFAULT_LOCALE;
}

const gchar *languagesGetCountry()
{
    static gchar *country = NULL;
    char         *cp;
    
    g_free(country);
    country = g_strdup(g_szUserLanguage);
    
    cp = strchr(country, '_');
    if (cp != NULL)
    {
        *cp = '\0';
    }

    return country;
}

const gchar *languagesGetDefaultCountry()
{
    static gchar *country = NULL;
    char         *cp;
    
    g_free(country);
    country = g_strdup(DEFAULT_LOCALE);
    
    cp = strchr(country, '_');
    if (cp != NULL)
    {
        *cp = '\0';
    }

    return country;
}
