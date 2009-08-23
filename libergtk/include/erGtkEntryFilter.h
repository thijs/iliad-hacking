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
 * \file erGtkEntryFilter.h
 * \brief ereader gtk library - filters for erGtkEntry object
 *                              ipv4 filter and integer filter
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 */

#ifndef __ERGTK_ENTRY_FILTER_H__
#define __ERGTK_ENTRY_FILTER_H__

#include <gtk/gtk.h>
#include <glib.h>

typedef enum
{
    ipv4Complete_e = 0,
    ipv4Incomplete_e,
    ipv4Error_e
}ipAddress_t;


#ifdef __cplusplus
extern "C"
{
#endif

    ipAddress_t ipv4_filter_check_address (const gchar *text);
    void ipv4_filter_init(GtkEntry *entry);
    void integer_filter_init(GtkEntry *entry);

#ifdef __cplusplus
}
#endif

#endif                          /* __ERGTK_ENTRY_FILTER_H__ */
