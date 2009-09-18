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
#include <Processor/Processor.h>

/****f* Core/thread_exit
 * SUMMARY
 * Exit from a thread.
 *
 * SYNOPSIS
 */

void thread_exit (int32_t value)

/*
 * ARGUMENTS
 * * value : the thread's return value
 *
 * SOURCE
 */

{
  status_t status;
  uint32_t current_cpuid = cpu_mp_id();
  thread_t self = scheduler . cpu[current_cpuid] . current_thread;
  thread_t target = NULL, p = NULL;
  interrupt_status_t it_status = 0;

  /*
   * First, we lock ourselves
   */

  it_status = cpu_trap_mask_and_backup();
  lock_acquire (& self -> lock);

  /*
   * And we place the return value in our structure
   */

  self -> signature . return_value = value;

  /*
   * Mark self as zombie
   */

  self -> info . status = DNA_THREAD_ZOMBIE;

  lock_acquire (& self -> wait . lock);
  lock_release (& self -> lock);

  /*
   * Then we can wake up the waiting threads
   */

  while ((p = queue_rem (& self -> wait)) != NULL)
  {
    lock_acquire (& p -> lock);
    p -> info . status = DNA_THREAD_READY;

    lock_acquire (& scheduler . xt[p -> info . affinity] . lock);
    lock_release (& p -> lock);

    queue_add (& scheduler . xt[p -> info . affinity], p);
    lock_release (& scheduler . xt[p -> info . affinity] . lock);
  }

  lock_release (& self -> wait . lock);

  /*
   * Elect a the next thread and run it
   */

  status = scheduler_elect (& target, true);
  panic (status == DNA_OK);

  target -> info . status = DNA_THREAD_RUNNING;
  target -> info . cpu_id = current_cpuid;

  lock_acquire (& scheduler . cpu[current_cpuid] . lock);
  lock_release (& target -> lock);

  scheduler . cpu[current_cpuid] . current_thread = target;
  lock_release (& scheduler . cpu[current_cpuid] . lock);

  cpu_context_load (& target -> context);
}

/*
 ****/

