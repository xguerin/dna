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

#include <Private/VirtualFileSystem.h>
#include <DnaTools/DnaTools.h>

/****f* fdarray/fdarray_id_inspector
 * SUMMARY
 * Compare an id and a fdarray's id.
 *
 * SYNOPSIS
 */

bool fdarray_id_inspector (void * fdarray, void * p_id, void * p_dummy)

/*
 * ARGUMENTS
 * * fdarray : a fdarray element.
 * * p_id : a pointer to an id.
 *
 * FUNCTION
 * Compare *p_id and fdarray -> id.
 *
 * RESULT
 * Return TRUE if the strings match, FALSE otherwise.
 *
 * SOURCE
 */

{
  fdarray_t array = fdarray;
  int32_t * id = p_id;

  watch (bool)
  {
    ensure (array != NULL && id != NULL, false);
    return array -> associated_id == *id;
  }
}

/*
 ****/
