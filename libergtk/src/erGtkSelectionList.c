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
 * \file erGtkSelectionList.c
 * \brief ereader gtk library - control a group of GtkToggleButton objects, similar to a radiobutton.
 *
 * Ensure only the allowed number of buttons is selected at any time.
 * Note: no relation with the standard GTK class GtkSelection.
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 */

// system include files
#include <gtk/gtk.h>

// ereader library includes

// local includes
#include "ergtklog.h"
#include "erGtkSelectionGroup.h"
#include "erGtkSelectionList.h"
#include "erGtkToggleButton.h"


// local data
static erGtkSelectionGroupClass* g_parent_class = NULL;

// class-specific signals for erGtkSelectionList

// overloaded methods for base class

// overloaded signal handlers for base class

// implementation of virtual methods

// lcoal functions
static void     ergtk_selection_list_class_init(erGtkSelectionListClass* klass);
static void     ergtk_selection_list_init(erGtkSelectionList* selection);

static GtkToggleButton* create_button(const erGtkSelectionList_displayAs_e display_as, const gchar* label_text);
static guint button_height_from_display_as(const erGtkSelectionList_displayAs_e display_as);

static void add_buttons_to_screen_layout(erGtkSelectionList* item,
                                               GtkVBox*      parent_vbox,
                                         const GSList*       buttons,
                                         const guint         first_item,
                                         const guint         max_height,
                                               guint*        allocated_buttons,
                                               guint*        allocated_height);


// create a new erGtkSelectionList object
GtkWidget *ergtk_selection_list_new(const erGtkSelectionList_displayAs_e display_as,
                                    const gchar*  title,
                                    const gchar*  instruction,
                                    const gchar** item_names,
                                    const guint   max_height,
                                          guint*  allocated_items,
                                          guint*  allocated_height)
{
    int               i;
    const gchar**     cpp;
    guint             height;
    guint             num_items = 0;
    guint             item_height = 0;
    GtkToggleButton** button_tbl = NULL;
    guint             items_allocated = 0;
    gboolean          instruction_added = FALSE;

    GtkWidget*        widget;
    GtkWidget*        alignment;
    GtkWidget*        background;
    GtkWidget*        vbox;

    LOGPRINTF("entry: title [%s]", title);

    // create new object
    erGtkSelectionList* item = (erGtkSelectionList*) g_object_new(ERGTK_SELECTION_LIST_TYPE, NULL);
    gtk_widget_show(GTK_WIDGET(item));
    item->display_as = display_as;

    // create buttons for all selection items (including the items that will not be displayed)
    for (cpp = item_names ; *cpp != NULL ; cpp++)
    {
        num_items++;
    }
    button_tbl = g_new0(GtkToggleButton*, num_items + 1);
    g_assert(button_tbl != NULL);
    for (i = 0 ; i < num_items ; i++)
    {
        button_tbl[i] = create_button(display_as, item_names[i]);
    }
    button_tbl[i] = NULL;

    // append buttons to selection group (parent class)
    g_parent_class->add_buttons(&(item->parent), button_tbl);

    // create screen objects
    //
    // widget hierarchy:
    //   item (erGtkSelectionList inherits from GtkEventBox)
    //     |-- vbox
    //           |
    widget = gtk_vbox_new(FALSE, ERGTK_SELECTION_LIST_TITLE_SPACING);
    gtk_widget_show(widget);
    gtk_container_add(GTK_CONTAINER(item), widget);
    vbox = widget;
    //           |
    //           |   #  title
    //           |-- titlebar
    //           |     |-- label
    //           |
    widget = gtk_event_box_new();
    gtk_widget_show(widget);
    gtk_widget_set_name(widget, ERGTK_SELECTION_LIST_TITLE_BACKGROUND_STYLE);
    gtk_widget_set_size_request(widget, -1, ERGTK_SELECTION_LIST_TITLE_HEIGHT);
    gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    item_height += ERGTK_SELECTION_LIST_TITLE_HEIGHT;
    item->titlebar = widget;
    //
    widget = gtk_label_new(title);
    gtk_widget_show(widget);
    gtk_widget_set_name(widget, ERGTK_SELECTION_LIST_TITLE_TEXT_STYLE);
    gtk_misc_set_padding(GTK_MISC(widget), ERGTK_SELECTION_LIST_H_PADDING, 0);
    gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.5);
    gtk_container_add(GTK_CONTAINER(item->titlebar), widget);
    //           |
    //           |   #  selection list
    //           |-- background
    //                 |-- alignment
    //                       |-- vbox
    //                             |
    widget = gtk_event_box_new();
    gtk_widget_show(widget);
    gtk_widget_set_name(widget, ERGTK_SELECTION_LIST_BACKGROUND_STYLE);
    gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
    item_height += ERGTK_SELECTION_LIST_TITLE_SPACING;
    background = widget;
    //
    widget = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_widget_show(widget);
    gtk_alignment_set_padding(GTK_ALIGNMENT(widget),
                              ERGTK_SELECTION_LIST_V_PADDING, ERGTK_SELECTION_LIST_V_PADDING,
                              ERGTK_SELECTION_LIST_H_PADDING, ERGTK_SELECTION_LIST_H_PADDING);
    gtk_container_add(GTK_CONTAINER(background), widget);
    item_height += 2 * ERGTK_SELECTION_LIST_V_PADDING;
    alignment = widget;
    //
    widget = gtk_vbox_new(FALSE, ERGTK_SELECTION_LIST_BUTTON_V_SPACING);
    gtk_widget_show(widget);
    gtk_container_add(GTK_CONTAINER(alignment), widget);
    vbox = widget;
    //                             |-- [instruction]
    //                             |
    if (instruction  &&  instruction[0] != '\0')
    {
        instruction_added = TRUE;
        widget = gtk_label_new(instruction);
        gtk_widget_show(widget);
        gtk_widget_set_name(widget, ERGTK_SELECTION_LIST_INSTRUCTION_STYLE);
        gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.5);
        gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
        item_height += ERGTK_SELECTION_LIST_INSTRUCTION_HEIGHT;
    }
    //                             |-- buttons (0 ..)
    add_buttons_to_screen_layout( item,
                                  GTK_VBOX(vbox),
                                  item->parent.buttons,
                                  0,
                                  max_height - item_height,
                                  &items_allocated,
                                  &height );
    item_height += height;
    //
    if (instruction_added  &&  items_allocated > 0)
    {
        item_height += ERGTK_SELECTION_LIST_BUTTON_V_SPACING;
    }

    // that's all folks ..
    *allocated_items  = items_allocated;
    *allocated_height = item_height;
    g_free(button_tbl);
    if (item_height <= max_height)
    {
        return GTK_WIDGET(item);
    }
    else
    {
        gtk_widget_destroy(GTK_WIDGET(item));
        return NULL;
    }
}

// create a new erGtkSelectionList object as extension from another (master) object
GtkWidget* ergtk_selection_list_new_from_master(const erGtkSelectionList* master,
                                                const guint   first_item,
                                                const guint   max_height,
                                                      guint*  allocated_items,
                                                      guint*  allocated_height)
{
    g_return_val_if_fail(ERGTK_IS_SELECTION_LIST(master), FALSE);

    guint      height;
    guint      item_height = 0;
    guint      items_allocated = 0;
    
    GtkWidget* widget;
    GtkWidget* alignment;
    GtkWidget* vbox;

    // create new object
    erGtkSelectionList* item = (erGtkSelectionList*) g_object_new(ERGTK_SELECTION_LIST_TYPE, NULL);
    item->display_as = master->display_as;
    gtk_widget_show(GTK_WIDGET(item));

    // create screen objects
    //
    // widget hierarchy:
    //   item (erGtkSelectionList inherits from GtkEventBox)
    //     |
    gtk_widget_set_name(GTK_WIDGET(item), ERGTK_SELECTION_LIST_BACKGROUND_STYLE);
    //     |
    //     |   #  selection list
    //     |-- alignment
    //           |-- vbox
    //                 |
    widget = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_widget_show(widget);
    gtk_alignment_set_padding(GTK_ALIGNMENT(widget),
                              ERGTK_SELECTION_LIST_V_PADDING, ERGTK_SELECTION_LIST_V_PADDING,
                              ERGTK_SELECTION_LIST_H_PADDING, ERGTK_SELECTION_LIST_H_PADDING);
    gtk_container_add(GTK_CONTAINER(item), widget);
    item_height += 2 * ERGTK_SELECTION_LIST_V_PADDING;
    alignment = widget;
    //
    widget = gtk_vbox_new(FALSE, ERGTK_SELECTION_LIST_BUTTON_V_SPACING);
    gtk_widget_show(widget);
    gtk_container_add(GTK_CONTAINER(alignment), widget);
    vbox = widget;
    //                 |
    //                 |-- buttons (0 ..)
    add_buttons_to_screen_layout( item,
                                  GTK_VBOX(vbox),
                                  master->parent.buttons,
                                  first_item,
                                  max_height,
                                  &items_allocated,
                                  &height);
    item_height += height;

    // that's all folks ..
    *allocated_items  = items_allocated;
    *allocated_height = item_height;
    if (item_height <= max_height)
    {
        return GTK_WIDGET(item);
    }
    else
    {
        gtk_widget_destroy(GTK_WIDGET(item));
        return NULL;
    }
}

// create button for item
static GtkToggleButton* create_button(const erGtkSelectionList_displayAs_e display_as, const gchar* label_text)
{
    GtkWidget*  widget = NULL;
    const guint button_height = button_height_from_display_as(display_as);

    switch (display_as)
    {
        case esl_Checklist:
            widget = gtk_check_button_new_with_label(label_text);
            gtk_widget_set_name(widget, ERGTK_SELECTION_LIST_CHECKBUTTON_STYLE);
            gtk_widget_set_size_request(widget, -1, button_height);
            break;

        case esl_RadioButton:
            widget = gtk_check_button_new_with_label(label_text);
            gtk_widget_set_name(widget, ERGTK_SELECTION_LIST_RADIOBUTTON_STYLE);
            gtk_widget_set_size_request(widget, -1, button_height);
            break;

        case esl_TextButton:
        default:
            widget = ergtk_toggle_button_new_with_label(label_text);
            gtk_widget_set_name(widget, ERGTK_SELECTION_LIST_TEXTBUTTON_STYLE);
            gtk_widget_set_size_request(widget, ERGTK_SELECTION_LIST_TEXTBUTTON_WIDTH, button_height);
    }

    GtkToggleButton* button = GTK_TOGGLE_BUTTON(widget);
    gtk_toggle_button_set_active(button, FALSE);

    return button;
}

static guint button_height_from_display_as(const erGtkSelectionList_displayAs_e display_as)
{
    guint height = 0;

    switch (display_as)
    {
        case esl_Checklist:
            height = ERGTK_SELECTION_LIST_CHECKBUTTON_HEIGHT;
            break;

        case esl_RadioButton:
            height = ERGTK_SELECTION_LIST_RADIOBUTTON_HEIGHT;
            break;

        case esl_TextButton:
            height = ERGTK_SELECTION_LIST_TEXTBUTTON_HEIGHT;
            break;
            
        default:
            ;  // ignore
    }
    
    return height;
}

// add buttons to screen widgets
//
// widget hierarchy:
//   parent_vbox
//     |-- [continuation_before]
//     |
//     |   # for text-buttons four on a row:
//     |-- hbox (0 ..)
//     |     |-- button (0 .. 3)
//     |
//     |   # for check-buttons one on a row:
//     |-- button (0 ..)
//     |
//     |-- [continuation_after]
// 
static void add_buttons_to_screen_layout(erGtkSelectionList* item,
                                               GtkVBox*      parent_vbox,
                                         const GSList*       buttons,
                                         const guint         first_button,
                                         const guint         max_height,
                                               guint*        allocated_buttons,
                                               guint*        allocated_height)
{
    guint         item_height = 0;
    guint         buttons_added = 0;
    GtkWidget*    hbox = NULL;

    GtkWidget*    widget;
    GtkWidget*    button;
    const GSList* button_node;

    const guint   num_buttons      = g_slist_length((GSList*)buttons);
    const guint   buttons_per_line = (item->display_as == esl_TextButton) ? 4 : 1;
    guint         button_height    = button_height_from_display_as(item->display_as);

    guint lines_needed;
    guint lines_available;
    guint lines_used;
    guint line;

    if (first_button > 0)
    {
        // continuation_before
        widget = gtk_label_new(ERGTK_SELECTION_LIST_CONTINUATION_TEXT);
        gtk_widget_show(widget);
        gtk_widget_set_name(widget, ERGTK_SELECTION_LIST_CONTINUATION_STYLE);
        gtk_widget_set_size_request(widget, -1, button_height);
        gtk_misc_set_padding(GTK_MISC(widget), ERGTK_SELECTION_LIST_CONTINUATION_H_PADDING, 0);
        gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.0);
        gtk_box_pack_start(GTK_BOX(parent_vbox), widget, FALSE, FALSE, 0);
        item_height += button_height;
    }

    // calculate how many lines with buttons to show
    lines_available = (max_height - item_height + ERGTK_SELECTION_LIST_BUTTON_V_SPACING) / (button_height + ERGTK_SELECTION_LIST_BUTTON_V_SPACING);
    lines_needed    = ((num_buttons - first_button) + (buttons_per_line - 1)) / buttons_per_line;
    if (lines_needed <= lines_available)
    {
        // use the only the lines we need
        lines_used = lines_needed;
    }
    else
    {
        // use all available lines but one, last one used for continuaton_after
        lines_used = lines_available - 1;

        // continuation_after
        widget = gtk_label_new(ERGTK_SELECTION_LIST_CONTINUATION_TEXT);
        gtk_widget_show(widget);
        gtk_widget_set_name(widget, ERGTK_SELECTION_LIST_CONTINUATION_STYLE);
        gtk_widget_set_size_request(widget, -1, button_height);
        gtk_misc_set_padding(GTK_MISC(widget), ERGTK_SELECTION_LIST_CONTINUATION_H_PADDING, 0);
        gtk_misc_set_alignment(GTK_MISC(widget), 0.0, 0.0);
        gtk_box_pack_end(GTK_BOX(parent_vbox), widget, FALSE, FALSE, 0);
        item_height += button_height;
    }

    // add buttons
    line = 0;
    for ( button_node  = g_slist_nth((GSList*)buttons, first_button) ;
          button_node != NULL ;
          button_node  = g_slist_next(button_node) )
    {
        // we have another button
        button = GTK_WIDGET(button_node->data);
        gtk_widget_show(button);

        if (buttons_added % buttons_per_line == 0)
        {
            // need a new line to show this button
            if (line < lines_used)
            {
                // add a new line
                line++;
                if (buttons_per_line == 1)
                {
                    // add button directly
                    gtk_box_pack_start(GTK_BOX(parent_vbox), button, FALSE, FALSE, 0);
                    item_height += button_height;
                    buttons_added++;
                }
                else
                {
                    // create a new hbox to pack these buttons
                    hbox = gtk_hbox_new(FALSE, ERGTK_SELECTION_LIST_BUTTON_H_SPACING);
                    gtk_widget_show(hbox);
                    gtk_box_pack_start(GTK_BOX(parent_vbox), hbox, FALSE, FALSE, 0);
                    item_height += button_height;

                    // add button to hbox
                    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
                    buttons_added++;
                }
            }
        }
        else
        {
            // append button to current line
            gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
            buttons_added++;
        }
    }

    // correct for no spacing before the first widget added to parent_vbox
    if (item_height >= ERGTK_SELECTION_LIST_BUTTON_V_SPACING)
    {
        item_height -= ERGTK_SELECTION_LIST_BUTTON_V_SPACING;
    }

    // report what we've done
    *allocated_buttons = buttons_added;
    *allocated_height  = item_height;
}

void ergtk_selection_list_show_titlebar(const erGtkSelectionList* item, const gboolean show)
{
    g_return_if_fail(item != NULL);
    g_return_if_fail(ERGTK_IS_SELECTION_LIST(item));

    if (show)
    {
        gtk_widget_show(item->titlebar);
    }
    else
    {
        gtk_widget_hide(item->titlebar);
    }
}

// class type
GType ergtk_selection_list_get_type(void)
{
    static GType class_type = 0;

    if (class_type == 0)
    {
        static const GTypeInfo class_info =
        {
            sizeof(erGtkSelectionListClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) ergtk_selection_list_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(erGtkSelectionList),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) ergtk_selection_list_init,
        };

        class_type = g_type_register_static(ERGTK_SELECTION_GROUP_TYPE, "erGtkSelectionList", &class_info, 0);
    }
    return class_type;
}

// init class
static void ergtk_selection_list_class_init(erGtkSelectionListClass* klass)
{
    // remember parent class struct, needed for chaining up to parent class
    g_parent_class = g_type_class_peek_parent(klass);

    // overload some virtual methods
}

// init instance
static void ergtk_selection_list_init(erGtkSelectionList* item)
{
    g_return_if_fail(ERGTK_IS_SELECTION_LIST(item));

    // set object defaults
    item->dispose_has_run = FALSE;
    item->display_as      = esl_TextButton;
    item->titlebar        = NULL;
}


// public methods

// signal handlers

