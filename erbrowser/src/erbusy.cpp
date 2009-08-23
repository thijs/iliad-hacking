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
 * \file erbusy.c
 *
 * Copyright (C) 2006-2008 iRex Technologies B.V.
 */

#include <liberipc/eripc.h>
#include <liberipc/eripcbusyd.h>
#include "browserLog.h"

static erClientChannel_t erbusyChannel;

void erbusy_init()
{
    BR_IPCPRINTF("entry");
    erIpcStartClient(ER_BUSYD_CHANNEL, &erbusyChannel);
}


static void erbusy_request(eCcBusyState busy)
{
    BR_IPCPRINTF("entry %d", busy);
    busySetBusy(erbusyChannel, busy);
    return;
}

void erbusy_off()
{
    erbusy_request(ccBusyState_Off);
}

void erbusy_blink()
{
    erbusy_request(ccBusyState_Blink);
}

void erbusy_on()
{
    erbusy_request(ccBusyState_On);
}

void erbusy_shutdown()
{
    erbusy_request(ccBusyState_Shutdown);
}

