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

status_t fatfs_walk (void * ns, void * base, char * restrict path,
    char ** new_path, int64_t * p_vnid)
    
{
  fatfs_t fatfs = ns;
  fatfs_inode_t base_inode = base;
  int64_t vnid = -1;

  unsigned char item=0;
  uint16_t recordoffset = 0;
  unsigned char i=0;
  unsigned int sector=0;
  unsigned char *LongFilename;
  char ShortFilename[13];
  struct lfn_cache lfn;
  int dotRequired = 0;
  fatfs_entry_t directoryEntry;
/*  int a;*/

  log (INFO_LEVEL, "FATFS walk [start] :: path %s", path);

  log (INFO_LEVEL, "\tbase_inode id HI 0x%lx LO 0x%lx", (uint32_t)( base_inode -> id >> 32), (uint32_t)( base_inode -> id & 0x00000000FFFFFFFF));
  log (INFO_LEVEL, "\tbase_inode cc_dirid HI 0x%l LO 0x%lx", (uint32_t)base_inode -> cc_dirid);
  
  watch (status_t)
  {
    ensure (ns != NULL && base != NULL, DNA_ERROR);
   
    ensure (vnid != fatfs -> root_vnid ||
        dna_strcmp (path, "..") != 0, DNA_ALREADY_AT_ROOT);
        
	if(base_inode -> cluster_chain_directory == NULL)
	{
		vnid = base_inode -> id;
	}
	else
	{
		for(sector = 0; sector < base_inode -> nb_sector; sector++)
		{
			/* Analyse Sector */
			for (item = 0; item < 16; item++)
			{
				/* Create the multiplier for sector access */
				recordoffset = (32*item);

				/* Overlay directory entry over buffer */
				directoryEntry = (fatfs_entry_t)(base_inode -> cluster_chain_directory + sector*FAT_SECTOR_SIZE + recordoffset);

				/* Long File Name Text Found */
				if (fatfs_entry_lfn_text(directoryEntry) )
					fatfs_lfn_cache_entry(&lfn, base_inode -> cluster_chain_directory + sector*FAT_SECTOR_SIZE + recordoffset);

				/* If Invalid record found delete any long file name information collated */
				else if (fatfs_entry_lfn_invalid(directoryEntry) )
					fatfs_lfn_cache_init(&lfn, false);

				/* Normal SFN Entry and Long text exists  */
				else if (fatfs_entry_lfn_exists(&lfn, directoryEntry) )
				{
					LongFilename = fatfs_lfn_cache_get(&lfn);

					/* Compare names to see if they match */
					if (fatfs_compare_names((char *)LongFilename, (char *)path))
					{
						log (INFO_LEVEL, "\t1. entry : Name %s", directoryEntry -> Name);
						vnid = ((uint64_t)base_inode -> cc_dirid << 32) + sector*FAT_SECTOR_SIZE + recordoffset;
						break;
					}

		 			fatfs_lfn_cache_init(&lfn, false);
				}

				/* Normal Entry, only 8.3 Text */
				else if (fatfs_entry_sfn_only(directoryEntry) )
				{
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

					/* Compare names to see if they match */
					if (fatfs_compare_names(ShortFilename, path))
					{
						log (INFO_LEVEL, "\t2. entry : Name %s", directoryEntry -> Name);
						vnid = ((uint64_t)base_inode -> cc_dirid << 32) + sector*FAT_SECTOR_SIZE + recordoffset;
						break;
					}

					fatfs_lfn_cache_init(&lfn, false);
				}
			} /* End of for */
		}/* End of for */
    } /* End of if */
    
    log (INFO_LEVEL, "\tvnid HI 0x%lx LO 0x%lx", (uint32_t)( vnid >> 32), (uint32_t)( vnid & 0x00000000FFFFFFFF));
    
    *p_vnid = vnid;
    
    log (INFO_LEVEL, "FATFS walk [end]");
    
    return DNA_OK;
  } /* End of watch */

}

