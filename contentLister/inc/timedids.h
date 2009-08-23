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
 * \file timedids.h
 * \brief control - handle timed-iDS connection
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2007-2008 iRex Technologies B.V.
 *
 */
#ifndef __TIMED_IDS__H__
#define __TIMED_IDS__H__

#define TZ_FILE "/etc/localtime"

typedef enum
{
    connect_after_wakeup,
    connect_when_running,
    connect_undefined
}timedids_connect_reason;

// Header of timezone files, you can find them in /usr/share/zoneinfo
struct tzhead
{
    char tzh_magic[4];       /* TZ_MAGIC */
    char tzh_reserved[16];   /* reserved for future use */
    char tzh_ttisgmtcnt[4];  /* coded number of trans. time flags */
    char tzh_ttisstdcnt[4];  /* coded number of trans. time flags */
    char tzh_leapcnt[4];     /* coded number of leap seconds */
    char tzh_timecnt[4];     /* coded number of transition times */
    char tzh_typecnt[4];     /* coded number of local time types */
    char tzh_charcnt[4];     /* coded number of abbr. chars */
};

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

void timed_ids_init();
void timed_ids_final();

gboolean on_timedids_connect_after_wakeup(gpointer p);

void timed_ids_handle_connmgr_quit();
void timed_ids_handle_reread_settings();
void timed_ids_handle_recreate_timer(gint interval);
timedids_connect_reason get_timedids_connect_reason();
void set_timedids_connect_reason(timedids_connect_reason connReason);

#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__TIMED_IDS__H__
