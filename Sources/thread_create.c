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
    char * name, int32_t group, int32_t affinity,
    void * stack_base, int32_t stack_size, int32_t * tid)

/*
 * ARGUMENTS
 * * handler : the thread's handler
 * * arguments : the handler's arguments
 * * name : the thread's name
 * * group : the thread's group
 * * affinity : the thread's processor affinity
 * * stack_base : the base of the stack
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
  int32_t index = 0;
  thread_t thread = NULL;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (handler != NULL && name != NULL && tid != NULL, DNA_BAD_ARGUMENT);
    ensure (affinity == DNA_NO_AFFINITY || (affinity >= 0
          && affinity < cpu_mp_count ()), DNA_BAD_ARGUMENT);
    ensure (group >= 0 && group < DNA_MAX_GROUP, DNA_BAD_ARGUMENT);

    /*
     * Allocate the new thread structure.
     */

    thread = kernel_malloc (sizeof (struct _thread), true);
    ensure (thread != NULL, DNA_OUT_OF_MEM);

    /*
     * Deal with the thread group. In the future, only the kernel
     * and the application server will be able to create thread for
     * a different group than self.
     */

    thread -> id . s . group = group;

    /*
     * Allocate its stack.
     */

    if (stack_base == NULL)
    {
      thread -> stack . base = kernel_malloc (stack_size, false);
      check (error, thread -> stack . base != NULL, DNA_OUT_OF_MEM);
    }
    else
    {
      thread -> stack . base = stack_base;
    }

    thread -> stack . size = stack_size;

    /*
     * Fill in the information.
     */

    dna_strcpy (thread -> info . name, name);
    thread -> info . cpu_id = 0;

    if (affinity == DNA_NO_AFFINITY)
    {
      thread -> info .affinity = cpu_mp_count ();
    }
    else 
    {
      thread -> info . affinity = affinity;
    }

    thread -> info . status = DNA_THREAD_SUSPENDED;
    thread -> info . resource = DNA_NO_RESOURCE;
    thread -> info . resource_id = -1;

    /*
     * Fill in the signature
     */

    thread -> signature . handler = handler;
    thread -> signature . arguments = arguments;

    /*
     * Initialize the context.
     */

    cpu_context_init (& thread -> context, thread -> stack . base,
        thread -> stack . size, thread_bootstrap, & thread -> signature);

    /*
     * Find a free thread slot
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& thread_pool . lock);

    for (index = 0; index < DNA_MAX_THREAD; index += 1)
    {
      if (thread_pool . thread[group][index] == NULL)
      {
        thread -> id . s . value = thread_pool . counter;
        thread -> id . s . index = index;

        thread_pool . thread[group][index] = thread;
        thread_pool . counter += 1;

        break;
      }
    }

    lock_release (& thread_pool . lock);
    cpu_trap_restore(it_status);

    check (error, index != DNA_MAX_THREAD, DNA_NO_MORE_THREAD);

    /*
     * Return the thread ID and success
     */

    *tid = thread -> id . raw;
    return DNA_OK;
  }

  rescue (error)
  {
    if (stack_base == NULL)
    {
      kernel_free (thread -> stack . base);
    }

    kernel_free (thread);
    leave;
  }
}

/*
 ****/
