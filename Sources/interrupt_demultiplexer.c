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
#include <Processor/Processor.h>

/****f* Core/interrupt_demultiplexer
 * SUMMARY
 * Handler for mulitplexed interrupts.
 *
 * SYNOPSIS
 */

int32_t interrupt_demultiplexer (int32_t itn)

/*
 * ARGUMENTS
 * * data : the ID of the interrupt
 *
 * RESULT
 * DNA_OK.
 *
 * SOURCE
 */

{
  isr_t isr;
  status_t status = DNA_UNHANDLED_INTERRUPT;
  int32_t current_cpuid = cpu_mp_id ();
  cpu_t * cpu = & cpu_pool . cpu[current_cpuid];
  queue_t * queue = & cpu -> isr_list[itn];

  watch (int32_t)
  {
    ensure (itn < cpu_trap_count (), DNA_BAD_ARGUMENT);

    /*
     * Look for the corresponding handler
     */

    lock_acquire (& queue -> lock);

    for (isr = (isr_t) queue -> head; isr != NULL; isr = isr -> next)
    {
      status = isr -> handler (itn);
      if (status == DNA_INVOKE_SCHEDULER || status == DNA_HANDLED_INTERRUPT)
      {
        break;
      }
    }

    lock_release (& queue -> lock);

    /*
     * Depending on the status, invoke the scheduler
     */

    if (status == DNA_INVOKE_SCHEDULER)
    {
      status = thread_yield ();

      if (status == DNA_NO_AVAILABLE_THREAD && cpu -> status == DNA_CPU_READY)
      {
        log (INFO_LEVEL, "Nothing to do...");
        lock_acquire (& cpu_pool . cpu_queue . lock);
        queue_add (& cpu_pool . cpu_queue, cpu);
        lock_release (& cpu_pool . cpu_queue . lock);
      }
    }

    return status;
  }
}

/*
 ****/

