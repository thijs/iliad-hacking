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
 * \file eripclog.h
 * \brief liberipc - enable/disable messages printed to stderr.
 *
 * The value of ERIPC_X_ON (with X one of LOGGING, WARNING or ERROR)
 * determines whether a message will be printed. The value as defined
 * in this file can be overruled by defining them before the inclusion of this file.
 *
 * This file is best included in c files only.
 *
 */

/* liberipc - A library to enable IPC on the eReader platform
 * Copyright (C) 2005 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#ifndef _ERIPCLOG_H_
#define _ERIPCLOG_H_

#define ERIPC_LOGGING_ON 0
#define ERIPC_WARNING_ON 1
#define ERIPC_ERROR_ON   1

#define SYSLOG_ON        1
#if (SYSLOG_ON)
#include <syslog.h>
#endif

#if (ERIPC_LOGGING_ON)
#if (SYSLOG_ON)
#define ERIPC_LOGPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ERIPC_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ERIPC_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERIPC_WARNING_ON)
#if (SYSLOG_ON)
#define ERIPC_WARNPRINTF(x, args...) \
{\
    syslog(LOG_WARNING, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ERIPC_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ERIPC_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERIPC_ERROR_ON)
#if (SYSLOG_ON)
#define ERIPC_ERRORPRINTF(x, args...) \
{\
    syslog(LOG_ERR, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ERIPC_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ERIPC_ERRORPRINTF(x, args...) do {} while (0)
#endif

#endif
