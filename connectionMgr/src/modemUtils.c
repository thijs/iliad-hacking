/*
 * This file is part of connectionMgr.
 *
 * connectionMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * connectionMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file modemUtils.c
 * \brief connectionMgr - provide wireless modem related utilities
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

#include "connectionMgrLog.h"
#include "modemUtils.h"

#define DEVICE_NAME    "/dev/modem"
#define BAUD_RATE      B115200

int get_modem_signal_strength()
{
    CN_LOGPRINTF("entry");

    int fd = open(DEVICE_NAME, O_RDWR | O_NOCTTY);

    if (fd < 0)
    {
        CN_ERRORPRINTF("Can't open %s", DEVICE_NAME);
        return 0;
    }

    struct termios new_opt, old_opt;

    // Save original settings
    tcgetattr(fd, &old_opt);

    // Set terminal attribute
    new_opt.c_cflag = BAUD_RATE | CS8 | CLOCAL | CREAD;
    new_opt.c_iflag = IGNPAR;
    new_opt.c_oflag = 0;
    new_opt.c_lflag = 0;

    // Blocking read until 1 char arrives
    new_opt.c_cc[VMIN] = 1;
    new_opt.c_cc[VTIME] = 0;

    // Now clean the modem line and activate the settings for modem
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &new_opt);

    char buf[BUF_LEN] = "AT+CSQ?\r";
    int  len = strlen(buf);
    if (len != write(fd, buf, len))
    {
        CN_ERRORPRINTF("Can't write data to %s", DEVICE_NAME);
        return 0;
    }

    len = read(fd, buf, BUF_LEN);

    if (strstr(buf, "\r\nOK\r\n") == 0)
    {
        // Modem returned error
        CN_ERRORPRINTF("Modem returned error for command AT+CSQ?");
        return 0;
    }

    int rssi = 0, fer = 0;
    sscanf(buf, "\r\n+CSQ: %d, %d\r\n", &rssi, &fer);

    // Restore original settings
    tcsetattr(fd, TCSANOW, &old_opt);
    close(fd);
    return rssi;
}

