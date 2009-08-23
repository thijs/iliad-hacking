/*
 * This file is part of connectionMgr.
 *
 * connectionMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * connectionMgr is distributed in the hope that it will be useful,
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

#ifndef _ECDC_ERR_H_
#define _ECDC_ERR_H_

#ifndef ECDC_LIBRARY
/* 
 * Type definitions
 */
typedef enum
{
    ECD_NO_ERROR = 0,
    ECD_NOT_INITIALIZED,
    ECD_NO_CONNECTION,
    ECD_BAD_REQUEST,
    ECD_UNAUTHORIZED,
    ECD_INTERNAL_SERVER_ERROR,
    ECD_FAILURE,
    ECD_NOT_IMPLEMENTED,
    ECD_UNKNOWN_STATUS,
    ECD_UNKNOWN_SERVER_COMMAND,
    ECD_OUT_OF_MEMORY,
    ECD_UPLOAD_FAILED,
    ECD_FORBIDDEN,
    ECD_NOT_FOUND,
    ECD_WIRED_ERROR,
    ECD_WIRELESS_ERROR,
    ECD_DIALUP_ERROR
} ecd_err_t; 
#endif

#endif /* ECDC_ERR_H */
