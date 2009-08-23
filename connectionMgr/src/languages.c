/*
 * This file is part of connectionMgr.
 *
 * connectionMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * connectionMgr is distributed in the hope that it will be useful,
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
#include <glib.h>
#include "config.h"
#include <locale.h>
#include "gettext.h"
#include "languages.h"

#include <liberregxml/erregapi.h>

void languagesInit()
{
    char   *result;
    regUserSetting_t *theUserSetting = NULL;

    // tryout
    //setlocale(LC_ALL,"nl_NL");
    
    // get sections from registry 
    theUserSetting = erRegGetUserSetting();
    if (theUserSetting)
    {
        setlocale(LC_ALL, theUserSetting->language);
        erRegFreeUserSetting(theUserSetting);
    }
    else
    {
        // default is english
        setlocale(LC_ALL, "");
    }

    textdomain(PACKAGE);

    result = bindtextdomain(PACKAGE, LOCALE_DIR);;

    textdomain(PACKAGE);
}
