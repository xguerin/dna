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

#include <Private/DeviceFileSystem.h>
#include <MemoryManager/MemoryManager.h>
#include <DnaTools/DnaTools.h>

status_t devfs_mount (int32_t vid, const char * dev_path, uint32_t flags, void * params, void ** data, int64_t * vnid) {
	devfs_t devfs = NULL;
	devfs_inode_t inode = NULL, root_inode = NULL;
	devfs_entry_t entry = NULL;
	char ** devices = NULL;

 	devfs = kernel_malloc (sizeof (struct devfs), true);
	if (devfs == NULL) return DNA_OUT_OF_MEM;

 	root_inode = kernel_malloc (sizeof (struct devfs_inode), true);
	if (root_inode == NULL) return DNA_OUT_OF_MEM;

	devfs -> inode_index = 1;
	devfs -> vid = vid;

	root_inode -> id = devfs -> inode_index ++;
	dna_strcpy (root_inode -> name, "");
	root_inode -> class = DNA_DEVFS_DIRECTORY;
	queue_item_init (& root_inode -> link, root_inode);
	queue_add (& devfs -> inode_list, & root_inode -> link);

	devfs -> root_vnid = root_inode -> id;

	/*
	 * Add the "." entry
	 */

	entry = kernel_malloc (sizeof (struct devfs_entry), true);
	if (entry == NULL) return DNA_OUT_OF_MEM;
	entry -> id = root_inode -> id;
	dna_strcpy (entry -> name, ".");
	queue_item_init (& entry -> link, entry);
	queue_add (& root_inode -> entry_list, & entry -> link);

	/*
	 * Add the ".." entry
	 */

	entry = kernel_malloc (sizeof (struct devfs_entry), true);
	if (entry == NULL) return DNA_OUT_OF_MEM;
	entry -> id = root_inode -> id;
	dna_strcpy (entry -> name, "..");
	queue_item_init (& entry -> link, entry);
	queue_add (& root_inode -> entry_list, & entry -> link);

	/*
	 * Load the drivers and register them as files
	 */

	for (int32_t i = 0; i < OS_N_DRIVERS; i ++) {
		OS_DRIVERS_LIST[i] -> init_hardware ();
		OS_DRIVERS_LIST[i] -> init_driver ();
		devices = (char **) OS_DRIVERS_LIST[i] -> publish_devices ();

		for (int32_t j = 0; devices[j] != NULL; j ++) {
		 	inode = kernel_malloc (sizeof (struct devfs_inode), true);
			if (inode == NULL) return DNA_OUT_OF_MEM;

			inode -> id = devfs -> inode_index ++;
			dna_strcpy (inode -> name, devices[j]);
			inode -> class = DNA_DEVFS_FILE;
			inode -> dev_cmd = OS_DRIVERS_LIST[i] -> find_device (devices[j]);
			queue_item_init (& inode -> link, inode);
			queue_add (& devfs -> inode_list, & inode -> link);

			entry = kernel_malloc (sizeof (struct devfs_entry), true);
			if (entry == NULL) return DNA_OUT_OF_MEM;
			entry -> id = inode -> id;
			dna_strcpy (entry -> name, devices[j]);
			queue_item_init (& entry -> link, entry);
			queue_add (& root_inode -> entry_list, & entry -> link);
		}
	}

	/*
	 * Add the root vnode to the system
	 */

	*data = devfs;
	*vnid = root_inode -> id;

	return vnode_create (root_inode -> id, devfs -> vid, (void *) root_inode);
}

