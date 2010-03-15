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

#include <Private/DeviceFileSystem.h>
#include <DnaTools/DnaTools.h>

bool devfs_remove_path_inspector (void * entry, va_list list)
{
  devfs_entry_t e = entry;
  devfs_t devfs = va_arg (list, devfs_t);
  devfs_inode_t next_inode = NULL;
  
  watch (bool)
  {
    ensure (e != NULL, false);
    ensure (devfs != NULL, false);

    next_inode = queue_lookup (& devfs -> inode_list,
        devfs_inode_inspector, e -> id);
    devfs_remove_path (devfs, next_inode);

    return false;
  }
}

