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
 * \file editScreenProfiles.h
 * \brief connectionMgr - the first page of 'edit' screen
 *                        the edit profiles screen
 *                        containing a selection for network type
 *                               and a new profile entry 
 *                               and a profile list(registry)
 *
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */

#ifndef _EDIT_PROFILES_H_
#define _EDIT_PROFILES_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>

// time-out in seconds before "listitem select" mode exits automatically
#define ITEM_SELECT_TIMEOUT        6       // seconds
#define ITEM_SELECT_TIMER_INTERVAL 500     // milliseconds
#define ITEM_SELECT_TIMEOUT_TICKS  ((ITEM_SELECT_TIMEOUT * 1000) / ITEM_SELECT_TIMER_INTERVAL)

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

    GtkWidget *edit_profiles_create(void);
    void edit_profiles_set_text(void);
    void edit_profiles_display(void);

    char *edit_profiles_default_profilename(void);

    void edit_profiles_create_profile(void);
    int edit_profiles_get_edited_profile_index(void);

    void on_edit_profiles_icon_clicked(int iconID, int iconState);

    gboolean on_edit_profiles_keypress(GdkEventKey *event); 

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_EDIT_PROFILES_H_

