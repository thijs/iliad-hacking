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
 *
 */

#ifndef __ERROR_SCREEN_H__
#define __ERROR_SCREEN_H__

#include <gtk/gtk.h>
#include <gdk/gdk.h>

typedef enum
{
    ctrlNoError = 0,
    ctrlScanTooManyItemsError,
    ctrlScanFailedError,
    ctrlContentNotAvailableError,
    ctrlUnkownError,
    ctrlNoNetworkConfiguredError,
    ctrlConnectedUSBError,
    ctrlExportingDevUnavailError,
    ctrlBatteryLowError,
    ctrlSystemShutdownError
} ctrlErrorType_e;

typedef enum
{
    ctrlWarningIcon = 0,
    ctrlErrorIcon,
    ctrlInfoIcon,
    ctrlProhibitedIcon,
    ctrlDontDisconnectNWIcon,
    ctrlBatteryLowIcon,
    ctrlSystemShutdownIcon,
    ctrlLastIcon                
} ctrlErrorIcon_e;

#define BOTTOM_TOP_BORDER               70
#define VBOX_TEXT_IMAGE_SPACING         50
#define VBOX_BORDER                     20
#define ERROR_IMAGE_MIN_WIDTH          400
#define ERROR_IMAGE_MIN_HEIGHT         350
#define ERROR_DESCRIPTION_WIDTH        600

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

#define GTK_ERROR_SCREEN_TYPE                 (gtk_error_screen_get_type ())
#define GTK_ERROR_SCREEN(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_ERROR_SCREEN_TYPE, GtkErrorScreen))
#define GTK_ERROR_SCREEN_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_ERROR_SCREEN_TYPE, GtkErrorScreenClass))
#define IS_GTK_ERROR_SCREEN(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_ERROR_SCREEN_TYPE))
#define IS_GTK_ERROR_SCREEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_ERROR_SCREEN_TYPE))

typedef struct _GtkErrorScreen GtkErrorScreen;
typedef struct _GtkErrorScreenClass GtkErrorScreenClass;

struct _GtkErrorScreen
{
    GtkEventBox box;        //container
    GtkWidget *text;
    GtkWidget *image;
    GdkPixbuf *pixmap;      //to check image retrieved from file before displaying something
};

struct _GtkErrorScreenClass
{
    GtkEventBoxClass parent_class;
};

/**
 * creates a new GtkErrorScreen widget
 *
 * @param - 
 *
 * @returns reference to created widget
 */
GtkWidget *gtk_error_screen_new();


/**
 * returns type of GtkErrorScreen widget
 *
 * @returns type
 */
GType gtk_error_screen_get_type(void);

/**
 * Set text of GtkErrorScreen widget
 *
 * @param item the GtkListItem widget
 * @param text
 *
 * @returns -
 */
void gtk_error_screen_set_text(GtkErrorScreen *screen, const gchar * text);

/**
 * Set thumb of GtkErrorScreen widget
 *
 * @param item the GtkErrorScreen widget
 * @param location of the image
 *
 * @returns -
 */
void gtk_error_screen_set_image(GtkErrorScreen *error_screen, ctrlErrorIcon_e icon);



#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__ERROR_SCREEN_H__
