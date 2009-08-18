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
#include <Processor/Processor.h>

/****f* Core/scheduler_elect
 * SUMMARY
 * Elects a thread.
 *
 * SYNOPSIS
 */

status_t scheduler_elect (thread_t * p_thread, bool idle)

/*
 * ARGUMENTS
 * * p_thread : a pointer to a valid thread_t variable
 *
 * RESULT
 * * DNA_OK if a valid thread has been elected
 * * DNA_ERROR othrerwise
 *
 * SOURCE
 */

{
  status_t status;
  thread_t thread = NULL;
  int32_t current_cpuid = cpu_mp_id();

  watch (status_t)
  {
    ensure (p_thread != NULL, DNA_BAD_ARGUMENT);

    /*
     * First, we look into the local thread list.
     */

    if (scheduler . xt[current_cpuid] . status != 0)
    {
      lock_acquire (& scheduler . xt[current_cpuid] . lock);
      thread = queue_rem (& scheduler . xt[current_cpuid]);
      lock_release (& scheduler . xt[current_cpuid] . lock);

      if (thread != NULL)
      {
        *p_thread = thread;
        return DNA_OK;
      }
    }

    /*
     * If nothing is available, we look into the global thread list.
     */

    if (scheduler . xt[cpu_mp_count] . status != 0)
    {
      lock_acquire (& scheduler . xt[cpu_mp_count] . lock);
      thread = queue_rem (& scheduler . xt[cpu_mp_count]);
      lock_release (& scheduler . xt[cpu_mp_count] . lock);

      if (thread != NULL)
      {
        *p_thread = thread;
        return DNA_OK;
      }
    }

    /*
     * If really nothing is available and idle is true then
     * return the current idle thread
     */

    if (idle)
    {
      status = scheduler_push_cpu ();
      ensure (status == DNA_OK, status);

      *p_thread = scheduler . cpu[current_cpuid] . idle_thread;
      return DNA_OK;
    }
    else
    {
      return DNA_NO_AVAILABLE_THREAD;
    }
  }
}

/*
 ****/

