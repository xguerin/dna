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

/****f* Core/time_cancel_alarm
 * SUMMARY
 * Cancel an existing alarm.
 *
 * SYNOPSIS
 */

status_t time_cancel_alarm (int32_t aid)

/*
 * ARGUMENTS
 * * alarm : the alarm to cancel
 *
 * RESULT
 * * DNA_ERROR if the alarm doesn't exist
 * * DNA_OK if the operation succeeded
 *
 * SOURCE
 */

{
  alarm_t a = NULL;
  bigtime_t current_time = 0, quantum = 0;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (time_manager . has_timer, DNA_NO_TIMER);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& time_manager . lock);

    if (time_manager . current_alarm -> id == aid)
    {
      time_manager . system_timer . cancel ();

      a = time_manager . current_alarm; 
      kernel_free (a);

      if (time_manager . alarm_queue . status != 0)
      {
        a = queue_rem (& time_manager . alarm_queue);
        time_manager . system_timer . get (& current_time);
        quantum = a -> deadline - current_time;
        time_manager . current_alarm = a;
        time_manager . system_timer . set (quantum, time_callback, a);
      }
      else
      {
        time_manager . current_alarm = NULL;
      }
    }
    else
    {
      a = queue_lookup (& time_manager . alarm_queue,
          alarm_id_inspector, & aid, NULL); 

      check (alarm_error, a != NULL, DNA_ERROR);

      queue_extract (& time_manager . alarm_queue, & a -> link);
      kernel_free (a);
    }

    lock_release (& time_manager . lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }

  rescue (alarm_error)
  {
    lock_release (& time_manager . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

