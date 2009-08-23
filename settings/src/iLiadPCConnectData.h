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
 * \file iLiadPCConnectData.h
 * \brief setup application - E-reader PCConnect screen data handling                

 * <File description>
 * 
 */

#ifndef _ILIAD_PC_CONNECT_DATA_H_
#define _ILIAD_PC_CONNECT_DATA_H_

#include <liberregxml/erregapi.h>

typedef enum
{
    e_pcfield_profile_name = 0,
    e_pcfield_pcname,
    e_pcfield_sharename,
    e_pcfield_workgroup,
    e_pcfield_username,
    e_pcfield_password
} pc_fieldtype_t;

void iLiad_pc_connect_data_init(void);

void iLiad_pc_connect_data_destroy(void);

void iLiad_pc_connect_data_store(void);

const regPCProfile_t *iLiad_pc_connect_get_profiledata(const gint profile_index);

gboolean iLiad_pc_connect_set_profile_field(gint profile_index, pc_fieldtype_t field, const char* value);


#endif // _ILIAD_PC_CONNECT_DATA_H_
