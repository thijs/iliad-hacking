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
 * \file languages.h
 * \brief contentLister application - internationalization 
 
 * <File description>
 * 
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef languages_h
#define languages_h


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "config.h"
#include "gettext.h"

#define _(String) gettext (String)

#define DEFAULT_LOCALE  "en_US"
#define LOCALE_LIB_DIR  "/usr/lib/locale"


gboolean languageSelected(void);

/**
 *  init the language/localisation settings
 *
 * @param none
 *
 * @returns void
 */
void languagesInit(void);

/**
 *  return the current locale setting
 *
 * @param none
 *
 * @returns character pointer to the locale code (e.g. "en_US"
 */
const gchar *languagesGetLocale(void);

/**
 *  return the current country setting
 *
 * @param none
 *
 * @returns character pointer to the country code (e.g. "en")
 */
const gchar *languagesGetCountry(void);

/**
 *  return the default country
 *
 * @param none
 *
 * @returns character pointer to the country code (e.g. "en")
 */
const gchar *languagesGetDefaultCountry(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif //languages_h
