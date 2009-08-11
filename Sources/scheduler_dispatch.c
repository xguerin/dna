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

void scheduler_dispatch (thread_t thread)

/*
 * ARGUMENTS
 * * A valid thread_t 
 *
 * SOURCE
 */

{
  int32_t next_cpuid;

  if ((next_cpuid = scheduler_pop_cpu (thread -> cpu_affinity)) != -1)
  {
    lock_acquire (& thread -> lock); 

    thread -> status = DNA_THREAD_RUNNING;

    lock_release (& thread -> lock);

    ipi_send (next_cpuid, DNA_IPI_EXECUTE, thread);
  }
  else
  {
    lock_acquire (& thread -> lock); 

    thread -> status = DNA_THREAD_READY;

    lock_acquire (& scheduler . xt[thread -> cpu_affinity] . lock);
    lock_release (& thread -> lock);

    queue_add (& scheduler . xt[thread -> cpu_affinity],
        & thread -> status_link);

    lock_release (& scheduler . xt[thread -> cpu_affinity] . lock);
  }
}

/*
 ****/

