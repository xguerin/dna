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

/****f* Core/alarm_id_inspector
 * SUMMARY
 * Find a alarm with its id.
 *
 * SYNOPSIS
 */

bool alarm_id_inspector (void * item, void * p_id, void * dummy)

/*
 * ARGUMENTS
 * * item : a pointer to an alarm_t
 * * p_id : a pointer to an ID
 * * dummy : not used 
 *
 * RESULT
 * True in case of success, false otherwise.
 *
 * SOURCE
 */

{
	alarm_t alarm = item;
	int32_t * id = p_id;
	
  watch (bool)
  {
    ensure (alarm != NULL && id != NULL, false);
    return alarm -> id == *id;
  }
}

/*
 ****/

