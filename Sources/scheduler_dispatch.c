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

/****f* Core/scheduler_dispatch
 * SUMMARY
 * Dispatch a thread.
 *
 * SYNOPSIS
 */

status_t scheduler_dispatch (thread_t thread)

/*
 * ARGUMENTS
 * * A valid thread_t 
 *
 * SOURCE
 */

{
  cpu_t * current_cpu = NULL;
  status_t status = DNA_OK;
  int32_t next_cpuid = -1;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_BAD_ARGUMENT);

    /*
     * Look for an available processor
     */

    if (thread -> info . affinity == cpu_mp_count ())
    {
      lock_acquire (& scheduler . lock);

      for (int32_t i = 0; i < cpu_mp_count (); i += 1)
      {
        if (scheduler . cpu[i] . status == DNA_CPU_READY
            && ! scheduler . cpu[i] . on_hold)
        {
          scheduler . cpu[i] . on_hold = true;
          next_cpuid = i;
          break;
        }
      }

      lock_release (& scheduler . lock);
    }
    else
    {
      lock_acquire (& scheduler . lock);

      if (scheduler . cpu[thread -> info . affinity] . status == DNA_CPU_READY
          && ! scheduler . cpu[thread -> info . affinity] . on_hold)
      {
        scheduler . cpu[thread -> info . affinity] . on_hold = true;
        next_cpuid = thread -> info . affinity;
      }

      lock_release (& scheduler . lock);
    }

    /*
     * Deal with the dispatch
     */

    if (next_cpuid != -1)
    {
      log (VERBOSE_LEVEL, "Remote execute %d on %d.",
          thread -> info . id, next_cpuid);

      lock_acquire (& scheduler . cpu[next_cpuid] . ipi_lock);
      cpu_mp_send_ipi (next_cpuid, DNA_IPI_EXECUTE, thread);
    }
    else
    {
      log (VERBOSE_LEVEL, "Enqueue %d on %d.",
          thread -> info . id, thread -> info . affinity);

      lock_acquire (& scheduler . xt[thread -> info . affinity] . lock);
      lock_release (& thread -> lock);

      queue_add (& scheduler . xt[thread -> info . affinity], thread);
      lock_release (& scheduler . xt[thread -> info . affinity] . lock);

      /*
       * Check if we can yield the current processor
       */

      current_cpu = & scheduler . cpu[cpu_mp_id ()];

      if (current_cpu -> current_thread == current_cpu -> idle_thread)
      {
        if (thread -> info . affinity == cpu_mp_count ()
            || thread -> info . affinity == cpu_mp_id ())
        {
          status = DNA_INVOKE_SCHEDULER;
        }
      }
    }

    return status;
  }
}

/*
 * NOTE
 * Interrupts must be disabled.
 ****/

