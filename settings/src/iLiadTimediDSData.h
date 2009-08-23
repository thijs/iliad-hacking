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
 * \file iLiadTimediDSData.h
 * \brief settings - Timed-iDS connection settings data operations
 *
 * Handling timed-iDS connection settings data.
 * 
 */

#ifndef __ILIAD_TIMEDIDS_DATA_H__
#define __ILIAD_TIMEDIDS_DATA_H__

#include <liberregxml/erregapi.h> 

#ifdef __cplusplus
extern "C"
{
#endif

void iLiad_timedids_data_init(void);
void iLiad_timedids_data_destroy(void);
void iLiad_timedids_data_store(void);
regTimedIds_t* get_current_timedids_settings(void);

#ifdef __cplusplus
}
#endif

#endif // __ILIAD_TIMEDIDS_DATA_H__

