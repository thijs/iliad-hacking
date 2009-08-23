/**
 * \file erreglog.h
 * \brief liberreg - enable/disable messages printed to stderr.
 *
 * The value of ERREGLOG_X_ON (with X one of LOGGING, WARNING or ERROR etc)
 * determines whether a message will be printed. The value as defined
 * in this file can be overruled by defining them before the inclusion of this file.
 *
 * This file is best included in c files only.
 *
 */

/* 
 * Copyright (C) 2005 Philips Apptech
 *
 */

#ifndef _ER_REGISTRY_LOG_H_
#define _ER_REGISTRY_LOG_H_

// TODO  add syslog as well
//
#define ERREG_BASIS_LOG_ON           0
#define ERREG_BASIS_WARN_ON          1
#define ERREG_BASIS_ERROR_ON         1

#define ERREG_NWPROFILES_LOG_ON      0
#define ERREG_NWPROFILES_WARN_ON     1
#define ERREG_NWPROFILES_ERROR_ON    1

#define ERREG_TOOLBAR_LOG_ON         0
#define ERREG_TOOLBAR_WARN_ON        1
#define ERREG_TOOLBAR_ERROR_ON       1

#define ERREG_LOGGING_ON             0
#define ERREG_WARNING_ON             1
#define ERREG_ERROR_ON               1

#define ERREG_XSD_LOG_ON             0
#define ERREG_XSD_WARN_ON            1
#define ERREG_XSD_ERROR_ON           1

#define ERREG_CFG_LOG_ON             0
#define ERREG_CFG_WARN_ON            1  
#define ERREG_CFG_ERROR_ON           1 

#define ERREG_RWLOCK_LOG_ON          0
#define ERREG_RWLOCK_WARN_ON         1
#define ERREG_RWLOCK_ERROR_ON        1        

#define ERREG_DUMP_ON                0

#define ERREG_TIME_ON                0

#if (ERREG_BASIS_LOG_ON)
#define ERREG_BASIS_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_BASIS_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_BASIS_WARN_ON)
#define ERREG_BASIS_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_BASIS_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_BASIS_ERROR_ON)
#define ERREG_BASIS_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_BASIS_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_TOOLBAR_LOG_ON)
#define ERREG_TOOLBAR_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_TOOLBAR_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_TOOLBAR_WARN_ON)
#define ERREG_TOOLBAR_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_TOOLBAR_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_TOOLBAR_ERROR_ON)
#define ERREG_TOOLBAR_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_TOOLBAR_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_NWPROFILES_LOG_ON)
#define ERREG_NWPROFILES_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_NWPROFILES_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_NWPROFILES_WARN_ON)
#define ERREG_NWPROFILES_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_NWPROFILES_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_NWPROFILES_ERROR_ON)
#define ERREG_NWPROFILES_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_NWPROFILES_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_LOGGING_ON)
#define ERREG_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_WARNING_ON)
#define ERREG_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_ERROR_ON)
#define ERREG_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_XSD_LOG_ON)
#define ERREG_XSD_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_XSD_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_XSD_WARN_ON)
#define ERREG_XSD_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_XSD_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_XSD_ERROR_ON)
#define ERREG_XSD_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_XSD_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_CFG_LOG_ON)
#define ERREG_CFG_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_CFG_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_CFG_WARN_ON)
#define ERREG_CFG_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_CFG_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_CFG_ERROR_ON)
#define ERREG_CFG_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_CFG_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_RWLOCK_LOG_ON)
#define ERREG_RWLOCK_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_RWLOCK_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_RWLOCK_WARN_ON)
#define ERREG_RWLOCK_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_RWLOCK_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_RWLOCK_ERROR_ON)
#define ERREG_RWLOCK_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERREG_RWLOCK_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_DUMP_ON)
#define ERREG_DUMPPRINTF(x, args...) fprintf(stderr, x "\n", ##args)
#else
#define ERREG_DUMPPRINTF(x, args...) do {} while (0)
#endif

#if (ERREG_TIME_ON)
#include <sys/time.h>
#include <time.h> 
#include <glib.h>
#define ERREG_TIMEDISPLAY(x, args...) \
{\
    struct timeval now;\
    gettimeofday(&now, NULL);\
    fprintf(stderr, "(L)" __FILE__ ":%d,%s() " "%ld.%06ld: " x "\n", __LINE__, __func__ , now.tv_sec, now.tv_usec, ##args);\
}
#else
#define ERREG_TIMEDISPLAY(x, args...) do {} while (0)
#endif

#endif //_ER_REGISTRY_LOG_H_
