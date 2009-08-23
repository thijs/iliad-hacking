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
 * \file 
 * \brief liberreg -        typedef struct regSchema_t           
 *                          functions declarations        
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#ifndef _ERREG_XSD__H__
#define _ERREG_XSD__H__

#include <glib.h>

#include <libxml/xmlversion.h>
#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <libxml/schemasInternals.h>

typedef struct
{
    xmlDocPtr pDoc;
    xmlSchemaParserCtxtPtr pParser;
    xmlSchemaPtr pCxt;
    xmlSchemaValidCtxtPtr pValid;
    char *szFileName;
} regSchema_t;

#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

    gboolean xsdLoad(regSchema_t * pXsdCxt, const char *szFileName);
    void xsdUnload(regSchema_t * pXsdCxt);

    gboolean xsdValidateXmlDoc(regSchema_t * pXsdCxt, xmlDocPtr pDoc);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          //_ERREG_XSD__H__
