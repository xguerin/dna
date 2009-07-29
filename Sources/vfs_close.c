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

/****f* vfs/vfs_close
 * SUMMARY
 * Closes a file.
 *
 * SYNOPSIS
 */

status_t vfs_close (int16_t file_id)

/*
 * ARGUMENTS
 * * file_id : the file identifier.
 *
 * FUNCTION
 * Not implemented yet.
 *
 * RESULT
 * Always returns DNA_NOT_IMPLEMENTED
 *
 * SOURCE
 */

{
  file_t file = NULL;
  status_t status = DNA_OK;
  fdarray_t fdarray = NULL;
  int32_t current_team = -1;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (file_id >= 0 && file_id < DNA_MAX_FILE, DNA_INVALID_FD);

    /*
     * Get the current fdarray
     */

    team_find (NULL, & current_team);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray_manager . fdarray_list . lock);
    
    fdarray = queue_lookup (& fdarray_manager. fdarray_list,
        fdarray_id_inspector, (void *) & current_team, NULL);
    
    lock_release (& fdarray_manager . fdarray_list . lock);
    cpu_trap_restore(it_status);

    /*
     * Get the file associated to the file_id
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray -> lock);

    file = fdarray -> fds[file_id];

    lock_release (& fdarray -> lock);
    cpu_trap_restore(it_status);

    ensure (file != NULL && file != (file_t) -1, DNA_INVALID_FD);

    /*
     * Close the file
     */

    status = file -> vnode -> volume -> cmd -> close
      (file -> vnode -> volume -> data, file -> vnode -> data, file -> data);

    ensure (status == DNA_OK, status);

    /*
     * Reset the file's entry in the fd_array
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray -> lock);

    fdarray -> fds[file_id] = NULL;

    lock_release (& fdarray -> lock);
    cpu_trap_restore(it_status);

    /*
     * Put the corresponding vnode, free the memory
     */

    status = vnode_put (file -> vnode -> volume -> id, file -> vnode -> id);
    ensure (status == DNA_OK, status);

    kernel_free (file);
    return DNA_OK;
  }
}

/*
 ****/

