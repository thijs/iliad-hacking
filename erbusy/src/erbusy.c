/*
 * This file is part of erbusy.
 *
 * erbusy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * erbusy is distributed in the hope that it will be useful,
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

#include <liberipc/eripc.h>
#include <liberipc/eripcbusyd.h>

#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    erClientChannel_t channel;
    erIpcCmd_t  cmd;

    erIpcStartClient(ER_BUSYD_CHANNEL, &channel);
    strcpy(cmd.name, "busySetBusy");
    cmd.cc = ccBusySetBusy;
    cmd.nArg = 1;
    strcpy(cmd.arg[0], argv[1]);
    erIpcSndCommand(channel, &cmd);

    return 0;
}
