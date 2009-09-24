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

status_t ipi_callback (int32_t command, void * cookie)

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

  watch (status_t)
  {
    /*
     * Release the IPI lock and proceed with the IPI
     */

    lock_release (& scheduler . cpu[current_cpuid] . ipi_lock);

    switch (command)
    {
      case DNA_IPI_EXECUTE :
        {
          thread_t thread = cookie;
          thread_t self = scheduler . cpu[cpu_mp_id ()] . current_thread;

          ensure (thread != NULL, DNA_ERROR);

          log (VERBOSE_LEVEL, "%d EXECUTE %d (was %d)",
              cpu_mp_id (), thread -> info . id, self -> info . id);

          lock_acquire (& self -> lock);

          self -> info . status = DNA_THREAD_READY;
          self -> info . previous_status = DNA_THREAD_RUNNING;

          status = scheduler_switch (thread, NULL);
          ensure (status == DNA_OK, status);

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

    return DNA_OK;
  }
}

/*
 ****/

