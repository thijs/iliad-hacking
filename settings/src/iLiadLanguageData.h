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
 * \file iLiadLanguageData.h
 * \brief settings - Language settings data
 *
 * Providing language settings data functions. 
 * 
 */
#ifndef __ILIAD_LANGUAGE_DATA_H__
#define __ILIAD_LANGUAGE_DATA_H__

#ifdef __cplusplus
extern "C"
{
#endif

void iLiad_language_data_init(void);
void iLiad_language_data_destroy(void);
void iLiad_language_data_store(void);

void iLiad_language_data_set(const char* language);
char* iLiad_language_data_get(void);

#ifdef __cplusplus
}
#endif

#endif // __ILIAD_LANGUAGE_DATA_H__
