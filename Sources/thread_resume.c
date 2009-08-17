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
  thread_t target = NULL;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& scheduler . lock);

    target = queue_lookup (& scheduler. thread_list,
        thread_id_inspector, (void *) & id, NULL);

    check (invalid_thread, target != NULL, DNA_UNKNOWN_THREAD);
    check (invalid_thread, target -> status == DNA_THREAD_SLEEP, DNA_ERROR);

    lock_release (& scheduler . lock);

    scheduler_dispatch (target);

    cpu_trap_restore(it_status);
    return DNA_OK;
  }

  rescue (invalid_thread)
  {
    lock_release (& scheduler . lock);
    cpu_trap_restore(it_status);

    leave;
  }
}

/*
 ****/

