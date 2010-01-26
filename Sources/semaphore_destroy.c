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
#include <MemoryManager/MemoryManager.h>
#include <Processor/Processor.h>

/****f* Core/semaphore_destroy
 * SUMMARY
 * Destroy a semaphore_t.
 *
 * SYNOPSIS
 */

status_t semaphore_destroy (int32_t sid)

/*
 * ARGUMENTS
 * * sid : the semaphore id.
 *
 * RESULT
 * Not implemented.
 *
 * SOURCE
 */

{
  thread_t thread = NULL;
  semaphore_t sem = NULL;
  semaphore_id_t sem_id = { .raw = sid };
  interrupt_status_t it_status = 0;
  bool smart_to_reschedule = false;

  watch (status_t)
  {
    ensure (sem_id . s . index < DNA_MAX_SEM, DNA_BAD_SEM_ID);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& semaphore_pool . lock);

    /*
     * Look for the semaphore with ID sid. If found,
     * remove its entry from the pool.
     */

    sem = semaphore_pool . semaphore[sem_id . s . index];
    check (invalid_semaphore, sem != NULL, DNA_BAD_SEM_ID);
    check (invalid_semaphore, sem -> id . raw == sem_id . raw, DNA_BAD_SEM_ID);

    semaphore_pool . semaphore[sem_id . s . index] = NULL;

    lock_acquire (& sem -> lock);
    lock_release (& semaphore_pool . lock);

    /*
     * Reschedule each waiting thread, and
     * reset its information.
     */

    lock_acquire (& sem -> waiting_queue . lock);

    while ((thread = queue_rem (& sem -> waiting_queue)) != NULL)
    {
      lock_acquire (& thread -> lock);

      thread -> info . sem_tokens = 0;
      thread -> info . resource = DNA_NO_RESOURCE;
      thread -> info . resource_id = -1;

      if (thread -> info . status == DNA_THREAD_WAITING)
      {
        thread -> info . status = DNA_THREAD_READY;
        thread -> info . previous_status = DNA_THREAD_WAITING;

        if (scheduler_dispatch (thread) == DNA_INVOKE_SCHEDULER)
        {
          smart_to_reschedule = true;
        }
      }

      lock_release (& thread -> lock);
    }

    lock_release (& sem -> waiting_queue . lock);

    /*
     * Delete the semaphore's memory.
     */

    kernel_free (sem);
    return DNA_OK;
  }

  rescue (invalid_semaphore)
  {
    lock_release (& semaphore_pool . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

