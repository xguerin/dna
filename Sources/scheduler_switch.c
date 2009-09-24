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

/****f* Core/scheduler_switch
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
 * SOURCE
 */

{
  bool release_cpu = false;
  uint32_t current_cpuid = cpu_mp_id ();
  bigtime_t current_time = 0;
  extern uint32_t __scheduler_switch_end;
  cpu_t * cpu = & scheduler . cpu[current_cpuid];
  thread_t self = cpu -> current_thread;

  /*
   * Update the status of the current thread
   */

  self -> info . cpu_id = -1;

  /*
   * Compute the correct times if necessary
   */

  cpu_timer_get (current_cpuid, & current_time);
  self -> info . kernel_time = current_time;
  self -> info . kernel_time -= cpu -> lap_date;

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

  lock_release (& self -> lock);

  /*
   * Update the status of the target thread
   */

  if (thread == NULL)
  {
    release_cpu = true;
    thread = cpu -> idle_thread;
    lock_acquire (& thread -> lock);
  }

  thread -> info . previous_status = thread -> info . status;
  thread -> info . status = DNA_THREAD_RUNNING;
  thread -> info . cpu_id = current_cpuid;

  lock_acquire (& cpu -> lock);
  lock_release (& thread -> lock);

  /*
   * Update the processor's status
   */

  cpu -> lap_date = current_time;
  cpu -> current_thread = thread;

  if (release_cpu)
  {
    lock_acquire (& scheduler . lock);
    lock_release (& cpu -> lock);

    cpu -> status = DNA_CPU_READY;
    lock_release (& scheduler . lock);
  }
  else
  {
    if (cpu -> on_hold) cpu -> on_hold = false;

    lock_acquire (& scheduler . lock);
    lock_release (& cpu -> lock);

    cpu -> status = DNA_CPU_RUNNING;
    lock_release (& scheduler . lock);
  }

  /*
   * Load the target context
   */

  cpu_context_load (& thread -> context);

  /*
   * FIXME If someone has a better idea to get the
   * right address after cpu_context_load, one can change
   * what follows ...
   */

  __asm__ volatile ("__scheduler_switch_end:");

  return DNA_OK;
}

/*
 ****/

