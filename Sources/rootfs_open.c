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

status_t rootfs_open (void * ns, void * node, int32_t mode, void ** data) {
  //rootfs_inode_t inode = node;

  //dna_printf ("[rootfs] Inode : name = %s, id = %d", inode -> name, inode -> id);

  *data = NULL;
  return DNA_OK;
}

