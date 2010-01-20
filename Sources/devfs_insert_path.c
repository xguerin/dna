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
#include <DnaTools/DnaTools.h>
#include <MemoryManager/MemoryManager.h>

status_t devfs_insert_path (devfs_t fs, devfs_inode_t inode,
    char * path, device_cmd_t * commands)
{
  devfs_entry_t entry = NULL;
  devfs_inode_t next_inode = NULL;
  char token[DNA_FILENAME_LENGTH];
  status_t status = DNA_OK;

  watch (status_t)
  {
    ensure (path != NULL, DNA_BAD_ARGUMENT);

    path_get_next_entry (& path, token);

    log (VERBOSE_LEVEL, "token = %s, path = %s, inode = %s",
        token, path, inode -> name);

    entry = queue_lookup (& inode -> entry_list,
        devfs_entry_name_inspector, (void *) token, NULL);

    if (entry == NULL)
    {
      next_inode = kernel_malloc (sizeof (struct devfs_inode), true);
      ensure (next_inode != NULL, DNA_OUT_OF_MEM);

      /*
       * Create the new inode.
       */

      next_inode -> id = fs -> inode_index ++;
      dna_strcpy (next_inode -> name, token);
      next_inode -> dev_cmd = commands;

      /*
       * Create an entry of the corresponding file
       */

      entry = kernel_malloc (sizeof (struct devfs_entry), true);
      check (entry_malloc, entry != NULL, DNA_OUT_OF_MEM);

      entry -> id = next_inode -> id;
      dna_strcpy (entry -> name, token);

      /*
       * Check if there is more to parse.
       */

      if (dna_strlen (path) != 0)
      {
        log (VERBOSE_LEVEL, "parsing %s", token);
        next_inode -> class = DNA_DEVFS_DIRECTORY;

        status = devfs_insert_path (fs, next_inode, path, commands);
        check (general_error, status == DNA_OK, status);
      }
      else
      {
        next_inode -> class = DNA_DEVFS_FILE;
      }

      /*
       * Add the inode to the fs and the entry to the node.
       */

      queue_add (& fs -> inode_list, next_inode);
      queue_add (& inode -> entry_list, entry);

      log (VERBOSE_LEVEL, "file %s created", token);
    }
    else
    {
      ensure (dna_strlen (path) != 0, DNA_INODE_EXISTS);

      /*
       * Load the corresponding inode. TODO: Check if we can use directly
       * the devfs_read_vnode hook since we are in a pseudo file system.
       */

      log (VERBOSE_LEVEL, "read vnode");

      devfs_read_vnode ((void *)fs, entry -> id, (void **) & next_inode);
      ensure (next_inode != NULL, DNA_NO_VNODE);
      ensure (next_inode -> class == DNA_DEVFS_DIRECTORY, DNA_BAD_INODE_TYPE);

      /*
       * Parse the rest of the path.
       */

      log (VERBOSE_LEVEL, "parsing %s", token);
      status = devfs_insert_path (fs, next_inode, path, commands);
    }

    return status;
  }

  rescue (general_error)
  {
    queue_extract (& next_inode -> entry_list, entry);
    kernel_free (entry);
  }

  rescue (entry_malloc)
  {
    queue_extract (& fs -> inode_list, next_inode);
    kernel_free (next_inode);
    leave;
  }
}

