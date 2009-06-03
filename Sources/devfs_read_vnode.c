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
#include <DnaLibrary/DnaLibrary.h>

status_t devfs_read_vnode (void * ns, int64_t vnid, void ** data) {
	devfs_t devfs = ns;
	devfs_inode_t inode = NULL;

  watch (status_t)
  {
    inode = queue_lookup (& devfs -> inode_list,
        devfs_inode_inspector, (void *) & vnid, NULL);
    ensure (inode != NULL, DNA_NO_VNODE);

    *data = inode;
    return DNA_OK;
  }
}

