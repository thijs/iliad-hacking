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

//
//
// Description: internationalistation
//
// Author: ann <ann@localhost>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef languages_h
#define languages_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if ENABLE_NLS
#include <libintl.h>
#include <locale.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)
#else
#define _(String) (String)
#define N_(String) String
#define textdomain(Domain)
#define bindtextdomain(Package, Directory)
#endif

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
