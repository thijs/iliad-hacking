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

#ifndef ERDM_INTERNAL_H
#define ERDM_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "erdm.h"

#define DMPORT 50555

#define DM_MAXCHARONLINE 1024
#define DM_CMD_NAME 128
#define DM_ARGLENGTH 256
#define DM_N_ARG       6

// Define a struct for each PML command
// Assemble all PML commands in one union.
typedef struct
{
    eDmCommandCode cmd;
    eDmCmdPriority priority;
    int     par0;
    int     par1;
    int     par2;
} sDmCommandGeneric;

typedef struct
{
    eDmCommandCode cmd;
    eDmCmdPriority priority;
    eDmQuality qual;
} sDmCommandDisplay;

typedef struct
{
    eDmCommandCode cmd;
    eDmCmdPriority priority;
    eDmQuality qual;
    int     xUp;
    int     yUp;
    int     xDown;
    int     yDown;
} sDmCommandDisplayPartial;

typedef struct
{
    eDmCommandCode cmd;
    eDmCmdPriority priority;
    char   *szFilename;
    int     nFilenam;
} sDmCommandDump;

typedef union
{
    sDmCommandGeneric dmCmdGeneric;
    sDmCommandDisplay dmCmdDisplay;
    sDmCommandDisplayPartial dmCmdDisplayPartial;
    sDmCommandDump dmCmdDump;
} uDmCommand;

typedef struct
{
    char    name[DM_CMD_NAME]; // Command Name
    int     cc;             // Command Code
    char    arg[DM_N_ARG][DM_ARGLENGTH];
    int     nArg;
} erDmCmd_t;

void erDmGetArgs(char *pChar, erDmCmd_t * pCmd, int nReqArgs);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
