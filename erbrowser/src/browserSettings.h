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
 * \file browserSettings.h
 * \brief browser - read/write the browser settings and configure mozilla with these settings
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __BROWSER_SETTINGS_H__
#define __BROWSER_SETTINGS_H__

#define PREF_ID NS_PREF_CONTRACTID
#define MAX_SETTINGSPATH_LENGTH		128
#define MAX_PROXY					15
#define MAX_PROXY_PORT				5
#define MAX_URL_SIZE				256

#define CONFIG_FILE_NAME	"/config"
#define STARTUP_FILE_NAME	"/startup"

typedef struct _ProxySettings 
{
	gint direct_connection;
	gchar http_proxy[MAX_PROXY];
  	gchar http_proxy_port[MAX_PROXY_PORT];
  	gchar ftp_proxy[MAX_PROXY];
  	gchar ftp_proxy_port[MAX_PROXY_PORT];
  	gchar ssl_proxy[MAX_PROXY];
  	gchar ssl_proxy_port[MAX_PROXY];
	gchar no_proxy_for[MAX_PROXY]; 	//one IP address wihtout proxy allowed
} ProxySettings;


typedef struct _BrowserSettings
{
  ProxySettings 	theProxySettings; 
  float				zoom_factor;
} BrowserSettings;

/**
 * creates (if needed) the directory on the system used to store the settings files 
 * retrieve the current browser settings and use these to configure mozilla/Gecko.  
 *
 * @param b reference to the MozEmbed widget 
 *
 * @returns TRUE in case of succes
 */
gboolean browser_settings_init(GtkMozEmbed *b);

/**
 * write the current browser settings into $home/.Browser/config 
 *
 * @returns TRUE in case of succes
 */
gboolean browser_settings_store(void);

/**
 * retrieve the current content of the settingsfile
 *
 * @param theBrowserSettings where the retreived values should be stored 
 *
 * @returns TRUE in case of succes
 */
gboolean browser_settings_get(BrowserSettings* theBrowserSettings);

/**
 * configure mozilla with the current settings 
 *
 * @param theBrowserSettings the current settings
 *
 * @returns TRUE in case of succes
 */
gboolean browser_settings_set_mozilla_settings(BrowserSettings* theBrowserSettings);

/**
 * Method used to retrieve the zoomfactor used by the browser .
 *
 * @returns zoomfactor
 */
float browser_settings_get_zoom_factor(void);

/**
 * Method used to set the the zoomfactor used by the browser .
 *
 * @returns void
 */
void browser_settings_set_zoom_factor(float zoom_factor);

#endif //__BROWSER_SETTINGS_H__



