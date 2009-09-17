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

/****f* Core/semaphore_acquire
 * SUMMARY
 * Acquire a semaphore.
 *
 * SYNOPSIS
 */

status_t semaphore_acquire (int32_t sid, int32_t tokens,
    int32_t flags, bigtime_t timeout)

/*
 * ARGUMENTS
 * * sid : the ID of the semaphore to acquire.
 * * tokens : the number of tokens to acquire
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
  int32_t alarm, rem_tokens;
  interrupt_status_t it_status = 0;
  status_t status = DNA_OK;

  watch (status_t)
  {
    ensure (sid >= 0 && sid < DNA_MAX_SEM, DNA_BAD_SEM_ID);
    ensure (tokens > 0, DNA_BAD_ARGUMENT);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& semaphore_pool . lock);

    /*
     * Look for the semaphore with ID sid
     */

    sem = semaphore_pool . semaphore[sid];
    check (invalid_semaphore, sem != NULL, DNA_BAD_SEM_ID);

    lock_acquire (& sem -> lock);
    lock_release (& semaphore_pool . lock);

    /*
     * Remove the necessary tokens
     */

    rem_tokens = sem -> tokens - tokens;

    /*
     * And decide what to do next depending on the result
     */

    if (rem_tokens >= 0) sem -> tokens -= tokens;
    else
    {
      sem -> tokens = 0;

      switch (timeout)
      {
        /*
         * First choice, we just wait
         */

        case -1 :
          lock_acquire (& self -> lock);
          self -> info . sem_tokens = -1 * rem_tokens;
          self -> info . status = DNA_THREAD_WAIT;
          lock_release (& self -> lock);

          lock_acquire (& sem -> waiting_queue . lock);
          lock_release (& sem -> lock);

          /*
           * Elect a the next thread and run it
           * If target is IDLE, we can safely push the CPU
           * since we disabled the interrupts.
           */

          status = scheduler_elect (& thread, true);
          ensure (status != DNA_ERROR && status != DNA_BAD_ARGUMENT, status);

          status = scheduler_switch (thread, & sem -> waiting_queue);
          ensure (status == DNA_OK, status);

          lock_acquire (& sem -> lock);
          break;

        /*
         * Second choice, it was just a check, so 
         * we return DNA_WOULD_BLOCK
         */

        case 0 :
          sem -> tokens = rem_tokens + tokens;
          status = DNA_WOULD_BLOCK;

          break;

        /*
         * Last choice, we set a timer
         */

        default :
          status = time_set_alarm (timeout, DNA_RELATIVE_ALARM
              | DNA_ONE_SHOT_ALARM, thread_alarm, self, & alarm);
          check (invalid_alarm, status == DNA_OK, status);

          lock_acquire (& self -> lock);
          self -> info . sem_tokens = -1 * rem_tokens;
          self -> info . status = DNA_THREAD_WAIT;
          lock_release (& self -> lock);

          lock_acquire (& sem -> waiting_queue . lock);
          lock_release (& sem -> lock);

          /*
           * Elect a the next thread and run it
           * If target is IDLE, we can safely push the CPU
           * since we disabled the interrupts.
           */

          status = scheduler_elect (& thread, true);
          ensure (status != DNA_ERROR && status != DNA_BAD_ARGUMENT, status);

          status = scheduler_switch (thread, & sem -> waiting_queue);
          ensure (status == DNA_OK, status);

          lock_acquire (& sem -> lock);

          status = time_cancel_alarm (alarm);
          check (invalid_alarm, status != DNA_NO_TIMER
              && status != DNA_BAD_ARGUMENT, status);

          if (status == DNA_UNKNOWN_ALARM)
          {
            queue_extract (& sem -> waiting_queue, self);
            status = DNA_TIMED_OUT;
            sem -> tokens = rem_tokens + tokens;
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
    lock_release (& semaphore_pool . lock);
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

