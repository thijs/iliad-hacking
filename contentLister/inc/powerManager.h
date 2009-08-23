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
 * \file programManager.c
 * \brief content lister - eReader program manager
 * 
 * <File description>
 * 
 * Manages the programs running on the eReader
 * spawns processes and sends keys
 */
 
 /* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __POWERMANAGER__H__
#define __POWERMANAGER__H__

#define MHZ                             1000    // CPU speed is in KHz, not MHz
#define CPU_FULL_SPEED_FREQUENCY        400 * MHZ
#define CPU_HALF_SPEED_FREQUENCY        200 * MHZ
#define CPU_IDLE_SPEED_FREQUENCY        33 * MHZ

#define CPU_FULL_SPEED_TIMEOUT          0       // Do it NOW
#define CPU_HALF_SPEED_TIMEOUT          3
#define CPU_IDLE_SPEED_TIMEOUT          10

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int power_init();

void power_cpu_full();

void power_cpu_half();

void power_cpu_idle();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
