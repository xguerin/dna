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
#include <DnaLibrary/DnaLibrary.h>
#include <Processor/Processor.h>

/****f* time/time_set_alarm
 * SUMMARY
 * Set an alarm.
 *
 * SYNOPSIS
 */

status_t time_set_alarm (bigtime_t quanta, int32_t mode,
    timer_callback_t callback, void * data, int32_t * alarm)

/*
 * ARGUMENTS
 * * deadline : the alarm's deadline
 * * mode : the alarm's mode
 * * callback : the function to call when the alarm goes off
 * * data : the data associated to the function
 *
 * RESULT
 *
 * SOURCE
 */

{
  alarm_t new_alarm = NULL, old_alarm = NULL;
  bigtime_t current_time = 0;
	interrupt_status_t status;
  
  watch (status_t)
  {
    ensure (! (mode & DNA_ABSOLUTE_ALARM), DNA_NOT_IMPLEMENTED);
    ensure (time_manager . has_timer, DNA_NO_TIMER);

    /*
     * Allocate the new alarm
     */

    new_alarm = kernel_malloc (sizeof (struct _alarm), true);
    ensure (new_alarm != NULL, DNA_OUT_OF_MEM);

    /*
     * Set its parameters
     */

    new_alarm -> id = atomic_add (& time_manager . alarm_counter, 1);
    new_alarm -> mode = mode;

    time_manager . system_timer . get (& current_time);
    new_alarm -> quanta = quanta;
    new_alarm -> deadline = quanta + current_time;
    new_alarm -> callback = callback;
    new_alarm -> data = data;
    queue_item_init (& new_alarm -> link, new_alarm);

    /*
     * Deal with the new alarm
     */

    status = cpu_trap_mask_and_backup();
    lock_acquire (& time_manager . lock);

    if (time_manager . current_alarm == NULL)
    {
       time_manager . current_alarm = new_alarm;
       time_manager . system_timer . set (quanta, time_callback, new_alarm);
    }
    else
    {
      if (time_manager . current_alarm -> deadline > new_alarm -> deadline)
      {
        /*
         * Cancel the current timer
         */

        time_manager . system_timer . cancel ();

        /*
         * Replace the current alarm
         */

        old_alarm = time_manager . current_alarm;
        time_manager . current_alarm = new_alarm;

        /*
         * Insert the old alarm in the alarm list
         */

        queue_insert (& time_manager . alarm_queue,
            alarm_comparator, & old_alarm -> link);

        /*
         * Set the new timer
         */
        
        time_manager . system_timer . set (quanta, time_callback, new_alarm);
      }
      else
      {
        queue_insert (& time_manager . alarm_queue,
            alarm_comparator, & new_alarm -> link);
      }
    }

    lock_release (& time_manager . lock);
    cpu_trap_restore(status);

    *alarm = new_alarm -> id;
    return DNA_OK;
  }
}

/*
 ****/

