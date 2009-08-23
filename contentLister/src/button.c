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

/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

/**.
 * \file button.c
 * \brief content lister - eReader button handling                  
 * 
 * <File description>
 * Depending on the configure value "enable-command-line-input" the button input will come
 * from the command line (number 1 till 11) or from the actual button driver  
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
#include <config.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <X11/keysymdef.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>
#include <libermanifest/ermanifest.h>

#ifdef ENABLE_LOGGING
#include <liberlog/erlog.h>
#endif /* ENABLE_LOGGING */

#include "contentListerLog.h"
#include "erbusy.h"
#include "button.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "programManager.h"
#include "stack.h"
#include "powerManager.h"
#include "click.h"
#include "toolbar.h"
#include "gtkDistListWnd.h"


// internal data
static gboolean g_block_all_keys = FALSE;

// read "g_flipbar_direction" from er_registry.txt
static gboolean g_flipbar_direction = TRUE;

// read from er_registry.txt
static gint g_keybuffering_setting = FALSE;

// when users enable keybuffering in "settings" app
// (here, means g_keybuffering_setting == TRUE),
// use 'g_enable_key_buffering' to control whether discard the keys pressed 
// after first key when busy or not
static gboolean g_enable_key_buffering = TRUE;

// for keybuffering timeout
static int g_timeout_handler_id = 0;

// internally used routines
#ifdef COMMAND_LINE_INPUT
static void *readFromNamedPipe(void *arg);
#else
static void *readFromButtons(void *arg);
static void *buttonStateMachine(void *arg);
static int button_get_timout(void);
static pthread_cond_t buttonCond;
static pthread_mutex_t buttonCondMutex;
static int protectedButton;
static struct timespec g_timoutValue;
#endif //COMMAND_LINE_INPUT

static void button_handler(int button, gpointer data);
static void default_button_handling(int button, gpointer data);
static void misc_state_button_handling(int button, gpointer data);
static void pincode_state_button_handling(int button, gpointer data);
static void langsel_button_handling(int button, gpointer data);

static gboolean button_get_flipbar_direction_setting(void);
static gboolean button_key_buffering_timeout(gpointer data);
static gint button_get_key_buffering_setting(void);

int button_init(gpointer data)
{

    pthread_t theThread;

#ifdef COMMAND_LINE_INPUT
    CL_LOGPRINTF("Command line input");

    // Create the thread that will read from the keyboard.
    if (pthread_create(&theThread, NULL, readFromNamedPipe, (void *)data) != 0)
    {
        CL_ERRORPRINTF("Could not create thread");
        return -1;
    }
    else
    {
        return 0;
    }
#else
    CL_LOGPRINTF("Button driver input");
    int timoutValue = button_get_timout();
    g_timoutValue.tv_sec = timoutValue / 1000;
    g_timoutValue.tv_nsec = (timoutValue % 1000) * 1000000;
    keybufferingSettingInit();
    flipbarDirectionSettingInit();
    protectedButton = -1;
    pthread_mutex_init(&buttonCondMutex, NULL);
    pthread_cond_init(&buttonCond, NULL);
    // Create the thread that will read from the button driver.
    if (pthread_create(&theThread, NULL, readFromButtons, (void *)data) != 0)
    {
        CL_ERRORPRINTF("Could not create thread");
        return -1;
    }
    if (pthread_create(&theThread, NULL, buttonStateMachine, (void *)data) != 0)
    {
        CL_ERRORPRINTF("Could not create thread");
        return -1;
    }

    return 0;
#endif //COMMAND_LINE_INPUT
}


void button_block_all_keys(gboolean do_block)
{
    g_block_all_keys = do_block;
}
//
// when need to enable/disable key buffering, call this function
// firstly, it sets the value of "g_enable_key_buffering" to parameter "enable" 
// sencondly, 
// if it's enable, adds timeout handler in order to when timeouted we need 
// enable key buffering again no matter whether the system is busy or not
// if it's disable, removes the timeout handler at the same time 
// 
void button_enable_key_buffering(gboolean enable)
{
    g_enable_key_buffering = enable;

    CL_LOGPRINTF("set g_enable_key_buffering =%d",  enable);        

    if  (TRUE == enable)
    {
        if (g_timeout_handler_id != 0)
        {
              g_source_remove(g_timeout_handler_id);
              g_timeout_handler_id = 0;
        }
    }
    else
    { 
         if  (g_timeout_handler_id == 0)
         {
             g_timeout_handler_id = g_timeout_add (KEYBUFFERING_OFF_TIMEOUT, 
                                          button_key_buffering_timeout, NULL);
         }
    }
}

static gboolean button_is_enable_key_buffering()
{
    return g_enable_key_buffering;
}


static gboolean button_key_buffering_timeout(gpointer data)
{
    CL_LOGPRINTF("enable key buffering again for timeout");         
    button_enable_key_buffering(TRUE);
    return TRUE;
}

#ifdef COMMAND_LINE_INPUT
static void *readFromNamedPipe(void *arg)
{
    char szBuffer[MAX_COMMAND_SIZE];
    int button;
    int pipe_fd;
    int res;
    int exit = 0;

    CL_LOGPRINTF("Enter");

    while (exit == 0)
    {
        pipe_fd = open(FIFO_NAME, O_RDONLY);

        CL_LOGPRINTF("%s open fd %d", FIFO_NAME, pipe_fd);

        if (pipe_fd != -1)
        {
            res = read(pipe_fd, szBuffer, MAX_COMMAND_SIZE);
            if (res > 0)
            {
                CL_LOGPRINTF("read %s [%d]", szBuffer, button);
                button = atoi(szBuffer);
                button_handler(button, arg);
            }
            close(pipe_fd);
        }
        else
        {
            // leave loop 
            CL_ERRORPRINTF("pipe_fd = -1");
            exit = 1;
        }
    }

    pthread_exit(NULL);
}
#else
/**
 * This thread will do a blocking read until a button is pressed or released;
 * store the button in protectedButton;
 * signal the buttonStateMachine thread
 */
static void *readFromButtons(void *arg)
{
    int status;
    int button_device;
    int ret;

    button_device = open("/dev/buttons", O_RDWR);
    if (button_device == -1)
    {
        CL_ERRORPRINTF("Error opening buttondriver.");
        exit(-1);
    }

    // Disable the activity led 
    ioctl(button_device, BUTTON_IOCTL_WRITE_ACTIVITY_LED, 0);

    // Scan keycode and detect status change
    int button;
    int button_old = 0xff;
    while (1)
    {
        usleep(100 * 1000L);
        ret = ioctl(button_device, BUTTON_IOCTL_GET_STATUS, &status);
        if (ret)
        {
            CL_ERRORPRINTF("ioctl returned 0x%x", ret);
        }
        else
        {
            button = GETBUTTON(status);
            if (button != button_old)
            {
                // Button has changed
                CL_LOGPRINTF("Button changed: new 0x%04X, old 0x%04X", button, button_old);
                button_old = button;
                pthread_mutex_lock(&buttonCondMutex);
                protectedButton = button;

                /* Check if a button was pressed or released */
                if (protectedButton == 0xff)
                {
                    CL_LOGPRINTF("ioctlthread: Button released");
                    if (ret)
                    {
                        CL_ERRORPRINTF("ioctl returned 0x%x", ret);
                    }
                }
                else if (protectedButton >= 0 && protectedButton < NUM_BUTTONS)
                {
                    CL_LOGPRINTF
                        ("ioctlthread: Button pressed: %d, Pen detect: %s",
                         protectedButton, GETPENDETECT(status) ? "Yes" : "No");
                    if (ret)
                    {
                        CL_ERRORPRINTF("ioctl returned 0x%x", ret);
                    }
                }
                else
                {
                    CL_LOGPRINTF("ioctlthread: button weirdness");
                }
                pthread_cond_signal(&buttonCond);
                pthread_mutex_unlock(&buttonCondMutex);
            }
        }
    }
    pthread_exit(NULL);
}

/**
 * This thread implements the long keypress state machine.
 * The state machine has two states:
 * 0: no button is currently pressed
 * 1: some button is currently pressed. the currently pressed button is stored in int button
 *
 * in state 0, we wait for a button event to proceed. when a press is received, we go to state 1
 * in state 1, we do a wait with a timeout of BUTTON_LONG_PRESS_TIMEOUT ms
 * if the wait returns with a timeout, it means the button is still pressed, and we handle it as a long press.
 * if the wait returns because of being signalled, it means a button event has happened. if this is a release,
 * we handle it as a short press.
 */
static void *buttonStateMachine(void *arg)
{
    int button;
    struct timespec times;
    struct timeval timev;

    enum
    {
        buttonReleased,
        buttonPressed
    } state = buttonReleased;

    button = -1;
    while (1)
    {
        CL_LOGPRINTF("smthread: start of loop, state %d", state);
        switch (state)
        {
            case buttonReleased:
                pthread_mutex_lock(&buttonCondMutex);
                CL_LOGPRINTF("smthread: waiting for press");
                pthread_cond_wait(&buttonCond, &buttonCondMutex);
                if (protectedButton == 0xFF)
                {
                    CL_LOGPRINTF("smthread: release in released");
                    pthread_mutex_unlock(&buttonCondMutex);
                }
                else
                {
                    CL_LOGPRINTF("smthread: press in released");
                    button = protectedButton;
                    state = buttonPressed;
                    pthread_mutex_unlock(&buttonCondMutex);
                }
                break;

            case buttonPressed:
                pthread_mutex_lock(&buttonCondMutex);
                // get current time, and add BUTTON_LONG_PRESS_TIMEOUT ms
                // a timeval -> timespec conversion is needed (nanosec to microsec)
                gettimeofday(&timev, NULL);
                times.tv_sec = timev.tv_sec + g_timoutValue.tv_sec;
                times.tv_nsec = (timev.tv_usec * 1000) + g_timoutValue.tv_nsec;
                if (times.tv_nsec >= 1000000000)
                {
                    times.tv_nsec -= 1000000000;
                    times.tv_sec += 1;
                }
                CL_LOGPRINTF("smthread: timed waiting for release");
                if (pthread_cond_timedwait(&buttonCond, &buttonCondMutex, &times) == ETIMEDOUT)
                {
                    CL_LOGPRINTF("smthread: timeout in pressed");
                    pthread_mutex_unlock(&buttonCondMutex);
                    if (button >= 0 && button < NUM_BUTTONS)
                    {
                        CL_LOGPRINTF("smthread: long button handler");
                        button_handler(button | BUTTON_LONG_PRESS, arg);        // long
                    }
                    button = -1;
                    state = buttonReleased;
                }
                else
                {
                    if (protectedButton == 0xFF)
                    {
                        CL_LOGPRINTF("smthread: release in pressed");
                        state = buttonReleased;
                        pthread_mutex_unlock(&buttonCondMutex);
                        if (button >= 0 && button < NUM_BUTTONS)
                        {
                            CL_LOGPRINTF("smthread: short button handler");
                            button_handler(button, arg);        // short
                        }
                        button = -1;
                    }
                    else
                    {
                        CL_LOGPRINTF("smthread: press in pressed");
                        button = protectedButton;
                        pthread_mutex_unlock(&buttonCondMutex);
                    }
                }
                break;
        }
    }
    pthread_exit(NULL);
}

static regUserSetting_t* btn_get_user_setting(void)
{
    return erRegGetUserSetting();
}

static int button_get_timout(void)
{
    regUserSetting_t *theUserSetting = NULL;
    int value = BUTTON_LONG_PRESS_TIMEOUT;

    theUserSetting = btn_get_user_setting();
    if ((theUserSetting) && (theUserSetting->longkeypressTimeout > 0))
    {
        value = theUserSetting->longkeypressTimeout;
        erRegFreeUserSetting(theUserSetting);
    }
    else
    {
        CL_WARNPRINTF("no reader settings");
    }

    CL_LOGPRINTF("timout %d", value);
    return value;
}

static gboolean button_get_flipbar_direction_setting(void)
{
    regUserSetting_t *theUserSetting = NULL;
    gboolean     value =  TRUE;
    
    theUserSetting = btn_get_user_setting();
    if (theUserSetting)
    {
        value = theUserSetting->flipbarLeftIsDown;
        erRegFreeUserSetting(theUserSetting);
    }
    else
    {
        CL_WARNPRINTF("no reader settings");
    }

    CL_LOGPRINTF("flipbarDirection %d", value);
    return value;
}

void flipbarDirectionSettingInit()
{
    // get the value of flipbar direction from the user settings
    g_flipbar_direction = button_get_flipbar_direction_setting();
}

static gint button_get_key_buffering_setting(void)
{
    regUserSetting_t *theUserSetting = NULL;
    gint     value =  0;

    theUserSetting = btn_get_user_setting();
    if (theUserSetting)
    {
        value = theUserSetting->keyBuffering;
        erRegFreeUserSetting(theUserSetting);
    }
    else
    {
        CL_WARNPRINTF("no reader settings");
    }

    CL_LOGPRINTF("keybuffering %d", value);
    return value;
}

void keybufferingSettingInit()
{
    // get the value of key buffering from the user settings
    g_keybuffering_setting = button_get_key_buffering_setting();
}

#endif //COMMAND_LINE_INPUT
static void button_handler(int button, gpointer data)
{    
    CL_LOGPRINTF("entry: button [0x%04X] state [%d]", button, getListerState());

    // users set "Flipbar right" as default, need to exchange the UP/DOWN button
    if (FALSE == g_flipbar_direction)
    { 
       if (PAGETURN_FORWARD == button)
       {
           button = PAGETURN_BACK;
       }
       else if (PAGETURN_BACK == button)
       {
           button = PAGETURN_FORWARD;
       }
       else if ((PAGETURN_FORWARD | BUTTON_LONG_PRESS) == button)
       {
           button = (PAGETURN_BACK | BUTTON_LONG_PRESS);
       }
       else if ((PAGETURN_BACK | BUTTON_LONG_PRESS) == button)
       {
           button = (PAGETURN_FORWARD | BUTTON_LONG_PRESS);
       }
    }

    // discard key as needed
    if (g_block_all_keys && button != (POWER) && button != (POWER | BUTTON_LONG_PRESS))
    {
        return;
    }

    // with key-lock enabled, click only on the first few keys but discard all keys
    static int keyclicks_when_locked = MAX_KEYCLICKS_WHEN_LOCKED;

    if (   toolbar_is_locked_selected()
        && button != (POWER                    )
        && button != (POWER | BUTTON_LONG_PRESS) )
    {
        // sound key-click as needed
        if (keyclicks_when_locked > 0)
        {
            --keyclicks_when_locked;
            click_key_discard();
        }

        // discard key
        return;
    }   

    // If g_keybuffering_setting > 0, all keys are passed to app in focus.    
    // If g_keybuffering_setting == 0
    // - Keyhandling gets blocked i.e thrown away
    //   - after first key when key handling is unblocked
    // - Keyhandling gets unblocked
    //   - after 1 second timeout
    //   - at a display update
    //   - when busy indicator is switched off
    if (0 == g_keybuffering_setting)
    {
        if (FALSE == button_is_enable_key_buffering() )
        {
            CL_LOGPRINTF("ingore key =%d\n",  button);
            click_key_discard();
            return;
        }
        button_enable_key_buffering(FALSE);
    }


    // key to be processed: reset key-click counter
    keyclicks_when_locked = MAX_KEYCLICKS_WHEN_LOCKED;

    // Button has been pressed: go back to full cpu
    // power management disabled for now, needs more testing
    //power_cpu_full();

#ifdef ENABLE_LOGGING
    // MvdW: Log this key
    erlog_event_button(button);
#endif /* ENABLE LOGGING */

    // sound key-click
    click_key();

    listerState_t lister_state = getListerState();
    if (lister_state == STATE_NORMAL)
    {
        if (listerIsErrorScreenVisible())
        {
            gdk_threads_enter();
            listerHideErrorScreen();
            gdk_threads_leave();
        }
        else
        {
            default_button_handling(button, data);
        }
    }
    else if (lister_state == STATE_USB_CONNECTED)
    {
        switch (button)
        {
            case POWER:
            case POWER | BUTTON_LONG_PRESS:
                // Powerdown button is the only button that works when connected via USB
                gdk_threads_enter();
                ctrl_shutdown();
                gdk_threads_leave();
                break;

            default:
                ctrl_usbmode_keypress();
        }
    }
    else if (lister_state == STATE_DOWNLOADMGR)
    {
        // downloadMgr is launched from pincode screen, user can't press any keys
        if (TRUE == ctrl_is_connect_from_pincode())
        {
            switch (button)
            {
                case OVERVIEW:
                case OVERVIEW | BUTTON_LONG_PRESS:
                case POWER:
                case POWER | BUTTON_LONG_PRESS:
                    CL_LOGPRINTF("OVERVIEW_BUTTON or POWER_BUTTON");
                    ctrl_disconnect(connectScrUnchanged);
                    break;

                default:
                    CL_WARNPRINTF("DownloadMgr state from pincode: buttons disabled");
                    erbusy_off();
            }
        }
        else
        {
            erbusy_blink();

            switch (button)
            {
                case OVERVIEW:
                case OVERVIEW | BUTTON_LONG_PRESS:
                    CL_LOGPRINTF("OVERVIEW_BUTTON");
                    gdk_threads_enter();
                    pm_SendKey(GDK_F5);
                    gdk_threads_leave();
                    break;

                case MODE:
                case MODE | BUTTON_LONG_PRESS:
                case POWER:
                case POWER | BUTTON_LONG_PRESS:
                    CL_LOGPRINTF("MODE_BUTTON or POWER_BUTTON");
                    ctrl_disconnect(connectScrMode);
                    break;

                case NEWSPAPER:
                case NEWSPAPER | BUTTON_LONG_PRESS:
                    CL_LOGPRINTF("MY_NEWSPAPERS_BUTTON");
                    ctrl_disconnect(connectScrNews);
                    break;

                case BOOKS:
                case BOOKS | BUTTON_LONG_PRESS:
                    CL_LOGPRINTF("MY_BOOKS_BUTTON");
                    ctrl_disconnect(connectScrBooks);
                    break;

                case DOCUMENTS:
                case DOCUMENTS | BUTTON_LONG_PRESS:
                    CL_LOGPRINTF("MY_DOCS_BUTTON");
                    ctrl_disconnect(connectScrDocs);
                    break;

                case NOTES:
                case NOTES | BUTTON_LONG_PRESS:
                    CL_LOGPRINTF("MY_NOTES_BUTTON");
                    ctrl_disconnect(connectScrNotes);
                    break;

                default:
                    CL_WARNPRINTF("DownloadMgr state: send key to connectionMgr");
                    default_button_handling(button, data);
                    erbusy_off();
            }
        }
    }
    else if (lister_state == STATE_MISC)
    {
        misc_state_button_handling(button, data);
    }
    else if (lister_state == STATE_PINCODE)
    {
        if (listerIsErrorScreenVisible())
        {
            gdk_threads_enter();
            listerHideErrorScreen();
            gdk_threads_leave();
        }
        else
        {
             pincode_state_button_handling(button, data);
        }
    }
    else if (lister_state == STATE_LANG_SEL)
    {
        if (listerIsErrorScreenVisible())
        {
            gdk_threads_enter();
            listerHideErrorScreen();
            gdk_threads_leave();
        }
        else
        {
            langsel_button_handling(button, data);
        }
    }
    else
    {
        CL_ERRORPRINTF("Unexpected lister_state [%d]", lister_state);
    }

    CL_LOGPRINTF("leave");
}

static void default_button_handling(int button, gpointer data)
{
    CL_LOGPRINTF("entry: button [0x%04X]", button);

    erbusy_blink();

    switch (button)
    {
        case NEWSPAPER:
        case NEWSPAPER | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("MY_NEWSPAPERS_BUTTON");
            gdk_threads_enter();
            ctrl_contentType_button_clicked(st_ContentTypeNews);
            gdk_threads_leave();
            break;

        case BOOKS:
        case BOOKS | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("MY_BOOKS_BUTTON");
            gdk_threads_enter();
            ctrl_contentType_button_clicked(st_ContentTypeBooks);
            gdk_threads_leave();
            break;

        case DOCUMENTS:
        case DOCUMENTS | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("MY_DOCS_BUTTON");
            gdk_threads_enter();
            ctrl_contentType_button_clicked(st_ContentTypeDocs);
            gdk_threads_leave();
            break;

        case NOTES:
        case NOTES | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("MY_NOTES_BUTTON");
            gdk_threads_enter();
            ctrl_contentType_button_clicked(st_ContentTypeNotes);
            gdk_threads_leave();
            break;

        case MODE:
            CL_LOGPRINTF("MODE_BUTTON");
            gdk_threads_enter();
            ctrl_mode_button_clicked();
            gdk_threads_leave();
            break;

        case MODE | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("MODE_BUTTON LONG");
            gdk_threads_enter();
            ctrl_mode_button_long_clicked();
            gdk_threads_leave();
            break;

        case PAGETURN_FORWARD:
            CL_LOGPRINTF("PAGETURN_FORWARD");
            gdk_threads_enter();
            pm_SendKey(GDK_Page_Down);
            gdk_threads_leave();
            break;

        case PAGETURN_FORWARD | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("PAGETURN_FORWARD LONG");
            gdk_threads_enter();
            pm_SendKey(GDK_F1);
            gdk_threads_leave();
            break;

        case PAGETURN_BACK:
            CL_LOGPRINTF("PAGETURN_BACK");
            gdk_threads_enter();
            pm_SendKey(GDK_Page_Up);
            gdk_threads_leave();
            break;

        case PAGETURN_BACK | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("PAGETURN_BACK LONG");
            gdk_threads_enter();
            pm_SendKey(GDK_F2);
            gdk_threads_leave();
            break;

        case OVERVIEW:
            CL_LOGPRINTF("OVERVIEW_BUTTON");
            gdk_threads_enter();
            pm_SendKey(GDK_F5);
            gdk_threads_leave();
            break;

        case OVERVIEW | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("OVERVIEW_BUTTON LONG");
            gdk_threads_enter();
            pm_SendKey(GDK_Home);
            gdk_threads_leave();
            break;

        case NAVIGATE_3:
            CL_LOGPRINTF("NEXT_LINK_BUTTON");
            gdk_threads_enter();
            pm_SendKey(GDK_Down);
            gdk_threads_leave();
            break;

        case NAVIGATE_3 | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("NEXT_LINK_BUTTON LONG");
            gdk_threads_enter();
            pm_SendKey(GDK_F3);
            gdk_threads_leave();
            break;

        case NAVIGATE_1:
            CL_LOGPRINTF("PREVIOUS_LINK_BUTTON");
            gdk_threads_enter();
            pm_SendKey(GDK_Up);
            gdk_threads_leave();
            break;

        case NAVIGATE_1 | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("PREVIOUS_LINK_BUTTON LONG");
            gdk_threads_enter();
            pm_SendKey(GDK_F4);
            gdk_threads_leave();
            break;

        case NAVIGATE_2:
            CL_LOGPRINTF("CONFIRM_BUTTON");
            gdk_threads_enter();
            pm_SendKey(GDK_Return);
            gdk_threads_leave();
            break;

        case NAVIGATE_2 | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("CONFIRM_BUTTON LONG");
            gdk_threads_enter();
            pm_SendKey(GDK_F6);
            gdk_threads_leave();
            break;

        case CONNECT:
            CL_LOGPRINTF("CONNECT_BUTTON");
            gdk_threads_enter();
            ctrl_display_download_history();
            gdk_threads_leave();
            break;

        case CONNECT | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("CONNECT_BUTTON LONG");
            gdk_threads_enter();
            ctrl_connect_button_clicked();
            gdk_threads_leave();
            break;

        case POWER:
        case POWER | BUTTON_LONG_PRESS:
            gdk_threads_enter();
            ctrl_shutdown();
            gdk_threads_leave();
            break;

        default:
            CL_ERRORPRINTF("unknown value %d", button);
            erbusy_off();
    }
}

// button handling when CL is in rename or search or sort state
static void misc_state_button_handling(int button, gpointer data)
{
    CL_LOGPRINTF("entry");

    int i;

    erbusy_blink();

    switch (button)
    {
        case OVERVIEW:
        case OVERVIEW | BUTTON_LONG_PRESS:
            gdk_threads_enter();
            ctrl_hide_misc_screen();
            gdk_threads_leave();
            break;
            
        case MODE:
        case MODE | BUTTON_LONG_PRESS:
        case NEWSPAPER:
        case NEWSPAPER | BUTTON_LONG_PRESS:
        case BOOKS:
        case BOOKS | BUTTON_LONG_PRESS:
        case DOCUMENTS:
        case DOCUMENTS | BUTTON_LONG_PRESS:
        case NOTES:
        case NOTES | BUTTON_LONG_PRESS:
        case POWER:
        case POWER | BUTTON_LONG_PRESS:
            default_button_handling(button, data);
            break;

        case PAGETURN_FORWARD:
        case PAGETURN_FORWARD | BUTTON_LONG_PRESS:
            gdk_threads_enter();
            if (dist_list_wnd_is_visible())
            {
                i = (button & BUTTON_LONG_PRESS) ? 5 : 1;
                dist_list_wnd_page_forward(i);
            }
            else
            {
                CL_WARNPRINTF("misc state: buttons disabled");
                erbusy_off();
            }
            gdk_threads_leave();
            break;

        case PAGETURN_BACK:
        case PAGETURN_BACK | BUTTON_LONG_PRESS:
            gdk_threads_enter();
            if (dist_list_wnd_is_visible())
            {
                i = (button & BUTTON_LONG_PRESS) ? 5 : 1;
                dist_list_wnd_page_back(i);
            }
            else
            {
                CL_WARNPRINTF("misc state: buttons disabled");
                erbusy_off();
            }
            gdk_threads_leave();
            break;

        default:
            CL_WARNPRINTF("misc state: buttons disabled");
            erbusy_off();
    }
}

static void pincode_state_button_handling(int button, gpointer data)
{
    erbusy_blink();
    
    switch (button)
    {
       case OVERVIEW:
            CL_LOGPRINTF("OVERVIEW_BUTTON");
            gdk_threads_enter();
            ctrl_pincode_overview_button_clicked();
            gdk_threads_leave();
            break;

       case CONNECT | BUTTON_LONG_PRESS:
            CL_LOGPRINTF("CONNECT_BUTTON LONG");
            gdk_threads_enter();
            ctrl_pincode_connect_button_clicked();
            gdk_threads_leave();
            break;

        case POWER:
        case POWER | BUTTON_LONG_PRESS:
            gdk_threads_enter();
            ctrl_shutdown();
            gdk_threads_leave();
            break;

        default:
            CL_WARNPRINTF("Pincode state: buttons(%d) disabled", button);
            ctrl_pincode_button_clicked();
    }
}

static void langsel_button_handling(int button, gpointer data)
{
    CL_LOGPRINTF("entry: button [0x%04X]", button);

    erbusy_blink();

    switch (button)
    {
        case OVERVIEW: 
        case OVERVIEW | BUTTON_LONG_PRESS:
        case NAVIGATE_3:
        case NAVIGATE_3 | BUTTON_LONG_PRESS:
        case NAVIGATE_1:
        case NAVIGATE_1 | BUTTON_LONG_PRESS:
        case NAVIGATE_2:
        case NAVIGATE_2 | BUTTON_LONG_PRESS:
        case POWER:
        case POWER | BUTTON_LONG_PRESS:
            default_button_handling(button, data);
            break;
        default:
            CL_ERRORPRINTF("unknown value %d", button);
            erbusy_off();
            break;
    }
}


