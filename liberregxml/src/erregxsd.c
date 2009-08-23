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
 * \file erregxsd.c
 * \brief liberreg -  load and unload xsd file
 *                    validate xml against xsd
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#include <stdio.h>

#include <glib.h>

#include "erreglog.h"
#include "erreg.h"
#include "erregxsd.h"
#include "erregxml.h"
#include "erregtypes_f.h"

gboolean xsdLoad(regSchema_t * pXsdCxt, const char *szXsdFile)
{
    ERREG_XSD_LOGPRINTF("entry %p %s", pXsdCxt, szXsdFile);

    if (NULL == pXsdCxt || NULL == szXsdFile)
    {
        ERREG_XSD_ERRORPRINTF("invalid arguments");
        return FALSE;
    }

    pXsdCxt->pDoc = xmlParseFile(szXsdFile);
    if (pXsdCxt->pDoc)
    {
        pXsdCxt->pParser = xmlSchemaNewDocParserCtxt(pXsdCxt->pDoc);
        if (pXsdCxt->pParser)
        {
            pXsdCxt->pCxt = xmlSchemaParse(pXsdCxt->pParser);
            if (pXsdCxt->pCxt)
            {
                pXsdCxt->pValid = xmlSchemaNewValidCtxt(pXsdCxt->pCxt);
                if (pXsdCxt->pValid)
                {
                    pXsdCxt->szFileName = g_strdup(szXsdFile);
                    return TRUE;
                }
            }
        }
    }

    xsdUnload(pXsdCxt);
    return FALSE;
}

void xsdUnload(regSchema_t * pXsdCxt)
{
    ERREG_XSD_LOGPRINTF("entry %p", pXsdCxt);

    if (pXsdCxt)
    {
        if (pXsdCxt->pDoc)
        {
            xmlFreeDoc(pXsdCxt->pDoc);
            pXsdCxt->pDoc = NULL;
        }
      
        if (pXsdCxt->pParser)
        {
            xmlSchemaFreeParserCtxt(pXsdCxt->pParser);
            pXsdCxt->pParser = NULL;
        }

        if (pXsdCxt->pCxt)
        {
            xmlSchemaFree(pXsdCxt->pCxt);
            pXsdCxt->pCxt = NULL;
        }

        if (pXsdCxt->pValid)
        {
            xmlSchemaFreeValidCtxt(pXsdCxt->pValid);
            pXsdCxt->pValid = NULL;
        }

        if (pXsdCxt->szFileName)
        {
            g_free(pXsdCxt->szFileName);
            pXsdCxt->szFileName = NULL;
        }
    }
}

gboolean xsdValidateXmlDoc(regSchema_t * pXsdCxt, xmlDocPtr pDoc)
{
    gboolean bRet = FALSE;

    ERREG_XSD_LOGPRINTF("entry %p %p", pXsdCxt, pDoc);

    if (pXsdCxt && pXsdCxt->pValid && pDoc)
    {
        int nRet = xmlSchemaValidateDoc(pXsdCxt->pValid, pDoc);
        if (0 == nRet)
        {
            bRet = TRUE;
        }
    }
    ERREG_XSD_LOGPRINTF("Returns [%d]", bRet);
    return bRet;
}
