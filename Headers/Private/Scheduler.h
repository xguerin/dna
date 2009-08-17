/****h* core/scheduler
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

#include <Private/Thread.h>

#include <Core/Core.h>
#include <DnaTools/DnaTools.h>

/****t* scheduler/cpu_status_t
 * SUMMARY
 * CPU status type.
 *
 * SOURCE
 */

typedef enum _cpu_status
{
  DNA_CPU_READY        = 0xFACE,
  DNA_CPU_RUNNING      = 0xBEEF,
  DNA_CPU_DISABLED    = 0xDEAD
}
cpu_status_t;

/*
 ****/

/****t* scheduler/cpu_t
 * SUMMARY
 * CPU type. Contains various information concerning
 * a CPU execution context.
 *
 * SOURCE
 */

typedef struct _cpu
{
  int32_t id;
  cpu_status_t status;

  spinlock_t lock;

  thread_t current_thread;
  thread_t idle_thread;
  queue_item_t link;
}
cpu_t;

/*
 ****/

/****t* scheduler/scheduler_t
 * SUMMARY
 * Scheduler manager type.
 *
 * SOURCE
 */

typedef struct _scheduler
{
  spinlock_t lock;

  int32_t xt_index;
  queue_t * xt;

  cpu_t * cpu;
  queue_t cpu_pool;

  int32_t thread_index;
  queue_t thread_list;
}
scheduler_t;

/*
 ****/

extern scheduler_t scheduler;

extern status_t scheduler_elect (thread_t * p_thread);
extern status_t scheduler_dispatch (thread_t thread);
extern status_t scheduler_alarm (void * data);

extern status_t scheduler_push_cpu (void);
extern status_t scheduler_pop_cpu (int32_t affinity, int32_t * p_id);

extern status_t scheduler_switch (thread_t thread, queue_t * queue);

#endif

