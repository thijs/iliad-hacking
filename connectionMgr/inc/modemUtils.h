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
 * \file modemUtils.h
 * \brief connectionMgr - provide wireless modem related utilities 
 *                        (e.g. signal strength)
 * 
 * Copyright (C) 2007 iRex Technologies BV
 *
 */

#ifndef _MODEM_UTILS_H
#define _MODEM_UTILS_H

#define BUF_LEN       256

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    int get_modem_signal_strength();

#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif // _MODEM_UTILS_H

