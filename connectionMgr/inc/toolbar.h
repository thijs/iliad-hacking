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
 * \file toolbar.h
 * \brief connectionMgr - toolbar
 * 
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef __EREADER_TOOLBAR_H__
#define __EREADER_TOOLBAR_H__

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    // Init toolbar (open communication channel)
    void toolbar_init(void);

    void toolbar_disableUpdate();

    void toolbar_enableUpdate();

    // Change state of a toolbar icon
    void toolbar_setIconState(int iconID, int iconState);

    void toolbar_synchronise(void);

#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__EREADER_TOOLBAR_H__

