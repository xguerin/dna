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

/****f* Core/scheduler_elect
 * SUMMARY
 * Elects a thread.
 *
 * SYNOPSIS
 */

thread_t scheduler_elect (void)

/*
 * RESULT
 * * A valid thread_t if something can be executed.
 * * NULL otherwise. If NULL is returned, the system will switch to idle.
 *
 * SOURCE
 */

{
  thread_t target = NULL;
  int32_t current_cpuid = cpu_mp_id();

  /*
   * First, we look into the local thread list.
   */

  if (scheduler . xt[current_cpuid] . status != 0)
  {
    lock_acquire (& scheduler . xt[current_cpuid] . lock);
    target = queue_rem (& scheduler . xt[current_cpuid]);
    lock_release (& scheduler . xt[current_cpuid] . lock);

    if (target != NULL) return target;
  }

  /*
   * If nothing is available, we look into the global thread list.
   */

  if (scheduler . xt[scheduler . xt_index] . status != 0)
  {
    lock_acquire (& scheduler . xt[scheduler . xt_index] . lock);
    target = queue_rem (& scheduler . xt[scheduler . xt_index]);
    lock_release (& scheduler . xt[scheduler . xt_index] . lock);
  }

  return target;
}

/*
 ****/

