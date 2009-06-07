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

/****f* Core/core_initialize
 * SUMMARY
 * Initializes the core component.
 *
 * SYNOPSIS
 */

status_t core_start (void)

/*
 * FUNCTION
 * Start the core component.
 *
 * SOURCE
 */

{
  uint32_t current_cpuid = cpu_mp_id ();
  thread_t thread = scheduler . cpu[current_cpuid] . idle_thread;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_ERROR);

    thread -> status = DNA_THREAD_RUNNING;
    cpu_context_load((& thread -> ctx));

    return DNA_OK;
  }
}

/*
 ****/

