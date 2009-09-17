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

  watch (status_t)
  {
    switch (command)
    {
      case DNA_IPI_EXECUTE :
        {
          thread_t thread = cookie;
          ensure (thread != NULL, DNA_ERROR);

          log (VERBOSE_LEVEL, "%d EXECUTE %d",
              cpu_mp_id (), thread -> info . id);

          status = scheduler_switch (thread, NULL);
          ensure (status == DNA_OK, status);

          break;
        }

      case DNA_IPI_SUSPEND :
        {
          thread_t thread = cookie;
          ensure (thread != NULL, DNA_ERROR);

          log (VERBOSE_LEVEL, "%d SUSPEND %d",
              cpu_mp_id (), thread -> info . id);
          
          lock_release (& thread -> lock);
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

