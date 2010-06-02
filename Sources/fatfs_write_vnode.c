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
#include <Private/FATFileSystem.h>
#include <DnaTools/DnaTools.h>

status_t fatfs_write_vnode (void * ns, void * node)
{
    fatfs_inode_t inode = node;

	log (VERBOSE_LEVEL, "FATFS write_vnode ");

	watch (status_t)
	{
		ensure (ns != NULL && node != NULL, DNA_ERROR);
		
		if (inode -> cluster_chain_directory != NULL)
			free (inode -> cluster_chain_directory);
			
		free(inode);
	}
	
	return DNA_OK;
}

