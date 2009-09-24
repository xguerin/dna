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
  status_t status = DNA_OK;
  cpu_status_t cpu_status;
  alarm_t next_alarm = NULL;
  int32_t current_cpuid = cpu_mp_id ();
  bigtime_t current_time = 0, quantum = 0;
  bool process_next_alarm = true, delete_alarm = false;
  cpu_t * cpu = & scheduler . cpu[current_cpuid];
  alarm_t alarm = cpu -> current_alarm;

  /*
   * Save the previous CPU status,
   * replace with SERVICING_INTERRUPT
   */

  lock_acquire (& scheduler . lock);
  cpu_status = scheduler . cpu[current_cpuid] . status;
  scheduler . cpu[current_cpuid] . status = DNA_CPU_SERVICING_INTERRUPT;
  lock_release (& scheduler . lock);

  /*
   * Proceed with the alarm
   */

  do
  {
    cpu_timer_get (cpu -> id, & current_time);

    /*
     * We check whether or not the alarm has to be restarted
     */

    if ((alarm -> mode & DNA_PERIODIC_ALARM) != 0)
    {
      alarm -> deadline = alarm -> quantum +  current_time;

      lock_acquire (& cpu -> alarm_queue . lock);
      queue_insert (& cpu -> alarm_queue, alarm_comparator, alarm);
      lock_release (& cpu -> alarm_queue . lock);
    }
    else delete_alarm = true;

    /*
     * Execute the alarm
     */

    status = alarm -> callback (alarm -> data);

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

    if (cpu -> alarm_queue . status == 0)
    {
      cpu -> current_alarm = NULL;
      process_next_alarm = false;
    }
    else
    {
      lock_acquire (& cpu -> alarm_queue . lock);
      next_alarm = queue_rem (& cpu -> alarm_queue);

      lock_acquire (& cpu -> lock);
      lock_release (& cpu -> alarm_queue . lock);

      cpu -> current_alarm = next_alarm;
      lock_release (& cpu -> lock);

      quantum = next_alarm -> deadline - current_time;

      if (quantum <= DNA_TIMER_JIFFY)
      {
        log (PANIC_LEVEL, "alarm %d, low quantum (%d)",
            next_alarm -> id, (int32_t) quantum);

        alarm = next_alarm;
      }
      else
      {
        process_next_alarm = false;
        cpu_timer_set (cpu -> id, quantum);
      }
    }
  }
  while (process_next_alarm);

  /*
   * Restore the previous CPU status
   */

  lock_acquire (& scheduler . lock);
  scheduler . cpu[current_cpuid] . status = cpu_status;
  lock_release (& scheduler . lock);
}

/*
 ****/

