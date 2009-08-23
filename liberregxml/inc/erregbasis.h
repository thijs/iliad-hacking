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
 * \file   erregbasis.h 
 * \brief  liberreg - ereader basis registry
 *                    loading and unloading basis registry
 *                    getting and setting functions
 *                    free memory functions
 *                    recovering basis registry
 *                    build-in basis registry
 * 
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#ifndef _ERREG_BASIS_H_
#define _ERREG_BASIS_H_

#include "erregtypes.h"
#include "erregxml.h"
#include "erreg.h"

// basis registry files
#define REG_BASIS_XML_FILE          "registry.xml"
#define REG_BASIS_XSD_PATH          TEMPLATE_DIR "/registry.xsd"

#define DEFAULT_STARTUP_BEHAVIOUR   recentDocuments_t

#ifdef __cplusplus
extern "C"
{
#endif

    regLoad_t basisRegLoad (regRegistry_t **ppRegistry);
    void basisRegUnload (regRegistry_t *pRegistry);
    void basisRegRecover (regLoad_t source);
    gboolean basisRegStore(regRegistry_t* pRegistry);

    void basisRegInitXpathList(regRegistry_t *pRegistry);
    void basisRegDestroyXpathList(regRegistry_t *pRegistry);
   
    regPCProfile_t *basisRegGetPCProfileFromList(regPCConfig_t *pPCConfig, 
                                                 regPCProfile_t **pPCs,
                                                 const char *ID,
                                                 int *index);
    void basisRegAddPCProfileToList(regPCConfig_t **pPCConfig, 
                                    regPCProfile_t ***pPCs,
                                    regPCProfile_t *pNewPCProfile);
    void basisRegDeletePCProfileFromList(regPCConfig_t **pPCConfig, 
                                         regPCProfile_t ***pPCs,
                                         const char *ID);
    void basisRegSetPCProfileDefault(regPCConfig_t ** pPCConfig, 
                                     regPCProfile_t *** pPCs,
                                     const char *ID);

    // internal functions
    regUserProfile_t *basisRegNewUserProfile(void);
    regPCConfig_t *basisRegNewPCConfig(int size);

#ifdef __cplusplus
}
#endif

#endif // _ERREG_BASIS_H_
