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
#include <MemoryManager/MemoryManager.h>
#include <DnaLibrary/DnaLibrary.h>
#include <Processor/Processor.h>

/****f* Core/core_create
 * SUMMARY
 * Create the core component.
 *
 * SYNOPSIS
 */

status_t core_create (void)

/*
 * FUNCTION
 * * Initialize the team manager
 * * Initialize the scheduler manager
 * * Initialize the it multiplexer
 * * Initialize the time manager
 * * Initialize the semaphore pool
 * * Initialize the system team and the IDLE threads
 * * Create the system team.
 *
 * SOURCE
 */

{
  team_t team = NULL;
  thread_t thread = NULL;
  void * stack_base = NULL;
  char id_buffer[16];

  watch (status_t)
  {
    /*
     * Initialize the team manager
     */

    dna_memset (& team_manager, 0, sizeof (team_manager_t));

    /*
     * Initialize the scheduler manager
     */

    dna_memset (& scheduler, 0, sizeof (scheduler_t));

    scheduler . xt_index = cpu_mp_count;
    scheduler . first_available = 0;
    scheduler . last_available = cpu_mp_count - 1;

    scheduler . cpu = kernel_malloc (sizeof (cpu_t) * DNA_MAX_CPU, true);
    ensure (scheduler . cpu != NULL, DNA_OUT_OF_MEM);

    scheduler . xt = kernel_malloc (sizeof (queue_t) * (DNA_MAX_CPU + 1), true);
    check (sched_xt_alloc, scheduler . xt != NULL, DNA_OUT_OF_MEM);

    /*
     * Initialize the IT multiplexer
     */

    dna_memset (& it_manager, 0, sizeof (it_manager_t));

    /*
     * Initialize the time manager
     */

    dna_memset (& time_manager, 0, sizeof (time_manager_t));

    /*
     * Initialize the semaphore pool
     */

    dna_memset (& sem_pool, 0, sizeof (sem_pool_t));
    sem_pool . semaphore = kernel_malloc
      (sizeof (semaphore_t) * DNA_MAX_SEM, true);
    check (sem_pool_alloc, sem_pool . semaphore != NULL, DNA_OUT_OF_MEM);

    /*
     * Allocate the system team structure.
     */

    team = kernel_malloc (sizeof (struct _team), true);
    check (team_alloc, team != NULL, DNA_OUT_OF_MEM);

    /*
     * Fill-in the necessary fields.
     */

    team -> id = atomic_add (& team_manager . thread_index, 1);
    dna_strcpy (team -> name, "KernelTeam");

    queue_item_init (& team -> sched_link, team);

    /*
     * Add the new team to the scheduler.
     */

    queue_add (& team_manager . team_list, & team -> sched_link);

    for (int32_t cpu_i = 0; cpu_i < cpu_mp_count; cpu_i++)
    {
      /*
       * Allocate the idle thread structures.
       */

      thread = kernel_malloc (sizeof (struct _thread), true);
      check (thread_alloc, thread != NULL, DNA_OUT_OF_MEM);

      /*
       * Allocate its stack.
       */

      stack_base = kernel_malloc (0x400, true);
      check (thread_alloc, stack_base != NULL, DNA_OUT_OF_MEM);

      /*
       * Fill-in the necessary fields.
       */

      dna_itoa (cpu_i, id_buffer);
      dna_strcpy (thread -> name, "ThreadIdle");
      dna_strcat (thread -> name, id_buffer);

      thread -> id = atomic_add (& team_manager . team_index, 1);
      thread -> type = DNA_IDLE_THREAD;
      thread -> cpu_id = cpu_i;
      thread -> cpu_affinity = cpu_i;
      thread -> team = team;

      thread -> signature . stack_base = stack_base;
      thread -> signature . stack_size = 0x400;
      thread -> signature . handler = thread_idle;
      thread -> signature . arguments = NULL;

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
          thread -> signature . stack_size, thread -> signature . handler,
          thread -> signature . arguments);

      /*
       * Deal with the new thread
       */

      scheduler . cpu[cpu_i] . status = DNA_CPU_DISABLED;

      scheduler . cpu[cpu_i] . current_team = team;
      scheduler . cpu[cpu_i] . idle_thread = thread;
      scheduler . cpu[cpu_i] . current_thread = thread;

      if (cpu_i != (cpu_mp_count - 1))
      {
        scheduler . cpu[cpu_i] . next_available = cpu_i + 1;
      }
      else
      {
        scheduler . cpu[cpu_i] . next_available = -1;
      }
    }

    return DNA_OK;
  }

  rescue (thread_alloc)
  {
    thread_t idle_thread = NULL;

    /*
     * We free the team
     */

    kernel_free (team);

    /*
     * We free each allocated thread and its stack
     */

    for (int32_t cpu_i = 0; cpu_i < cpu_mp_count; cpu_i++)
    {
      idle_thread = scheduler . cpu[cpu_i] . idle_thread;
      if (idle_thread != NULL)
      {
        kernel_free (idle_thread -> signature . stack_base);
        kernel_free (idle_thread);
      }
    }

    /*
     * We check if a temporary thread has been created and, if so,
     * we free it
     */

    if (thread != NULL) kernel_free (thread);
  }
  
  rescue (team_alloc)
  {
    kernel_free (sem_pool . semaphore);
  }

  rescue (sem_pool_alloc)
  {
    kernel_free (scheduler . xt);
  }

  rescue (sched_xt_alloc)
  {
    kernel_free (scheduler . cpu);
    leave;
  }
}

/*
 ****/

