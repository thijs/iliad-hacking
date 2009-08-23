/*
 * This file is part of liberipc.
 *
 * liberipc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberipc is distributed in the hope that it will be useful,
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

#ifndef _ERIPCPRIVATE_H_
#define _ERIPCPRIVATE_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "eripc.h"

#ifdef __cplusplus
extern  "C"
{
#endif                          /* __cplusplus */

typedef struct erInternalClientChannel_t
{
    int     sockfd;
    struct sockaddr_in serverAdr;
} erInternalClientChannel_t;

typedef struct erInternalServerChannel_t
{
    int     sockfd;
} erInternalServerChannel_t;

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif
