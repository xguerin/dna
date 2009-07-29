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

/****f* Core/thread_get_info
 * SUMMARY
 * Gathers information about a thread.
 *
 * SYNOPSIS
 */

status_t thread_get_info (int32_t id, thread_info_t * info)

/*
 * ARGUMENTS
 * * id : a thread identifier.
 * * info : a pointer to a thread_info_t structure.
 *
 * RESULT
 * * DNA_OK if the operation succeeded.
 * * DNA_ERROR if the info argument is NULL.
 * * DNA_INVALID_THREAD_ID if the argument id is invalid.
 *
 * SOURCE
 */

{
  thread_t thread = NULL;
  bigtime_t current_time = 0;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (info != NULL, DNA_ERROR);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& team_manager . lock);

    thread = queue_lookup (& team_manager . thread_list,
        thread_id_inspector, (void *) & id, NULL);

    check (invalid_thread, thread != NULL, DNA_INVALID_THREAD_ID);

    lock_acquire (& thread -> lock);
    lock_release (& team_manager . lock);

    dna_strcpy (info -> name, thread -> name);
    info -> cpu_id = cpu_mp_id ();
    info -> cpu_affinity = thread -> cpu_affinity;
    info -> stack . base = thread -> signature . stack_base;
    info -> stack . size = thread -> signature . stack_size;
    info -> bootstrap . handler = thread -> signature . handler;
    info -> bootstrap . arguments = thread -> signature . arguments;

    if (thread -> status == DNA_THREAD_RUNNING)
    {
      time_manager . system_timer . get (& current_time);
      info -> kernel_time = thread -> kernel_time . elapsed
        + current_time - thread -> kernel_time . start;
    }
    else
    {
      info -> kernel_time = thread -> kernel_time . elapsed;
    }

    info -> user_time = thread -> user_time . elapsed;

    lock_release (& thread -> lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }

  rescue (invalid_thread)
  {
    lock_release (& team_manager . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

