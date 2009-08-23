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
 * \file erregcfg.h 
 * \brief liberreg -  import and export device configurations
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
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
