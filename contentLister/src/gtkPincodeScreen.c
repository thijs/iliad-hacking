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
 * \file gtkPincodeScreen.c
 * \brief content lister - Input Pincode Screen
 *
 * (1)when user boot system, if PINCODE is set ON, first show this pincode screen
 * at here, user can input PINCODE.
 * if PINCODE passed, on_passed_callback is invoked(boot device normally)
 * if PINCODE not passed after users try 3 times, power down the device
 * in some other case, no user activities happened,
 * after some time e.g 5mins, power down the device too
 *
 * (2)when user click 'lock screen' icon on toolbar to lock screen, show pincode screen
 * if PINCODE passed, on_passed_callback is invoked(go back to the previous state)
 * otherwise deal with the same as PINCODE at startup
 *
 * (3)when signing a document(only pincode is on, CB can allow signing), show pincode screen
 * if PINCODE passed, on_passed_callback is invoked(signed the document)
 * otherwise deal with the same as PINCODE at startup
 *
 * (4)before user enter the settings, show pincode screen
 * if PINCODE passed, on_passed_callback is invoked(let user enter the settings application)
 * otherwise deal with the same as PINCODE at startup
 *
 *  as above,
 *  (1)(2)for locking purpose, CB locks all keys except 'CONNECT LONG' which is for RESET pinocde
 *  (3)(4)for authorizing purpose, CB locks all keys except 'OVERVIEW' which is for cancel(leaving) the pincode
 *
 * Copyright (C) 2006-2008 iRex Technologies BV.
 * All rights reserved.
 *
 */


// system includes
#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

// ereader library includes
#include <liberdm/display.h>
#include <liberdm/erdm.h>
#include <libergtk/ergtk.h>
#include <liberipc/eripctoolbar.h>
#include <libermanifest/ermanifest.h>

// local includes
#include "contentListerLog.h"
#include "system.h"
#include "pincodeIdleTime.h"
#include "gtkPincodeScreen.h"
#include "displayUpdate.h"
#include "erConnect.h"
#include "control.h"
#include "erbusy.h"
#include "languages.h"

#define PINCODE_UI_MIN_LENGTH 4
#define PINCODE_UI_MAX_LENGTH 4

static gint g_timeout_handler_id = 0;
//
// internally used routines forward declarations
//
// create widgets
static void gtk_pincode_screen_class_init(GtkPincodeScreenClass * klass);
static void gtk_pincode_screen_init(GtkPincodeScreen * screen);
static GtkWidget *create_pincode_input_widget(GtkPincodeScreen * screen);
static GtkWidget *create_pincode_message_widget(GtkPincodeScreen * screen);
//
static void gtk_pincode_screen_set_text(GtkPincodeScreen * screen);
static void pincode_message_widget_set_text(GtkPincodeScreen * screen, gchar * text);
static void gtk_pincode_screen_show_input_widget(GtkPincodeScreen * screen);
static void gtk_pincode_screen_show_message_widget(GtkPincodeScreen * screen);
// create pincode stars
static GtkWidget *create_pincode_stars(GtkPincodeScreen * screen);
static void destroy_pincode_stars(GtkPincodeScreen * screen);
static void pincode_stars_append_number(pincodeStars * stars, gint number);
static void pincode_stars_backspace(pincodeStars * stars);
static gchar *pincode_stars_get_text(pincodeStars * stars);
static gboolean pincode_stars_check_text(pincodeStars * stars);
static void pincode_stars_reset(pincodeStars * stars);
static gboolean delayed_on_changed(gpointer user_data);

// gtkPincodeScreen signals
static gboolean on_gtk_pincode_screen_expose_event(GtkPincodeScreen * screen,
                                                   GdkEventExpose * event, gpointer user_data);
static gboolean on_gtk_pincode_screen_hide(GtkPincodeScreen * screen, gpointer user_data);
static gboolean on_gtk_pincode_screen_button_press(GtkPincodeScreen * screen,
                                                   GdkEventButton * event, gpointer user_data);
static gboolean on_gtk_pincode_motion_notify_event(GtkPincodeScreen * screen,
                                                   GdkEventMotion * event, gpointer user_data);
static gboolean on_gtk_pincode_screen_button_release(GtkPincodeScreen *
                                                     screen, GdkEventButton * event, gpointer user_data);
static gboolean on_gtk_pincode_screen_key_press(GtkPincodeScreen * screen, GdkEventKey * event, gpointer user_data);
static gboolean on_gtk_pincode_screen_key_release(GtkPincodeScreen * screen, GdkEventKey * event, gpointer user_data);
static gboolean on_gtk_pincode_screen_destroy(GtkPincodeScreen * screen, GdkEvent * event, gpointer user_data);
// keyboard signals
static gboolean on_gtk_pincode_keyboard_button_press(GtkWidget * keyboard, GdkEventButton * event, gpointer user_data);
static gint get_clicked_number(gint x, gint y);
// confirmButton signals
static void on_confirm_button_toggled(GtkToggleButton * button, gpointer screen);
static gboolean delayed_on_confirm_button_clicked(gpointer user_data);
static gboolean pincode_shutdown(gpointer user_data);
static gboolean pincode_passed(gpointer user_data);
static gboolean pincode_decrease_display_level(gpointer data);
// messageWidget signals
static gboolean on_message_widget_clicked_event(GtkWidget * widget, GdkEventButton * event, gpointer user_data);

//
// create a gtkPincodeScreen object
// 
GtkWidget *gtk_pincode_screen_new()
{
    GtkPincodeScreen *screen;
    screen = (GtkPincodeScreen *) g_object_new(GTK_PINCODE_SCREEN_TYPE, NULL);
    return GTK_WIDGET(screen);
}

GType gtk_pincode_screen_get_type(void)
{
    static GType pincode_screen_type = 0;

    if (!pincode_screen_type)
    {
        static const GTypeInfo pincode_screen_info = {
            sizeof(GtkPincodeScreenClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) gtk_pincode_screen_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(GtkPincodeScreen),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) gtk_pincode_screen_init,
        };
        pincode_screen_type = g_type_register_static(GTK_TYPE_EVENT_BOX, "PincodeScreen", &pincode_screen_info, 0);
    }
    return pincode_screen_type;
}

//
// create gtkPincodeScreen - set the default values 
//
// object hierarchy:
// screen (GtkPincodeScreen background)
//    |--alignment
//       |--clientArea(GtkVBox)
//          |--inputWidget
//          |--messageWidget
// 
static void gtk_pincode_screen_init(GtkPincodeScreen * screen)
{
    GtkWidget *alignment;
    GtkWidget *clientArea;      // GtkVBox
    GtkWidget *inputWidget;     // input widget
    GtkWidget *messageWidget;   // message widget
    gboolean enable = FALSE;
    gchar pincode[PINCODE_MAX_LENGTH + 1] = "";

    if (NULL == screen)
        return;
    gtk_widget_set_name(GTK_WIDGET(screen), "pincode_screen_background");
    gtk_widget_set_size_request(GTK_WIDGET(screen), PINCODE_SCREEN_WIDTH, PINCODE_SCREEN_HEIGHT);
    // alignment 
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_container_add(GTK_CONTAINER(screen), GTK_WIDGET(alignment));
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              PINCODE_SCREEN_PADDING_TOP,
                              PINCODE_SCREEN_PADDING_BOTTOM, PINCODE_SCREEN_PADDING_LEFT, PINCODE_SCREEN_PADDING_RIGHT);
    // clientArea
    clientArea = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(alignment), clientArea);
    // inputWidget
    inputWidget = create_pincode_input_widget(screen);
    gtk_box_pack_start(GTK_BOX(clientArea), GTK_WIDGET(inputWidget), FALSE, FALSE, 0);
    // messageWidget
    messageWidget = create_pincode_message_widget(screen);
    gtk_box_pack_start(GTK_BOX(clientArea), GTK_WIDGET(messageWidget), FALSE, FALSE, 0);
    // signals 
    g_signal_connect(G_OBJECT(screen), "expose_event", G_CALLBACK(on_gtk_pincode_screen_expose_event), NULL);
    g_signal_connect(G_OBJECT(screen), "hide", G_CALLBACK(on_gtk_pincode_screen_hide), NULL);
    g_signal_connect(G_OBJECT(screen), "button-press-event", G_CALLBACK(on_gtk_pincode_screen_button_press), NULL);
    g_signal_connect(G_OBJECT(screen), "motion_notify_event", G_CALLBACK(on_gtk_pincode_motion_notify_event), NULL);
    g_signal_connect(G_OBJECT(screen), "button-release-event", G_CALLBACK(on_gtk_pincode_screen_button_release), NULL);
    g_signal_connect(G_OBJECT(screen), "key_press_event", G_CALLBACK(on_gtk_pincode_screen_key_press), NULL);
    g_signal_connect(G_OBJECT(screen), "key_release_event", G_CALLBACK(on_gtk_pincode_screen_key_release), NULL);
    g_signal_connect(G_OBJECT(screen), "destroy", G_CALLBACK(on_gtk_pincode_screen_destroy), NULL);
    // set gtkPincodeScreen default values
    screen->clientArea = clientArea;
    screen->inputWidget = inputWidget;
    screen->messageWidget = messageWidget;
    screen->tryTimes = 0;
    screen->passed = FALSE;
    screen->isVisible = FALSE;
    // store 'enable' and 'pincode' to gtkPincodeScreen
    screen->pincodeOn = enable;
    strcpy(screen->pincode, pincode);
    // 
    gtk_pincode_screen_set_text(screen);
    // show widget 
    gtk_widget_show(GTK_WIDGET(clientArea));
    gtk_widget_show(GTK_WIDGET(alignment));
    gtk_pincode_screen_show_input_widget(screen);
//  add timeout in order to compute user idle time later
//  accumulate_idle_time_begin();
}

//
// init class
//
static void gtk_pincode_screen_class_init(GtkPincodeScreenClass * klass)
{
    // nothing needs to be done here 
}

//
// create input pincode widget
//
// object hierarchy:
// |--box(GtkEventBox background)
//    |--fixed(GtkFixed)
//       |--inputLabel
//       |--starsWidget
//       |--keyboard(GtkEventBox)
//       |--confirmButton(GtkToggleButton)
//       |--inputInfoLabel
// 
static GtkWidget *create_pincode_input_widget(GtkPincodeScreen * screen)
{
    GtkWidget *box;
    GtkWidget *fixed;
    GtkWidget *inputLabel;
    GtkWidget *starsWidget;
    GtkWidget *keyboard;
    GtkWidget *confirmButton;
    GtkWidget *inputInfoLabel;
    // box
    box = gtk_event_box_new();
    gtk_widget_set_name(box, "pincode_input_background");
    gtk_widget_set_size_request(box, PINCODE_BACKGROUND_WIDTH, PINCODE_BACKGROUND_HEIGHT);
    // fixed
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(box), fixed);
    // inputLabel
    inputLabel = gtk_label_new("");
    gtk_fixed_put(GTK_FIXED(fixed), inputLabel, PINCODE_INPUT_LABEL_XPOS, PINCODE_INPUT_LABEL_YPOS);
    gtk_widget_set_name(inputLabel, "pincode_input_title_text");
    gtk_widget_set_size_request(inputLabel, PINCODE_INPUT_LABEL_WIDTH, PINCODE_INPUT_LABEL_HEIGHT);
    gtk_label_set_justify(GTK_LABEL(inputLabel), GTK_JUSTIFY_CENTER);
    // starsWidget
    starsWidget = create_pincode_stars(screen);
    gtk_fixed_put(GTK_FIXED(fixed), starsWidget, PINCODE_STARS_XPOS, PINCODE_STARS_YPOS);
    // keyboard
    keyboard = gtk_event_box_new();
    gtk_fixed_put(GTK_FIXED(fixed), keyboard, PINCODE_KEYBOARD_XPOS, PINCODE_KEYBOARD_YPOS);
    gtk_widget_set_name(keyboard, "pincode_keyboard_background");
    gtk_widget_set_size_request(GTK_WIDGET(keyboard), PINCODE_KEYBOARD_WIDTH, PINCODE_KEYBOARD_HEIGHT);
    // confirmButton
    confirmButton = ergtk_toggle_button_new();
    gtk_widget_set_name(confirmButton, "pincode_confirm");
    gtk_fixed_put(GTK_FIXED(fixed), confirmButton, PINCODE_BUTTON_XPOS, PINCODE_BUTTON_YPOS);
    gtk_widget_set_size_request(confirmButton, PINCODE_BUTTON_WIDTH, PINCODE_BUTTON_HEIGHT);
    // inputInfoLabel  
    inputInfoLabel = gtk_label_new("");
    gtk_fixed_put(GTK_FIXED(fixed), inputInfoLabel, PINCODE_INFO_LABEL_XPOS, PINCODE_INFO_LABEL_YPOS);
    gtk_widget_set_name(inputInfoLabel, "pincode_screen_info_text");
    gtk_widget_set_size_request(GTK_WIDGET(inputInfoLabel), PINCODE_INFO_LABEL_WIDTH, PINCODE_INFO_LABEL_HEIGHT);
    gtk_label_set_justify(GTK_LABEL(inputInfoLabel), GTK_JUSTIFY_CENTER);
    // set screen values
    screen->inputLabel = inputLabel;
    screen->keyboard = keyboard;
    screen->confirmButton = confirmButton;
    screen->inputInfoLabel = inputInfoLabel;
    // keyboard signals 
    g_signal_connect(G_OBJECT(keyboard), "button-press-event",
                     G_CALLBACK(on_gtk_pincode_keyboard_button_press), screen);
    // confirmButton signals
    g_signal_connect(G_OBJECT(confirmButton), "toggled", G_CALLBACK(on_confirm_button_toggled), screen);
    // show widgets
    gtk_widget_show(fixed);
    gtk_widget_show(inputLabel);
    gtk_widget_show(starsWidget);
    gtk_widget_show(keyboard);
    gtk_widget_show(confirmButton);
    gtk_widget_show(inputInfoLabel);

    return box;
}

// 
// screen
//   |--box(GtkEventBox background)
//      |--vbox
//         |--messageLabel
//         |--infoLabel
// 
static GtkWidget *create_pincode_message_widget(GtkPincodeScreen * screen)
{
    GtkWidget *box;
    GtkWidget *vbox;
    GtkWidget *messageLabel;
    GtkWidget *messageInfoLabel;
    // box
    box = gtk_event_box_new();
    gtk_widget_set_name(box, "pincode_message_background");
    gtk_widget_set_size_request(box, PINCODE_BACKGROUND_WIDTH, PINCODE_BACKGROUND_HEIGHT);
    // vbox
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(box), vbox);
    // messageLabel 
    messageLabel = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), messageLabel, FALSE, FALSE, 0);
    gtk_widget_set_name(messageLabel, "pincode_message_title_text");
    gtk_label_set_line_wrap(GTK_LABEL(messageLabel), TRUE);
    gtk_widget_set_size_request(messageLabel, PINCODE_MESSAGE_LABEL_WIDTH, PINCODE_MESSAGE_LABEL_HEIGHT);
    gtk_label_set_justify(GTK_LABEL(messageLabel), GTK_JUSTIFY_CENTER);
    // messageInfoLabel
    messageInfoLabel = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), messageInfoLabel, FALSE, FALSE, 0);
    gtk_widget_set_name(messageInfoLabel, "pincode_screen_info_text");
    gtk_widget_set_size_request(GTK_WIDGET(messageInfoLabel), PINCODE_INFO_LABEL_WIDTH, PINCODE_INFO_LABEL_HEIGHT);
    gtk_label_set_justify(GTK_LABEL(messageInfoLabel), GTK_JUSTIFY_CENTER);
    // signals 
    g_signal_connect(G_OBJECT(box), "button-press-event", G_CALLBACK(on_message_widget_clicked_event), screen);
    // set screen values
    screen->messageLabel = messageLabel;
    screen->messageInfoLabel = messageInfoLabel;
    // show widgets
    gtk_widget_show_all(vbox);

    return box;
}

//
// widgets display text string
//
static void gtk_pincode_screen_set_text(GtkPincodeScreen * screen)
{
    CL_LOGPRINTF("entry");
    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));
    if (screen->inputLabel)
    {
        gtk_label_set_text(GTK_LABEL(screen->inputLabel), _("Enter your PIN code"));
    }
    if (screen->confirmButton)
    {
        gtk_button_set_label(GTK_BUTTON(screen->confirmButton), _("Confirm"));
    }
    if (screen->inputInfoLabel)
    {
        gtk_label_set_text(GTK_LABEL(screen->inputInfoLabel),
                           _("Forgot your PIN code? Please visit your MyiRex account."));
    }
    if (screen->messageInfoLabel)
    {
        gtk_label_set_text(GTK_LABEL(screen->messageInfoLabel),
                           _("Forgot your PIN code? Please visit your MyiRex account."));
    }
}

static void pincode_message_widget_set_text(GtkPincodeScreen * screen, gchar * text)
{
    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));
    g_return_if_fail(NULL != screen->messageLabel);
    g_return_if_fail(NULL != text);
    gtk_label_set_text(GTK_LABEL(screen->messageLabel), text);
}

//
// show input widget, hide message widget
//
static void gtk_pincode_screen_show_input_widget(GtkPincodeScreen * screen)
{
    GtkWidget *clientArea = NULL;
    GtkWidget *inputWidget = NULL;
    GtkWidget *messageWidget = NULL;

    gboolean show = FALSE;

    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));

    clientArea = GTK_PINCODE_SCREEN(screen)->clientArea;
    messageWidget = GTK_PINCODE_SCREEN(screen)->messageWidget;
    inputWidget = GTK_PINCODE_SCREEN(screen)->inputWidget;

    g_return_if_fail(NULL != clientArea);
    g_return_if_fail(NULL != inputWidget);
    g_return_if_fail(NULL != messageWidget);

    show = GTK_WIDGET_VISIBLE(inputWidget);
    if (TRUE == show)
    {
        return;
    }

    gtk_widget_hide(messageWidget);
    gtk_widget_show(inputWidget);

    gtk_widget_grab_focus(GTK_WIDGET(inputWidget));
}

//
// hide input widget, show message widget
//
static void gtk_pincode_screen_show_message_widget(GtkPincodeScreen * screen)
{
    GtkWidget *clientArea = NULL;
    GtkWidget *inputWidget = NULL;
    GtkWidget *messageWidget = NULL;
    gboolean show = FALSE;

    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));

    clientArea = GTK_PINCODE_SCREEN(screen)->clientArea;
    messageWidget = GTK_PINCODE_SCREEN(screen)->messageWidget;
    inputWidget = GTK_PINCODE_SCREEN(screen)->inputWidget;

    g_return_if_fail(NULL != clientArea);
    g_return_if_fail(NULL != inputWidget);
    g_return_if_fail(NULL != messageWidget);

    show = GTK_WIDGET_VISIBLE(messageWidget);
    if (TRUE == show)
    {
        return;
    }

    gtk_widget_hide(inputWidget);
    gtk_widget_show(messageWidget);

    gtk_widget_grab_focus(GTK_WIDGET(GTK_PINCODE_SCREEN(screen)->messageWidget));
}

static void gtk_pincode_show_message_info_label(GtkPincodeScreen * screen, gboolean show)
{
    GtkWidget *messageInfoLabel = NULL;
    gboolean visible = FALSE;

    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));

    messageInfoLabel = GTK_PINCODE_SCREEN(screen)->messageInfoLabel;
    g_return_if_fail(NULL != messageInfoLabel);

    visible = GTK_WIDGET_VISIBLE(messageInfoLabel);
    if (show == visible)
    {
        return;
    }
    if (TRUE == show)
    {
        gtk_widget_show(messageInfoLabel);
    }
    else
    {
        gtk_widget_hide(messageInfoLabel);
    }
}

void gtk_pincode_screen_layout_reset(GtkWidget*screen)
{
    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));
    
    gtk_pincode_screen_set_text(GTK_PINCODE_SCREEN(screen));
    pincode_stars_reset(GTK_PINCODE_SCREEN(screen)->stars);
    gtk_pincode_screen_show_input_widget(GTK_PINCODE_SCREEN(screen));
    gtk_pincode_show_message_info_label(GTK_PINCODE_SCREEN(screen), TRUE);
}

void gtk_pincode_screen_data_reset(GtkWidget * screen)
{
    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));

//    GTK_PINCODE_SCREEN(screen)->tryTimes = 0;// remove to- only pincode passed, reset tryTimes
    GTK_PINCODE_SCREEN(screen)->passed = 0;
}

void gtk_pincode_screen_set_pincode_settings(GtkWidget * screen, gboolean on, const gchar * pincode)
{
    g_return_if_fail(NULL != screen && NULL != pincode);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));

    GTK_PINCODE_SCREEN(screen)->pincodeOn = on;
    strcpy(GTK_PINCODE_SCREEN(screen)->pincode, pincode);
}

void gtk_pincode_screen_get_pincode_settings(GtkWidget * screen, gboolean * on, gchar * pincode)
{
    g_return_if_fail(NULL != screen && NULL != on && NULL != pincode);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));

    *on = GTK_PINCODE_SCREEN(screen)->pincodeOn;
    strcpy(pincode, GTK_PINCODE_SCREEN(screen)->pincode);
}

void gtk_pincode_screen_set_reason(GtkWidget * screen, showPincodeReason_t reason)
{
    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));

    GTK_PINCODE_SCREEN(screen)->reason = reason;
}

showPincodeReason_t gtk_pincode_screen_get_reason(GtkWidget * screen)
{
    showPincodeReason_t reason = undefinedReason_t;
    if (NULL == screen)
        return reason;
    if (!IS_GTK_PINCODE_SCREEN(screen))
        return reason;

    return GTK_PINCODE_SCREEN(screen)->reason;
}

void gtk_pincode_screen_set_on_cancel_callback(GtkWidget * screen, on_cancel_callback_t * func)
{
    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));

    GTK_PINCODE_SCREEN(screen)->on_cancel_callback = func;
}

void gtk_pincode_screen_on_cancel_callback(GtkWidget * screen)
{
    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));
    if (GTK_PINCODE_SCREEN(screen)->on_cancel_callback)
    {
        GTK_PINCODE_SCREEN(screen)->on_cancel_callback();
    }
}

void gtk_pincode_screen_set_on_passed_callback(GtkWidget * screen, on_passed_callback_t * func)
{
    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));

    GTK_PINCODE_SCREEN(screen)->on_passed_callback = func;
}
//
// pincode screen signals callback functions
//
static gboolean
on_gtk_pincode_screen_expose_event(GtkPincodeScreen * screen, GdkEventExpose * event, gpointer user_data)
{
    // ignore partial exposure events
    if ( !screen->isVisible )
    {
        screen->isVisible = TRUE;
        display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL);
    }
    return FALSE;
}

static gboolean
on_gtk_pincode_screen_hide(GtkPincodeScreen * screen, gpointer user_data)
{
    screen->isVisible = FALSE;
    return FALSE;
}

static gboolean
on_gtk_pincode_screen_button_press(GtkPincodeScreen * screen, GdkEventButton * event, gpointer user_data)
{
    accumulate_idle_time_reset();
    return FALSE;
}

static gboolean
on_gtk_pincode_motion_notify_event(GtkPincodeScreen * screen, GdkEventMotion * event, gpointer user_data)
{
    accumulate_idle_time_reset();
    return FALSE;
}

static gboolean
on_gtk_pincode_screen_button_release(GtkPincodeScreen * screen, GdkEventButton * event, gpointer user_data)
{
    accumulate_idle_time_reset();
    return FALSE;
}

static gboolean on_gtk_pincode_screen_key_press(GtkPincodeScreen * screen, GdkEventKey * event, gpointer user_data)
{
    accumulate_idle_time_reset();
    return FALSE;
}

static gboolean on_gtk_pincode_screen_key_release(GtkPincodeScreen * screen, GdkEventKey * event, gpointer user_data)
{
    accumulate_idle_time_reset();
    return FALSE;
}

static gboolean on_gtk_pincode_screen_destroy(GtkPincodeScreen * screen, GdkEvent * event, gpointer user_data)
{
    accumulate_idle_time_end();
    destroy_pincode_stars(screen);
    return FALSE;
}

//
// keyboard signals
//
static gboolean on_gtk_pincode_keyboard_button_press(GtkWidget * button, GdkEventButton * event, gpointer user_data)
{
    GtkPincodeScreen *screen = user_data;
    gint xPos = 0;
    gint yPos = 0;
    gint number;

    if (NULL == screen)
        return FALSE;
    if (!IS_GTK_PINCODE_SCREEN(screen))
        return FALSE;
    if (event->type != GDK_BUTTON_PRESS)
    {
        CL_ERRORPRINTF("event->type(%d)",event->type);
        return FALSE;
    }
    xPos = event->x;
    yPos = event->y;
    number = get_clicked_number(xPos, yPos);
    if (number >= 0 && number <= 9)
    {
        pincode_stars_append_number(screen->stars, number);
    }
    else if (-1 == number)
    {
        pincode_stars_backspace(screen->stars);
    }
    return FALSE;
}

// 
// return 0-9
// return -1 means backspace
// return other means invalid  
// 
gint get_clicked_number(gint x, gint y)
{
    int i, numbers, w, h;
    int spacing, leftPadding, rightPadding, topPadding, bottomPadding;
    int rcL, rcR, rcT, rcB;
    int returnVal = -2;

    numbers = 11;
    w = 35;
    h = 35;
    spacing = 4;
    leftPadding = 9;
    rightPadding = 9;
    topPadding = 7;
    bottomPadding = 7;

    if (x >= 0 && x <= PINCODE_KEYBOARD_WIDTH && y >= 0 && y <= PINCODE_KEYBOARD_HEIGHT)
    {
        rcT = topPadding;
        rcB = rcT + h;
        i = 0;
        for (i = 0; i <= numbers; i++)
        {
            if (0 == i)         // zero
            {
                rcL = leftPadding;
                rcR = rcL + w;
            }
            else if (10 == i)   // backspace
            {
                rcR = PINCODE_KEYBOARD_WIDTH - rightPadding;
                rcL = rcR - w;
            }
            else
            {
                rcL = leftPadding + i * (w + spacing);
                rcR = rcL + w;
            }
            if (x >= rcL && x <= rcR && y >= rcT && y <= rcB)
            {
                returnVal = i + 1;
                if (10 == returnVal)
                    returnVal = 0;
                else if (11 == returnVal)
                    returnVal = -1;
                break;
            }
        }                       // for
    }
    return returnVal;
}

//
// confirmButton signals handlers
//

// confirm button selected
static void on_confirm_button_toggled(GtkToggleButton * button, gpointer screen)
{
    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));
    pincodeStars *stars = (pincodeStars *) (GTK_PINCODE_SCREEN(screen)->stars);
    g_return_if_fail(NULL != stars);

    gboolean active = gtk_toggle_button_get_active(button);
    if (active)
    {
        g_timeout_add(1000, delayed_on_confirm_button_clicked, screen);
    }
}

static gboolean delayed_on_confirm_button_clicked(gpointer user_data)
{
    GtkPincodeScreen *screen = user_data;
    GtkWidget *button = screen->confirmButton;
    const gchar *text = NULL;
    gboolean check = FALSE;

    erbusy_blink();

    //  check pincode
    if (screen->stars)
    {
        screen->tryTimes++;
        // for signing document, we don't check pincode onoff flag
        if (   (signDocument_t != screen->reason && FALSE == screen->pincodeOn)
            || (0 == strlen(screen->pincode)) )
        {
            check = TRUE;
        }
        else
        {
            check = pincode_stars_check_text(screen->stars);
        }
        if (FALSE == check)
        {
            screen->passed = FALSE;
            if (screen->tryTimes >= MAX_TRY_PINCODE_TIMES)
            {
//                pincode_message_widget_set_text(screen,_("Tried too many times! System is going to power off now."));
//                gtk_pincode_screen_show_message_widget(screen);
//                display_update_request_screen_refresh(TEXTENTRY_CHANGED_LEVEL, NULL);

//                g_timeout_handler_id = g_timeout_add(500, pincode_shutdown, NULL);
                pincode_shutdown(NULL);
            }
            else
            {
                pincode_message_widget_set_text(screen, _("PIN code is incorrect. Please try again."));
                gtk_pincode_screen_show_message_widget(screen);               
                display_update_request_screen_refresh(TEXTENTRY_CHANGED_LEVEL);

                g_timeout_handler_id = g_timeout_add(3000, pincode_passed, (gpointer)screen);
            }
        }
        else
        {
            text = pincode_stars_get_text(screen->stars);
            if (   (signDocument_t != screen->reason && FALSE == screen->pincodeOn) 
                || (0 == strlen(screen->pincode)) 
                || (0 == strcmp(screen->pincode, text)) )
            {
                screen->passed = TRUE;
                screen->tryTimes = 0;
            }
            else
            {
                screen->passed = FALSE;
            }
            if (FALSE == screen->passed)
            {
                if (screen->tryTimes >= MAX_TRY_PINCODE_TIMES)
                {
//                  pincode_message_widget_set_text(screen, _("Tried too many times! System is going to power off now."));
//                  gtk_pincode_screen_show_message_widget(screen);
//                  display_update_request_screen_refresh(TEXTENTRY_CHANGED_LEVEL, NULL);

//                  g_timeout_handler_id = g_timeout_add(500, pincode_shutdown, NULL);
                   pincode_shutdown(NULL);
                }
                else
                {
                    pincode_message_widget_set_text(screen, _("PIN code is incorrect. Please try again."));
                    gtk_pincode_screen_show_message_widget(screen);
                    display_update_request_screen_refresh(TEXTENTRY_CHANGED_LEVEL);

                    g_timeout_handler_id = g_timeout_add(3000, pincode_passed, (gpointer)screen);
                }
            }
            else
            {
//                pincode_message_widget_set_text(screen, _("Your code is accepted."));
//                gtk_pincode_screen_show_message_widget(screen);
//                gtk_pincode_show_message_info_label(screen, FALSE);
//                display_update_request_screen_refresh(TEXTENTRY_CHANGED_LEVEL, NULL);
                
//                g_timeout_handler_id = g_timeout_add(500, pincode_passed, (gpointer) screen);
                pincode_passed(screen);                
            }
        }
    }

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
    return FALSE;               // don't call me anymore
}

static gboolean pincode_shutdown(gpointer user_data)
{
     if (g_timeout_handler_id)
    {
        gtk_timeout_remove(g_timeout_handler_id);
        g_timeout_handler_id = 0;
    }
   ctrl_hide_pincode_screen();
   ctrl_shutdown();
   return FALSE;
}

static gboolean pincode_passed(gpointer user_data)
{
    GtkPincodeScreen* screen = user_data;
    gboolean passed = screen->passed;
    
    if (g_timeout_handler_id)
    {
        gtk_timeout_remove(g_timeout_handler_id);
        g_timeout_handler_id = 0;
    }

    if (!passed)
    {
        // show input messaget to let user try again
        pincode_stars_reset(screen->stars);
        gtk_pincode_screen_show_input_widget(screen);
        display_update_request_screen_refresh( TEXTENTRY_CHANGED_LEVEL);
    }
    else
    {
        int level = display_update_get_level();

        // hide pincode
        if (screen->reason == enterSettings_t)
        {
            display_update_increase_level(NO_DISPLAY_UPDATE_LEVEL);
        }
        else
        {
             display_update_increase_level(MAIN_WINDOW_EXPOSE_LEVEL);
        }
        ctrl_hide_pincode_screen();
        if (screen->reason != startup_t)
        {
            g_idle_add(pincode_decrease_display_level, (gpointer)level);
        }

        // trigger on_passed_callback
        if (screen->on_passed_callback)
        {
            screen->on_passed_callback();
        }
    }
    return FALSE;               // not call again
}

static gboolean pincode_decrease_display_level(gpointer data)
{
    // allow display updates again
    display_update_decrease_level((gint)data/*LOWEST_LEVEL*/);

    return FALSE;               // don't call me again
}

//
// messageWidget signals fucntions
//
static gboolean on_message_widget_clicked_event(GtkWidget * widget, GdkEventButton * event, gpointer user_data)
{
    GtkPincodeScreen *screen = user_data;

    if (!IS_GTK_PINCODE_SCREEN(screen))
        return FALSE;
    if (screen->tryTimes >= MAX_TRY_PINCODE_TIMES)
    {
        pincode_shutdown(NULL);
    }
    else
    {
        // when error message appears, click to return back input pincode again
        // when passed message appears, click to (boot normally...)
        pincode_passed((gpointer) screen);
    }
    return FALSE;
}

//
// pincode stars
//
//|--hbox
//   |--vbox1
//      |--grayStar(GtkEventBox)
//      |--whiteStar(GtkEventBox)
//   |--vbox2
//      |--grayStar
//      |--whiteStar
//   |--vbox3
//      |--grayStar
//      |--whiteStar
//   |--vbox4
//      |--grayStar
//      |--whiteStar
//    
static GtkWidget *create_pincode_stars(GtkPincodeScreen * screen)
{
    pincodeStars *stars = NULL;

    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *grayStar;
    GtkWidget *whiteStar;

    if (NULL == screen)
        return NULL;
    if (!IS_GTK_PINCODE_SCREEN(screen))
        return NULL;
    stars = g_new0(pincodeStars, 1);
    if (NULL == stars)
        return NULL;

    hbox = gtk_hbox_new(FALSE, PINCODE_STAR_SPACING);
    stars->hbox = hbox;
// vbox1
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
    stars->vbox1 = vbox;
    grayStar = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), grayStar, FALSE, FALSE, 0);
    stars->grayStar1 = grayStar;
    gtk_widget_set_name(grayStar, "pincode_gray_star");
    gtk_widget_set_size_request(grayStar, PINCODE_STAR_WIDTH, PINCODE_STAR_HEIGHT);
    whiteStar = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), whiteStar, FALSE, FALSE, 0);
    stars->whiteStar1 = whiteStar;
    gtk_widget_set_name(whiteStar, "pincode_white_star");
    gtk_widget_set_size_request(whiteStar, PINCODE_STAR_WIDTH, PINCODE_STAR_HEIGHT);
// vbox2
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
    stars->vbox2 = vbox;
    grayStar = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), grayStar, FALSE, FALSE, 0);
    stars->grayStar2 = grayStar;
    gtk_widget_set_name(grayStar, "pincode_gray_star");
    gtk_widget_set_size_request(grayStar, PINCODE_STAR_WIDTH, PINCODE_STAR_HEIGHT);
    whiteStar = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), whiteStar, FALSE, FALSE, 0);
    stars->whiteStar2 = whiteStar;
    gtk_widget_set_name(whiteStar, "pincode_white_star");
    gtk_widget_set_size_request(whiteStar, PINCODE_STAR_WIDTH, PINCODE_STAR_HEIGHT);
// vbox3
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
    stars->vbox3 = vbox;
    grayStar = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), grayStar, FALSE, FALSE, 0);
    stars->grayStar3 = grayStar;
    gtk_widget_set_name(grayStar, "pincode_gray_star");
    gtk_widget_set_size_request(grayStar, PINCODE_STAR_WIDTH, PINCODE_STAR_HEIGHT);
    whiteStar = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), whiteStar, FALSE, FALSE, 0);
    stars->whiteStar3 = whiteStar;
    gtk_widget_set_name(whiteStar, "pincode_white_star");
    gtk_widget_set_size_request(whiteStar, PINCODE_STAR_WIDTH, PINCODE_STAR_HEIGHT);
// vbox4
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
    stars->vbox4 = vbox;
    grayStar = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), grayStar, FALSE, FALSE, 0);
    stars->grayStar4 = grayStar;
    gtk_widget_set_name(grayStar, "pincode_gray_star");
    gtk_widget_set_size_request(grayStar, PINCODE_STAR_WIDTH, PINCODE_STAR_HEIGHT);
    whiteStar = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), whiteStar, FALSE, FALSE, 0);
    stars->whiteStar4 = whiteStar;
    gtk_widget_set_name(whiteStar, "pincode_white_star");
    gtk_widget_set_size_request(whiteStar, PINCODE_STAR_WIDTH, PINCODE_STAR_HEIGHT);
// 
    memset(stars->pincode, 0, sizeof(stars->pincode));
// show widgets
    gtk_widget_show(stars->hbox);
    gtk_widget_show(stars->vbox1);
    gtk_widget_show(stars->grayStar1);
    gtk_widget_show(stars->vbox2);
    gtk_widget_show(stars->grayStar2);
    gtk_widget_show(stars->vbox3);
    gtk_widget_show(stars->grayStar3);
    gtk_widget_show(stars->vbox4);
    gtk_widget_show(stars->grayStar4);

    screen->stars = stars;
    stars->on_change_occurred = FALSE;
    stars->display_update_pending = FALSE;

    return hbox;
}

static void destroy_pincode_stars(GtkPincodeScreen * screen)
{
    pincodeStars *stars;
    g_return_if_fail(NULL != screen);
    g_return_if_fail(IS_GTK_PINCODE_SCREEN(screen));
    stars = (pincodeStars *) screen->stars;
    g_return_if_fail(NULL != stars);
    g_free(stars);
    screen->stars = NULL;
}

static void pincode_stars_append_number(pincodeStars * stars, gint number)
{
    GtkWidget *vbox = NULL;
    GtkWidget *grayStar = NULL;
    GtkWidget *whiteStar = NULL;

    gchar string[2];
    int len;

    if (NULL == stars)
        return;
    len = strlen(stars->pincode);
    if (len >= PINCODE_UI_MAX_LENGTH)
    {
        CL_ERRORPRINTF("input pincode length reaches pincode_max_length");
        return;
    }
    sprintf(string, "%d", number);
    if (0 == len)
        strcpy(stars->pincode, string);
    else
        strcat(stars->pincode, string);
    len = strlen(stars->pincode);
    if (1 == len)
    {
        vbox = stars->vbox1;
        grayStar = stars->grayStar1;
        whiteStar = stars->whiteStar1;
    }
    else if (2 == len)
    {
        vbox = stars->vbox2;
        grayStar = stars->grayStar2;
        whiteStar = stars->whiteStar2;
    }
    else if (3 == len)
    {
        vbox = stars->vbox3;
        grayStar = stars->grayStar3;
        whiteStar = stars->whiteStar3;
    }
    else if (4 == len)
    {
        vbox = stars->vbox4;
        grayStar = stars->grayStar4;
        whiteStar = stars->whiteStar4;
    }
    else
    {
        CL_ERRORPRINTF("error");
    }
    if (vbox && grayStar && whiteStar)
    {
        gtk_widget_hide(vbox);
        gtk_widget_hide(grayStar);
        gtk_widget_show(whiteStar);
        gtk_widget_show(vbox);
    }

    if (stars->display_update_pending)
    {
        // timer already running: tell it that another change has occurred
        stars->on_change_occurred = TRUE;
    }
    else
    {
        // timer not running: start it now
        g_timeout_add(500, delayed_on_changed, stars);
        stars->display_update_pending = TRUE;
    }
}

static void pincode_stars_backspace(pincodeStars * stars)
{
    GtkWidget *vbox = NULL;
    GtkWidget *grayStar = NULL;
    GtkWidget *whiteStar = NULL;
    int len;

    if (NULL == stars)
        return;
    len = strlen(stars->pincode);
    if (len <= 0)
    {
        CL_ERRORPRINTF("input pincode length equals zero, neen't backspace");
        return;
    }
    stars->pincode[len - 1] = '\0';
    if (1 == len)
    {
        vbox = stars->vbox1;
        grayStar = stars->grayStar1;
        whiteStar = stars->whiteStar1;
    }
    else if (2 == len)
    {
        vbox = stars->vbox2;
        grayStar = stars->grayStar2;
        whiteStar = stars->whiteStar2;
    }
    else if (3 == len)
    {
        vbox = stars->vbox3;
        grayStar = stars->grayStar3;
        whiteStar = stars->whiteStar3;
    }
    else if (4 == len)
    {
        vbox = stars->vbox4;
        grayStar = stars->grayStar4;
        whiteStar = stars->whiteStar4;
    }
    else
    {
        CL_ERRORPRINTF("error");
    }
    if (vbox && grayStar && whiteStar)
    {
        gtk_widget_hide(vbox);
        gtk_widget_hide(whiteStar);
        gtk_widget_show(grayStar);
        gtk_widget_show(vbox);
    }
 
    if (stars->display_update_pending)
    {
        // timer already running: tell it that another change has occurred
        stars->on_change_occurred = TRUE;
    }
    else
    {
        // timer not running: start it now
        g_timeout_add(500, delayed_on_changed, stars);
        stars->display_update_pending = TRUE;
    }
}

static gboolean delayed_on_changed(gpointer user_data)
{
    pincodeStars *stars = user_data;

    if (NULL == stars)
        return FALSE;

    if (stars->on_change_occurred)
    {
        // item text has been changed: check again later
        stars->on_change_occurred = FALSE;
    }
    else
    {
        // for back to pincode after downloadMgr
         int level = display_update_get_level();
         if (level > TEXTENTRY_CHANGED_LEVEL)
         {
             display_update_decrease_level(TEXTENTRY_CHANGED_LEVEL);
         }
        // item text is stable: update screen
        display_update_request_screen_refresh(TEXTENTRY_CHANGED_LEVEL);
        stars->display_update_pending = FALSE;
    }

    return stars->display_update_pending;       // TRUE = call me again, FALSE = don't call
}

static gchar *pincode_stars_get_text(pincodeStars * stars)
{
    if (NULL == stars)
        return NULL;
    return stars->pincode;
}

static gboolean pincode_stars_check_text(pincodeStars * stars)
{
    gchar *text;
    int len;

    if (NULL == stars)
        return FALSE;
    text = pincode_stars_get_text(stars);
    if (NULL == text)
        return FALSE;
    len = strlen(text);
    if (len >= PINCODE_UI_MIN_LENGTH && len <= PINCODE_UI_MAX_LENGTH)
        return TRUE;
    else
        return FALSE;
}

static void pincode_stars_reset(pincodeStars * stars)
{
    if (NULL == stars)
        return;
    memset(stars->pincode, 0, sizeof(stars->pincode));

    gtk_widget_hide(stars->vbox1);
    gtk_widget_hide(stars->vbox2);
    gtk_widget_hide(stars->vbox3);
    gtk_widget_hide(stars->vbox4);

    gtk_widget_hide(stars->whiteStar1);
    gtk_widget_hide(stars->whiteStar2);
    gtk_widget_hide(stars->whiteStar3);
    gtk_widget_hide(stars->whiteStar4);

    gtk_widget_show(stars->grayStar1);
    gtk_widget_show(stars->grayStar2);
    gtk_widget_show(stars->grayStar3);
    gtk_widget_show(stars->grayStar4);

    gtk_widget_show(stars->vbox1);
    gtk_widget_show(stars->vbox2);
    gtk_widget_show(stars->vbox3);
    gtk_widget_show(stars->vbox4);

    display_update_request_screen_refresh(TEXTENTRY_CHANGED_LEVEL);
}
