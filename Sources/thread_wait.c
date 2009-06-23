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

/****f* Core/thread_wait
 * SUMMARY
 * Waits for a thread to complete.
 *
 * SYNOPSIS
 */

status_t thread_wait (int32_t id, int32_t * value)

/*
 * ARGUMENTS
 * * id : a thread identifier.
 * * value : a pointer to the return variable.
 *
 * RESULT
 * * DNA_OK in case of success.
 * * DNA_INVALID_THREAD_ID if the argument id is invalid.
 *
 * SOURCE
 */

{
	uint32_t current_cpuid = cpu_mp_id();
	thread_t self = scheduler . cpu[current_cpuid] . current_thread;
	thread_t thread = NULL, target = NULL;
	interrupt_status_t it_status = 0;

  watch (status_t)
  {
    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& team_manager . lock);

    /*
     * We look for the thread to wait for
     */

    thread = queue_lookup (& team_manager . thread_list,
        thread_id_inspector, (void *) & id, NULL);

    check (invalid_thread, thread != NULL, DNA_INVALID_THREAD_ID);

    lock_acquire (& thread -> lock);
    lock_release (& team_manager . lock);

    /*
     * If the thread is already dead, it is not necessary to
     * go through all the rescheduling pain
     */

    if (thread -> status != DNA_THREAD_ZOMBIE)
    {

      lock_acquire (& thread -> wait . lock);
      lock_release (& thread -> lock);

      /*
       * If not, put ourselve in wait mode and we switch
       */

      self -> status = DNA_THREAD_WAIT;

      target = scheduler_elect ();
      if (target == NULL)
      {
        target = scheduler . cpu[current_cpuid] . idle_thread;
      }

      scheduler_switch (target, & thread -> wait);
      cpu_trap_restore(it_status);
    }
    else
    {
      lock_release (& thread -> lock);
    }

    /*
     * We get the return value of the thread and return
     */

    if (value != NULL)
    {
      *value = thread -> signature . return_value;
    }

    return DNA_OK;
  }

  rescue (invalid_thread)
  {
    lock_release (& team_manager . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

