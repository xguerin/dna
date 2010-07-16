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

status_t fatfs_get_info (void * ns, void * node,
    void * data, file_info_t * p_info)
{
  fatfs_t fatfs = ns;
  fatfs_inode_t inode = node;

  watch (status_t)
  {
    ensure (fatfs != NULL, DNA_BAD_ARGUMENT);
    ensure (inode != NULL, DNA_BAD_ARGUMENT);
    ensure (p_info != NULL, DNA_BAD_ARGUMENT);

    dna_memset (p_info, 0, sizeof (file_info_t));

    p_info -> volume = fatfs -> vid;
    p_info -> vnode = inode -> id;
    p_info -> type = (inode -> id == fatfs -> root_vnid 
    	|| inode -> cluster_chain_directory == NULL) ? 
    	DNA_FILE_DIRECTORY : DNA_FILE_REGULAR;

	if(inode -> id != fatfs -> root_vnid)
	{
		p_info -> last_access = 0; /* FIXME */
		p_info -> last_change_time = 0; /* FIXME */
		p_info -> last_modification_time = p_info -> last_change_time;

		p_info -> byte_size = inode -> entry . FileSize;
		p_info -> block_count = inode -> entry . FileSize / (fatfs->sectors_per_cluster * FAT_SECTOR_SIZE);
		p_info -> optimal_io_block_size = fatfs->sectors_per_cluster;
	}

    return DNA_OK;
  }

  return DNA_OK;
}

