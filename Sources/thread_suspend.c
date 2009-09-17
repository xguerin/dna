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
  status_t status;
  thread_t target = NULL;
  uint32_t current_cpuid = cpu_mp_id();
  thread_t thread = scheduler . thread[id];
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_BAD_ARGUMENT);

    it_status = cpu_trap_mask_and_backup ();
    lock_acquire (& thread -> lock);

    check (bad_status, thread -> info . status != DNA_THREAD_ZOMBIE, DNA_ERROR);
    check (bad_status, thread -> info . status != DNA_THREAD_SLEEP, DNA_ERROR);

    if (thread -> info . status == DNA_THREAD_RUNNING)
    {
      if (thread -> info . cpu_id == current_cpuid)
      {
        thread -> info . status = DNA_THREAD_SLEEP;
        lock_release (& thread -> lock);

        status = scheduler_elect (& target, true);
        ensure (status != DNA_ERROR && status != DNA_BAD_ARGUMENT, status);

        status = scheduler_switch (target, NULL);
        ensure (status == DNA_OK, status);
      }
      else
      {
        cpu_mp_send_ipi (thread -> info . cpu_id,
            DNA_IPI_SUSPEND, (void *) thread);
      }
    }
    else
    {
      thread -> info . previous_status = thread -> info . status;
      thread -> info . status = DNA_THREAD_SLEEP;

      if (thread -> info . previous_status == DNA_THREAD_READY)
      {
        lock_acquire (& scheduler . xt[thread -> info . cpu_affinity] . lock);
        lock_release (& thread -> lock);

        queue_extract (& scheduler . xt[thread -> info . cpu_affinity], thread);
        lock_release (& scheduler . xt[thread -> info . cpu_affinity] . lock);
      }
      else lock_release (& thread -> lock);
    }

    cpu_trap_restore(it_status);
    return DNA_OK;
  }

  rescue (bad_status)
  {
    lock_release (& thread -> lock);
    leave;
  }
}

/*
 ****/

