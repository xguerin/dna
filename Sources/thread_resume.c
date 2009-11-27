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

/****t* Core/thread_resume
 * SUMMARY
 * Resume a thread.
 *
 * SYNOPSIS
 */

status_t thread_resume (int32_t id)

/*
 * ARGUMENTS
 * * id : a thread id.
 *
 * RESULT
 * * DNA_ERROR if the argument thread is NULL.
 * * DNA_OK if the operation succeeded.
 *
 * SOURCE
 */

{
  thread_t thread = scheduler . thread[id];
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    check (bad_thread, thread != NULL, DNA_UNKNOWN_THREAD);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& thread -> lock);

    check (bad_status,
        thread -> info . status == DNA_THREAD_SUSPENDED,
        DNA_ERROR);

    thread -> info . status = thread -> info . previous_status;

    if (thread -> info . status == DNA_THREAD_SLEEPING ||
        thread -> info . status == DNA_THREAD_WAITING)
    {
      thread -> info . previous_status = DNA_THREAD_RUNNING;
    }
    else thread -> info . previous_status = DNA_THREAD_NOSTATUS;

    if (thread -> info . status == DNA_THREAD_READY)
    {
      thread -> info . status = DNA_THREAD_READY;
      scheduler_dispatch (thread);
    }
    else lock_release (& thread -> lock);

    cpu_trap_restore (it_status);
    return DNA_OK;
  }

  rescue (bad_status)
  {
    lock_release (& thread -> lock);
  }

  rescue (bad_thread)
  {
    cpu_trap_restore (it_status);
    leave;
  }
}

/*
 ****/

