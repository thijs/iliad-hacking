/**
 * \file ermanifest.h
 * \brief eReader manifest 
 *
 * <File description>
 * Use xml XPath to provide easy using interface.
 */

/* 
 * Copyright (C) 2005 Philips Apptech
 *
 */

#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#ifndef _ER_MANIFEST__H__
#define _ER_MANIFEST__H__

#define RET_OK      0
#define RET_ERR     -1

#define MANIFEST_FILENAME     "manifest.xml"
#define MANIFEST_SCHEMA_FILE  (DATA_DIR "/manifest.xsd")


#ifdef __cplusplus
extern "C"
{

#endif                          /* __cplusplus */

    typedef struct
            {
                xmlDocPtr          pDoc;
                xmlXPathContextPtr pPathCtx;
                char               *szFileName;
            } erManifest;

/**
 * Open a XML file for reading/writing
 * 
 * @param szFileName File location
 * @param pCtx erManifest struct pointer
 *
 * @returns RET_OK means open operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlOpenFile(const char *szFileName, erManifest *pCtx);

/**
 * Open a XML manifest file for reading/writing
 * 
 * @param szContainerName Container location
 * @param pCtx erManifest struct pointer
 *
 * @returns RET_OK means open operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlOpenManifest(const char *szContainerName, erManifest *pCtx);

/**
 * Validate manifest file against XML schema definition
 * May modify XML to account for default values in XML schema definition
 *
 * @param pCtx erManifest struct pointer
 *
 * @returns RET_OK when manifest validated ok, otherwise RET_ERR
 */
int ermXmlValidateManifest(erManifest *pCtx);

/**
 * Create a default XML file for reading/writing
 * It will overwrite existing one. 
 * 
 * @param szFileName File location
 * @param pCtx erManifest struct pointer
 *
 * @returns RET_OK means open operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlCreateFile(const char *szFileName, erManifest *pCtx);

/**
 * Create a default XML manifest file for reading/writing
 * It will overwrite existing one. 
 * 
 * @param szContainerName Container location
 * @param pCtx erManifest struct pointer
 *
 * @returns RET_OK means open operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlCreateManifest(const char *szContainerName, erManifest *pCtx);

/**
 * Report XML filename
 *
 * @param pCtx erManifest struct pointer
 *
 * @returns filename or NULL pointer
 */
    const char *ermXmlGetFileName(const erManifest *pCtx);

/**
 * create a new xml handle only, do not save anything till  
 * it is saved explicitly, for reading/writing.
 * 
 * @param pCtx erManifest struct pointer
 *
 * @returns RET_OK means open operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlCreateHandleOnly(erManifest *pCtx);

/**
 * Read a nodelist from a manifest file accorting to XPath express
 *
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 *
 * @returns pointer to the xpath object, or NULL when not found
 */
    xmlXPathObjectPtr LocateTo(const char *express, const erManifest *pCtx);
#define ermXmlSelectNodes(pCtx, express)  LocateTo( (express), (pCtx) )

/**
 * Find first child node with specified name
 *
 * @param parent - ptr to the parent node
 * @param childName - name of the child node to find
 *
 * @returns pointer to the child node, or NULL if not found
 */
xmlNodePtr ermXmlGetChildNode(xmlNodePtr parent, const char* childName);

/**
 * Find all child node with specified name
 *
 * @param parent - ptr to the parent node
 * @param childName - name of the child node to find
 *
 * @returns pointer to set of child nodes, must be released using xmlXPathFreeNodeSet()
 */
xmlNodeSetPtr ermXmlGetChildNodeSet(xmlNodePtr parent, const char* childName);

/**
 * Find child node on name and attribute
 *
 * @param parent - ptr to the parent node
 * @param nodeName - name of the child node to find
 * @param attrName - name of the attribute that must be present on the child node
 * @param attrValue - required value of the attribute, or NULL if don't care
 *
 * @returns pointer to the child node, or NULL if not found
 */
    xmlNodePtr ermXmlGetChildNodeWithAttr(xmlNodePtr parent, const char *nodeName, const char *attrName,
                                          const char *attrValue);

    int ermXmlGetNodeNr(const erManifest *pCtx, const char *express, int *number);

/**
 * Find child node on name but without the specified attribute
 *
 * @param parent - ptr to the parent node
 * @param nodeName - name of the child node to find
 * @param attrName - name of the attribute that must NOT be present on the child node
 *
 * @returns pointer to the child node, or NULL if not found
 */
xmlNodePtr ermXmlGetChildNodeWithoutAttr(xmlNodePtr parent, const char* nodeName, const char* attrName);

/**
 * Read a string from a manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param pRet char buffer pointer used to store result
 * @param length char buffer length
 *
 * @returns RET_OK means get operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlGetString(const erManifest *pCtx, const char *express, char *pRet, const int length);


/**
 * Read a string from a manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param length of buffer
 *
 * @returns content start address
 */
    const char *ermXmlGetStringBuffer(const erManifest *pCtx, const char *express, int *length);


/**
 * Free the memory buffer allocated by ermXmlGetStringBuffer
 *
 * @param ptr the pointer returned by ermXmlGetStringBuffer
 */
    void ermXmlFreeStringBuffer(const char *ptr);


/**
 * Read a int from a manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param pRet int buffer pointer used to store result
 *
 * @returns RET_OK means get operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlGetInt(const erManifest *pCtx, const char *express, int *pRet);

/**
 * Read a boiolean from a manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param pRet int buffer pointer used to store result (0=false, 1=true)
 *
 * @returns RET_OK means get operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlGetBoolean(const erManifest *pCtx, const char *express, int *pRet);

/**
 * Check an element is existing or not
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 *
 *
 * @returns RET_OK means exist otherwise returns RET_ERR 
 */
    int ermXmlExist(const erManifest *pCtx, const char *express);

/**
 * Read a attribute string from a manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param attrName attribute name
 * @param pRet char buffer pointer used to store result
 * @param length char buffer length
 *
 * @returns RET_OK means get operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlGetAttributeString(const erManifest *pCtx, const char *express, const char *attrName, char *pRet,
                                 const int length);

/**
 * Get attribute string buffer directly from a manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param attrName attribute name
 * @param length of attribute string
 *
 * @returns buffer pointer
 */
    const char *ermXmlGetAttributeStringBuffer(const erManifest *pCtx, const char *express, const char *attrName,
                                               int *length);

/**
 * Read a int attribute from a manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param attrName attribute name
 * @param pRet int buffer pointer used to store result
 *
 * @returns RET_OK means get operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlGetAttributeInt(const erManifest *pCtx, const char *express, const char *attrName, int *pRet);


/**
 * Update the existing element in the manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param pData the new string value (null-terminated)
 *
 * @returns RET_OK means set operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlSetString(erManifest *pCtx, const char *express, const char *pData);

/**
 * Update the existing element in the manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param data the new integer value
 * @param length string length
 *
 * @returns RET_OK means set operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlSetInt(erManifest *pCtx, const char *express, const int data);

/**
 * Update the existing element in the manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param data the new boolean value (0=false, 1=true)
 * @param length string length
 *
 * @returns RET_OK means set operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlSetBoolean(erManifest *pCtx, const char *express, const int data);

/**
 * Write or Create an attribute string to a manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param attrName attribute name
 * @param pSrc char buffer pointer contains source content
 * @param length char buffer length
 *
 * @returns RET_OK means get operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlSetAttributeString(const erManifest *pCtx, const char *express, const char *attrName, const char *pSrc,
                                 const int length);

/**
 * Write or Create an attribute integer to a manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param attrName attribute name
 * @param data source integer data
 *
 * @returns RET_OK means get operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlSetAttributeInt(erManifest *pCtx, const char *express, const char *attrName, const int data);


/**
 * Create a new string element in the manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param parentExpress XPath express
 * @param name the new element name
 * @param content the new element value
 *
 * @returns RET_OK means new operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlNewString(const erManifest *pCtx, const char *parentExpress, const char *name, const char *content);

/**
 * Create a new int element in the manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param parentExpress XPath express
 * @param name the new element name
 * @param content the new element value
 *
 * @returns RET_OK means new operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlNewInt(erManifest *pCtx, const char *parentExpress, const char *name, const int content);

/**
 * Create a new boolean element in the manifest file according to XPath express
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param parentExpress XPath express
 * @param name the new element name
 * @param content the new element value (0=false, 1=true)
 *
 * @returns RET_OK means new operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlNewBoolean(erManifest *pCtx, const char *parentExpress, const char *name, const int content);

/**
 * Remove an element and its children elements
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 * @param name the new element name
 * @param content the new element value
 *
 * @returns RET_OK means new operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlRemoveNode(erManifest *pCtx, const char *express);


/**
 * Save current memory XML Doc into file
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 *
 * @returns RET_OK means save operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlSave(erManifest *pCtx);

/**
 * Save current memory XML Doc into a specified file
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param szFileName the file to be stored
 *
 * @returns RET_OK means save operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlSaveAs(erManifest *pCtx, const char *szFileName);

/**
 * Save current memory XML Doc into file and close the manifest
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 *
 * @returns RET_OK means save operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlSaveAndClose(erManifest *pCtx);

/**
 * Close the manifest file WITHOUT saving
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 *
 * @returns RET_OK means close operation succeeds, otherwise returns RET_ERR 
 */
    int ermXmlClose(erManifest *pCtx);

/**
 * check the xpath indicated by 'express' exists or not.
 * if not exists, create it and its ancestors automatically,
 * also create attributes automatically as well.
 *
 * notes: only partly supports xpath
 * 
 * @param pCtx erManifest struct pointer initialized by ermXmlOpen
 * @param express XPath express
 *
 * @returns RET_OK means succeeds, otherwise returns RET_ERR 
 */
    int ermXmlCheckXpath(erManifest *pCtx, char* express);
    
    int ermXmlParseBuffer(const char* xmlString, const char* szFileName, erManifest *pCtx);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          //_ER_MANIFEST__H__
