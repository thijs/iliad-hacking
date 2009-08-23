/*
 * This file is part of liberscribble.
 *
 * liberscribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberscribble is distributed in the hope that it will be useful,
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
