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
 */

#include "config.h"
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include <glib.h>
#include <gtk/gtk.h>

#include <libergtk/ergtk.h>

#include "setupLog.h"
#include "gettext.h"
#include "languages.h"
#include "iLiadConfigData.h"

// List of known locales, last entry has NULL pointer in name
// Note: order in this list determines order on screen
static const iLiad_locale_t  g_locale_known_tbl[] = {
                                                      { "en_US",  "English"    , "Set English as the language for your iLiad"       },  // English
                                                      { "de_DE",  "Deutsch"    , "Ich möchte Deutsch als Sprache des iLiad"         },  // German
                                                      { "es_ES",  "Español"    , "Seleccionar el idioma Español para su iLiad"      },  // Spanish
                                                      { "fr_FR",  "Français"   , "Employer le français comme langue de votre iLiad" },  // French
                                                      { "it_IT",  "Italiano"   , "Scegli italiano come lingua per il tuo iLiad"     },  // Itialian
                                                      { "nl_NL",  "Nederlands" , "Kies Nederlands als taal voor uw iLiad"           },  // Dutch
                                                      { "no_NO",  "Norsk"      , "Set Norwegian as the language for your iLiad"     },  // Norwegian
                                                      { "pl_PL",  "Polski"     , "Set Polish as the language for your iLiad"        },  // Polish
                                                      { "pt_PT",  "Português"  , "Seleccionar Português como língua do seu iLiad"   },  // Portuguese
                                                      { "ru_RU",  "Русский"    , "Set Russian as the language for your iLiad"       },  // Russian
                                                      { "fi_FI",  "Suomi"      , "Set Finnish as the language for your iLiad"       },  // Finnish
                                                      { "sv_SE",  "Svenska"    , "Sätt svenska som språk för din iLiad"             },  // Swedish
                                                      { "zh_CN",  "中文"       , "设置中文为iLiad的默认语言"                        },  // Chinese
                                                      { "ja_JP",  "日本語"     , "Set Japanese as the language for your iLiad"      },  // Japanese
                                                      { "ko_KR",  "한국어"     , "Set Korean as the language for your iLiad"        },  // Korean
                                                      { NULL,      NULL        , NULL                                               }
                                                    };

static int locale_cmp(const void *left, const void *right)
{
    const iLiad_locale_t *locp;
    const gchar          *left_name  = ((iLiad_locale_t*)left )->name;
    const gchar          *right_name = ((iLiad_locale_t*)right)->name;

    gint  pos;
    gint  left_pos  = 9999;
    gint  right_pos = 9999;

    pos = 0;
    for (locp = g_locale_known_tbl ; locp->code ; locp++)
    {
        if (strcmp(left_name, locp->name) == 0)
        {
            left_pos = pos;
        }
        if (strcmp(right_name, locp->name) == 0)
        {
            right_pos = pos;
        }
        pos++;
    }
    
    return left_pos - right_pos; 
}

guint find_installed_locales(iLiad_locale_t** locale_installed_tbl)
{
    ST_LOGPRINTF("entry");

    int           num;
    DIR           *dirp;
    struct dirent *direntp;
    struct stat   statbuf;
    char          *cp;
    int           i;
   
    const iLiad_locale_t *locp;

    int              locale_installed_num = 0;
    iLiad_locale_t*  loc_locale_installed_tbl= NULL;

    // get locales currently installed
    dirp = opendir(LOCALE_LIB_DIR);
    if (dirp)
    {
        // count number of directory entries
        num = 0;
        while ((direntp = readdir(dirp)) != NULL)
        {
            num++;
        }

        // allocate table for installed locales
        loc_locale_installed_tbl = g_new0(iLiad_locale_t, num);
        locale_installed_num = 0;

        // get currently installed locales
        rewinddir(dirp);
        while ((direntp = readdir(dirp)) != NULL)
        {
            cp = g_strdup_printf(LOCALE_LIB_DIR "/%s", direntp->d_name);
            i  = stat(cp, &statbuf);
            g_free(cp);
            if (   i == 0
                && S_ISDIR(statbuf.st_mode)
                && direntp->d_name[0] != '.' )
            {
                // strip extension, if any
                cp = strchr(direntp->d_name, '.');
                if (cp)
                {
                    *cp = '\0';
                }

                // find locale in known locales
                for (locp = g_locale_known_tbl ; locp->code ; locp++)
                {
                    if (strcmp(direntp->d_name, locp->code) == 0)
                    {
                        // found
                        break;  // exit for
                    }
                }

                // add this locale to installed locales
                if (locp->code)
                {
                    loc_locale_installed_tbl[locale_installed_num].code      = g_strdup(locp->code     );
                    loc_locale_installed_tbl[locale_installed_num].name      = g_strdup(locp->name     );
                    loc_locale_installed_tbl[locale_installed_num].long_name = g_strdup(locp->long_name);
                    locale_installed_num++;
                }
                else
                {
                    loc_locale_installed_tbl[locale_installed_num].code      = g_strdup(direntp->d_name);
                    loc_locale_installed_tbl[locale_installed_num].name      = g_strdup(direntp->d_name);
                    loc_locale_installed_tbl[locale_installed_num].long_name = g_strdup(direntp->d_name);
                    locale_installed_num++;
                }
            }
        }
        
        // sort installed locales on name
        qsort( loc_locale_installed_tbl,
               locale_installed_num,
               sizeof(loc_locale_installed_tbl[0]),
               locale_cmp                         );
   
        closedir(dirp);
    }

    // return values
    ST_LOGPRINTF("Installed %d locales.", locale_installed_num);
    *locale_installed_tbl = loc_locale_installed_tbl;
    return locale_installed_num;
}

void free_installed_locales(iLiad_locale_t** locale_installed_tbl, gint locale_installed_num)
{
    ST_LOGPRINTF("entry [%d]", locale_installed_num);
    gint            i;
    iLiad_locale_t* locp;
    iLiad_locale_t* loc_locale_installed_tbl;

    g_return_if_fail(locale_installed_tbl != NULL);
    g_return_if_fail(locale_installed_num > 0);

    loc_locale_installed_tbl = *locale_installed_tbl;
    g_return_if_fail(loc_locale_installed_tbl != NULL);

    for (i = 0; i < locale_installed_num; i++)
    {
        locp = &loc_locale_installed_tbl[i];
        ST_LOGPRINTF("locp [%d] [%p]", i, locp);
        if (locp)
        {
            ST_LOGPRINTF("%s, %s", locp->code, locp->name);
            g_free( (gchar*)(locp->code     ) );  // const_cast
            g_free( (gchar*)(locp->name     ) );  // const_cast
            g_free( (gchar*)(locp->long_name) );  // const_cast
        }
    }

    g_free(loc_locale_installed_tbl);
    *locale_installed_tbl = NULL;
}

void languagesInit()
{
    char   *result;
    char   *theLanguage;

#if ENABLE_NLS
    ST_LANGUAGEPRINTF("ENABLE_NLS = TRUE");
#else
    ST_LANGUAGEPRINTF("ENABLE_NLS = FALSE");
#endif

    theLanguage = iLiad_config_data_get_language();
    if (   theLanguage    == NULL
        || theLanguage[0] == '\0' )
    {
        // default is english
        theLanguage = DEFAULT_LOCALE;
    }

    setlocale(LC_ALL, theLanguage);
    g_setenv("LANG", theLanguage, 1);

    textdomain(PACKAGE);

    ST_LANGUAGEPRINTF("Language: [%s]", theLanguage);
    ST_LANGUAGEPRINTF("LOCALE_DIR: [%s]", LOCALE_DIR);
    ST_LANGUAGEPRINTF("PACKAGE: [%s]", PACKAGE);

    result = bindtextdomain(PACKAGE, LOCALE_DIR);;

    ST_LANGUAGEPRINTF("bindtextdomain: [%s]", result);

    textdomain(PACKAGE);
}


