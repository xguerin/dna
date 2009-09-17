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

/****f* Core/scheduler_dispatch
 * SUMMARY
 * Dispatch a thread.
 *
 * SYNOPSIS
 */

status_t scheduler_dispatch (thread_t thread)

/*
 * ARGUMENTS
 * * A valid thread_t 
 *
 * SOURCE
 */

{
  int32_t next_cpuid;
  status_t status;

  watch (status_t)
  {
    status = scheduler_pop (thread -> info . cpu_affinity, & next_cpuid);
    ensure (status != DNA_BAD_ARGUMENT, status);

    switch (status)
    {
      case DNA_OK :
        cpu_mp_send_ipi (next_cpuid, DNA_IPI_EXECUTE, thread);
        break;

      case DNA_NO_AVAILABLE_CPU :
        lock_acquire (& scheduler . xt[thread -> info . cpu_affinity] . lock);
        queue_add (& scheduler . xt[thread -> info . cpu_affinity], thread);
        lock_release (& scheduler . xt[thread -> info . cpu_affinity] . lock);
        break;

      default:
        return DNA_ERROR;
    }

    return DNA_OK;
  }
}

/*
 ****/

