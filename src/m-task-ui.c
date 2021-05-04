/*
 * Copyright (C) 2021 Felix Exner <git@fexner.de>
 *
 * This library is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib/gi18n-lib.h>

#include "m-task-ui.h"

void
entry_changed (GtkEntry *entry, char **target)
{
  *target = strdup (gtk_entry_get_text (entry));
}

void
combo_changed (GtkComboBoxText *widget, char **target)
{
  g_print ("combo changed: %s\n", gtk_combo_box_text_get_active_text (widget));
  *target = strdup (gtk_combo_box_text_get_active_text (widget));
}

GtkWidget *
create_task_dialog (GtkWindow *parent, Task *task)
{

  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *grid;
  GtkWidget *entry;
  GtkWidget *combo;
  GtkWidget *content_area;
  GtkDialogFlags flags;

  /*Create the widgets*/
  flags = GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_dialog_new_with_buttons ("Message",
                                        parent,
                                        flags,
                                        _ ("_Create Task"),
                                        GTK_RESPONSE_OK,
                                        _ ("_Abort"),
                                        GTK_RESPONSE_CANCEL,
                                        NULL);
  content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  grid = gtk_grid_new ();

  label = gtk_label_new ("Task title");
  GtkWidget *entry_title = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (entry_title), task->title);
  gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), entry_title, 1, 0, 4, 1);
  g_signal_connect (entry_title,
                    "changed",
                    G_CALLBACK (entry_changed),
                    &task->title);

  label = gtk_label_new ("project");
  entry = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (entry), task->project);
  gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), entry, 1, 1, 1, 1);
  g_signal_connect (entry,
                    "changed",
                    G_CALLBACK (entry_changed),
                    &task->project);

  label = gtk_label_new ("Task description");
  entry = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (entry), task->description);
  gtk_grid_attach (GTK_GRID (grid), label, 0, 2, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), entry, 1, 2, 1, 1);
  g_signal_connect (entry,
                    "changed",
                    G_CALLBACK (entry_changed),
                    &task->description);

  label = gtk_label_new ("Tags");
  entry = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (entry), task->tags);
  gtk_grid_attach (GTK_GRID (grid), label, 0, 3, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), entry, 1, 3, 1, 1);
  g_signal_connect (entry,
                    "changed",
                    G_CALLBACK (entry_changed),
                    &task->tags);

  label = gtk_label_new ("Due");
  entry = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (entry), task->due);
  gtk_grid_attach (GTK_GRID (grid), label, 0, 4, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), entry, 1, 4, 1, 1);
  g_signal_connect (entry,
                    "changed",
                    G_CALLBACK (entry_changed),
                    &task->due);

  label = gtk_label_new ("Scheduled");
  entry = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (entry), task->scheduled);
  gtk_grid_attach (GTK_GRID (grid), label, 0, 5, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), entry, 1, 5, 1, 1);
  g_signal_connect (entry,
                    "changed",
                    G_CALLBACK (entry_changed),
                    &task->scheduled);

  label = gtk_label_new ("priority");
  combo = gtk_combo_box_text_new ();
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), "");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), "L");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), "M");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), "H");
  gtk_combo_box_set_active (GTK_COMBO_BOX (combo), 0);
  gtk_grid_attach (GTK_GRID (grid), label, 0, 6, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), combo, 1, 6, 1, 1);
  g_signal_connect (combo,
                    "changed",
                    G_CALLBACK (combo_changed),
                    &task->priority);

  /*Ensure that the dialog box is destroyed when the user responds*/
  gtk_container_add (GTK_CONTAINER (content_area), grid);

  gtk_widget_grab_focus (gtk_dialog_get_widget_for_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK));
  return dialog;
}
