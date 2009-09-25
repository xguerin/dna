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

/****f* Core/time_callback
 * SUMMARY
 * Time callback used for alarms.
 *
 * SYNOPSIS
 */

void timer_callback (void)

/*
 * SOURCE
 */

{
  alarm_t alarm = NULL;
  status_t status = DNA_OK;
  int32_t current_cpuid = cpu_mp_id ();
  bigtime_t current_time = 0, quantum = 0;
  bool reschedule = false;
  bool process_next_alarm = true, delete_alarm = false;
  cpu_t * cpu = & scheduler . cpu[current_cpuid];

  /*
   * Proceed with the alarm
   */

  lock_acquire (& cpu -> lock);
  alarm = cpu -> current_alarm;

  while (process_next_alarm)
  {
    cpu_timer_get (cpu -> id, & current_time);

    /*
     * Check if this is not a false alarm
     */

    if (alarm -> deadline > current_time + DNA_TIMER_JIFFY)
    {
      quantum = alarm -> deadline - current_time;
      cpu_timer_set (cpu -> id, quantum);
      break;
    }

    log (VERBOSE_LEVEL, "Processing alarm %d", alarm -> id);

    /*
     * We check whether or not the alarm has to be restarted
     */

    if ((alarm -> mode & DNA_PERIODIC_ALARM) != 0)
    {
      alarm -> deadline = alarm -> quantum +  current_time;
      queue_insert (& cpu -> alarm_queue, alarm_comparator, alarm);
    }
    else delete_alarm = true;

    lock_release (& cpu -> lock);

    /*
     * Execute the alarm
     */

    status = alarm -> callback (alarm -> data);
    if (status == DNA_INVOKE_SCHEDULER) reschedule = true;

    if (delete_alarm)
    {
      lock_acquire (& alarm_manager . lock);
      alarm_manager . alarm[alarm -> id] = NULL;
      lock_release (& alarm_manager . lock);

      delete_alarm = false;
      kernel_free (alarm);
    }

    /*
     * Look through the next alarm
     */

    lock_acquire (& cpu -> lock);

    alarm = queue_rem (& cpu -> alarm_queue);
    cpu -> current_alarm = alarm;

    if (alarm != NULL)
    {
      quantum = alarm -> deadline - current_time;

      if (quantum <= DNA_TIMER_JIFFY)
      {
        log (PANIC_LEVEL, "alarm %d from %d, low quantum (%d)",
            alarm -> id, alarm -> thread_id, (int32_t) quantum);
      }
      else
      {
        process_next_alarm = false;
        cpu_timer_set (cpu -> id, quantum);
      }
    }
    else process_next_alarm = false;
  }

  lock_release (& cpu -> lock);

  /*
   * Reschedule of necessary
   */

  if (reschedule) thread_yield ();
}

/*
 ****/

