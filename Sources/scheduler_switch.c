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

/****f* SchedulerPrivate/scheduler_switch
 * SUMMARY
 * Switches contexts between a thread and the current running thread.
 *
 * SYNOPSIS
 */

status_t scheduler_switch (thread_t thread, queue_t * queue)

/*
 * ARGUMENTS
 * * thread : a valid ready thread
 * * queue : the waiting queue where the current thread has to be stored
 *
 * RETURN
 * * DNA_BAD_ARGUMENT: thread is not valid
 * * DNA_OK: the operation succeeded
 *
 * SOURCE
 */

{
  uint32_t current_cpuid = cpu_mp_id ();
  bigtime_t current_time = 0;
  extern uint32_t __scheduler_switch_end;
  cpu_t * cpu = & cpu_pool . cpu[current_cpuid];
  thread_t self = cpu -> current_thread;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_BAD_ARGUMENT);

    /*
     * Compute the correct times if necessary
     */

    cpu_timer_get (current_cpuid, & current_time);
    self -> info . kernel_time += current_time - cpu -> lap_date;
    lock_release (& self -> lock);

    /*
     * Update the status of the target thread
     */

    thread -> info . status = DNA_THREAD_RUNNING;
    thread -> info . cpu_id = current_cpuid;
    lock_release (& thread -> lock);

    /*
     * Update the processor's status
     */

    cpu -> lap_date = current_time;
    cpu -> current_thread = thread;

    /*
     * Save the current context
     */

    cpu_context_save (& self -> context, & __scheduler_switch_end);

    /*
     * If queue is not NULL, then add the current thread to this queue
     */

    if (queue != NULL)
    {
      queue_add (queue, self);
      lock_release (& queue -> lock);
    }

    /*
     * Load the target context
     */

    cpu_context_load (& thread -> context);

    /*
     * FIXME: Find a better idea for what follows 
     */

    __asm__ volatile ("__scheduler_switch_end:");
    return DNA_OK;
  }
}

/*
 ****/

