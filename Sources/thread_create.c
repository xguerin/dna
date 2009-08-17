/*
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

#include <Private/Core.h>
#include <DnaTools/DnaTools.h>
#include <MemoryManager/MemoryManager.h>
#include <Processor/Processor.h>

/****f* thread/thread_create
 * SUMMARY
 * create a new thread_t element.
 *
 * SYNOPSIS
 */

status_t thread_create (thread_handler_t handler, void * arguments,
    char * name, int32_t affinity, int32_t stack_size, int32_t * tid)

/*
 * ARGUMENTS
 * * handler : the thread's handler
 * * arguments : the handler's arguments
 * * name : the thread's name
 * * affinity : the thread's processor affinity
 * * stack_size : the thread's stack size
 * * tid : the placeholder of the created thread's ID
 *
 * RESULT
 * * DNA_OUT_OF_MEM if the system ran out of memory
 * * DNA_ERROR if one of the arguments is invalid
 * * DNA_OK if everything went fine
 *
 * SOURCE
 */

{
  int32_t current_cpuid = cpu_mp_id ();
  thread_t thread = NULL;
  void * stack_base = NULL;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (handler != NULL && name != NULL && tid != NULL, DNA_BAD_ARGUMENT);
    ensure (affinity == DNA_NO_AFFINITY || (affinity >= 0
          && affinity < cpu_mp_count), DNA_BAD_ARGUMENT);

    /*
     * Allocate the new thread structure.
     */

    thread = kernel_malloc (sizeof (struct _thread), true);
    ensure (thread != NULL, DNA_OUT_OF_MEM);

    /*
     * Allocate its stack.
     */

    stack_base = kernel_malloc (stack_size, false);
    check (stack_alloc, stack_base != NULL, DNA_OUT_OF_MEM);

    /*
     * Fill-in the necessary fields.
     */

    dna_strcpy (thread -> name, name);
    thread -> id = atomic_add (& scheduler . thread_index, 1);
    thread -> status = DNA_THREAD_SLEEP;
    thread -> cpu_id = -1;

    if (affinity == DNA_NO_AFFINITY)
    {
      thread -> cpu_affinity = scheduler . xt_index;
    }
    else 
    {
      thread -> cpu_affinity = affinity;
    }

    thread -> signature . handler = handler;
    thread -> signature . arguments = arguments;
    thread -> signature . stack_base = stack_base;
    thread -> signature . stack_size = stack_size;

    /*
     * Initialize the queueing elements.
     */

    queue_item_init (& (thread -> status_link), thread);
    queue_item_init (& (thread -> sched_link), thread);

    /*
     * Initialize the context.
     */

    cpu_context_init ((& thread -> ctx), thread -> signature . stack_base,
        thread -> signature . stack_size, thread_bootstrap,
        & thread -> signature);

    /*
     * Register the thread in the global threads list
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& scheduler . lock);

    queue_add (& scheduler . thread_list, & thread -> sched_link);

    lock_release (& scheduler . lock);
    cpu_trap_restore(it_status);

    *tid = thread -> id;
    return DNA_OK;
  }

  rescue (stack_alloc)
  {
    kernel_free (thread);
    leave;
  }
}

/*
 ****/
