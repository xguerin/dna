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

/****t* Core/thread_yield
 * SUMMARY
 * Yield some CPU time to someone else.
 *
 * SYNOPSIS
 */

status_t thread_yield (void)

/*
 * RESULT
 * DNA_OK.
 *
 * SOURCE
 */

{
	thread_t self = scheduler . cpu[cpu_mp_id()] . current_thread;
	thread_t thread = NULL;
	interrupt_status_t it_status = 0;

	it_status = cpu_trap_mask_and_backup();

	if ((thread = scheduler_elect ()) != NULL)
  {
		self -> status = DNA_THREAD_READY;

		if (self -> type != DNA_IDLE_THREAD)
    {
      lock_acquire (& scheduler . xt[self -> cpu_affinity] . lock);
      scheduler_switch (thread, & scheduler . xt[self -> cpu_affinity]);
    }
    else
    {
      scheduler_switch (thread, NULL);
    }
	}

	cpu_trap_restore(it_status);
	return DNA_OK;
}

/*
 ****/

