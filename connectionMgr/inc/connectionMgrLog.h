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
 * \file connectionMgrLog.h
 * \brief connectionMgr - enable/disable messages printed to stderr.
 * 
 * <File description>
 * The value of CN_X_ON (with X one of LOGGING, WARNING, ERROR, ...)
 * determines whether a message will be printed. The value as defined
 * in this file can be overruled by defining them 
 * before the inclusion of this file.
 *
 * This file is best included in c files only.
 *
 *  Copyright (C) 2007 iRex Technologies BV.
 */

#ifndef _CONNECTION_MGR_LOG_H_
#define _CONNECTION_MGR_LOG_H_

#include <stdio.h>

#define CN_DISPLAY_ON   0
#define CN_LOGGING_ON   0
#define CN_WARNING_ON   1
#define CN_ERROR_ON     1
#define CN_SELECTION_ON 0
#define CN_LANGUAGE_ON  0
#define CN_STORE_ON     1
#define CN_IPC_ON       0

#define SYSLOG_ON       1

#if (SYSLOG_ON)
#include <syslog.h>
#endif

#if (CN_DISPLAY_ON)
#if (SYSLOG_ON)
#define CN_DMPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CN_D)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CN_D)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CN_DMPRINTF(x, args...) fprintf(stderr, "(CN_D)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CN_DMPRINTF(x, args...) do {} while (0)
#endif

#if (CN_STORE_ON)
#if (SYSLOG_ON)
#define CN_STOREPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CN_ST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CN_ST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CN_STOREPRINTF(x, args...) fprintf(stderr, "(CN_ST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CN_STOREPRINTF(x, args...) do {} while (0)
#endif

#if (CN_SELECTION_ON)
#if (SYSLOG_ON)
#define CN_SELPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CN_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CN_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CN_SELPRINTF(x, args...) fprintf(stderr, "(CN_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CN_SELPRINTF(x, args...) do {} while (0)
#endif

#if (CN_LANGUAGE_ON)
#if (SYSLOG_ON)
#define CN_LANGUAGEPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CN_LA)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CN_LA)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CN_LANGUAGEPRINTF(x, args...) fprintf(stderr, "(CN_LA)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CN_LANGUAGEPRINTF(x, args...) do {} while (0)
#endif

#if (CN_LOGGING_ON)
#if (SYSLOG_ON)
#define CN_LOGPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CN_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CN_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CN_LOGPRINTF(x, args...) fprintf(stderr, "(CN_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CN_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (CN_WARNING_ON)
#if (SYSLOG_ON)
#define CN_WARNPRINTF(x, args...) \
{\
    syslog(LOG_WARNING, "(CN_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CN_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CN_WARNPRINTF(x, args...) fprintf(stderr, "(CN_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CN_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (CN_ERROR_ON)
#if (SYSLOG_ON)
#define CN_ERRORPRINTF(x, args...) \
{\
    syslog(LOG_ERR, "(CN_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CN_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CN_ERRORPRINTF(x, args...) fprintf(stderr, "(CN_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CN_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (CN_IPC_ON)
#if (SYSLOG_ON)
#define CN_IPCPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CN_I)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CN_I)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CN_IPCPRINTF(x, args...) fprintf(stderr, "(CN_I)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CN_IPCPRINTF(x, args...) do {} while (0)
#endif

#endif //_CONNECTION_MGR_LOG_H_

