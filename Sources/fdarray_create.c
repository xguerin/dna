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
#include <Processor/Processor.h>

status_t fdarray_create (int32_t id, int32_t parent_id)
{
  file_t file = NULL;
  fdarray_t fdarray = NULL, parent_array = NULL;
  status_t status = DNA_OK;
  interrupt_status_t it_status = 0;
  void * dummy = NULL;

  watch (status_t)
  {
    fdarray = kernel_malloc (sizeof (struct _fdarray), true);
    ensure (fdarray != NULL, DNA_OUT_OF_MEM);

    fdarray -> associated_id = id;
    queue_item_init (& fdarray -> link, fdarray);

    if (parent_id != -1)
    {
      it_status = cpu_trap_mask_and_backup();
      lock_acquire (& fdarray_manager . fdarray_list . lock);

      parent_array = queue_lookup (& fdarray_manager. fdarray_list,
          fdarray_id_inspector, (void *) & parent_id, NULL);

      lock_release (& fdarray_manager . fdarray_list . lock);
      cpu_trap_restore(it_status);

      if (parent_array != NULL)
      {
        it_status = cpu_trap_mask_and_backup();
        lock_acquire (& parent_array -> lock);

        for (int32_t i = 0; i < DNA_MAX_FILE; i++)
        {
          file = NULL;
          if (parent_array -> fds[i] != NULL)
          {
            file = kernel_malloc (sizeof (struct _file), true);
            *file = *parent_array -> fds[i];

            status = vnode_get (file -> vnode -> volume -> id,
                file -> vnode -> id, & dummy);
          }
          fdarray -> fds[i] = file;
        }

        lock_release (& parent_array -> lock);
        cpu_trap_restore(it_status);
      }
    }

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& fdarray_manager . fdarray_list . lock);
    
    queue_add (& fdarray_manager . fdarray_list, & fdarray -> link);

    lock_release (& fdarray_manager . fdarray_list . lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }
}

/*
 ****/
