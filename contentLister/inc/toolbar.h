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
 * \file toolbar.h
 * \brief contentLister - toolbar interaction with contentLister application
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __EREADER_TOOLBAR_H__
#define __EREADER_TOOLBAR_H__

#ifdef __cplusplus
extern  "C" {
#endif  /* __cplusplus */


//
// Init toolbar (open communication channel and enable/set the correct icons)
void toolbar_init(void);

// Restore toolbar (restore CTL icons after application switch)
void toolbar_restore();

// Clear all toolbar icons
void toolbar_clear(void);

// Freeze toolbar layout
void toolbar_disableUpdate(void);

// Redraw toolbar now
void toolbar_enableUpdate(void);

// Change state of a toolbar icon
void toolbar_setIconState(int iconID, int iconState);

// Synchronise with toolbar application
void toolbar_synchronise(void);

// Request state of a toolbar icon
gboolean toolbar_is_trashcan_selected(void);
gboolean toolbar_is_locked_selected(void);
gboolean toolbar_is_tagging_selected(void);
gboolean toolbar_is_search_selected(void);
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__EREADER_TOOLBAR_H__




