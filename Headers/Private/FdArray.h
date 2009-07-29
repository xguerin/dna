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

#ifndef DNA_VFS_FDARRAY_PRIVATE_H
#define DNA_VFS_FDARRAY_PRIVATE_H

#include <stdint.h>
#include <stdbool.h>
#include <Private/VNode.h>
#include <DnaTools/DnaTools.h>

/****t* ioctx/file_t
 * SUMMARY
 * File type.
 *
 * SOURCE
 */

typedef struct _file
{
  vnode_t vnode;
  spinlock_t lock;
  int32_t mode;
  int64_t offset;
  void * data;
}
* file_t;

/*
 ****/

/****t* ioctx/fdarray_t
 * SUMMARY
 * File descriptor array type.
 *
 * SOURCE
 */

typedef struct _fdarray
{
  queue_item_t link;

  int32_t associated_id;
  spinlock_t lock;
  file_t fds[DNA_MAX_FILE];
}
* fdarray_t;

/*
 ****/

/****t* ioctx/fdarray_t
 * SUMMARY
 * File descriptor array type.
 *
 * SOURCE
 */

typedef struct _fdarray_manager
{
  spinlock_t lock;
  queue_t fdarray_list;
}
fdarray_manager_t;

/*
 ****/

extern fdarray_manager_t fdarray_manager;

extern bool fdarray_id_inspector (void * fdarray, void * p_id, void * p_dummy);

#endif
