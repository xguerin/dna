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
#include <MemoryManager/MemoryManager.h>
#include <VirtualFileSystem/VirtualFileSystem.h>
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
#if 0
  int32_t alarm_id;
#endif

  status_t status;
  uint32_t current_cpuid = cpu_mp_id ();
  thread_t thread = scheduler . cpu[current_cpuid] . idle_thread;
  team_t team = scheduler . cpu[cpu_mp_id ()] . current_team;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_ERROR);

    if (current_cpuid == 0)
    {
      status = fdarray_create (team -> id, -1);
      ensure (status == DNA_OK, status);

#if 0
      status = time_set_alarm (4000000, DNA_RELATIVE_ALARM |
          DNA_PERIODIC_ALARM, scheduler_alarm, NULL, & alarm_id);
      ensure (status == DNA_OK, status);
#endif
    }

    thread -> status = DNA_THREAD_RUNNING;
    cpu_context_load((& thread -> ctx));

    return DNA_OK;
  }
}

/*
 ****/

