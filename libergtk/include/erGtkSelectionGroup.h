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
 * \file erGtkSelectionGroup.h
 * \brief ereader gtk library - control a group of GtkToggleButton objects, similar to a radiobutton.
 *
 * Ensure only the allowed number of buttons is selected at any time.
 * Note: no relation with the standard GTK class erGtkSelection.
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 */

#ifndef __ERGTK_SELECTION_GROUP_H__
#define __ERGTK_SELECTION_GROUP_H__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define ERGTK_SELECTION_GROUP_TYPE             (ergtk_selection_group_get_type())
#define ERGTK_SELECTION_GROUP(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), ERGTK_SELECTION_GROUP_TYPE, erGtkSelectionGroup))
#define ERGTK_IS_SELECTION_GROUP(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), ERGTK_SELECTION_GROUP_TYPE))

#define ERGTK_SELECTION_GROUP_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass),  ERGTK_SELECTION_GROUP_TYPE, erGtkSelectionGroupClass))
#define ERGTK_SELECTION_GROUP_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), ERGTK_SELECTION_GROUP_TYPE, erGtkSelectionGroupClass))
#define ERGTK_IS_SELECTION_GROUP_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass),  ERGTK_SELECTION_GROUP_TYPE))


typedef struct _erGtkSelectionGroup       erGtkSelectionGroup;
typedef struct _erGtkSelectionGroupClass  erGtkSelectionGroupClass;

struct _erGtkSelectionGroup
{
    GtkEventBox       parent;

    // protected
    GSList*           buttons;               // buttons controlled by this object

    // private
    gboolean          dispose_has_run;
    guint             min_selected;          // minimum number of buttons that must be selected
    guint             max_selected;          // maximum number of buttons that may be selected
    gboolean          freeze_buttons;        // keep button state as is, even when user changes them
    GSList*           history;               // selected buttons in the order these have been selected
};

struct _erGtkSelectionGroupClass
{
    GtkEventBoxClass                parent_class;

    // public
    //   virtual methods
    gboolean (*set_details)        (erGtkSelectionGroup *item, const guint min_selected, const guint max_selected);
    void (*freeze_buttons)         (erGtkSelectionGroup *item, const gboolean freeze);
    GtkToggleButton* (*get_button) (erGtkSelectionGroup *item, const guint button_id);
    void (*get_selected_buttons)   (erGtkSelectionGroup *item, gint* button_ids, const guint len);

    // protected
    //   non-virtual methods
    void (*add_buttons)            (erGtkSelectionGroup *item, GtkToggleButton** button_tbl);

    // private
    //   signal handlers
    void (*selection_update)       (erGtkSelectionGroup *selection_group);
};


/**
 * create a new erGtkSelectionGroup object
 *
 * @param button_tbl - NULL-terminated array of ptrs to the buttons that must be controlled
 *
 * @returns reference to created widget
 */
GtkWidget* ergtk_selection_group_new(GtkToggleButton** button_tbl);

/**
 * set selection details for the buttons controlled by the erGtkSelectionGroup object
 *
 * @param item - the erGtkSelectionGroup object
 * @param min_selected - the minimum number of buttons that must be selected
 * @param max_selected - the maximum number of buttons that must be selected
 * @param enforce_min - enforce that at least min_selected buttons are selected
 * @param enforce_max - enforce that at most  max_selected buttons are selected
 *
 * @returns TRUE ok, FALSE error
 */
gboolean ergtk_selection_group_set_details(erGtkSelectionGroup *item,
                                           const guint min_selected,
                                           const guint max_selected);

/**
 * force buttons to stay in their current state, even when user changes them
 *
 * @param item - the erGtkSelectionGroup object
 * @param freeze - TRUE force buttons in current state, FALSE allow user to change buttons
 *
 * @returns --
 */
void ergtk_selection_group_freeze_buttons(erGtkSelectionGroup *item,
                                          const gboolean freeze);

/**
 * get button by index
 *
 * @param item - the erGtkSelectionGroup object
 * @param button_id - id of the requested button as an index (0 ..) in button_tbl on .._new()
 *
 * @returns ptr to the requested button, or NULL
 */
GtkToggleButton* ergtk_selection_group_get_button(erGtkSelectionGroup *item,
                                                  const guint button_id);

/**
 * get index of currently selected buttons
 *
 * @param item - the erGtkSelectionGroup object
 * @param button_ids (out) - array selected buttons id (0 ..), latest selected first, unused items set to -1
 * @param len - number of elements in array button_ids
 *
 * @returns --
 */
void ergtk_selection_group_get_selected_buttons(erGtkSelectionGroup *item,
                                                gint* button_ids, const guint len);

/**
 * get the number of how many buttons in erGtkSelectionGroup
 *
 * @param item - the erGtkSelectionGroup object
 *
 * @returns the number of buttons
 */
gint ergtk_selection_group_get_length(erGtkSelectionGroup *item);

// returns type of this widget 
GType ergtk_selection_group_get_type (void);


#ifdef __cplusplus
}
#endif

#endif  // __ERGTK_SELECTION_GROUP_H__



