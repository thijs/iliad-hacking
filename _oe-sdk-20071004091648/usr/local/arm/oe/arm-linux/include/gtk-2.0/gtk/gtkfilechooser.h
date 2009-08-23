/* GTK - The GIMP Toolkit
 * gtkfilechooser.h: Abstract interface for file selector GUIs
 * Copyright (C) 2003, Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_FILE_CHOOSER_H__
#define __GTK_FILE_CHOOSER_H__

#include "gtkfilefilter.h"
#include <gtk/gtkwidget.h>

G_BEGIN_DECLS

#define GTK_TYPE_FILE_CHOOSER             (gtk_file_chooser_get_type ())
#define GTK_FILE_CHOOSER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_FILE_CHOOSER, GtkFileChooser))
#define GTK_IS_FILE_CHOOSER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_FILE_CHOOSER))

typedef struct _GtkFileChooser      GtkFileChooser;

typedef enum
{
  GTK_FILE_CHOOSER_ACTION_OPEN,
  GTK_FILE_CHOOSER_ACTION_SAVE,
  GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
  GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER
} GtkFileChooserAction;

GType gtk_file_chooser_get_type (void) G_GNUC_CONST;

/* GError enumeration for GtkFileChooser */

#define GTK_FILE_CHOOSER_ERROR (gtk_file_chooser_error_quark ())

typedef enum {
  GTK_FILE_CHOOSER_ERROR_NONEXISTENT,
  GTK_FILE_CHOOSER_ERROR_BAD_FILENAME
} GtkFileChooserError;

GQuark gtk_file_chooser_error_quark (void);

/* Configuration
 */
void                 gtk_file_chooser_set_action          (GtkFileChooser       *chooser,
							   GtkFileChooserAction  action);
GtkFileChooserAction gtk_file_chooser_get_action          (GtkFileChooser       *chooser);
void                 gtk_file_chooser_set_local_only      (GtkFileChooser       *chooser,
							   gboolean              local_only);
gboolean             gtk_file_chooser_get_local_only      (GtkFileChooser       *chooser);
void                 gtk_file_chooser_set_select_multiple (GtkFileChooser       *chooser,
							   gboolean              select_multiple);
gboolean             gtk_file_chooser_get_select_multiple (GtkFileChooser       *chooser);
void                 gtk_file_chooser_set_show_hidden     (GtkFileChooser       *chooser,
							   gboolean              show_hidden);
gboolean             gtk_file_chooser_get_show_hidden     (GtkFileChooser       *chooser);

/* Suggested name for the Save-type actions
 */
void     gtk_file_chooser_set_current_name   (GtkFileChooser *chooser,
					      const gchar    *name);

/* Filename manipulation
 */
#ifdef G_OS_WIN32
/* Reserve old names for DLL ABI backward compatibility */
#define gtk_file_chooser_get_filename gtk_file_chooser_get_filename_utf8
#define gtk_file_chooser_set_filename gtk_file_chooser_set_filename_utf8
#define gtk_file_chooser_select_filename gtk_file_chooser_select_filename_utf8
#define gtk_file_chooser_unselect_filename gtk_file_chooser_unselect_filename_utf8
#define gtk_file_chooser_get_filenames gtk_file_chooser_get_filenames_utf8
#define gtk_file_chooser_set_current_folder gtk_file_chooser_set_current_folder_utf8
#define gtk_file_chooser_get_current_folder gtk_file_chooser_get_current_folder_utf8
#define gtk_file_chooser_get_preview_filename gtk_file_chooser_get_preview_filename_utf8
#define gtk_file_chooser_add_shortcut_folder gtk_file_chooser_add_shortcut_folder_utf8
#define gtk_file_chooser_remove_shortcut_folder gtk_file_chooser_remove_shortcut_folder_utf8
#define gtk_file_chooser_list_shortcut_folders gtk_file_chooser_list_shortcut_folders_utf8
#endif

gchar *  gtk_file_chooser_get_filename       (GtkFileChooser *chooser);
gboolean gtk_file_chooser_set_filename       (GtkFileChooser *chooser,
					      const char     *filename);
gboolean gtk_file_chooser_select_filename    (GtkFileChooser *chooser,
					      const char     *filename);
void     gtk_file_chooser_unselect_filename  (GtkFileChooser *chooser,
					      const char     *filename);
void     gtk_file_chooser_select_all         (GtkFileChooser *chooser);
void     gtk_file_chooser_unselect_all       (GtkFileChooser *chooser);
GSList * gtk_file_chooser_get_filenames      (GtkFileChooser *chooser);
gboolean gtk_file_chooser_set_current_folder (GtkFileChooser *chooser,
					      const gchar    *filename);
gchar *  gtk_file_chooser_get_current_folder (GtkFileChooser *chooser);


/* URI manipulation
 */
gchar *  gtk_file_chooser_get_uri                (GtkFileChooser *chooser);
gboolean gtk_file_chooser_set_uri                (GtkFileChooser *chooser,
						  const char     *uri);
gboolean gtk_file_chooser_select_uri             (GtkFileChooser *chooser,
						  const char     *uri);
void     gtk_file_chooser_unselect_uri           (GtkFileChooser *chooser,
						  const char     *uri);
GSList * gtk_file_chooser_get_uris               (GtkFileChooser *chooser);
gboolean gtk_file_chooser_set_current_folder_uri (GtkFileChooser *chooser,
						  const gchar    *uri);
gchar *  gtk_file_chooser_get_current_folder_uri (GtkFileChooser *chooser);

/* Preview widget
 */
void       gtk_file_chooser_set_preview_widget        (GtkFileChooser *chooser,
						       GtkWidget      *preview_widget);
GtkWidget *gtk_file_chooser_get_preview_widget        (GtkFileChooser *chooser);
void       gtk_file_chooser_set_preview_widget_active (GtkFileChooser *chooser,
						       gboolean        active);
gboolean   gtk_file_chooser_get_preview_widget_active (GtkFileChooser *chooser);
void       gtk_file_chooser_set_use_preview_label     (GtkFileChooser *chooser,
						       gboolean        use_label);
gboolean   gtk_file_chooser_get_use_preview_label     (GtkFileChooser *chooser);

char *gtk_file_chooser_get_preview_filename (GtkFileChooser *chooser);
char *gtk_file_chooser_get_preview_uri      (GtkFileChooser *chooser);

/* Extra widget
 */
void       gtk_file_chooser_set_extra_widget (GtkFileChooser *chooser,
					      GtkWidget      *extra_widget);
GtkWidget *gtk_file_chooser_get_extra_widget (GtkFileChooser *chooser);

/* List of user selectable filters
 */
void    gtk_file_chooser_add_filter    (GtkFileChooser *chooser,
					GtkFileFilter  *filter);
void    gtk_file_chooser_remove_filter (GtkFileChooser *chooser,
					GtkFileFilter  *filter);
GSList *gtk_file_chooser_list_filters  (GtkFileChooser *chooser);

/* Current filter
 */
void           gtk_file_chooser_set_filter (GtkFileChooser *chooser,
					   GtkFileFilter  *filter);
GtkFileFilter *gtk_file_chooser_get_filter (GtkFileChooser *chooser);

/* Per-application shortcut folders */

gboolean gtk_file_chooser_add_shortcut_folder    (GtkFileChooser *chooser,
						  const char     *folder,
						  GError        **error);
gboolean gtk_file_chooser_remove_shortcut_folder (GtkFileChooser *chooser,
						  const char     *folder,
						  GError        **error);
GSList *gtk_file_chooser_list_shortcut_folders   (GtkFileChooser *chooser);

gboolean gtk_file_chooser_add_shortcut_folder_uri    (GtkFileChooser *chooser,
						      const char     *uri,
						      GError        **error);
gboolean gtk_file_chooser_remove_shortcut_folder_uri (GtkFileChooser *chooser,
						      const char     *uri,
						      GError        **error);
GSList *gtk_file_chooser_list_shortcut_folder_uris   (GtkFileChooser *chooser);

G_END_DECLS

#endif /* __GTK_FILE_CHOOSER_H__ */
