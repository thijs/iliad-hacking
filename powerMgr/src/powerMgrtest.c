/*
 * This file is part of powerMgr.
 *
 * powerMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * powerMgr is distributed in the hope that it will be useful,
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

#include <string.h>
#include <unistd.h>

#include <liberipc/eripc.h>
#include <liberipc/eripcpowermgr.h>

int
main(int argc, char *argv[])
{
    erClientChannel_t channel;

    if (argc < 3)
    {
	fprintf(stderr, "usage: %s [wifi | audio | cpu ] [0 |1 | #Mhz]\n",
		argv[0]);
	return -1;
    }

    erIpcStartClient(ER_POWERMGR_CHANNEL, &channel);

    if (0 == strcmp(argv[1], "audio"))
    {
	printf("setting audio to %d\n", atoi(argv[2]));
	pwrSetAC97(channel, atoi(argv[2]));
    }
    /** Outdated **
    if (0 == strcmp(argv[1], "wifi"))
    {
	printf("setting wifi to %d\n", atoi(argv[2]));
	pwrSetWifi(channel, atoi(argv[2]));
    }
    ** Outdated **/
    if (0 == strcmp(argv[1], "cpu"))
    {
	printf("setting cpu speed to %d\n", atoi(argv[2]));
  if (argc > 3)
	  pwrGotoIdleMode(channel, atoi(argv[2]), atoi(argv[3]));
  else
    pwrGotoIdleMode(channel, atoi(argv[2]), 0);
    }
    return 0;
}
