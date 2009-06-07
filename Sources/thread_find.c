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

/****f* Core/thread_find
 * SUMMARY
 * Finds a thread by its name.
 *
 * SYNOPSIS
 */

status_t thread_find (char * name, int32_t * tid)

/*
 * ARGUMENTS
 * * name : thread's name
 * * tid : a placeholder for the thread's ID
 *
 * RESULT
 * * DNA_OK and a valid thread ID if the operation succeeded
 * * DNA_UNKNOWN_THREAD if no thread correspond to name
 *
 * SOURCE
 */

{
	thread_t thread = NULL;
	interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (tid != NULL, DNA_ERROR);

    if (name == NULL)
    {
      *tid = scheduler . cpu[cpu_mp_id()] . current_thread -> id;
      return DNA_OK;
    }
    else
    {
      it_status = cpu_trap_mask_and_backup();
      lock_acquire (& team_manager . lock);

      thread = queue_lookup (& team_manager . thread_list,
          thread_name_inspector, (void *) name, NULL);

      lock_release (& team_manager . lock);
      cpu_trap_restore(it_status);

      ensure (thread != NULL, DNA_UNKNOWN_THREAD);

      *tid = thread -> id;
      return DNA_OK;
    }
  }
}

/*
 ****/

