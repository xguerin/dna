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
#include <MemoryManager/MemoryManager.h>
#include <DnaTools/DnaTools.h>

status_t fdarray_destroy (int32_t id)
{
  file_t file = NULL;
  fdarray_t fdarray = NULL;
  status_t status;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray_manager . fdarray_list . lock);

    fdarray = queue_lookup (& fdarray_manager. fdarray_list,
        fdarray_id_inspector, (void *) & id, NULL);

    lock_release (& fdarray_manager . fdarray_list . lock);
    cpu_trap_restore(it_status);

    ensure (fdarray != NULL, DNA_ERROR);

    /*
     * Check if there is any file left opened
     */

    for (int32_t i = 0; i < DNA_MAX_FILE; i += 1)
    {
      file = fdarray -> fds[i];

      if (file != NULL)
      {
        /*
         * Close the file
         */

        status = file -> vnode -> volume -> cmd -> close (file
            -> vnode -> volume -> data, file -> vnode -> data, file -> data);
        ensure (status == DNA_OK, status);

        /*
         * Put the corresponding vnode, free the memory
         */

        status = vnode_put (file -> vnode -> volume -> id, file -> vnode -> id);
        ensure (status == DNA_OK, status);

        kernel_free (file);
      }
    }

    kernel_free (fdarray);

    return DNA_OK;
  }
}

/*
 ****
 */

