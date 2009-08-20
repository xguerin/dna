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
    ensure (id >= 0 && id < DNA_MAX_THREAD, DNA_BAD_ARGUMENT);
    ensure (info != NULL, DNA_BAD_ARGUMENT);

    /*
     * Get the thread corresponding to the current id
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& scheduler . lock);

    thread = scheduler . thread[id];
    ensure (thread != NULL, DNA_INVALID_THREAD_ID);

    lock_acquire (& thread -> lock);
    lock_release (& scheduler . lock);

    /*
     * Copy the thread information
     */

    *info = thread -> info;

    /*
     * Update the time of the running thread
     */

    if (thread -> info . status == DNA_THREAD_RUNNING)
    {
      time_manager . system_timer . get (thread -> info
          . cpu_id, & current_time);
      info -> kernel_time = current_time;
      info -> kernel_time -= scheduler . cpu[cpu_mp_id ()] . lap_date;
    }

    lock_release (& thread -> lock);
    cpu_trap_restore (it_status);

    return DNA_OK;
  }
}

/*
 ****/

