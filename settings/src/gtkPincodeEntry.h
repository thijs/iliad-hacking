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
 * \file gtkPincodeEntry.h
 * \brief content lister - Input Pincode Entry
 *
 * override the standard GtkEntry
 *
 * input pincode entry, limited between 4-8 digital numbers
 * characters are shown as '********'
 * call displayMgr to show the user's input to user on eink screen
 * 
 */

#ifndef __GTK_PINCODE_ENTRY_H__
#define __GTK_PINCODE_ENTRY_H__
#ifdef __cplusplus
extern "C"
{
#endif

#define PINCODE_UI_MIN_LENGTH 4
#define PINCODE_UI_MAX_LENGTH 4    

#define PINCODE_INPUT_ENTRY_WIDTH 150   // value from the designed screen
#define PINCODE_INPUT_ENTRY_HEIGHT 30

#define GTK_PINCODE_ENTRY_TYPE                 (gtk_pincode_entry_get_type ())
#define GTK_PINCODE_ENTRY(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_PINCODE_ENTRY_TYPE, GtkPincodeEntry))
#define GTK_PINCODE_ENTRY_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_PINCODE_ENTRY_TYPE, GtkPincodeEntryClass))
#define IS_GTK_PINCODE_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_PINCODE_ENTRY_TYPE))
#define IS_GTK_PINCODE_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_PINCODE_ENTRY_TYPE))

typedef struct _GtkPincodeEntry GtkPincodeEntry;
typedef struct _GtkPincodeEntryClass GtkPincodeEntryClass;

struct _GtkPincodeEntry
{
    erGtkEntry  parent;
};

struct _GtkPincodeEntryClass
{
    erGtkEntryClass  parent_class;
};

/**
 * creates a new GtkPincodeEntry widget
 *
 * @param - 
 *
 * @returns reference to created widget
 */
GtkWidget *gtk_pincode_entry_new();

/**
 * returns type of GtkPincodeEntry widget
 *
 * @returns type
 */
GType gtk_pincode_entry_get_type(void);

/**
 * 
 * check user input is valid or not, should be 4-8 digital numbers
 * @param pincode_entry the GtkPincodeEntry widget
 *
 * @returns -TRUE valid pincode string, FALSE invalid pincode string
 */
gboolean gtk_pincode_entry_check_text(GtkPincodeEntry* pincode_entry);


#ifdef __cplusplus
}
#endif

#endif                          /* __GTK_PINCODE_ENTRY_H__ */
