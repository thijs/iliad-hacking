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
 * \file gtkErrorScreen.h
 * \brief error screen - GTK widget used to inform the user an error has occured
 *                       this widget contains an image and some explenation text 
 * 
 * <File description>
 * 
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#include <sys/types.h>

#include <glib.h>
#include <gdk/gdkkeysyms.h>

#include <liberdm/display.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "gtkErrorScreen.h"
#include "displayUpdate.h"
#include "stack.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"

static GdkPixbuf *g_error_images[ctrlLastIcon];

//callbacks
gboolean error_screen_expose_event(GtkWidget * widget, GdkEventExpose * event, gpointer data);
static void on_error_screen_destroy(GtkWidget * widget, gpointer data);
guint   error_screen_keypress(GtkWidget * widget, GdkEventKey * event, gpointer data);

// internally used routines
static void gtk_error_screen_class_init(GtkErrorScreenClass * klass);
static void gtk_error_screen_init(GtkErrorScreen * error_screen);

static void error_screen_init_images(void);
static void error_screen_destroy_images(void);
GdkPixbuf *error_screen_get_image(unsigned int error_ID);

// creates a new GtkErrorScreen widget
GtkWidget *gtk_error_screen_new()
{
    GtkErrorScreen *errorScreen;

    errorScreen = (GtkErrorScreen *) g_object_new(GTK_ERROR_SCREEN_TYPE, NULL);

    CL_LOGPRINTF("error screen 0x%x", (unsigned int) errorScreen);

    return GTK_WIDGET(errorScreen);
}

// returns type of this widget 
GType gtk_error_screen_get_type(void)
{
    static GType error_screen_type = 0;

    if (!error_screen_type)
    {
        static const GTypeInfo error_screen_info = {
            sizeof(GtkErrorScreenClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) gtk_error_screen_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(GtkErrorScreen),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) gtk_error_screen_init,
        };

        error_screen_type = g_type_register_static(GTK_TYPE_EVENT_BOX, "ErrorScreen", &error_screen_info, 0);
    }
    return error_screen_type;
}

// init class
static void gtk_error_screen_class_init(GtkErrorScreenClass * klass)
{
    // nothing needs to be done here 
}

// create the composite widget - set the default values 
//
// object hierarchy:
//     error_screen (GtkErrorScreen)
//       |-- topLevelVBox (GtkVBox)
//             |-- vbox (GtkVBox)
//                   |-- error_screen->image (GtkImage)
//                   |-- error_screen->text (GtkLabel)
// 
static void gtk_error_screen_init(GtkErrorScreen * error_screen)
{
    GtkWidget *vbox;
    GtkWidget *topLevelVBox;

    error_screen_init_images();

    // error_screen
    gtk_widget_set_name(GTK_WIDGET(error_screen), "error_screen_background");

    // topLevelVBox - not homogenous, zero spacing, for bottom/top border
    topLevelVBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(error_screen), topLevelVBox);

    // vbox
    vbox = gtk_vbox_new(TRUE, VBOX_TEXT_IMAGE_SPACING);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), VBOX_BORDER);
    gtk_box_pack_start(GTK_BOX(topLevelVBox), vbox, FALSE, FALSE, BOTTOM_TOP_BORDER);

    // error_screen->image
    error_screen->image = gtk_image_new();
    gtk_widget_set_size_request(GTK_WIDGET(error_screen->image), ERROR_IMAGE_MIN_WIDTH, ERROR_IMAGE_MIN_HEIGHT);
    gtk_box_pack_start(GTK_BOX(vbox), error_screen->image, FALSE, FALSE, 0);
    error_screen->pixmap = NULL;

    // error_screen->text
    error_screen->text = gtk_label_new(NULL);
    gtk_widget_set_name(GTK_WIDGET(error_screen->text), "error_screen_text");
    gtk_widget_set_size_request(GTK_WIDGET(error_screen->text), ERROR_DESCRIPTION_WIDTH, (-1));
    gtk_misc_set_alignment(GTK_MISC(error_screen->text), 0.5, 0.0);
    gtk_label_set_single_line_mode(GTK_LABEL(error_screen->text), FALSE);
    gtk_label_set_line_wrap(GTK_LABEL(error_screen->text), TRUE);
    gtk_label_set_justify(GTK_LABEL(error_screen->text), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), error_screen->text, FALSE, FALSE, 0);

    // signal handlers
    g_signal_connect(G_OBJECT(error_screen), "key_press_event", G_CALLBACK(error_screen_keypress), NULL);
    g_signal_connect(G_OBJECT(error_screen), "expose-event", G_CALLBACK(error_screen_expose_event), NULL);
    g_signal_connect(G_OBJECT(error_screen), "destroy", G_CALLBACK(on_error_screen_destroy), NULL);

    gtk_widget_show_all(GTK_WIDGET(topLevelVBox));
}

static void on_error_screen_destroy(GtkWidget * widget, gpointer data)
{
    GtkErrorScreen *error_screen = (GtkErrorScreen *) widget;

    CL_WARNPRINTF("entry");

    // unref pixbuf object
    gtk_image_set_from_pixbuf(GTK_IMAGE(error_screen->image), NULL);

    error_screen_destroy_images();
}


guint error_screen_keypress(GtkWidget * widget, GdkEventKey * event, gpointer data)
{
    int     returnValue = 0;    // return FALSE => default gtk handling

    CL_LOGPRINTF("receiving keypress 0x%x", event->keyval);

    switch (event->keyval)
    {
    case GDK_Page_Down:
    case GDK_F1:
    case GDK_Page_Up:
    case GDK_F2:
    case GDK_Home:
    case GDK_F5:
    case GDK_Return:
    case GDK_Up:
    case GDK_F4:
    case GDK_Down:
    case GDK_F3:
        CL_SCREENPRINTF("invalid key press");
        display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL);
        break;

#ifdef COMMAND_LINE_INPUT
    case GDK_F12:
        // test case 
        ctrl_contentType_button_clicked(st_ContentTypeBooks);
        break;
    case GDK_F11:
        // test case 
        ctrl_contentType_button_clicked(st_ContentTypeNews);
        break;
    case GDK_F10:
        // test case 
        ctrl_contentType_button_clicked(st_ContentTypeDocs);
        break;
    case GDK_F9:
        // test case 
        ctrl_contentType_button_clicked(st_ContentTypeNotes);
        break;
    case GDK_F8:
        // test case 
        ctrl_mode_button_clicked();
        break;
#endif //COMMAND_LINE_INPUT
    }
    return returnValue;
}


void gtk_error_screen_set_text(GtkErrorScreen * error_screen, const gchar * text)
{
    g_return_if_fail(IS_GTK_ERROR_SCREEN(error_screen));
    g_return_if_fail(text != NULL);

    CL_LOGPRINTF("text %s", text);

    gtk_label_set_text(GTK_LABEL(error_screen->text), text);
}

void gtk_error_screen_set_image(GtkErrorScreen * error_screen, ctrlErrorIcon_e icon)
{
    g_return_if_fail(IS_GTK_ERROR_SCREEN(error_screen));

    error_screen->pixmap = error_screen_get_image(icon);
    gtk_image_set_from_pixbuf(GTK_IMAGE(error_screen->image), error_screen->pixmap);
    //gtk_widget_show(GTK_WIDGET(error_screen->image));
    return;
}


gboolean error_screen_expose_event(GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
    CL_SCREENPRINTF("entry");
    display_update_request_screen_refresh(LISTER_EXPOSE_LEVEL);
    return FALSE;
}

static void error_screen_init_images(void)
{
    GError *error = NULL;

    g_error_images[ctrlWarningIcon] = gdk_pixbuf_new_from_file(DATA_DIR "/Warning.png", &error);
    if (NULL == g_error_images[ctrlWarningIcon])
    {
        CL_ERRORPRINTF("ctrlWarningIcon - error %s", error->message);
    }

    g_error_images[ctrlInfoIcon] = gdk_pixbuf_new_from_file(DATA_DIR "/Info.png", &error);
    if (NULL == g_error_images[ctrlInfoIcon])
    {
        CL_ERRORPRINTF("ctrlInfoIcon - error %s", error->message);
    }

    g_error_images[ctrlErrorIcon] = gdk_pixbuf_new_from_file(DATA_DIR "/Error.png", &error);
    if (NULL == g_error_images[ctrlErrorIcon])
    {
        CL_ERRORPRINTF("ctrlErrorIcon - error %s", error->message);
    }

    g_error_images[ctrlProhibitedIcon] = gdk_pixbuf_new_from_file(DATA_DIR "/Prohibited.png", &error);
    if (NULL == g_error_images[ctrlProhibitedIcon])
    {
        CL_ERRORPRINTF("ctrlProhibitedIcon - error %s", error->message);
    }

    g_error_images[ctrlDontDisconnectNWIcon] = gdk_pixbuf_new_from_file(DATA_DIR "/DontDisconnect-NW.png", &error);
    if (NULL == g_error_images[ctrlDontDisconnectNWIcon])
    {
        CL_ERRORPRINTF("ctrlDontDisconnectNWIcon - error %s", error->message);
    }

    g_error_images[ctrlBatteryLowIcon] = gdk_pixbuf_new_from_file(DATA_DIR "/BatteryLow.png", &error);
    if (NULL == g_error_images[ctrlBatteryLowIcon])
    {
        CL_ERRORPRINTF("ctrlBatteryLowIcon - error %s", error->message);
    }

    g_error_images[ctrlSystemShutdownIcon] = gdk_pixbuf_new_from_file(DATA_DIR "/Shutdown.png", &error);
    if (NULL == g_error_images[ctrlSystemShutdownIcon])
    {
        CL_ERRORPRINTF("ctrlSystemShutdownIcon - error %s", error->message);
    }
}

static void error_screen_destroy_images(void)
{
    int     index;

    CL_LOGPRINTF("entry");

    for (index = 0; index < ctrlLastIcon; index++)
    {
        if (g_error_images[index])
        {
            g_free(g_error_images[index]);
            g_error_images[index] = NULL;
        }
    }
}

GdkPixbuf *error_screen_get_image(unsigned int error_ID)
{
    if ((error_ID < ctrlLastIcon))
    {
        return g_error_images[error_ID];
    }
    else
    {
        return NULL;
    }
}
