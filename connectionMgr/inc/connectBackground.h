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
 * \file connectBackground.h
 * \brief connectionMgr - 
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _CONNECT_BACKGROUND_H_
#define _CONNECT_BACKGROUND_H_

#include <glib.h>

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    void connect_background_restore_scan_ctxt(void);
    void connect_background_restore_ping_ctxt(void);

    int connect_background_get_initial_profile(void);

    void connect_background_on_connected(int profileIndex);
    void connect_background_on_failed_all(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_CONNECT_BACKGROUND_H_

