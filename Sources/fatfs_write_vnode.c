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

#include <Private/FATFileSystem.h>
#include <DnaTools/DnaTools.h>

/****f* FATFileSystem/fatfs_write_vnode
 * SUMMARY
 * Build and write a new inode on FAT volume.
 *
 * SYNOPSIS
 */

status_t fatfs_write_vnode (void * ns, void * node)

/*  
 * ARGUMENTS
 * * ns : the namespace (fatfs_t)
 * * node : the inode to write (fatfs_inode_t)
 * 	
 * FUNCTION
 * Build and write a new inode on FAT volume.
 * Not implemented.
 *
 * RESULT
 * * DNA_OK
 *
 * SOURCE
 */

{
	return DNA_OK;
}

/*
 ****/


