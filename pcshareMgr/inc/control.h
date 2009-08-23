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
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

/**
 * \file control.h
 * \brief control - handle input and keep state                  
 * 
 * <File description>
 *
 */

#ifndef __CONTROL_MODULE__H__
#define __CONTROL_MODULE__H__

//#include "libermds/ermds.h"

#ifdef __cplusplus
extern  "C"
{
#endif /* __cplusplus */

#define DL_OK 0
#define DL_DOWNLOAD_FAILED 1
#define DL_CONNECTION_FAILED 2
#define DL_HISTORY_ACCESS_FAILED 3	
#define DL_HISTORY_LOCATION_FAILED 4
#define DL_USER_PROFILE_MISSING 5
#define DL_EMAIL_MISSING		6
#define DL_PASSWORD_MISSING		7
#define DL_INIT_CONNECTION_FAILURE	8
#define DL_CONNECT_TO_SERVER_FAILURE 9


typedef struct _shareMgr_t
{
    GtkWidget *topLevelWindow;
    int returnVal;     
} shareMgr_t;


/**
 * Init download manager data : - init screen layout
 *                              - allocate downloadMgr_t, for maintaining state	   
 *
 * @param window reference to the top level window 
 *
 * @returns reference to downloadMgr_t structure, NULL in case of failure
 */
shareMgr_t * ctrlInit(GtkWidget * topLevelWindow);

/**
 * clean up allocated memory
 *
 * @param dlMgr reference downloadMgrdata
 *
 * @returns -
 */
void ctrlDestroy(shareMgr_t * shMgr);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__CONTROL_MODULE__H__
