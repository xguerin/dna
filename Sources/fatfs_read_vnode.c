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
#include <Private/fatlib_misc.h>

status_t fatfs_read_vnode (void * ns, int64_t vnid, void ** data)
{
	fatfs_t fatfs = ns;
    fatfs_inode_t inode = NULL;
	uint32_t entry_offset = (uint32_t)(vnid && 0x00000000FFFFFFFF);
	uint32_t dirid = (uint32_t)(vnid >> 32);
	uint32_t sector_offset = 0;
	unsigned char buffer[FAT_SECTOR_SIZE];
	
	log (INFO_LEVEL, "FATFS read_vnode [start]");
	
	watch(status_t)
	{
		ensure (ns != NULL, DNA_ERROR);
		
		inode = malloc (sizeof (struct fatfs_inode));
		ensure (inode != NULL, DNA_OUT_OF_MEM);
		
		inode -> id = vnid;
		inode -> cc_dirid = -1;
		inode -> cluster_chain_directory = NULL;
	
		if (vnid != fatfs -> root_vnid) 
		{
			sector_offset = entry_offset*32 / FAT_SECTOR_SIZE;
			
			check(source_error, fatfs_sector_reader(fatfs, dirid , sector_offset, buffer), DNA_ERROR)
			
			memcpy(&(inode -> entry), buffer + (entry_offset % 16) * 32, sizeof(struct fatfs_entry));
		}
	
		if(vnid == fatfs -> root_vnid || fatfs_entry_is_dir(&(inode -> entry)))
		{
			dirid = (vnid == fatfs -> root_vnid) ? (uint32_t)(vnid >> 32) : ((uint32_t)inode -> entry . FstClusHI << 16) + inode -> entry . FstClusLO;
			inode -> cc_dirid = dirid;
			
			/* FIXME long file name */
			sector_offset = 0;	
			while(fatfs_sector_reader(fatfs, dirid, sector_offset++, NULL));
		
			inode -> nb_sector = sector_offset;
		
			inode -> cluster_chain_directory = malloc (inode -> nb_sector * FAT_SECTOR_SIZE * sizeof(unsigned char));
			check (source_error, inode -> cluster_chain_directory != NULL, DNA_OUT_OF_MEM);
		
			sector_offset = 0;
			while(fatfs_sector_reader(fatfs, dirid, sector_offset, buffer))
			{
		   		memcpy(inode -> cluster_chain_directory + sector_offset, buffer, sizeof (buffer));
		   		sector_offset++;
			}
		}
	
		*data = inode;
		
		log (INFO_LEVEL, "FATFS read_vnode [end]");
		
		return DNA_OK;
	}
	
   rescue (source_error)
   {
     free (inode);
     leave;
   }
}

