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

/****f* Core/interrupt_handler_inspector
 * SUMMARY
 * Look for the handler that can deal with the IT.
 *
 * SYNOPSIS
 */

bool interrupt_handler_inspector (void * item, void * p_id, void * dummy)

/*
 * ARGUMENTS
 * * item : an isr_t
 * * p_id : a pointer to the interrupt's ID
 *
 * RESULT
 * True if the handler corresponds, false otherwise.
 *
 * SOURCE
 */

{
	isr_t isr = item;
	int32_t * id = p_id;
	status_t status = DNA_UNHANDLED_INTERRUPT;

  watch (bool)
  {
    ensure (isr != NULL, false);
    ensure (id != NULL, false);

    status = isr -> handler (*id);

    switch (status)
    {
      case DNA_UNHANDLED_INTERRUPT : return false;
      case DNA_INVOKE_SCHEDULER :
      case DNA_HANDLED_INTERRUPT : return true;
      default : return false;
    }
  }
}

/*
 ****/

