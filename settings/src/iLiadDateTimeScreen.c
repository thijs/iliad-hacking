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
 * \file iLiadDateTimeScreen.c
 * \brief settings - iLiad date time screen
 *
 * User interface for user to set iLiad date and time
 *
 */

#include <time.h>
#include <unistd.h>
#include "iLiadDateTimeScreen.h"
#include <libergtk/ergtk.h>
#include "gtkInfoItem.h"
#include "setupLog.h"
#include "gtkSettingItem.h"
#include "background.h"
#include "languages.h"

// Global variables
static gtk_datetime_screen* g_datetime_screen = NULL;
static const tz_info timezones[TOTAL_TIMEZONES] =
{
    {-39600,       "GMT+11:00"},
    {-36000,       "GMT+10:00"},
    {-32400,       "GMT+09:00"},
    {-28800,       "GMT+08:00"},
    {-25200,       "GMT+07:00"},
    {-21600,       "GMT+06:00"},
    {-18000,       "GMT+05:00"},
    {-14400,       "GMT+04:00"},
    {-12600,       "GMT+03:30"},
    {-10800,       "GMT+03:00"},
    { -9000,       "GMT+02:30"},
    { -7200,       "GMT+02:00"},
    { -3600,       "GMT+01:00"},
    {     0,       "GMT+00:00"},
    {  3600,       "GMT-01:00"},
    {  7200,       "GMT-02:00"},
    { 10800,       "GMT-03:00"},
    { 12600,       "GMT-03:30"},
    { 14400,       "GMT-04:00"},
    { 16200,       "GMT-04:30"},
    { 18000,       "GMT-05:00"},
    { 19800,       "GMT-05:30"},
    { 20700,       "GMT-05:45"},
    { 21600,       "GMT-06:00"},
    { 23400,       "GMT-06:30"},
    { 25200,       "GMT-07:00"},
    { 28800,       "GMT-08:00"},
    { 32400,       "GMT-09:00"},
    { 34200,       "GMT-09:30"},
    { 36000,       "GMT-10:00"},
    { 37800,       "GMT-10:30"},
    { 39600,       "GMT-11:00"},
    { 43200,       "GMT-12:00"},
    { 46800,       "GMT-13:00"}
};

static time_t g_utc_time = 0;
static int g_date_boundary = 0;
static int g_current_tz_offset = -1;   /* -1 indicates an invalid value */

// Internal used functions
static GtkWidget *create_current_time_display(void);
static GtkWidget *create_timezone_selection(void);
static void iLiad_update_datetime(int tzoffset);
static int get_current_tz_offset();
static gboolean set_tz_offset(const char* tzfile);
static void on_selection_update(erGtkSelectionGroup * selection, gpointer button, gpointer data);

// widget hierarchy
// |--vbox
//    |--settingitem (GtkSettingItem)
//    |--background (GtkEventBox)
GtkWidget *iLiad_date_time_create()
{
    GtkWidget* vbox;
    GtkWidget* settingitem;
    GtkWidget* background;
    GtkWidget* info;
    GtkWidget* selection;

    ST_LOGPRINTF("entry");
    g_datetime_screen = g_new0(gtk_datetime_screen, 1);

    vbox = gtk_vbox_new(FALSE, DATETIME_SCREEN_ITEM_SPACING);
    g_datetime_screen->top_vbox = vbox;
    
    settingitem = create_current_time_display();
    gtk_box_pack_start(GTK_BOX(vbox), settingitem, FALSE, FALSE, 0);
    g_datetime_screen->settingitem = settingitem;

    background = create_timezone_selection();
    gtk_box_pack_start(GTK_BOX(vbox), background, FALSE, FALSE, 0);
    g_datetime_screen->timezones = background;

    // selection group not part of widget hierarchy
    selection = ergtk_selection_group_new(g_datetime_screen->button_timezones);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(selection), 1, 1);
    g_signal_connect(G_OBJECT(selection), "selection-update", G_CALLBACK(on_selection_update), NULL);
    g_datetime_screen->selection = selection;
 
    info = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), info, FALSE, FALSE, 0);
    g_datetime_screen->infoitem = info;

    // Show widgets
    gtk_widget_show(vbox);
    return vbox;
}

// widget hierarchy
// |--settingitem (GtkSettingItem)
//    |--background (GtkEventBox)
//       |--alignment
//          |--vbox
//             |--label_show_time_prompt
//             |--label_current_time
GtkWidget *create_current_time_display(void)
{
    GtkWidget* settingitem;
    GtkWidget* background;
    GtkWidget* alignment;
    GtkWidget* vbox;
    GtkWidget* label;

    // settingitem
    settingitem = gtk_settingitem_new("");
    gtk_widget_set_size_request(settingitem, -1, -1);

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "datetime_display_background");
    gtk_widget_set_size_request(background, TITLE_MIN_WIDTH, DATETIME_DISPLAY_HEIGHT);
    gtk_settingitem_add_details(GTK_SETTINGITEM(settingitem), background);

    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              DATETIME_SCREEN_ITEM_PADDING_TOP,
                              DATETIME_SCREEN_ITEM_PADDING_BOTTOM,
                              DATETIME_SCREEN_ITEM_PADDING_LEFT,
                              DATETIME_SCREEN_ITEM_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // vbox
    vbox = gtk_vbox_new(FALSE, DATETIME_DISPLAY_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);

    label = gtk_label_new("");
    gtk_widget_set_name(label, "datetime_screen_info_text");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    g_datetime_screen->label_show_time_prompt = label;

    label = gtk_label_new("");
    gtk_widget_set_name(label, "datetime_screen_date_text");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    g_datetime_screen->label_current_time = label;
    
    gtk_widget_show_all(background);
    gtk_widget_show(settingitem);
    return settingitem;
}

// widget hierarchy
// |--background
//    |--alignment
//       |--vbox
//          |--label_timezone_prompt
//          |--label_possible_date[0]
//          |--hbox
//             |--button
//             |--button
//             |--button
//             |--button
//             |--button
//          ...
//          |--label_possible_date[1]
//          |--hbox
//             |--button
//             |--button
//             |--button
//             |--button
//             |--button
//          ...
GtkWidget *create_timezone_selection(void)
{
    GtkWidget* background;
    GtkWidget* alignment;
    GtkWidget* vbox;
    GtkWidget* label;
    GtkWidget* button;

    // background
    background = gtk_event_box_new();
    gtk_widget_set_name(background, "timezone_selection_background");
    gtk_widget_set_size_request(background, TITLE_MIN_WIDTH, TIMEZONE_SELECTION_HEIGHT);

    // alignment
    alignment = gtk_alignment_new(0, 0, 0, 0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              DATETIME_SCREEN_ITEM_PADDING_TOP,
                              DATETIME_SCREEN_ITEM_PADDING_BOTTOM,
                              DATETIME_SCREEN_ITEM_PADDING_LEFT,
                              DATETIME_SCREEN_ITEM_PADDING_RIGHT);
    gtk_container_add(GTK_CONTAINER(background), alignment);

    // vbox
    vbox = gtk_vbox_new(FALSE, DATETIME_SCREEN_ITEM_SPACING);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    
    label = gtk_label_new("");
    gtk_widget_set_name(label, "datetime_screen_info_text");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    g_datetime_screen->label_timezone_prompt = label;

    g_utc_time = time(NULL);
    
    // divide the buttons into 2 parts, determine the boundary first
    int i;
    for (i=0; i<TOTAL_TIMEZONES; i++)
    {
        if ((g_utc_time+timezones[i].tzoffset) / 86400 > (g_utc_time+timezones[0].tzoffset) / 86400)
        {
            g_date_boundary = i;
            break;
        }
    }
    
    // draw the first possible date label
    label = gtk_label_new("");
    gtk_widget_set_name(label, "datetime_date_heading_text");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    g_datetime_screen->label_possible_date[0] = label;

    // draw the rest part
    int minutes_in_tz;
    char button_text[8];
    GtkWidget* hbox = NULL;
    for (i=0; i<TOTAL_TIMEZONES; i++)
    {
        if (i == g_date_boundary)
        {
            // draw the second possible date label
            label = gtk_label_new("");
            gtk_widget_set_name(label, "datetime_date_heading_text");
            gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
            gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
            g_datetime_screen->label_possible_date[1] = label;
        }
        
        if ((i<g_date_boundary && i%5 == 0) ||
            (i>=g_date_boundary && (i-g_date_boundary)%5 == 0))
        {
            // we should create a new hbox, for we have a new line of buttons
            hbox = gtk_hbox_new(FALSE, TIMEZONE_BUTTON_ITEM_SPACING);
            gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(hbox), FALSE, FALSE, 0);
        }
        
        minutes_in_tz = ((g_utc_time + timezones[i].tzoffset) % 86400) / 60;
        sprintf(button_text, "%02d:%02d", minutes_in_tz / 60, minutes_in_tz % 60);

        button = ergtk_toggle_button_new_with_label(button_text);
        gtk_widget_set_size_request(GTK_WIDGET(button), TZ_BUTTON_WIDTH, TZ_BUTTON_HEIGHT);
        gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(button), FALSE, FALSE, 0);
        g_datetime_screen->button_timezones[i] = GTK_TOGGLE_BUTTON(button);
    }

    g_datetime_screen->button_timezones[TOTAL_TIMEZONES] = NULL;

    gtk_widget_show_all(background);
    return background;
}

void iLiad_date_time_set_text(void)
{
    ST_LOGPRINTF("entry");
    if (g_datetime_screen->settingitem)
    {
        gtk_settingitem_set_header_text(GTK_SETTINGITEM(g_datetime_screen->settingitem),
                _("Time Selection"));
        
        gtk_label_set_text(GTK_LABEL(g_datetime_screen->label_show_time_prompt),
                _("The current time and date on your iLiad is:"));

        if (g_current_tz_offset != -1)
        {
            iLiad_update_datetime(g_current_tz_offset);
        }
    }

    if (g_datetime_screen->timezones)
    {
        gtk_label_set_text(GTK_LABEL(g_datetime_screen->label_timezone_prompt),
                _("To change the clock, select your current date and time from the list below."));

        // set label text for the first possible date
        time_t tm = g_utc_time + timezones[0].tzoffset;
        char buf[DATE_STRING_LENGTH];
        /* TRANSLATORS: This is a date and time format specifier, according to the strftime(3) man page.
           The corresponding string should be something like Monday, July 30 2007. */
        // xgettext:no-c-format
        strftime(buf, DATE_STRING_LENGTH, _("%A, %x"), gmtime(&tm));  // xgettext:no-c-format
        gtk_label_set_text(GTK_LABEL(g_datetime_screen->label_possible_date[0]), buf);

        // set label text for the second possible date
        tm = g_utc_time + timezones[g_date_boundary].tzoffset;
        // xgettext:no-c-format
        strftime(buf, DATE_STRING_LENGTH, _("%A, %x"), gmtime(&tm));  // xgettext:no-c-format
        gtk_label_set_text(GTK_LABEL(g_datetime_screen->label_possible_date[1]), buf);
    }

    if (g_datetime_screen->infoitem)
    {
        // set help text
        gtk_infoitem_set_text(GTK_INFOITEM(g_datetime_screen->infoitem),
                _("You can set your date and time by connecting to iDS. Afterwards you can use this page to update the time for your current timezone."));
    }
}

void iLiad_date_time_display_data(void)
{
    int i = 0;
    
    ST_LOGPRINTF("entry");

    g_current_tz_offset = get_current_tz_offset();

    for (i=0; i<TOTAL_TIMEZONES; i++)
    {
        if (timezones[i].tzoffset == g_current_tz_offset)
        {
            break;
        }
    }

    GtkToggleButton* button_to_be_active = g_datetime_screen->button_timezones[i];
    gtk_toggle_button_set_active(button_to_be_active, TRUE);
}

void iLiad_date_time_handle_refresh(void)
{
    GtkWidget* background;
    GtkWidget* info;
    GtkWidget* selection;
    GtkWidget* vbox = g_datetime_screen->top_vbox;

    ST_LOGPRINTF("entry");

    if (g_datetime_screen->timezones)
    {
        gtk_widget_destroy(g_datetime_screen->timezones);
        g_datetime_screen->timezones = NULL;
    }

    if (g_datetime_screen->selection)
    {
        gtk_object_destroy(GTK_OBJECT(g_datetime_screen->selection));
        g_datetime_screen->selection = NULL;
    }

    if (g_datetime_screen->infoitem)
    {
        gtk_widget_destroy(g_datetime_screen->infoitem);
        g_datetime_screen->infoitem = NULL;
    }

    background = create_timezone_selection();
    gtk_box_pack_start(GTK_BOX(vbox), background, FALSE, FALSE, 0);
    g_datetime_screen->timezones = background;

    // selection group not part of widget hierarchy
    selection = ergtk_selection_group_new(g_datetime_screen->button_timezones);
    ergtk_selection_group_set_details(ERGTK_SELECTION_GROUP(selection), 1, 1);
    g_signal_connect(G_OBJECT(selection), "selection-update", G_CALLBACK(on_selection_update), NULL);
    g_datetime_screen->selection = selection;

    info = gtk_infoitem_new(FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), info, FALSE, FALSE, 0);
    g_datetime_screen->infoitem = info;

    // Refresh the text displayed on the widget
    g_current_tz_offset = -1;
    iLiad_date_time_set_text();

    // Set the selected button and update current time
    iLiad_date_time_display_data();
}

void on_selection_update(erGtkSelectionGroup * selection, gpointer button, gpointer data)
{
    int index = 0;

    ST_LOGPRINTF("entry");
    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(selection), &index, 1);

    int tzoffset = timezones[index].tzoffset;
    
    // Update current timezone offset settings
    if (g_current_tz_offset != tzoffset)
    {
        g_current_tz_offset = tzoffset;
        
        // Update symbol link /mnt/settings/timezone
        // re-direct to the correct timezone file
        set_tz_offset(timezones[index].tzname);
    }

    // Update current date time displayed in the screen
    iLiad_update_datetime(g_current_tz_offset);
}

gboolean set_tz_offset(const char* tzfile)
{
    char actual_tzfile[256];
    
    snprintf(actual_tzfile, 256, "%s%s", TZ_FILES_DIR, tzfile);
    if (access(TZ_SETTING_FILE, F_OK) == 0)
    {
        if (unlink(TZ_SETTING_FILE) != 0)
        {
            ST_ERRORPRINTF("Can't remove %s", TZ_SETTING_FILE);
            return FALSE;
        }
    }
   
    if (symlink(actual_tzfile, TZ_SETTING_FILE) != 0)
    {
        ST_ERRORPRINTF("Can't create symbol link from %s to %s", actual_tzfile, TZ_SETTING_FILE);
        return FALSE;
    }

    return TRUE;
}

void iLiad_update_datetime(int tzoffset)
{
    if (g_datetime_screen)
    {
        char buf[DATE_STRING_LENGTH];
        time_t tm_local = g_utc_time + tzoffset;
        /* TRANSLATORS: This is a date and time format specifier, according to the strftime(3) man page.
           The corresponding string should be something like 13:23 Monday, July 30 2007.
           Keep %R unchanged. */
        // xgettext:no-c-format
        strftime(buf, DATE_STRING_LENGTH, _("%R %A, %x"), gmtime(&tm_local));  // xgettext:no-c-format
        gtk_label_set_text(GTK_LABEL(g_datetime_screen->label_current_time), buf);
    }
}

static int get_current_tz_offset()
{
    FILE* fp = fopen(TZ_FILE, "rb");

    if (NULL == fp)
    {
        ST_ERRORPRINTF("Can't open %s, use default timezone settings.", TZ_FILE);
        goto error;
    }

    fseek(fp, sizeof(struct tzhead), SEEK_SET);
    
    int offset;
    if (fread(&offset, sizeof(int), 1, fp) != 1)
    {
        ST_ERRORPRINTF("Error reading %s, use default timezone settings.", TZ_FILE);
        fclose(fp);
        goto error;
    }
    
    fclose(fp);

    return GINT32_FROM_BE(offset);

error:
    set_tz_offset(DEFAULT_TIMEZONE_NAME);
    return DEFAULT_TIMEZONE_OFFSET;
}

