/****h* core_private/scheduler_private
 * SUMMARY
 * Scheduler management.
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

#ifndef DNA_CORE_SCHEDULER_PRIVATE_H
#define DNA_CORE_SCHEDULER_PRIVATE_H

#include <stdbool.h>
#include <Private/Thread.h>

#include <Core/Core.h>
#include <DnaTools/DnaTools.h>

/****t* scheduler_private/scheduler_t
 * SUMMARY
 * Scheduler manager type.
 *
 * SOURCE
 */

typedef struct _scheduler
{
  spinlock_t lock;
  queue_t * queue;
}
scheduler_t;

/*
 ****/

extern scheduler_t scheduler;

extern status_t scheduler_elect (thread_t * p_thread, bool with_idle);
extern status_t scheduler_dispatch (thread_t thread);
extern status_t scheduler_switch (thread_t thread, queue_t * queue);

#endif

