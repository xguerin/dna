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

/****f* Core/semaphore_alarm
 * SUMMARY
 * Semaqphore acquire alarm.
 *
 * SYNOPSIS
 */

status_t semaphore_alarm (void * data)

/*
 * ARGUMENTS
 * * data : a thread_t element.
 *
 * SOURCE
 */

{
  status_t status = DNA_OK;
  thread_t thread = data;
  semaphore_t semaphore;
  semaphore_id_t sem_id;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_ERROR);
    ensure (thread -> info . status == DNA_THREAD_WAITING, DNA_ERROR);
    ensure (thread -> info . resource == DNA_RESOURCE_SEMAPHORE, DNA_ERROR);

    /*
     * Get the corresponding semaphore
     */

    lock_acquire (& semaphore_pool . lock);

    sem_id . raw = thread -> info . resource_id;
    check (invalid_semaphore, sem_id . s . index < DNA_MAX_SEM, DNA_ERROR);

    log (VERBOSE_LEVEL, "ID(%d:%d)", sem_id . s . value, sem_id . s . index);

    semaphore = semaphore_pool . semaphore[sem_id . s . index];
    check (invalid_semaphore, semaphore != NULL, DNA_ERROR);
    check (invalid_semaphore, semaphore -> id . raw == sem_id . raw, DNA_ERROR);

    lock_acquire (& semaphore -> waiting_queue . lock);
    lock_release (& semaphore_pool . lock);

    /*
     * Extract the thread from the waiting list
     */

    status = queue_extract (& semaphore -> waiting_queue, thread);
    lock_release (& semaphore -> waiting_queue . lock);

    /*
     * If the thread was waiting, we can dispatch it
     */

    if (status == DNA_OK)
    {
      lock_acquire (& thread -> lock);

      thread -> info . status = DNA_THREAD_READY;
      thread -> info . previous_status = DNA_THREAD_WAITING;

      status = scheduler_dispatch (thread);
    }

    return status;
  }

  rescue (invalid_semaphore)
  {
    lock_release (& semaphore_pool . lock);
    leave;
  }
}

/*
 ****/

