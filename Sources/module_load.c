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

#include <DnaTools/DnaTools.h>

/****f* module/module_load
 * SUMMARY
 * Load a module.
 *
 * SYNOPSIS
 */

status_t module_load (char * name, module_t ** module)

/*
 * SOURCE
 */

{
  for (int32_t i = 0; i < OS_N_MODULES; i++)
  {
    if (dna_strcmp (OS_MODULES_LIST[i] -> name, name) == 0)
    {
      *fs = OS_MODULES_LIST[i];
      return DNA_OK;
    }
  }

  return DNA_ERROR;
}

/*
 ****/
