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
 * \file erGtkToggleButton.h
 * \brief ereader gtk library - GtkToggleButton object adapted for ereader system
 *
 * Changes colour of the button's child object as a fucntion of the button state.
 * 
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 */

#ifndef __ERGTK_TOGGLE_BUTTON_H__
#define __ERGTK_TOGGLE_BUTTON_H__
#ifdef __cplusplus
extern "C"
{
#endif


#define ERGTK_TOGGLE_BUTTON_DEFAULT_WIDTH  130 
#define ERGTK_TOGGLE_BUTTON_DEFAULT_HEIGHT  27

#define ERGTK_TOGGLE_BUTTON_TYPE                (ergtk_toggle_button_get_type())
#define ERGTK_TOGGLE_BUTTON(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), ERGTK_TOGGLE_BUTTON_TYPE, erGtkToggleButton))
#define ERGTK_IS_TOGGLE_BUTTON(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), ERGTK_TOGGLE_BUTTON_TYPE))

#define ERGTK_TOGGLE_BUTTON_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST((klass),  ERGTK_TOGGLE_BUTTON_TYPE, erGtkToggleButtonClass))
#define ERGTK_TOGGLE_BUTTON_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj),  ERGTK_TOGGLE_BUTTON_TYPE, erGtkToggleButtonClass))
#define ERGTK_IS_TOGGLE_BUTTON_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE((klass),  ERGTK_TOGGLE_BUTTON_TYPE))


typedef struct _erGtkToggleButton      erGtkToggleButton;
typedef struct _erGtkToggleButtonClass erGtkToggleButtonClass;

struct _erGtkToggleButton
{
    GtkToggleButton parent;     // parent class

    // private:
};

struct _erGtkToggleButtonClass
{
    GtkToggleButtonClass parent_class;

    // virtual methods
    
    // signal handlers
    void (*screen_refresh)     (erGtkToggleButton* er_toggleButton);
};


/**
 * create a new erGtkToggleButton widget
 *
 * @param - 
 *
 * @returns reference to created widget
 */
GtkWidget* ergtk_toggle_button_new();
GtkWidget* ergtk_toggle_button_new_with_label(const gchar *label);

/**
 * get type of erGtkToggleButton widget
 *
 * @returns type
 */
GType ergtk_toggle_button_get_type(void);


#ifdef __cplusplus
}
#endif

#endif  // __ERGTK_TOGGLE_BUTTON_H__
