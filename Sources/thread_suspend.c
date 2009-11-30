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

/****f* Core/thread_suspend
 * SUMMARY
 * Suspend a thread.
 *
 * SYNOPSIS
 */

status_t thread_suspend (int32_t id)

/*
 * ARGUMENTS
 * * id : a thread id.
 *
 * RESULT
 * * DNA_ERROR if the argument lock is invalid.
 * * DNA_OK if the operation succeeded.
 *
 * SOURCE
 */

{
  status_t status, result;
  thread_t target = NULL;
  uint32_t current_cpuid = cpu_mp_id(), next_cpuid;
  thread_t thread = scheduler . thread[id];
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_BAD_ARGUMENT);
    it_status = cpu_trap_mask_and_backup ();

    /*
     * Apply banker's algorithm to lock the thread and the scheduler
     */

    do
    {
      lock_acquire (& thread -> lock);
      result = lock_try
        (& scheduler . xt[thread -> info . affinity] . lock, true);
      if (result == DNA_ERROR) lock_release (& thread -> lock);
    }
    while (result == DNA_ERROR);

    /*
     * Check the thread status
     */

    check (bad_status,
        thread -> info . status != DNA_THREAD_ZOMBIE &&
        thread -> info . status != DNA_THREAD_SUSPENDED,
        DNA_ERROR);

    /*
     * We need to extract the thread right away,
     * even if the thread is not present in the queue
     */

    queue_extract (& scheduler . xt[thread -> info . affinity], thread);
    lock_release (& scheduler . xt[thread -> info . affinity] . lock);

    /*
     * And now we deal with the thread according to its status
     */

    switch (thread -> info . status)
    {
      case DNA_THREAD_RUNNING :
        {
          if (thread -> info . cpu_id == current_cpuid)
          {
            thread -> info . status = DNA_THREAD_SUSPENDED;
            thread -> info . previous_status = DNA_THREAD_READY;

            log (VERBOSE_LEVEL, "Local RUN suspend %d.", thread -> info . id)

            status = scheduler_elect (& target, true);
            ensure (status != DNA_ERROR && status != DNA_BAD_ARGUMENT, status);

            status = scheduler_switch (target, NULL);
            ensure (status == DNA_OK, status);
          }
          else
          {
            log (VERBOSE_LEVEL, "Remote suspend %d on %d.",
                thread -> info . id, thread -> info . cpu_id);

            next_cpuid = thread -> info . cpu_id;

#if 0
            lock_acquire (& scheduler . cpu[next_cpuid] . ipi_lock);
#endif
            lock_release (& thread -> lock);

            cpu_mp_send_ipi (next_cpuid, DNA_IPI_SUSPEND,
                (void *) thread -> info . id);
          }

          break;
        }

      case DNA_THREAD_READY :
        {
          log (VERBOSE_LEVEL, "Local READY suspend %d.", thread -> info . id)

          thread -> info . status = DNA_THREAD_SUSPENDED;
          thread -> info . previous_status = DNA_THREAD_READY;

          lock_release (& thread -> lock);
          break;
        }

      case DNA_THREAD_SLEEPING :
        {
          log (VERBOSE_LEVEL, "Local SLEEP suspend %d.", thread -> info . id)

          thread -> info . status = DNA_THREAD_SUSPENDED;
          thread -> info . previous_status = DNA_THREAD_SLEEPING;

          lock_release (& thread -> lock);
          break;
        }

      case DNA_THREAD_WAITING :
        {
          log (VERBOSE_LEVEL, "Local WAIT suspend %d.", thread -> info . id)

          thread -> info . status = DNA_THREAD_SUSPENDED;
          thread -> info . previous_status = DNA_THREAD_WAITING;

          lock_release (& thread -> lock);
          break;
        }

      default :
        {
          log (PANIC_LEVEL, "unknown thread status.");
          for (;;);
          break;
        }
    }

    cpu_trap_restore (it_status);
    return DNA_OK;
  }

  rescue (bad_status)
  {
    log (PANIC_LEVEL, "Oups! bad status");

    lock_release (& thread -> lock);
    lock_release (& scheduler . xt[thread -> info . affinity] . lock);

    leave;
  }
}

/*
 ****/

