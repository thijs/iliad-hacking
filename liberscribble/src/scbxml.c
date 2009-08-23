/*
 * This file is part of liberscribble.
 *
 * liberscribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberscribble is distributed in the hope that it will be useful,
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

#include "scbxml.h"
#include "scblog.h"
#include <glib.h>

ScbXmlPtr scb_xml_clone(const ScbXmlPtr src)
{
    if (NULL == src)
    {
        SCB_ERROR("Invalid src pointer!");
        return NULL;            
    }

    ScbXmlPtr ret = g_new0(ScbXml, 1);
    if (NULL == ret)
    {
        SCB_ERROR("Not enough memory!");
        return NULL;
    }
    // add ref
    ret->handle.pDoc = src->handle.pDoc;
    ret->handle.pPathCtx = src->handle.pPathCtx;
    strncpy(ret->xPath, src->xPath, SCB_MAX_XML_PATH);
    ret->index = src->index;
    return ret;
}

void scb_xml_free(ScbXmlPtr ptr)
{
    g_free(ptr);
}
