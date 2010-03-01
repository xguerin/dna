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

/****f* SchedulerPrivate/scheduler_elect
 * SUMMARY
 * Elects a thread.
 *
 * SYNOPSIS
 */

status_t scheduler_elect (thread_t * p_thread, bool with_idle)

/*
 * ARGUMENTS
 * * p_thread : a pointer to a valid thread_t variable
 *
 * RESULT
 * * DNA_BAD_ARGUMENT: one of the arguments is invalid
 * * DNA_NO_AVAILABLE_THREAD: the available thread and with_idle is false
 * * DNA_OK: the operation succeeded
 *
 * SOURCE
 */

{
  thread_t thread = NULL;
  int32_t current_cpuid = cpu_mp_id();

  watch (status_t)
  {
    ensure (p_thread != NULL, DNA_BAD_ARGUMENT);

    /*
     * Check the local queue
     */

    lock_acquire (& scheduler . queue[current_cpuid] . lock);
    thread = queue_rem (& scheduler . queue[current_cpuid]);

    if (thread != NULL)
    {
      lock_acquire (& thread -> lock);
      lock_release (& scheduler . queue[current_cpuid] . lock);

      *p_thread = thread;
      return DNA_OK;
    }

    /*
     * Check the global queue
     */

    lock_acquire (& scheduler . queue[cpu_mp_count ()] . lock);
    lock_release (& scheduler . queue[current_cpuid] . lock);

    thread = queue_rem (& scheduler . queue[cpu_mp_count ()]);

    if (thread != NULL)
    {
      lock_acquire (& thread -> lock);
      lock_release (& scheduler . queue[cpu_mp_count ()] . lock);

      *p_thread = thread;
      return DNA_OK;
    }

    /*
     * Return the IDLE thread
     */

    if (with_idle)
    {
      lock_acquire (& cpu_pool . cpu[current_cpuid] . lock);
      lock_release (& scheduler . queue[cpu_mp_count ()] . lock);

      log (VERBOSE_LEVEL, "Set CPU %d @ READY", current_cpuid);
      cpu_pool . cpu[current_cpuid] . status = DNA_CPU_READY;

      lock_acquire (& cpu_pool . queue . lock);
      lock_release (& cpu_pool . cpu[current_cpuid] . lock);

      queue_add (& cpu_pool . queue, & cpu_pool . cpu[current_cpuid]);
      lock_release (& cpu_pool . queue . lock);

      thread = cpu_pool . cpu[current_cpuid] . idle_thread;
      lock_acquire (& thread -> lock);

      *p_thread = thread;
      return DNA_OK;
    }
    else
    {
      lock_release (& scheduler . queue[cpu_mp_count ()] . lock);

      *p_thread = NULL;
      return DNA_NO_AVAILABLE_THREAD;
    }
  }
}

/*
 ****/

