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
 * \file iLiadLanguageData.c
 * \brief settings - Language settings data
 *
 * Providing language settings data functions. 
 * 
 */

#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>

#include "setupLog.h"
#include "iLiadLanguageData.h"
#include "languages.h"
#include "settings.h"

static regUserSetting_t* g_settings = NULL;

static char *g_cur_language = NULL;
static char *g_stored_language = NULL;

void iLiad_language_data_init(void)
{
    ST_LOGPRINTF("entry");

    // get settings from registry
    iLiad_language_data_destroy();
    g_settings = erRegGetUserSetting();

    if (g_settings == NULL)
    {
        ST_WARNPRINTF("use default values");
        
        g_settings = g_new0(regUserSetting_t, 1);
        g_settings->language = strdup(DEFAULT_LOCALE);
        g_settings->volume = 75;
        g_settings->keyBuffering = 0;
        g_settings->longkeypressTimeout = 500;
        g_settings->flipbarLeftIsDown = TRUE;

        g_cur_language = g_strdup(g_settings->language);
    }
    else
    {
        g_cur_language = g_strdup(g_settings->language);
        g_stored_language = g_strdup(g_cur_language);
    }

    ST_LOGPRINTF("current language is %s", g_cur_language);
}

void iLiad_language_data_destroy(void)
{
     ST_LOGPRINTF("entry");

     if (g_settings)
     {
         erRegFreeUserSetting(g_settings);
         g_settings = NULL;
     }

     if (g_cur_language)
     {
         g_free(g_cur_language);
         g_cur_language = NULL;
     }

     if (g_stored_language)
     {
         g_free(g_stored_language);
         g_stored_language = NULL;
     }
}

void iLiad_language_data_store(void)
{
     ST_LOGPRINTF("entry");

     gboolean bNeedSave = FALSE;
     
     g_return_if_fail(g_cur_language);
     
     if ( (NULL == g_stored_language) || strcmp(g_cur_language, g_stored_language))
     {
         bNeedSave = TRUE;
     }

     ST_LOGPRINTF("Need save? %s.", bNeedSave ? "Yes":"No");

    if (bNeedSave)
    {         
        prepare_registry_write();

        // prepare g_settings
        g_free(g_settings->language);
        g_settings->language = g_strdup(g_cur_language);

        // store g_settings to registry
        ST_STOREPRINTF("calling erRegSetUserSetting");
        erRegSetUserSetting(g_settings);

        // update g_stored_language
        g_free(g_stored_language);
        g_stored_language = g_strdup(g_cur_language);

        do_registry_write();
    }
}

void iLiad_language_data_set(const char* language)
{
     ST_LOGPRINTF("entry, change language from '%s' to be '%s'", 
             g_cur_language, language);
     
     g_free(g_cur_language);
     g_cur_language = strdup(language);
}

char* iLiad_language_data_get(void)
{
     ST_LOGPRINTF("entry");

     return g_cur_language;
}

