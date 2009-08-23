/*
 * This file is part of liberdm.
 *
 * liberdm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberdm is distributed in the hope that it will be useful,
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
 * The value of ERDM_X_ON (with X one of LOGGING, WARNING or ERROR)
 * determines whether a message will be printed. The value as defined
 * in this file can be overruled by defining them before the inclusion of this file.
 *
 * This file is best included in c files only.
 *
 */

/* liberdm - A library to control the ePaper display of the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#ifndef _ERDMLOG_H_
#define _ERDMLOG_H_

#define ERDM_LOGGING_ON 0
#define ERDM_WARNING_ON 1
#define ERDM_ERROR_ON   1

#define SYSLOG_ON       1
#if (SYSLOG_ON)
#include <syslog.h>
#endif

#if (ERDM_LOGGING_ON)
#if (SYSLOG_ON)
#define ERDM_LOGPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ERDM_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ERDM_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERDM_WARNING_ON)
#if (SYSLOG_ON)
#define ERDM_WARNPRINTF(x, args...) \
{\
    syslog(LOG_WARNING, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ERDM_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ERDM_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERDM_ERROR_ON)
#if (SYSLOG_ON)
#define ERDM_ERRORPRINTF(x, args...) \
{\
    syslog(LOG_ERR, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ERDM_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ERDM_ERRORPRINTF(x, args...) do {} while (0)
#endif

#endif
