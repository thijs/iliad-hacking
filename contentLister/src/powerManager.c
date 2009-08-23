/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file powerManager.c
 * \brief content lister - eReader power manager
 * 
 * <File description>
 * 
 * Interaction of contentLister with the powerManager
 * simple implementation is used at the moment
 */
 
/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2006-2008 iRex Technologies
 * All rights reserved.
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
#include <config.h>

#include <liberipc/eripc.h>
#include <liberipc/eripcpowermgr.h>

#include "powerManager.h"


static erClientChannel_t powerChannel;

int power_init()
{
    return erIpcStartClient(ER_POWERMGR_CHANNEL, &powerChannel);
}

void power_cpu_full()
{
    pwrGotoIdleMode(powerChannel, CPU_FULL_SPEED_FREQUENCY, CPU_FULL_SPEED_TIMEOUT);
}

void power_cpu_half()
{
    pwrGotoIdleMode(powerChannel, CPU_HALF_SPEED_FREQUENCY, CPU_HALF_SPEED_TIMEOUT);
}

void power_cpu_idle()
{
    pwrGotoIdleMode(powerChannel, CPU_IDLE_SPEED_FREQUENCY, CPU_IDLE_SPEED_TIMEOUT);
}

// TODO: More powerManagement functionality needs to be implemented here...
