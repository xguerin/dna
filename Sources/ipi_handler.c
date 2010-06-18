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

/****f* Interrupt/ipi_handler
 * SUMMARY
 * Handler for inter-processor interrupts.
 *
 * SYNOPSIS
 */

status_t ipi_handler (int32_t command, void * cookie)

/*
 * ARGUMENTS
 * * command : the command of the ipi
 * * cookie : the data of the ipi
 *
 * RESULT
 * * DNA_INVOKE_SCHEDULER: success, invoke the scheduler on return
 * * DNA_ERROR: the command is not supported
 * * DNA_OK: the operation succeeded
 *
 * SOURCE
 */

{
  status_t status = DNA_OK;

  switch (command)
  {
    case DNA_IPI_DISPATCH :
      {
        thread_t target = (thread_t)cookie;
        thread_t self = cpu_pool . cpu[cpu_mp_id ()] . current_thread;

        log (VERBOSE_LEVEL, "%d YIELD", cpu_mp_id ());

        if (self == cpu_pool . cpu[cpu_mp_id ()] . idle_thread)
        {
          lock_acquire (& self -> lock);
          self -> info . status = DNA_THREAD_READY;
          status = scheduler_switch (target, NULL);
        }
        else
        {
          lock_acquire (& scheduler . queue[target -> info . affinity] . lock);
          lock_release (& target -> lock);

          queue_add (& scheduler . queue[target -> info . affinity], target);
          lock_release (& scheduler . queue[target -> info . affinity] . lock);
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

    case DNA_IPI_TRAP_ENABLE :
      {
        int32_t id = (int32_t) cookie;

        log (VERBOSE_LEVEL, "%d ENABLE %d", cpu_mp_id (), id);

        cpu_trap_enable (id);
        break;
      }

    case DNA_IPI_TRAP_DISABLE :
      {
        int32_t id = (int32_t) cookie;

        log (VERBOSE_LEVEL, "%d DISABLE %d", cpu_mp_id (), id);

        cpu_trap_disable (id);
        break;
      }

    default :
      log (PANIC_LEVEL, "Unknown command: %d", command);
      status = DNA_ERROR;
      break;
  }

  return status;
}

/*
 ****/

