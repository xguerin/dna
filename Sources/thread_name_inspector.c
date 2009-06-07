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

/****f* Core/thread_name_inspector
 * SUMMARY
 * Finds a thread by name.
 *
 * SYNOPSIS
 */

bool thread_name_inspector (void * item, void * name, void * dummy)

/*
 * ARGUMENTS
 * * item : a thread_t element
 * * name : a character string
 * * dummy : not used
 *
 * RESULT
 * True if the two names match, false otherwise.
 *
 * SOURCE
 */

{
	thread_t thread = item;
	const char * thread_name = name;

  watch (bool)
  {
    ensure (thread != NULL && thread_name != NULL, false);
    return dna_strcmp (thread_name, thread -> name) == 0;
  }
}

/*
 ****/

