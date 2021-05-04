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

#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

typedef struct TaskDefinition Task;

struct TaskDefinition
{
  char *title;
  char *description;
  char *project;
  char *tags;
  char *due;
  char *scheduled;
  char *priority;
};

Task *create_task ();
#endif // ifndef TASK_H_INCLUDED
