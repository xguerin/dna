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

    lock_acquire (& scheduler . thread_queue[thread -> info . affinity] . lock);
    lock_release (& thread -> lock);

    queue_add (& scheduler . thread_queue[thread -> info . affinity], thread);

    /*
     * Look for an available processor.
     */

    lock_acquire (& cpu_pool . cpu_queue . lock);
    lock_release (& scheduler . thread_queue[thread -> info . affinity] . lock);

    if (thread -> info . affinity == cpu_mp_count ())
    {
      cpu = queue_rem (& cpu_pool . cpu_queue);
    }
    else
    {
      cpu = & cpu_pool . cpu[thread -> info . affinity] ;
      if (cpu -> status == DNA_CPU_READY)
      {
        queue_extract (& cpu_pool . cpu_queue, cpu);
      }
      else cpu = NULL;
    }

    lock_release (& cpu_pool . cpu_queue . lock);

    /*
     * Deal with the dispatch.
     */

    if (cpu != NULL && cpu -> id != cpu_mp_id ())
    {
      cpu_mp_send_ipi (cpu -> id, DNA_IPI_YIELD, NULL);
    }
    else
    {
      affinity = thread -> info . affinity;

      /*
       * Explanation of what follows: in the first part of the test,
       * we check whether a distant, compatible CPU is available. If
       * not, if the thread is compatible with the current processor,
       * and whether this processor is available or not, we return
       * that it is necessary to invoke the scheduler. It will be the
       * role of the calling function to decide what to do.
       */

      if (affinity == cpu_mp_count () || affinity == cpu_mp_id ())
      {
        status = DNA_INVOKE_SCHEDULER;
      }
    }

    return status;
  }
}

/*
 * NOTE
 * Interrupts must be disabled.
 ****/

