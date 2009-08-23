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
 * \file erregtoolbar.h
 * \brief liberreg - ereader toolbar registry(read only)
 *                   loading and unloading functions
 *                   getting functions
 *                   free memory functions
 * 
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#ifndef _ERREG_TOOLBAR_H_
#define _ERREG_TOOLBAR_H_

#include "erregtypes.h"
#include "erregxml.h"
#include "erreg.h"

#define REG_TOOLBAR_XML_PATH    TEMPLATE_DIR "/toolbar.xml"
#define REG_TOOLBAR_XSD_PATH    TEMPLATE_DIR "/toolbar.xsd"   

#ifdef __cplusplus
extern "C"
{
#endif

    regLoad_t tbRegLoad(regRegistry_t** ppRegistry);
    void tbRegUnload(regRegistry_t* pRegistry);

    regIconSet_t* tbRegGetIconSet(regRegistry_t * pRegistry);
    void tbRegFreeIconSet(regIconSet_t * theIconSet);

#ifdef __cplusplus
}
#endif

#endif // _ERREG_TOOLBAR_H_
