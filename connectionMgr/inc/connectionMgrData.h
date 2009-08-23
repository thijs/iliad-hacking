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
 * \file connectionMgrData.h
 * \brief connectionMgr - data handling with screen creation and handling
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _CONNECTION_MGR_DATA_H_
#define _CONNECTION_MGR_DATA_H_

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    // open the registry file en read the relevant settings data
    void cmgr_data_init(void);

    // free allocated settings data
    void cmgr_data_destroy(void);

#ifdef __cplusplus
}
#endif  //* __cplusplus */
 
#endif //_CONNECTION_MGR_DATA_H_

