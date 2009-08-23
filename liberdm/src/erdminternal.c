/*
 * This file is part of liberdm.
 *
 * liberdm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberdm is distributed in the hope that it will be useful,
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "erdminternal.h"

void erDmGetArgs(char *pChar, erDmCmd_t * pCmd, int nReqArgs)
{
    int     nArg;
    int     i;
    char    szToken[DM_MAXCHARONLINE];

    for (nArg = 0; nArg < nReqArgs; nArg++)
    {
        i = 0;
        while (*pChar != '\0')
        {
            szToken[i] = '\0';
            if (*pChar == ',')
            {
                pChar++;
                break;
            }
            szToken[i] = *pChar++;
            i++;
            szToken[i] = '\0';
        }
        strcpy(pCmd->arg[nArg], szToken);
    }
}
