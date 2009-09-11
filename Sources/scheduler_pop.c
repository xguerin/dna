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

/****f* Core/scheduler_pop
 * SUMMARY
 * Get the next available CPU.
 *
 * SYNOPSIS
 */

status_t scheduler_pop (int32_t affinity, int32_t * p_id)

/*
 * ARGUMENTS
 * * affinity : specifies the required processor affinity
 * * p_id : pointer to the variable that will receive the processor id
 *
 * RESULT
 * * DNA_OK if a valid CPU can be found
 * * DNA_ERROR otherwise
 *
 * SOURCE
 */

{
  int32_t id = -1;
  cpu_t * cpu = NULL;
  status_t status = DNA_NO_AVAILABLE_CPU;

  watch (status_t)
  {
    ensure (p_id != NULL, DNA_BAD_ARGUMENT);
    ensure (affinity >= 0 && affinity <= CPU_MP_COUNT, DNA_BAD_ARGUMENT);

    if (affinity == CPU_MP_COUNT)
    {
      if (scheduler . cpu_pool . status != 0)
      {
        lock_acquire (& scheduler . cpu_pool . lock);

        cpu = queue_rem (& scheduler . cpu_pool);
        check (no_cpu, cpu != NULL, DNA_NO_AVAILABLE_CPU);

        lock_acquire (& cpu -> lock);
        lock_release (& scheduler . cpu_pool . lock);

        cpu -> status = DNA_CPU_RUNNING;
        id = cpu -> id;
        status = DNA_OK;

        lock_release (& cpu -> lock);
      }
    }
    else
    {
      lock_acquire (& scheduler . cpu_pool . lock);

      check (no_cpu,
          scheduler . cpu[affinity] . status == DNA_CPU_READY,
          DNA_NO_AVAILABLE_CPU);

      lock_acquire (& scheduler . cpu[affinity] . lock);

      queue_extract (& scheduler . cpu_pool, & scheduler . cpu[affinity]);

      scheduler . cpu[affinity] . status = DNA_CPU_RUNNING;
      id = scheduler . cpu[affinity] . id;

      lock_release (& scheduler . cpu[affinity] . lock);
      lock_release (& scheduler . cpu_pool . lock);
    }

    *p_id = id;
    return status;
  }

  rescue (no_cpu)
  {
    lock_release (& scheduler . cpu_pool . lock);
    leave;
  }
}

/*
 * NOTE
 * Interrupts must ne disabled.
 ****/

