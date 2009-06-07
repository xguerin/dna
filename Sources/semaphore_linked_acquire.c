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
#include <DnaLibrary/DnaLibrary.h>
#include <Processor/Processor.h>

/****f* Core/semaphore_linked_acquire
 * SUMMARY
 * Acquire a semaphore after one has been released atomically.
 *
 * SYNOPSIS
 */

status_t semaphore_linked_acquire (int32_t sid, int32_t lsid)

/*
 * ARGUMENTS
 * * sid : the ID of the semaphore to acquire.
 * * lsid : the ID of the linked semaphore.
 *
 * RESULT
 * * DNA_BAD_SEM_ID if the sid parameter is invalid.
 * * DNA_OK if the operation succeded.
 *
 * SOURCE
 */

{
	uint32_t current_cpuid = cpu_mp_id();
	thread_t self = scheduler . cpu[current_cpuid] . current_thread;
	thread_t thread = NULL;
	semaphore_t sem = NULL, lsem = NULL;
	int32_t index = 0;
	interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (sid >= 0 && sid < DNA_MAX_SEM , DNA_BAD_SEM_ID);
    ensure (lsid >= 0 && lsid < DNA_MAX_SEM , DNA_BAD_SEM_ID);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& sem_pool . lock);

    /*
     * Check the first semaphore
     */

    sem = sem_pool . semaphore[sid];
    check (invalid_sem, sem != NULL, DNA_BAD_SEM_ID);

    lock_acquire (& sem -> lock);

    /*
     * Check the second semaphore
     */

    lsem = sem_pool . semaphore[lsid];
    check (invalid_lsem, index < DNA_MAX_SEM, DNA_BAD_SEM_ID);

    lock_acquire (& lsem -> lock);
    lock_release (& sem_pool . lock);

    /*
     * Release the second semaphore
     */

    lsem -> tokens += 1;

    lock_acquire (& lsem -> waiting_queue . lock);
    lock_release (& lsem -> lock);

    thread = queue_rem (& lsem -> waiting_queue);
    lock_release (& lsem -> waiting_queue . lock);

    if (thread != NULL) 
    {
      thread -> status = DNA_THREAD_READY;

      lock_acquire (& scheduler . xt[thread -> cpu_affinity] . lock);
      queue_add (& scheduler . xt[thread -> cpu_affinity],
          & thread -> status_link);
      lock_release (& scheduler . xt[thread -> cpu_affinity] . lock);
    }

    /*
     * Acquire the first semaphore
     */

    sem -> tokens -= 1;

    if (sem -> tokens < 0)
    {
      self -> status = DNA_THREAD_WAIT;

      lock_acquire (& sem -> waiting_queue . lock);
      lock_release (& sem -> lock);

      thread = scheduler_elect ();
      if (thread == NULL)
      {
        thread = scheduler . cpu[current_cpuid] . idle_thread;
      }

      scheduler_switch (thread, & sem -> waiting_queue);
      lock_acquire (& sem -> lock);
    }
    
    sem -> latest_holder = self;

    lock_release (& sem -> lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }

  rescue (invalid_lsem)
  {
    lock_release (& sem -> lock);
  }

  rescue (invalid_sem)
  {
    lock_release (& sem_pool . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

