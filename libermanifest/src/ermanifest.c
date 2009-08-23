/*
 * This file is part of libermanifest.
 *
 * libermanifest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libermanifest is distributed in the hope that it will be useful,
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


#include <stdio.h>
#include <unistd.h>

#define LIBXML_SCHEMAS_ENABLED
#include <libxml/xmlschemas.h>

#include "ermanifest.h"


#define DEFAULT_ENCODING            "utf-8"
#define INT_MAX_BUF                 15
#define BOOL_MAX_BUF                6

#define _DEBUG  0                   /* debug and release version */

#if (_DEBUG)
#define TRACE(x, args...) fprintf(stderr, "(ERM_TRACE)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define TRACE(x...) do {} while(0)
#endif


/**
 * ConvertInput:
 * @in: string in a given encoding
 * @encoding: the encoding used
 *
 * Converts @in into UTF-8 for processing with libxml2 APIs
 * From http://www.xmlsoft.org/examples/testWriter.c
 *
 * Returns the converted UTF-8 string, or NULL in case of error.
 */
xmlChar *ConvertInput(const char *in, const char *encoding)
{
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler)
    {
        TRACE("ConvertInput: no encoding handler found for '%s'\n", encoding ? encoding : "");
        return 0;
    }

    size = (int)strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *)xmlMalloc((size_t) out_size);

    if (out != 0)
    {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *)in, &temp);
        if ((ret < 0) || (temp - size + 1))
        {
            if (ret < 0)
            {
                TRACE("ConvertInput: conversion wasn't successful.\n");
            }
            else
            {
                TRACE("ConvertInput: conversion wasn't successful. converted: %i octets.\n", temp);
            }

            xmlFree(out);
            out = 0;
        }
        else
        {
            out = (unsigned char *)xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    }
    else
    {
        TRACE("ConvertInput: no mem\n");
    }

    return out;
}


/* internal routine */
// here, when the doucment has no root element, an error/warning information
// will be dump to screen, but it works. 
// we can create a new method to solve this problem. such as
// create(docname, rootname, pCtx)
/* MvdW: This function does not do ANY checking on the input data. BAD_CAST should actually be replaced with a 
   function that does escaping and entity encoding! */
xmlXPathObjectPtr LocateTo(const char *express, const erManifest *pCtx)
{
    xmlXPathObjectPtr xpathObj;

    xpathObj = xmlXPathEvalExpression(BAD_CAST express, pCtx->pPathCtx);
    if (xpathObj == NULL)
    {
        TRACE("LocateTo failed! Express %s\n", express);
        return NULL;
    }
    if (xpathObj->nodesetval == NULL || xpathObj->nodesetval->nodeNr <= 0)
    {
        TRACE("LocateTo failed! no or empty xmlNodeSet");
        xmlXPathFreeObject(xpathObj);
        return NULL;
    }

    return xpathObj;
}

xmlNodePtr ermXmlGetChildNode(xmlNodePtr parent, const char* childName)
{
    xmlNodePtr cur = NULL;

    //TRACE("entry: parent [%p] [%s]", parent, parent ? parent->name : NULL);
    //TRACE("entry: node [%s]", childName);

    if (parent && childName && childName[0] != '\0')
    {
        cur = parent->children;
        while (cur != NULL)
        {
            if ( xmlStrcmp(cur->name, (const xmlChar*)childName) == 0 )
            {
                //TRACE("Found child node %s", childName);
                break;          // exit while
            }
            cur = cur->next;
        }
    }
    return cur;
}

xmlNodeSetPtr ermXmlGetChildNodeSet(xmlNodePtr parent, const char* childName)
{
    int  i;

    xmlNodeSetPtr nodeSet = NULL;  // return value
    xmlNodePtr    cur     = NULL;

    //TRACE("entry: parent [%p] [%s]", parent, parent ? parent->name : NULL);
    //TRACE("entry: node [%s]", childName);

    // allocate space for nodeset
    nodeSet = xmlMalloc( sizeof(xmlNodeSet) );
    if (nodeSet == NULL) { return NULL; }
    nodeSet->nodeNr  = 0;
    nodeSet->nodeMax = 10;
    nodeSet->nodeTab = xmlMalloc(10 * sizeof(xmlNode));
    if (nodeSet->nodeTab == NULL) { return NULL; }
    
    if (parent && childName && childName[0] != '\0')
    {
        cur = parent->children;
        while (cur != NULL)
        {
            if ( xmlStrcmp(cur->name, (const xmlChar*)childName) == 0 )
            {
                //TRACE("Found child node %s", childName);

                // extend space for nodeset
                if (nodeSet->nodeNr >= nodeSet->nodeMax)
                {
                    i = 2 * nodeSet->nodeMax;
                    nodeSet->nodeMax = i;
                    nodeSet->nodeTab = xmlRealloc(nodeSet->nodeTab, i * sizeof(xmlNode));
                    if (nodeSet->nodeTab == NULL) { return NULL; }
                }

                // add node to nodeset
                nodeSet->nodeTab[ nodeSet->nodeNr ] = cur;
                nodeSet->nodeNr++;
            }
            cur = cur->next;
        }
    }
    return nodeSet;
}

xmlNodePtr ermXmlGetChildNodeWithAttr(xmlNodePtr parent, const char* nodeName, const char* attrName, const char* attrValue)
{
    xmlNodePtr cur = NULL;
    xmlNodePtr ret = NULL;
    xmlChar *attr;

    //TRACE("entry: parent [%p] [%s]", parent, parent ? parent->name : NULL);
    //TRACE("entry: node [%s] attr [%s] value [%s]", nodeName, attrName, attrValue);

    if (parent && nodeName && nodeName[0] != '\0')
    {
        cur = parent->children;
        while (cur != NULL && ret == NULL)
        {
            if (xmlStrcmp(cur->name, (const xmlChar *)nodeName) == 0)
            {
                attr = xmlGetProp(cur, attrName);
                if (attr)
                {
                    if (attrValue == NULL       // attribute value not specified
                        || xmlStrcmp(attr, (const xmlChar *)attrValue) == 0)    // attribute value specified and matches
                    {
                        //TRACE("Found child node %s", nodeName);
                        ret = cur;
                    }
                    xmlFree(attr);
                }
            }
            cur = cur->next;
        }
    }
    return ret;
}

xmlNodePtr ermXmlGetChildNodeWithoutAttr(xmlNodePtr parent, const char* nodeName, const char* attrName)
{
    xmlNodePtr cur = NULL;
    xmlNodePtr ret = NULL;
    xmlChar*   attr;

    //TRACE("entry: parent [%p] [%s]", parent, parent ? parent->name : NULL);
    //TRACE("entry: node [%s] attr [%s]", nodeName, attrName);

    if (parent && nodeName && nodeName[0] != '\0')
    {
        cur = parent->children;
        while (cur != NULL  &&  ret == NULL)
        {
            if ( xmlStrcmp(cur->name, (const xmlChar*)nodeName) == 0 )
            {
                attr = xmlGetProp(cur, attrName);
                if (attr == NULL)
                {
                    //TRACE("Found child node %s", nodeName);
                    ret = cur;
                }
                else
                {
                    xmlFree(attr);
                }
            }
            cur = cur->next;
        }
    }
    return ret;
}

int ermXmlExist(const erManifest *pCtx, const char *express)
{
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return RET_ERR;         // nothing

    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    if (nodes && nodes->nodeNr > 0 && nodes->nodeTab[0])
    {
        xmlXPathFreeObject(xpathObj);
        return RET_OK;
    }
    xmlXPathFreeObject(xpathObj);
    return RET_ERR;
}


/* open manifest file */
int ermXmlOpenFile(const char *szFileName, erManifest *pCtx)
{
    pCtx->pDoc = NULL;
    pCtx->pPathCtx = NULL;
    pCtx->szFileName = NULL;

    if (NULL == szFileName)
    {
        TRACE("szFileName pointer is empty!\n");
        return RET_ERR;
    }

    if (0 >= strlen(szFileName))
    {
        TRACE("length of szFileName is zero!\n");
        return RET_ERR;
    }

    pCtx->pDoc = xmlParseFile(szFileName);
    if (NULL == pCtx->pDoc)
        return RET_ERR;

    pCtx->pPathCtx = xmlXPathNewContext(pCtx->pDoc);
    if (NULL == pCtx->pPathCtx)
    {
        xmlFreeDoc(pCtx->pDoc);
        pCtx->pDoc = NULL;
        return RET_ERR;
    }

    pCtx->szFileName = strdup(szFileName);
    return RET_OK;
}

/* open manifest file */
int ermXmlOpenManifest(const char *szContainerName, erManifest *pCtx)
{
    int ret;
    char *xpath;
    char buf[100];
    
    if (NULL == szContainerName)
    {
        TRACE("szContainerName pointer is empty!\n");
        return RET_ERR;
    }

    int n = strlen(szContainerName);
    if (0 >= n)
    {
        TRACE("length of szFileContainer is zero!\n");
        return RET_ERR;
    }

    char *szFileName = alloca(n + 1 + strlen(MANIFEST_FILENAME) + 1);
    if (NULL == szFileName)
    {
        TRACE("alloca failedi!\n");
        return RET_ERR;
    }
    sprintf(szFileName, "%s/" MANIFEST_FILENAME, szContainerName);

    ret = ermXmlOpenFile(szFileName, pCtx);
    if (ret == RET_OK)
    {
        // fix issues in case of old version manifest file:

        //   remove dc-metadata/Date if empty
        xpath = "/package/metadata/dc-metadata/Date";
        if (   ermXmlGetString(pCtx, xpath, buf, sizeof(buf)) == RET_OK
            && buf[0] == '\0' )
        {
            ermXmlRemoveNode(pCtx, xpath);
        }

        //   remove dc-metadata/Identified if empty
        xpath = "/package/metadata/dc-metadata/Identifier";
        if (   ermXmlGetString(pCtx, xpath, buf, sizeof(buf)) == RET_OK
            && buf[0] == '\0' )
        {
            ermXmlRemoveNode(pCtx, xpath);
        }

        //   remove dc-metadata/Type if empty
        xpath = "/package/metadata/dc-metadata/Type";
        if (   ermXmlGetString(pCtx, xpath, buf, sizeof(buf)) == RET_OK
            && buf[0] == '\0' )
        {
            ermXmlRemoveNode(pCtx, xpath);
        }

        //   remove dc-metadata/Format
        xpath = "/package/metadata/dc-metadata/Format";
        ermXmlRemoveNode(pCtx, xpath);

        //   remove dc-metadata/Language
        xpath = "/package/metadata/dc-metadata/Language";
        ermXmlRemoveNode(pCtx, xpath);
    }

    return ret;
}

/* validate manifest file */
int ermXmlValidateManifest(erManifest *pCtx)
{
    int  retVal = RET_ERR;

    xmlSchemaParserCtxtPtr  parserCtxtPtr = NULL;
    xmlSchemaValidCtxtPtr   validCtxPtr   = NULL;  
    xmlSchemaPtr            schema        = NULL;

    int result;

    if (NULL == pCtx->pDoc)
    {
        TRACE("pCtx->pDoc pointer is empty!\n");        
        return retVal;
    }

    // get XML schema
    parserCtxtPtr = xmlSchemaNewParserCtxt(MANIFEST_SCHEMA_FILE);
    if (parserCtxtPtr)
    {
        schema = xmlSchemaParse(parserCtxtPtr);
        if (schema)
        {
            validCtxPtr = xmlSchemaNewValidCtxt(schema);
            if (validCtxPtr)
            {
                if (xmlSchemaIsValid(validCtxPtr))
                {
                    // validate XML document
                    result = xmlSchemaValidateDoc(validCtxPtr, pCtx->pDoc);
                    TRACE("ValidateDoc returns [%d] (0 == OK)\n", result);        
                    if (result == 0)
                    {
                        retVal = RET_OK;
                    }
                }
                xmlSchemaFreeValidCtxt(validCtxPtr);
            }
            xmlSchemaFree(schema);
        }
    }
    xmlSchemaFreeParserCtxt(parserCtxtPtr);

    return retVal;
}

/* create a new xml handle only, do not save anything till  it is saved
   explicitly. */
int ermXmlCreateHandleOnly(erManifest *pCtx)
{
    pCtx->pDoc = NULL;
    pCtx->pPathCtx = NULL;
    pCtx->szFileName = NULL;

    pCtx->pDoc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
    if (NULL == pCtx->pDoc)
        return RET_ERR;

    pCtx->pPathCtx = xmlXPathNewContext(pCtx->pDoc);
    if (NULL == pCtx->pPathCtx)
    {
        xmlFreeDoc(pCtx->pDoc);
        return RET_ERR;
    }

    return RET_OK;
}

/* create a new xml file */
int ermXmlCreateFile(const char *szFileName, erManifest *pCtx)
{
    if (NULL == szFileName)
    {
        TRACE("szFileName pointer is empty!\n");
        return RET_ERR;
    }

    if (0 >= strlen(szFileName))
    {
        TRACE("length of szFileName is zero!\n");
        return RET_ERR;
    }

    int iRet = ermXmlCreateHandleOnly(pCtx);
    if (RET_OK == iRet)
    {
        iRet = ermXmlSaveAs(pCtx, szFileName);
    }
    if (RET_OK != iRet)
    {
        ermXmlClose(pCtx);
    }
    return iRet;
}

/* create manifest file from container */
int ermXmlCreateManifest(const char *szContainerName, erManifest *pCtx)
{
    if (NULL == szContainerName)
    {
        TRACE("szContainerName pointer is empty!\n");
        return RET_ERR;
    }

    int n = strlen(szContainerName);
    if (0 >= n)
    {
        TRACE("length of szFileContainer is zero!\n");
        return RET_ERR;
    }

    char *szFileName = alloca(n + 1 + strlen(MANIFEST_FILENAME) + 1);
    if (NULL == szFileName)
    {
        TRACE("alloca failed!\n");
        return RET_ERR;
    }
    sprintf(szFileName, "%s/" MANIFEST_FILENAME, szContainerName);

    return ermXmlCreateFile(szFileName, pCtx);
}

int ermXmlParseBuffer(const char* xmlString, const char* szFileName, erManifest *pCtx)
{
    pCtx->pDoc = NULL;
    pCtx->pPathCtx = NULL;
    pCtx->szFileName = NULL;

    if (NULL == xmlString)
    {
        TRACE("xmlString pointer is empty!\n");
        return RET_ERR;
    }

    if (NULL == szFileName)
    {
        TRACE("szFileName pointer is empty!\n");
        return RET_ERR;
    }

    if (0 >= strlen(szFileName))
    {
        TRACE("length of szFileName is zero!\n");
        return RET_ERR;
    }

    pCtx->pDoc = xmlParseMemory(xmlString, strlen(xmlString));
    if (NULL == pCtx->pDoc)
    {
        TRACE("xmlParseMemory failed!\n");
        return RET_ERR;
    }

    pCtx->pPathCtx = xmlXPathNewContext(pCtx->pDoc);
    if (NULL == pCtx->pPathCtx)
    {
        TRACE("xmlXPathNewContext failed!\n");
        xmlFreeDoc(pCtx->pDoc);
        pCtx->pDoc = NULL;
        return RET_ERR;
    }

    pCtx->szFileName = strdup(szFileName);
    return RET_OK;
}

// Report XML filename
const char *ermXmlGetFileName(const erManifest *pCtx)
{
    return pCtx->szFileName;
}

int ermXmlGetNodeNr(const erManifest *pCtx, const char *express, int *number)
{
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return RET_ERR;

    /* read from path object */
    xmlNodeSetPtr nodes = xpathObj->nodesetval;
    if (nodes)
    {
        *number = nodes->nodeNr;
        xmlXPathFreeObject(xpathObj);
        return RET_OK;
    }
    xmlXPathFreeObject(xpathObj);
    return RET_ERR;
}

int ermXmlGetString(const erManifest *pCtx, const char *express, char *pRet, const int length)
{
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return RET_ERR;

    /* read from path object */
    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    if (nodes && nodes->nodeNr > 0 && nodes->nodeTab[0] && nodes->nodeTab[0]->children)
    {
        //xmlChar * content = xmlNodeGetContent(&nodes->nodeTab[0]->children[0]);
        xmlChar *content = xmlNodeGetContent(nodes->nodeTab[0]->children);

        strncpy(pRet, content, length - 1);
        pRet[length - 1] = '\0';
        xmlXPathFreeObject(xpathObj);
        /*we should release the memory, otherwise memory leak */
        xmlFree(content);
        return RET_OK;
    }
    else if (nodes && nodes->nodeNr > 0 && nodes->nodeTab[0])
    {
        pRet[0] = '\0';
        xmlXPathFreeObject(xpathObj);
        return RET_OK;
    }

    TRACE("No nodes found!");
    xmlXPathFreeObject(xpathObj);
    return RET_ERR;
}

const char *ermXmlGetStringBuffer(const erManifest *pCtx, const char *express, int *length)
{
    *length = 0;
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return NULL;

    /* read from path object */
    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    if (nodes && nodes->nodeNr > 0 && nodes->nodeTab[0] && nodes->nodeTab[0]->children)
    {
        xmlChar *content = xmlNodeGetContent(&nodes->nodeTab[0]->children[0]);
        *length = xmlStrlen(content);
        xmlXPathFreeObject(xpathObj);
        return (const char *)content;
    }

    TRACE("No nodes found!\n");
    xmlXPathFreeObject(xpathObj);
    return NULL;
}

void ermXmlFreeStringBuffer(const char *ptr)
{
    if (NULL == ptr)
    {
        TRACE("Attempt to free null pointer!\n");
        return;
    }
    xmlFree((xmlChar *) ptr);
}

int ermXmlGetInt(const erManifest *pCtx, const char *express, int *pRet)
{
    /* for int */
    char tmp[INT_MAX_BUF] = { 0 };

    if (RET_ERR == ermXmlGetString(pCtx, express, tmp, INT_MAX_BUF))
        return RET_ERR;

    if (1 == sscanf(tmp, "%d", pRet))
        return RET_OK;
    return RET_ERR;
}

int ermXmlGetBoolean(const erManifest *pCtx, const char *express, int *pRet)
{
    /* for int */
    char tmp[BOOL_MAX_BUF] = { 0 };

    if (RET_ERR == ermXmlGetString(pCtx, express, tmp, BOOL_MAX_BUF))
        return RET_ERR;

    if (0 == strcmp(tmp, "false"))
    {
        *pRet = 0;
        return RET_OK;
    }
    if (0 == strcmp(tmp, "true"))
    {
        *pRet = 1;
        return RET_OK;
    }
    return RET_ERR;
}

int ermXmlGetAttributeString(const erManifest *pCtx, const char *express,
                             const char *attrName, char *pRet, const int length)
{
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return RET_ERR;

    /* read from path object */
    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    if (nodes && nodes->nodeNr && nodes->nodeTab[0])
    {
        xmlChar *pSrc = xmlGetProp(nodes->nodeTab[0], attrName);
        strncpy(pRet, pSrc, length - 1);
        pRet[length - 1] = '\0';
        xmlXPathFreeObject(xpathObj);
        /* we should release the memory */
        /* http://www.xmlsoft.org/html/libxml-tree.html#xmlGetProp */
        xmlFree(pSrc);
        return RET_OK;
    }

    xmlXPathFreeObject(xpathObj);
    return RET_ERR;
}

const char *ermXmlGetAttributeStringBuffer(const erManifest *pCtx, const char *express, const char *attrName, int *length)
{
    *length = 0;
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return NULL;

    /* read from path object */
    xmlNodeSetPtr nodes = xpathObj->nodesetval;
    if (nodes && nodes->nodeNr && nodes->nodeTab[0])
    {
        xmlChar *pSrc = xmlGetProp(nodes->nodeTab[0], attrName);
        xmlXPathFreeObject(xpathObj);
        *length = xmlStrlen(pSrc);
        return pSrc;
    }

    xmlXPathFreeObject(xpathObj);
    return NULL;
}

int ermXmlGetAttributeInt(const erManifest *pCtx, const char *express, const char *attrName, int *pRet)
{
    char tmp[INT_MAX_BUF] = { 0 };
    if (RET_OK != ermXmlGetAttributeString(pCtx, express, attrName, tmp, INT_MAX_BUF))
        return RET_ERR;
    if (1 == sscanf(tmp, "%d", pRet))
        return RET_OK;
    return RET_ERR;
}

/* only change the menory */
int ermXmlSetString(erManifest *pCtx, const char *express, const char *pData)
{
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return RET_ERR;

    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    /* MvdW: we need to encode special chars in the input */
    xmlChar *xmlString = xmlEncodeSpecialChars(pCtx->pDoc, (xmlChar *) pData);

    if (nodes && nodes->nodeTab[0])
    {
        xmlNodeSetContent(nodes->nodeTab[0], xmlString);
        xmlFree(xmlString);

        /* See http://www.xmlsoft.org/examples/xpath2.c for detail */
        if (nodes->nodeTab[0]->type != XML_NAMESPACE_DECL)
            nodes->nodeTab[0] = NULL;
        xmlXPathFreeObject(xpathObj);
        return RET_OK;
    }
    xmlXPathFreeObject(xpathObj);
    return RET_ERR;
}

int ermXmlSetInt(erManifest *pCtx, const char *express, const int data)
{
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return RET_ERR;

    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    if (nodes && nodes->nodeTab[0])
    {
        char tmp[INT_MAX_BUF] = { 0 };
        snprintf(tmp, INT_MAX_BUF, "%d", data);
        xmlNodeSetContent(nodes->nodeTab[0], tmp);

        if (nodes->nodeTab[0]->type != XML_NAMESPACE_DECL)
            nodes->nodeTab[0] = NULL;
        xmlXPathFreeObject(xpathObj);
        return RET_OK;
    }
    xmlXPathFreeObject(xpathObj);
    return RET_ERR;
}

int ermXmlSetBoolean(erManifest *pCtx, const char *express, const int data)
{
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return RET_ERR;
    if (0 != data && 1 != data)
        return RET_ERR;

    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    if (nodes && nodes->nodeTab[0])
    {
        const char *tmp = data ? "true" : "false";
        xmlNodeSetContent(nodes->nodeTab[0], tmp);

        if (nodes->nodeTab[0]->type != XML_NAMESPACE_DECL)
            nodes->nodeTab[0] = NULL;
        xmlXPathFreeObject(xpathObj);
        return RET_OK;
    }
    xmlXPathFreeObject(xpathObj);
    return RET_ERR;
}

/* MvdW: This function does not do ANY checking on the input data. BAD_CAST should actually be replaced with a 
   function that does escaping and entity encoding! */
int ermXmlSetAttributeString(const erManifest *pCtx, const char *express,
                             const char *attrName, const char *pSrc, const int length)
{
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return RET_ERR;

    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    if (nodes && nodes->nodeNr && nodes->nodeTab[0])
    {
        xmlSetProp(nodes->nodeTab[0], attrName, BAD_CAST pSrc);
        xmlXPathFreeObject(xpathObj);
        return RET_OK;
    }
    xmlXPathFreeObject(xpathObj);
    return RET_ERR;
}


int ermXmlSetAttributeInt(erManifest *pCtx, const char *express, const char *attrName, const int data)
{
    char tmp[INT_MAX_BUF] = { 0 };
    snprintf(tmp, INT_MAX_BUF, "%d", data);
    return ermXmlSetAttributeString(pCtx, express, attrName, tmp, INT_MAX_BUF);
}

/* Create a new element with string value */
int ermXmlNewString(const erManifest *pCtx, const char *parentExpress, const char *name, const char *content)
{
    xmlXPathObjectPtr xpathObj = LocateTo(parentExpress, pCtx);
    if (NULL == xpathObj)
        return RET_ERR;

    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    /* MvdW: we need to encode special chars in the input */
    xmlChar *xmlString = xmlEncodeSpecialChars(pCtx->pDoc, (xmlChar *) content);

    if (nodes && nodes->nodeTab[0])
    {
        /*xmlNodePtr node = */ xmlNewChild(nodes->nodeTab[0],
                                           NULL, BAD_CAST name, // MvdW: this should ALSO be encoded!
                                           xmlString);
        xmlFree(xmlString);
        xmlXPathFreeObject(xpathObj);
        return RET_OK;
    }
    xmlXPathFreeObject(xpathObj);
    return RET_ERR;
}

int ermXmlNewInt(erManifest *pCtx, const char *parentExpress, const char *name, const int content)
{
    char tmp[INT_MAX_BUF] = { 0 };
    snprintf(tmp, INT_MAX_BUF, "%d", content);
    return ermXmlNewString(pCtx, parentExpress, name, tmp);
}

int ermXmlNewBoolean(erManifest *pCtx, const char *parentExpress, const char *name, const int content)
{
    if (0 != content && 1 != content)
        return RET_ERR;

    const char *tmp = content ? "true" : "false";
    return ermXmlNewString(pCtx, parentExpress, name, tmp);
}


int ermXmlRemoveNode(erManifest *pCtx, const char *express)
{
    xmlXPathObjectPtr xpathObj = LocateTo(express, pCtx);
    if (NULL == xpathObj)
        return RET_ERR;

    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    if (nodes && nodes->nodeTab[0])
    {
        xmlUnlinkNode(nodes->nodeTab[0]);
        xmlFreeNode(nodes->nodeTab[0]);
        xmlXPathFreeObject(xpathObj);
        return RET_OK;
    }
    xmlXPathFreeObject(xpathObj);
    return RET_ERR;
}


// Save current memory XML Doc into file
int ermXmlSave(erManifest *pCtx)
{
    if (NULL == pCtx->szFileName)
    {
        TRACE("pCtx->szFileName pointer is empty!\n");
        return RET_ERR;
    }
    if (0 >= strlen(pCtx->szFileName))
    {
        TRACE("length of pCtx->szFileName is zero!\n");
        return RET_ERR;
    }

    int n = xmlSaveFileEnc(pCtx->szFileName, pCtx->pDoc, DEFAULT_ENCODING);
    sync();
    return (n >= 0) ? RET_OK : RET_ERR;
}

// Save current memory XML Doc into a specified file
int ermXmlSaveAs(erManifest *pCtx, const char *szFileName)
{
    if (NULL == szFileName)
    {
        TRACE("szFileName pointer is empty!\n");
        return RET_ERR;
    }

    if (0 >= strlen(szFileName))
    {
        TRACE("length of szFileName is zero!\n");
        return RET_ERR;
    }

    if (pCtx->szFileName)
    {
        free(pCtx->szFileName);
    }
    pCtx->szFileName = strdup(szFileName);

    int iRet = ermXmlSave(pCtx);
    return iRet;
}

// Save current memory XML Doc into file and close the manifest
int ermXmlSaveAndClose(erManifest *pCtx)
{
    int iRet = ermXmlSave(pCtx);
    ermXmlClose(pCtx);
    return iRet;
}

// Close the manifest file WITHOUT saving
int ermXmlClose(erManifest *pCtx)
{
    if (pCtx->pDoc)
    {
        xmlFreeDoc(pCtx->pDoc);
        pCtx->pDoc = NULL;
    }
    if (pCtx->pPathCtx)
    {
        xmlXPathFreeContext(pCtx->pPathCtx);
        pCtx->pPathCtx = NULL;
    }
    if (pCtx->szFileName)
    {
        free(pCtx->szFileName);
        pCtx->szFileName = NULL;
    }
    return RET_OK;
}

/*****************************************************************************/
typedef struct
{
    char *name;
    char *value;
} pairs_t;

typedef enum
{
    with_none_t,
    with_index_t,
    with_attr_t,
    with_child_t
} with_type_t;

typedef struct
{
    char *xpath;
    char *name;

    with_type_t type;
    union
    {
        pairs_t pairs;
        int index;
    } u;
} a_step_t;

static a_step_t *newAStep(void)
{
    a_step_t *theStep = (a_step_t *) malloc(sizeof(a_step_t));
    if (theStep)
    {
        memset(theStep, 0, sizeof(a_step_t));
    }
    return theStep;
}

static void freeAStep(a_step_t * theStep)
{
    if (theStep)
    {
        if (theStep->xpath)
        {
            free(theStep->xpath);
            theStep->xpath = NULL;
        }

        if (theStep->name)
        {
            free(theStep->name);
            theStep->name = NULL;
        }

        if ((theStep->type == with_attr_t) || (theStep->type == with_child_t))
        {
            if (theStep->u.pairs.name)
            {
                free(theStep->u.pairs.name);
                theStep->u.pairs.name = NULL;
            }

            if (theStep->u.pairs.value)
            {
                free(theStep->u.pairs.value);
                theStep->u.pairs.value = NULL;
            }
        }

        free(theStep);
    }
}

/*
    calculate the postions of all delimiters in a step

    examples:
        /application[type="browser_e"]/
        /lastRead[@type="books_e"]/
*/
static void calcDelimiterPositions(char *curSlashPos, char *nextSlashPos,
                                   char **beginBracketPos, char **atPos, char **equalPos, char **endBracketPos)
{
    char *cp = NULL;

    cp = curSlashPos + 1;
    while (cp < nextSlashPos)
    {
        switch (*cp)
        {
            case '[':
                *beginBracketPos = cp;
                break;
            case ']':
                *endBracketPos = cp;
                break;
            case '@':
                *atPos = cp;
                break;
            case '=':
                *equalPos = cp;
                break;
            default:
                break;
        }
        cp++;
    }
}

/*
   get the attribute or sub-element's name and value 

   examples:
        extName="pdf"
        type="browser_e"

    notes: don't forget the memory malloced here
*/
static void getNameAndValue(char *beginPos, char *equalPos, char *endPos, char **name, char **value)
{
    char *szName = NULL, *szValue = NULL;
    int nNameLen, nValueLen;

    nNameLen = equalPos - beginPos;
    nValueLen = endPos - equalPos - 2;

    szName = (char *)malloc(nNameLen + 1);
    szValue = (char *)malloc(nValueLen + 1);

    if (szName)
    {
        strncpy(szName, beginPos, nNameLen);
        szName[nNameLen] = '\0';
    }

    if (szValue)
    {
        strncpy(szValue, equalPos + 2, nValueLen);
        szValue[nValueLen] = '\0';
    }

    *name = szName;
    *value = szValue;
}

/*
   get the index of element 

   examples: 3
*/
static void getIndex(const char *begin, const char *end, int *index)
{
    char *pBuffer = NULL;
    int nLen;

    *index = 0;

    nLen = end - begin + 1;
    pBuffer = malloc(nLen + 1);
    if (pBuffer)
    {
        strncpy(pBuffer, begin, nLen);
        pBuffer[nLen] = '\0';
        *index = atoi(pBuffer);
        free(pBuffer);
        pBuffer = NULL;
    }
}

static int lexicalizeAStep(char *express, char *curSlashPos, a_step_t * data)
{
    char *nextSlashPos = NULL;
    char *beginBracketPos = NULL, *endBracketPos = NULL;
    char *atPos = NULL, *equalPos = NULL;
    int nLen = 0;

    nextSlashPos = strchr(curSlashPos + 1, '/');
    // reach the end
    if (NULL == nextSlashPos)
    {
        nextSlashPos = (express + strlen(express));
    }

    // get the positions of delimiters in one step
    calcDelimiterPositions(curSlashPos, nextSlashPos, &beginBracketPos, &atPos, &equalPos, &endBracketPos);

    // data->xpath
    nLen = curSlashPos - express;
    data->xpath = malloc(nLen + 1);
    if (NULL == data->xpath)
    {
        return RET_ERR;
    }
    snprintf(data->xpath, nLen + 1, "%s", express);
    
    if (beginBracketPos && endBracketPos)
    {
        // data->name
        nLen = beginBracketPos - (curSlashPos + 1);
        data->name = malloc(nLen + 1);
        if (NULL == data->name)
        {
            return RET_ERR;
        }
        snprintf(data->name, nLen + 1, "%s", curSlashPos + 1);

        if (atPos)
        {
            // has attribute
            data->type = with_attr_t;
            getNameAndValue(atPos + 1, equalPos, endBracketPos - 1, &data->u.pairs.name, &data->u.pairs.value);
        }
        else if (equalPos)
        {
            // has sub-element
            data->type = with_child_t;
            getNameAndValue(beginBracketPos + 1, equalPos, endBracketPos - 1,
                            &data->u.pairs.name, &data->u.pairs.value);
        }
        else
        {
            // has index
            data->type = with_index_t;
            getIndex(beginBracketPos + 1, endBracketPos - 1, &data->u.index);
        }
    }
    else
    {
        // has nothing
        data->type = with_none_t;
        nLen = nextSlashPos - (curSlashPos + 1);

        data->name = malloc(nLen + 1);
        if (NULL == data->name)
        {
            return RET_ERR;
        }
        snprintf(data->name, nLen + 1, "%s", curSlashPos + 1);
    }

    return (nextSlashPos - curSlashPos);
}

#define BUF_LEN 1024
static int checkAStep(const erManifest *pCtx, char *express, char *curSlashPos)
{
    a_step_t *data = NULL;
    char xpath[BUF_LEN + 1];
    int nChars, nLen, nRet;

    data = newAStep();
    nChars = lexicalizeAStep(express, curSlashPos, data);
    if (RET_ERR == nChars)
    {
        freeAStep(data);
        data = NULL;
        return RET_ERR;
    }

    switch (data->type)
    {
        case with_attr_t:
            // has attribute
            snprintf(xpath, BUF_LEN, "%s/%s[@%s=\"%s\"]",
                     data->xpath, data->name, data->u.pairs.name, data->u.pairs.value);
            if (RET_ERR == ermXmlExist(pCtx, xpath))
            {
                if (strlen(data->xpath))
                {
                    nRet = ermXmlNewString(pCtx, data->xpath, data->name, "");
                }
                else
                {
                    // it's root element
                    nRet = ermXmlNewString(pCtx, "/", data->name, "");
                }
                
                if (RET_ERR == nRet)
                {
                    freeAStep(data);
                    data = NULL;
                    return RET_ERR;
                }

                snprintf(xpath, BUF_LEN, "%s/%s[last()]", data->xpath, data->name);
                nRet =
                    ermXmlSetAttributeString(pCtx, xpath, data->u.pairs.name,
                                             data->u.pairs.value, strlen(data->u.pairs.value));
                if (RET_ERR == nRet)
                {
                    freeAStep(data);
                    data = NULL;
                    return RET_ERR;
                }
            }
            break;
        case with_child_t:
            // has sub-element
            nLen = snprintf(xpath, BUF_LEN, "%s/%s[%s=\"%s\"]",
                            data->xpath, data->name, data->u.pairs.name, data->u.pairs.value);
            if (RET_ERR == ermXmlExist(pCtx, xpath))
            {
                if (strlen(data->xpath))
                {
                    nRet = ermXmlNewString(pCtx, data->xpath, data->name, "");
                }
                else
                {
                    // it's root element
                    nRet = ermXmlNewString(pCtx, "/", data->name, "");
                }
                
                if (RET_ERR == nRet)
                {
                    freeAStep(data);
                    data = NULL;
                    return RET_ERR;
                }

                nLen = snprintf(xpath, BUF_LEN, "%s/%s[last()]", data->xpath, data->name);
                nRet = ermXmlNewString(pCtx, xpath, data->u.pairs.name, data->u.pairs.value);
                if (RET_ERR == nRet)
                {
                    freeAStep(data);
                    data = NULL;
                    return RET_ERR;
                }
            }
            break;
        case with_index_t:
            // has index
            nLen = snprintf(xpath, BUF_LEN, "%s/%s[%d]", data->xpath, data->name, data->u.index);
            if (RET_ERR == ermXmlExist(pCtx, xpath))
            {
                int nNodes = 0;

                nLen = snprintf(xpath, BUF_LEN, "%s/%s", data->xpath, data->name);
                nRet = ermXmlGetNodeNr(pCtx, xpath, &nNodes);
                while (nNodes < data->u.index)
                {
                    if (strlen(data->xpath))
                    {
                        nRet = ermXmlNewString(pCtx, data->xpath, data->name, "");
                    }
                    else
                    {
                        // it's root element
                        nRet = ermXmlNewString(pCtx, "/", data->name, "");
                    }
                    
                    if (RET_ERR == nRet)
                    {
                        freeAStep(data);
                        data = NULL;
                        return RET_ERR;
                    }
                    nNodes++;
                }
            }
            break;
        case with_none_t:
        default:
            // has nothing
            nLen = snprintf(xpath, BUF_LEN, "%s/%s", data->xpath, data->name);
            if (RET_ERR == ermXmlExist(pCtx, xpath))
            {
                if (strlen(data->xpath))
                {
                    nRet = ermXmlNewString(pCtx, data->xpath, data->name, "");
                }
                else
                {
                    // it's root element
                    nRet = ermXmlNewString(pCtx, "/", data->name, "");
                }

                if (RET_ERR == nRet)
                {
                    freeAStep(data);
                    data = NULL;
                    return RET_ERR;
                }
            }
            break;
    }

    freeAStep(data);
    data = NULL;

    return nChars;
}

/*
    check the xpath indicated by 'express' exists or not.
    if not exists, create it and its ancestors automatically,
    also create attributes automatically as well.

    notes: only partly supports xpath as below
    
    /iLiadRegistry/userSetting/name
    /iLiadRegistry/pcProfileList/pcProfile[i]
    /iLiadRegistry/pcProfileList/pcProfile[@profileName="PC_0"]
    /iLiadRegistry/categories/category[@type="books_e"]
    /iLiadRegistry/lastReads/lastRead[@type="books_e"]
    /iLiadRegistry/applications/application[type="browser_e"]
    /iLiadRegistry/application/application[type="browser_e"]/supportFormat/extName[i]
*/
int ermXmlCheckXpath(erManifest * pCtx, char *express)
{
    int i, nChars;

    if ((NULL == pCtx) || (NULL == express))
    {
        return RET_ERR;
    }

    if (RET_OK == ermXmlExist(pCtx, express))
    {
        return RET_OK;
    }

    i = 0;
    while (express[i])
    {
        nChars = checkAStep(pCtx, express, express + i);
        if (RET_ERR == nChars)
        {
            return RET_ERR;
        }
        i += nChars;
    }

    return RET_OK;
}
