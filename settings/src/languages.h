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
 * \file languages.c
 * \brief setup application - language support                

 * <File description>
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

// Definition of an iLiad locale
typedef struct
{
    const gchar *code;       // Language code, e.g. "nl_NL"
    const gchar *name;       // Name in the local language, e.g. "Nederlands"
    const gchar *long_name;  // e.g. "Set English as the language for your iLiad"
} iLiad_locale_t;

guint find_installed_locales(iLiad_locale_t** locale_installed_tbl);
void free_installed_locales(iLiad_locale_t** locale_installed_tbl, gint locale_installed_num);

/**
 *  init the language/localisation settings
 *
 * @param none
 *
 * @returns void
 */
void languagesInit();

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif //languages_h
