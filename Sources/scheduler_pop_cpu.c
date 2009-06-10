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

int32_t scheduler_pop_cpu (int32_t affinity)

/*
 * RESULT
 * * A valid CPU ID
 * * -1 otherwise 
 *
 * SOURCE
 */

{
  int32_t cpu_id = -1;
  cpu_t * cpu = NULL;

  if (affinity == DNA_NO_AFFINITY)
  {
    if (scheduler . cpu_pool . status != 0)
    {
      lock_acquire (& scheduler . lock);
      cpu = queue_rem (& scheduler . cpu_pool);
      lock_release (& scheduler . lock);
    }

    if (cpu != NULL)
    {
      cpu -> status = DNA_CPU_RUNNING;
      cpu_id = cpu -> id;
    }
  }
  else
  {
    /*
     * Not supported yet
     */
  }

	return cpu_id;
}

/*
 * NOTE
 * Interrupts must ne disabled.
 ****/

