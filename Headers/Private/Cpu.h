/****h* core/cpu
 * SUMMARY
 * CPU management.
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

/****t* cpu/cpu_status_t
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

/****t* cpu/cpu_t
 * SUMMARY
 * CPU type. Contains various information concerning
 * a CPU execution context.
 *
 * SOURCE
 */

typedef struct _cpu
{ 
  queue_item_t next;

  int32_t id;
  cpu_status_t status;

  spinlock_t lock;

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

/****t* cpu/cpu_pool_t
 * SUMMARY
 * CPU pool type.
 *
 * SOURCE
 */

typedef struct _cpu_pool
{
  spinlock_t lock;

  cpu_t cpu[DNA_MAX_CPU];
  queue_t cpu_queue;
}
cpu_pool_t;

/*
 ****/

extern cpu_pool_t cpu_pool;

#endif

