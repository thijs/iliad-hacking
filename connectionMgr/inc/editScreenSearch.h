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
 * \file editScreenSearch.h
 * \brief connectionMgr - edit "search and select" wireless screen
 *
 * Copyright (C) 2007 iRex Technologies BV.
 * 
 */

#ifndef _EDIT_SCREEN_SEARCH_H_
#define _EDIT_SCREEN_SEARCH_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>

#ifdef __cplusplus
extern "C"
{
#endif

    GtkWidget *edit_search_create(void);
    void edit_search_set_text(void);

    void edit_search_restore_screen(void);

    gboolean on_edit_search_keypress(GdkEventKey *event); 

#ifdef __cplusplus
}
#endif

#endif //_EDIT_SCREEN_SEARCH_H_


