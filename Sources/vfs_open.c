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

#include <Private/VirtualFileSystem.h>
#include <Core/Core.h>
#include <MemoryManager/MemoryManager.h>
#include <DnaTools/DnaTools.h>

/****f* vfs/vfs_open
 * SUMMARY
 * Open a file.
 *
 * SYNOPSIS
 */

status_t vfs_open (char * restrict path, int32_t mode,
    int32_t perms, int16_t * p_fd)

/*
 * ARGUMENTS
 * * path : path of the file to open.
 * * flags : ignored.
 * * mode : ignored.
 * * p_fd : a pointer to a file descriptor
 *
 * FUNCTION
 *
 * RESULT
 *
 * SOURCE
 */

{
  int16_t fd_index = 0;
  file_t file = NULL;
  int64_t vnid = -1, file_vnid = -1;
  vnode_t vnode = NULL;
  volume_t volume = NULL;
  void * data = NULL;
  void * file_data = NULL;
  char buffer[DNA_PATH_LENGTH], token[DNA_FILENAME_LENGTH];
  fdarray_t fdarray = NULL;
  int32_t current_team = -1;
  interrupt_status_t it_status = 0;
  status_t status = DNA_OK;

  watch (status_t)
  {
    ensure (path != NULL && p_fd != NULL, DNA_ERROR);

    /*
     * Get the current team ID
     */

    status = team_find (NULL, & current_team);
    ensure (status == DNA_OK, status);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray_manager . fdarray_list . lock);
    
    fdarray = queue_lookup (& fdarray_manager. fdarray_list,
        fdarray_id_inspector, (void *) & current_team, NULL);

    check (invalid_array, fdarray != NULL, DNA_ERROR);
    
    lock_acquire (& fdarray -> lock);
    lock_release (& fdarray_manager . fdarray_list . lock);

    /*
     * Look for an available file slot.
     */

    for (fd_index = 0; fd_index < DNA_MAX_FILE; fd_index ++)
    {
      if (fdarray -> fds[fd_index] == NULL) break;
    }

    check (invalid_file, fd_index != DNA_MAX_FILE, DNA_MAX_OPENED_FILES);

    /*
     * TODO: the line that follows tag fd_index as reserved.
     * We need to undo that when the operation fails.
     */

    fdarray -> fds[fd_index] = (file_t) -1;

    lock_release (& fdarray -> lock);
    cpu_trap_restore(it_status);

    /*
     * Open or create the file
     */

    dna_strcpy (buffer, path);

    if ((mode & DNA_CREATE) != 0)
    {
      /*
       * Spilt the path into base + token
       */

      status = path_split (buffer, token);
      check (mode_error, status == DNA_OK, status);

      /*
       * Get the vnode corresponding to the base directory
       */

      status = vnode_walk (buffer, & volume, & vnid, & data);
      check (mode_error, status == DNA_OK, status);

      /*
       * Ask to create the file
       */

      status = volume -> cmd -> create (volume -> data, data, token,
          mode, perms, & file_vnid, & file_data);
      check (mode_error, status == DNA_OK, status);

      status = vnode_put (volume -> id, vnid);
      check (mode_error, status == DNA_OK, status);
    }
    else
    {
      /*
       * Get the vnode corresponding to the base directory
       */

      status = vnode_walk (buffer, & volume, & vnid, & data);
      check (mode_error, status == DNA_OK, status);

      /*
       * Ask to open the file
       */

      status = volume -> cmd -> open (volume -> data, data, mode, & file_data);
      check (mode_error, status == DNA_OK, status);
    }
    
    /*
     * Find the new vnode
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& vnode_manager . lock);

    vnode = queue_lookup (& vnode_manager . vnode_list,
        vnode_id_inspector, (void *) & vnid, (void *) & volume -> id);

    lock_release (& vnode_manager . lock);
    cpu_trap_restore (it_status);

    ensure (vnode != NULL, DNA_NO_VNODE);

    /*
     * Allocate the new file.
     */

    file = kernel_malloc (sizeof (struct _file), true);
    ensure (file != NULL, DNA_OUT_OF_MEM);

    file -> vnode = vnode;
    file -> mode = mode;
    file -> data = file_data;

    fdarray -> fds[fd_index] = file;
    *p_fd = fd_index;

    return DNA_OK;
  }

  rescue (mode_error)
  {
    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray -> lock);

    fdarray -> fds[fd_index] = NULL;

    lock_release (& fdarray -> lock);
    cpu_trap_restore(it_status);

    leave;
  }

  rescue (invalid_file)
  {
    lock_release (& fdarray -> lock);
    cpu_trap_restore (it_status);
    leave;
  }

  rescue (invalid_array)
  {
    lock_release (& fdarray_manager . fdarray_list . lock);
    cpu_trap_restore (it_status);
    leave;
  }
}

/*
 ****/

