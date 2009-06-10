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
#include <stdint.h>
#include <Private/Core.h>

/****f* CireManager/thread_idle
 * SUMMARY
 * The idle thread.
 *
 * SYNOPSIS
 */

int32_t thread_idle (void * data)

/*
 * ARGUMENTS
 * Ignored. Argument data is here to keep GCC happy.
 *
 * RESULT
 * Ignored.
 *
 * SOURCE
 */

{
  int32_t thread = -1;
  int32_t current_cpuid = cpu_mp_id ();
  interrupt_status_t it_status;

  scheduler . cpu[current_cpuid] . status = DNA_CPU_READY;

  it_status = cpu_trap_mask_and_backup ();
  lock_acquire (& scheduler . lock);

  queue_add (& scheduler . cpu_pool, & scheduler . cpu[current_cpuid] . link);

  lock_release (& scheduler . lock);
  cpu_trap_restore (it_status);

  if (current_cpuid == 0)
  {
    thread_create (thread_root, NULL, "Root", DNA_NO_AFFINITY, 0x2000, & thread);
    thread_resume (thread);
  }

	while (true);
	return 0;
}

/*
 ****/

