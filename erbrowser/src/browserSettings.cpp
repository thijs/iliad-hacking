/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file browserSettings.cpp
 * \brief browser - read/write the browser settings and configure mozilla with these settings
 *
 * \todo - link this with the setup application entries in the future 
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gstdio.h>

#include "gtkmozembed.h"
#include "gtkmozembed_internal.h"

#include "browserTypes.h"
#include "browserSettings.h"
#include "mozillaPreferences.h"

#include "nsCOMPtr.h"
#include "prenv.h"
#include "nsNetUtil.h"
#include "nsIWebBrowser.h"
#include "nsIScrollable.h"
#include "browserLog.h"

// global variables
static gchar gBrowserSettingsPath[MAX_SETTINGSPATH_LENGTH];
static BrowserSettings gBrowserSettings;

//internally used routines
static gboolean browser_settings_get_defaults(BrowserSettings * theBrowserSettings);
static void browser_set_proxy_prefs(ProxySettings * theProxySettings);
static void browser_display_settings(BrowserSettings * theBrowserSettings);

/*
 * browser_settings_init
 * 
 *     creates (if needed) the directory on the system used to store the settings files 
 *     retrieve the current browser settings and use these to configure mozilla/Gecko.       
 */
gboolean browser_settings_init(GtkMozEmbed * b)
{
    gboolean returnValue = FALSE;
    gchar  *directory;
    gchar  *browserUserHomePath;
    gchar  *filename;
    BrowserSettings *theBrowserSettings;

    BR_LOGPRINTF("entry");

    browserUserHomePath = PR_GetEnv("HOME");

    if (!browserUserHomePath)
    {
        BR_ERRORPRINTF("Failed to get HOME");
        return returnValue;
    }

    // create directory [g_strconcat alllocates memory]
    directory = g_strconcat(browserUserHomePath, "/.Browser", NULL);
    mkdir(directory, 0755);

    // store the path value
    if (strlen(directory) < MAX_SETTINGSPATH_LENGTH)
    {
        g_strlcpy(gBrowserSettingsPath, directory, MAX_SETTINGSPATH_LENGTH);
    }

    g_free(directory);

    BR_LOGPRINTF("path %s", gBrowserSettingsPath);

    filename = g_strconcat(gBrowserSettingsPath, CONFIG_FILE_NAME, NULL);

    theBrowserSettings = &gBrowserSettings;

    if (g_file_test(filename, G_FILE_TEST_EXISTS) == TRUE)
    {
        //retrieve the current content of the settingsfile 
        returnValue = browser_settings_get(theBrowserSettings);
    }
    else
    {
        browser_settings_get_defaults(theBrowserSettings);
        theBrowserSettings->zoom_factor = 1.0;
        returnValue = TRUE;
    }

    //only for debug reasons
    browser_display_settings(&gBrowserSettings);

    browser_settings_set_mozilla_settings(theBrowserSettings);
    
    // MvdW: Quick hack to allow XHTML navigation with hardkeys:
    //  made focus_ring visible, so links are outlined
    //  this needs to be moved to more generic settings functions
    //  which also need to read the registry for proxy settings and such
    mozilla_pref_set_int("browser.display.focus_ring_width", 3);
    mozilla_pref_set_boolean("browser.display.focus_ring_on_anything", TRUE);

    g_free(filename);

    return returnValue;
}


/*
 * browser_settings_get
 * 
 *     retrieve the current content of the settingsfile 
 */
gboolean browser_settings_get(BrowserSettings * theBrowserSettings)
{
    FILE   *settings_file = NULL;
    gchar  *filename = NULL;

    BR_LOGPRINTF("entry");

    if (theBrowserSettings)
    {
        filename = g_strconcat(gBrowserSettingsPath, CONFIG_FILE_NAME, NULL);
        settings_file = fopen(filename, "r");

        if (settings_file)
        {
            fscanf(settings_file, "directconnection=%d\n", &theBrowserSettings->theProxySettings.direct_connection);
            fscanf(settings_file, "http_proxy=%s\n", &theBrowserSettings->theProxySettings.http_proxy);
            fscanf(settings_file, "http_proxy_port=%s\n", &theBrowserSettings->theProxySettings.http_proxy_port);
            fscanf(settings_file, "ftp_proxy=%s\n", &theBrowserSettings->theProxySettings.ftp_proxy);
            fscanf(settings_file, "ftp_proxy_port=%s\n", &theBrowserSettings->theProxySettings.ftp_proxy_port);
            fscanf(settings_file, "ssl_proxy=%s\n", &theBrowserSettings->theProxySettings.ssl_proxy);
            fscanf(settings_file, "ssl_proxy_port=%s\n", &theBrowserSettings->theProxySettings.ssl_proxy_port);
            fscanf(settings_file, "no_proxy_for=%s\n", &theBrowserSettings->theProxySettings.no_proxy_for);
            fscanf(settings_file, "zoom_factor=%f\n", &theBrowserSettings->zoom_factor);

            fclose(settings_file);
        }
        else
        {
            BR_ERRORPRINTF("could not open settings file %s", filename);
        }

        g_free(filename);
        return TRUE;

    }
    else
    {
        BR_ERRORPRINTF("input parameter == NULL");
        return FALSE;
    }
}

/*
 * browser_display_settings
 * 
 *     print out struct content - DEBUG ROUTINE 
 *
 */
static void browser_display_settings(BrowserSettings * theBrowserSettings)
{
    if (theBrowserSettings)
    {
        BR_LOGPRINTF("\n------------------theBrowserSettings---------------------");
        BR_LOGPRINTF("direct_connection= %sn", (theBrowserSettings->theProxySettings.direct_connection) ? "TRUE" : "FALSE");
        BR_LOGPRINTF("http_proxy= %s", theBrowserSettings->theProxySettings.http_proxy);
        BR_LOGPRINTF("http_proxy_port= %s", theBrowserSettings->theProxySettings.http_proxy_port);
        BR_LOGPRINTF("ftp_proxy= %s", theBrowserSettings->theProxySettings.ftp_proxy);
        BR_LOGPRINTF("ftp_proxy_port= %s", theBrowserSettings->theProxySettings.ftp_proxy_port);
        BR_LOGPRINTF("ssl_proxy= %s", theBrowserSettings->theProxySettings.ssl_proxy);
        BR_LOGPRINTF("ssl_proxy_port= %s", theBrowserSettings->theProxySettings.ssl_proxy_port);
        BR_LOGPRINTF("zoom_factor= %f", theBrowserSettings->zoom_factor);
        BR_LOGPRINTF("---------------------------------------------------");
    }
    else
    {
        BR_ERRORPRINTF("input parameter == NULL");
    }
}

/*
 * browser_settings_get_defaults
 * 
 *     default browser settings 
 *
 */
static gboolean browser_settings_get_defaults(BrowserSettings * theBrowserSettings)
{
    if (theBrowserSettings)
    {
        theBrowserSettings->theProxySettings.direct_connection = 1;
        strcpy(theBrowserSettings->theProxySettings.http_proxy, "");
        strcpy(theBrowserSettings->theProxySettings.http_proxy_port, "");
        strcpy(theBrowserSettings->theProxySettings.ftp_proxy, "");
        strcpy(theBrowserSettings->theProxySettings.ftp_proxy_port, "");
        strcpy(theBrowserSettings->theProxySettings.ssl_proxy, "");
        strcpy(theBrowserSettings->theProxySettings.ssl_proxy_port, "");
        strcpy(theBrowserSettings->theProxySettings.no_proxy_for, "");
        return TRUE;
    }
    else
    {
        BR_ERRORPRINTF("input parameter == NULL");
        return FALSE;
    }
}

/*
 * browser_settings_store
 * 
 *     write the current browser settings into $home/.Browser/config 
 *
 */
gboolean browser_settings_store(BrowserSettings * theBrowserSettings)
{
    FILE   *settings_file;
    gchar  *file;

    BR_LOGPRINTF("entry");

    file = g_strconcat(gBrowserSettingsPath, CONFIG_FILE_NAME, NULL);
    settings_file = fopen(file, "w");

    if (settings_file == NULL)
    {
        BR_ERRORPRINTF("cannot open config file %s!", CONFIG_FILE_NAME);
        return FALSE;
    }

    BR_LOGPRINTF("file %s", file);
    fprintf(settings_file, "directconnection=%d\n", theBrowserSettings->theProxySettings.direct_connection);
    fprintf(settings_file, "http_proxy=%s\n", theBrowserSettings->theProxySettings.http_proxy);
    fprintf(settings_file, "http_proxy_port=%s\n", theBrowserSettings->theProxySettings.http_proxy_port);
    fprintf(settings_file, "ftp_proxy=%s\n", theBrowserSettings->theProxySettings.ftp_proxy);
    fprintf(settings_file, "ftp_proxy_port=%s\n", theBrowserSettings->theProxySettings.ftp_proxy_port);
    fprintf(settings_file, "ssl_proxy=%s\n", theBrowserSettings->theProxySettings.ssl_proxy);
    fprintf(settings_file, "ssl_proxy_port=%s\n", theBrowserSettings->theProxySettings.ssl_proxy_port);
    fprintf(settings_file, "no_proxy_for=%s\n", theBrowserSettings->theProxySettings.no_proxy_for);

    fprintf(settings_file, "zoom_factor=%f\n", theBrowserSettings->zoom_factor);

    fclose(settings_file);
    g_free(file);

    browser_settings_set_mozilla_settings(theBrowserSettings);

    return TRUE;
}


/*
 * browser_settings_set_mozilla_settings
 * 
 *     configure mozilla with the current settings 
 *
 */
gboolean browser_settings_set_mozilla_settings(BrowserSettings * theBrowserSettings)
{
    // set proxy preferences 
    browser_set_proxy_prefs(&theBrowserSettings->theProxySettings);
    mozilla_save_prefs();
    return TRUE;
}

/*
 * browser_set_proxy_prefs
 * 
 *     configure mozilla with the current proxy settings 
 *
 */
static void browser_set_proxy_prefs(ProxySettings * theProxySettings)
{
    gint    network_type = 0;

    BR_LOGPRINTF("entry");

    if (theProxySettings)
    {
        if (theProxySettings->direct_connection)
        {
            mozilla_pref_set_char("network.proxy.http", "");
            mozilla_pref_set_char("network.proxy.ssl", "");
            mozilla_pref_set_char("network.proxy.ftp", "");
            mozilla_pref_set_char("network.proxy.no_proxies_on", "");
            mozilla_pref_set_int("network.proxy.type", network_type);
        }
        else
        {
            if (strlen(theProxySettings->http_proxy) != 0
                && strcmp(theProxySettings->http_proxy, "") != 0
                && strlen(theProxySettings->http_proxy_port) > 0 && strcmp(theProxySettings->http_proxy_port, "") != 0)
            {
                mozilla_pref_set_int("network.proxy.type", 1);
                network_type = 1;
                mozilla_pref_set_char("network.proxy.http", theProxySettings->http_proxy);
                mozilla_pref_set_int("network.proxy.http_port", atoi(theProxySettings->http_proxy_port));
            }

            if (strlen(theProxySettings->ftp_proxy) != 0
                && strcmp(theProxySettings->ftp_proxy, "") != 0
                && strlen(theProxySettings->ftp_proxy_port) > 0 && strcmp(theProxySettings->http_proxy_port, "") != 0)
            {
                if (!network_type)
                {
                    mozilla_pref_set_int("network.proxy.type", 1);
                }
                mozilla_pref_set_char("network.proxy.ftp", theProxySettings->ftp_proxy);
                mozilla_pref_set_int("network.proxy.ftp_port", atoi(theProxySettings->ftp_proxy_port));
            }

            if (strlen(theProxySettings->ssl_proxy) != 0
                && strcmp(theProxySettings->ssl_proxy, "") != 0
                && strlen(theProxySettings->ssl_proxy_port) > 0 && strcmp(theProxySettings->ssl_proxy_port, "") != 0)
            {
                if (!network_type)
                {
                    mozilla_pref_set_int("network.proxy.type", 1);
                }
                mozilla_pref_set_char("network.proxy.ssl", theProxySettings->ssl_proxy);
                mozilla_pref_set_int("network.proxy.ssl_port", atoi(theProxySettings->ssl_proxy_port));
            }

            if (strlen(theProxySettings->no_proxy_for) != 0)
            {
                mozilla_pref_set_char("network.proxy.no_proxies_on", theProxySettings->no_proxy_for);
            }
            else
                mozilla_pref_set_char("network.proxy.no_proxies_on", "127.0.0.1");
        }
    }
    else
    {
        BR_ERRORPRINTF("input parameter == NULL");
    }
}

float browser_settings_get_zoom_factor(void)
{
    return gBrowserSettings.zoom_factor;
}

void browser_settings_set_zoom_factor(float zoom_factor)
{
    if (zoom_factor >= 0)
    {
        gBrowserSettings.zoom_factor = zoom_factor;
    }
}
