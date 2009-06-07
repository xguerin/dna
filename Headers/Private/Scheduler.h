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

#include <Private/Team.h>
#include <Private/Thread.h>

#include <Core/Core.h>
#include <DnaLibrary/DnaLibrary.h>

/****t* scheduler/cpu_t
 * SUMMARY
 * CPU type. Contains various information concerning a CPU execution context.
 *
 * SOURCE
 */

typedef struct _cpu
{
	team_t current_team;
	thread_t current_thread;
	thread_t idle_thread;
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
}
scheduler_t;

/*
 ****/

extern scheduler_t scheduler;

extern thread_t scheduler_elect (void);
extern void scheduler_switch (thread_t thread, queue_t * queue);
extern void scheduler_restore (void);

#endif

