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
  thread_t self = NULL;
  thread_t thread = NULL;
  semaphore_t sem = NULL;
  semaphore_id_t sem_id = { .raw = sid };
  status_t status = DNA_OK;
  int32_t alarm, rem_tokens;
  uint32_t current_cpuid = 0;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (tokens > 0, DNA_BAD_ARGUMENT);
    ensure (sem_id . s . index < DNA_MAX_SEM, DNA_BAD_SEM_ID);

    it_status = cpu_trap_mask_and_backup();
    current_cpuid = cpu_mp_id();
    self = cpu_pool . cpu[current_cpuid] . current_thread;

    /*
     * Look for the semaphore with ID sid
     */

    lock_acquire (& semaphore_pool . lock);

    sem = semaphore_pool . semaphore[sem_id . s . index];
    check (invalid_semaphore, sem != NULL, DNA_BAD_SEM_ID);
    check (invalid_semaphore, sem -> id . raw == sem_id . raw, DNA_BAD_SEM_ID);

    log (VERBOSE_LEVEL, "%d tokens on ID(%d:%d) TOKEN(%d)",
        tokens, sem_id . s . value,
        sem_id . s . index, sem -> info . tokens);

    lock_acquire (& sem -> lock);
    lock_release (& semaphore_pool . lock);

    /*
     * Remove the necessary tokens
     */

    rem_tokens = sem -> info . tokens - tokens;

    /*
     * And decide what to do next depending on the result
     */

    if (rem_tokens >= 0)
    {
      sem -> info . tokens -= tokens;
    }
    else
    {
      sem -> info . tokens = 0;

      switch (timeout)
      {
        /*
         * First choice, we just wait
         */

        case -1 :
          lock_acquire (& self -> lock);
          self -> info . sem_tokens = -1 * rem_tokens;

          self -> info . status = DNA_THREAD_WAITING;
          self -> info . previous_status = DNA_THREAD_RUNNING;

          self -> info . resource = DNA_RESOURCE_SEMAPHORE;
          self -> info . resource_id = sem -> id . raw;

          lock_acquire (& sem -> waiting_queue . lock);
          lock_release (& sem -> lock);

          status = scheduler_elect (& thread, true);
          ensure (status != DNA_ERROR && status != DNA_BAD_ARGUMENT, status);

          status = scheduler_switch (thread, & sem -> waiting_queue);
          ensure (status == DNA_OK, status);

          /*
           * TODO: check if the semaphore still exist
           */

          lock_acquire (& sem -> lock);
          break;

        /*
         * Second choice, it was just a check, so 
         * we return DNA_WOULD_BLOCK
         */

        case 0 :
          sem -> info . tokens = rem_tokens + tokens;
          status = DNA_WOULD_BLOCK;

          break;

        /*
         * Last choice, we set a timer
         */

        default :
          status = alarm_create (timeout, DNA_RELATIVE_ALARM
              | DNA_ONE_SHOT_ALARM, semaphore_alarm, self, & alarm);
          check (invalid_alarm, status == DNA_OK, status);

          lock_acquire (& self -> lock);
          self -> info . sem_tokens = -1 * rem_tokens;

          self -> info . status = DNA_THREAD_WAITING;
          self -> info . previous_status = DNA_THREAD_RUNNING;

          self -> info . resource = DNA_RESOURCE_SEMAPHORE;
          self -> info . resource_id = sem -> id . raw;

          lock_acquire (& sem -> waiting_queue . lock);
          lock_release (& sem -> lock);

          status = scheduler_elect (& thread, true);
          ensure (status != DNA_ERROR && status != DNA_BAD_ARGUMENT, status);

          status = scheduler_switch (thread, & sem -> waiting_queue);
          ensure (status == DNA_OK, status);

          /*
           * TODO: check if the semaphore still exist
           */

          lock_acquire (& sem -> lock);

          status = alarm_destroy (alarm);
          check (invalid_alarm, status != DNA_NO_TIMER, status);
          check (invalid_alarm, status != DNA_BAD_ARGUMENT, status);

          if (status == DNA_UNKNOWN_ALARM)
          {
            queue_extract (& sem -> waiting_queue, self);
            status = DNA_TIMED_OUT;
            sem -> info . tokens = rem_tokens + tokens;
          }
          else
          {
            status = DNA_OK;
          }

          break;
      }
    }
    
    if (status == DNA_OK)
    {
      sem -> info . latest_holder = self -> info . id;
    }

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

