/**
 * \file erregcfg.h 
 * \brief liberreg -  import and export device configurations
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _ERREG_CFG_H_
#define _ERREG_CFG_H_

#include <glib.h>

#define CONFIG_DIR                 "/mnt/free/_autocfg"
#define EXPORT_CONFIG_XML_FILE     "iliadconfig.xml"
#define IMPORT_CONFIG_XML_FILE     "iliadconfig-ics.xml"
#define CONFIG_XSD_PATH            TEMPLATE_DIR "/iliadconfig.xsd"

#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/**
 * Detect whether import configuration file exists or not. 
 * Call this function before calling erRegImportConfig when user disconnects iliad from PC. 
 * 
 * @returns TRUE means file exists, otherwise returns FALSE 
 */
    // NOT TESTED: gboolean erRegCfgFileExist(void);

/**
 * Import user-defined configuration to registry
 * Call this function when user disconnects iliad from PC. 
 * 
 * @returns TRUE means importing operation succeeds, otherwise returns FALSE 
 */
    // NOT TESTED: gboolean erRegImportConfig(void);

/**
 * Export device configuration from registry 
 * 
 * @returns TRUE means exporting operation succeeds, otherwise returns FALSE 
 */
    // NOT TESTED: gboolean erRegExportConfig(void);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          //_ERREG_CFG_H_
