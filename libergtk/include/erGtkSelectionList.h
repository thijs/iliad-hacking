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
 * \file erGtkSelectionList.h
 * \brief ereader gtk library - display list of button that the user can select/deselect.
 *
 * Ensure only the allowed number of buttons is selected at any time.
 * Note: no relation with the standard GTK class GtkSelection.
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 */

#ifndef __ERGTK_SELECTION_LIST_H__
#define __ERGTK_SELECTION_LIST_H__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define ERGTK_SELECTION_LIST_H_PADDING                13
#define ERGTK_SELECTION_LIST_V_PADDING                10

#define ERGTK_SELECTION_LIST_TITLE_HEIGHT             27
#define ERGTK_SELECTION_LIST_TITLE_SPACING             5
#define ERGTK_SELECTION_LIST_TITLE_BACKGROUND_STYLE  "erGtkSelectionList_TitleBackground"
#define ERGTK_SELECTION_LIST_TITLE_TEXT_STYLE        "erGtkSelectionList_TitleText"

#define ERGTK_SELECTION_LIST_BACKGROUND_STYLE        "erGtkSelectionList_Background"

#define ERGTK_SELECTION_LIST_INSTRUCTION_HEIGHT       29
#define ERGTK_SELECTION_LIST_INSTRUCTION_STYLE       "erGtkSelectionList_Instruction"

#define ERGTK_SELECTION_LIST_BUTTON_H_SPACING         13
#define ERGTK_SELECTION_LIST_BUTTON_V_SPACING          7

#define ERGTK_SELECTION_LIST_CHECKBUTTON_HEIGHT       29
#define ERGTK_SELECTION_LIST_CHECKBUTTON_STYLE       "erGtkSelectionList_CheckButton"
    
#define ERGTK_SELECTION_LIST_RADIOBUTTON_HEIGHT       22
#define ERGTK_SELECTION_LIST_RADIOBUTTON_STYLE       "erGtkSelectionList_RadioButton"

#define ERGTK_SELECTION_LIST_TEXTBUTTON_WIDTH        150
#define ERGTK_SELECTION_LIST_TEXTBUTTON_HEIGHT        27
#define ERGTK_SELECTION_LIST_TEXTBUTTON_STYLE        "erGtkSelectionList_TextButton"

#define ERGTK_SELECTION_LIST_CONTINUATION_H_PADDING  5
#define ERGTK_SELECTION_LIST_CONTINUATION_TEXT       "..."
#define ERGTK_SELECTION_LIST_CONTINUATION_STYLE      "erGtkSelectionList_Continuation"

    
#define ERGTK_SELECTION_LIST_TYPE             (ergtk_selection_list_get_type())
#define ERGTK_SELECTION_LIST(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), ERGTK_SELECTION_LIST_TYPE, erGtkSelectionList))
#define ERGTK_IS_SELECTION_LIST(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), ERGTK_SELECTION_LIST_TYPE))

#define ERGTK_SELECTION_LIST_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass),  ERGTK_SELECTION_LIST_TYPE, erGtkSelectionListClass))
#define ERGTK_SELECTION_LIST_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), ERGTK_SELECTION_LIST_TYPE, erGtkSelectionListClass))
#define ERGTK_IS_SELECTION_LIST_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass),  ERGTK_SELECTION_LIST_TYPE))

typedef enum
        {
            esl_Checklist,
            esl_RadioButton,
            esl_TextButton
        } erGtkSelectionList_displayAs_e;

typedef struct _erGtkSelectionList       erGtkSelectionList;
typedef struct _erGtkSelectionListClass  erGtkSelectionListClass;

struct _erGtkSelectionList
{
    erGtkSelectionGroup parent;

    // private
    gboolean            dispose_has_run;

    erGtkSelectionList_displayAs_e  display_as;

    GtkWidget*          titlebar;
};

struct _erGtkSelectionListClass
{
    erGtkSelectionGroupClass    parent_class;

    // virtual methods

    // signal handlers
};


/**
 * Create a new erGtkSelectionList object
 *
 * @param in  display_as: layout of selection list object
 * @param in  title: text to be displayed in the object's title bar
 * @param in  instruction: text to be displayed above the selection items
 * @param in  item_names: NULL-terminated array of strings to be displayed on the selection items
 * @param in  max_height: maximum height of the object, measured in pixels
 * @param out allocated_items: number of selection items displayed in the new list
 * @param out allocated_height: number of pixels required to display the new list
 *
 * @returns reference to created widget
 */
GtkWidget* ergtk_selection_list_new(const erGtkSelectionList_displayAs_e display_as,
                                    const gchar*  title,
                                    const gchar*  instruction,
                                    const gchar** item_names,
                                    const guint   max_height,
                                          guint*  allocated_items,
                                          guint*  allocated_height);

/**
 * Create a new erGtkSelectionList object as extension from another (master) object
 *
 * @param in  master: the master erGtkSelectionList object
 * @param in  first_item: index (0 ..) of first item displayed
 * @param in  max_height: maximum height of the object, measured in pixels
 * @param out allocated_items: number of selection items displayed in the new list
 * @param out allocated_height: number of pixels required to display the new list
 *
 * @returns reference to created widget
 */
GtkWidget* ergtk_selection_list_new_from_master(const erGtkSelectionList* master,
                                                const guint   first_item,
                                                const guint   max_height,
                                                      guint*  allocated_items,
                                                      guint*  allocated_height);

/**
 * Show or hide the title bar
 *
 * @param in  item: the erGtkSelectionList object
 * @param in  show: TRUE/FALSE = show/hide title bar
 *
 * @returns --
 */
void ergtk_selection_list_show_titlebar(const erGtkSelectionList* item, const gboolean show);

// returns type of this widget 
GType ergtk_selection_list_get_type (void);


#ifdef __cplusplus
}
#endif

#endif  // __ERGTK_SELECTION_LIST_H__



