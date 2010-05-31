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

#include <Private/fatlib_access.h>

status_t fatfs_read (void * ns, void * node, void * file, void * data, int64_t offset, int32_t * p_count)
{
	fatfs_t fatfs = ns;
	fatfs_entry_t entry = file;

	uint32_t startcluster;
	uint32_t sector;
	uint32_t sector_offset;
	uint32_t copyCount;
	uint32_t count = *p_count;
	uint32_t bytesRead = 0;
  
	struct sector_buffer data_buffer;
	
	log (INFO_LEVEL, "FATFS read [start]");

	watch(status_t)
	{
	
		ensure (ns != NULL && node != NULL && file != NULL, DNA_ERROR)
	
		/* Nothing to be done */
		if (!count)
			return DNA_OK;

		/* Check if read starts past end of file */
		if (offset >= entry -> FileSize)
			return DNA_ERROR;

		/* Limit to file size */
		if ((offset + count) > entry -> FileSize)
			count = entry -> FileSize - offset;

		startcluster = ((uint32_t)(entry -> FstClusHI) << 16) + entry -> FstClusLO;

		/* Calculate start sector */
		sector = offset / FAT_SECTOR_SIZE;

		/* Offset to start copying data from first sector */
		sector_offset = offset % FAT_SECTOR_SIZE;

		data_buffer.address = 0xFFFFFFFF;

		while (bytesRead < count)
		{
			/* Do we need to re-read the sector? */
			if (data_buffer.address != sector)
			{
				/* Read sector of file */
				if (!fatfs_sector_reader(fatfs, startcluster, sector, data_buffer.sector))
					break;

				data_buffer.address = sector;
			}

			/* We have upto one sector to copy */
			copyCount = FAT_SECTOR_SIZE - offset;

			/* Only require some of this sector? */
			if (copyCount > (count - bytesRead))
				copyCount = (count - bytesRead);

			/* Copy to application buffer */
			memcpy((unsigned char*)((unsigned char*)data + bytesRead), (unsigned char*)(data_buffer.sector + sector_offset), copyCount);

			/* Increase total read count */
			bytesRead += copyCount;

			/* Move onto next sector and reset copy offset */
			sector++;
			sector_offset = 0;
		}

	}
		  
	 *p_count = bytesRead;

	  log (INFO_LEVEL, "FATFS read [end]");

	  return DNA_OK;
}

