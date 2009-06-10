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
#include <DnaLibrary/DnaLibrary.h>
#include <Processor/Processor.h>

/****f* Core/ipi_callback
 * SUMMARY
 * Handler for inter-processor interrupts.
 *
 * SYNOPSIS
 */

status_t ipi_callback (int32_t command, int32_t data)

/*
 * ARGUMENTS
 * * command : the command of the ipi
 * * data : the data of the ipi
 *
 * RESULT
 * DNA_OK.
 *
 * SOURCE
 */

{
  thread_t thread;
	thread_t self = scheduler . cpu[cpu_mp_id()] . current_thread;

  switch (command)
  {
    case DNA_IPI_YIELD :
      log (1, "YIELD(%d) on processor %d", data, cpu_mp_id ());

      lock_acquire (& team_manager . lock);

      thread = queue_lookup (& team_manager . thread_list,
          thread_id_inspector, (void *) & data, NULL);

      lock_release (& team_manager . lock);

      if (thread != NULL)
      {
        self -> status = DNA_THREAD_READY;
        scheduler_switch (thread, NULL);
      }

      break;

    case DNA_IPI_PING :
      log (1, "PING received on processor %d", cpu_mp_id ());
      break;

    default :
      log (1, "Unknown command received on processor %d", cpu_mp_id ());
      break;
  }

	return DNA_OK;
}

/*
 ****/

