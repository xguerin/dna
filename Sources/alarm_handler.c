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
#include <MemoryManager/MemoryManager.h>
#include <DnaTools/DnaTools.h>

/****f* Alarm/alarm_handler
 * SUMMARY
 * Time manager used for alarms.
 *
 * SYNOPSIS
 */

status_t alarm_handler (void)

/*
 * RESULT
 * * DNA_ERROR: their is no alarm to execute, or it is a false alarm
 * * DNA_INVOKE_SCHEDULER: success, the scheduler has to be invoked
 * * DNA_OK: the operation succeeded
 *
 * SOURCE
 */

{
  alarm_t current_alarm = NULL, next_alarm = NULL;
  status_t status = DNA_OK;
  int32_t current_cpuid = cpu_mp_id ();
  bigtime_t start_time = 0, quantum = 0, updated_time;
  bool reschedule = false;
  bool process_next_alarm = true, delete_alarm = false;
  cpu_t * cpu = & cpu_pool . cpu[current_cpuid];

  watch (status_t)
  {
    while (process_next_alarm)
    {
      lock_acquire (& cpu -> lock);
      current_alarm = cpu -> current_alarm;
      check (no_alarm, current_alarm != NULL, DNA_ERROR);

      /*
       * Get the present time and check if this is not
       * a false alarm.
       */

      cpu_timer_get (cpu -> id, & start_time);

      check (false_alarm,
          current_alarm -> deadline <= start_time + DNA_TIMER_DELAY, DNA_ERROR);

      log (VERBOSE_LEVEL, "Processing alarm (%d:%d) of thread 0x%x",
          current_alarm -> id . s . value, current_alarm -> id . s . index,
          current_alarm -> thread_id);

      /*
       * We check whether or not the alarm has to be restarted
       */

      if ((current_alarm -> mode & DNA_PERIODIC_ALARM) != 0)
      {
        current_alarm -> deadline += current_alarm -> quantum;
        queue_insert (& cpu -> alarm_queue, alarm_comparator, current_alarm);
      }
      else
      {
        delete_alarm = true;
      }

      /*
       * Look through the next alarm
       */

      next_alarm = queue_rem (& cpu -> alarm_queue);
      cpu -> current_alarm = next_alarm;

      if (next_alarm != NULL)
      {
        /*
         * We get the new current time in order to adjust the alarm.
         * We should also substract the time spent in the processor
         * handler. We might be off by approx. 500 cycles anyway,
         * due to the interrupt demultiplexer and the platform driver's
         * timer ISR.
         */

        cpu_timer_get (cpu -> id, & updated_time);
        quantum = next_alarm -> deadline - updated_time;

        if (quantum <= DNA_TIMER_DELAY)
        {
          log (VERBOSE_LEVEL, "low (%d), alarm (%d:%d) from thread 0x%x",
              (int32_t) quantum, next_alarm -> id . s . value,
              next_alarm -> id . s . index, next_alarm -> thread_id);
        }
        else
        {
          process_next_alarm = false;
          cpu_timer_set (cpu -> id, quantum);
        }
      }
      else
      {
        process_next_alarm = false;
      }

      lock_release (& cpu -> lock);

      /*
       * Execute the alarm
       */

      status = current_alarm -> callback (current_alarm -> data);
      if (status == DNA_INVOKE_SCHEDULER)
      {
        reschedule = true;
      }

      /*
       * Delete the alarm if necessary
       */

      if (delete_alarm)
      {
        lock_acquire (& alarm_manager . lock);
        alarm_manager . alarm[current_alarm -> id . s . index] = NULL;
        lock_release (& alarm_manager . lock);

        delete_alarm = false;
        kernel_free (current_alarm);
      }
    }

    return reschedule ? DNA_INVOKE_SCHEDULER : DNA_OK;
  }

  rescue (false_alarm)
  {
    quantum = current_alarm -> deadline - start_time;
    cpu_timer_set (cpu -> id, quantum);
  }

  rescue (no_alarm)
  {
    lock_release (& cpu -> lock);
    leave;
  }
}

/*
 ****/

