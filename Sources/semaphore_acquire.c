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

/****f* Core/semaphore_acquire
 * SUMMARY
 * Acquire a semaphore.
 *
 * SYNOPSIS
 */

status_t semaphore_acquire (int32_t sid, int32_t flags, bigtime_t timeout)

/*
 * ARGUMENTS
 * * sid : the ID of the semaphore to acquire.
 * * flags : flags of the operation.
 * * timeout : time, in millisecond, when the acquire process must abort.
 *
 * RESULT
 * * DNA_BAD_SEM_ID if the sid parameter is invalid.
 * * DNA_WOULD_BLOCK if the semaphore is already locked and the timeout is 0.
 * * DNA_OK if the operation succeded.
 *
 * SOURCE
 */

{
	uint32_t current_cpuid = cpu_mp_id();
	thread_t self = scheduler . cpu[current_cpuid] . current_thread;
	thread_t thread = NULL;
	semaphore_t sem = NULL;
	int32_t alarm;
	interrupt_status_t it_status = 0;
	status_t status = DNA_OK;

  watch (status_t)
  {
    ensure (sid >= 0 && sid < DNA_MAX_SEM, DNA_BAD_SEM_ID);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& sem_pool . lock);

    /*
     * Look for the semaphore with ID sid
     */

    sem = sem_pool . semaphore[sid];
    check (invalid_semaphore, sem != NULL, DNA_BAD_SEM_ID);

    lock_acquire (& sem -> lock);
    lock_release (& sem_pool . lock);

    /*
     * Remove one token, and decide what to do according to the
     * timeout value if there are no tokens left
     */

    sem -> tokens -= 1;

    if (sem -> tokens < 0)
    {
      switch (timeout)
      {
        case -1 : /* Just wait */
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

          break;

        case 0 : /* Test if we are in a blocking situation */
          sem -> tokens += 1;
          status = DNA_WOULD_BLOCK;

          break;

        default : /* Wait for timeout µS and retry */
          status = time_set_alarm (timeout, DNA_RELATIVE_ALARM
              | DNA_ONE_SHOT_ALARM, semaphore_alarm, self, & alarm);

          check (invalid_alarm, status == DNA_OK, status);

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

          if (time_cancel_alarm (alarm) == DNA_ERROR)
          {
            queue_extract (& sem -> waiting_queue, & self -> status_link);
            status = DNA_TIMED_OUT;
            sem -> tokens += 1;
          }
          else
          {
            status = DNA_OK;
          }

          break;
      }
    }
    
    if (status == DNA_OK) sem -> latest_holder = self;

    lock_release (& sem -> lock);
    cpu_trap_restore(it_status);

    return status;
  }

  rescue (invalid_semaphore)
  {
    lock_release (& sem_pool . lock);
    cpu_trap_restore(it_status);
    leave;
  }

  rescue (invalid_alarm)
  {
    lock_release (& sem -> lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

