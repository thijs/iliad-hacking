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
 * \file gtkPincodeScreen.h
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
 * Copyright (C) 2006-2008 iRex Technologies B.V.
 * 
 */

#ifndef __GTK_PINCODE_SCREEN_H__
#define __GTK_PINCODE_SCREEN_H__
#ifdef __cplusplus
extern "C"
{
#endif

#define  MAX_TRY_PINCODE_TIMES 3

#define PINCODE_SCREEN_WIDTH 768// the value from png files
#define PINCODE_SCREEN_HEIGHT 1024

#define PINCODE_BACKGROUND_WIDTH  707
#define PINCODE_BACKGROUND_HEIGHT 347

#define PINCODE_SCREEN_PADDING_LEFT  (PINCODE_SCREEN_WIDTH - PINCODE_BACKGROUND_WIDTH)/2
#define PINCODE_SCREEN_PADDING_RIGHT  PINCODE_SCREEN_PADDING_LEFT
#define PINCODE_SCREEN_PADDING_BOTTOM 35
#define PINCODE_SCREEN_PADDING_TOP    (PINCODE_SCREEN_HEIGHT-PINCODE_BACKGROUND_HEIGHT-PINCODE_SCREEN_PADDING_BOTTOM)

#define PINCODE_INFO_LABEL_WIDTH   PINCODE_BACKGROUND_WIDTH
#define PINCODE_INFO_LABEL_HEIGHT  30
#define PINCODE_MESSAGE_LABEL_WIDTH  PINCODE_BACKGROUND_WIDTH
#define PINCODE_MESSAGE_LABEL_HEIGHT (PINCODE_BACKGROUND_HEIGHT - PINCODE_INFO_LABEL_HEIGHT-15)

#define PINCODE_INPUT_LABEL_WIDTH   PINCODE_BACKGROUND_WIDTH
#define PINCODE_INPUT_LABEL_HEIGHT  99
#define PINCODE_STAR_WIDTH   16
#define PINCODE_STAR_HEIGHT  16
#define PINCODE_STAR_SPACING  5
#define PINCODE_KEYBOARD_WIDTH    447
#define PINCODE_KEYBOARD_HEIGHT   49
#define PINCODE_BUTTON_WIDTH 100
#define PINCODE_BUTTON_HEIGHT 27

#define PINCODE_INPUT_LABEL_XPOS 0
#define PINCODE_INPUT_LABEL_YPOS 0
#define PINCODE_STARS_XPOS ((PINCODE_BACKGROUND_WIDTH-PINCODE_STAR_WIDTH*4-PINCODE_STAR_SPACING*3)/2)
#define PINCODE_STARS_YPOS (115-PINCODE_STAR_HEIGHT)
#define PINCODE_KEYBOARD_XPOS ((PINCODE_BACKGROUND_WIDTH-PINCODE_KEYBOARD_WIDTH)/2)
#define PINCODE_KEYBOARD_YPOS (200-PINCODE_KEYBOARD_HEIGHT)
#define PINCODE_BUTTON_XPOS ((PINCODE_BACKGROUND_WIDTH-PINCODE_BUTTON_WIDTH)/2)
#define PINCODE_BUTTON_YPOS (260-PINCODE_BUTTON_HEIGHT)
#define PINCODE_INFO_LABEL_XPOS 0
#define PINCODE_INFO_LABEL_YPOS (PINCODE_BACKGROUND_HEIGHT-PINCODE_INFO_LABEL_HEIGHT-15)

#define GTK_PINCODE_SCREEN_TYPE                 (gtk_pincode_screen_get_type ())
#define GTK_PINCODE_SCREEN(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_PINCODE_SCREEN_TYPE, GtkPincodeScreen))
#define GTK_PINCODE_SCREEN_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_PINCODE_SCREEN_TYPE, GtkPincodeScreenClass))
#define IS_GTK_PINCODE_SCREEN(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_PINCODE_SCREEN_TYPE))
#define IS_GTK_PINCODE_SCREEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_PINCODE_SCREEN_TYPE))

typedef void on_passed_callback_t(void);
typedef void on_cancel_callback_t(void);

typedef enum
{
    startup_t=0,
    lockScreen_t,
    signDocument_t,
    enterSettings_t,
    importSettings_t,
    undefinedReason_t
}showPincodeReason_t;

typedef struct _pincodeStars pincodeStars;

struct _pincodeStars
{
  GtkWidget* hbox;
  GtkWidget* vbox1;
  GtkWidget* grayStar1;
  GtkWidget* whiteStar1;
  GtkWidget* vbox2;
  GtkWidget* grayStar2;
  GtkWidget* whiteStar2;
  GtkWidget* vbox3;
  GtkWidget* grayStar3;
  GtkWidget* whiteStar3;
  GtkWidget* vbox4;
  GtkWidget* grayStar4;
  GtkWidget* whiteStar4;
  gchar pincode[PINCODE_MAX_LENGTH+1];
  gboolean on_change_occurred;
  gboolean display_update_pending;
};

typedef struct _GtkPincodeScreen GtkPincodeScreen;
typedef struct _GtkPincodeScreenClass GtkPincodeScreenClass;

struct _GtkPincodeScreen
{
    GtkEventBox box;        // container and background
//    GtkWidget *image;     // TODO: background image, user can change this image in settings
//    GdkPixbuf *pixmap;
    GtkWidget *clientArea;// GtkVBox
    GtkWidget *inputWidget;// input widget
    GtkWidget *messageWidget;// message widget
    GtkWidget *inputLabel;// GtkLabel
    pincodeStars *stars;// pincodeStars
    GtkWidget *keyboard;// GtkEventBox
    GtkWidget *confirmButton;// GtkToggleButton
    GtkWidget *inputInfoLabel;// GtkLabel
    GtkWidget *messageInfoLabel;// GtkLabel
    GtkWidget *messageLabel;// GtkLabel
    showPincodeReason_t reason;// show pincode screen for what purpose
    on_passed_callback_t* on_passed_callback;// pincode passed callback func
    on_cancel_callback_t* on_cancel_callback;// cancel(leave) pincode screen
    gboolean pincodeOn;// pincode on/off from sysset
    gchar pincode[PINCODE_MAX_LENGTH+1];// pincode value from syset
    guint     tryTimes;// store user tried times already
    gboolean  passed;// store pincode passed or not
    gboolean  isVisible; // pincode screen visible or not
};

struct _GtkPincodeScreenClass
{
    GtkEventBoxClass parent_class;
};

/**
 * creates a new GtkPincodeScreen widget
 *
 * @param - 
 *
 * @returns reference to created widget
 */
GtkWidget *gtk_pincode_screen_new();


/**
 * returns type of GtkPincodeScreen widget
 *
 * @returns type
 */
GType gtk_pincode_screen_get_type(void);

/**
 * reset screen layout, such as, reset all stars to be grey, set text
 *
 * @param screen the GtkPincodeScreen widget
 * @param  
 *
 * @returns -
 */

void gtk_pincode_screen_layout_reset(GtkWidget *screen);
/**
 * reset screen->passed to be FALSE
 *
 * @param screen the GtkPincodeScreen widget
 * @param  
 *
 * @returns -
 */
void gtk_pincode_screen_data_reset(GtkWidget *screen);


/**
 * set screen->pincodeOn, set screen->pincode
 *
 * @param screen the GtkPincodeScreen widget
 * @param  on
 * @param  pincode
 *
 * @returns -
 */
void gtk_pincode_screen_set_pincode_settings(GtkWidget *screen, gboolean on, const gchar*pincode);

/**
 * get screen->pincodeOn, get screen->pincode
 *
 * @param screen the GtkPincodeScreen widget
 * @param on 
 * @param pincode
 *
 * @returns -
 */
void gtk_pincode_screen_get_pincode_settings(GtkWidget *screen, gboolean *on, gchar*pincode);

/**
 * set screen->reason
 *
 * @param screen the GtkPincodeScreen widget
 * @param reason
 *
 * @returns -
 */
void gtk_pincode_screen_set_reason(GtkWidget *screen, showPincodeReason_t reason);

/**
 * get screen->reason
 *
 * @param screen the GtkPincodeScreen widget
 *
 * @returns -reason
 */
showPincodeReason_t gtk_pincode_screen_get_reason(GtkWidget* screen);

/**
 * set screen->on_passed_callback
 *
 * @param screen the GtkPincodeScreen widget
 * @param func
 *
 * @returns -
 */
void gtk_pincode_screen_set_on_passed_callback(GtkWidget *screen, on_passed_callback_t *func);

/**
 * set screen->on_cancel_callback
 *
 * @param screen the GtkPincodeScreen widget
 * @param func
 *
 * @returns -
 */
void gtk_pincode_screen_set_on_cancel_callback(GtkWidget *screen, on_cancel_callback_t *func);

/**
 * run screen->on_passed_callback
 *
 * @param screen the GtkPincodeScreen widget
 *
 * @returns -
 */
void gtk_pincode_screen_on_cancel_callback(GtkWidget *screen);

#ifdef __cplusplus
}
#endif

#endif /* __GTK_PINCODE_SCREEN_H__ */
