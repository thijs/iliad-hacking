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
 * \file mozillaPreferences.h
 * \brief browser - interface with gecko for setting/getting preference values
 *                  (the startup preferences of the browser app should be put in a "*.js" file
 *                  in the mozilla three)
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __MOZILLA_PREFERENCES_H__
#define __MOZILLA_PREFERENCES_H__

/**
 * store the preference changes
 *
 * @param void
 *
 * @returns TRUE on succes
 */
gboolean mozilla_save_prefs (void);

/**
 * remove a mozilla preference, reverting to the default value
 *
 * @param preferenceName ID of the mozilla reference
 *
 * @returns TRUE on succes
 */
gboolean mozilla_pref_remove (const gchar *preferenceName);

/**
 * set a string mozilla preference
 *
 * @param preferenceName ID of the mozilla reference
 * @param newValue new string value
 *
 * @returns TRUE on succes
 */
gboolean mozilla_pref_set_char (const char *preferenceName, const char *newValue);

/**
 * set a boolean mozilla preference
 *
 * @param preferenceName ID of the mozilla reference
 * @param newValue new boolean value
 *
 * @returns TRUE on succes
 */
gboolean mozilla_pref_set_boolean (const char *preferenceName, gboolean newValue);

/**
 * set a integer mozilla preference
 *
 * @param preferenceName ID of the mozilla reference
 * @param newValue new integer value
 *
 * @returns TRUE on succes
 */
gboolean mozilla_pref_set_int (const char *preferenceName, int newValue);

/**
 * get a string mozilla preference (calling app should handle free)
 *
 * @param preferenceName ID of the mozilla reference
 *
 * @returns string
 */
gchar *mozilla_pref_get_char (const char *preferenceName);

/**
 * get a boolean mozilla preference 
 *
 * @param preferenceName ID of the mozilla reference
 * @param defaultValue value that will be returned in case of error
 *
 * @returns boolean (default_value on failure)
 */
gboolean mozilla_pref_get_boolean (const gchar *preferenceName, gboolean defaultValue);

/**
 * get an integer mozilla preference 
 *
 * @param preferenceName ID of the mozilla reference
  *
 * @returns int (default_value = -1 on failure)
 */
gint mozilla_pref_get_int (const char *preferenceName);

#endif //__MOZILLA_PREFERENCES_H__




