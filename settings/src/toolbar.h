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
 * \file toolbar.h
 * \brief setup application - E-reader settings application screen creation and handling                
 *
 */

#ifndef __EREADER_TOOLBAR_H__
#define __EREADER_TOOLBAR_H__

//
// Init toolbar (open communication channel)
void toolbar_init(void);

// Change state of a toolbar icon
void toolbar_setIconState(int iconID, int iconState);

#endif //__EREADER_TOOLBAR_H__
