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

/****f* Core/team_get_info
 * SUMMARY
 * Get some information about a team.
 *
 * SYNOPSIS
 */

status_t team_get_info (int32_t id, team_info_t * info)

/*
 * ARGUMENTS
 * * id : a team id.
 * * info : a pointer to a team_info_t structure.
 *
 * RESULT
 * * DNA_OK in case of success.
 * * DNA_ERROR if the info pointer is NULL.
 * * DNA_INVALID_TEAM_ID if the id argument is not valid.
 *
 * SOURCE
 */

{
	team_t team = NULL;
	interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (info != NULL, DNA_ERROR);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& team_manager . lock);

    team = queue_lookup (& team_manager . team_list,
        team_id_inspector, (void *) & id, NULL);

    check (invalid_team, team != NULL, DNA_INVALID_TEAM_ID);

    lock_acquire (& team -> lock);
    lock_release (& team_manager . lock);

    dna_strcpy (info -> name, team -> name);

    lock_release (& team -> lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }

  rescue (invalid_team)
  {
    lock_release (& team_manager . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

