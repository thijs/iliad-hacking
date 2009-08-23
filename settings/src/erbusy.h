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

#ifndef __EREADER_ERBUSY_H__
#define __EREADER_ERBUSY_H__

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

/**
 * Open communication channel with erbusy deamon
 * counts on the fact that erbusy is running
 *
 * @returns void
 */
void erbusy_init();

/**
 * request from contentlister
 *
 * @returns -
 */
void erbusy_off(void);
void erbusy_blink(void);
void erbusy_on(void);
void erbusy_shutdown(void);

#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__EREADER_ERBUSY_H__




