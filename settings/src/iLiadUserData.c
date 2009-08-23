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
 * \file iLiadUserData.c
 * \brief setup application - E-reader user-profile screen data handling (registry interaction)               
 * 
 */

#include <gtk/gtk.h>
#include <string.h>

#include <liberregxml/erregapi.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "iLiadUserScreen.h"
#include "iLiadUserData.h"
#include "languages.h"
#include "settings.h"


static regUserProfile_t* g_user_profile = NULL;
static regUserProfile_t* g_stored_profile = NULL;


static regUserProfile_t* user_profile_dup(const regUserProfile_t* profile);
    

void iLiad_user_data_init(void)
{
    ST_LOGPRINTF("entry");

    iLiad_user_data_destroy();

    g_user_profile = erRegGetUserProfile();

    if (g_user_profile == NULL)
    {
        ST_WARNPRINTF("use default values");
        g_user_profile = g_new0(regUserProfile_t, 1);

        g_user_profile->name = g_strdup("iRex");
        g_user_profile->email = g_strdup("i@r.x");
        g_user_profile->password = g_strdup("98In4w5tg");
        g_user_profile->redirectUrl = g_strdup("https://ids.irexnet.com:443/redirector");
    }
    else
    {
        g_stored_profile = user_profile_dup(g_user_profile);
    }
}

void iLiad_user_data_destroy(void)
{
    ST_LOGPRINTF("entry");

    if (g_user_profile)
    {
        erRegFreeUserProfile(g_user_profile);
        g_user_profile = NULL;
    }

    if (g_stored_profile)
    {
        erRegFreeUserProfile(g_stored_profile);
        g_stored_profile = NULL;
    }
}

void iLiad_user_data_display(void)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(NULL != g_user_profile);
    g_assert(NULL != g_user_profile);

    // name
    if (g_user_profile->name)
    {
        iLiad_user_set_name(g_user_profile->name);
    }

    // email
    if (g_user_profile->email)
    {
        iLiad_user_set_email(g_user_profile->email);
    }

    // password
    if (g_user_profile->password)
    {
        iLiad_user_set_password(g_user_profile->password);
    }

    // redirect-url: not displayed
    
    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_FULLSCREEN);
}


// callback functions
void on_user_name_changed(GtkWidget* item, const gchar* text)
{
    g_return_if_fail(NULL != g_user_profile);

    if (g_user_profile->name)
    {
        g_free(g_user_profile->name);
    }
    g_user_profile->name = strdup(text);
}

void on_user_email_changed(GtkWidget* item, const gchar* text)
{
    g_return_if_fail(NULL != g_user_profile);

    if (g_user_profile->email)
    {
        g_free(g_user_profile->email);
    }
    g_user_profile->email = strdup(text);

    // remove spaces from email
    char* cp;
    while ( (cp = strchr(g_user_profile->email, ' ')) != NULL )
    {
        strcpy(cp, cp+1);
    }
}

void on_user_password_changed(GtkWidget* item, const gchar* text)
{
    g_return_if_fail(NULL != g_user_profile);

    if (g_user_profile->password)
    {
        g_free(g_user_profile->password);
    }
    g_user_profile->password = strdup(text);
}



//store the user-profile information in the registry
void iLiad_user_data_store(void)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(NULL != g_user_profile);
    g_assert(NULL != g_user_profile);
    g_assert(NULL != g_user_profile->name);
    g_assert(NULL != g_user_profile->email);
    g_assert(NULL != g_user_profile->password);
    g_assert(NULL != g_user_profile->redirectUrl);

    if (   g_stored_profile != NULL
        && strcmp(g_user_profile->name,        g_stored_profile->name       ) == 0
        && strcmp(g_user_profile->email,       g_stored_profile->email      ) == 0
        && strcmp(g_user_profile->password,    g_stored_profile->password   ) == 0
        && strcmp(g_user_profile->redirectUrl, g_stored_profile->redirectUrl) == 0 )
    {
        // Nothing changed, nothing to save
        return;
    }

    prepare_registry_write();

    ST_STOREPRINTF("calling erRegSetUserProfile");
    erRegSetUserProfile(g_user_profile);

    erRegFreeUserProfile(g_stored_profile);
    g_stored_profile = user_profile_dup(g_user_profile);

    do_registry_write();
}


static regUserProfile_t* user_profile_dup(const regUserProfile_t* profile)
{
    regUserProfile_t* dup = NULL;
   
    if (profile != NULL)
    {
        dup = g_new0(regUserProfile_t, 1);
        dup->name = strdup(profile->name);
        dup->email = strdup(profile->email);
        dup->password = strdup(profile->password);
        dup->redirectUrl = strdup(profile->redirectUrl);
    }

    return dup;
}

regUserProfile_t* get_user_profile_ptr()
{
    return g_user_profile;
}

