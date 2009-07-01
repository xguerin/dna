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

/****f* Core/thread_suspend
 * SUMMARY
 * Suspend a thread.
 *
 * SYNOPSIS
 */

status_t thread_suspend (int32_t id)

/*
 * ARGUMENTS
 * * id : a thread id.
 *
 * RESULT
 * * DNA_ERROR if the argument lock is invalid.
 * * DNA_OK if the operation succeeded.
 *
 * SOURCE
 */

{
	uint32_t current_cpuid = cpu_mp_id();
	thread_t self = scheduler . cpu[current_cpuid] . current_thread;
  thread_t target = NULL;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (id == self -> id, DNA_NOT_IMPLEMENTED);

    if (id == self -> id)
    {
      it_status = cpu_trap_mask_and_backup();
      self -> status = DNA_THREAD_SLEEP;

      /*
       * Elect a the next thread and run it
       * If target is IDLE, we can safely push the CPU
       * since we disabled the interrupts.
       */

      if ((target = scheduler_elect ()) == NULL)
      {
        scheduler_push_cpu (current_cpuid);
        target = scheduler . cpu[current_cpuid] . idle_thread;
      }

      scheduler_switch (target, NULL);
      cpu_trap_restore(it_status);
    }

    return DNA_OK;
  }
}

/*
 ****/

