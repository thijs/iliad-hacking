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
 *\file 
 *\brief liberreg -  parse and destroy registry
 *                         the functions of getting 
 *                         the functions of setting 
 *                         the functions of checking xpath exists or not
 *
 *Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <glib.h>

#include "erreglog.h"
#include "erreg.h"
#include "erregtypes_f.h"
#include "erregrwlock.h"
#include "erregxml.h"
#include "system.h"


///////////////////////////////////////////////////////////////////////////////
//
//  parse/destory/store/validate functions
//
///////////////////////////////////////////////////////////////////////////////

gboolean regValidate(regRegistry_t* pRegistry)
{
    ERREG_LOGPRINTF("entry");
    g_assert(pRegistry);
    g_assert(pRegistry->xmlCxt.pDoc);

    gboolean bRet;

    bRet = xsdValidateXmlDoc(&pRegistry->xsdCxt, pRegistry->xmlCxt.pDoc);
    if (bRet)
    {
        ERREG_LOGPRINTF("Validation succeeded, filename [%s]", pRegistry->xmlCxt.szFileName);
    }
    else
    {
        ERREG_ERRORPRINTF("Validation failed, filename [%s]", pRegistry->xmlCxt.szFileName);
    }

    return bRet;
}

static regRegistry_t *regParseSingleFile(const char *szXmlFile, const char *szXsdFile)
{
    gboolean bRet = FALSE;  // FALSE == error
    regRegistry_t *pRegistry = NULL;

    ERREG_LOGPRINTF("entry: xml [%s] xsd [%s]", szXmlFile, szXsdFile);
    g_assert(szXmlFile);
    g_assert(szXsdFile);

    // malloc memory
    pRegistry = g_new0(regRegistry_t, 1);
    g_assert(pRegistry != NULL);

    // construct context for xml
    int nRet = ermXmlOpenFile(szXmlFile, &(pRegistry->xmlCxt));
    if (RET_OK == nRet)
    {
        // construct context for xsd
        bRet = xsdLoad(&(pRegistry->xsdCxt), szXsdFile);
        if (bRet)
        {
            // XSD loaded ok: validate xml against xsd
            bRet = regValidate(pRegistry);
            if (FALSE == bRet)
            {
                ERREG_ERRORPRINTF("xsd validation error on [%s] [%s]", szXmlFile, szXsdFile);
            }
        }
        else
        {
            // XSD load error
            ERREG_ERRORPRINTF("xsdLoad error on [%s]", szXsdFile);
        }
    }
    else
    {
        ERREG_ERRORPRINTF("ermXmlOpenFile error on [%s]", szXmlFile);
    }

    // cleanup in case of error
    if (FALSE == bRet)
    {
        regDestroy(pRegistry);
        pRegistry = NULL;
    }

    ERREG_LOGPRINTF("leave: pRegistry [%p]", pRegistry);
    return pRegistry;
}

regRegistry_t *regParseFiles(const char *szXmlFile, const char *szXsdFile)
{
    ERREG_LOGPRINTF("entry: xml [%s] xsd [%s]", szXmlFile, szXsdFile);
    g_assert(szXmlFile);
    g_assert(szXsdFile);

    regRegistry_t *pRegistry = NULL;
    gchar         *cp;

    // load registry file
    pRegistry = regParseSingleFile(szXmlFile, szXsdFile);
    if (pRegistry == NULL)
    {
        // load error: try .new file
        cp = g_strdup_printf("%s.new", szXmlFile);
        ERREG_WARNPRINTF("Trying registry file [%s]", cp);
        pRegistry = regParseSingleFile(cp, szXsdFile);
        g_free(cp);
        if (pRegistry == NULL)
        {
            // load error: try .old file
            cp = g_strdup_printf("%s.old", szXmlFile);
            ERREG_WARNPRINTF("Trying registry file [%s]", cp);
            pRegistry = regParseSingleFile(szXmlFile, cp);
            g_free(cp);
        }

        // store the recovered registry values
        if (pRegistry)
        {
            pRegistry->changed = TRUE;
            if (erRegGetLockState() == lock_write)
            {
                regStore(pRegistry, szXmlFile);
            }
        }
    }

    ERREG_LOGPRINTF("leave: pRegistry [%p]", pRegistry);
    return pRegistry;
}

gboolean regStore(regRegistry_t *pRegistry, const char *szPath)
{

    ERREG_LOGPRINTF("entry: szPath [%s]", szPath);
    g_assert(pRegistry);
    g_assert(szPath);

    gboolean bRet = FALSE;  // FALSE = error
    int   rc;
    int   n;
    char  szDirPath[BUF_LEN];
    gchar *szPath_old = g_strdup_printf("%s.old", szPath);
    gchar *szPath_new = g_strdup_printf("%s.new", szPath);

    // save registry only when it has been changed and validates correctly
    if (pRegistry->changed)
    {
        if ( regValidate(pRegistry) )
        {
            // create registry directory when needed
            n = readlink(REG_DIR, szDirPath, sizeof(szDirPath));
            if (n > 0  &&  n < sizeof(szDirPath))
            {
                // REG_DIR is a symbolic link -> use the target it points to
                szDirPath[n] = '\0';
            }
            else
            {
                snprintf(szDirPath, sizeof(szDirPath), "%s", REG_DIR);
            }
            // note: ignore error code, probably reports "directory exists"
            mkdir(szDirPath, 0755);

            // move current file to .old, if any
            rename(szPath, szPath_old);
            
            // save to .new
            rc = ermXmlSaveAs(&pRegistry->xmlCxt, szPath_new);
            if (RET_OK == rc)
            {
                // saved ok: rename .new to current
                rc = rename(szPath_new, szPath);
                if (0 == rc)
                {
                    // registry saved alright
                    pRegistry->changed = FALSE;
                    bRet = TRUE;  // TRUE = ok
                }
            }
            else
            {
                // save error: discard .new
                ERREG_ERRORPRINTF("Registry section [%d] save error [%s]", pRegistry->section, szPath_new);
                unlink(szPath_new);
            }
        }
        else
        {
            ERREG_ERRORPRINTF("Registry section [%d] validation error", pRegistry->section);
        }
    }
    else
    {
        ERREG_WARNPRINTF("Registry section [%d] not saved because no changes", pRegistry->section);
        bRet = TRUE;  // TRUE = ok
    }

    g_free(szPath_new);
    g_free(szPath_old);
    ERREG_LOGPRINTF("leave: bRet [%d]", bRet);
    return bRet;
}

void regDestroy(regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry");
    g_assert(pRegistry);

    ermXmlClose(&(pRegistry->xmlCxt));
    xsdUnload(&(pRegistry->xsdCxt));
    g_free(pRegistry);
}

///////////////////////////////////////////////////////////////////////////////
//
//  get/set functions(basis sections)
//
///////////////////////////////////////////////////////////////////////////////
regVersion_t *regGetVersion(const regRegistry_t *pRegistry)
{
    regVersion_t *theVersion = NULL;

    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->version);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theVersion = g_new0(regVersion_t, 1);
    g_assert(theVersion != NULL);

    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->version, EL_REGISTRY);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get regVersion_t from memory.");
        regFreeVersion(theVersion);
        theVersion = NULL;
        return NULL;
    }
    else
    {
        theVersion->registry = g_strdup(szTmp);
    }

    regDumpVersion(theVersion);
    return theVersion;
}

gboolean regSetVersion(const regVersion_t *theVersion, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p %p", theVersion, pRegistry);
    g_assert(theVersion);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->version);

    regDumpVersion(theVersion);

    if (theVersion->registry)
    {
        char xpath[BUF_LEN + 1];
        int nLen, nRet;

        erManifest *pXmlCxt = &(pRegistry->xmlCxt);

        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->version, EL_REGISTRY);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, theVersion->registry);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

regUserProfile_t *regGetUserProfile(const regRegistry_t *pRegistry)
{
    regUserProfile_t *theUserProfile = NULL;

    ERREG_LOGPRINTF("entry");
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->userProfile);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theUserProfile = g_new0(regUserProfile_t, 1);
    g_assert(theUserProfile != NULL);

    // name
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userProfile, EL_NAME);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get name from memory.");
        regFreeUserProfile(theUserProfile);
        theUserProfile = NULL;
        return NULL;
    }
    theUserProfile->name = g_strdup(szTmp);

    // email
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userProfile, EL_EMAIL);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get email from memory.");
        regFreeUserProfile(theUserProfile);
        theUserProfile = NULL;
        return NULL;
    }
    theUserProfile->email = g_strdup(szTmp);

    // password
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userProfile, EL_PASSWORD);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get password from memory.");
        regFreeUserProfile(theUserProfile);
        theUserProfile = NULL;
        return NULL;
    }
    theUserProfile->password = g_strdup(szTmp);

    // redirectUrl
    gboolean bConfig = FALSE;
    char* cp = strstr(pRegistry->xpaths->userProfile, EL_CONFIG_ROOT);
    bConfig = (cp != NULL) ? TRUE : FALSE;
    ERREG_LOGPRINTF("bConfig=%d", bConfig);
    if (!bConfig)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userProfile, EL_REDIRECT_URL);
        nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
        if (RET_ERR == nRet)
        {
            ERREG_ERRORPRINTF("Can't get redirectUrl from memory.");
            regFreeUserProfile(theUserProfile);
            theUserProfile = NULL;
            return NULL;
        }
        theUserProfile->redirectUrl = g_strdup(szTmp);
    }

    regDumpUserProfile(theUserProfile);
    return theUserProfile;
}

gboolean regSetUserProfile(const regUserProfile_t *theUserProfile, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p %p", theUserProfile, pRegistry);
    g_assert(theUserProfile);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->userProfile);

    regDumpUserProfile(theUserProfile);

    char xpath[BUF_LEN + 1];
    int nLen, nRet = RET_OK;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // name
    if (theUserProfile->name)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userProfile, EL_NAME);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, theUserProfile->name);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // email 
    if (theUserProfile->email)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userProfile, EL_EMAIL);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, theUserProfile->email);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // password
    if (theUserProfile->password)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userProfile, EL_PASSWORD);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, theUserProfile->password);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // redirectUrl
    if (theUserProfile->redirectUrl)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userProfile, EL_REDIRECT_URL);
        nRet == ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, theUserProfile->redirectUrl);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

regUserSetting_t *regGetUserSetting(const regRegistry_t *pRegistry)
{
    regUserSetting_t *theUserSetting = NULL;

    ERREG_LOGPRINTF("entry");
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->userSetting);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;
    gboolean bEnable;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theUserSetting = g_new0(regUserSetting_t, 1);
    g_assert(theUserSetting != NULL);

    // language
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userSetting, EL_LANGUAGE);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get language from memory.");
        regFreeUserSetting(theUserSetting);
        theUserSetting = NULL;
        return NULL;
    }
    theUserSetting->language = g_strdup(szTmp);

    // sound
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->userSetting, EL_SOUND, EL_ENABLE);
    nRet = ermXmlGetBoolean(pXmlCxt, xpath, &bEnable);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get sound enable flag from memory.");
        regFreeUserSetting(theUserSetting);
        theUserSetting = NULL;
        return NULL;
    }

    if (!bEnable)
    {
        theUserSetting->volume = 0;
    }
    else
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->userSetting, EL_SOUND, EL_VOLUME);
        nRet = ermXmlGetInt(pXmlCxt, xpath, &theUserSetting->volume);
        if (RET_ERR == nRet)
        {
            ERREG_ERRORPRINTF("Can't get sound volume from memory.");
            regFreeUserSetting(theUserSetting);
            theUserSetting = NULL;
            return NULL;
        }
    }

    // keyBuffering
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->userSetting, EL_KEY_BUFFERING, EL_ENABLE);
    nRet = ermXmlGetBoolean(pXmlCxt, xpath, &bEnable);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get key buffering enable flag from memory.");
        regFreeUserSetting(theUserSetting);
        theUserSetting = NULL;
        return NULL;
    }

    if (!bEnable)
    {
        theUserSetting->keyBuffering = 0;
    }
    else
    {
        nLen =
            g_snprintf(xpath, BUF_LEN, "%s/%s/%s",
                       pRegistry->xpaths->userSetting, EL_KEY_BUFFERING, EL_NUMBER_OF_KEYS);
        nRet = ermXmlGetInt(pXmlCxt, xpath, &theUserSetting->keyBuffering);
        if (RET_ERR == nRet)
        {
            ERREG_ERRORPRINTF("Can't get the number of key buffering from memory.");
            regFreeUserSetting(theUserSetting);
            theUserSetting = NULL;
            return NULL;
        }
    }

    // longkeypressTimeout 
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userSetting, EL_LONG_KP_TIMEOUT);
    nRet = ermXmlGetInt(pXmlCxt, xpath, &theUserSetting->longkeypressTimeout);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get long key press timeout from memory.");
        regFreeUserSetting(theUserSetting);
        theUserSetting = NULL;
        return NULL;
    }

    // flipbarLeftIsDown
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userSetting, EL_FLIPBAR_DIRECTION);
    nRet = ermXmlGetBoolean(pXmlCxt, xpath, &theUserSetting->flipbarLeftIsDown);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get flipbar direction from memory.");
        regFreeUserSetting(theUserSetting);
        theUserSetting = NULL;
        return NULL;
    }

    regDumpUserSetting(theUserSetting);
    return theUserSetting;
}

gboolean regSetUserSetting(const regUserSetting_t *theUserSetting, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p %p", theUserSetting, pRegistry);
    g_assert(theUserSetting);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->userSetting);

    regDumpUserSetting(theUserSetting);

    char xpath[BUF_LEN + 1];
    int nLen, nRet;
    gboolean bEnable;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // language
    if (theUserSetting->language)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userSetting, EL_LANGUAGE);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, theUserSetting->language);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // sound 
    if (theUserSetting->volume)
    {
        bEnable = TRUE;
    }
    else
    {
        bEnable = FALSE;
    }
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->userSetting, EL_SOUND, EL_ENABLE);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetBoolean(pXmlCxt, xpath, bEnable);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->userSetting, EL_SOUND, EL_VOLUME);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetInt(pXmlCxt, xpath, theUserSetting->volume);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // key buffering
    if (theUserSetting->keyBuffering)
    {
        bEnable = TRUE;
    }
    else
    {
        bEnable = FALSE;
    }
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->userSetting, EL_KEY_BUFFERING, EL_ENABLE);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetBoolean(pXmlCxt, xpath, bEnable);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->userSetting, 
                    EL_KEY_BUFFERING, EL_NUMBER_OF_KEYS);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetInt(pXmlCxt, xpath, theUserSetting->keyBuffering);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // longkeypresstimeout 
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userSetting, EL_LONG_KP_TIMEOUT);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetInt(pXmlCxt, xpath, theUserSetting->longkeypressTimeout);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // flipbarLeftIsDown
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->userSetting, EL_FLIPBAR_DIRECTION);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetBoolean(pXmlCxt, xpath, theUserSetting->flipbarLeftIsDown);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

regPCConfig_t *regGetPCConfig(const regRegistry_t *pRegistry)
{
    regPCConfig_t *thePCConfig = NULL;

    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->pcProfile);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, i, nProfiles, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // get the number of nodes('application') 
    nLen = g_snprintf(xpath, BUF_LEN, "%s", pRegistry->xpaths->pcProfile);
    nRet = ermXmlGetNodeNr(pXmlCxt, xpath, &nProfiles);
    if (RET_ERR == nRet || 0 == nProfiles)
    {
        ERREG_ERRORPRINTF("%s not present", xpath);
        return NULL;
    }

    // memory malloc
    thePCConfig = g_new0(regPCConfig_t, 1);
    g_assert(thePCConfig != NULL);

    thePCConfig->size = nProfiles;
    thePCConfig->pcList = g_new0(char *, nProfiles + 1);
    thePCConfig->pcList[nProfiles] = NULL;

    // get the value of attribute(pcProfile[i]/@type)
    for (i = 1; i <= nProfiles; i++)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s[%d]", pRegistry->xpaths->pcProfile, i);
        nRet = ermXmlGetAttributeString(pXmlCxt, xpath, ATTR_PROFILE_ID, szTmp, BUF_LEN);
        if (RET_OK == nRet)
        {
            thePCConfig->pcList[i - 1] = g_strdup(szTmp);
        }
    }

    regDumpPCConfig(thePCConfig);
    return thePCConfig;
}

gboolean regSetPCConfig(const regPCConfig_t *thePCConfig, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p %p", thePCConfig, pRegistry);
    g_assert(thePCConfig);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->pcProfile);

    regDumpPCConfig(thePCConfig);

    char xpath[BUF_LEN + 1];
    int nLen, i, nProfiles, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    nProfiles = thePCConfig->size;
    for (i = 1; i <= nProfiles; i++)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s[%d]", pRegistry->xpaths->pcProfile, i);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetAttributeString(pXmlCxt, xpath, ATTR_PROFILE_ID,
                         thePCConfig->pcList[i - 1], strlen(thePCConfig->pcList[i - 1]));
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

regPCProfile_t *regGetPCProfile(const char *ID, const regRegistry_t *pRegistry)
{
    regPCProfile_t *thePCProfile = NULL;

    ERREG_LOGPRINTF("entry %s %p", ID, pRegistry);
    g_assert(ID);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->pcProfile);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1], xpathExt[BUF_LEN + 1];
    int nLen, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    thePCProfile = g_new0(regPCProfile_t, 1);
    g_assert(thePCProfile != NULL);

    // xpath
    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->pcProfile, ATTR_PROFILE_ID, ID);

    // name
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_PROFILE_NAME);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        g_snprintf(szTmp, BUF_LEN, "%s", ID);
    }
    thePCProfile->name = g_strdup(szTmp);

    // pcname
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_PC_NAME);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get pcname from memory.");
        regFreePCProfile(thePCProfile);
        thePCProfile = NULL;
        return NULL;
    }
    thePCProfile->pcname = g_strdup(szTmp);

    // sharename
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_SHARE_NAME);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get sharename from memory.");
        regFreePCProfile(thePCProfile);
        thePCProfile = NULL;
        return NULL;
    }
    thePCProfile->sharename = g_strdup(szTmp);

    // workgroup
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_WORKGROUP);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get workgroup from memory.");
        regFreePCProfile(thePCProfile);
        thePCProfile = NULL;
        return NULL;
    }
    thePCProfile->workgroup = g_strdup(szTmp);

    // username
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_USER_NAME);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get username from memory.");
        regFreePCProfile(thePCProfile);
        thePCProfile = NULL;
        return NULL;
    }
    thePCProfile->username = g_strdup(szTmp);

    // password
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_PASSWORD);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get password from memory.");
        regFreePCProfile(thePCProfile);
        thePCProfile = NULL;
        return NULL;
    }
    thePCProfile->password = g_strdup(szTmp);

    regDumpPCProfile(ID, thePCProfile);
    return thePCProfile;
}

gboolean regSetPCProfile(const char *ID, const regPCProfile_t *thePCProfile, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %s %p %p", ID, thePCProfile, pRegistry);
    g_assert(ID);
    g_assert(thePCProfile);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->pcProfile);

    regDumpPCProfile(ID, thePCProfile);

    char xpath[BUF_LEN + 1], xpathExt[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // check if relevant values are present before adding them to the registry file
    if ((thePCProfile->pcname == NULL) || (thePCProfile->sharename == NULL))
    {
        ERREG_WARNPRINTF("Some fields in thePCProfile shouldn't containis NULL.");
        return FALSE;
    }

    // xpath
    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->pcProfile, ATTR_PROFILE_ID, ID);

    // profile name
    if (thePCProfile->name)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_PROFILE_NAME);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, thePCProfile->name);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }
    
    // pcname
    if (thePCProfile->pcname)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_PC_NAME);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, thePCProfile->pcname);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // sharename
    if (thePCProfile->sharename)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_SHARE_NAME);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, thePCProfile->sharename);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // workgroup
    if (thePCProfile->workgroup)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_WORKGROUP);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, thePCProfile->workgroup);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // username 
    if (thePCProfile->username)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_USER_NAME);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, thePCProfile->username);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // password
    if (thePCProfile->password)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_PASSWORD);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, thePCProfile->password);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

gboolean regRemovePCProfile(const char *ID, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %s %p", ID, pRegistry);
    g_assert(ID);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->pcProfile);

    char xpath[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // xpath
    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->pcProfile, ATTR_PROFILE_ID, ID);
    nRet = ermXmlRemoveNode(pXmlCxt, xpath);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    return TRUE;
}

gboolean regRemovePCConfig(regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->pcProfiles);

    char xpath[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // xpath
    nLen = g_snprintf(xpath, BUF_LEN, "%s", pRegistry->xpaths->pcProfiles);
    nRet = ermXmlRemoveNode(pXmlCxt, xpath);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    return TRUE;
}

void regGetPCProfiles(regPCConfig_t **pPCConfig, regPCProfile_t ***pPCs, const regRegistry_t* pRegistry)
{
    int i;
    regPCConfig_t *thePCConfig = NULL;
    regPCProfile_t **thePCs = NULL;

    ERREG_LOGPRINTF("entry");
    g_assert(pPCConfig);
    g_assert(pPCs);
    g_assert(pRegistry);

    //destroy old profiles, just in case
    regFreePCProfiles(*pPCConfig, *pPCs);
    *pPCConfig = NULL;
    *pPCs = NULL;

    // retieve the pc config data
    thePCConfig = regGetPCConfig(pRegistry);
    if (thePCConfig == NULL)
    {
        ERREG_WARNPRINTF("thePCConfig not present");
    }
    else
    {
        ERREG_LOGPRINTF("thePCConfig exist");

        thePCs = g_new0(regPCProfile_t *, thePCConfig->size);
        for (i = 0; i < thePCConfig->size; i++)
        {
            //retrieve the pc data 
            thePCs[i] = regGetPCProfile(thePCConfig->pcList[i], pRegistry);
            ERREG_LOGPRINTF("thePCs[%d] = [%p]", i, thePCs[i]);
        }
    }

    *pPCConfig = thePCConfig;
    *pPCs = thePCs;
}

gboolean regSetPCProfiles(const regPCConfig_t *thePCConfig, const regPCProfile_t **thePCs, regRegistry_t* pRegistry)
{
    ERREG_LOGPRINTF("entry");
    g_assert(thePCConfig);
    g_assert(thePCs);
    g_assert(pRegistry);

    gboolean bRet = TRUE;  // TRUE = ok
    gboolean b;
    int      i;

    // remove the ones
    regRemovePCConfig(pRegistry);
    
    // set the new ones
    regSetPCConfig(thePCConfig, pRegistry);
    for (i = 0; i < thePCConfig->size; i++)
    {
        b = regSetPCProfile(thePCConfig->pcList[i], thePCs[i], pRegistry);
        if (FALSE == b)
        {
            bRet = FALSE;
        }
    }

    if (bRet)
    {
        pRegistry->changed = TRUE;
    }
    return bRet;
}

regAutoConnect_t *regGetAutoConnect(const regRegistry_t *pRegistry)
{
    regAutoConnect_t *theAutoConnect = NULL;

    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->autoConnect);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theAutoConnect = g_new0(regAutoConnect_t, 1);
    g_assert(theAutoConnect != NULL);

    // connect to which server
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->autoConnect, EL_SERVER_TYPE);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get which server to connect from memory.");
        regFreeAutoConnect(theAutoConnect);
        theAutoConnect = NULL;
        return NULL;
    }
    theAutoConnect->backgroundConnectTo = regServerTypeToEnum(szTmp);
    
    // background enable?
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_BACKGOUND, EL_ENABLE);
    nRet = ermXmlGetBoolean(pXmlCxt, xpath, &theAutoConnect->backgroundEnable);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get background enable flag from memory.");
        regFreeAutoConnect(theAutoConnect);
        theAutoConnect = NULL;
        return NULL;
    }

    // background interval  
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_BACKGOUND, EL_INTERVAL);
    nRet = ermXmlGetInt(pXmlCxt, xpath, &theAutoConnect->backgroundInterval);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get background interval from memory.");
        regFreeAutoConnect(theAutoConnect);
        theAutoConnect = NULL;
        return NULL;
    }

    regDumpAutoConnect(theAutoConnect);
    return theAutoConnect;
}

gboolean regSetAutoConnect(const regAutoConnect_t *theAutoConnect, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p %p", theAutoConnect, pRegistry);
    g_assert(theAutoConnect);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->autoConnect);

    regDumpAutoConnect(theAutoConnect);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // connect to which server
    regServerTypeToString(theAutoConnect->backgroundConnectTo, szTmp, BUF_LEN);
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->autoConnect, EL_SERVER_TYPE);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, szTmp);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // backgroundEnable
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_BACKGOUND, EL_ENABLE);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetBoolean(pXmlCxt, xpath, theAutoConnect->backgroundEnable);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // backgroundInterval
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_BACKGOUND, EL_INTERVAL);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetInt(pXmlCxt, xpath, theAutoConnect->backgroundInterval);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

static int lt(const void *l, const void *r)
{
    return *(int *)l - *(int *)r;
}

// Remove duplicate elements in an integer sorted array, and return the number of unique elements
static int rmDup(int arr[], int* pSize)
{
    int i, tmpArr[*pSize];
    int nLastStored = -1;
    int pos = 0;

    for (i=0; i<*pSize; i++)
    {
        if (arr[i] != nLastStored)
        {
            tmpArr[pos++] = arr[i];
            nLastStored = arr[i];
        }
    }

    memcpy(arr, tmpArr, sizeof(int)*pos);
    return (*pSize = pos);
}

regTimedIds_t *regGetTimedIds(const regRegistry_t *pRegistry)
{
    regTimedIds_t *theTimedIds = NULL;

    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->autoConnect);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int  nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theTimedIds = g_new0(regTimedIds_t, 1);
    g_assert(theTimedIds != NULL);

    // Enable timed-iDS connection or not
    g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_TIMED_IDS, EL_ENABLE_TIMEDIDS);
    nRet = ermXmlGetBoolean(pXmlCxt, xpath, &theTimedIds->enable);
    if (nRet == RET_ERR)
    {
        regFreeTimedIds(theTimedIds);
        return NULL;
    }

    // How many valid time count in time set
    g_snprintf(xpath, BUF_LEN, "%s/%s/%s/%s", pRegistry->xpaths->autoConnect, EL_TIMED_IDS, EL_TIMESET, EL_TIME);
    nRet = ermXmlGetNodeNr(pXmlCxt, xpath, &theTimedIds->timeCnt);
    if (nRet == RET_ERR)
    {
        regFreeTimedIds(theTimedIds);
        return NULL;
    }

    // Time set
    int i, hh, mm, ss;
    for (i=1; i<=theTimedIds->timeCnt; i++)
    {
        g_snprintf(xpath, BUF_LEN, "%s/%s/%s/%s[%d]", pRegistry->xpaths->autoConnect, EL_TIMED_IDS, EL_TIMESET, EL_TIME, i);
        nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);

        if (RET_ERR == nRet)
        {
            regFreeTimedIds(theTimedIds);
            return NULL;
        }

        sscanf(szTmp, "%02d:%02d:%02d", &hh, &mm, &ss);
        theTimedIds->timeSet[i-1] = hh*3600 + mm*60 + ss;
    }

    // Enable software updates while connected to iDS
    g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_TIMED_IDS, EL_ENABLE_SWUPDATE);
    nRet = ermXmlGetBoolean(pXmlCxt, xpath, &theTimedIds->swUpdate);
    if (nRet == RET_ERR)
    {
        regFreeTimedIds(theTimedIds);
        return NULL;
    }

    // Switch off iLiad after waking iLiad up
    g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_TIMED_IDS, EL_ENABLE_SWITCHOFF);
    nRet = ermXmlGetBoolean(pXmlCxt, xpath, &theTimedIds->switchOff);
    if (nRet == RET_ERR)
    {
        regFreeTimedIds(theTimedIds);
        return NULL;
    }

    regDumpTimedIds(theTimedIds);
    return theTimedIds;
}

gboolean regRemoveTimeSet(regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->autoConnect);

    char xpath[BUF_LEN + 1];

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // xpath
    g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_TIMED_IDS, EL_TIMESET);
    if (ermXmlRemoveNode(pXmlCxt, xpath) == RET_ERR)
    {
        return FALSE;
    }

    return TRUE;
}

gboolean regSetTimedIds(regTimedIds_t *theTimedIds, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p %p", theTimedIds, pRegistry);
    g_assert(theTimedIds);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->autoConnect);

    regDumpTimedIds(theTimedIds);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    
    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // Enable timed-iDS connection or not
    g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_TIMED_IDS, EL_ENABLE_TIMEDIDS);
    if (RET_ERR == ermXmlCheckXpath(pXmlCxt, xpath))
    {
        return FALSE;
    }
    if (RET_ERR == ermXmlSetBoolean(pXmlCxt, xpath, theTimedIds->enable))
    {
        return FALSE;
    }

    // Time set
    // Sort
    qsort(theTimedIds->timeSet, theTimedIds->timeCnt, sizeof(int), lt);
    // Remove duplicate items
    rmDup(theTimedIds->timeSet, &theTimedIds->timeCnt);

    // Remove existing time set first
    regRemoveTimeSet(pRegistry);

    int i, hh, mm, ss;
    for (i=0; i<theTimedIds->timeCnt; i++)
    {
        g_snprintf(xpath, BUF_LEN, "%s/%s/%s/%s[%d]", pRegistry->xpaths->autoConnect, EL_TIMED_IDS, EL_TIMESET, EL_TIME, i+1);

        if (RET_ERR == ermXmlCheckXpath(pXmlCxt, xpath))
        {
            return FALSE;
        }

        hh = theTimedIds->timeSet[i] / 3600;
        mm = theTimedIds->timeSet[i] / 60 % 60;
        ss = theTimedIds->timeSet[i] % 60;
        snprintf(szTmp, BUF_LEN, "%02d:%02d:%02d", hh, mm, ss);

        if (RET_ERR == ermXmlSetString(pXmlCxt, xpath, szTmp))
        {
            return FALSE;
        }
    }

    // Enable software updates while connected to iDS
    g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_TIMED_IDS, EL_ENABLE_SWUPDATE);
    if (RET_ERR == ermXmlCheckXpath(pXmlCxt, xpath))
    {
        return FALSE;
    }
    if (RET_ERR == ermXmlSetBoolean(pXmlCxt, xpath, theTimedIds->swUpdate))
    {
        return FALSE;
    }
    
    // Switch off iLiad after waking iLiad up
    g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->autoConnect, EL_TIMED_IDS, EL_ENABLE_SWITCHOFF);
    if (RET_ERR == ermXmlCheckXpath(pXmlCxt, xpath))
    {
        return FALSE;
    }
    if (RET_ERR == ermXmlSetBoolean(pXmlCxt, xpath, theTimedIds->switchOff))
    {
        return FALSE;
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

regContentCategory_t *regGetContentCategory(const char *ID, const regRegistry_t *pRegistry)
{
    regContentCategory_t *theContentCategory = NULL;

    ERREG_LOGPRINTF("entry: ID [%s] pRegistry [%p]", ID, pRegistry);
    g_assert(ID);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->category);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1], xpathExt[BUF_LEN + 1];
    int nLen, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theContentCategory = g_new0(regContentCategory_t, 1);
    g_assert(theContentCategory != NULL);

    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->category, ATTR_TYPE, ID);

    // location
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_LOCATION);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get category from memory, xpath [%s]", xpath);
        regFreeContentCategory(theContentCategory);
        theContentCategory = NULL;
        return NULL;
    }
    theContentCategory->location = g_strdup(szTmp);

    // sort
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s/%s", xpath, EL_SORT, EL_SORT_ASCENDING);
    nRet = ermXmlGetBoolean(pXmlCxt, xpathExt, &theContentCategory->sortAscending);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get sort from memory, xpath [%s]", xpath);
        regFreeContentCategory(theContentCategory);
        theContentCategory = NULL;
        return NULL;
    }

    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s/%s", xpath, EL_SORT, EL_SORT_FIELD_TYPE);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get sort field from memory, xpath [%s]", xpath);
        regFreeContentCategory(theContentCategory);
        theContentCategory = NULL;
        return NULL;
    }
    theContentCategory->sortFieldType = g_strdup(szTmp);

    regDumpContentCategory(ID, theContentCategory);
    ERREG_LOGPRINTF("leave: theContentCategory [%p]", theContentCategory);
    return theContentCategory;
}

gboolean regSetContentCategory(const char *ID, const regContentCategory_t *theContentCategory, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("%s %p %p", ID, theContentCategory, pRegistry);
    g_assert(ID);
    g_assert(theContentCategory);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->category);

    regDumpContentCategory(ID, theContentCategory);

    char xpath[BUF_LEN + 1], xpathExt[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->category, ATTR_TYPE, ID);

    if (theContentCategory->location)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_LOCATION);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, theContentCategory->location);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    if (theContentCategory->sortFieldType)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s/%s", xpath, EL_SORT, EL_SORT_FIELD_TYPE);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, theContentCategory->sortFieldType);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s/%s", xpath, EL_SORT, EL_SORT_ASCENDING);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetBoolean(pXmlCxt, xpathExt, theContentCategory->sortAscending);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

regExportMemType_t *regGetExportMemType(const regRegistry_t *pRegistry)
{
    regExportMemType_t *theExportMemType = NULL;

    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->usbExport);

    char szTmp[BUF_LEN + 1];
    int  nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theExportMemType = g_new0(regExportMemType_t, 1);
    g_assert(theExportMemType != NULL);

    // Get the memory type which will be exported to user via USB connection
    nRet = ermXmlGetString(pXmlCxt, pRegistry->xpaths->usbExport, szTmp, BUF_LEN);
    if (nRet == RET_ERR)
    {
        regFreeExportMemType(theExportMemType);
        return NULL;
    }

    theExportMemType->location = g_strdup(szTmp);
    return theExportMemType;
}

gboolean regSetExportMemType(const regExportMemType_t *theExportMemType, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("%p %p", theExportMemType, pRegistry);
    g_assert(theExportMemType);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->usbExport);
    g_assert(theExportMemType);
    g_assert(theExportMemType->location);

    regDumpExportMemType(theExportMemType);

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    if (RET_ERR == ermXmlCheckXpath(pXmlCxt, pRegistry->xpaths->usbExport))
    {
        return FALSE;
    }
    if (RET_ERR == ermXmlSetString(pXmlCxt, pRegistry->xpaths->usbExport, theExportMemType->location))
    {
        return FALSE;
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

regLastRead_t *regGetLastRead(const char *ID, regRegistry_t *pRegistry)
{
    regLastRead_t *theLastRead = NULL;

    ERREG_LOGPRINTF("entry %s %p", ID, pRegistry);
    g_assert(ID);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->lastRead);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1], xpathExt[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theLastRead = g_new0(regLastRead_t, 1);
    g_assert(theLastRead != NULL);

    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->lastRead, ATTR_TYPE, ID);

    // extension
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_EXT_NAME);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get extension from memory.");
        regFreeLastRead(theLastRead);
        theLastRead = NULL;
        return NULL;
    }
    theLastRead->extension = g_strdup(szTmp);

    // documentPath
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_DOCUMENT_PATH);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get documentPath from memory.");
        regFreeLastRead(theLastRead);
        theLastRead = NULL;
        return NULL;
    }
    theLastRead->documentPath = g_strdup(szTmp);

    // manifestPath
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_MANIFEST_PATH);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get manifestPath from memory.");
        regFreeLastRead(theLastRead);
        theLastRead = NULL;
        return NULL;
    }
    theLastRead->manifestPath = g_strdup(szTmp);

    // position
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_POSITION);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get position from memory.");
        regFreeLastRead(theLastRead);
        theLastRead = NULL;
        return NULL;
    }
    theLastRead->position = g_strdup(szTmp);

    regDumpLastRead(ID, theLastRead);
    return theLastRead;
}

gboolean regSetLastRead(const char *ID, const regLastRead_t *theLastRead, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %s %p %p", ID, theLastRead, pRegistry);
    g_assert(ID);
    g_assert(theLastRead);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->lastRead);

    regDumpLastRead(ID, theLastRead);

    char xpath[BUF_LEN + 1], xpathExt[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->lastRead, ATTR_TYPE, ID);

    if (theLastRead->extension)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_EXT_NAME);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, theLastRead->extension);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    if (theLastRead->documentPath)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_DOCUMENT_PATH);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, theLastRead->documentPath);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    if (theLastRead->manifestPath)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_MANIFEST_PATH);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, theLastRead->manifestPath);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    if (theLastRead->position)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_POSITION);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, theLastRead->position);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

regUserAppList_t *regGetUserAppList(const regRegistry_t *pRegistry)
{
    regUserAppList_t *theUserAppList = NULL;

    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->application);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, i, nApp, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // get the number of nodes('application') 
    nLen = g_snprintf(xpath, BUF_LEN, "%s", pRegistry->xpaths->application);
    nRet = ermXmlGetNodeNr(pXmlCxt, xpath, &nApp);
    if (RET_ERR == nRet || 0 == nApp)
    {
        ERREG_ERRORPRINTF("%s not present", xpath);
        return NULL;
    }

    // memory malloc
    theUserAppList = g_new0(regUserAppList_t, 1);
    theUserAppList->uaIDArraySize = nApp;
    theUserAppList->uaIDArray = g_new0(char *, nApp + 1);
    theUserAppList->uaIDArray[nApp] = NULL;

    // get the value of attribute(application[i]/@type)
    for (i = 1; i <= nApp; i++)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s[%d]", pRegistry->xpaths->application, i);
        nRet = ermXmlGetAttributeString(pXmlCxt, xpath, ATTR_TYPE, szTmp, BUF_LEN);
        if (RET_OK == nRet)
        {
            theUserAppList->uaIDArray[i - 1] = g_strdup(szTmp);
        }
    }

    regDumpUserAppList(theUserAppList);
    return theUserAppList;
}

regUserApp_t *regGetUserApp(const char *uaID, const regRegistry_t *pRegistry)
{
    regUserApp_t *theUserApp = NULL;

    ERREG_LOGPRINTF("entry %s %p", uaID, pRegistry);
    g_assert(uaID);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->application);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1], xpathExt[BUF_LEN + 1];
    int nLen, nRet, nFormat;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theUserApp = g_new0(regUserApp_t, 1);
    g_assert(theUserApp != NULL);

    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->application, ATTR_TYPE, uaID);

    // appCategory
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_APP_CATEGORY);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("%s not present", xpathExt);
        regFreeUserApp(theUserApp);
        return NULL;
    }
    theUserApp->szCategory = g_strdup(szTmp);

    // szArgMask
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_ARG_MASK);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("%s not present", xpathExt);
        regFreeUserApp(theUserApp);
        return NULL;
    }
    theUserApp->szArgMask = g_strdup(szTmp);

    // szExec
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_EXECUTE);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("%s not present", xpathExt);
        regFreeUserApp(theUserApp);
        return NULL;
    }
    theUserApp->szExec = g_strdup(szTmp);

    // xResName
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_X_RES_NAME);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("%s not present", xpathExt);
        regFreeUserApp(theUserApp);
        return NULL;
    }
    theUserApp->xResName = g_strdup(szTmp);

    // ipcChannel
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_IPC_CHANNEL);
    nRet = ermXmlGetInt(pXmlCxt, xpathExt, &(theUserApp->ipcChannel));
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("%s not present", xpathExt);
        regFreeUserApp(theUserApp);
        return NULL;
    }

    // appIconPath
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_APP_ICON_PATH);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_WARNPRINTF("%s not present", xpathExt);
        theUserApp->appIconPath = g_strdup(szTmp);
    }

    // supportFormat
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s/%s", xpath, EL_SUPPORT_FORMAT, EL_EXT_NAME);
    nRet = ermXmlGetNodeNr(pXmlCxt, xpathExt, &nFormat);
    if ((RET_OK == nRet) && nFormat)
    {
        int i;

        theUserApp->extensionArray = g_new0(char *, nFormat + 1);
        theUserApp->extensionArray[nFormat] = NULL;
        theUserApp->extensionArraySize = nFormat;

        for (i = 1; i <= nFormat; i++)
        {
            nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s/%s[%d]", xpath, EL_SUPPORT_FORMAT, EL_EXT_NAME, i);
            nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
            if (RET_OK == nRet)
            {
                theUserApp->extensionArray[i - 1] = g_strdup(szTmp);
            }
        }                   // for (i...)
    }                       // if ((RET_OK == nRet) && nFormat)

    regDumpUserApp(uaID, theUserApp);
    return theUserApp;
}

gboolean regSetUserApp(const char *uaID, const regUserApp_t *theUserApp, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %s %p %p", uaID, theUserApp, pRegistry);
    g_assert(uaID);
    g_assert(theUserApp);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->application);

    regDumpUserApp(uaID, theUserApp);

    char xpath[BUF_LEN + 1], xpathExt[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->application, ATTR_TYPE, uaID);

    // appCategory
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_APP_CATEGORY);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpathExt, theUserApp->szCategory);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // szArgMask
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_ARG_MASK);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpathExt, theUserApp->szArgMask);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // szExec
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_EXECUTE);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpathExt, theUserApp->szExec);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // xResName
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_X_RES_NAME);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpathExt, theUserApp->xResName);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // ipcChannel
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_IPC_CHANNEL);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetInt(pXmlCxt, xpathExt, theUserApp->ipcChannel);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_APP_ICON_PATH);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpathExt, theUserApp->appIconPath);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // supportFormat
    if (theUserApp->extensionArray && theUserApp->extensionArraySize)
    {
        int i;

        for (i = 1; i <= theUserApp->extensionArraySize; i++)
        {
            nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s/%s[%d]", xpath, EL_SUPPORT_FORMAT, EL_EXT_NAME, i);
            if (theUserApp->extensionArray[i - 1])
            {
                nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
                if (RET_OK == nRet)
                {
                    nRet = ermXmlSetString(pXmlCxt, xpathExt, theUserApp->extensionArray[i - 1]);
                }
                if (RET_ERR == nRet)
                {
                    return FALSE;
                }
            }               //if (theUserApp->extensionArray[i - 1])
        }                   // for (i...)
    }                       // if (theUserApp->extensionArray ...)

    pRegistry->changed = TRUE;
    return TRUE;
}

/// @brief This function will return an array of regExtInfo_t nodes.
/// @param extension The extension name. If NULL it will return all extention info, otherwise only 
/// return the nodes with the same extension name.
/// @param pRegistry The registry handle.
/// @return It returns NULL if something wrong. Otherwise it will return the
/// an array of regExtInfo_t.
regExtInfoList_t *regGetExtInfoList(const char *extension, const regRegistry_t * pRegistry)
{
    regExtInfoList_t   *theExtInfoList = NULL;
    xmlXPathObjectPtr   xpathObj       = NULL;
    xmlNodePtr*         p_node         = NULL;
    xmlNodeSetPtr       extensionNodes = NULL;  // list of extensionInfo nodes
    

    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->extensionInfos);

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // get extensionInfos node by the extension name.
    char xpath[BUF_LEN + 1] = {0};
    if (extension)
    {
        g_snprintf(xpath, BUF_LEN, "%s/%s[%s=\"%s\"]",
                        pRegistry->xpaths->extensionInfos, 
                        EL_EXTENSION_INFO,
                        EL_EXT_NAME, 
                        extension);
    }
    else
    {
        g_snprintf(xpath, BUF_LEN, "%s/%s",
                        pRegistry->xpaths->extensionInfos, 
                        EL_EXTENSION_INFO);
    }
    xpathObj = ermXmlSelectNodes(pXmlCxt, xpath);                            
    if (xpathObj == NULL)
    {
        ERREG_WARNPRINTF("%s not found", xpath);
        return NULL;
    }
    
    // get extensionInfo nodes
    extensionNodes = xpathObj->nodesetval;

    // number and nodes
    int num = extensionNodes->nodeNr;
    p_node = extensionNodes->nodeTab;

    // allocate the list
    theExtInfoList = g_new0(regExtInfoList_t, 1);
    g_assert(theExtInfoList != NULL);
    theExtInfoList->extArray = g_new0(regExtInfo_t, num);
    theExtInfoList->extArraySize = num;

    // Walk all nodes and get the extensionInfo from it
    int i = 0;
    for (; i < num ; ++i, ++p_node)
    {
        xmlChar * content = 0;
        
        // it's better to use libxml API instead of xpath here.
        // extIcon
        xmlNodePtr iconPtr = ermXmlGetChildNode(*p_node, EL_EXT_ICON);
        content = xmlNodeGetContent(iconPtr);
        theExtInfoList->extArray[i].iconLocation = g_strdup(content);
        xmlFree(content);

        // extension name
        xmlNodePtr extPtr = ermXmlGetChildNode(*p_node, EL_EXT_NAME);
        content = xmlNodeGetContent(extPtr);
        theExtInfoList->extArray[i].extName = g_strdup(content);
        xmlFree(content);

        // associate application
        xmlNodePtr appPtr = ermXmlGetChildNode(*p_node, EL_ASSOCIATE_APP);
        content = xmlNodeGetContent(appPtr);
        theExtInfoList->extArray[i].associateApp = g_strdup(content);
        xmlFree(content);

        // associate order
        xmlNodePtr appOrderPtr = ermXmlGetChildNode(*p_node, EL_ASSOCIATE_ORDER);
        content = xmlNodeGetContent(appOrderPtr);
        int order = 1;
        if (content)
        {                
            sscanf(content, "%d", &order);
        }                    
        theExtInfoList->extArray[i].associateOrder = order;
    }
    regDumpExtInfoList(theExtInfoList);

    xmlXPathFreeObject(xpathObj);
    xpathObj = NULL;

    return theExtInfoList;
}

/** NOT TESTED: not used, may not work, maybe needs to be reworked to regSetExtInfoList. **
gboolean regSetExtInfo(const char *extension, const regExtInfo_t *theExtInfo, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %s %p %p", extension, theExtInfo, pRegistry);
    g_assert(extension);
    g_assert(theExtInfo);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->extensionInfo);

    regDumpExtInfo(theExtInfo);

    char xpath[BUF_LEN + 1], xpathExt[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // extensionInfo[extName=%s]
    nLen = g_snprintf(xpath, BUF_LEN, "%s[%s=\"%s\"]", pRegistry->xpaths->extensionInfo, EL_EXT_NAME, extension);

    // extensionInfo[extName=%s]/associateApp
    if (theExtInfo->associateApp)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_ASSOCIATE_APP);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, theExtInfo->associateApp);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // extensionInfo[extName=%s]/extIcon
    if (theExtInfo->iconLocation)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_EXT_ICON);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpathExt, theExtInfo->iconLocation);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    pRegistry->changed = TRUE;
    return TRUE;
}
**/

regStartUp_t *regGetStartUp(const regRegistry_t *pRegistry)
{
    regStartUp_t *theStartUp = NULL;

    ERREG_LOGPRINTF("entry");
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->startup);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theStartUp = g_new0(regStartUp_t, 1);
    g_assert(theStartUp != NULL);

    // behaviour
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->startup, EL_BEHAVIOUR);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        regFreeStartUp(theStartUp);
        theStartUp = NULL;
        return NULL;
    }
    theStartUp->behaviour = regBehaviourToEnum(szTmp);
    
    // documentPath
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->startup, EL_DOCUMENT_PATH);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        regFreeStartUp(theStartUp);
        theStartUp = NULL;
        return NULL;
    }
    theStartUp->documentPath = g_strdup(szTmp);

    regDumpStartUp(theStartUp);
    return theStartUp;

}

gboolean regSetStartUp(const regStartUp_t *theStartUp, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p %p", theStartUp, pRegistry);
    g_assert(theStartUp);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->startup);

    regDumpStartUp(theStartUp);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet = RET_OK;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // behaviour
    if ((theStartUp->behaviour >=0) && (theStartUp->behaviour < behaviourUndefined_t))
    {
        regBehaviourToString(theStartUp->behaviour, szTmp, BUF_LEN);
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->startup, EL_BEHAVIOUR);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, szTmp);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // documentPath 
    if (theStartUp->documentPath)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->startup, EL_DOCUMENT_PATH);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, theStartUp->documentPath);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  get/set functions(network profiles section)
//
///////////////////////////////////////////////////////////////////////////////
static gboolean getProxySettings(const regRegistry_t *pRegistry, const char *curXpath, regProxySetting_t *proxySettings)
{
    ERREG_LOGPRINTF("entry %p, %s, %p", pRegistry, curXpath, proxySettings);
    g_assert(pRegistry);
    g_assert(curXpath);
    g_assert(proxySettings);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nPort, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // proxyserver
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_PROXY_DETAILS, EL_PROXY_SERVER);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    proxySettings->address = g_strdup(szTmp);

    // proxyport
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_PROXY_DETAILS, EL_PROXY_PORT);
    nRet = ermXmlGetInt(pXmlCxt, xpath, &nPort);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    proxySettings->port = g_strdup_printf("%d", nPort);

    return TRUE;
}

static gboolean getIpSettings(const regRegistry_t *pRegistry, const char *curXpath, regIpSetting_t *ipSettings)
{
    ERREG_LOGPRINTF("entry %p %s %p", pRegistry, curXpath, ipSettings);
    g_assert(pRegistry);
    g_assert(curXpath);
    g_assert(ipSettings);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // ipaddress 
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_IP_STATIC_DETAILS, EL_IP_ADDRESS);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    ipSettings->address = g_strdup(szTmp);

    // netmask
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_IP_STATIC_DETAILS, EL_IP_NETMASK);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    ipSettings->netmask = g_strdup(szTmp);

    // dns
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_IP_STATIC_DETAILS, EL_IP_DNS);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    ipSettings->dns = g_strdup(szTmp);

    // gateway
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_IP_STATIC_DETAILS, EL_IP_GATEWAY);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    ipSettings->gateway = g_strdup(szTmp);

    return TRUE;
}

static gboolean getWirelessSettings(const regRegistry_t *pRegistry,
                                    const char *curXpath,
                                    regWirelessSetting_t *wirelessSettings)
{
    ERREG_LOGPRINTF("entry %p %s %p", pRegistry, curXpath, wirelessSettings);
    g_assert(pRegistry);
    g_assert(curXpath);
    g_assert(wirelessSettings);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // ssid
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_WLAN_DETAILS, EL_WLAN_SSID);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    wirelessSettings->SSID = g_strdup(szTmp);

    // encryption_type_t
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_WLAN_DETAILS, EL_WLAN_ENCRYPTION_TYPE);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    wirelessSettings->encrType = regWlanEncrToEnum(szTmp);

    // encrKey
    if (wirelessSettings->encrType == encr_wep_t || wirelessSettings->encrType == encr_wpa_t)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_WLAN_DETAILS, EL_WLAN_ENCRYPTION_KEY);
        nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
        wirelessSettings->encrKey = g_strdup(szTmp);
    }

    // wlan broadcast
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_WLAN_DETAILS, EL_WLAN_BROADCAST);
    nRet = ermXmlGetBoolean(pXmlCxt, xpath, &wirelessSettings->broadcast);
    if (RET_ERR == nRet)
    {
        // because this element is optional, don't treat it as a error
        wirelessSettings->broadcast = TRUE;
    }

    return TRUE;
}

static gboolean getDialupSettings(const regRegistry_t *pRegistry,
                                  const char *curXpath,
                                  regDialupSetting_t *dialupSettings)
{
    ERREG_LOGPRINTF("entry %p %s %p", pRegistry, curXpath, dialupSettings);
    g_assert(pRegistry);
    g_assert(curXpath);
    g_assert(dialupSettings);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // phone
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_DIALUP_DETAILS, EL_DIALUP_PHONE);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    dialupSettings->phone = g_strdup(szTmp);

    // user
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_DIALUP_DETAILS, EL_DIALUP_USER);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    dialupSettings->user = g_strdup(szTmp);
    
    // password
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_DIALUP_DETAILS, EL_DIALUP_PASSWORD);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
    dialupSettings->password = g_strdup(szTmp);

    return TRUE;
}

static int compare_profileID(const void* l, const void* r)
{
    const char*  left  = *((char**) l);
    const char*  right = *((char**) r);

    g_assert(left);
    g_assert(right);

    ERREG_LOGPRINTF("entry: left [%s] right [%s]", left, right);

    if (   strncmp(left,  "NW_", 3) == 0
        && strncmp(right, "NW_", 3) == 0)
    {
        return (atoi(left+3) - atoi(right+3));
    }
    else
    {
        return strcmp(left, right);
    }
}

regNetworkConfig_t *regGetNetworkConfig(const regRegistry_t *pRegistry)
{
    regNetworkConfig_t *theNetworkConfig = NULL;

    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->networkProfile);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, i, nProfiles, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // get the number of nodes('application') 
    nLen = g_snprintf(xpath, BUF_LEN, "%s", pRegistry->xpaths->networkProfile);
    nRet = ermXmlGetNodeNr(pXmlCxt, xpath, &nProfiles);
    if (RET_ERR == nRet || 0 == nProfiles)
    {
        ERREG_ERRORPRINTF("%s not present", xpath);
        return NULL;
    }

    // memory malloc
    theNetworkConfig = g_new0(regNetworkConfig_t, 1);
    g_assert(theNetworkConfig != NULL);

    theNetworkConfig->size = nProfiles;
    theNetworkConfig->networkList = g_new0(char *, nProfiles + 1);
    theNetworkConfig->networkList[nProfiles] = NULL;

    // get the value of attribute(networkProfile[i]/@profileID)
    for (i = 1; i <= nProfiles; i++)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s[%d]", pRegistry->xpaths->networkProfile, i);
        nRet = ermXmlGetAttributeString(pXmlCxt, xpath, ATTR_PROFILE_ID, szTmp, BUF_LEN);
        if (RET_OK == nRet)
        {
            theNetworkConfig->networkList[i - 1] = g_strdup(szTmp);
        }
    }

    // sort on profileID
    qsort( theNetworkConfig->networkList,
           theNetworkConfig->size,
           sizeof(theNetworkConfig->networkList[0]),
           compare_profileID );

    regDumpNetworkConfig(theNetworkConfig);
    return theNetworkConfig;
}

gboolean regSetNetworkConfig(const regNetworkConfig_t *theNetworkConfig, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p %p", theNetworkConfig, pRegistry);
    g_assert(theNetworkConfig);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->networkProfile);

    regDumpNetworkConfig(theNetworkConfig);

    char xpath[BUF_LEN + 1];
    int nLen, i, nProfiles, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    nProfiles = theNetworkConfig->size;
    for (i = 1; i <= nProfiles; i++)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s[%d]", pRegistry->xpaths->networkProfile, i);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetAttributeString(pXmlCxt, xpath, ATTR_PROFILE_ID,
                                         theNetworkConfig->networkList[i-1],
                                         strlen(theNetworkConfig->networkList[i-1]));
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

regNetworkProfile_t *regGetNetworkProfile(const char *ID, const regRegistry_t *pRegistry)
{
    regNetworkProfile_t *theNetworkProfile = NULL;

    ERREG_LOGPRINTF("entry: ID [%s] pXmlCxt [%p]", ID, pRegistry);
    g_assert(ID);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->networkProfile);

    char xpath[BUF_LEN + 1], xpathExt[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;
    gboolean bDhcp, bRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theNetworkProfile = g_new0(regNetworkProfile_t, 1);
    g_assert(theNetworkProfile != NULL);

    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", 
                      pRegistry->xpaths->networkProfile, ATTR_PROFILE_ID, ID);

    // profile name
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_PROFILE_NAME);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        g_snprintf(szTmp, BUF_LEN, "%s", ID);
    }
    theNetworkProfile->name = g_strdup(szTmp);

    // networkType
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_NETWORK_TYPE);
    nRet = ermXmlGetString(pXmlCxt, xpathExt, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get network type from memory.");
        regFreeNetworkProfile(theNetworkProfile);
        theNetworkProfile = NULL;
        return NULL;
    }
    theNetworkProfile->connection = regNetworkTypeToEnum(szTmp);

    // useProxy
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_USE_PROXY);
    nRet = ermXmlGetBoolean(pXmlCxt, xpathExt, &theNetworkProfile->proxy);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get use proxy flag from memory.");
        regFreeNetworkProfile(theNetworkProfile);
        theNetworkProfile = NULL;
        return NULL;
    }

    // userDhcp
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_USE_DHCP);
    nRet = ermXmlGetBoolean(pXmlCxt, xpathExt, &bDhcp);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get use dhcp flag from memory.");
        regFreeNetworkProfile(theNetworkProfile);
        theNetworkProfile = NULL;
        return NULL;
    }
    theNetworkProfile->addressMode = regIpAddressToEnum(bDhcp);

    if (theNetworkProfile->connection == wireless_t)
    {
        // wireless settings
        theNetworkProfile->wirelessSettings = g_new0(regWirelessSetting_t, 1);
        bRet = getWirelessSettings(pRegistry, xpath, theNetworkProfile->wirelessSettings);
        if (FALSE == bRet)
        {
            regFreeWirelessSetting(theNetworkProfile->wirelessSettings);
            theNetworkProfile->wirelessSettings = NULL;
            theNetworkProfile->connection = wired_t;
        }
    }
    else if (theNetworkProfile->connection == dialup_t)
    {
        // wireless settings
        theNetworkProfile->dialupSettings = g_new0(regDialupSetting_t, 1);
        bRet = getDialupSettings(pRegistry, xpath, theNetworkProfile->dialupSettings);
        if (FALSE == bRet)
        {
            regFreeDialupSetting(theNetworkProfile->dialupSettings);
            theNetworkProfile->dialupSettings = NULL;
            theNetworkProfile->connection = wired_t;
        }
    }

    // proxy settings
    if (theNetworkProfile->proxy == TRUE)
    {
        theNetworkProfile->proxySettings = g_new0(regProxySetting_t, 1);
        bRet = getProxySettings(pRegistry, xpath, theNetworkProfile->proxySettings);
        if (FALSE == bRet)
        {
            regFreeProxySetting(theNetworkProfile->proxySettings);
            theNetworkProfile->proxySettings = NULL;
            theNetworkProfile->proxy = FALSE;
        }
    }

    // IP settings
    if (theNetworkProfile->addressMode == static_t)
    {
        theNetworkProfile->ipSettings = g_new0(regIpSetting_t, 1);
        bRet = getIpSettings(pRegistry, xpath, theNetworkProfile->ipSettings);
        if (FALSE == bRet)
        {
            regFreeIpSetting(theNetworkProfile->ipSettings);
            theNetworkProfile->ipSettings = NULL;
            theNetworkProfile->addressMode = dhcp_t;
        }
    }

    // timestamp
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_TIMESTAMP);
    nRet = ermXmlGetInt(pXmlCxt, xpathExt, &theNetworkProfile->timestamp);
    if (RET_ERR == nRet)
    {
        ERREG_LOGPRINTF("no timestamp found");
        theNetworkProfile->timestamp = 0;
    }

    regDumpNetworkProfile(ID, theNetworkProfile);
    return theNetworkProfile;
}

// check if relevant values are present before adding them to the registry file
static gboolean regChkNetworkProfile(const regNetworkProfile_t *theNetworkProfile)
{
    ERREG_LOGPRINTF("entry %p", theNetworkProfile);
    g_assert(theNetworkProfile);

    if (theNetworkProfile->addressMode == static_t)
    {
        if (theNetworkProfile->ipSettings)
        {
            if ((theNetworkProfile->ipSettings->address == NULL)
                || (theNetworkProfile->ipSettings->netmask == NULL)
                || (theNetworkProfile->ipSettings->dns == NULL) 
                || (theNetworkProfile->ipSettings->gateway == NULL))
            {
                ERREG_WARNPRINTF("theNetworkProfile->ipSettings contains NULL");
                return FALSE;
            }
        }
        else
        {
            ERREG_WARNPRINTF("theNetworkProfile->ipSettings == NULL while address mode static_t");
            return FALSE;
        }
    }

    if (theNetworkProfile->proxy == TRUE)
    {
        if (theNetworkProfile->proxySettings)
        {
            if ((theNetworkProfile->proxySettings->address == NULL) 
                || (theNetworkProfile->proxySettings->port == NULL))
            {
                ERREG_WARNPRINTF("theNetworkProfile->proxySettings contains NULL");
                return FALSE;
            }
        }
        else
        {
            ERREG_WARNPRINTF("theNetworkProfile->proxySettings == NULL while proxy == TRUE");
            return FALSE;
        }
    }

    if (theNetworkProfile->connection == wireless_t)
    {
        if (theNetworkProfile->wirelessSettings)
        {
            switch (theNetworkProfile->wirelessSettings->encrType)
            {
                case encr_none_t:
                    break;
                case encr_wep_t:
                case encr_wpa_t:
                    if (theNetworkProfile->wirelessSettings->encrKey == NULL
                       /* || theNetworkProfile->wirelessSettings->encrKey[0] == '\0' */)
                    {
                        ERREG_WARNPRINTF("theNetworkProfile->wirelessSettings->encrKey missing");
                        return FALSE;
                    }
                    break;
                default:
                    ERREG_WARNPRINTF
                        ("theNetworkProfile->wirelessSettings->encrType illegal value [%d]",
                         theNetworkProfile->wirelessSettings->encrType);
                    return FALSE;
            }
        }
        else
        {
            ERREG_WARNPRINTF("theNetworkProfile->wirelessSettings == NULL while connection == wireless_t ");
            return FALSE;
        }
    }

    return TRUE;
}

static gboolean setWirelessSettings(regRegistry_t *pRegistry, char *curXpath, regWirelessSetting_t *wirelessSettings)
{
    ERREG_LOGPRINTF("entry %p %s %p", pRegistry, curXpath, wirelessSettings);
    g_assert(pRegistry);
    g_assert(curXpath);
    g_assert(wirelessSettings);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // ssid
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_WLAN_DETAILS, EL_WLAN_SSID);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, wirelessSettings->SSID);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // encryption_type_t
    regWlanEncrToString(wirelessSettings->encrType, szTmp, BUF_LEN);
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_WLAN_DETAILS, EL_WLAN_ENCRYPTION_TYPE);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, szTmp);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // encrKey
    if (wirelessSettings->encrType == encr_wep_t || wirelessSettings->encrType == encr_wpa_t)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_WLAN_DETAILS, EL_WLAN_ENCRYPTION_KEY);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, wirelessSettings->encrKey);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // broadcast
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_WLAN_DETAILS, EL_WLAN_BROADCAST);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetBoolean(pXmlCxt, xpath, wirelessSettings->broadcast);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    return TRUE;
}

static gboolean setDialupSettings(regRegistry_t *pRegistry, char *curXpath, regDialupSetting_t *dialupSettings)
{
    ERREG_LOGPRINTF("entry %p %s %p", pRegistry, curXpath, dialupSettings);
    g_assert(pRegistry);
    g_assert(curXpath);
    g_assert(dialupSettings);

    char xpath[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // phone
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_DIALUP_DETAILS, EL_DIALUP_PHONE);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, dialupSettings->phone);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }
   
    // user
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_DIALUP_DETAILS, EL_DIALUP_USER);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, dialupSettings->user);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // password
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_DIALUP_DETAILS, EL_DIALUP_PASSWORD);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, dialupSettings->password);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    return TRUE;
}


static gboolean setProxySettings(regRegistry_t *pRegistry, char *curXpath, regProxySetting_t *proxySettings)
{
    ERREG_LOGPRINTF("entry %p %s %p", pRegistry, curXpath, proxySettings);
    g_assert(pRegistry);
    g_assert(curXpath);
    g_assert(proxySettings);

    char xpath[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // proxyserver
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_PROXY_DETAILS, EL_PROXY_SERVER);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, proxySettings->address);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // proxyport
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_PROXY_DETAILS, EL_PROXY_PORT);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, proxySettings->port);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    return TRUE;
}

static gboolean setIpSettings(regRegistry_t *pRegistry, char *curXpath, regIpSetting_t *ipSettings)
{
    ERREG_LOGPRINTF("entry %p %s %p", pRegistry, curXpath, ipSettings);
    g_assert(pRegistry);
    g_assert(curXpath);
    g_assert(ipSettings);

    char xpath[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // ipaddress 
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_IP_STATIC_DETAILS, EL_IP_ADDRESS);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, ipSettings->address);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // netmask
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_IP_STATIC_DETAILS, EL_IP_NETMASK);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, ipSettings->netmask);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // dns
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_IP_STATIC_DETAILS, EL_IP_DNS);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, ipSettings->dns);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // gateway
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", curXpath, EL_IP_STATIC_DETAILS, EL_IP_GATEWAY);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, ipSettings->gateway);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    return TRUE;
}

gboolean regSetNetworkProfile(const char *ID, const regNetworkProfile_t *theNetworkProfile, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %s %p %p", ID, theNetworkProfile, pRegistry);
    g_assert(ID);
    g_assert(theNetworkProfile);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->networkProfile);

    regDumpNetworkProfile(ID, theNetworkProfile);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1], xpathExt[BUF_LEN + 1];
    int nLen, nRet;
    gboolean bDhcp, bRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    if (FALSE == regChkNetworkProfile(theNetworkProfile))
    {
        return FALSE;
    }

    // xpath
    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->networkProfile, ATTR_PROFILE_ID, ID);

    // profile name
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_PROFILE_NAME);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpathExt, theNetworkProfile->name);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // networkType
    regNetworkTypeToString(theNetworkProfile->connection, szTmp, BUF_LEN);
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_NETWORK_TYPE);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpathExt, szTmp);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // useProxy
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_USE_PROXY);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetBoolean(pXmlCxt, xpathExt, theNetworkProfile->proxy);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // userDhcp
    nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_USE_DHCP);
    bDhcp = regIpAddressToBool(theNetworkProfile->addressMode);
    nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetBoolean(pXmlCxt, xpathExt, bDhcp);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    if (theNetworkProfile->connection == wireless_t)
    {
        // wireless settings
        bRet = setWirelessSettings(pRegistry, xpath, theNetworkProfile->wirelessSettings);
        if (FALSE == bRet)
        {
            return FALSE;
        }
    }
    else if (theNetworkProfile->connection == dialup_t)
    {
        // dialup settings
        bRet = setDialupSettings(pRegistry, xpath, theNetworkProfile->dialupSettings);
        if (FALSE == bRet)
        {
            return FALSE;
        }
    }

    // proxy settings
    if (theNetworkProfile->proxy == TRUE)
    {
        bRet = setProxySettings(pRegistry, xpath, theNetworkProfile->proxySettings);
        if (FALSE == bRet)
        {
            return FALSE;
        }
    }

    // IP settings
    if (theNetworkProfile->addressMode == static_t)
    {
        bRet = setIpSettings(pRegistry, xpath, theNetworkProfile->ipSettings);
        if (FALSE == bRet)
        {
            return FALSE;
        }
    }

    // timestamp
    if (theNetworkProfile->timestamp != 0)
    {
        nLen = g_snprintf(xpathExt, BUF_LEN, "%s/%s", xpath, EL_TIMESTAMP);
        nRet = ermXmlCheckXpath(pXmlCxt, xpathExt);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetInt(pXmlCxt, xpathExt, theNetworkProfile->timestamp);
        }
        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

gboolean regRemoveNetworkProfile(const char *ID, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(ID);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->networkProfile);

    char xpath[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // xpath
    nLen = g_snprintf(xpath, BUF_LEN, "%s[@%s=\"%s\"]", pRegistry->xpaths->networkProfile, ATTR_PROFILE_ID, ID);
    nRet = ermXmlRemoveNode(pXmlCxt, xpath);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    return TRUE;
}

gboolean regRemoveNetworkConfig(regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->networkProfiles);

    char xpath[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // xpath
    nLen = g_snprintf(xpath, BUF_LEN, "%s", pRegistry->xpaths->networkProfiles);
    nRet = ermXmlRemoveNode(pXmlCxt, xpath);
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    return TRUE;
}

void regGetNetworkProfiles(regNetworkConfig_t **pNetworkConfig, regNetworkProfile_t ***pNetworks, const regRegistry_t* pRegistry)
{
    int i;
    regNetworkConfig_t *theNetworkConfig = NULL;
    regNetworkProfile_t **theNetworks = NULL;

    ERREG_LOGPRINTF("entry");
    g_assert(pNetworkConfig);
    g_assert(pNetworks);
    g_assert(pRegistry);

    //destroy old profiles, just in case
    regFreeNetworkProfiles(*pNetworkConfig, *pNetworks);
    *pNetworkConfig = NULL;
    *pNetworks = NULL;

    // retieve the network profile list data
    theNetworkConfig = regGetNetworkConfig(pRegistry);
    if (theNetworkConfig == NULL)
    {
        ERREG_WARNPRINTF("theNetworkConfig not present");
    }
    else
    {
        ERREG_LOGPRINTF("theNetworkConfig exist");

        theNetworks = g_new0(regNetworkProfile_t *, theNetworkConfig->size);
        for (i = 0; i < theNetworkConfig->size; i++)
        {
            //retrieve the network data 
            theNetworks[i] = regGetNetworkProfile(theNetworkConfig->networkList[i], pRegistry);
            ERREG_LOGPRINTF("theNetworks[%d] = [%p]", i, theNetworks[i]);
        }
    }

    *pNetworkConfig = theNetworkConfig;
    *pNetworks = theNetworks;
}

gboolean regSetNetworkProfiles(const regNetworkConfig_t *theNetworkConfig, const regNetworkProfile_t **theNetworks, regRegistry_t* pRegistry)
{
    ERREG_LOGPRINTF("entry");
    g_assert(theNetworkConfig);
    g_assert(theNetworks);
    g_assert(pRegistry);

    gboolean bRet = TRUE;  // TRUE = ok
    gboolean b;
    int      i;
    
    // remove the ones
    regRemoveNetworkConfig(pRegistry);
    // set the new ones
    regSetNetworkConfig(theNetworkConfig, pRegistry);
    for (i = 0; i < theNetworkConfig->size; i++)
    {
        b = regSetNetworkProfile(theNetworkConfig->networkList[i], theNetworks[i],  pRegistry);
        if (FALSE == b)
        {
            bRet = FALSE;
        }
    }

    if (bRet)
    {
        pRegistry->changed = TRUE;
    }
    return TRUE;
}

regLastConnect_t *regGetLastConnect(const regRegistry_t *pRegistry)
{
    regLastConnect_t *theLastConnect = NULL;

    ERREG_LOGPRINTF("entry");
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->lastConnect);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    theLastConnect = g_new0(regLastConnect_t, 1);
    g_assert(theLastConnect != NULL);

    // profileConnectedIDS
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->lastConnect, EL_PROFILE_CONNECTED_IDS);
    nRet = ermXmlGetInt(pXmlCxt, xpath, &theLastConnect->profileConnectedIDS);
    if (RET_ERR == nRet)
    {
        regFreeLastConnect(theLastConnect);
        theLastConnect = NULL;
        return NULL;
    }
    
    // profileConnectedPC
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->lastConnect, EL_PROFILE_CONNECTED_PC);
    nRet = ermXmlGetInt(pXmlCxt, xpath, &theLastConnect->profileConnectedPC);
    if (RET_ERR == nRet)
    {
        regFreeLastConnect(theLastConnect);
        theLastConnect = NULL;
        return NULL;
    }

    // connect to which server
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->lastConnect, EL_MANUAL_CONNECT_TYPE);
    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get which server to connect from memory.");
        regFreeLastConnect(theLastConnect);
        theLastConnect = NULL;
        return NULL;
    }
    theLastConnect->manualConnectType = regServerTypeToEnum(szTmp);

    regDumpLastConnect(theLastConnect);
    return theLastConnect;
}

gboolean regSetLastConnect(const regLastConnect_t *theLastConnect, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p %p", theLastConnect, pRegistry);
    g_assert(theLastConnect);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->lastConnect);

    regDumpLastConnect(theLastConnect);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1];
    int nLen, nRet = RET_OK;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // profileConnectedIDS
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->lastConnect, EL_PROFILE_CONNECTED_IDS);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetInt(pXmlCxt, xpath, theLastConnect->profileConnectedIDS);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // profileConnectedPC
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->lastConnect, EL_PROFILE_CONNECTED_PC);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetInt(pXmlCxt, xpath, theLastConnect->profileConnectedPC);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }

    // connect to which server
    regServerTypeToString(theLastConnect->manualConnectType, szTmp, BUF_LEN);
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->lastConnect, EL_MANUAL_CONNECT_TYPE);
    nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetString(pXmlCxt, xpath, szTmp);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }       

    pRegistry->changed = TRUE;
    return TRUE;
}

gboolean regGetAutoConnectWlan(const regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry");
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->autoConnectWlan);

    gboolean enable = TRUE;
    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);
    char* xpath = pRegistry->xpaths->autoConnectWlan;
    int nRet = ermXmlGetBoolean(pXmlCxt, xpath, &enable);

    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("Can't get the settings %s", xpath);
    }

    regDumpAutoConnectWlan(enable);
    return enable;
}

gboolean regSetAutoConnectWlan(gboolean enable, regRegistry_t *pRegistry)
{
    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->autoConnectWlan);

    regDumpAutoConnectWlan(enable);

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);
    char *xpath = pRegistry->xpaths->autoConnectWlan;
    int nRet = ermXmlCheckXpath(pXmlCxt, xpath);
    if (RET_OK == nRet)
    {
        nRet = ermXmlSetBoolean(pXmlCxt, xpath, enable);
    }
    if (RET_ERR == nRet)
    {
        return FALSE;
    }      

    pRegistry->changed = TRUE;
    return TRUE;
}   

///////////////////////////////////////////////////////////////////////////////
//
//  get/set functions(toolbar section)
//
///////////////////////////////////////////////////////////////////////////////
regIconSet_t *regGetIconSet(const regRegistry_t *pRegistry)
{
    regIconSet_t *theRegIconSet = NULL;

    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->toolbarIcons);
    g_assert(pRegistry->xpaths->toolbarIcon);

    char xpath[BUF_LEN + 1], szTmp[BUF_LEN + 1], szBase[BUF_LEN + 1];
    int nLen, nIcons, nFiles, i, j, nRet;

    const erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // get the number of Nodes('toolbarIcon')
    nLen = g_snprintf(xpath, BUF_LEN, "%s", pRegistry->xpaths->toolbarIcon);
    nRet = ermXmlGetNodeNr(pXmlCxt, xpath, &nIcons);
    if (RET_ERR == nRet || 0 == nIcons)
    {
        ERREG_ERRORPRINTF("%s not present", xpath);
        return NULL;
    }

    // get the string of node('iconBasePath')
    nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->toolbarIcons, EL_ICON_BASE_PATH);
    nRet = ermXmlGetString(pXmlCxt, xpath, szBase, BUF_LEN);
    if (RET_ERR == nRet)
    {
        ERREG_ERRORPRINTF("%s not present", xpath);
        return NULL;
    }

    // memory malloc according to the 'nIcons' 
    theRegIconSet = g_new0(regIconSet_t, 1);
    theRegIconSet->iconArray = g_new0(regIcon_t, nIcons);
    theRegIconSet->nIcons = nIcons;

    // walk through and get 'toolbarIcon[i]'      
    for (i = 1; i <= nIcons; i++)
    {
        // get the name of node('toolbarIcon[i]') 
        nLen = g_snprintf(xpath, BUF_LEN, "%s[%d]", pRegistry->xpaths->toolbarIcon, i);
        nRet = ermXmlGetAttributeString(pXmlCxt, xpath, ATTR_NAME, szTmp, BUF_LEN);
        if (RET_OK == nRet)
        {
            theRegIconSet->iconArray[i - 1].iconName = g_strdup(szTmp);

            // get the number of node('iconFileName') in node('toolbarIcon[i]')
            nLen = g_snprintf(xpath, BUF_LEN, "%s[%d]/%s", pRegistry->xpaths->toolbarIcon, i, EL_ICON_FILE_NAME);
            nRet = ermXmlGetNodeNr(pXmlCxt, xpath, &nFiles);

            if ((RET_OK == nRet) && (nFiles != 0))
            {
                // memory malloc according to the 'nFiles'
                theRegIconSet->iconArray[i - 1].iconPathsSize = nFiles;
                theRegIconSet->iconArray[i - 1].iconPaths = g_new0(char *, (nFiles + 1));
                theRegIconSet->iconArray[i - 1].iconPaths[nFiles] = NULL;

                // walk through and get 'toolbarIcon[i]/iconFileName[@id=j]'
                for (j = 1; j <= nFiles; j++)
                {
                    nLen = g_snprintf(xpath, BUF_LEN, "%s[%d]/%s[@id=\"%d\"]",
                                                       pRegistry->xpaths->toolbarIcon,
                                                          i,  
                                                              EL_ICON_FILE_NAME,
                                                                        j       );
                    nRet = ermXmlGetString(pXmlCxt, xpath, szTmp, BUF_LEN);
                    if (RET_OK == nRet)
                    {
                        theRegIconSet->iconArray[i - 1].iconPaths[j - 1] = g_strdup_printf("%s%s", szBase, szTmp);
                    }
                }           // for (j...);
            }               // if ((RET_OK == nRet) && (nFiles != 0))
        }                   // if (RET_OK == nRet)
    }                       // for (i...)

    regDumpIconSet(theRegIconSet);
    return theRegIconSet;
}

///////////////////////////////////////////////////////////////////////////////
//
//  get/set functions(device details section)
//
///////////////////////////////////////////////////////////////////////////////
deviceDetails_t* regGetDeviceDetails(const regRegistry_t *pRegistry)
{
    deviceDetails_t *theDeviceDetails = NULL;
    
    ERREG_LOGPRINTF("entry %p", pRegistry);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);

    char* cp = NULL;

    // memory malloc 
    theDeviceDetails = g_new0(deviceDetails_t, 1);
    g_assert(theDeviceDetails != NULL);

    theDeviceDetails->software = g_new0(software_t, 1);
    g_assert(theDeviceDetails->software);
    
    // build name
    theDeviceDetails->software->szBuildName = g_strdup(get_software_version_commercial());
    
    // build number
    theDeviceDetails->software->szBuildNum = g_strdup(get_software_version());

    // mac address of wireless
//    theDeviceDetails->szMacOfWireless = g_strdup(get_wireless_macAddress());
    theDeviceDetails->szMacOfWireless = g_strdup(get_wlanmactxt_macAddress());
       
    // mac address of wired
//    theDeviceDetails->szMacOfWired = g_strdup(get_wired_macAddress());
    theDeviceDetails->szMacOfWired = g_strdup(get_sysset_macAddress());
   
    // public key
    if (get_public_key(&cp) <= 0)
    {
        theDeviceDetails->szPublicKey = g_strdup("");
    }
    else
    {
        theDeviceDetails->szPublicKey = cp;
    }

    regDumpDeviceDetails(theDeviceDetails);
    return theDeviceDetails;
}

gboolean regSetDeviceDetails(const deviceDetails_t* theDeviceDetails, regRegistry_t* pRegistry)
{
    ERREG_LOGPRINTF("entry %p %p", theDeviceDetails, pRegistry);
    g_assert(theDeviceDetails);
    g_assert(pRegistry);
    g_assert(pRegistry->xpaths);
    g_assert(pRegistry->xpaths->deviceDetails);
    
    regDumpDeviceDetails(theDeviceDetails);

    char xpath[BUF_LEN + 1];
    int nLen, nRet;

    erManifest *pXmlCxt = &(pRegistry->xmlCxt);

    // software
    if (theDeviceDetails->software)
    {
        // buildName
        if (theDeviceDetails->software->szBuildName)
        {
            nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->deviceDetails, EL_SOFTWARE, EL_BUILD_NAME);
            nRet = ermXmlCheckXpath(pXmlCxt, xpath);
            if (RET_OK == nRet)
            {
                nRet = ermXmlSetString(pXmlCxt, xpath, theDeviceDetails->software->szBuildName);
            }

            if (RET_ERR == nRet)
            {
                return FALSE;
            }
        }

        // buildNum
        if (theDeviceDetails->software->szBuildNum)
        {
            nLen = g_snprintf(xpath, BUF_LEN, "%s/%s/%s", pRegistry->xpaths->deviceDetails, EL_SOFTWARE, EL_BUILD_NUM);
            nRet = ermXmlCheckXpath(pXmlCxt, xpath);
            if (RET_OK == nRet)
            {
                nRet = ermXmlSetString(pXmlCxt, xpath, theDeviceDetails->software->szBuildNum);
            }

            if (RET_ERR == nRet)
            {
                return FALSE;
            }
        }
    }

    // szMacOfWired
    if (theDeviceDetails->szMacOfWired)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->deviceDetails, EL_MAC_OF_WIRED);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, theDeviceDetails->szMacOfWired);
        }

        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    // szMacOfWireless
    if (theDeviceDetails->szMacOfWireless)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->deviceDetails, EL_MAC_OF_WIRELESS);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, theDeviceDetails->szMacOfWireless);
        }

        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }
   
    // szPublicKey
    if (theDeviceDetails->szPublicKey)
    {
        nLen = g_snprintf(xpath, BUF_LEN, "%s/%s", pRegistry->xpaths->deviceDetails, EL_PUBLIC_KEY);
        nRet = ermXmlCheckXpath(pXmlCxt, xpath);
        if (RET_OK == nRet)
        {
            nRet = ermXmlSetString(pXmlCxt, xpath, theDeviceDetails->szPublicKey);
        }

        if (RET_ERR == nRet)
        {
            return FALSE;
        }
    }

    pRegistry->changed = TRUE;
    return TRUE;
}

