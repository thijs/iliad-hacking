/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file browserLog.h
 * \brief browserLog - enable/disable messages printed to stderr.
 * 
 * <File description>
 * The value of CL_X_ON (with X one of LOGGING, WARNING or ERROR)
 * determines whether a message will be printed. The value as defined
 * in this file can be overruled by defining them before the inclusion of this file.
 *
 * This file is best included in c files only.
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef _BROWSER_LOG_H_
#define _BROWSER_LOG_H_

#define BR_LOGGING_ON     0
#define BR_WARNING_ON     1
#define BR_ERROR_ON       1
#define BR_MANIFEST_ON    0
#define BR_PAGEBAR_ON     0
#define BR_IPC_ON         0
#define BR_MOZILLA_DOC_ON 0 //parsing the XHTML DOC 
#define BR_DISPLAY_ON     0

#define SYSLOG_ON         1
#if (SYSLOG_ON)
#include <syslog.h>
#endif

#if (BR_LOGGING_ON)
#if (SYSLOG_ON)
#define BR_LOGPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(BR_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(BR_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define BR_LOGPRINTF(x, args...) fprintf(stderr, "(BR_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define BR_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (BR_WARNING_ON)
#if (SYSLOG_ON)
#define BR_WARNPRINTF(x, args...) \
{\
    syslog(LOG_WARNING, "(BR_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(BR_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define BR_WARNPRINTF(x, args...) fprintf(stderr, "(BR_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define BR_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (BR_ERROR_ON)
#if (SYSLOG_ON)
#define BR_ERRORPRINTF(x, args...) \
{\
    syslog(LOG_ERR, "(BR_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(BR_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define BR_ERRORPRINTF(x, args...) fprintf(stderr, "(BR_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define BR_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (BR_IPC_ON)
#if (SYSLOG_ON)
#define BR_IPCPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(BR_IP)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(BR_IP)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define BR_IPCPRINTF(x, args...) fprintf(stderr, "(BR_IP)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define BR_IPCPRINTF(x, args...) do {} while (0)
#endif

#if (BR_MANIFEST_ON)
#if (SYSLOG_ON)
#define BR_MANIFESTPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(BR_MA)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(BR_MA)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define BR_MANIFESTPRINTF(x, args...) fprintf(stderr, "(BR_MA)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define BR_MANIFESTPRINTF(x, args...) do {} while (0)
#endif

#if (BR_PAGEBAR_ON)
#if (SYSLOG_ON)
#define BR_PAGEBARPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(BR_PB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(BR_PB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define BR_PAGEBARPRINTF(x, args...) fprintf(stderr, "(BR_PB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define BR_PAGEBARPRINTF(x, args...) do {} while (0)
#endif

#if (BR_MOZILLA_DOC_ON)
#if (SYSLOG_ON)
#define BR_MOZILLA_DOCPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(BR_MO)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(BR_MO)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define BR_MOZILLA_DOCPRINTF(x, args...) fprintf(stderr, "(BR_MO)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define BR_MOZILLA_DOCPRINTF(x, args...) do {} while (0)
#endif

#if (BR_DISPLAY_ON)
#if (SYSLOG_ON)
#define BR_DISPLAYPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(BR_D)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(BR_D)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define BR_DISPLAYPRINTF(x, args...) fprintf(stderr, "(BR_D)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define BR_DISPLAYPRINTF(x, args...) do {} while (0)
#endif

#endif //_BROWSER_LOG_H_
