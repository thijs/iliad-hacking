/*
 * This file is part of scribble.
 *
 * scribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * scribble is distributed in the hope that it will be useful,
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

/**
 * \file ScribbleLog.h
 * \brief ScribbleLog - enable/disable messages printed to stderr.
 * 
 * <File description>
 * The value of SB_X_ON (with X one of LOGGING, WARNING or ERROR)
 * determines whether a message will be printed. The value as defined
 * in this file can be overruled by defining them before the inclusion of this file.
 *
 * This file is best included in c files only.
 *
 */

#ifndef _SCRIBBLELOG_H_
#define _SCRIBBLELOG_H_

#define SB_LOGGING_ON  0
#define SB_WARNING_ON  0
#define SB_ERROR_ON    1

#define SB_PAGEBAR_ON  0
#define SB_TOOLBAR_ON  0
#define SB_IPC_ON      0
//for FileManager
#define SB_FM_ON       0
//for ink file
#define SB_INK_ON      0
#define SB_INKERR_ON   1
#define SB_TIME_ON     0

#define SYSLOG_ON      1
#if (SYSLOG_ON)
#include <syslog.h>
#endif

#if (SB_LOGGING_ON)
#if (SYSLOG_ON)
#define SB_LOGPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(SB_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(SB_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define SB_LOGPRINTF(x, args...) fprintf(stderr, "(SB_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define SB_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (SB_WARNING_ON)
#if (SYSLOG_ON)
#define SB_WARNPRINTF(x, args...) \
{\
    syslog(LOG_WARNING, "(SB_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(SB_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define SB_WARNPRINTF(x, args...) fprintf(stderr, "(SB_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define SB_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (SB_ERROR_ON)
#if (SYSLOG_ON)
#define SB_ERRORPRINTF(x, args...) \
{\
    syslog(LOG_ERR, "(SB_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(SB_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define SB_ERRORPRINTF(x, args...) fprintf(stderr, "(SB_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define SB_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (SB_IPC_ON)
#if (SYSLOG_ON)
#define SB_IPCPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(SB_IP)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(SB_IP)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define SB_IPCPRINTF(x, args...) fprintf(stderr, "(SB_IP)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define SB_IPCPRINTF(x, args...) do {} while (0)
#endif

#if (SB_PAGEBAR_ON)
#if (SYSLOG_ON)
#define SB_PAGEBARPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(SB_PB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(SB_PB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define SB_PAGEBARPRINTF(x, args...) fprintf(stderr, "(SB_PB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define SB_PAGEBARPRINTF(x, args...) do {} while (0)
#endif

#if (SB_TOOLBAR_ON)
#if (SYSLOG_ON)
#define SB_TOOLBARPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(SB_TB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(SB_TB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define SB_TOOLBARPRINTF(x, args...) fprintf(stderr, "(SB_TB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define SB_TOOLBARPRINTF(x, args...) do {} while (0)
#endif

#if (SB_FM_ON)
#if (SYSLOG_ON)
#define SB_FMPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(SB_FM)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(SB_FM)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define SB_FMPRINTF(x, args...) fprintf(stderr, "(SB_FM)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define SB_FMPRINTF(x, args...) do {} while (0)
#endif

#if (SB_INK_ON)
#if (SYSLOG_ON)
#define SB_INKPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(SB_INK)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(SB_INK)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define SB_INKPRINTF(x, args...) fprintf(stderr, "(SB_INK)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define SB_INKPRINTF(x, args...) do {} while (0)
#endif

#if (SB_INKERR_ON)
#if (SYSLOG_ON)
#define SB_INKERRPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(SB_INKERR)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(SB_INKERR)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define SB_INKERRPRINTF(x, args...) fprintf(stderr, "(SB_INKERR)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define SB_INKERRPRINTF(x, args...) do {} while (0)
#endif


#if (SB_TIME_ON)
#include <sys/time.h>
#include <time.h> 
#include <glib.h>
#if (SYSLOG_ON)
#define SB_TIMEDISPLAY(x, args...) \
{\
	struct timeval now;\
	gettimeofday(&now, NULL);\
	syslog(LOG_INFO, "(SB_T)" __FILE__ ":%d,%s() " "%ld.%06ld: " x "\n", __LINE__, __func__ , now.tv_sec, now.tv_usec, ##args);\
	fprintf(stderr, "(SB_T)" __FILE__ ":%d,%s() " "%ld.%06ld: " x "\n", __LINE__, __func__ , now.tv_sec, now.tv_usec, ##args);\
}
#else
#define SB_TIMEDISPLAY(x, args...) \
{\
	struct timeval now;\
	gettimeofday(&now, NULL);\
	fprintf(stderr, "(SB_T)" __FILE__ ":%d,%s() " "%ld.%06ld: " x "\n", __LINE__, __func__ , now.tv_sec, now.tv_usec, ##args);\
}
#endif
#else
#define SB_TIMEDISPLAY(x, args...) do {} while (0)
#endif

#endif //_SCRIBBLE_LOG_H_
