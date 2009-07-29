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
#include <Processor/Processor.h>

/****f* vnode/vnode_walk
 * SUMMARY
 * Retrieve a reference to a vnode from a path.
 *
 * SYNOPSIS
 */

status_t vnode_walk (char * restrict path, volume_t * p_volume,
    int64_t * p_vnid, void ** p_data)

/*
 * ARGUMENTS
 * * path : the path.
 * * p_vnode : a pointer to that will contain the corresponding vnode.
 *
 * RESULT
 * * DNA_OK if the operation succeeded.
 * * DNA_ERROR if the operation failed.
 * * DNA_NO_VOLUME if no root volume is present.
 *
 * SOURCE
 */

{
  int64_t vnid = -1, new_vnid = -1;
  void * data = NULL;
  volume_t volume = NULL, new_volume = NULL;
  char token[DNA_FILENAME_LENGTH], buffer[DNA_PATH_LENGTH];
  char * path_ptr = buffer;
  interrupt_status_t it_status = 0;
  status_t status = DNA_OK;

  watch (status_t)
  {
    dna_strcpy (buffer, path);

    /*
     * First we need to load the root volume
     */
    
    status = path_get_next_entry (& path_ptr, token);
    ensure (status == DNA_OK, status);
    ensure (dna_strlen (token) == 0, DNA_ERROR);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& volume_manager . volume_list . lock);

    new_volume = queue_lookup (& volume_manager . volume_list,
        volume_host_inspector, (void *) volume, (void *) & vnid);

    lock_release (& volume_manager . volume_list . lock);
    cpu_trap_restore (it_status);

    ensure (new_volume != NULL, DNA_NO_VOLUME);

    volume = new_volume;
    vnid = new_volume -> root_vnid;

    status = vnode_get (volume -> id, vnid, & data);
    ensure (status == DNA_OK, status);

    /*
     * We now walk the remaining path
     */

    while ((status = path_get_next_entry (& path_ptr, token)) == DNA_OK)
    {
      status = volume -> cmd -> walk
        (volume -> data, data, token, NULL, & new_vnid);
      ensure (status == DNA_ALREADY_AT_ROOT || status == DNA_OK, DNA_ERROR);

      /*
       * If the walk is stuck to the volume's root directory,
       * we try to move one stage upward.
       */

      if (status == DNA_ALREADY_AT_ROOT && volume -> host_volume != NULL)
      {
        status = vnode_put (volume -> id, new_vnid);
        if (status != DNA_OK) return status;

        volume = volume -> host_volume;
        new_vnid = volume -> host_vnid;

        status = vnode_get (volume -> id, new_vnid, & data);
        if (status != DNA_OK) return status;

        status = volume -> cmd -> walk
          (volume -> data, data, token, NULL, & new_vnid);
        if (status != DNA_OK) return status;
      }

      /*
       * Do we need to update the node ? If yes, put back
       * the old one, look for a vname and load the new one.
       */

      if (vnid != new_vnid)
      {
        status = vnode_put (volume -> id, vnid);
        ensure (status == DNA_OK, status);

        vnid = new_vnid;

        it_status = cpu_trap_mask_and_backup();
        lock_acquire (& volume_manager . volume_list . lock);

        new_volume = queue_lookup (& volume_manager . volume_list,
            volume_host_inspector, (void *) volume, (void *) & vnid);

        lock_release (& volume_manager . volume_list . lock);
        cpu_trap_restore (it_status);

        if (new_volume != NULL)
        {
          volume = new_volume;
          vnid = new_volume -> root_vnid;
        }

        status = vnode_get (volume -> id, vnid, & data);
        ensure (status == DNA_OK, status);
      }
    }

    *p_volume = volume;
    *p_vnid = vnid;
    *p_data = data;

    return DNA_OK;
  }
}

/*
 ****/

