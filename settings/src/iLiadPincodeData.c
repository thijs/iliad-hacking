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
 * \file iLiadPincodeData.c
 * \brief settings - Pincode Data
 *
 * in Pincode data of settings, read or write pincode data from sysset table
 *
 */
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <glib.h>

#include "setupLog.h"
#include "system.h"
#include "iLiadPincodeData.h"


// store the current pincode in settings app
static sysPincodeSetting_t *g_current_pincode = NULL;
// store the lastest stored pincode settings in sysset
static sysPincodeSetting_t *g_stored_pincode = NULL;

static sysPincodeSetting_t *pincode_setting_dup(const sysPincodeSetting_t * settings);
static void iLiad_pincode_data_read_pincode(sysPincodeSetting_t * settings);
static void iLiad_pincode_data_write_pincode(const sysPincodeSetting_t * settings);

// load pincode data from sysset table into memory
void iLiad_pincode_data_init(void)
{
    iLiad_pincode_data_destroy();

    g_current_pincode = g_new0(sysPincodeSetting_t, 1);
    iLiad_pincode_data_read_pincode(g_current_pincode);

    if (NULL == g_current_pincode)
    {
        ST_WARNPRINTF("use default values");
        g_current_pincode = g_new0(sysPincodeSetting_t, 1);
        g_current_pincode->enable = FALSE;
        g_current_pincode->pincode[0] = '\0';
    }
    else
    {
        g_stored_pincode = pincode_setting_dup(g_current_pincode);
    }
}

void iLiad_pincode_data_destroy(void)
{
    if (g_current_pincode)
    {
        g_free(g_current_pincode);
        g_current_pincode = NULL;
    }

    if (g_stored_pincode)
    {
        g_free(g_stored_pincode);
        g_stored_pincode = NULL;
    }
}

gboolean iLiad_pincode_data_is_pincode_on(void)
{
    if (NULL == g_current_pincode)
        return FALSE;
    return g_current_pincode->enable;
}

void iLiad_pincode_data_set_pincode_on(gboolean enable)
{
    if (NULL == g_current_pincode)
        return;
    g_current_pincode->enable = enable;
}

unsigned char *iLiad_pincode_data_get_pincode(void)
{
    if (NULL == g_current_pincode)
        return NULL;
    return g_current_pincode->pincode;
}

void iLiad_pincode_data_set_pincode(const gchar * text)
{
    int len;
    if (NULL == text)
        return;
    if (NULL == g_current_pincode)
        return;
    len = strlen(text);
    if (len > PINCODE_MAX_LENGTH)
        len = PINCODE_MAX_LENGTH;
    strncpy(g_current_pincode->pincode, text, len);
}

// check text is valid pincode string
// a valid pincode string should be only digital numbers
// and it's length is limited between 4-8 characters(letters)
gboolean iLiad_pincode_data_check_pincode(gchar * text)
{
    int i, len;
    gboolean is_digital = FALSE;
    gboolean returnVal = FALSE;
    gchar c;

    if (NULL == text)
        return FALSE;
    len = strlen(text);
    if (len < PINCODE_MIN_LENGTH || len > PINCODE_MAX_LENGTH)
        return FALSE;
    for (i = 0; i < len; i++)
    {
        c = text[i];
        is_digital = (isdigit(c)) ? TRUE : FALSE;
        if (FALSE == is_digital)
            break;
    }
    if (TRUE == is_digital)
        returnVal = TRUE;
    else
        returnVal = FALSE;
    return returnVal;
}

// store the pincode information into sysset table
void iLiad_pincode_data_store()
{
    if ((NULL == g_current_pincode) || (NULL == g_stored_pincode))
        return;

    if ((g_current_pincode->enable != g_stored_pincode->enable)
        || (0 != strcmp(g_current_pincode->pincode, g_stored_pincode->pincode)))
    {
        iLiad_pincode_data_write_pincode(g_current_pincode);

        if (g_stored_pincode->enable != g_current_pincode->enable)
            g_stored_pincode->enable = g_current_pincode->enable;

        if (strcmp(g_current_pincode->pincode, g_stored_pincode->pincode))
            strcpy(g_stored_pincode->pincode, g_current_pincode->pincode);
    }
}

static sysPincodeSetting_t *pincode_setting_dup(const sysPincodeSetting_t * settings)
{
    sysPincodeSetting_t *dup = NULL;

    if (NULL == settings)
        return NULL;

    dup = g_new0(sysPincodeSetting_t, 1);

    dup->enable = settings->enable;

    if (0 == strlen(settings->pincode))
        dup->pincode[0] = '\0';
    else
        strcpy(dup->pincode, settings->pincode);

    return dup;
}

// sysset functions
static void iLiad_pincode_data_read_pincode(sysPincodeSetting_t * settings)
{
    if (NULL == settings)
        return;
    sysset_read_pincode_onoff(&(settings->enable));
    sysset_read_pincode_string(settings->pincode);
}

static void iLiad_pincode_data_write_pincode(const sysPincodeSetting_t * settings)
{
    if (NULL == settings)
        return;

    sysset_write_pincode_onoff(settings->enable);
    sysset_write_pincode_string(settings->pincode);
}
