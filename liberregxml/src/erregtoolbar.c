/*
 * This file is part of liberregxml.
 *
 * liberregxml is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberregxml is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file erregtoolbar.c
 * \brief liberreg - ereader toolbar registry(read only)
 *                   loading and unloading functions
 *                   getting functions
 *                   free memory functions
 * 
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "erreglog.h"
#include "erreg.h"
#include "erregtoolbar.h"
#include "erregtypes_f.h"
#include "erregxml.h"

static void tbRegInitXpathList(regRegistry_t * pRegistry)
{
    ERREG_TOOLBAR_LOGPRINTF("entry[%p]", pRegistry);
    g_assert(pRegistry);

    xpathList_t *xpaths = g_new0(xpathList_t, 1);
    g_assert(xpaths);

    xpaths->toolbarIcons = "/" EL_REGISTRY_ROOT "/" EL_TOOLBAR_ICONS;
    xpaths->toolbarIcon  = "/" EL_REGISTRY_ROOT "/" EL_TOOLBAR_ICONS "/" EL_TOOLBAR_ICON;

    pRegistry->xpaths = xpaths;
}

static void tbRegDestroyXpathList(regRegistry_t * pRegistry)
{
    ERREG_TOOLBAR_LOGPRINTF("entry[%p]", pRegistry);
    g_assert(pRegistry);

    g_free(pRegistry->xpaths);
    pRegistry->xpaths = NULL;
}

regLoad_t tbRegLoad(regRegistry_t** ppRegistry)
{
    ERREG_TOOLBAR_LOGPRINTF("entry");

    regRegistry_t* pRegistry = NULL;
    regLoad_t ret;

    pRegistry = regParseFiles(REG_TOOLBAR_XML_PATH, REG_TOOLBAR_XSD_PATH); 
    if (pRegistry)
    {
        ERREG_TOOLBAR_LOGPRINTF("Loading toolbar registry succeeded.");
        ret = loadOk_t;
    }
    else
    {
        ERREG_TOOLBAR_ERRORPRINTF("Loading toolbar registry failed.");
        ret = loadError_t;
    }

    if (pRegistry)
    {
        tbRegInitXpathList(pRegistry);
    }

    *ppRegistry = pRegistry;
     return ret;
}

void tbRegUnload(regRegistry_t* pRegistry)
{
    ERREG_TOOLBAR_LOGPRINTF("entry[%p]", pRegistry);

    if (pRegistry)
    {
        tbRegDestroyXpathList(pRegistry);
        regDestroy(pRegistry);
    }
}

regIconSet_t* tbRegGetIconSet(regRegistry_t * pRegistry)
{
    ERREG_TOOLBAR_LOGPRINTF("entry");
    
    regIconSet_t *theIconSet = NULL;
    
    if (pRegistry)
    {
        theIconSet = regGetIconSet(pRegistry);
    }
    
    return theIconSet;
}

void tbRegFreeIconSet(regIconSet_t * theIconSet)
{
    ERREG_TOOLBAR_LOGPRINTF("entry");

    if (theIconSet)
    {
        regFreeIconSet(theIconSet);
    }
}

