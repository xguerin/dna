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

status_t devfs_mount (int32_t vid, const char * dev_path, uint32_t flags,
    void * params, void ** data, int64_t * vnid)
{
  devfs_t devfs = NULL;
  devfs_inode_t root_inode = NULL;
  devfs_entry_t entry = NULL;
  device_cmd_t * commands = NULL;
  status_t status_hw, status_sw;
  char ** devices = NULL, * path = NULL;

  watch (status_t)
  {
    /*
     * Allocate the base structures
     */

    devfs = kernel_malloc (sizeof (struct devfs), true);
    ensure (devfs != NULL, DNA_OUT_OF_MEM);

    root_inode = kernel_malloc (sizeof (struct devfs_inode), true);
    check (root_malloc, root_inode != NULL, DNA_OUT_OF_MEM);

    /*
     * Initialize the devfs structure
     */

    devfs -> inode_index = 1;
    devfs -> vid = vid;
    devfs -> root_vnid = devfs -> inode_index ++;

    /*
     * Initialize the root inode structure
     */

    root_inode -> id = devfs -> root_vnid;
    dna_strcpy (root_inode -> name, "");
    root_inode -> class = DNA_DEVFS_DIRECTORY;
    queue_add (& devfs -> inode_list, root_inode);

    /*
     * Add the "." entry
     */

    entry = kernel_malloc (sizeof (struct devfs_entry), true);
    check (dot_malloc, entry != NULL, DNA_OUT_OF_MEM);

    entry -> id = root_inode -> id;
    dna_strcpy (entry -> name, ".");
    queue_add (& root_inode -> entry_list, entry);

    /*
     * Add the ".." entry
     */

    entry = kernel_malloc (sizeof (struct devfs_entry), true);
    check (dotdot_malloc, entry != NULL, DNA_OUT_OF_MEM);

    entry -> id = root_inode -> id;
    dna_strcpy (entry -> name, "..");
    queue_add (& root_inode -> entry_list, entry);

    /*
     * Load the drivers and register them as files
     */

    path = kernel_malloc (DNA_PATH_LENGTH, false);

    for (int32_t i = 0; i < OS_N_DRIVERS; i ++)
    {
      status_hw = OS_DRIVERS_LIST[i] -> init_hardware ();
      status_sw = OS_DRIVERS_LIST[i] -> init_driver ();

      if (status_hw == DNA_OK && status_sw == DNA_OK)
      {
        devices = (char **) OS_DRIVERS_LIST[i] -> publish_devices ();

        for (int32_t j = 0; devices[j] != NULL; j ++)
        {
          dna_strcpy (path, devices[j]);
          commands = OS_DRIVERS_LIST[i] -> find_device (path);
          devfs_insert_path (devfs, root_inode, path, commands);
        }
      }
    }

    kernel_free (path);

    /*
     * Add the root vnode to the system
     */

    *data = devfs;
    *vnid = root_inode -> id;

    return vnode_create (root_inode -> id, devfs -> vid, (void *) root_inode);
  }

  rescue (dotdot_malloc)
  {
    devfs_entry_t entry = queue_rem (& root_inode -> entry_list);
    kernel_free (entry);
  }

  rescue (dot_malloc)
  {
    kernel_free (root_inode);
  }

  rescue (root_malloc)
  {
    kernel_free (devfs);
    leave;
  }
}

