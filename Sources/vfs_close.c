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

status_t vfs_close (int16_t fd)

/*
 * ARGUMENTS
 * * fd : the file descriptor.
 *
 * FUNCTION
 * Not implemented yet.
 *
 * RESULT
 * * DNA_INVALID_FD if fd is not a valid file
 * * DNA_OK if the operation succeed
 *
 * SOURCE
 */

{
  file_t file = NULL;
  status_t status = DNA_OK;
  fdarray_t fdarray = fdarray_manager . fdarray[0];
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (fd >= 0 && fd < DNA_MAX_FILE, DNA_INVALID_FD);

    /*
     * Get the file associated to the fd.
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray -> lock);

    file = fdarray -> fds[fd];
    check (error, file != NULL && file != (file_t) -1, DNA_INVALID_FD);
    check (error, file -> vnode -> volume -> cmd -> close != NULL, DNA_ERROR);

    lock_acquire (& file -> lock);
    lock_release (& fdarray -> lock);

    file -> usage_counter += 1;

    lock_release (& file -> lock);
    cpu_trap_restore(it_status);

    /*
     * Close the file.
     */

    status = file -> vnode -> volume -> cmd -> close
      (file -> vnode -> volume -> data, file -> vnode -> data, file -> data);

    /*
     * Reset the file's entry in the fd_array
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray -> lock);

    file -> usage_counter -= 1;
    check (error, status == DNA_OK, status);

    if (file -> usage_counter == 0)
    {
      fdarray -> fds[fd] = NULL;
      free_file = true;
    }

    lock_release (& fdarray -> lock);
    cpu_trap_restore(it_status);

    if (free_file)
    {
      kernel_free (file);
    }

    /*
     * Put the corresponding vnode, free the memory
     */

    status = vnode_put (file -> vnode -> volume -> id, file -> vnode -> id);
    ensure (status == DNA_OK, status);

    return DNA_OK;
  }

  rescue (error)
  {
    lock_release (& fdarray -> lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

