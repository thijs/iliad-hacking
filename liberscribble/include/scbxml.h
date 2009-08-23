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

#ifndef SCBXML_H_
#define SCBXML_H_

// xml file support
// internal use
#include <libermanifest/ermanifest.h>
#include "scbconfig.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _ScbXml
{
    erManifest  handle;
    char        xPath[SCB_MAX_XML_PATH];
    int         index;
}ScbXml;
typedef ScbXml * ScbXmlPtr;

// shallow copy
ScbXmlPtr scb_xml_clone(const ScbXmlPtr src);
void scb_xml_free(ScbXmlPtr ptr);

#ifdef __cplusplus
}
#endif

#endif
