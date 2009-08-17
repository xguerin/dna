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

/****f* vfs/vfs_read
 * SUMMARY
 * Read data from a file.
 *
 * SYNOPSIS
 */

status_t vfs_read (int16_t fd, void * data, int32_t count, int32_t * p_ret)

/*
 * ARGUMENTS
 * * fd : the identifier of the file.
 * * buffer : the destination buffer.
 * * count : the number of bytes to read.
 * * p_ret : a pointer to the return value
 *
 * FUNCTION
 * * Looks-up for the file corresponding to fd
 * * If it exists, then calls the file's read () function.
 *
 * RESULT
 * Positive number if the function succeeded, -1 otherwise.
 *
 * SOURCE
 */

{
  file_t file = NULL;
  fdarray_t fdarray = & fdarray_manager . fdarray[0];
  int32_t n_data = count;
  interrupt_status_t it_status = 0;
  status_t status = DNA_OK;

  watch (status_t)
  {
    ensure (data != NULL && p_ret != NULL && count > 0, DNA_ERROR);
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

    status = file -> vnode -> volume -> cmd -> read (file -> vnode -> volume -> data,
        file -> vnode -> data, file -> data, data, file -> offset, & n_data);

    check (read_error, status == DNA_OK, status);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& file -> lock);

    file -> offset += n_data;

    lock_release (& file -> lock);
    cpu_trap_restore(it_status);

    *p_ret = n_data;
    return status;
  }

  rescue (read_error)
  {
    *p_ret = -1;
    leave;
  }
}

/*
 ****/

