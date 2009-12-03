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
    ensure (tid != NULL, DNA_BAD_ARGUMENT);

    if (name == NULL)
    {
      *tid = cpu_pool . cpu[cpu_mp_id()] . current_thread -> info . id;
      return DNA_OK;
    }
    else
    {
      it_status = cpu_trap_mask_and_backup();
      lock_acquire (& scheduler . lock);

      /*
       * Find the thread with name corresponding to name
       */

      for (int i = 0; i < DNA_MAX_THREAD; i += 1)
      {
        if (scheduler . thread[i] != NULL)
        {
          if (dna_strcmp (name, scheduler . thread[i] -> info . name) == 0)
          {
            thread = scheduler . thread[i];
            break;
          }
        }
      }

      lock_release (& scheduler . lock);
      cpu_trap_restore (it_status);

      ensure (thread != NULL, DNA_UNKNOWN_THREAD);

      /*
       * Return the ID of the thread
       */

      *tid = thread -> info . id;
      return DNA_OK;
    }
  }
}

/*
 ****/

