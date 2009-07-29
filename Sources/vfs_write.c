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

/****f* vfs/vfs_write
 * SUMMARY
 * Write data to a file.
 *
 * SYNOPSIS
 */

status_t vfs_write (int16_t file_id, void * data,
    int32_t count, int32_t * p_ret)

/*
 * ARGUMENTS
 * * file_id : the identifier of the file.
 * * buffer : the destination buffer.
 * * count : the number of bytes to write.
 *
 * FUNCTION
 * * Looks-up for the file corresponding to file_id in the current team.
 * * If it exists, then calls the file's write () function.
 *
 * RESULT
 *
 * SOURCE
 */

{
  file_t file = NULL;
  status_t status = DNA_OK;
  int32_t n_data = count;
  fdarray_t fdarray = NULL;
  int32_t current_team = 0;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (data != NULL && p_ret != NULL && count > 0, DNA_ERROR);
    ensure (file_id >= 0 && file_id < DNA_MAX_FILE, DNA_INVALID_FD);

    status = team_find (NULL, & current_team);
    ensure (status == DNA_OK, status);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray_manager . fdarray_list . lock);
    
    fdarray = queue_lookup (& fdarray_manager. fdarray_list,
        fdarray_id_inspector, (void *) & current_team, NULL);

    check (invalid_array, fdarray != NULL, DNA_ERROR);
    
    lock_acquire (& fdarray -> lock);
    lock_release (& fdarray_manager . fdarray_list . lock);

    file = fdarray -> fds[file_id];

    lock_release (& fdarray -> lock);
    cpu_trap_restore(it_status);

    ensure (file != NULL && file != (file_t) -1, DNA_INVALID_FD);

    status = file -> vnode -> volume -> cmd -> write
      (file -> vnode -> volume -> data, file -> vnode -> data, file -> data,
       data, file -> offset, & n_data);

    check (invalid_file, status == DNA_OK, status);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& file -> lock);

    file -> offset += n_data;

    lock_release (& file -> lock);
    cpu_trap_restore(it_status);

    *p_ret = n_data;
    return status;
  }

  rescue (invalid_array)
  {
    lock_release (& fdarray_manager . fdarray_list . lock);
    cpu_trap_restore(it_status);
    leave;
  }

  rescue (invalid_file)
  {
    *p_ret = -1;
    leave;
  }
}

/*
 ****/

