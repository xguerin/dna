/****h* vfs/volume
 * SUMMARY
 * Volume management.
 ****
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

#ifndef DNA_VFS_VOLUME_H
#define DNA_VFS_VOLUME_H

#include <stdint.h>
#include <stdbool.h>
#include <Core/Core.h>
#include <DnaTools/DnaTools.h>

/****t* vfs/volume_t
 * SUMMARY
 * Name space type.
 *
 * SOURCE
 */

typedef struct _volume
{
  int32_t id;
  int64_t root_vnid;
  spinlock_t lock;

  struct _volume * host_volume;
  int64_t host_vnid;

  filesystem_cmd_t * cmd;
  void * data;

  queue_item_t link;
}
* volume_t;

/*
 ****/

/****t* volume/volume_manager_t
 * SUMMARY
 * Volume manager type.
 *
 * SOURCE
 */

typedef struct _volume_manager
{
  spinlock_t lock;
  int32_t volume_index;
  queue_t volume_list;
}
volume_manager_t;

/*
 ****/

extern volume_manager_t volume_manager;

extern status_t volume_create (volume_t host_volume, int64_t host_vnid,
    filesystem_cmd_t * cmd, volume_t * p_volume);
extern status_t volume_destroy (volume_t volume);

extern bool volume_id_inspector (void * item, void * p_vid, void * dummy);
extern bool volume_host_inspector (void * item, void * p_vid, void * p_vnid);

#endif

