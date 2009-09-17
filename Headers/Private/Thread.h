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
#include <Core/Core.h>

#include <DnaTools/DnaTools.h>
#include <Processor/Processor.h>

/****t* types/thread_stack_t
 * SUMMARY
 * Thread stack type.
 *
 * SOURCE
 */

typedef struct _thread_stack
{
  uint32_t size;
  void * base;
}
thread_stack_t;

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
  queue_item_t link;

  queue_t wait;
  spinlock_t lock;

  thread_info_t info;
  thread_signature_t signature;

  thread_stack_t stack;
  cpu_context_t context;
}
* thread_t;

/*
 ****/

extern thread_handler_t APP_ENTRY_POINT;

extern status_t thread_destroy (thread_t thread);

extern bool thread_id_inspector (void * item, void * p_id, void * dummy);
extern bool thread_name_inspector (void * item, void * name, void * dummy);

extern status_t thread_alarm (void * data);

extern int32_t thread_idle (void * data);
extern int32_t thread_bootstrap (void * data);

#endif

