/****h* VirtualFileSystem/FilePrivate
 * SUMMARY
 * VFS File private header.
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

#ifndef DNA_VFS_FILE_PRIVATE_H
#define DNA_VFS_FILE_PRIVATE_H

#include <stdint.h>
#include <stdbool.h>
#include <Private/VNode.h>
#include <DnaTools/DnaTools.h>

/****t* FilePrivate/thread_id_t
 * SUMMARY
 * Thread ID type.
 * TODO find a better way to handle this, so as to prevent the duplication.
 *
 * SOURCE
 */

typedef union _thread_id
{
  int32_t raw;

  struct _thread_id_s
  {
    uint16_t value;
    uint8_t group;
    uint8_t index;
  }
  s;
}
thread_id_t;

/*
 ****/

/****t* FilePrivate/file_t
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

/****t* FilePrivate/file_manager_t
 * SUMMARY
 * File manager type.
 *
 * SOURCE
 */

typedef struct _file_pool
{
  spinlock_t lock;
  file_t ** file;
}
file_pool_t;

/*
 ****/

extern file_pool_t file_pool;

extern status_t file_acquire (int16_t fd, file_t * p_file, int32_t tokens);
extern status_t file_release (int16_t fd, int32_t tokens);

#endif
