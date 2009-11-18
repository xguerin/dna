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

#ifndef DNA_CORE_CPU_PRIVATE_H
#define DNA_CORE_CPU_PRIVATE_H

#include <stdbool.h>

#include <Private/Alarm.h>
#include <Private/Interrupt.h>
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
  DNA_CPU_READY     = 0xFACE,
  DNA_CPU_RUNNING   = 0xBEEF,
  DNA_CPU_DISABLED  = 0xDEAD
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
  struct _cpu * next;

  int32_t id;
  cpu_status_t status;

  spinlock_t lock;
  spinlock_t ipi_lock;

  bigtime_t lap_date;
  queue_t * isr_list;

  thread_t current_thread;
  thread_t idle_thread;

  alarm_t current_alarm;
  queue_t alarm_queue;

  int8_t stack[DNA_IDLE_STACK_SIZE];
}
cpu_t;

/*
 ****/

#endif

