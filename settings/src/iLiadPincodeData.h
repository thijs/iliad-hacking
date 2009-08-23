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
 * \file iLiadPincodeData.h
 * \brief settings - Pincode Data
 *
 * in Pincode data of settings, read or write pincode data from sysset table
 *
 */
#ifndef __ILIAD_PINCODE_DATA_H__
#define __ILIAD_PINCODE_DATA_H__
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        gboolean enable;        // enbale(TRUE) or disable(FALSE) pincode?
        gchar pincode[PINCODE_MAX_LENGTH + 1];  // pincode string
    } sysPincodeSetting_t;


// load pincode data from sysset table into memory
    void iLiad_pincode_data_init(void);

// free the pincode data in memory
    void iLiad_pincode_data_destroy(void);

// return the value of "disable/enable" pincode
// TRUE: enable pincode
// FALSE: disable pincode
    gboolean iLiad_pincode_data_is_pincode_on(void);
    void iLiad_pincode_data_set_pincode_on(gboolean enable);

// return the pincode string
    unsigned char *iLiad_pincode_data_get_pincode(void);
    void iLiad_pincode_data_set_pincode(const gchar * text);

// store the pincode information into sysset table
    void iLiad_pincode_data_store();

#ifdef __cplusplus
}
#endif
#endif                          /* __ILIAD_PINCODE_DATA_H__ */
