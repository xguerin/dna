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
#include <MemoryManager/MemoryManager.h>
#include <Processor/Processor.h>

/****f* Core/thread_exit
 * SUMMARY
 * Exit from a thread.
 *
 * SYNOPSIS
 */

void thread_exit (int32_t value)

/*
 * ARGUMENTS
 * * value : the thread's return value
 *
 * SOURCE
 */

{
	uint32_t current_cpuid = cpu_mp_id();
	thread_t self = scheduler . cpu[current_cpuid] . current_thread;
	thread_t target = NULL, p = NULL;
	interrupt_status_t it_status = 0;

  /*
   * First, we lock ourselves
   */

	it_status = cpu_trap_mask_and_backup();
	lock_acquire (& self -> lock);

  /*
   * Then we can wake up the waiting threads
   */

	while ((p = queue_rem (& self -> wait)) != NULL)
  {
		p -> status = DNA_THREAD_READY;
		p -> signature . return_value = value;

		lock_acquire (& scheduler . xt[p -> cpu_affinity] . lock);

		queue_add (& scheduler . xt[p -> cpu_affinity],
				& p -> status_link);

		lock_release (& scheduler . xt[p -> cpu_affinity] . lock);
	}

  /*
   * Mark self as zombie
   */

  self -> status = DNA_THREAD_ZOMBIE;
	lock_release (& self -> lock);

  /*
   * Elect a the next thread and run it
   */

  if ((target = scheduler_elect ()) == NULL)
  {
    target = scheduler . cpu[current_cpuid] . idle_thread;
  }

  lock_acquire (& target -> lock);
  target -> status = DNA_THREAD_RUNNING;
  target -> cpu_id = current_cpuid;
  lock_release (& target -> lock);

  scheduler . cpu[current_cpuid] . current_thread = target;
  scheduler . cpu[current_cpuid] . current_team = target -> team;

  cpu_context_load((& target -> ctx));
}

/*
 ****/

