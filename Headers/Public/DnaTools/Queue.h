/****h* support/types
 * SUMMARY
 * Support types.
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

#ifndef DNA_SUPPORT_QUEUE_H
#define DNA_SUPPORT_QUEUE_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <Core/Lock.h>
#include <DnaTools/Status.h>

/****t* support/queue_inspector_t
 * SUMMARY
 * Queue inspector type.
 *
 * SOURCE
 */

typedef bool (* queue_inspector_t)(void * item, void * a0, void * a1); 

/*
 ****/

/****t* support/queue_comparator_t
 * SUMMARY
 * Queue comparator type. Represent an ordering < function.
 *
 * SOURCE
 */

typedef bool (* queue_comparator_t)(void * item1, void * item2); 

/*
 ****/

/****t* support/queue_item_t
 * SUMMARY
 * Queue item type.
 *
 * SOURCE
 */

typedef struct _queue_item
{
	struct _queue_item * next;
	void * owner;
}
queue_item_t;

/*
 ****/

/****t* support/queue_t
 * SUMMARY
 * Queue type.
 *
 * SOURCE
 */

typedef struct queue
{
  spinlock_t lock;
	queue_item_t * head;
	queue_item_t * tail;
	volatile uint32_t status;
}
queue_t;

/*
 ****/

extern void queue_item_init (queue_item_t * item, void * owner);

extern status_t queue_add (queue_t * queue, queue_item_t * item);

extern void * queue_rem (queue_t * queue);

extern void queue_extract (queue_t * queue, queue_item_t * item);

extern void * queue_lookup (queue_t * queue,
    queue_inspector_t inspector, void * a0, void * a1);

extern void queue_walk (queue_t * queue,
    queue_inspector_t inspector, void * a0, void * a1);

extern void queue_insert (queue_t * queue,
    queue_comparator_t comparator, queue_item_t * item);

#endif

