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

/****f* Core/team_name_inspector
 * SUMMARY
 * Finds a team by name.
 *
 * SYNOPSIS
 */

bool team_name_inspector (void * item, void * name, void * dummy)

/*
 * ARGUMENTS
 * * item : a team_t element
 * * name : a character string
 * * dummy : not used
 *
 * RESULT
 * True if the two names match, false otherwise.
 *
 * SOURCE
 */

{
  team_t team = item;
  const char * team_name = name;

  watch (bool)
  {
    ensure (item != NULL && name != NULL, false);
    return dna_strcmp (team_name, team -> name) == 0;
  }
}

/*
 ****/

