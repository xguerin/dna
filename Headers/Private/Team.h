/****h* core/team
 * SUMMARY
 * Team management.
 ****
 * Copyright (C) 2007 TIMA Laboratory
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DNA_CORE_TEAM_PRIVATE_H
#define DNA_CORE_TEAM_PRIVATE_H

#include <stdint.h>
#include <stdarg.h>
#include <Core/Core.h>
#include <DnaTools/DnaTools.h>

/****t* types/team_t
 * SUMMARY
 * Type of a team.
 *
 * SOURCE
 */

typedef struct _team
{
  int32_t id;
  char name[DNA_NAME_LENGTH];

  spinlock_t lock;
  queue_t thread_list;
  queue_item_t sched_link;
}
* team_t;

/*
 ****/

/****t* team/team_manager_t
 * SUMMARY
 * Team and thread manager type.
 *
 * SOURCE
 */

typedef struct _team_manager
{
  spinlock_t lock;

  int32_t team_index;
  queue_t team_list;

  int32_t thread_index;
  queue_t thread_list;
}
team_manager_t;

/*
 ****/

extern team_manager_t team_manager;

extern status_t team_destroy (team_t team);

extern bool team_id_inspector (void * item, void * p_id, void * dummy);
extern bool team_name_inspector (void * item, void * name, void * dummy);

#endif

