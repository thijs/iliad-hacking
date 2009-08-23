#ifndef SCB_H_
#define SCB_H_

#ifdef __cplusplus
extern "C"
{
#endif

// all header files exposed
#include "scbcolor.h"
#include "scbconfig.h"
#include "scbpage.h"
#include "scbpages.h"
#include "scbpath.h"
#include "scbpoint.h"
#include "scbpoints.h"
#include "scbstroke.h"
#include "scbstrokes.h"
#include "scbdoc.h"
#include "scbtype.h"
#include "scbmacro.h"

void scb_init();
void scb_uninit();

#ifdef __cplusplus
}
#endif

#endif
