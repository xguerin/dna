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

/****f* alarm/alarm_destroy
 * SUMMARY
 * Destroy an existing alarm.
 *
 * SYNOPSIS
 */

status_t alarm_destroy (int32_t aid)

/*
 * ARGUMENTS
 * * alarm : the alarm to cancel
 *
 * RESULT
 * * DNA_UNKNOWN_ALARM if the alarm doesn't exist
 * * DNA_OK if the operation succeeded
 *
 * SOURCE
 */

{
  cpu_t * cpu = NULL;
  alarm_t alarm = NULL;
  status_t status = DNA_OK;
  bigtime_t current_time = 0, quantum = 0;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (aid >= 0 && aid < DNA_MAX_ALARM, DNA_BAD_ARGUMENT);

    /*
     * First, we lock the time manager and get the alarm
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& alarm_manager . lock);

    alarm = alarm_manager . alarm[aid];
    check (alarm_error, alarm != NULL, DNA_UNKNOWN_ALARM);

    /*
     * Next, we lock the related CPU
     */

    cpu = & scheduler . cpu[alarm -> cpu_id];
    lock_acquire (& cpu -> lock);

    /*
     * Then, we check if alarm is the current alarm
     */

    if (cpu -> current_alarm != NULL && cpu -> current_alarm -> id == aid)
    {
      cpu_timer_cancel (cpu -> id);

      alarm_manager . alarm[aid] = NULL;
      kernel_free (alarm);

      if (cpu -> alarm_queue . status != 0)
      {
        alarm = queue_rem (& cpu -> alarm_queue);
        cpu_timer_get (cpu -> id, & current_time);

        quantum = alarm -> deadline - current_time;
        cpu -> current_alarm = alarm;
        cpu_timer_set (cpu -> id, quantum);
      }
      else
      {
        cpu -> current_alarm = NULL;
      }
    }
    else
    {
      alarm = queue_lookup (& cpu -> alarm_queue,
          alarm_id_inspector, & aid, NULL); 

      if (alarm == NULL) status = DNA_UNKNOWN_ALARM;
      else
      {
        queue_extract (& cpu -> alarm_queue, alarm);
        alarm_manager . alarm[aid] = NULL;
        kernel_free (alarm);
      }
    }

    lock_release (& cpu -> lock);
    lock_release (& alarm_manager . lock);

    cpu_trap_restore(it_status);
    return status;
  }

  rescue (alarm_error)
  {
    lock_release (& alarm_manager . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

