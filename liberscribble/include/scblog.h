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

#ifndef SCBLOG_H_
#define SCBLOG_H_
#include <stdio.h>
#include <glib.h>

#define SCB_TRACE_ON        0
#define SCB_DUMP_ON         0
#define SCB_WARNING_ON      0
#define SCB_ERROR_ON        1
#define SCB_ENABLE_CHECK    1
#define SCB_DEBUG_ON        0

#define SYSLOG_ON           1
#if (SYSLOG_ON)
#include <syslog.h> 
#endif

#if (SCB_TRACE_ON)
#if (SYSLOG_ON)
#define SCB_TRACE(x, args...) \
{\
    syslog(LOG_INFO, "(SCB_TRACE)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args);\
    fprintf(stderr, "(SCB_TRACE)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args);\
}
#else
#define SCB_TRACE(x, args...) fprintf(stderr, "(SCB_TRACE)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args)
#endif
#else
#define SCB_TRACE(x, args...) do {} while (0)
#endif

#if (SCB_DUMP_ON)
#if (SYSLOG_ON)
#define SCB_DUMP(x, args...) \
{\
    syslog(LOG_INFO, "(SCB_DUMP)"  x "\n", ##args);\
    fprintf(stderr, "(SCB_DUMP)"  x "\n", ##args);\
}
#else
#define SCB_DUMP(x, args...) fprintf(stderr, "(SCB_DUMP)"  x "\n", ##args)
#endif
#else
#define SCB_DUMP(x, args...) do {} while (0)
#endif


#if (SCB_WARNING_ON)
#if (SYSLOG_ON)
#define SCB_WARN(x, args...) \
{\
    syslog(LOG_WARNING, "(SCB_WARNING)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args);\
    fprintf(stderr, "(SCB_WARNING)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args);\
}
#else
#define SCB_WARN(x, args...) fprintf(stderr, "(SCB_WARNING)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args)
#endif
#else
#define SCB_WARN(x, args...) do {} while (0)
#endif

#if (SCB_ERROR_ON)
#if (SYSLOG_ON)
#define SCB_ERROR(x, args...) \
{\
    syslog(LOG_ERR, "(SCB_ERROR)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args);\
    fprintf(stderr, "(SCB_ERROR)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args);\
}
#else
#define SCB_ERROR(x, args...) fprintf(stderr, "(SCB_ERROR)" __FILE__ ":%d,%s() " x "\n", __LINE__, __FUNCTION__ , ##args)
#endif
#else
#define SCB_ERROR(x, args...) do {} while (0)
#endif

#if (SCB_ENABLE_CHECK)
#define SCB_RET_FALSE_IF(expr, x, args...) if (expr) { SCB_ERROR(x, ##args); return FALSE; }
#define SCB_RET_NULL_IF(expr, x, args...) if (expr) { SCB_ERROR(x, ##args); return NULL; }
#define SCB_RET_IF(expr, x, args...) if (expr) { SCB_ERROR(x, ##args); return; }
#define SCB_RET_INT_IF(expr, ret, x, args...) if (expr) { SCB_ERROR(x, ##args); return ret; }
#else
#define SCB_RET_BOOL_IF(expr, x, args...) do {} while(0)
#define SCB_RET_NULL_IF(expr, x, args...) do {} while(0)
#define SCB_RET_IF(expr, x, args...) do {} while(0)
#define SCB_RET_INT_IF(expr, ret, x, args...) do {} while(0)
#endif

#endif
