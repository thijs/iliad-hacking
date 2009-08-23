/*
 * This file is part of settings.
 *
 * settings is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * settings is distributed in the hope that it will be useful,
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
 * \file setupLog.h
 * \brief setupLog - enable/disable messages printed to stderr.
 * 
 * <File description>
 * The value of ST_X_ON (with X one of LOGGING, WARNING or ERROR)
 * determines whether a message will be printed. The value as defined
 * in this file can be overruled by defining them before the inclusion of this file.
 *
 * This file is best included in c files only.
 *
 */

#ifndef _SETTINGS_APP_LOG_H_
#define _SETTINGS_APP_LOG_H_

#define ST_LOGGING_ON   0
#define ST_WARNING_ON   1
#define ST_ERROR_ON     1
#define ST_SELECTION_ON 0
#define ST_LANGUAGE_ON  0
#define ST_STORE_ON     1
#define ST_IPC_ON       0
#define ST_SCREEN_ON    0

#define SYSLOG_ON       1
#if (SYSLOG_ON)
#include <syslog.h>
#endif

#if (ST_STORE_ON)
#if (SYSLOG_ON)
#define ST_STOREPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(S_ST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(S_ST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ST_STOREPRINTF(x, args...) fprintf(stderr, "(S_ST)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ST_STOREPRINTF(x, args...) do {} while (0)
#endif

#if (ST_SELECTION_ON)
#if (SYSLOG_ON)
#define ST_SELPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(S_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(S_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ST_SELPRINTF(x, args...) fprintf(stderr, "(S_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ST_SELPRINTF(x, args...) do {} while (0)
#endif

#if (ST_LANGUAGE_ON)
#if (SYSLOG_ON)
#define ST_LANGUAGEPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(S_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(S_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ST_LANGUAGEPRINTF(x, args...) fprintf(stderr, "(S_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ST_LANGUAGEPRINTF(x, args...) do {} while (0)
#endif

#if (ST_LOGGING_ON)
#if (SYSLOG_ON)
#define ST_LOGPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(S_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(S_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ST_LOGPRINTF(x, args...) fprintf(stderr, "(S_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ST_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ST_WARNING_ON)
#if (SYSLOG_ON)
#define ST_WARNPRINTF(x, args...) \
{\
    syslog(LOG_WARNING, "(S_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(S_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ST_WARNPRINTF(x, args...) fprintf(stderr, "(S_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ST_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ST_ERROR_ON)
#if (SYSLOG_ON)
#define ST_ERRORPRINTF(x, args...) \
{\
    syslog(LOG_ERR, "(S_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(S_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ST_ERRORPRINTF(x, args...) fprintf(stderr, "(S_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ST_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (ST_IPC_ON)
#if (SYSLOG_ON)
#define ST_IPCPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(S_I)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(S_I)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define ST_IPCPRINTF(x, args...) fprintf(stderr, "(S_I)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define ST_IPCPRINTF(x, args...) do {} while (0)
#endif

#endif //_SETTINGS_APP_LOG_H_
