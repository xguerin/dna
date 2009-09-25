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

/****f* Core/ipi_callback
 * SUMMARY
 * Handler for inter-processor interrupts.
 *
 * SYNOPSIS
 */

void ipi_callback (int32_t command, void * cookie)

/*
 * ARGUMENTS
 * * command : the command of the ipi
 * * cookie : the data of the ipi
 *
 * RESULT
 * DNA_OK.
 *
 * SOURCE
 */

{
  status_t status;
  int32_t current_cpuid = cpu_mp_id ();

  /*
   * Release the IPI lock and proceed with the IPI
   */

  lock_release (& scheduler . cpu[current_cpuid] . ipi_lock);

  switch (command)
  {
    case DNA_IPI_YIELD :
      {
        log (VERBOSE_LEVEL, "%d YIELD", current_cpuid);

        scheduler . cpu[current_cpuid] . status = DNA_CPU_RUNNING;
        status = thread_yield ();

        if (status == DNA_NO_AVAILABLE_THREAD)
        {
          scheduler . cpu[current_cpuid] . status = DNA_CPU_READY;
          lock_acquire (& scheduler . cpu_pool . lock);
          queue_add (& scheduler . cpu_pool, & scheduler . cpu[current_cpuid]);
          lock_release (& scheduler . cpu_pool . lock);
        }

        break;
      }

    case DNA_IPI_SUSPEND :
      {
        int32_t thread_id = (int32_t) cookie;

        log (VERBOSE_LEVEL, "%d SUSPEND %d", cpu_mp_id (), thread_id);

        thread_suspend (thread_id);
        break;
      }

    case DNA_IPI_TRAP_ENABLE:
      {
        int32_t id = (int32_t) cookie;

        log (VERBOSE_LEVEL, "%d ENABLE %d", cpu_mp_id (), id);

        cpu_trap_enable (id);
        break;
      }

    default :
      log (PANIC_LEVEL, "Unknown command: %d", command);
      break;
  }
}

/*
 ****/

