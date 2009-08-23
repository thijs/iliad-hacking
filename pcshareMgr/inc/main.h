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
 * \file main.h
 * \brief main - main module of the PC Share Manager application                  
 * 
 */

/* pcshareMgr - A GTK based PC Share Manager application on the eReader platform
 * Copyright (C) 2006 iRex Technologies BV
 *
 */
#ifndef __MAIN_MODULE__H__
#define __MAIN_MODULE__H__


#ifdef __cplusplus
extern  "C"
{
#endif /* __cplusplus */


gboolean main_get_background(void);

void main_report_new_content();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__MAIN_MODULE__H__

