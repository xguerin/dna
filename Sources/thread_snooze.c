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
#include <Processor/Processor.h>

/****f* Core/thread_snooze
 * SUMMARY
 * Snooze the current thread for a specified amount of time.
 *
 * SYNOPSIS
 */

status_t thread_snooze (bigtime_t value)

/*
 * ARGUMENTS
 * * value : the number of microseconds to wait for
 *
 * RESULT
 * * DNA_OK in case of success.
 *
 * SOURCE
 */

{
	int32_t alarm_id = -1;
	uint32_t current_cpuid = cpu_mp_id();
	thread_t self = scheduler . cpu[current_cpuid] . current_thread;
	thread_t target = NULL;
	interrupt_status_t it_status = 0;
  status_t status = DNA_OK;

  watch (status_t)
  {
    it_status = cpu_trap_mask_and_backup();

    status = time_set_alarm (value, DNA_RELATIVE_ALARM | DNA_ONE_SHOT_ALARM,
        thread_alarm, self, & alarm_id);

    check (alarm_error, status == DNA_OK, status);

    self -> status = DNA_THREAD_SLEEP;

    target = scheduler_elect ();
    if (target == NULL) target = scheduler . cpu[current_cpuid] . idle_thread;

    scheduler_switch (target, NULL);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }

  rescue (alarm_error)
  {
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

