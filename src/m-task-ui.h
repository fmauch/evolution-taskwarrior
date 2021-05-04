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

#ifndef M_TASK_UI_H_INCLUDED
#define M_TASK_UI_H_INCLUDED

#include "task.h"
#include <gtk/gtk.h>

GtkWidget *create_task_dialog (GtkWindow *parent, Task *task);
#endif // ifndef M_TASK_UI_H_INCLUDED
