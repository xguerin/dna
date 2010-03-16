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
#include <MemoryManager/MemoryManager.h>

status_t devfs_inode_create (devfs_t fs, devfs_inode_t parent,
    devfs_inode_class_t class, char * name, int64_t vnid,
    device_cmd_t * commands, devfs_inode_t * p_inode)
{
  devfs_inode_t inode;

  watch (status_t)
  {
    ensure (name != NULL, DNA_BAD_ARGUMENT);

    inode = kernel_malloc (sizeof (struct _devfs_inode), true);
    ensure (inode != NULL, DNA_OUT_OF_MEM);

    inode -> id = vnid;
    inode -> class = class;
    inode -> parent = parent;
    inode -> dev_cmd = commands;
    dna_strcpy (inode -> name, name);

    if (fs != NULL)
    {
      queue_add (& fs -> inode_list, inode);
    }

    *p_inode = inode;
    return DNA_OK;
  }
}
