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
 * \file erGtkSelectionGroup.c
 * \brief ereader gtk library - control a group of GtkToggleButton objects, similar to a radiobutton.
 *
 * Ensure only the allowed number of buttons is selected at any time.
 * Note: no relation with the standard GTK class erGtkSelection.
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


// local data
static GtkEventBoxClass* g_parent_class = NULL;

// class-specific signals for erGtkSelectionGroup
enum
{
    SELECTION_UPDATE,  // selection has changed, i.e. one or more buttons have changed state
    LAST_SIGNAL,
};

static gint g_signals[LAST_SIGNAL];  // rely on the C compiler to initialise these to 0x00


// overloaded methods for base class

// overloaded signal handlers for base class

// implementation of non-virtual methods
static void add_buttons(erGtkSelectionGroup *item, GtkToggleButton** button_tbl);

// implementation of virtual methods
static gboolean real_set_details(erGtkSelectionGroup *item, const guint min_selected, const guint max_selected);
static void real_freeze_buttons(erGtkSelectionGroup *item, const gboolean freeze);
static GtkToggleButton* real_get_button(erGtkSelectionGroup *item, const guint button_id);
static void real_get_selected_buttons(erGtkSelectionGroup *item, gint* button_ids, const guint len);

// lcoal functions
static void     ergtk_selection_group_class_init(erGtkSelectionGroupClass* klass);
static void     ergtk_selection_group_init(erGtkSelectionGroup* selection);
static void     ergtk_selection_group_dispose(GObject* object);
static void     ergtk_selection_group_finalize(GObject* object);
static void     on_button_toggled(GtkToggleButton* button, gpointer user_data);
static gboolean on_delayed_select_button(gpointer user_data);
static gboolean on_delayed_deselect_button(gpointer user_data);


// creates a new erGtkSelectionGroup widget 
GtkWidget *ergtk_selection_group_new(GtkToggleButton** button_tbl)
{
    GtkToggleButton** p_button;

    // check input
    for (p_button = button_tbl ; *p_button ; p_button++)
    {
        g_return_val_if_fail(GTK_IS_TOGGLE_BUTTON(*p_button), FALSE);
    }

    // create new object
    erGtkSelectionGroup* item = (erGtkSelectionGroup*) g_object_new(ERGTK_SELECTION_GROUP_TYPE, NULL);

    // add buttons to new object
    add_buttons(item, button_tbl);
    
    return GTK_WIDGET(item);
}

static void add_buttons(erGtkSelectionGroup *item, GtkToggleButton** button_tbl)
{
    g_return_if_fail(ERGTK_IS_SELECTION_GROUP(item));
    g_return_if_fail(item->buttons == NULL);
    g_return_if_fail(item->history == NULL);

    GtkToggleButton*  button;
    GtkToggleButton** p_button;

    // add buttons to object
    for (p_button = button_tbl ; *p_button ; p_button++)
    {
        button = *p_button;
        g_object_ref(G_OBJECT(button));
        g_signal_connect_after(G_OBJECT(button), "toggled", G_CALLBACK(on_button_toggled), item);
        item->buttons = g_slist_append(item->buttons, button);

        if (gtk_toggle_button_get_active(button))
        {
            item->history = g_slist_append(item->history, button);
        }
    }
}

// class type
GType ergtk_selection_group_get_type(void)
{
    static GType class_type = 0;

    if (class_type == 0)
    {
        static const GTypeInfo class_info =
        {
            sizeof(erGtkSelectionGroupClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) ergtk_selection_group_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(erGtkSelectionGroup),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) ergtk_selection_group_init,
        };

        class_type = g_type_register_static(GTK_TYPE_EVENT_BOX, "erGtkSelectionGroup", &class_info, 0);
    }
    return class_type;
}

// init class
static void ergtk_selection_group_class_init(erGtkSelectionGroupClass* klass)
{
    GObjectClass* object_class = (GObjectClass *)klass;

    // remember parent class struct, needed for chaining up to parent class
    g_parent_class = g_type_class_peek_parent(klass);

    // overload some virtual methods
    object_class->dispose  = ergtk_selection_group_dispose;
    object_class->finalize = ergtk_selection_group_finalize;
    //
    klass->set_details          = real_set_details;
    klass->freeze_buttons       = real_freeze_buttons;
    klass->get_button           = real_get_button;
    klass->get_selected_buttons = real_get_selected_buttons;
    klass->add_buttons          = add_buttons;

    // event to notify selection update 
    g_signals[SELECTION_UPDATE] = g_signal_new( "selection_update",
                                                G_OBJECT_CLASS_TYPE(object_class),
                                                G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                                                G_STRUCT_OFFSET(erGtkSelectionGroupClass, selection_update),  // class closure
                                                NULL,             // accumulator
                                                NULL,             // accu data
                                                gtk_marshal_VOID__POINTER,
                                                G_TYPE_NONE,      // return type
                                                1,                // #params
                                                G_TYPE_POINTER ); // param types
}

// init instance
static void ergtk_selection_group_init(erGtkSelectionGroup* item)
{
    g_return_if_fail(ERGTK_IS_SELECTION_GROUP(item));

    // set object defaults
    item->dispose_has_run      = FALSE;
    item->buttons              = NULL;
    item->min_selected         = 0;
    item->max_selected         = 9999;
    item->freeze_buttons       = FALSE;
    item->history              = NULL;
}

// dispose instance
static void ergtk_selection_group_dispose(GObject* object)
{
    g_return_if_fail(ERGTK_IS_SELECTION_GROUP(object));

    erGtkSelectionGroup* item = (erGtkSelectionGroup*)object;

    // release reference on buttons
    if (!item->dispose_has_run)
    {
        item->dispose_has_run = TRUE;

        g_slist_foreach(item->buttons, (GFunc)g_object_unref, NULL);
    }

    // chain to parent class
    GObjectClass* parent_object_class = G_OBJECT_CLASS(g_parent_class);
    if (parent_object_class->dispose)
    {
        parent_object_class->dispose(object);
    }
}

// finalise instance
static void ergtk_selection_group_finalize(GObject* object)
{
    g_return_if_fail(ERGTK_IS_SELECTION_GROUP(object));

    erGtkSelectionGroup* item = (erGtkSelectionGroup*)object;

    // free linked lists in item
    if (item->buttons)
    {
        g_slist_free(item->buttons);
    }
    if (item->history)
    {
        g_slist_free(item->history);
    }

    // chain to parent class
    GObjectClass* parent_object_class = G_OBJECT_CLASS(g_parent_class);
    if (parent_object_class->finalize)
    {
        parent_object_class->finalize(object);
    }
}

// public methods
gboolean ergtk_selection_group_set_details(erGtkSelectionGroup *item, const guint min_selected, const guint max_selected)
{
    LOGPRINTF("entry: item [%p] min [%u] max [%u]", item, min_selected, max_selected);
    g_return_val_if_fail(ERGTK_IS_SELECTION_GROUP(item),   FALSE);
    g_return_val_if_fail((item->dispose_has_run == FALSE), FALSE);

    erGtkSelectionGroupClass* klass = ERGTK_SELECTION_GROUP_GET_CLASS(item);

    // chain to real method, which may be overloaded
    if (klass->set_details)
    {
        return klass->set_details(item, min_selected, max_selected);
    }
    else
    {
        return FALSE;  // not ok
    }
}

static gboolean real_set_details(erGtkSelectionGroup *item, const guint min_selected, const guint max_selected)
{
    LOGPRINTF("entry: item [%p] min [%u] max [%u]", item, min_selected, max_selected);
    g_return_val_if_fail(ERGTK_IS_SELECTION_GROUP(item),   FALSE);
    g_return_val_if_fail((item->dispose_has_run == FALSE), FALSE);

    gint num_buttons = g_slist_length(item->buttons);
    g_return_val_if_fail(min_selected <= max_selected, FALSE);
    g_return_val_if_fail(min_selected <= num_buttons,  FALSE);
    g_return_val_if_fail(max_selected >= 0,            FALSE);

    GSList* history_last          = g_slist_last(item->history);
    GSList* buttons_last_selected = history_last ? g_slist_find(item->buttons, history_last->data) : NULL;
    
    GtkToggleButton* button_tmp = NULL;

    // store settings in item
    item->min_selected = min_selected;
    item->max_selected = max_selected;
    
    // enforce button states to be within specified range
    gint num_selected = g_slist_length(item->history);
    LOGPRINTF("num_buttons [%d] num_selected [%d]", num_buttons, num_selected);
    while (num_selected < min_selected)
    {
        // find the next button after the newest button from history list
        if (buttons_last_selected)
        {
            buttons_last_selected = g_slist_next(buttons_last_selected);
        }
        else
        {
            buttons_last_selected = item->buttons;
        }
        item->history = g_slist_append(item->history, buttons_last_selected->data);
        history_last  = g_slist_last(item->history);
        num_selected++;

        // and select this button
        // Note: xx_set_active calls signal handler who modifies item data,
        //       so make sure all item data has been updated before
        gtk_toggle_button_set_active(buttons_last_selected->data, TRUE);
    }
    while (num_selected > max_selected)
    {
        // remove the newest button from history list
        button_tmp = history_last->data;
        item->history = g_slist_delete_link(item->history, history_last);
        history_last  = g_slist_last(item->history);
        num_selected--;
        
        // and deselect this button
        // Note: xx_set_active calls signal handler who modifies item data,
        //       so make sure all item data has been updated before
        gtk_toggle_button_set_active(button_tmp, FALSE);
    }

    LOGPRINTF("leave: num_selected [%d] history_length [%d]", num_selected, g_slist_length(item->history));
    return TRUE;  // ok
}


void ergtk_selection_group_freeze_buttons(erGtkSelectionGroup *item,
                                              const gboolean freeze)
{
    LOGPRINTF("entry: item [%p] freeze [%d]", item, freeze);
    g_return_if_fail(ERGTK_IS_SELECTION_GROUP(item));
    g_return_if_fail((item->dispose_has_run == FALSE));

    erGtkSelectionGroupClass* klass = ERGTK_SELECTION_GROUP_GET_CLASS(item);

    // chain to real method, which may be overloaded
    if (klass->freeze_buttons)
    {
        klass->freeze_buttons(item, freeze);
    }
}

static void real_freeze_buttons(erGtkSelectionGroup *item,
                                const gboolean freeze)
{
    LOGPRINTF("entry: item [%p] freeze [%d]", item, freeze);
    g_return_if_fail(ERGTK_IS_SELECTION_GROUP(item));
    g_return_if_fail((item->dispose_has_run == FALSE));

    item->freeze_buttons = freeze;
}


GtkToggleButton* ergtk_selection_group_get_button(erGtkSelectionGroup *item, const guint button_id)
{
    LOGPRINTF("entry: item [%p] button_id [%d]", item, button_id);
    g_return_val_if_fail(ERGTK_IS_SELECTION_GROUP(item),   NULL);
    g_return_val_if_fail((item->dispose_has_run == FALSE), NULL);

    erGtkSelectionGroupClass* klass = ERGTK_SELECTION_GROUP_GET_CLASS(item);

    // chain to real method, which may be overloaded
    if (klass->get_button)
    {
        return klass->get_button(item, button_id);
    }
    else
    {
        return NULL;
    }
}

static GtkToggleButton* real_get_button(erGtkSelectionGroup *item, const guint button_id)
{
    LOGPRINTF("entry: item [%p] button_id [%d]", item, button_id);
    g_return_val_if_fail(ERGTK_IS_SELECTION_GROUP(item),   NULL);
    g_return_val_if_fail((item->dispose_has_run == FALSE), NULL);

    GtkToggleButton* button = g_slist_nth_data(item->buttons, button_id);

    return button;
}


void ergtk_selection_group_get_selected_buttons(erGtkSelectionGroup *item, gint* button_ids, const guint len)
{
    LOGPRINTF("entry: item [%p]", item);
    g_return_if_fail(ERGTK_IS_SELECTION_GROUP(item));
    g_return_if_fail((item->dispose_has_run == FALSE));

    erGtkSelectionGroupClass* klass = ERGTK_SELECTION_GROUP_GET_CLASS(item);

    // chain to real method, which may be overloaded
    if (klass->get_selected_buttons)
    {
        klass->get_selected_buttons(item, button_ids, len);
    }
}

static void real_get_selected_buttons(erGtkSelectionGroup *item, gint* button_ids, const guint len)
{
    LOGPRINTF("entry: item [%p]", item);
    g_return_if_fail(ERGTK_IS_SELECTION_GROUP(item)  );
    g_return_if_fail((item->dispose_has_run == FALSE));

    int     i;
    GSList* history = item->history;

    for (i = 0 ; i < len ; i++)
    {
        if (history)
        {
            button_ids[i] = g_slist_index(item->buttons, history->data);
            history = g_slist_next(history);
        }
        else
        {
            button_ids[i] = -1;
        }
        LOGPRINTF("button_id[%d] = [%d]", i, button_ids[i]);
    }
}


// signal handlers
static void on_button_toggled(GtkToggleButton* button, gpointer user_data)
{
    LOGPRINTF("entry: button [%p] item [%p]", button, user_data);
    erGtkSelectionGroup* item = (erGtkSelectionGroup*)user_data;
    
    g_return_if_fail(ERGTK_IS_SELECTION_GROUP(item));
    g_return_if_fail((item)->dispose_has_run == FALSE);

    gboolean active         = gtk_toggle_button_get_active(button);
    GSList*  history_button = g_slist_find(item->history, button);
    guint    history_length = g_slist_length(item->history);
    LOGPRINTF("active [%d] history_button [%p] history_len [%u]", 
               active, history_button, history_length);

    GtkToggleButton* button_tmp;

    if (item->freeze_buttons)
    {
        // force button to the state conform our administration
        if (history_button)
        {
            // force button to state selected
            if (!active)
            {
                g_timeout_add(500, on_delayed_select_button, (gpointer)button);
            }
        }
        else
        {
            // force button to state de-selected
            if (active)
            {
                g_timeout_add(500, on_delayed_deselect_button, (gpointer)button);
            }
        }
    }
    else
    {
        // enforce min/max selected buttons
        if (active)
        {
            if (history_button == NULL)
            {
                // remember we've seen this button selected
                LOGPRINTF("add button [%p] to history", button);
                item->history = g_slist_append(item->history, button);
                history_length++;

                // enforce max selected buttons
                LOGPRINTF("enforce [%d] history_len [%d] max_selected [%d]", 
                        item->enforce_max_selected, history_length, item->max_selected);
                if (history_length > item->max_selected)
                {
                    
                    // too many buttons selected, deselect the oldest selected button
                    button_tmp = GTK_TOGGLE_BUTTON(item->history->data);
                    LOGPRINTF("remove button [%p] to history", button_tmp);
                    item->history = g_slist_delete_link(item->history, item->history);

                    // Note: xx_set_active may call us again,
                    //       so make sure all item data has been updated before
                    gtk_toggle_button_set_active(button_tmp, FALSE);
                    LOGPRINTF("button [%p] set to not-active", button_tmp);
                }
            }
            else
            {
                // we've seen this button active before, ignore
            }
        }
        else
        {
            if (history_button)
            {
                LOGPRINTF("enforce [%d] history_len [%d] min_selected [%d]", 
                        item->enforce_min_selected, history_length, item->min_selected);
                if (history_length > item->min_selected)
                {

                    // remember we've seen this button de-selected
                    item->history = g_slist_delete_link(item->history, history_button);
                }
                else
                {
                    // too few buttons selected, refuse de-selecting this button
                    g_timeout_add(500, on_delayed_select_button, (gpointer)button);
                }
            }
            else
            {
                // we did not see this button active, ignore
            }
        }
    }

    // report selectionGroup has changed
    g_signal_emit(item, g_signals[SELECTION_UPDATE], 0, (gpointer)button);
}

static gboolean on_delayed_select_button(gpointer user_data)
{
    LOGPRINTF("entry: button [%p]", user_data);
    // TODO: check on dispose_has_run, but how to do this?

    GtkToggleButton* button = (GtkToggleButton*)user_data;
    
    g_return_val_if_fail(GTK_IS_TOGGLE_BUTTON(button), FALSE);

    gtk_toggle_button_set_active(button, TRUE);

    return FALSE;  // FALSE = don't call again
}

static gboolean on_delayed_deselect_button(gpointer user_data)
{
    LOGPRINTF("entry: button [%p]", user_data);
    // TODO: check on dispose_has_run, but how to do this?

    GtkToggleButton* button = (GtkToggleButton*)user_data;
    
    g_return_val_if_fail(GTK_IS_TOGGLE_BUTTON(button), FALSE);

    gtk_toggle_button_set_active(button, FALSE);

    return FALSE;  // FALSE = don't call again
}

gint ergtk_selection_group_get_length(erGtkSelectionGroup *item)
{
    LOGPRINTF("entry: item [%p]", item);

    gint num_buttons = 0;
    
    g_return_val_if_fail(ERGTK_IS_SELECTION_GROUP(item), 0);
    g_return_val_if_fail(item->buttons, 0);
 
    num_buttons = g_slist_length(item->buttons);

    LOGPRINTF("%d buttons cotrolled by erGtkSelectionGroup [%p]",num_buttons, item);

    return num_buttons;
}

