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

/****f* Core/semaphore_release
 * SUMMARY
 * Release a semaphore.
 *
 * SYNOPSIS
 */

status_t semaphore_release (int32_t sid, int32_t tokens, int32_t flags)

/*
 * ARGUMENTS
 * * sid : a semaphore ID
 * * tokens : the number of tokens to release
 * * flags : the flags of the operation
 *
 * RESULT
 * * DNA_BAD_SEM_ID if the argument sid is invalid
 * * DNA_OK in case of success
 *
 * SOURCE
 */

{
  thread_t thread = NULL;
  semaphore_t sem = NULL;
  interrupt_status_t it_status = 0;
  status_t status = DNA_OK;

  watch (status_t)
  {
    ensure (sid >= 0 && sid < DNA_MAX_SEM, DNA_BAD_SEM_ID);
    ensure (tokens > 0, DNA_ERROR);

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
     * Decide what to do according to the number
     * of tokens required by a potential waiting thread
     */

    while (tokens != 0)
    {
      thread = queue_rem (& sem -> waiting_queue);

      if (thread != NULL)
      {
        if (thread -> sem_tokens <= tokens)
        {
          tokens -= thread -> sem_tokens;
          thread -> sem_tokens = 0;

          scheduler_dispatch (thread);
        }
        else
        {
          thread -> sem_tokens -= tokens;
          tokens = 0;

          queue_pushback (& sem -> waiting_queue, & thread -> status_link);
        }
      }
      else
      {
        break;
      }
    }

    /*
     * Add the remaining number of tokens
     */

    sem -> tokens += tokens;

    /*
     * We release the sem's lock
     */

    lock_release (& sem -> lock);

    /*
     * Now we deal with the reschedule
     */

    if ((flags & DNA_NO_RESCHEDULE) != 0)
    {
      thread_yield ();
    }
 
    cpu_trap_restore(it_status);
    return status;
  }

  rescue (invalid_semaphore)
  {
    lock_release (& sem_pool . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

