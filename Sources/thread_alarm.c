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

/****f* Core/thread_alarm
 * SUMMARY
 * Thread snooze alarm.
 *
 * SYNOPSIS
 */

status_t thread_alarm (void * data)

/*
 * ARGUMENTS
 * * data : a thread_t element.
 *
 * SOURCE
 */

{
  status_t status = DNA_OK;
  thread_t thread = data;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_ERROR);

    lock_acquire (& thread -> lock);

    switch (thread -> info . status)
    {
      case DNA_THREAD_SUSPENDED :
        {
          log (VERBOSE_LEVEL, "thread %d supended, leave it this way",
              thread -> info . id);
          
          lock_release (& thread -> lock);
          break;
        }

      case DNA_THREAD_SLEEPING :
        {
          log (VERBOSE_LEVEL, "thread %d sleeping, resuming...",
              thread -> info . id);
 
          thread -> info . status = DNA_THREAD_READY;
          status = scheduler_dispatch (thread);

          break;
        }

      default :
        {
          log (PANIC_LEVEL, "status %x not handled.",
              thread -> info . status);

          lock_release (& thread -> lock);
          break;
        }
    }

    return status;
  }
}

/*
 ****/

