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
 * You should have received new_alarm copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Private/Core.h>
#include <MemoryManager/MemoryManager.h>
#include <DnaTools/DnaTools.h>
#include <Processor/Processor.h>

/****f* alarm/alarm_create
 * SUMMARY
 * Create an alarm.
 *
 * SYNOPSIS
 */

status_t alarm_create (bigtime_t quantum, int32_t mode,
    alarm_callback_t callback, void * data, int32_t * aid)

/*
 * ARGUMENTS
 * * deadline : the alarm's deadline
 * * mode : the alarm's mode
 * * callback : the function to call when the alarm goes off
 * * data : the data associated to the function
 * * aid : the recipient of the new alarm's id
 *
 * RESULT
 *
 * SOURCE
 */

{
  bigtime_t current_time = 0;
  int32_t current_cpuid = cpu_mp_id ();
  cpu_t * cpu = & scheduler . cpu[current_cpuid];
  interrupt_status_t it_status;
  alarm_t new_alarm = NULL, old_alarm = NULL;
  
  watch (status_t)
  {
    ensure (! (mode & DNA_ABSOLUTE_ALARM), DNA_NOT_IMPLEMENTED);

    /*
     * Allocate the new alarm
     */

    new_alarm = kernel_malloc (sizeof (struct _alarm), true);
    ensure (new_alarm != NULL, DNA_OUT_OF_MEM);

    /*
     * Set its parameters
     */

    new_alarm -> id = -1;
    new_alarm -> mode = mode;
    new_alarm -> cpu_id = current_cpuid;

    cpu_timer_get (current_cpuid, & current_time);
    new_alarm -> quantum = quantum;
    new_alarm -> deadline = quantum + current_time;
    new_alarm -> callback = callback;
    new_alarm -> data = data;

    /*
     * Find an empty slot to store the alarm
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& alarm_manager . lock);

    for (int32_t i = 0; i < DNA_MAX_THREAD; i += 1)
    {
      if (alarm_manager . alarm[i] == NULL)
      {
        alarm_manager . alarm[i] = new_alarm;
        new_alarm -> id = i;
        break;
      }
    }

    lock_release (& alarm_manager . lock);
    cpu_trap_restore(it_status);

    check (error, new_alarm -> id != -1, DNA_ERROR);

    /*
     * Deal with the new alarm
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& cpu -> lock);

    if (cpu -> current_alarm == NULL)
    {
       cpu -> current_alarm = new_alarm;
       cpu_timer_set (current_cpuid, quantum);
    }
    else
    {
      old_alarm = cpu -> current_alarm;

      if (old_alarm -> deadline > new_alarm -> deadline)
      {
        cpu_timer_cancel (current_cpuid);
        cpu -> current_alarm = new_alarm;

        queue_insert (& cpu -> alarm_queue, alarm_comparator, old_alarm);
        cpu_timer_set (current_cpuid, quantum);
      }
      else
      {
        queue_insert (& cpu -> alarm_queue, alarm_comparator, new_alarm);
      }
    }

    lock_release (& cpu -> lock);
    cpu_trap_restore(it_status);

    *aid = new_alarm -> id;
    return DNA_OK;
  }

  rescue (error)
  {
    kernel_free (new_alarm);
    leave;
  }
}

/*
 ****/

