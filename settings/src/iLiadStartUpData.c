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
 * \file iLiadStartUpData.c
 * \brief settings - user customized start-up behaviour data
 *
 * Handling user customized start-up behaviour data. 
 * 
 */

#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>

#include <libergtk/ergtk.h>
#include <liberregxml/erregapi.h>

#include "setupLog.h"
#include "iLiadStartUpData.h"
#include "background.h"
#include "displayStatus.h"
#include "languages.h"
#include "settings.h"


static regStartUp_t *g_current_startup = NULL;
static regStartUp_t *g_stored_startup = NULL;


void iLiad_startup_data_init(void)
{
    ST_LOGPRINTF("entry");

    iLiad_startup_data_destroy();

    g_current_startup = erRegGetStartUp();
    if (g_current_startup == NULL)
    {
        ST_WARNPRINTF("use default values");

        g_current_startup = g_new0(regStartUp_t, 1);
        g_assert(NULL != g_current_startup);
        g_current_startup->behaviour = 0;
    }
    else
    {
        g_stored_startup = erRegDupStartUp(g_current_startup);
    }
}

void iLiad_startup_data_destroy(void)
{
    ST_LOGPRINTF("entry");

    if (g_current_startup)
    {
        g_free(g_current_startup);
        g_current_startup = NULL;
    }

    if (g_stored_startup)
    {
        g_free(g_stored_startup);
        g_stored_startup = NULL;
    }
}

void iLiad_startup_data_store(void)
{
    gboolean bNeedSave = FALSE;

    ST_LOGPRINTF("entry");
    
    g_return_if_fail(NULL != g_current_startup);
   
    // need to save changes or not? 
    if (   NULL == g_stored_startup
        || g_stored_startup->behaviour != g_current_startup->behaviour
        || (g_stored_startup->documentPath  == NULL  &&  g_current_startup->documentPath != NULL)
        || (g_stored_startup->documentPath  != NULL  &&  g_current_startup->documentPath == NULL)
        || (g_stored_startup->documentPath  != NULL  &&  g_current_startup->documentPath != NULL
            && strcmp(g_stored_startup->documentPath, g_current_startup->documentPath) != 0     ) )
    {
        bNeedSave = TRUE;
    }
    ST_LOGPRINTF("Need save? %s.", bNeedSave ? "Yes":"No");
    
    if (bNeedSave)
    {
        prepare_registry_write();

        ST_STOREPRINTF("calling erRegSetStartUp");
        erRegSetStartUp(g_current_startup);

        erRegFreeStartUp(g_stored_startup);
        g_stored_startup = erRegDupStartUp(g_current_startup);

        do_registry_write();
    }
}

startup_behaviour_t iLiad_startup_data_get_behaviour(void)
{
    ST_LOGPRINTF("entry");

    if (g_current_startup)
    {
        return g_current_startup->behaviour;
    }
    else
    {
        ST_WARNPRINTF("g_current_startup == NULL");
        return 0;
    }
}

void iLiad_startup_data_set_behaviour(startup_behaviour_t behaviour, const gchar *document_path)
{
    ST_LOGPRINTF("entry: behaviour [%d] document_path [%s]", behaviour, document_path);

    g_return_if_fail(g_current_startup != NULL);

    if ((behaviour < 0) || (behaviour >= behaviourUndefined_t))
    {
        ST_WARNPRINTF("invalid value(%d), return immediately", behaviour);
        return;
    }
    
    g_current_startup->behaviour = behaviour;
    g_free(g_current_startup->documentPath);
    if (document_path)
    {
        g_current_startup->documentPath = g_strdup(document_path);
    }
    else
    {
        g_current_startup->documentPath = NULL;
    }
}

