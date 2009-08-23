/*
 * This file is part of libergtk.
 *
 * libergtk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libergtk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file ergtklog.h
 * \brief libergtk - ereader gtk library
 *
 * The value of ERREGLOG_X_ON (with X one of LOGGING, WARNING or ERROR)
 * determines whether a message will be printed. The value as defined
 * in this file can be overruled by defining them before the inclusion of this file.
 *
 * This file is best included in c files only.
 *
 */

/* 
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 *
 */

#ifndef _ER_GTK_LOG_H_
#define _ER_GTK_LOG_H_

#define LOGGING_ON 0
#define WARNING_ON 1
#define ERROR_ON 1

#if (LOGGING_ON)
#define LOGPRINTF(x, args...) fprintf(stderr, "(LG_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define LOGPRINTF(x, args...) do {} while (0)
#endif

#if (WARNING_ON)
#define WARNPRINTF(x, args...) fprintf(stderr, "(LG_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERROR_ON)
#define ERRORPRINTF(x, args...) fprintf(stderr, "(LG_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERRORPRINTF(x, args...) do {} while (0)
#endif

#endif //_ER_GTK_LOG_H_
