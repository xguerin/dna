/*
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

#include <Private/Core.h>
#include <DnaTools/DnaTools.h>

/****f* Core/team_find
 * SUMMARY
 * Finds a team by its name.
 *
 * SYNOPSIS
 */

status_t team_find (char * name, int32_t * tid)

/*
 * ARGUMENTS
 * * name : team's name
 * * tid : the placeholder of the team's ID
 *
 * RESULT
 * * DNA_OK and a valid int32_t in case of success
 * * DNA_UNKNOWN_TEAM if no team correspond to the given name.
 *
 * SOURCE
 */

{
  team_t team = NULL;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (tid != NULL, DNA_BAD_ARGUMENT);

    if (name == NULL)
    {
      *tid = scheduler . cpu[cpu_mp_id()] . current_team -> id;
      return DNA_OK;
    }
    else
    {
      it_status = cpu_trap_mask_and_backup();
      lock_acquire (& team_manager . lock);

      team = queue_lookup (& team_manager . team_list,
          team_name_inspector, (void *) name, NULL);

      lock_release (& team_manager . lock);
      cpu_trap_restore(it_status);

      ensure (team != NULL, DNA_UNKNOWN_TEAM);

      *tid =  team -> id;
      return DNA_OK;
    }
  }
}

/*
 ****/

