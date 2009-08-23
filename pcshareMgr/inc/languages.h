/*
 * This file is part of pcshareMgr.
 *
 * pcshareMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pcshareMgr is distributed in the hope that it will be useful,
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

#ifndef languages_h
#define languages_h


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "config.h"
#include "gettext.h"

#define _(String) gettext (String)

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
