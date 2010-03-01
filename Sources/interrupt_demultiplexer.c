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

#include <stdbool.h>
#include <Private/Core.h>
#include <DnaTools/DnaTools.h>
#include <Processor/Processor.h>

/****f* core_private/interrupt_demultiplexer
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
  bool cpu_was_ready = false;
  queue_link_t * isr;
  status_t status;
  int32_t current_cpuid = cpu_mp_id ();
  cpu_t * cpu = & cpu_pool . cpu[current_cpuid];
  queue_t * queue = & cpu -> isr[itn];
  bigtime_t time;

  watch (int32_t)
  {
    ensure (itn < cpu_trap_count (), DNA_BAD_ARGUMENT);
    cpu_timer_get (current_cpuid, & time);

    /*
     * Extract the processor from the cpu list.
     */
    
    lock_acquire (& cpu_pool . queue . lock);
    status = queue_extract (& cpu_pool . queue, cpu);

    lock_acquire (& cpu -> lock);
    lock_release (& cpu_pool . queue . lock);

    if (cpu -> status == DNA_CPU_READY)
    {
      log (VERBOSE_LEVEL, ">>>> CPU %d ready", cpu -> id);
      cpu_was_ready = true;
    }
    else
    {
      log (VERBOSE_LEVEL, ">>>> CPU %d running", cpu -> id);
    }

    cpu -> status = DNA_CPU_RUNNING;
    lock_release (& cpu -> lock);

    /*
     * Look for the corresponding handler
     */

    lock_acquire (& queue -> lock);
    status = DNA_UNHANDLED_INTERRUPT;

    for (isr = queue -> head; isr != NULL; isr = isr -> next)
    {
      status = ((isr_t)isr) -> handler (itn);

      if (status == DNA_INVOKE_SCHEDULER || status == DNA_HANDLED_INTERRUPT)
      {
        break;
      }
    }

    lock_release (& queue -> lock);

    /*
     * If necessary, invoke the scheduler.
     */

    if (status == DNA_INVOKE_SCHEDULER)
    {
      thread_yield ();
    }

    /*
     * Finally, check the status of the CPU.
     * If it entered ready and comes back not ready,
     * push it back into a ready state.
     */

    lock_acquire (& cpu -> lock);

    if (cpu -> status == DNA_CPU_READY)
    {
      log (VERBOSE_LEVEL, "<<<< CPU %d ready");
    }
    else
    {
      log (VERBOSE_LEVEL, "<<<< CPU %d running");
    }

    if (cpu_was_ready && cpu -> status != DNA_CPU_READY)
    {
      cpu -> status = DNA_CPU_READY;

      lock_acquire (& cpu_pool . queue . lock);
      lock_release (& cpu -> lock);

      queue_add (& cpu_pool . queue, cpu);
      lock_release (& cpu_pool . queue . lock);
    }
    else
    {
      lock_release (& cpu -> lock);
    }

    return DNA_OK;
  }
}

/*
 ****/

