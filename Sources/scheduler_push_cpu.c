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

/****f* Core/scheduler_push_cpu
 * SUMMARY
 * Push the CPU(id) as ready.
 *
 * SYNOPSIS
 */

status_t scheduler_push_cpu (void)

/*
 * SOURCE
 */

{
  uint32_t cpuid = cpu_mp_id ();

  watch (status_t)
  {
    ensure (scheduler . cpu[cpuid] . status != DNA_CPU_READY, DNA_BAD_ARGUMENT);

    scheduler . cpu[cpuid] . status = DNA_CPU_READY;

    lock_acquire (& scheduler . cpu_pool . lock);
    queue_add (& scheduler . cpu_pool, & scheduler . cpu[cpuid] . link);
    lock_release (& scheduler . cpu_pool . lock);

    return DNA_OK;
  }
}

/*
 ****/

