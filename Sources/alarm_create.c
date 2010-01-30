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

#include <stdint.h>
#include <stdbool.h>
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
  cpu_t * cpu = NULL;
  bigtime_t current_time = 0;
  int32_t current_cpuid = 0, index = 0;
  interrupt_status_t it_status;
  bool is_mode_valid = false;
  alarm_t new_alarm = NULL, old_alarm = NULL;
  
  watch (status_t)
  {
    ensure (quantum != 0, DNA_BAD_ARGUMENT);

    is_mode_valid = (mode & DNA_ABSOLUTE_ALARM) ^ (mode & DNA_RELATIVE_ALARM);
    ensure ((mode & DNA_ABSOLUTE_ALARM) ^ (mode & DNA_ABSOLUTE_ALARM), DNA_NOT_IMPLEMENTED);

    /*
     * Allocate the new alarm.
     */

    new_alarm = kernel_malloc (sizeof (struct _alarm), true);
    ensure (new_alarm != NULL, DNA_OUT_OF_MEM);

    /*
     * Deactivate interrupts and get current information.
     */

    it_status = cpu_trap_mask_and_backup();
    current_cpuid = cpu_mp_id ();
    cpu = & cpu_pool . cpu[current_cpuid];

    /*
     * Set various information.
     */

    new_alarm -> mode = mode;
    new_alarm -> thread_id = cpu -> current_thread -> id . raw;
    new_alarm -> cpu_id = current_cpuid;
    new_alarm -> callback = callback;
    new_alarm -> data = data;

    /*
     * Check and compute the deadline according to the
     * alarm mode passed as parameter.
     */

    cpu_timer_get (current_cpuid, & current_time);


    new_alarm -> quantum = quantum;
    new_alarm -> deadline = quantum + current_time;

    /*
     * Find an empty slot to store the alarm
     */

    lock_acquire (& alarm_manager . lock);

    for (index = 0; index < DNA_MAX_ALARM; index += 1)
    {
      if (alarm_manager . alarm[index] == NULL)
      {
        alarm_manager . alarm[index] = new_alarm;

        new_alarm -> id . s . index = index;
        new_alarm -> id . s . value = alarm_manager . counter;

        alarm_manager . counter += 1;
        break;
      }
    }

    lock_release (& alarm_manager . lock);
    check (error, index != DNA_MAX_ALARM, DNA_ERROR);

    /*
     * Deal with the new alarm
     */

    lock_acquire (& cpu -> lock);

    if (cpu -> current_alarm == NULL)
    {
      log (VERBOSE_LEVEL, "Set alarm (%d:%d)", new_alarm -> id . s . value,
          new_alarm -> id . s . index);

      cpu -> current_alarm = new_alarm;
      cpu_timer_set (current_cpuid, quantum);
    }
    else
    {
      old_alarm = cpu -> current_alarm;

      if (old_alarm -> deadline > new_alarm -> deadline)
      {
        log (VERBOSE_LEVEL, "Reset alarm (%d:%d) and set (%d:%d)",
            cpu -> current_alarm -> id . s . value,
            cpu -> current_alarm -> id . s . index,
            new_alarm -> id . s . value, new_alarm -> id . s . index);

        cpu_timer_cancel (current_cpuid);
        cpu -> current_alarm = new_alarm;
        queue_insert (& cpu -> alarm_queue, alarm_comparator, old_alarm);
        cpu_timer_set (current_cpuid, quantum);
      }
      else
      {
        log (VERBOSE_LEVEL, "Enqueue alarm (%d:%d)",
            new_alarm -> id . s . value, new_alarm -> id . s . index);

        queue_insert (& cpu -> alarm_queue, alarm_comparator, new_alarm);
      }
    }

    lock_release (& cpu -> lock);
    cpu_trap_restore(it_status);

    *aid = new_alarm -> id . raw;
    return DNA_OK;
  }

  rescue (error)
  {
    cpu_trap_restore(it_status);
    kernel_free (new_alarm);
    leave;
  }
}

/*
 ****/

