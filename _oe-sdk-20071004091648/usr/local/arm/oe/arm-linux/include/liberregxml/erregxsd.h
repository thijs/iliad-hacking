/**
 * \file 
 * \brief liberreg -        typedef struct regSchema_t           
 *                          functions declarations        
 *
 * Copyright (C) 2007 iRex Technologies BV.
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
