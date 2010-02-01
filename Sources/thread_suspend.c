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

#include <stdbool.h>
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
  uint32_t current_cpuid = 0, next_cpuid = 0;
  thread_t thread;
  bool restart_stabilization_loop = true;
  thread_id_t tid = { .raw = id };
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (tid . s . group >= 0, DNA_BAD_ARGUMENT);
    ensure (tid . s . group < DNA_MAX_GROUP, DNA_BAD_ARGUMENT);
    ensure (tid . s . index >= 0, DNA_BAD_ARGUMENT);
    ensure (tid . s . index < DNA_MAX_THREAD, DNA_BAD_ARGUMENT);

    /*
     * Get some information about the execution.
     */

    it_status = cpu_trap_mask_and_backup ();
    current_cpuid = cpu_mp_id ();

    lock_acquire (& thread_pool . lock);
    thread = thread_pool . thread[tid . s . group][tid . s . index];

    check (bad_thread, thread != NULL &&
        thread -> id . raw == tid . raw , DNA_INVALID_THREAD_ID);

    /*
     * Stabilization loop: depending on the current status of
     * the thread, we will have to apply the banker algorithm
     * in several different ways.
     */

    do
    {
      /*
       * Lock the thread and check its status.
       */

      lock_acquire (& thread -> lock);

      check (thread_error,
          thread -> info . status != DNA_THREAD_ZOMBIE &&
          thread -> info . status != DNA_THREAD_SUSPENDED,
          DNA_ERROR);

      /*
       * Discriminate in function of its status.
       */

      switch (thread -> info . status)
      {
        case DNA_THREAD_RUNNING :
          {
            if (thread -> info . cpu_id == current_cpuid)
            {
              log (VERBOSE_LEVEL, "Local RUN suspend 0x%x.", thread -> id);

              thread -> info . status = DNA_THREAD_SUSPENDED;

              status = scheduler_elect (& target, true);
              check (thread_error, status != DNA_BAD_ARGUMENT, status);

              status = scheduler_switch (target, NULL);
              check (thread_error, status == DNA_OK, status);
            }
            else
            {
              log (VERBOSE_LEVEL, "Remote suspend %d on %d.",
                  thread -> id, thread -> info . cpu_id);

              next_cpuid = thread -> info . cpu_id;
              lock_release (& thread -> lock);

              cpu_mp_send_ipi (next_cpuid, DNA_IPI_SUSPEND,
                  (void *) thread -> id . raw);
            }

            restart_stabilization_loop = false;
            break;
          }

        case DNA_THREAD_READY :
          {
            /*
             * Apply the banker's algorithm to lock both the thread
             * and the ready queue.
             */

            result = lock_try
              (& scheduler . queue[thread -> info . affinity] . lock, true);

            if (result != DNA_ERROR)
            {
              log (VERBOSE_LEVEL, "Local READY suspend 0x%x.", thread -> id);

              queue_extract
                (& scheduler . queue[thread -> info . affinity], thread);
              lock_release
                (& scheduler . queue[thread -> info . affinity] . lock);

              thread -> info . status = DNA_THREAD_SUSPENDED;
              restart_stabilization_loop = false;
            }

            lock_release (& thread -> lock);
            break;
          }

        case DNA_THREAD_SLEEPING :
          {
            log (VERBOSE_LEVEL, "Local SLEEP suspend %d.", thread -> id);

            /*
             * TODO : should we cancel the alarm ?
             */

            thread -> info . status = DNA_THREAD_SUSPENDED;
            lock_release (& thread -> lock);

            restart_stabilization_loop = false;
            break;
          }

        case DNA_THREAD_WAITING :
          {
            /*
             * Apply the banker's algorithm to lock both the thread
             * and the resource queue.
             */

            check (thread_error, thread -> resource_queue != NULL, DNA_ERROR);
            result = lock_try (& thread -> resource_queue -> lock, true);

            if (result != DNA_ERROR)
            {
              log (VERBOSE_LEVEL, "Local WAIT suspend %d.", thread -> id);

              queue_extract (thread -> resource_queue, thread);
              lock_release (& thread -> resource_queue -> lock);

              thread -> info . status = DNA_THREAD_SUSPENDED;
              restart_stabilization_loop = false;
            }

            lock_release (& thread -> lock);
            break;
          }

        default :
          {
            log (PANIC_LEVEL, "unknown thread status.");
            break;
          }
      }
    }
    while (restart_stabilization_loop);

    cpu_trap_restore (it_status);
    return DNA_OK;
  }

  rescue (thread_error)
  {
    lock_release (& thread -> lock);
  }

  rescue (bad_thread)
  {
    lock_release (& thread_pool . lock);
    cpu_trap_restore (it_status);
    leave;
  }
}

/*
 ****/

