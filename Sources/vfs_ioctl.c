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
#include <DnaTools/DnaTools.h>

/****f* vfs/vfs_ioctl
 * SUMMARY
 * Execute an implementation-dependent operation on a file.
 *
 * SYNOPSIS
 */

status_t vfs_ioctl (int16_t fd, int32_t function,
    va_list arguments, int32_t * p_ret)

/*
 * ARGUMENTS
 * * fd : the file identifier.
 * * function : the operation's number.
 * * arguments : the arguments of the operation.
 *
 * FUNCTION
 * * Looks-up for the file corresponding to fd
 * * If it exists, then calls the file's ioctl() function.
 *
 * RESULT
 * Implementation-dependent.
 *
 * SOURCE
 */

{
  file_t file = NULL;
  fdarray_t fdarray = fdarray_manager . fdarray[0];
  interrupt_status_t it_status = 0;
  status_t status = DNA_OK;

  watch (status_t)
  {
    ensure (fd >= 0 && fd < DNA_MAX_FILE, DNA_INVALID_FD);

    /*
     * Get the file associated to the fd
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray -> lock);

    file = fdarray -> fds[fd];

    lock_release (& fdarray -> lock);
    cpu_trap_restore(it_status);

    ensure (file != NULL && file != (file_t) -1, DNA_INVALID_FD);
    ensure (file -> vnode -> volume -> cmd -> ioctl != NULL, DNA_ERROR);

    status = file -> vnode -> volume -> cmd -> ioctl
      (file -> vnode -> volume -> data, file -> vnode -> data, file -> data,
       function, arguments, p_ret);

    return status;
  }
}

/*
 ****/

