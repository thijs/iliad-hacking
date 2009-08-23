/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/** 
 * \file contentListerLog.h
 * \brief contentListerLog - enable/disable messages printed to stderr.
 * 
 * <File description>
 * The value of CL_X_ON (with X one of LOGGING, WARNING or ERROR)
 * determines whether a message will be printed. The value as defined
 * in this file can be overruled by defining them before the inclusion of this file.
 *
 * This file is best included in c files only.
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef _CONTENT_LISTER_LOG_H_
#define _CONTENT_LISTER_LOG_H_

#define CL_LOGGING_ON       0
#define CL_WARNING_ON       1
#define CL_ERROR_ON         1
#define CL_CONNECT_DEBUG_ON 0
#define CL_LISTER_DEBUG_ON  0
#define CL_CONTROL_DEBUG_ON 0
#define CL_SCREEN_DEBUG_ON  0
#define CL_HISTORY_DEBUG_ON 0
#define CL_IPC_DEBUG_ON     0
#define CL_TIME_ON          0
#define CL_CURSOR_ON        0
#define CL_STACK_ON         0
#define CL_PM_ON            0
#define CL_TOOLBAR_ON       0
#define CL_SEARCH_ON        0

#define SYSLOG_ON           1

#if (SYSLOG_ON)
#include <syslog.h>
#endif


#if (CL_LOGGING_ON)
#if (SYSLOG_ON)
#define CL_LOGPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CL_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_LOGPRINTF(x, args...) fprintf(stderr, "(CL_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);
#endif
#else
#define CL_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (CL_WARNING_ON)
#if (SYSLOG_ON)
#define CL_WARNPRINTF(x, args...) \
{\
    syslog(LOG_WARNING, "(CL_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_WARNPRINTF(x, args...) fprintf(stderr, "(CL_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);
#endif
#else
#define CL_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (CL_ERROR_ON)
#if (SYSLOG_ON)
#define CL_ERRORPRINTF(x, args...) \
{\
    syslog(LOG_ERR, "(CL_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_ERRORPRINTF(x, args...) fprintf(stderr, "(CL_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);
#endif
#else
#define CL_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (CL_CONNECT_DEBUG_ON)
#if (SYSLOG_ON)
#define CL_CTPRINTF(x, args...) \
{\
    syslog(LOG_DEBUG, "(CL_CT)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_CT)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_CTPRINTF(x, args...) fprintf(stderr, "(CL_CT)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);
#endif
#else
#define CL_CTPRINTF(x, args...) do {} while (0)
#endif

#if (CL_LISTER_DEBUG_ON)
#if (SYSLOG_ON)
#define CL_LISTERPRINTF(x, args...) \
{\
    syslog(LOG_DEBUG, "(CL_LI)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_LI)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_LISTERPRINTF(x, args...) fprintf(stderr, "(CL_LI)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CL_LISTERPRINTF(x, args...) do {} while (0)
#endif

#if (CL_CONTROL_DEBUG_ON) 
#if (SYSLOG_ON)
#define CL_CONTROLPRINTF(x, args...) \
{\
    syslog(LOG_DEBUG, "(CL_C)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_C)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_CONTROLPRINTF(x, args...) fprintf(stderr, "(CL_C)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CL_CONTROLPRINTF(x, args...) do {} while (0)
#endif

#if (CL_SCREEN_DEBUG_ON)
#if (SYSLOG_ON)
#define CL_SCREENPRINTF(x, args...) \
{\
    syslog(LOG_DEBUG, "(CL_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_SCREENPRINTF(x, args...) fprintf(stderr, "(CL_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CL_SCREENPRINTF(x, args...) do {} while (0)
#endif

#if (CL_HISTORY_DEBUG_ON)
#if (SYSLOG_ON)
#define CL_HISTORYPRINTF(x, args...) \
{\
    syslog(LOG_DEBUG, "(CL_H)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_H)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_HISTORYPRINTF(x, args...) fprintf(stderr, "(CL_H)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CL_HISTORYPRINTF(x, args...) do {} while (0)
#endif

#if (CL_IPC_DEBUG_ON)
#if (SYSLOG_ON)
#define CL_IPCPRINTF(x, args...) \
{\
    syslog(LOG_DEBUG, "(CL_IPC)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_IPC)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_IPCPRINTF(x, args...) fprintf(stderr, "(CL_IPC)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CL_IPCPRINTF(x, args...) do {} while (0)
#endif

#if (CL_CURSOR_ON)
#if (SYSLOG_ON)
#define CL_CURSORPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CL_CU)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_CU)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_CURSORPRINTF(x, args...) fprintf(stderr, "(CL_CU)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CL_CURSORPRINTF(x, args...) do {} while (0)
#endif

#if (CL_STACK_ON)
#if (SYSLOG_ON)
#define CL_STACKPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CL_ST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_ST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_STACKPRINTF(x, args...) fprintf(stderr, "(CL_ST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CL_STACKPRINTF(x, args...) do {} while (0)
#endif

#if (CL_PM_ON)
#if (SYSLOG_ON)
#define CL_PMPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CL_PM)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_PM)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_PMPRINTF(x, args...) fprintf(stderr, "(CL_PM)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CL_PMPRINTF(x, args...) do {} while (0)
#endif

#if (CL_TOOLBAR_ON)
#if (SYSLOG_ON)
#define CL_TBPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CL_TB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_TB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_TBPRINTF(x, args...) fprintf(stderr, "(CL_TB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CL_TBPRINTF(x, args...) do {} while (0)
#endif

#if (CL_SEARCH_ON)
#if (SYSLOG_ON)
#define CL_SEARCHPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(CL_SC)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(CL_SC)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define CL_SEARCHPRINTF(x, args...) fprintf(stderr, "(CL_SC)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define CL_SEARCHPRINTF(x, args...) do {} while (0)
#endif

#if (CL_TIME_ON)
#include <sys/time.h>
#include <time.h> 
#include <glib.h>
#if (SYSLOG_ON)
#define CL_TIMEDISPLAY(x, args...) \
{\
	struct timeval now;\
	gettimeofday(&now, NULL);\
	syslog(LOG_INFO, "(CL_T)" __FILE__ ":%d,%s() " "%ld.%06ld: " x "\n", __LINE__, __func__ , now.tv_sec, now.tv_usec, ##args);\
	fprintf(stderr, "(CL_T)" __FILE__ ":%d,%s() " "%ld.%06ld: " x "\n", __LINE__, __func__ , now.tv_sec, now.tv_usec, ##args);\
}
#else
#define CL_TIMEDISPLAY(x, args...) \
{\
	struct timeval now;\
	gettimeofday(&now, NULL);\
	fprintf(stderr, "(CL_T)" __FILE__ ":%d,%s() " "%ld.%06ld: " x "\n", __LINE__, __func__ , now.tv_sec, now.tv_usec, ##args);\
}
#endif
#else
#define CL_TIMEDISPLAY(x, args...) do {} while (0)
#endif

#endif //_CONTENT_LISTER_LOG_H_
