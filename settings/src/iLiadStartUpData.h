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
 * \file iLiadStartUpData.h
 * \brief settings - user customized start-up behaviour data
 *
 * Handling user customized start-up behaviour data. 
 * 
 */

#ifndef __ILIAD_STARTUP_DATA_H__
#define __ILIAD_STARTUP_DATA_H__

#include <liberregxml/erregapi.h> 

#ifdef __cplusplus
extern "C"
{
#endif

#define QUICK_START_GUIDE_PATH   "/mnt/free/documents/Reference/10_Quick_Reference"

void iLiad_startup_data_init(void);
void iLiad_startup_data_destroy(void);
void iLiad_startup_data_store(void);

startup_behaviour_t iLiad_startup_data_get_behaviour(void);
void iLiad_startup_data_set_behaviour(startup_behaviour_t behaviour, const gchar *document_path);

#ifdef __cplusplus
}
#endif

#endif // __ILIAD_STARTUP_DATA_H__

