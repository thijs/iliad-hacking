#ifndef SCBPATH_H_
#define SCBPATH_H_

#include "scbconfig.h"
#include <glib.h>

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct _ScbPath
{
    char    dirname[SCB_MAX_PATH];
    char    manifest[SCB_MAX_PATH];
    char    scbname[SCB_MAX_PATH];
}ScbPath ;

typedef ScbPath* ScbPathPtr;
void scb_path_init(ScbPathPtr ptr);
gboolean scb_path_file_exist(const char *pathName);


#ifdef __cplusplus
}
#endif

#endif

