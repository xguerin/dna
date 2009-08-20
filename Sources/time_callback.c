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

status_t time_callback (void * data)

/*
 * ARGUMENTS
 * * data : 
 *
 * SOURCE
 */

{
  status_t status = DNA_OK;
  alarm_t next_alarm = NULL;
  alarm_t alarm = (alarm_t) data;
  bigtime_t current_time = 0, quantum = 0;
  bool reschedule = false, process_next_alarm = true;
  cpu_t * cpu = & scheduler . cpu[cpu_mp_id ()];

  /*
   * Now we execute the alarm and those
   * which deadline is included in the Jiffy
   */

  do
  {
    status = alarm -> callback (alarm -> data);
    if (status == DNA_INVOKE_SCHEDULER) reschedule = true;

    /*
     * We lock the related cpu structure
     */

    lock_acquire (& cpu -> alarm_queue . lock);

    /*
     * We check whether or not the alarm has to be restarted
     */

    if ((alarm -> mode & DNA_PERIODIC_ALARM) != 0)
    {
      time_manager . system_timer . get (cpu -> id, & current_time);
      alarm -> deadline = alarm -> quantum +  current_time;
      queue_insert (& cpu -> alarm_queue, alarm_comparator, alarm);
    }
    else
    {
      kernel_free (alarm);
    }

    /*
     * Look through the alarms, and restart or cancel them if necessary
     */

    if (cpu -> alarm_queue . status != 0)
    {
      next_alarm = queue_rem (& cpu -> alarm_queue);
      cpu -> current_alarm = next_alarm;

      time_manager . system_timer . get (cpu -> id, & current_time);
      quantum = next_alarm -> deadline - current_time;

      if (quantum <= DNA_TIMER_JIFFY)
      {
        alarm = next_alarm;
      }
      else
      {
        process_next_alarm = false;
        time_manager . system_timer . set (cpu -> id, quantum, next_alarm);
      }
    }
    else
    {
      cpu -> current_alarm = NULL;
      process_next_alarm = false;
    }

    lock_release (& cpu -> alarm_queue . lock);
  }
  while (process_next_alarm);

  /*
   * And reschedule if necessary
   */

  if (reschedule) thread_yield ();
  return DNA_OK;
}

/*
 ****/

