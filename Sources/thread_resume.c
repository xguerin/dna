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
#include <DnaLibrary/DnaLibrary.h>

/****t* Core/thread_resume
 * SUMMARY
 * Resume a thread.
 *
 * SYNOPSIS
 */

status_t thread_resume (int32_t id)

/*
 * ARGUMENTS
 * * id : a thread id.
 *
 * RESULT
 * * DNA_ERROR if the argument thread is NULL.
 * * DNA_OK if the operation succeeded.
 *
 * SOURCE
 */

{
	uint32_t current_cpuid = cpu_mp_id();
	team_t team = scheduler . cpu[current_cpuid] . current_team;
  thread_t target = NULL;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& team -> lock);

    target = queue_lookup (& team_manager . thread_list,
        thread_id_inspector, (void *) & id, NULL);

    check (invalid_thread, target != NULL, DNA_UNKNOWN_THREAD);

    lock_acquire (& target -> lock);
    lock_release (& team -> lock);

    target -> status = DNA_THREAD_READY;

    lock_acquire (& scheduler . xt[target -> cpu_affinity] . lock);
    lock_release (& target -> lock);

    queue_add (& scheduler . xt[target -> cpu_affinity],
        & target -> status_link);

    lock_release (& scheduler . xt[target -> cpu_affinity] . lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }

  rescue (invalid_thread)
  {
    lock_release (& team -> lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

