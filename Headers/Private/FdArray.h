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

/****t* fdarray/file_t
 * SUMMARY
 * File type.
 *
 * SOURCE
 */

typedef struct _file
{
  spinlock_t lock;
  vnode_t vnode;

  int32_t mode;
  int64_t offset;
  void * data;
}
* file_t;

/*
 ****/

/****t* fdarray/fdarray_t
 * SUMMARY
 * File descriptor array type.
 *
 * SOURCE
 */

typedef struct _fdarray
{
  spinlock_t lock;
  int32_t associated_id;
  file_t fds[DNA_MAX_FILE];
  queue_item_t link;
}
* fdarray_t;

/*
 ****/

/****t* fdarray/fdarray_t
 * SUMMARY
 * File descriptor array type.
 *
 * SOURCE
 */

typedef struct _fdarray_manager
{
  spinlock_t lock;
  fdarray_t fdarray[DNA_MAX_FDARRAY];
}
fdarray_manager_t;

/*
 ****/

extern fdarray_manager_t fdarray_manager;

#endif
