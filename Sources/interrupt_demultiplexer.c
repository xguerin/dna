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

int32_t interrupt_demultiplexer (int32_t data)

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
  cpu_status_t cpu_status;
  int32_t current_cpuid = cpu_mp_id ();

  /*
   * Save the previous CPU status,
   * replace with SERVICING_INTERRUPT
   */

  lock_acquire (& scheduler . lock);
  cpu_status = scheduler . cpu[current_cpuid] . status;
  scheduler . cpu[current_cpuid] . status = DNA_CPU_SERVICING_INTERRUPT;
  lock_release (& scheduler . lock);

  /*
   * Look for the corresponding handler
   */

  lock_acquire (& scheduler . cpu[current_cpuid]. isr_list . lock);

  queue_lookup (& scheduler . cpu[current_cpuid]. isr_list,
      interrupt_handler_inspector, & data, NULL);

  lock_release (& scheduler . cpu[current_cpuid]. isr_list . lock);

  /*
   * Restore the previous CPU status
   */

  lock_acquire (& scheduler . lock);
  scheduler . cpu[current_cpuid] . status = cpu_status;
  lock_release (& scheduler . lock);

  return DNA_OK;
}

/*
 ****/

