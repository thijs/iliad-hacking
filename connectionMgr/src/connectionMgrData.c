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
 * \file connectionMgrData.c
 * \brief connectionMgr - connectionMgr data handling
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>

#include "connectionMgrLog.h"
#include "commonData.h"
#include "connectScreenData.h"
#include "editScreenData.h"
#include "connectionMgrData.h"

void cmgr_data_init(void)
{
    CN_LOGPRINTF("entry");
   
    cmgr_data_destroy();

    common_data_init();
    connect_data_init();
    edit_data_init();
}

void cmgr_data_destroy(void)
{
    CN_LOGPRINTF("entry");

    common_data_destroy();
    connect_data_destroy();
    edit_data_destroy();
}

