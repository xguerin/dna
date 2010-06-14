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

/****f* FilePrivate/file_acquire
 * SUMMARY
 * Acquire a file from the file pool.
 *
 * SYNOPSIS
 */

status_t file_acquire (int16_t fd, file_t * p_file, int32_t tokens)

/*
 * ARGUMENTS
 * * fd : the file descriptor.
 * * p_file : a pointer to a file_t.
 *
 * FUNCTION
 * Check if the fd entry exist in the current group pool. If it is the case, put
 * the file into *file after incrementing its usage counter.
 *
 * RESULT
 * * DNA_INVALID_FD if fd is not a valid file
 * * DNA_OK if the operation succeed
 *
 * SOURCE
 */

{
  file_t file;
  thread_id_t tid;
  status_t status = DNA_OK;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    status = thread_find (NULL, & tid . raw);

    ensure (status == DNA_OK, status);
    ensure (tid . s . group >= 0, DNA_BAD_ARGUMENT);
    ensure (tid . s . group < DNA_MAX_GROUP, DNA_BAD_ARGUMENT);

    ensure (tokens > 0, DNA_BAD_ARGUMENT);
    ensure (p_file != NULL, DNA_BAD_ARGUMENT);

    /*
     * Look for the file in the pool.
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& file_pool . lock);

    file = file_pool . file[tid . s . group][fd];
    check (error, file != NULL && file != (file_t) -1, DNA_INVALID_FD);

    lock_acquire (& file -> lock);
    lock_release (& file_pool . lock);

    file -> usage_counter += tokens;

    /*
     * Check if we need to keep the file locked.
     */

    lock_release (& file -> lock);
    cpu_trap_restore(it_status);

    *p_file = file;
    return DNA_OK;
  }

  rescue (error)
  {
    lock_release (& file_pool . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

