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
 * \file iLiadConfigData.c
 * \brief setup application - E-reader config screen data handling (registry interaction)               

 * <File description>
 * 
 */

#include <gtk/gtk.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <unistd.h>

#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>
#include <liberipc/eripc.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>

#include "setupLog.h"
#include "displayStatus.h"
#include "background.h"
#include "erbusy.h"
#include "iLiadUserScreen.h"
#include "iLiadConfigScreen.h"
#include "iLiadBgConnectScreen.h"
#include "iLiadPincodeScreen.h"
#include "iLiadConfigData.h"
#include "iLiadDateTimeScreen.h"
#include "iLiadPCConnectScreen.h"
#include "iLiadArcLocData.h"
#include "iLiadArcLocScreen.h"
#include "iLiadStartUpScreen.h"
#include "iLiadDeviceOverview.h"
#include "iLiadTimediDSScreen.h"
#include "languages.h"
#include "settings.h"

// Global data

// Local constants
#define MIXER               "/dev/mixer"
#define AUDIO_DEVICE        "/dev/dsp"

// Local data
//
// 'g_init_settings' is read from the registry at startup of the settings app
// if the current setting inside the settings app(g_user_settings) is different 
// from 'g_init_user_settings', exchange the flipbar direction in the settings 
// application itself(in function on_settings_keypress() ).
static regUserSetting_t *g_init_settings = NULL;
static regUserSetting_t *g_user_settings = NULL;
static regUserSetting_t *g_stored_settings = NULL;

static regUserSetting_t* user_setting_dup(const regUserSetting_t* settings);

static erClientChannel_t contentListerChannel;

void iLiad_config_data_init(void)
{
    ST_LOGPRINTF("entry");

    iLiad_config_data_destroy();

    g_user_settings = erRegGetUserSetting();

    if (g_user_settings == NULL)
    {
        ST_WARNPRINTF("use default values");
        // default values from the er_registry.txt
        g_user_settings = g_new0(regUserSetting_t, 1);
        g_user_settings->language = strdup(DEFAULT_LOCALE);
        g_user_settings->volume = 75;
        g_user_settings->keyBuffering = 0;
        g_user_settings->longkeypressTimeout = 500;
        g_user_settings->flipbarLeftIsDown = TRUE;
    }
    else
    {
        g_stored_settings = user_setting_dup(g_user_settings);
    }
    g_init_settings = user_setting_dup(g_user_settings);

    erIpcStartClient(ER_CONTENTLISTER_CHANNEL, &contentListerChannel);
}

void iLiad_config_data_destroy(void)
{
    ST_LOGPRINTF("entry");

    if (g_user_settings)
    {
        erRegFreeUserSetting(g_user_settings);
        g_user_settings = NULL;
    }

    if (g_stored_settings)
    {
        erRegFreeUserSetting(g_stored_settings);
        g_stored_settings = NULL;
    }

    if (g_init_settings)
    {
        erRegFreeUserSetting(g_init_settings);
        g_init_settings = NULL;
    }
}

void iLiad_config_data_display(void)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(NULL != g_user_settings);
    g_assert(NULL != g_user_settings);

    //language
    if (g_user_settings->language)
    {
        iLiad_config_set_language(g_user_settings->language);
    }
    else
    {
        iLiad_config_set_language(DEFAULT_LOCALE);
    }

    //timeout value
    iLiad_config_set_keys(g_user_settings->longkeypressTimeout);

    // volume
    //iLiad_config_set_volume(g_user_settings->volume);
    if(g_user_settings->volume == 0)    
    {
        iLiad_config_set_volume(0);
    }
    else
    {
        iLiad_config_set_volume(1);
    }   
    
    // keybuffering value
    if(g_user_settings->keyBuffering == 0)    
    {
        iLiad_config_set_keybuffering(0);
    }   
    else
    {
        iLiad_config_set_keybuffering(1);
    }
    
    // flipbarLeftIsDown value
    if(g_user_settings->flipbarLeftIsDown == TRUE)    
    {
        iLiad_config_set_flipbar_direction(TRUE);
    }
    else
    {
        iLiad_config_set_flipbar_direction(FALSE);
    }
    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_FULLSCREEN);
}

gboolean iLiad_config_data_change_flipbar_direction()
{
    gboolean ret = FALSE;
    
    if (g_init_settings && g_user_settings)
    {
        if (g_init_settings->flipbarLeftIsDown 
              == g_user_settings->flipbarLeftIsDown)
            ret = FALSE;
        else
            ret = TRUE;
    }
    
    return ret;
}

void on_keys_selection_update(erGtkSelectionGroup* selection, gpointer button, gpointer data)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(NULL != g_user_settings);
    g_assert(NULL != g_user_settings);

    g_user_settings->longkeypressTimeout = iLiad_config_get_keys();

    ST_LOGPRINTF("longkeypressTimeout = %d", g_user_settings->longkeypressTimeout);

    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_TYPING);
}

void on_keybuffering_selection_update(erGtkSelectionGroup* selection, gpointer button, gpointer data)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(NULL != g_user_settings);
    g_assert(NULL != g_user_settings);

    g_user_settings->keyBuffering = iLiad_config_get_keybuffering();

    // TODO: Fix when user can input the number of keys if enbale keybuffering    
    if (g_user_settings->keyBuffering != 0)
    {
        g_user_settings->keyBuffering = 12;
    }

    ST_LOGPRINTF("KeyBuffering %s", g_user_settings->keyBuffering == 0 ? "OFF" : "ON");

    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_TYPING);
}

void on_flipbar_direction_selection_update(erGtkSelectionGroup* selection, gpointer button, gpointer data)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(NULL != g_user_settings);
    g_assert(NULL != g_user_settings);

    g_user_settings->flipbarLeftIsDown = iLiad_config_get_flipbar_direction();

    ST_LOGPRINTF("flipbarLeftIsDown %s", g_user_settings->flipbarLeftIsDown == TRUE ? "LEFT" : "RIGHT");

    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_TYPING);
}

void on_language_selection_update(erGtkSelectionGroup* selection, gpointer button, gpointer data)
{
    g_assert(NULL != g_user_settings);
    g_assert(NULL != g_user_settings->language);

    ST_LOGPRINTF("entry");

    const gchar *theLanguage = iLiad_config_get_language();

    if ( strcmp(theLanguage, g_user_settings->language) != 0 )
    {
        erbusy_blink();

        g_free(g_user_settings->language);
        g_user_settings->language = strdup(theLanguage);
        ST_LANGUAGEPRINTF("language [%s]", g_user_settings->language);

        // update languages 
        languagesInit();
        bg_set_text();
        iLiad_startup_screen_set_text();
        iLiad_config_set_text();
        iLiad_archive_location_set_text();
        iLiad_user_set_text();
        iLiad_pincode_set_text();
        iLiad_pc_connect_screen_set_text();
        iLiad_autoconnect_set_text();
        iLiad_timedids_screen_set_text();
        iLiad_date_time_set_text();
        iLiad_device_overview_set_text();

        display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_FULLSCREEN);
    }

    ST_LOGPRINTF("done");
}

void on_sound_selection_update(erGtkSelectionGroup* selection, gpointer button, gpointer data)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(NULL != g_user_settings);
    g_assert(NULL != g_user_settings);

    g_user_settings->volume = iLiad_config_get_volume();

    // TODO: Fix when selection becomes a slider     
    if (g_user_settings->volume != 0)
    {
        g_user_settings->volume = 75;
    }

    ST_LOGPRINTF("sound %s", g_user_settings->volume == 0 ? "OFF" : "ON");

    display_update_request_screen_refresh(SETTING_ITEM_CHANGE, WAVEFORM_TYPING);

    clEnableAudio(contentListerChannel, g_user_settings->volume);
}

char   *iLiad_config_data_get_language(void)
{
    g_return_val_if_fail(NULL != g_user_settings, NULL);
    g_return_val_if_fail(NULL != g_user_settings->language, NULL);

    return g_user_settings->language;
}

//store the config information in the registry
void iLiad_config_data_store(void)
{
    ST_LOGPRINTF("entry");

    g_return_if_fail(NULL != g_user_settings);
    g_assert(NULL != g_user_settings);

    if (   g_stored_settings != NULL
        && strcmp(g_user_settings->language, g_stored_settings->language) == 0
        && g_user_settings->volume              == g_stored_settings->volume
        && g_user_settings->keyBuffering        == g_stored_settings->keyBuffering
        && g_user_settings->longkeypressTimeout == g_stored_settings->longkeypressTimeout 
        && g_user_settings->flipbarLeftIsDown   == g_stored_settings->flipbarLeftIsDown)
    {
        // Nothing changed, nothing to save
        return;
    }

    prepare_registry_write();

    ST_STOREPRINTF("calling erRegSetUserSetting");
    erRegSetUserSetting(g_user_settings);

    g_stored_settings = user_setting_dup(g_user_settings);

    do_registry_write();
}


static regUserSetting_t* user_setting_dup(const regUserSetting_t* settings)
{
    regUserSetting_t* dup = NULL;
   
    if (settings)
    {
        dup = g_new0(regUserSetting_t, 1);
        dup->language            = strdup(settings->language);
        dup->volume              = settings->volume;
        dup->longkeypressTimeout = settings->longkeypressTimeout;
        dup->keyBuffering        = settings->keyBuffering;
        dup->flipbarLeftIsDown   = settings->flipbarLeftIsDown;
    }

    return dup;
}
