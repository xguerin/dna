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

#include <Processor/Processor.h>
#include <Platform/Platform.h>

void cpu_timer_set (int32_t id, bigtime_t deadline)
{
  bigtime_t local_deadline = 0;
  soclib_timer_port_t timer = & PLATFORM_TIMER_BASE[id];

  local_deadline = deadline / PLATFORM_TIMER_RES;

  cpu_write(UINT32, & (timer -> period), (uint32_t)local_deadline);
  cpu_write(UINT32, & (timer -> mode), 3);
}

