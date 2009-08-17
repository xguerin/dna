/****h* core/thread
 * SUMMARY
 * Thread management.
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

#ifndef DNA_CORE_THREAD_PRIVATE_H
#define DNA_CORE_THREAD_PRIVATE_H

#include <stdint.h>
#include <stdarg.h>

#include <Private/Time.h>
#include <Private/Team.h>
#include <Core/Core.h>

#include <DnaTools/DnaTools.h>
#include <Processor/Processor.h>

/****v* thread/thread_status
 * SUMMARY
 * Available thread status.
 *
 * SOURCE
 */

typedef enum _thread_status
{
  DNA_THREAD_SLEEP    = 0xBAFF,
  DNA_THREAD_READY    = 0xFACE,
  DNA_THREAD_RUNNING  = 0xBEEF,
  DNA_THREAD_WAIT      = 0xBADD,
  DNA_THREAD_ZOMBIE    = 0xDEAD
}
thread_status_t;

/*
 ****/

/****t* thread/thread_signature_t
 * SUMMARY
 * Thread signature.
 */

typedef struct _thread_signature
{
  thread_handler_t handler;
  void * arguments;

  void * stack_base;
  int32_t stack_size;

  int32_t return_value;
}
thread_signature_t;

/*
 ****/

/****t* types/thread_t
 * SUMMARY
 * Thread type.
 *
 * SOURCE
 */

typedef struct _thread
{
  int32_t id;
  char name[DNA_NAME_LENGTH];

  thread_status_t status;
  int32_t stopwatch;
  int32_t priority;
  int32_t cpu_id;
  int32_t cpu_affinity;

  team_t team;
  spinlock_t lock;
  int32_t sem_tokens;

  queue_item_t status_link;
  queue_item_t team_link;
  queue_item_t sched_link;
  queue_t wait;

  struct _kernel_time
   {
    bigtime_t start;
    bigtime_t elapsed;
  }
   kernel_time;

  struct _user_time
   {
    bigtime_t start;
    bigtime_t elapsed;
  }
   user_time;

  thread_signature_t signature;
  cpu_context_t ctx;
}
* thread_t;

/*
 ****/

extern thread_handler_t APP_ENTRY_POINT;

extern status_t thread_destroy (thread_t thread);

extern bool thread_id_inspector (void * item, void * p_id, void * dummy);
extern bool thread_name_inspector (void * item, void * name, void * dummy);

extern status_t thread_alarm (void * data);

extern int32_t thread_root (void * data);
extern int32_t thread_idle (void * data);
extern int32_t thread_bootstrap (void * data);

#endif

