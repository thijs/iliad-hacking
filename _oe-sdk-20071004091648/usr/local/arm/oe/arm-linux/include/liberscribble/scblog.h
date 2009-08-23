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
