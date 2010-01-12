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

#include <Private/Ipi.h>
#include <Processor/Processor.h>

int32_t ipi_handler (int32_t itn)
{
  int32_t data, command;
  int32_t current_cpuid = cpu_mp_id ();
  soclib_ipi_port_t ipi = SOCLIB_IPI_DEVICES[0] . port;

  cpu_read (UINT32, & (ipi[current_cpuid] . data), data);
  cpu_read (UINT32, & (ipi[current_cpuid] . command), command);
  cpu_write (UINT32, & (ipi[current_cpuid] . reset), 0);

  ipi_callback (command, (void *)data);
  return 0;
}

