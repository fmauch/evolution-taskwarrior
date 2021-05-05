/*
 * Copyright (C) 2016 Red Hat, Inc. (www.redhat.com)
 * Copyright (C) 2021 Felix Exner (git@fexner.de)
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib/gi18n-lib.h>

#include <gtk/gtk.h>
#include <shell/e-shell-view.h>

#include <mail/e-mail-paned-view.h>
#include <mail/e-mail-reader.h>
#include <mail/e-mail-view.h>
#include <mail/message-list.h>

#include "m-mail-ui.h"
#include "m-task-ui.h"
#include "m-utils.h"

static void
task_refinement_cb (GtkDialog *dialog,
                    gint response_id,
                    Task *task)
{
  /*This will cause the dialog to be destroyed*/
  gtk_widget_destroy (GTK_WIDGET (dialog));

  if (response_id == GTK_RESPONSE_OK)
    {
      size_t needed =
          snprintf (NULL, 0,
                    "task add +%s description:\"%s\" project:%s due:%s scheduled:%s priority:%s \"%s\"",
                    task->tags,
                    task->description,
                    task->project,
                    task->due,
                    task->scheduled,
                    task->priority);

      char *command = malloc (needed);
      sprintf (command,
               "task add +%s description:\"%s\" project:%s due:%s scheduled:%s priority:%s \"%s\"",
               task->tags,
               task->description,
               task->project,
               task->due,
               task->scheduled,
               task->priority);
      g_print ("Executing: %s\n", command);
      system (command);
    }
}

static void
action_message_cb (GtkAction *action,
                   EShellView *shell_view)
{
  EShellContent *shell_content;
  EMailView *mail_view = NULL;
  EShellBackend *shell_backend;

  GPtrArray *selected_uids = NULL;
  GtkWindow *window = NULL;
  CamelFolder *folder = NULL;
  guint ii;
  CamelMessageInfo *info = NULL;
  const gchar *message_uid;
  const gchar *config_dir;
  gchar *cfg_file;

  g_return_if_fail (E_IS_SHELL_VIEW (shell_view));

  shell_content = e_shell_view_get_shell_content (shell_view);
  shell_backend = e_shell_view_get_shell_backend (shell_view);
  config_dir = e_shell_backend_get_config_dir (shell_backend);
  cfg_file = g_build_filename (e_get_user_config_dir (), "evolution-taskwarrior.ini", NULL);

  g_object_get (shell_content, "mail-view", &mail_view, NULL);
  if (E_IS_MAIL_PANED_VIEW (mail_view))
    {
      selected_uids = e_mail_reader_get_selected_uids (E_MAIL_READER (mail_view));
      folder = e_mail_reader_ref_folder (E_MAIL_READER (mail_view));
      window = e_mail_reader_get_window (E_MAIL_READER (mail_view));
    }

  g_return_if_fail (E_IS_SHELL_VIEW (shell_view));
  for (ii = 0; selected_uids && ii < selected_uids->len; ii++)
    {
      message_uid = g_ptr_array_index (selected_uids, ii);
      info = camel_folder_get_message_info (folder, message_uid);

      Task *task = create_default_task (cfg_file);
      size_t needed = snprintf (NULL, 0, "%s (From: %s)", camel_message_info_get_subject (info), camel_message_info_get_from (info)) + 1;
      task->description = malloc (needed);
      sprintf (task->description, "%s (From: %s)", camel_message_info_get_subject (info), camel_message_info_get_from (info));
      GtkWidget *dialog = create_task_dialog (window, task);
      g_signal_connect (dialog,
                        "response",
                        G_CALLBACK (task_refinement_cb),
                        task);

      gtk_widget_show_all (dialog);
    }

  if (selected_uids)
    g_ptr_array_unref (selected_uids);
  g_clear_object (&folder);
}

static GtkActionEntry mail_message_context_entries[] = {
  { "my-message-action",
    "document-new",
    N_ ("To Task Warrior"),
    NULL,
    N_ ("Create TaskWarrior task"),
    G_CALLBACK (action_message_cb) }
};

static void
m_mail_ui_update_actions_cb (EShellView *shell_view,
                             GtkActionEntry *entries)
{
  EShellWindow *shell_window;
  EShellContent *shell_content;
  EMailView *mail_view = NULL;
  GtkActionGroup *action_group;
  GtkUIManager *ui_manager;
  gchar *selected_path = NULL;
  gboolean account_node = FALSE;
  gboolean folder_node = FALSE;
  gboolean has_message = FALSE;

  /* To get to messages in the separate window (those when double-cliecked in the message list),
	   a new extension extending E_TYPE_MAIL_BROWSER is required. The EMailBrowser implements
	   EMailReader, thus it's easier to get to the message list there. */
  shell_content = e_shell_view_get_shell_content (shell_view);
  g_object_get (shell_content, "mail-view", &mail_view, NULL);
  if (E_IS_MAIL_PANED_VIEW (mail_view))
    {
      GtkWidget *message_list;

      message_list = e_mail_reader_get_message_list (E_MAIL_READER (mail_view));
      has_message = message_list_selected_count (MESSAGE_LIST (message_list)) > 0;
    }

  shell_window = e_shell_view_get_shell_window (shell_view);
  ui_manager = e_shell_window_get_ui_manager (shell_window);
  action_group = e_lookup_action_group (ui_manager, "mail");

  m_utils_enable_actions (action_group, mail_message_context_entries, G_N_ELEMENTS (mail_message_context_entries), has_message);
}

void
m_mail_ui_init (GtkUIManager *ui_manager,
                EShellView *shell_view,
                gchar **ui_definition)
{
  const gchar *ui_def =
      "<toolbar name=\"main-toolbar\">\n"
      "  <placeholder name=\"mail-toolbar-common\">\n"
      "    <toolitem action=\"my-message-action\"/>\n"
      "  </placeholder>\n"
      "</toolbar>\n"
      "<popup name=\"mail-message-popup\">\n"
      "  <placeholder name=\"mail-message-popup-common-actions\">\n"
      "    <menuitem action=\"my-message-action\"/>\n"
      "  </placeholder>\n"
      "</popup>\n";

  EShellWindow *shell_window;
  GtkActionGroup *action_group;

  g_return_if_fail (ui_definition != NULL);

  *ui_definition = g_strdup (ui_def);

  shell_window = e_shell_view_get_shell_window (shell_view);
  action_group = e_shell_window_get_action_group (shell_window, "mail");

  /* Add actions to the action group. */
  e_action_group_add_actions_localized (
      action_group, GETTEXT_PACKAGE,
      mail_message_context_entries, G_N_ELEMENTS (mail_message_context_entries), shell_view);

  /* Decide whether we want this option to be visible or not */
  g_signal_connect (
      shell_view, "update-actions",
      G_CALLBACK (m_mail_ui_update_actions_cb),
      shell_view);
}

Task *
create_default_task (gchar *cfg_file)
{
  GKeyFile *keyfile;
  keyfile = g_key_file_new ();

  Task *task = create_task ();
  g_autoptr (GError) error = NULL;
  if (g_key_file_load_from_file (keyfile, cfg_file, 0, NULL))
    {
      {
        char* val = g_key_file_get_string (keyfile, "Default task", "project", NULL);
        if (val != NULL)
        {
          task->project = val;
        }
      }
      {
        char* val = g_key_file_get_string (keyfile, "Default task", "tags", NULL);
        if (val != NULL)
        {
          task->tags = val;
        }
      }
      {
        char* val = g_key_file_get_string (keyfile, "Default task", "due", NULL);
        if (val != NULL)
        {
          task->due = val;
        }
      }
      {
        char* val = g_key_file_get_string (keyfile, "Default task", "scheduled", NULL);
        if (val != NULL)
        {
          task->scheduled = val;
        }
      }
      {
        char* val = g_key_file_get_string (keyfile, "Default task", "priority", NULL);
        if (val != NULL)
        {
          task->priority = val;
        }
      }
      error = NULL;
    }
  else
    {
      g_warning ("Could not find configuration file: %s. Using default values.", cfg_file);
    }
  return task;
}
