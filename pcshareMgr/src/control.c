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
 * \file control.c
 * \brief control - keep state/control                  
 * 
 * <File description>
 *
 */

/* downloadMgr - A GTK based application on the eReader platform
 *
 */

#include <stdio.h>
#include <gtk/gtk.h>
#include <liberipc/eripc.h>

#include "logging.h"
#include "control.h"
#include "displayUpdate.h"
#include "view.h"

shareMgr_t *ctrlInit(GtkWidget * topLevelWindow)
{
    shareMgr_t *theShareMgr = NULL;

    // => only request a refresh on the background expose event
    display_update_increase_level(VIEW_WINDOW_EXPOSE_LEVEL);

    // create one theShareMgr struct and fill it with zeros
    theShareMgr = g_new0(shareMgr_t, 1);
    theShareMgr->topLevelWindow = topLevelWindow;
    theShareMgr->returnVal      = DL_OK;

    viewInit(topLevelWindow);
    viewSetText();
    return theShareMgr;
}

void ctrlDestroy(shareMgr_t * theShareMgr)
{
    g_return_if_fail(theShareMgr != NULL);

    DL_WARNPRINTF("Clean up wat needs to be cleared");

    g_free(theShareMgr);
}
