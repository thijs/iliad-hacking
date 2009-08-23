/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file mozillaPreferences.h
 * \brief browser - interface with gecko for setting/getting preference values
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#include "gtkmozembed.h"
#include "gtkmozembed_internal.h"

#include "nsCOMPtr.h"
#include "nsNetUtil.h"
#include "nsIPrefService.h"
#include "nsIPrefBranch.h"


gboolean mozilla_save_prefs(void)
{
    nsCOMPtr < nsIPrefService > prefService = do_GetService(NS_PREFSERVICE_CONTRACTID);
    g_return_val_if_fail(prefService != nsnull, FALSE);

    nsresult rv = prefService->SavePrefFile(nsnull);

    return NS_SUCCEEDED(rv) ? TRUE : FALSE;
}

/**
 * set a string mozilla preference
 */
gboolean mozilla_pref_set_char(const char *preferenceName, const char *newValue)
{
    g_return_val_if_fail(preferenceName != NULL, FALSE);

    /*It is legitimate to pass in a NULL value sometimes. So let's not
     *assert and just check and return.
     */
    if (!newValue)
        return FALSE;

    nsCOMPtr < nsIPrefService > prefService = do_GetService(NS_PREFSERVICE_CONTRACTID);
    nsCOMPtr < nsIPrefBranch > pref;
    prefService->GetBranch("", getter_AddRefs(pref));

    if (pref)
    {
        nsresult rv = pref->SetCharPref(preferenceName, newValue);

        return NS_SUCCEEDED(rv) ? TRUE : FALSE;
    }

    return FALSE;
}

/**
 * set a boolean mozilla preference
 */
gboolean mozilla_pref_set_boolean(const char *preferenceName, gboolean newValue)
{
    g_return_val_if_fail(preferenceName != NULL, FALSE);

    nsCOMPtr < nsIPrefService > prefService = do_GetService(NS_PREFSERVICE_CONTRACTID);
    nsCOMPtr < nsIPrefBranch > pref;
    prefService->GetBranch("", getter_AddRefs(pref));

    if (pref)
    {
        nsresult rv = pref->SetBoolPref(preferenceName, newValue ? PR_TRUE : PR_FALSE);

        return NS_SUCCEEDED(rv) ? TRUE : FALSE;
    }

    return FALSE;
}

/**
 * set an integer mozilla preference
 */
gboolean mozilla_pref_set_int(const char *preferenceName, int newValue)
{
    g_return_val_if_fail(preferenceName != NULL, FALSE);

    nsCOMPtr < nsIPrefService > prefService = do_GetService(NS_PREFSERVICE_CONTRACTID);
    nsCOMPtr < nsIPrefBranch > pref;
    prefService->GetBranch("", getter_AddRefs(pref));

    if (pref)
    {
        nsresult rv = pref->SetIntPref(preferenceName, newValue);

        return NS_SUCCEEDED(rv) ? TRUE : FALSE;
    }
    return FALSE;
}

/**
 * get a boolean mozilla preference
 */
gboolean mozilla_pref_get_boolean(const char *preferenceName, gboolean defaultValue)
{
    PRBool  value;

    g_return_val_if_fail(preferenceName != NULL, FALSE);

    nsCOMPtr < nsIPrefService > prefService = do_GetService(NS_PREFSERVICE_CONTRACTID);
    nsCOMPtr < nsIPrefBranch > pref;
    prefService->GetBranch("", getter_AddRefs(pref));

    if (pref)
    {
        nsresult result;

        result = pref->GetBoolPref(preferenceName, &value);
        if (NS_FAILED(result))
            return defaultValue;
    }

    return (value == PR_TRUE) ? TRUE : FALSE;
}

/**
 * get an integer mozilla preference / default value -1
 */
gint mozilla_pref_get_int(const char *preferenceName)
{
    int     value = -1;

    g_return_val_if_fail(preferenceName != NULL, FALSE);

    nsCOMPtr < nsIPrefService > prefService = do_GetService(NS_PREFSERVICE_CONTRACTID);
    nsCOMPtr < nsIPrefBranch > pref;
    prefService->GetBranch("", getter_AddRefs(pref));

    if (pref)
    {
        pref->GetIntPref(preferenceName, &value);
    }

    return value;
}

/**
 * get a string mozilla preference (calling app should handle free)
 */
gchar  *mozilla_pref_get_char(const char *preferenceName)
{
    gchar  *value = NULL;
    gchar  *result = NULL;

    g_return_val_if_fail(preferenceName != NULL, FALSE);
    nsCOMPtr < nsIPrefService > prefService = do_GetService(NS_PREFSERVICE_CONTRACTID);
    nsCOMPtr < nsIPrefBranch > pref;
    prefService->GetBranch("", getter_AddRefs(pref));

    if (pref)
    {
        pref->GetCharPref(preferenceName, &value);
    }

    if (value)
    {
        result = g_strdup(value);
        nsMemory::Free(value);
    }

    return result;
}

/**
 * remove a mozilla preference
 */
gboolean mozilla_pref_remove(const char *preferenceName)
{
    g_return_val_if_fail(preferenceName != NULL, FALSE);

    nsCOMPtr < nsIPrefService > prefService = do_GetService(NS_PREFSERVICE_CONTRACTID);
    nsCOMPtr < nsIPrefBranch > pref;
    prefService->GetBranch("", getter_AddRefs(pref));

    if (pref)
    {
        nsresult rv = pref->ClearUserPref(preferenceName);

        return NS_SUCCEEDED(rv) ? TRUE : FALSE;
    }

    return FALSE;
}
