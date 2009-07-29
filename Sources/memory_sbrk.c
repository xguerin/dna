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

#include <Private/MemoryManager.h>
#include <DnaTools/DnaTools.h>

/****f* user/memory_sbrk
 * SUMMARY
 * Increments the team's data section.
 *
 * SYNOPSIS
 */

void * memory_sbrk (uint32_t increment)

/*
 * FUNCTION
 * This implementation merely adds increment to the current
 * user's _end pointer. It also check the alignment.
 *
 * SOURCE
 */

{
  static uint8_t * current_end = & _end;
  uint8_t * ptr_start = NULL;
  
  ptr_start = (uint8_t *)(((uint32_t)(current_end + 8)) & 0xFFFFFFF8);
  current_end = current_end + increment; 

  return ptr_start;
}

/*
 ******/
