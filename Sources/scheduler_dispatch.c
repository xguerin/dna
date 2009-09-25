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
  cpu_t * cpu = NULL;
  status_t status = DNA_OK;
  int32_t affinity = -1;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_BAD_ARGUMENT);

    /*
     * Add the thread to the list it belongs to.
     */

    lock_acquire (& scheduler . xt[thread -> info . affinity] . lock);
    lock_release (& thread -> lock);

    queue_add (& scheduler . xt[thread -> info . affinity], thread);
    lock_release (& scheduler . xt[thread -> info . affinity] . lock);

    /*
     * Look for an available processor.
     */

    if (thread -> info . affinity == cpu_mp_count ())
    {
      lock_acquire (& scheduler . cpu_pool . lock);
      cpu = queue_rem (& scheduler . cpu_pool);
      lock_release (& scheduler . cpu_pool . lock);
    }
    else
    {
      lock_acquire (& scheduler . cpu_pool . lock);
      if (scheduler . cpu[thread -> info . affinity] . status == DNA_CPU_READY)
      {
        cpu = & scheduler . cpu[thread -> info . affinity] ;
        queue_extract (& scheduler . cpu_pool, cpu);
      }
      lock_release (& scheduler . cpu_pool . lock);
    }

    /*
     * Deal with the dispatch.
     */

    if (cpu != NULL)
    {
      log (VERBOSE_LEVEL, "Remote execute %d on %d.",
          thread -> info . id, next_cpuid);

      cpu -> status = DNA_CPU_RUNNING;
      lock_acquire (& cpu -> ipi_lock);
      cpu_mp_send_ipi (cpu -> id, DNA_IPI_YIELD, NULL);
    }
    else
    {
      affinity = thread -> info . affinity;
      cpu = & scheduler . cpu[cpu_mp_id ()];

      if (cpu -> current_thread == cpu -> idle_thread)
      {
        if (affinity == cpu_mp_count () || affinity == cpu_mp_id ())
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

