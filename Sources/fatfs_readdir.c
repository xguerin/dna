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

#include <string.h>
#include <Private/FATFileSystem.h>
#include <DnaTools/DnaTools.h>
#include <Private/fatlib_misc.h>
#include <Private/fatlib_string.h>

status_t fatfs_readdir (void * ns, void * node, void * data,
    void * entry_array, int64_t * offset, int32_t * p_count)
{
	fatfs_t fatfs = ns;
	fatfs_inode_t inode = node;
	directory_entry_t * p_entry = entry_array;

	unsigned char item=0;
	uint16_t recordoffset = 0;
	unsigned char i=0;
	unsigned int sector=0;
	unsigned char *LongFilename;
	char ShortFilename[13];
	struct lfn_cache lfn;
	int dotRequired = 0;
	fatfs_entry_t directoryEntry;
	int item_start;
	
	
	log (VERBOSE_LEVEL, "readdir [start]");
	
	watch (status_t)
	{
		ensure (ns != NULL && node != NULL && entry_array != NULL
			&& p_count != NULL, DNA_ERROR);
		ensure (inode->cluster_chain_directory != NULL, DNA_OK);
   	
	   	/* Initialise LFN cache first */
		fatfs_lfn_cache_init(&lfn, false);
		
		item_start = (*offset)%FAT_SECTOR_SIZE;

		for(sector = (*offset)/FAT_SECTOR_SIZE; sector < inode -> nb_sector; sector++)
		{
			/* Maximum of 16 directory entries */
			for (item = item_start; item < 16; item++)
			{
				/* Increase directory offset  */
				recordoffset = (32*item);

				/* Overlay directory entry over buffer */
				directoryEntry = (fatfs_entry_t)(inode -> cluster_chain_directory 
					+ sector*FAT_SECTOR_SIZE + recordoffset);

				/* Long File Name Text Found */
				if ( fatfs_entry_lfn_text(directoryEntry) )
					fatfs_lfn_cache_entry(&lfn, inode -> cluster_chain_directory 
						+ sector*FAT_SECTOR_SIZE + recordoffset);

				/* If Invalid record found delete any long file name information collated */
				else if ( fatfs_entry_lfn_invalid(directoryEntry) )
					fatfs_lfn_cache_init(&lfn, false);

				/* Normal SFN Entry and Long text exists  */
				else if (fatfs_entry_lfn_exists(&lfn, directoryEntry) )
				{
					/* Get text */
					LongFilename = fatfs_lfn_cache_get(&lfn);

					/* return data */
					strncpy (p_entry -> d_name, (char *)LongFilename, FATFS_NAME_LENGTH); /* FIXME */
				
				    p_entry -> vnid = ((uint64_t)inode -> cc_dirid << 32) + 
							sector*FAT_SECTOR_SIZE + recordoffset; /* FIXME : à vérifier */
				    p_entry -> vid = fatfs -> vid;

				    p_entry -> d_reclen = sizeof (struct _directory_entry)
		    		  + strlen (p_entry -> d_name);
		    		  
					/* Next starting position */
					*offset = sector*FAT_SECTOR_SIZE + item + 1;
					*p_count = p_entry -> d_reclen;

		 			return DNA_OK;
				}
				/* Normal Entry, only 8.3 Text		  */
				else if ( fatfs_entry_sfn_only(directoryEntry) )
				{
	   				fatfs_lfn_cache_init(&lfn, false);

					memset(ShortFilename, 0, sizeof(ShortFilename));

					/* Copy name to string */
					for (i=0; i<8; i++)
						ShortFilename[i] = directoryEntry->Name[i];

					/* Extension */
					dotRequired = 0;
					for (i=8; i<11; i++)
					{
						ShortFilename[i+1] = directoryEntry->Name[i];
						if (directoryEntry->Name[i] != ' ')
							dotRequired = 1;
					}

					/* Dot only required if extension present */
					if (dotRequired)
					{
						/* If not . or .. entry */
						if (ShortFilename[0]!='.')
							ShortFilename[8] = '.';
						else
							ShortFilename[8] = ' ';
					}
					else
						ShortFilename[8] = ' ';

					/* return data */
					strncpy (p_entry -> d_name, ShortFilename, FATFS_NAME_LENGTH);
			
				    p_entry -> vnid = ((uint64_t)inode -> cc_dirid << 32) + 
							sector*FAT_SECTOR_SIZE + recordoffset;
				    p_entry -> vid = fatfs -> vid;

				    p_entry -> d_reclen = sizeof (struct _directory_entry)
		    		  + strlen (p_entry -> d_name);

					/* Next starting position */
					*offset = sector*FAT_SECTOR_SIZE + item + 1;
					*p_count = p_entry -> d_reclen;

					return DNA_OK;
				}
			}/* end of for */
			
			item_start = 0;
		}
		
		/* empty directory */
		*p_count = 0;
	}

	log (VERBOSE_LEVEL, "readdir [end]");

    return DNA_OK;
}


