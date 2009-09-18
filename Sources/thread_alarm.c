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
  thread_t thread = data;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_ERROR);

    lock_acquire (& thread -> lock);

    if (thread -> info . status == DNA_THREAD_WAIT)
    {
      thread -> info . previous_status = thread -> info . status;
      thread -> info . status = DNA_THREAD_READY;
      scheduler_dispatch (thread);
    }
    else 
    {
      if (thread -> info . previous_status == DNA_THREAD_WAIT)
      {
        thread -> info . previous_status = DNA_THREAD_READY;
      }

      lock_release (& thread -> lock);
    }

    return DNA_OK;
  }
}

/*
 ****/

