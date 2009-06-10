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
#include <Processor/Processor.h>

/****f* Core/scheduler_pop_cpu
 * SUMMARY
 * Get the next available CPU.
 *
 * SYNOPSIS
 */

int32_t scheduler_pop_cpu (void)

/*
 * RESULT
 * * A valid CPU ID
 * * -1 otherwise 
 *
 * SOURCE
 */

{
  int32_t cpu_id;

  lock_acquire (& scheduler . lock);
  cpu_id = scheduler . first_available;

  if (cpu_id != -1)
  {
    scheduler . cpu[0] . status = DNA_CPU_RUNNING;
    scheduler . first_available = scheduler . cpu[cpu_id] . next_available;
  }

  lock_release (& scheduler . lock);
	return cpu_id;
}

/*
 ****/

