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

#ifndef DNA_VFS_FILE_PRIVATE_H
#define DNA_VFS_FILE_PRIVATE_H

#include <stdint.h>
#include <stdbool.h>
#include <Private/VNode.h>
#include <DnaTools/DnaTools.h>

/****t* VirtualFileSystem/file_t
 * SUMMARY
 * File type.
 *
 * SOURCE
 */

typedef struct _file
{
  spinlock_t lock;
  int32_t usage_counter;
  vnode_t vnode;

  int32_t mode;
  int64_t offset;
  void * data;
}
* file_t;

/*
 ****/

/****t* VirtualFileSystem/file_manager_t
 * SUMMARY
 * File manager type.
 *
 * SOURCE
 */

typedef struct _file_manager
{
  spinlock_t lock;
  file_t ** file;
}
file_manager;

/*
 ****/

extern file_manager_t file_manager;

#endif
