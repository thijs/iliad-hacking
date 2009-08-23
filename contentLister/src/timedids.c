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
 * \file timedids.c
 * \brief lister - Handle timed-iDS connection
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2007-2008 iRex Technologies
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <fcntl.h>
#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "erConnect.h"
#include "control.h"
#include "button.h"
#include "displayUpdate.h"
#include "timedids.h"

#define  SECONDS_OF_DAY 86400

// Global varibles
static regTimedIds_t *g_timedids_settings = NULL;
static timedids_connect_reason g_connect_reason = connect_undefined;
static guint g_timeout_source_id = 0;
static int g_current_tzoffset = 0;

// Internal used functions
static void handle_wakeup();
static void handle_normal_startup();
static regTimedIds_t* read_timedids_settings();
static void create_timedids_trigger();
static int get_current_tz_offset();

regTimedIds_t* read_timedids_settings()
{
    CL_LOGPRINTF("entry");

    regTimedIds_t* theTimedIds = NULL;

    theTimedIds = erRegGetTimedIds();
    if (theTimedIds == NULL)
    {
        CL_ERRORPRINTF("erRegGetTimedIds returns NULL");
    }

    return theTimedIds;
}

void timed_ids_init()
{
    CL_LOGPRINTF("entry");
    
    // Check if we are waken up by RTC alarm interrupt or not
    // Get settings from registry
    if ((g_timedids_settings = read_timedids_settings()) == NULL)
    {
        CL_ERRORPRINTF("Can NOT read timed-iDS connection settings");
        return;
    }

    // Get current timezone offset
    g_current_tzoffset = get_current_tz_offset();

    if (!g_timedids_settings->enable)
    {
        // Nothing to do
        return;
    }

    time_t utc_second = time(NULL);
    struct tm* tm_local =  localtime(&utc_second);
    int local_second = tm_local->tm_hour * 3600 + tm_local->tm_min * 60 + tm_local->tm_sec;
    
    gboolean wakeup_by_alarm = FALSE;
    int i;
    for (i=0; i<g_timedids_settings->timeCnt; i++)
    {
        // If current time is <90 seconds later than 1 alarm time,
        // then we think we are being waken up by alarm interrupt
        if (local_second > g_timedids_settings->timeSet[i] &&
            local_second - g_timedids_settings->timeSet[i] <= 90)
        {
            wakeup_by_alarm = TRUE;
            break;
        }
    }

    if (wakeup_by_alarm)
    {
        // The iLiad is waken up by RTC alarm interrupt
        handle_wakeup();
    }
    else
    {
        // Normal startup
        handle_normal_startup();
    }
}

void handle_wakeup()
{
    CL_LOGPRINTF("entry");

    // Block keys if iLiad is waken up by RTC alarm
    button_block_all_keys(TRUE);

    g_connect_reason = connect_after_wakeup;
}

gboolean on_timedids_connect_after_wakeup(gpointer p)
{
    CL_LOGPRINTF("entry");

    // Launch connectionMgr to download contents (include software updates if needed)
    ctrl_disconnect_wait();
    if (g_timedids_settings->swUpdate)
    {
        ctrl_connect(connectScrDownloadHistory, connect_timed_ids_with_sw);
    }
    else
    {
        ctrl_connect(connectScrDownloadHistory, connect_timed_ids_content_only);
    }

    display_update_decrease_level(LISTER_EXPOSE_LEVEL);

    return FALSE;
}

void handle_normal_startup()
{
    CL_LOGPRINTF("entry");
    
    int rtc, ret;

    // Open device
    rtc = open ("/dev/rtc", O_RDWR);
    if (rtc == -1)
    {
        CL_ERRORPRINTF("Can NOT open /dev/rtc");
        return;
    }
    
    // First, disable RTC alarm interrupt
    ret = ioctl(rtc, RTC_AIE_OFF, 0);
    if (ret == -1)
    {
        CL_ERRORPRINTF("Can NOT disable RTC alarm interrupt");
        return;
    }

    close(rtc);

    // Second, create a time-out event instead
    create_timedids_trigger();
}

void create_timedids_trigger()
{
    time_t utc_second = time(NULL);
    int local_second = (utc_second + g_current_tzoffset) % SECONDS_OF_DAY;

    int i, time_left = 0;
    for (i=0; i<g_timedids_settings->timeCnt; i++)
    {
        if (g_timedids_settings->timeSet[i] > local_second)
        {
            time_left = g_timedids_settings->timeSet[i] - local_second;
            break;
        }
    }

    if (i == g_timedids_settings->timeCnt)
    {
        time_left = SECONDS_OF_DAY + g_timedids_settings->timeSet[0] - local_second;
    }

    g_timeout_source_id = g_timeout_add(time_left*1000,
            on_idle_connect_start_background, (gpointer)connect_timed_ids_background);
}

void timed_ids_handle_recreate_timer(gint interval)
{
    if (g_timeout_source_id > 0)
    {
        g_source_remove(g_timeout_source_id);
        g_timeout_source_id = 0;
    }

    g_timeout_source_id = g_timeout_add(interval * 60 * 1000,
            on_idle_connect_start_background, (gpointer)connect_timed_ids_background);
}

void timed_ids_handle_connmgr_quit()
{
    CL_LOGPRINTF("entry");
    switch (g_connect_reason)
    {
        case connect_after_wakeup:
            if (g_timedids_settings->switchOff)
            {
                // Shut down iLiad, and also create RTC alarm for the next trigger point
                ctrl_shutdown();
            }
            else
            {
                create_timedids_trigger();

                button_block_all_keys(FALSE);
            }
            break;
        case connect_when_running:
            // Create time-out event for the next trigger point
            create_timedids_trigger();
            break;
        default:
            break;
    }
}

void timed_ids_handle_reread_settings()
{
    CL_LOGPRINTF("entry");

    // Re-read timed-iDS connection settings
    regTimedIds_t* stored_timedids_settings = NULL;
    if ((stored_timedids_settings = read_timedids_settings()) == NULL)
    {
        CL_ERRORPRINTF("Can NOT read timed-iDS connection settings");
        return;
    }

    int new_tzoffset = get_current_tz_offset();
    gboolean settings_changed = FALSE;

    if (memcmp(stored_timedids_settings, g_timedids_settings, sizeof(regTimedIds_t)) != 0)
    {
        // User changed timed-iDS connection settings
        *g_timedids_settings = *stored_timedids_settings;
        settings_changed = TRUE;
    }
    if (new_tzoffset != g_current_tzoffset)
    {
        // User changed timezone
        g_current_tzoffset = new_tzoffset;
        settings_changed = TRUE;
    }

    if (settings_changed)
    {
        // First, remove current time-out trigger
        if (g_timeout_source_id > 0)
        {
            g_source_remove(g_timeout_source_id);
            g_timeout_source_id = 0;
        }

        // Second, re-create time-out trigger if timed-iDS enabled
        if (g_timedids_settings->enable)
        {
            create_timedids_trigger();
        }
    }

    erRegFreeTimedIds(stored_timedids_settings);    
}

void timed_ids_final()
{
    CL_LOGPRINTF("entry");
 
    if (g_timedids_settings->enable)
    {
        // Re-create RTC alarm interrupt
        time_t utc_second = time(NULL);
        int local_second = (utc_second + g_current_tzoffset) % SECONDS_OF_DAY;

        int i;
        for(i=0; i<g_timedids_settings->timeCnt; i++)
        {
            if (g_timedids_settings->timeSet[i] > local_second)
            {
                break;
            }
        }

        if (i == g_timedids_settings->timeCnt)
        {
            i = 0;
        }
        
        int utc_alarm = g_timedids_settings->timeSet[i] - g_current_tzoffset;
        if (utc_alarm < 0)
        {
            utc_alarm += SECONDS_OF_DAY;
        }
        else if (utc_alarm > SECONDS_OF_DAY)
        {
            utc_alarm -= SECONDS_OF_DAY;
        }

        struct rtc_time rtc_tm;
        memset(&rtc_tm, 0, sizeof(struct rtc_time));
        rtc_tm.tm_hour = utc_alarm / 3600;
        rtc_tm.tm_min  = utc_alarm / 60 % 60;
        rtc_tm.tm_sec  = utc_alarm % 60;
       
        // Open device
        int rtc, ret;
        rtc = open ("/dev/rtc", O_RDWR);
        if (rtc == -1)
        {
            CL_ERRORPRINTF("Can NOT open /dev/rtc");
            return;
        }

        // Re-set RTC alarm to the next timeset in registry
        ret = ioctl(rtc, RTC_ALM_SET, &rtc_tm);
        if (ret == -1)
        {
            CL_ERRORPRINTF("Can NOT set RTC alarm");
            return;
        }

        CL_WARNPRINTF("RTC Alarm is set to %02d:%02d:%02d\n", rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
        
        // Re-enable RTC alarm interrupt
        ret = ioctl(rtc, RTC_AIE_ON, 0);
        if (ret == -1)
        {
            CL_ERRORPRINTF("Can NOT enable RTC alarm interrupt");
            return;
        }

        close(rtc);
    }
    
    erRegFreeTimedIds(g_timedids_settings);
}

int get_current_tz_offset()
{
    FILE* fp = fopen(TZ_FILE, "rb");

    if (NULL == fp)
    {
        CL_ERRORPRINTF("Can't open %s, use default timezone settings.", TZ_FILE);
        return 0;
    }

    fseek(fp, sizeof(struct tzhead), SEEK_SET);
    
    int offset;
    if (fread(&offset, sizeof(int), 1, fp) != 1)
    {
        CL_ERRORPRINTF("Error reading %s, use default timezone settings.", TZ_FILE);
        fclose(fp);
        return 0;
    }
    
    fclose(fp);
    
    return GINT32_FROM_BE(offset);
}

timedids_connect_reason get_timedids_connect_reason()
{
    return g_connect_reason;
}

void set_timedids_connect_reason(timedids_connect_reason connReason)
{
    g_connect_reason = connReason;
}

