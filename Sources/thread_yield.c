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

/****t* Core/thread_yield
 * SUMMARY
 * Yield some CPU time to someone else.
 *
 * SYNOPSIS
 */

status_t thread_yield (void)

/*
 * RESULT
 * * DNA_ERROR or DNA_BAD_ARGUMENT if something wrong happens with sched_elect
 * * DNA_OK otherwise
 *
 * SOURCE
 */

{
  status_t status;
  thread_t target = NULL;
  interrupt_status_t it_status = 0;
  thread_t self = scheduler . cpu[cpu_mp_id()] . current_thread;

  watch (status_t)
  {
    it_status = cpu_trap_mask_and_backup();

    status = scheduler_elect (& target);
    ensure (status != DNA_ERROR && status != DNA_BAD_ARGUMENT, status);

    if (status == DNA_OK)
    {
      self -> info . status = DNA_THREAD_READY;

      lock_acquire (& scheduler . xt[self -> info . cpu_affinity] . lock);

      status = scheduler_switch (target,
          & scheduler . xt[self -> info . cpu_affinity]);
      ensure (status == DNA_OK, status);
    }

    cpu_trap_restore(it_status);
    return DNA_OK;
  }
}

/*
 ****/

