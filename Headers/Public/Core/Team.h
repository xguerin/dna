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

#ifndef DNA_CORE_TEAM_PUBLIC_H
#define DNA_CORE_TEAM_PUBLIC_H

#include <Core/Thread.h>
#include <DnaTools/DnaTools.h>

#define DNA_SYSTEM_TEAM "SystemTeam"

/****t* types/team_info_t
 * SUMMARY
 * Type of team information.
 *
 * SOURCE
 */

typedef struct _team_info
{
	char name[DNA_NAME_LENGTH];
}
team_info_t;

/*
 ****/

extern status_t team_create (char * name, thread_handler_t thread_handler,
    void * thread_arguments, int32_t * tid);

extern status_t team_find (char * name, int32_t * tid);
extern status_t team_get_info (int32_t id, team_info_t * team_info);

#endif

