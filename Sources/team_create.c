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
#include <VirtualFileSystem/VirtualFileSystem.h>
#include <MemoryManager/MemoryManager.h>
#include <DnaLibrary/DnaLibrary.h>

/****f* Core/team_create
 * SUMMARY
 * Allocates a new team_t element.
 *
 * SYNOPSIS
 */

status_t team_create (char * team_name, thread_handler_t thread_handler,
    void * thread_arguments, int32_t * tid)

/*
 * ARGUMENTS
 * * name : team name
 * * handler : the main thread's handler
 * * thread_arguments : the main thread's arguments
 * * tid : the placeholder of the main thread's ID
 *
 * RESULT
 * * DNA_OUT_OF_MEM if no more memory is available.
 * * DNA_OK and a valid team_t in case of success.
 *
 * SOURCE
 */

{
	team_t team = NULL;
  team_t current_team = scheduler . cpu[cpu_mp_id ()] . current_team;
  thread_t thread = NULL;
  status_t status = DNA_OK;
  void * stack_base = NULL;
	interrupt_status_t it_status = 0;

  watch (status_t)
  {
    /*
     * Allocate the new team structure.
     */

    team = kernel_malloc (sizeof (struct _team), true);
    ensure (team != NULL, DNA_OUT_OF_MEM);

    /*
     * Fill-in and initialize the necessary fields.
     */

    team -> id = atomic_add (& team_manager . thread_index, 1);
    dna_strcpy (team -> name, team_name);

    queue_item_init (& team -> sched_link, team);

    /*
     * Add the new team to the scheduler.
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& team_manager . lock);

    queue_add (& team_manager . team_list, & team -> sched_link);

    lock_release (& team_manager . lock);
    cpu_trap_restore(it_status);

    /*
     * Allocate the FD array from the VFS
     */

    status = fdarray_create (team -> id, current_team -> id);
    check (fdarray_alloc, status == DNA_OK, status);

    /*
     * Allocate the main thread structure.
     */

    thread = kernel_malloc (sizeof (struct _thread), true);
    check (thread_alloc, thread != NULL, DNA_OUT_OF_MEM);

    /*
     * Allocate its stack.
     */

    stack_base = kernel_malloc (DNA_THREAD_STACK_SIZE, false);
    check (stack_alloc, stack_base != NULL, DNA_OUT_OF_MEM);

    /*
     * Fill-in the necessary fields.
     */

    dna_strcpy (thread -> name, "MainThread");
    thread -> id = atomic_add (& team_manager . team_index, 1);
    thread -> type = DNA_MAIN_THREAD;
    thread -> status = DNA_THREAD_READY;
    thread -> cpu_id = -1;
    thread -> cpu_affinity = scheduler . xt_index;
    thread -> team = team;

    thread -> signature . stack_base = stack_base;
    thread -> signature . stack_size = DNA_THREAD_STACK_SIZE;
    thread -> signature . handler = thread_handler;
    thread -> signature . arguments = thread_arguments;

    /*
     * Initialize the queueing elements.
     */

    queue_item_init (& (thread -> status_link), thread);
    queue_item_init (& (thread -> team_link), thread);
    queue_item_init (& (thread -> sched_link), thread);

    /*
     * Initialize the context.
     */

    cpu_context_init ((& thread -> ctx), thread -> signature . stack_base,
        thread -> signature . stack_size, thread_wrapper, & thread -> signature);

    /*
     * Register the main thread in the global threads list
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& team_manager . lock);

    queue_add (& team_manager . thread_list, & thread -> sched_link);

    /*
     * Register the main thread in the team's threads list
     */

    lock_acquire (& team -> lock);
    lock_release (& team_manager . lock);

    queue_add (& team -> thread_list, & thread -> team_link);

    lock_acquire (& scheduler . xt[thread -> cpu_affinity] . lock);
    lock_release (& team -> lock);

    /*
     * Register the main thread in the appropriate scheduler's runnable queue
     */

    queue_add (& scheduler . xt[thread -> cpu_affinity], & thread -> status_link);

    lock_release (& scheduler . xt[thread -> cpu_affinity] . lock);
    cpu_trap_restore(it_status);

    *tid = thread -> id;
    return DNA_OK;
  }

  rescue (stack_alloc)
  {
    kernel_free (thread);
  }

  rescue (thread_alloc)
  {
    fdarray_destroy (team -> id);
  }

  rescue (fdarray_alloc)
  {
    kernel_free (team);
    leave;
  }
}

/*
 ****/
