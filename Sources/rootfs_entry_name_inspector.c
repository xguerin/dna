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

#include <Private/RootFileSystem.h>
#include <DnaTools/DnaTools.h>

bool rootfs_entry_name_inspector (void * entry,
    void * entry_name, void * dummy)
{
	rootfs_entry_t e = entry;
	char * name = entry_name;

  watch (bool)
  {
    ensure (e != NULL, false);
    ensure (name != NULL, false);

    return dna_strcmp (e -> name, name) == 0;
  }
}

