/*
 * This file is part of pcshareMgr.
 *
 * pcshareMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pcshareMgr is distributed in the hope that it will be useful,
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
 * \file downloadMgrLog.h
 * \brief downloadMgrLog - enable/disable messages printed to stderr.
 * 
 * <File description>
 * The value of DL_X_ON (with X one of LOGGING, WARNING or ERROR)
 * determines whether a message will be printed. The value as defined
 * in this file can be overruled by defining them before the inclusion of this file.
 *
 * This file is best included in c files only.
 *
 */

#ifndef _DOWNLOAD_LOG_H_
#define _DOWNLOAD_LOG_H_

#define DL_DISPLAY_ON 0
#define DL_LOGGING_ON 0
#define DL_MESSAGE_ON 1
#define DL_WARNING_ON 1
#define DL_ERROR_ON 1

#if (DL_DISPLAY_ON)
#define DL_DMPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define DL_DMPRINTF(x, args...) do {} while (0)
#endif

#if (DL_LOGGING_ON)
#define DL_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define DL_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (DL_MESSAGE_ON)
#define DL_MSGPRINTF(x, args...) fprintf(stderr, "(M)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define DL_MSGPRINTF(x, args...) do {} while (0)
#endif

#if (DL_WARNING_ON)
#define DL_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define DL_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (DL_ERROR_ON)
#define DL_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define DL_ERRORPRINTF(x, args...) do {} while (0)
#endif

#endif //_DOWNLOAD_LOG_H_
