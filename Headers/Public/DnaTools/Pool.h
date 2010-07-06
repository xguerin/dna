/****h* Tools/Pool
 * SUMMARY
 * Pool and object management
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

#ifndef DNA_TOOLS_POOL_H
#define DNA_TOOLS_POOL_H

#include <stdint.h>
#include <Core/Lock.h>
#include <DnaTools/Configuration.h>
#include <DnaTools/Queue.h>
#include <DnaTools/Status.h>

/****t* Pool/object_t
 * SUMMARY
 * Object type.
 *
 * SOURCE
 */

typedef struct _object
{
  queue_link_t link;
  spinlock_t lock;
  char name[DNA_NAME_LENGTH];
  int32_t index;
  int32_t usage_counter;
}
object_t;

/*
 ****/

/****t* Pool/pool_t
 * SUMMARY
 * Pool type.
 *
 * SOURCE
 */

typedef struct _pool
{
  spinlock_t lock;
  size_t n_objects;
  object_t object[];
}
* pool_t;

/*
 ****/

extern status_t pool_create (int32_t n_objects, pool_t * p_pool);
extern status_t pool_destroy (pool_t pool);

extern status_t object_create (pool_t pool, char * name,
    size_t size, object_t * p_object);
extern status_t object_destroy (pool_t pool, object_t object);

#endif

