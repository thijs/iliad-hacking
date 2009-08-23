/**
 * \file erregtoolbar.h
 * \brief liberreg - ereader toolbar registry(read only)
 *                   loading and unloading functions
 *                   getting functions
 *                   free memory functions
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _ERREG_TOOLBAR_H_
#define _ERREG_TOOLBAR_H_

#include "erregtypes.h"
#include "erregxml.h"
#include "erreg.h"

#define REG_TOOLBAR_XML_PATH    TEMPLATE_DIR "/toolbar.xml"
#define REG_TOOLBAR_XSD_PATH    TEMPLATE_DIR "/toolbar.xsd"   

#ifdef __cplusplus
extern "C"
{
#endif

    regLoad_t tbRegLoad(regRegistry_t** ppRegistry);
    void tbRegUnload(regRegistry_t* pRegistry);

    regIconSet_t* tbRegGetIconSet(regRegistry_t * pRegistry);
    void tbRegFreeIconSet(regIconSet_t * theIconSet);

#ifdef __cplusplus
}
#endif

#endif // _ERREG_TOOLBAR_H_
