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
#include <Private/Media.h>
#include <DnaTools/DnaTools.h>

#include <Private/fatlib_access.h>


status_t fatfs_mount (int32_t vid, const char * dev_path, uint32_t flags, 
	void * params, void ** data, int64_t * vnid)
{
	fatfs_t fatfs = NULL;
	fatfs_inode_t root_inode = NULL;
	
	status_t status;

	log (VERBOSE_LEVEL, "[start] FATFS mount(dev_path = %s)", dev_path);

	watch(status_t)
	{
		ensure (dev_path != NULL, DNA_ERROR);

		/* create the fatfs structure */
		fatfs = malloc (sizeof (struct fatfs));
		ensure (fatfs != NULL, DNA_OUT_OF_MEM);
	
		/* open and get the file descriptor of the device */
		status = (status_t)media_open(dev_path, &(fatfs->fs_fd));
		check(source_error, status == 1, DNA_ERROR);
	
		/* attach opening/writing functions */
		fatfs -> disk_io.read_sector = media_read;
		fatfs -> disk_io.write_sector = media_write;

		/* initialize the fatfs structure */
		status = (status_t)fatfs_init(fatfs);
		log (VERBOSE_LEVEL, "\tfatfs_init status %d", status);
		check(source_error, status == FAT_INIT_OK, DNA_ERROR); 

		/* set volume id */		
		fatfs -> vid = vid;
		/* set cluster root vnode id */
		fatfs -> root_vnid = (((uint64_t)fatfs_get_root_cluster(fatfs)) << 32) + 0xFFFFFFFF;
			
		*data = fatfs;
		*vnid = fatfs -> root_vnid;
		
		/* get cluster root vnode */
		status = fatfs_read_vnode(fatfs, fatfs -> root_vnid, (void **)& root_inode);
		check(source_error, status == 0, DNA_ERROR);
	
		log (VERBOSE_LEVEL, "[end] FATFS mount ");

		return vnode_create (fatfs -> root_vnid, fatfs -> vid, (void *) root_inode);
	}
	
	rescue (source_error)
	{
		free (fatfs);
		leave;
	}
}

