/****h* semaphore/operation
 * SUMMARY
 * Semaphore management.
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

#ifndef DNA_CORE_SEMAPHORE_PRIVATE_H
#define DNA_CORE_SEMAPHORE_PRIVATE_H

#include <stdint.h>
#include <stdbool.h>
#include <Private/Team.h>
#include <Core/Core.h>
#include <DnaTools/DnaTools.h>

/****t* semaphore/semaphore_t
 * SUMMARY
 * Semaphore type.
 *
 * SOURCE
 */

typedef struct _semaphore
{
  int32_t id;
  char name[DNA_NAME_LENGTH];
  int32_t tokens;
  spinlock_t lock;
  team_t team;
  thread_t latest_holder;
  queue_t waiting_queue;
} * semaphore_t;

/*
 ****/

/****t* semaphore/sem_pool_t
 * SUMMARY
 * Semaphore manager type.
 *
 * SOURCE
 */

typedef struct _sem_pool
{
  spinlock_t lock;
  semaphore_t * semaphore;
}
sem_pool_t;

/*
 ****/

extern sem_pool_t sem_pool;

#endif

