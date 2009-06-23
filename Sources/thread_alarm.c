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
	interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (thread != NULL, DNA_ERROR);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& scheduler . xt[thread -> cpu_affinity] . lock);

    queue_add (& scheduler . xt[thread -> cpu_affinity],
        & thread -> status_link);

    lock_release (& scheduler . xt[thread -> cpu_affinity] . lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }
}

/*
 ****/

