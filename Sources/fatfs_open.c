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

#include <stdlib.h>
#include <string.h>

#include <Private/FATFileSystem.h>
#include <DnaTools/DnaTools.h>

#include <Private/fatlib_access.h>

status_t fatfs_open (void * ns, void * node, int32_t mode, void ** data)
{
	fatfs_t fatfs = ns;
    fatfs_inode_t inode = node;
	uint32_t entry_offset;
	uint32_t dirid;
	uint32_t sector_offset;
	fatfs_entry_t entry;
	unsigned char buffer[FAT_SECTOR_SIZE];
	
	log (INFO_LEVEL, "FATFS open [start]");
	
	watch(status_t)
	{
		ensure (ns != NULL && node != NULL, DNA_ERROR);
		ensure (inode -> cluster_chain_directory == NULL, DNA_ERROR);
		
		entry = (fatfs_entry_t)malloc(sizeof(struct fatfs_entry));
		ensure (entry != NULL, DNA_OUT_OF_MEM);
		
		entry_offset = (uint32_t)(inode -> id && 0x00000000FFFFFFFF);
		dirid = (uint32_t)(inode -> id >> 32);
	
		sector_offset = entry_offset*32 / FAT_SECTOR_SIZE;
			
		if(fatfs_sector_reader(fatfs, dirid , sector_offset, buffer))
			memcpy(entry, buffer + (entry_offset % 16) * 32, sizeof(struct fatfs_entry));
		else
			return DNA_ERROR;

		*data = entry;
	}
	
	log (INFO_LEVEL, "FATFS open [end]");

	return DNA_OK;
}

