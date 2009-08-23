/**
 * \file   erregbasis.h 
 * \brief  liberreg - ereader basis registry
 *                    loading and unloading basis registry
 *                    getting and setting functions
 *                    free memory functions
 *                    recovering basis registry
 *                    build-in basis registry
 * 
 * Copyright (C) 2007 iRex Technologies BV.
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
