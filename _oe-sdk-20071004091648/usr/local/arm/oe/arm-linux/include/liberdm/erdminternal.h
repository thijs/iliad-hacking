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
