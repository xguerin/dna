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

/****f* Core/semaphore_acquire
 * SUMMARY
 * Acquire a semaphore.
 *
 * SYNOPSIS
 */

status_t semaphore_acquire (int32_t id, int32_t tokens,
    int32_t flags, bigtime_t timeout)

/*
 * ARGUMENTS
 * * sid : the ID of the semaphore to acquire.
 * * tokens : the number of tokens to acquire
 * * flags : flags of the operation.
 * * timeout : time, in nanosecond, when the acquire process must abort.
 *
 * RESULT
 * * DNA_BAD_ARGUMENT if the tokens parameter is invalid.
 * * DNA_BAD_SEM_ID if the sid parameter is invalid.
 * * DNA_WOULD_BLOCK if the semaphore is already locked and the timeout is 0.
 * * DNA_SEM_DESTROYED if the semaphore has been deleted during a wait.
 * * DNA_OK if the operation succeded.
 *
 * SOURCE
 */

{
  bool has_alarm = false;
  thread_t self = NULL, thread = NULL;
  semaphore_t sem = NULL;
  semaphore_id_t sid = { .raw = id };
  status_t status = DNA_OK;
  int32_t alarm, rem_tokens;
  alarm_mode_t alarm_mode = DNA_ONE_SHOT_RELATIVE_ALARM;;
  uint32_t current_cpuid = 0;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (tokens > 0, DNA_BAD_ARGUMENT);
    ensure (sid . s . index < DNA_MAX_SEM, DNA_BAD_SEM_ID);

    /*
     * Check the flags for a timeout parameter.
     */

    if ((flags & DNA_RELATIVE_TIMEOUT) != 0)
    {
      has_alarm = true;
      alarm_mode = DNA_ONE_SHOT_RELATIVE_ALARM;
    }
    else if ((flags & DNA_ABSOLUTE_TIMEOUT) != 0)
    {
      has_alarm = true;
      alarm_mode = DNA_ONE_SHOT_ABSOLUTE_ALARM;
    }

    ensure (! has_alarm || (has_alarm && timeout >= 0), DNA_BAD_ARGUMENT);

    /*
     * Disable the interrupts, and get the current
     * execution environment (cpuid, self_id).
     */

    it_status = cpu_trap_mask_and_backup();
    current_cpuid = cpu_mp_id();
    self = cpu_pool . cpu[current_cpuid] . current_thread;

    /*
     * Look for the semaphore with ID sid.
     */

    lock_acquire (& semaphore_pool . lock);

    sem = semaphore_pool . semaphore[sid . s . index];
    check (bad_semid, sem != NULL, DNA_BAD_SEM_ID);
    check (bad_semid, sem -> id . raw == sid . raw, DNA_BAD_SEM_ID);

    log (VERBOSE_LEVEL, "%d tokens on ID(%d:%d) TOKEN(%d)",
        tokens, sid . s . value,
        sid . s . index, sem -> info . tokens);

    lock_acquire (& sem -> lock);
    lock_release (& semaphore_pool . lock);

    /*
     * Remove the necessary tokens and decide what
     * to do next depending on the result.
     */

    rem_tokens = sem -> info . tokens - tokens;

    if (rem_tokens >= 0)
    {
      /*
       * Everything is OK, take the requested tokens.
       */

      sem -> info . tokens -= tokens;
    }
    else
    {
      /*
       * Not enough tokens. Set the sem -> tokens to 0.
       */

      sem -> info . tokens = 0;

      /*
       * Take the action corresponding to the timeout.
       */

      if (has_alarm && timeout == 0)
      {
        sem -> info . tokens = rem_tokens + tokens;
        status = DNA_WOULD_BLOCK;
      }
      else
      {
        /*
         * Create the alarm if necessary.
         */

        if (has_alarm)
        {
          status = alarm_create (timeout, alarm_mode,
              semaphore_alarm, self, & alarm);
          check (invalid_alarm, status == DNA_OK, status);
        }

        /*
         * Update self information.
         */

        lock_acquire (& self -> lock);

        self -> resource_queue = & sem -> waiting_queue;
        self -> info . sem_tokens = -1 * rem_tokens;
        self -> info . status = DNA_THREAD_WAITING;
        self -> info . resource = DNA_RESOURCE_SEMAPHORE;
        self -> info . resource_id = sem -> id . raw;

        /*
         * Reschedule self.
         */

        lock_acquire (& sem -> waiting_queue . lock);
        lock_release (& sem -> lock);

        status = scheduler_elect (& thread, true);
        ensure (status != DNA_ERROR && status != DNA_BAD_ARGUMENT, status);

        status = scheduler_switch (thread, & sem -> waiting_queue);
        ensure (status == DNA_OK, status);

        /*
         * Check if the semaphore still exist.
         */

        lock_acquire (& semaphore_pool . lock);

        sem = semaphore_pool . semaphore[sid . s . index];
        check (bad_semid, sem != NULL, DNA_SEM_DESTROYED);
        check (bad_semid, sem -> id . raw == sid . raw, DNA_SEM_DESTROYED);

        lock_acquire (& sem -> lock);
        lock_release (& semaphore_pool . lock);

        /*
         * If necessary, check if the semaphore is ready,
         * or if we are here only due to the timeout.
         */

        if (has_alarm)
        {
          status = alarm_destroy (alarm);
          check (invalid_alarm, status != DNA_NO_TIMER, status);
          check (invalid_alarm, status != DNA_BAD_ARGUMENT, status);

          if (status == DNA_UNKNOWN_ALARM)
          {
            if (sem -> info . tokens >= tokens)
            {
              sem -> info . tokens -= tokens;
              self -> info . sem_tokens = 0;
              status = DNA_OK;
            }
            else
            {
              queue_extract (& sem -> waiting_queue, self);
              sem -> info . tokens = rem_tokens + tokens;
              status = DNA_TIMED_OUT;
            }
          }
        }
      }
    }
    
    if (status == DNA_OK)
    {
      sem -> info . latest_holder = self -> id . raw;
    }

    lock_release (& sem -> lock);
    cpu_trap_restore(it_status);

    return status;
  }

  rescue (bad_semid)
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

